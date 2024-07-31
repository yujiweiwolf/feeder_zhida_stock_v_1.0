FROM ubuntu:20.04
MAINTAINER bajizhh <bajizhh@gmail.com>

# --------------------------------------------------
# setup locale
RUN apt-get update && apt-get install -y --force-yes --no-install-recommends locales tzdata openssl
RUN apt-get update -y && apt-get install curl -y
RUN locale-gen en_US.UTF-8
ENV LC_ALL en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US.UTF-8
ENV LD_LIBRARY_PATH .:/usr/local/lib

# setup Timezone
RUN rm -rf /etc/localtime; ln -s /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
RUN echo Asia/Shanghai > /etc/timezone; dpkg-reconfigure -f noninteractive tzdata

# RUN echo "* - nofile 6553600" >> /etc/security/limits.conf
# clean
RUN rm -rf /tmp/*; apt-get clean; rm -rf /var/lib/apt/lists/*

# ==================================================
WORKDIR /opt/zhida_stock_feeder/bin
CMD ["./zhida_stock_feeder"]

# --------------------------------------------------
RUN cd /opt/zhida_stock_feeder; mkdir -p conf log data/tmp
COPY zhida_stock_feeder *.so* /opt/zhida_stock_feeder/bin/