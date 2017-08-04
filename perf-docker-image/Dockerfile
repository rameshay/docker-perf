FROM 730926784978.dkr.ecr.us-west-2.amazonaws.com/release/ubuntu:16.04
RUN apt-get update && apt-get -y install openssl zlib1g-dev wget curl libldns-dev
COPY files/polygraph-4.9.0 /tmp/polygaph-4.9.0
WORKDIR "/tmp/polygraph-4.9.0"
RUN cd /tmp/polygraph-4.9.0 && ./configure && make && make install
CMD ["/bin/sh"]
