FROM debian:bullseye
MAINTAINER Roberto Focosi, roberto.focosi@msx2cas.com

ENV WORKSPACE_ROOT=/workspace
ARG SDCC_LIB_MAIN_PATH=/usr/share/sdcc

RUN apt-get update && apt-get install -y make sdcc gettext-base bzip2

ENV SDCC_INCLUDE_MAIN=${SDCC_LIB_MAIN_PATH}/include
ENV SDCC_LIB_MAIN=${SDCC_LIB_MAIN_PATH}/lib
ENV SDCC_LIB_Z80=${SDCC_LIB_MAIN}/z80
ENV SDCC_INCLUDE_Z80=${SDCC_INCLUDE_MAIN}/z80
ENV SDCC_LIB=/extra-lib
ENV SDCC_INCLUDE=/extra-include

ARG Z80_LIB=${SDCC_LIB_Z80}/z80.lib

RUN sdar -d $Z80_LIB printf.rel && \
    sdar -d $Z80_LIB sprintf.rel && \
    sdar -d $Z80_LIB vprintf.rel && \
    sdar -d $Z80_LIB putchar.rel && \
    sdar -d $Z80_LIB getchar.rel


RUN mkdir -p $SDCC_LIB_MAIN/z80 && \
    mkdir -p $SDCC_INCLUDE_MAIN/z80 && \
    mkdir -p $SDCC_LIB && \
    mkdir -p $SDCC_INCLUDE && \
    mkdir -p $WORKSPACE_ROOT

ADD https://github.com/rfocosi/msx-utils/raw/master/fusion-c/fusion-c-include.tar.bz2 /tmp/
RUN tar jxf /tmp/fusion-c-include.tar.bz2 -C $SDCC_INCLUDE_MAIN/z80/

ADD https://github.com/rfocosi/msx-utils/raw/master/fusion-c/fusion-c-lib.tar.bz2 /tmp/
RUN tar jxf /tmp/fusion-c-lib.tar.bz2 -C $SDCC_LIB_MAIN/z80/

ADD https://github.com/rfocosi/msx-utils/raw/master/bin/msx-toolchain-bin.tar.bz2 /tmp/
RUN tar jxf /tmp/msx-toolchain-bin.tar.bz2 -C /usr/local/bin/

ADD https://github.com/rfocosi/msx-utils/raw/master/bin/hex2bin-2.5.tar.bz2 /tmp/
RUN tar -jxf /tmp/hex2bin-2.5.tar.bz2 --strip=1 -C /usr/local/bin/ Hex2bin-2.5/hex2bin Hex2bin-2.5/mot2bin

RUN chmod +x /usr/local/bin/build
RUN chmod +x /usr/local/bin/info

RUN rm -rf /tmp/*

WORKDIR ${WORKSPACE_ROOT}

CMD ["tail", "-f", "/dev/null"]
