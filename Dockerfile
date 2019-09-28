FROM debian:bullseye
MAINTAINER Roberto Focosi, roberto.focosi@msx2cas.com

ENV WORKSPACE_ROOT=/workspace
ARG SDCC_LIB_MAIN_PATH=/usr/share/sdcc

RUN apt-get update && apt-get install -y make sdcc gettext-base

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

ADD bin/msx-toolchain-bin.tar.bz2 /usr/local/bin/
ADD bin/build /usr/local/bin/

RUN chmod +x /usr/local/bin/build

RUN mkdir -p $SDCC_LIB_MAIN/z80 && \
    mkdir -p $SDCC_INCLUDE_MAIN/z80 && \
    mkdir -p $SDCC_LIB && \
    mkdir -p $SDCC_INCLUDE && \
    mkdir -p $WORKSPACE_ROOT

ADD fusion-c/fusion-c-lib.tar.bz2 $SDCC_LIB_MAIN/z80/
ADD fusion-c/fusion-c-include.tar.bz2 $SDCC_INCLUDE_MAIN/z80/

WORKDIR ${WORKSPACE_ROOT}

CMD ["tail", "-f", "/dev/null"]
