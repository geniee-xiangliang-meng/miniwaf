#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "miniwaf/variables.c"
#include "miniwaf/deny.c"
#include "miniwaf/rules.c"
#include "miniwaf/funcs.c"

int main(int argc,char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "e:d:r:p:b:")) != -1) {
        switch (opt) {
            case 'e':
                ngx_error_log = strdup(optarg);
                break;
            case 'd':
                ngx_deny_conf = strdup(optarg);
                break;
            case 'r':
                rule_file = strdup(optarg);
                break;
            case 'b':
                ngx_bin_path = strdup(optarg);
                break;
            default:
                usage();
                return 1;
        }
    }
    if (!ngx_error_log || !ngx_deny_conf || !rule_file) {
        usage();
        return 1;
    }

    if (init()) return 1;
    if (preload_rules()) return 1;
    if (preload_deny_conf()) return 1;

    int old_denied_ips_idx = denied_ips_idx;
    if (ngx_error_log_filelen > 0) {
        // do mapping
        if ((map = (char*)mmap(NULL, mapsize, PROT_READ, MAP_SHARED, ngx_error_log_fd, 0)) == MAP_FAILED) {
            fprintf(stderr, "mmap failed\n");
            return 1;
        }

        process_mmap();
        munmap(map, mapsize);
    } else {
        // reset pos file
        ngx_error_log_processed_position = 0;
    }
    close_all();

    if (denied_ips_idx - old_denied_ips_idx > 0) {
        fprintf(stderr, "reload nginx\n");
        if (ngx_bin_path) {
            char ngx_reload_cmd[1024]="";
            sprintf(ngx_reload_cmd, "%s -s reload", ngx_bin_path);
            system(ngx_reload_cmd);
        } else {
            system(NGX_RELOAD_CMD);
        }
    }

    update_pos_file();

    free(ngx_error_log);
    free(ngx_deny_conf);
    free(rule_file);

    return 0;
}
