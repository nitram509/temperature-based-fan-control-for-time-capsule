
# Using graphite

https://hub.docker.com/r/hopsoft/graphite-statsd/


### Run docker

````
docker run -d --name graphite --restart=always -p 80:80 -p 2003:2003 -p 8125:8125/udp -p 8126:8126 hopsoft/graphite-statsd
````