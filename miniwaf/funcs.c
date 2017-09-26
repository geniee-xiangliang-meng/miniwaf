void usage() {
    fprintf(stdout,
        "usage:\n\t./miniwaf -e INPUT_NGINX_ERROR_LOG -d OUTPUT_NGINX_DENY_CONF -r RULE_FILE\n\n"
        "for example:\n"
        "\t./miniwaf -e /usr/local/nginx/logs/error.log -d /usr/local/nginx/conf/deny.conf -r /usr/local/miniwaf.conf"
        "\n\n"
    );
}

void close_all() {
    close(ngx_error_log_fd);
    fclose(ngx_deny_fp);
    fclose(rule_file_fp);
    fclose(pos_file_fp);
}

void process_mmap() {
    if (!map) return;

    if (ngx_error_log_processed_position > ngx_error_log_filelen) {
        fprintf(stderr, "reset\n");
        ngx_error_log_processed_position = 0;
    }

    char *ptr_first = map + ngx_error_log_processed_position, *ptr_nl = NULL;
    unsigned long len;
    char *ptr_ip = NULL;
    long ip_addr=0;

    mmap_loop_start:

    if ((ptr_nl = strchr(ptr_first, '\n')) != NULL &&
    (len = ptr_nl - ptr_first) > 0 && len <= NGX_MAX_ERROR_STR) { // >= 0 && <= 100KB
        char line[NGX_MAX_ERROR_STR] = "";
        strncat(line, ptr_first, len);

        if (strstr(line, "[error]") &&
            (ptr_ip = strstr(line, "client: ")) &&
            strstr(line, "access forbidden by rule") == NULL) {

            char ip[16] = "";
            if (sscanf(ptr_ip, "client: %[0-9.]s", ip) && strlen(ip) &&
                (ip_addr = inet_addr(ip)) > 0 && !is_denied_ip(ip_addr) &&
                rule_matched(line)) {

                append_ip_to_denied_conf(ip_addr, ip);
				fprintf(stderr, "append: %s\n", ip);
            }
        }

        // move ptr_first to next log line
        ptr_first += len + 1; // 1 is \n
        ngx_error_log_processed_position += len + 1;

        if (ngx_error_log_processed_position < ngx_error_log_filelen) {
            goto mmap_loop_start;
        }
    }
}
