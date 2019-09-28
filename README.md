# msx-utils
Small applications to use with MSX

## Requirements

- Docker (https://docs.docker.com/install/)
- docker-compose (https://docs.docker.com/compose/install/)

## How to use

### Setup
Before start, build the container:

`docker-compose build`

### Running
To build a source file, run:

`docker-compose run sdcc build <source.c>`

Ex.:

`docker-compose run sdcc build dos-utils/sleep.c`

### SDCC Parameters
The SDCC parameters can be added to a `<source_name>.params` file

Ex.:

```
> ls
sleep.c
sleep.params

> cat sleep.params
--code-loc 0x180 --data-loc 0 -mz80 --disable-warning 196 --no-std-crt0 $SDCC_LIB_Z80/crt0_msxdos_advanced.rel $SDCC_LIB_Z80/printf.rel $SDCC_LIB_Z80/putchar_msxdos.rel asm.lib fusion.lib
```

Ps.: You can use the environment variables on `.params` file.
