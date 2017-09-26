int preload_deny_conf() {
    char BUF[1024]="";

    rewind(ngx_deny_fp);

    while (fgets(BUF, 1024, ngx_deny_fp) != NULL) {
        char *ip = calloc(15, sizeof(char));
        if (strlen(BUF) <= 32 && sscanf(BUF, "deny %[0-9.]s;", ip)) {
            long ip_addr = inet_addr(ip);
            denied_ips[denied_ips_idx++] = ip_addr;
        }
        free(ip);
    }

    fseek(ngx_deny_fp, 0, SEEK_END);
    return 0;
}

int is_denied_ip(long ip) {
    int i;for (i=0;i<denied_ips_idx;i++) {
        if (denied_ips[i] == ip) return 1;
    }   
    return 0;
}

void append_ip_to_denied_conf(const long ip_addr, const char* ip) {
    fprintf(ngx_deny_fp, "deny %s;\n", ip);
    fflush(ngx_deny_fp);
    denied_ips[denied_ips_idx++] = ip_addr;
}
