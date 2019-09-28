FROM debian:bullseye
MAINTAINER Roberto Focosi, roberto.focosi@msx2cas.com

ARG SDCC_VERSION=3.9.0

ARG SDCC_LIB_PATH=/usr/share/sdcc

ENV SDCC_INCLUDE=${SDCC_LIB_PATH}/include
ENV SDCC_LIB=${SDCC_LIB_PATH}/lib
ENV SDCC_LIB_Z80=${SDCC_LIB}/z80
ENV WORKSPACE_ROOT=/workspace

ARG Z80_LIB=${SDCC_LIB_Z80}/z80.lib

RUN apt-get update && apt-get install -y make sdcc gettext-base

RUN sdar -d $Z80_LIB printf.rel && \
    sdar -d $Z80_LIB sprintf.rel && \
    sdar -d $Z80_LIB vprintf.rel && \
    sdar -d $Z80_LIB putchar.rel && \
    sdar -d $Z80_LIB getchar.rel

ADD bin/hex2bin/hex2bin /usr/local/bin/
ADD bin/hex2bin/mot2bin /usr/local/bin/
ADD bin/build /usr/local/bin/

RUN chmod +x /usr/local/bin/build

RUN mkdir -p $SDCC_LIB/z80
RUN mkdir -p $SDCC_INCLUDE/z80

ADD fusion-c/fusion-c-lib.tar.bz2 $SDCC_LIB/z80/
ADD fusion-c/fusion-c-include.tar.bz2 $SDCC_INCLUDE/z80/

RUN mkdir $WORKSPACE_ROOT

WORKDIR ${WORKSPACE_ROOT}

CMD ["tail", "-f", "/dev/null"]
