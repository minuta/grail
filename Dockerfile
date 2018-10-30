FROM debian:stretch
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive \
  apt-get install -y -q --no-install-recommends \
          build-essential \
          python \
          git \
          libnl-3-dev \
          pkg-config
RUN rm -rf /var/lib/apt/lists/*
RUN git clone -c http.sslVerify=false --depth 1 --branch ns-3.29 https://github.com/nsnam/ns-3-dev-git.git /root/ns-3.29
RUN cd $HOME/ns-3.29 && ./waf configure --enable-tests --enable-examples && ./waf
ADD ./ /root/ns-3.29/src/grail/
RUN cd $HOME/ns-3.29 && ./waf configure --enable-tests --enable-examples
RUN cd $HOME/ns-3.29 && ./waf build
CMD cd $HOME/ns-3.29 && bash
