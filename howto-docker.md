
# Using Graphite

https://hub.docker.com/r/hopsoft/graphite-statsd/


### Run Graphite

````
docker run -d --name graphite --restart=always -p 80:80 -p 2003:2003 -p 8125:8125/udp -p 8126:8126 hopsoft/graphite-statsd
````


# Using Grafana

https://hub.docker.com/r/grafana/grafana/

### Run Grafana

````
docker run -d -p 3000:3000 -e "GF_SERVER_ROOT_URL=http://localhost:3000"  -e "GF_SECURITY_ADMIN_PASSWORD=secret"  grafana/grafana
````

Hint: Add a data source ```http://localhost``` with direct connection type (a.k.a. no proxy). 