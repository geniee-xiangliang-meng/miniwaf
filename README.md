# miniwaf beta 0.1
a mini waf tool can be used for blocking user ip from nginx error_logs  
it can parse your nginx error logs, and export illegal ips, write ips into deny.conf  
  
of cause you can custom your rules for your business.  
sample in conf/miniwaf.conf is blocking a phpmyadmin scanner. `conf/miniwaf`  
  
you can include this deny.conf to nginx, for blocking illegal request.  
  
it is easy to use, and very fast.  

# usage
`./miniwaf -e INPUT_NGINX_ERROR_LOG -d OUTPUT_NGINX_DENY_CONF -r RULE_FILE`

# for example:
`./miniwaf -e /usr/local/nginx/logs/error.log -d /usr/local/nginx/conf/deny.conf -r /usr/local/miniwaf.conf`

# sample of error_log
I put a sample file for your test.  
You can use `t/testfile` as a sample of error_logs.  
And, a conf example is there, too. `conf/miniwaf.conf`  

like these:  
`bin/miniwaf -e t/testfile -d /tmp/deny.conf -r conf/miniwaf.conf`
