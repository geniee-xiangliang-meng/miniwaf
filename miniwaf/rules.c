int preload_rules() {
    rules = calloc(16, sizeof(char*));

    char BUF[10240]="";
    while (fgets(BUF, 10240, rule_file_fp) != NULL) {
        int len = strlen(BUF);
        if (len) {
            char end = BUF[len-1];
            if (end == '\n') BUF[len-1] = '\0';
            rules[rules_idx++] = strdup(BUF);
        }
    }

    return 0;
}

int rule_matched(const char* line) {
    int i;for(i=0;i<rules_idx;i++) {
        if (strstr(line, rules[i]) != NULL) {
            return 1;
        }   
    }   
    return 0;
}
