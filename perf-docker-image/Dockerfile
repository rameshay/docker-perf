FROM alpine:3.5
RUN apk add --update alpine-sdk
RUN apk add py-pip
RUN apk add curl
RUN apk add wget
RUN apk add openssl
RUN apk add ldns-dev
RUN apk add zlib-dev
COPY files/polygraph-4.11.0-src.tar /tmp/polygraph-4.11.0-src.tar
RUN tar xf /tmp/polygraph-4.11.0-src.tar
WORKDIR "/tmp/polygraph-4.11.0"
RUN ./configure --with-static-linking && make install
CMD ["/bin/sh"]
