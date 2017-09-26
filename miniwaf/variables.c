#define NGX_MAX_ERROR_STR 102400
#define MAX_DENIED_IPNUM 1000000
#define NGX_RELOAD_CMD "/usr/local/nginx/sbin/nginx -s reload"
#define POS_FILE "/tmp/miniwaf.pos"

char *map = NULL,
     *ngx_error_log = NULL,
     *ngx_deny_conf = NULL,
     *rule_file = NULL;

unsigned long
    pagesize = 0,
    mapsize,
    ngx_error_log_filelen;
int ngx_error_log_processed_position = 0;

long* denied_ips;
int   denied_ips_idx = 0;

int ngx_error_log_fd;
FILE *ngx_deny_fp,
     *rule_file_fp,
     *pos_file_fp;

char **rules;
int rules_idx = 0;

int init() {
    if (!denied_ips) denied_ips = malloc(MAX_DENIED_IPNUM * sizeof(long));

    // open deny conf, if not exist, create it
    if ((ngx_deny_fp = fopen(ngx_deny_conf, "a+")) == NULL) {
        fprintf(stderr, "open deny conf failed\n");
        return 1;
    }
    if ((rule_file_fp = fopen(rule_file, "r")) == NULL) {
        fprintf(stderr, "open rule_file failed\n");
        return 1;
    }
    if ((pos_file_fp = fopen(POS_FILE, "r")) == NULL) {
		if ((pos_file_fp = fopen(POS_FILE, "w")) == NULL) {
            fprintf(stderr, "open POS_FILE failed (use w+ mode)\n");
            return 1;
		} else {
			fprintf(pos_file_fp, "%d", 0);
			fclose(pos_file_fp);
			pos_file_fp = fopen(POS_FILE, "r");
		}
    }

    // try open error_log_file
    if ((ngx_error_log_fd = open(ngx_error_log, O_RDONLY)) < 0) {
        fprintf(stderr, "open error log file failed\n");
        return 1;
    }

    if (!pagesize) pagesize = getpagesize();

    struct stat st;
    stat(ngx_error_log, &st);
    ngx_error_log_filelen = (unsigned long)st.st_size;
    mapsize = (ngx_error_log_filelen / pagesize + 1) * pagesize;

	int tmp_ngx_error_log_processed_position = 0;
	if (fscanf(pos_file_fp, "%d", &tmp_ngx_error_log_processed_position) != EOF) {
		ngx_error_log_processed_position = tmp_ngx_error_log_processed_position;
	}

    return 0;
}
