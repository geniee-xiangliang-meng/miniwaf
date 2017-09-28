# miniwaf beta 0.1
a mini waf tool can be used for blocking user ip from nginx error_logs  
it can parse your nginx error logs, and export illegal ips, write ips into deny.conf  
  
of cause you can custom your rules for your business.  
sample in conf/miniwaf.conf is blocking a phpmyadmin scanner. `conf/miniwaf`  
  
you can include this deny.conf to nginx, for blocking illegal request.  
  
it is easy to use, and very fast.  

# usage
`./miniwaf -e INPUT_NGINX_ERROR_LOG -d OUTPUT_NGINX_DENY_CONF -r RULE_FILE`

# flow

miniwaf do these things to blocking illegal request's ip

1. scan nginx error_log, by rule config.
2. parse client id, check if it is in blocked ip, if no, append it to deny.conf
3. nginx need include this deny.conf, for example:
```
        location / {
            include deny.conf;
            root   /var/www/site;
            index  index.html index.htm index.php;
        }
```
4. if new block ip is appended, miniwaf will reload nginx by `/usr/local/nginx/sbin/nginx -s reload` to apply it.

# for example:
`./miniwaf -e /usr/local/nginx/logs/error.log -d /usr/local/nginx/conf/deny.conf -r /usr/local/miniwaf.conf`

# sample of error_log
I put a sample file for your test.  
You can use `t/testfile` as a sample of error_logs.  
And, a conf example is there, too. `conf/miniwaf.conf`  

like these:  
`bin/miniwaf -e t/testfile -d /tmp/deny.conf -r conf/miniwaf.conf`
