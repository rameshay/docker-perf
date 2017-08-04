FROM alpine:3.5
RUN apk add --update alpine-sdk
RUN apk add py-pip
RUN apk add curl
RUN apk add wget
RUN apk add openssl
COPY files/polygraph-4.11.0-src.tgz /tmp
RUN cd /tmp
RUN tar xvzf polygraph-4.11-src.tgz
RUN cd /tmp/polygraph-4.11.0
RUN ./configure --without-ldns --with-static-linking
RUN make install
CMD ["/bin/sh"]
