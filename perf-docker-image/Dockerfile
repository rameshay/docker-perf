FROM alpine:3.5
RUN apk add --update alpine-sdk
RUN apk add py-pip
RUN apk add curl
RUN apk add wget
RUN cd /tmp
RUN wget http://www.web-polygraph.org/downloads/srcs/polygraph-4.11.0-src.tgz
RUN tar xvzf polygraph-4.11-src.tgz
RUN cd polygraph-4.11-src
RUN ./configure --without-dlns
RUN make install
CMD ["/bin/sh"]
