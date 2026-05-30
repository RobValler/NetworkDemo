FROM docker.io/library/debian:stable-slim

WORKDIR /app

COPY server_demo /app/server_demo

RUN chmod +x /app/server_demo

ENV APP_IP=0.0.0.0
ENV TCP_PORT=4001
ENV UDP_PORT=5001

EXPOSE 4001/tcp
EXPOSE 5001/udp

CMD ["/app/server_demo"]
