# msx-utils
Small applications to use with MSX

## Requirements

- Docker (https://docs.docker.com/install/)
- docker-compose (https://docs.docker.com/compose/install/)

## How to use

### Running
To build a source file, run:

`docker-compose run --rm sdcc build <source.c>`

Ex.:

`docker-compose run --rm sdcc build dos-utils/sleep.c`

Ps.: The root folder is ./workspace

For more information:

https://hub.docker.com/r/rfocosi/msx-sdcc-toolchain

### Extra libs
If you need extra libs to your project, add then on directories:

```
./share/include
./share/lib
```

# Credits

Some sources are based on:
- Konamiman's repository: https://github.com/Konamiman/
- Oduvaldo's repository: https://github.com/ducasp/
- Fusion-C lib: http://www.ebsoft.fr/shop/es/home/66-fusion-c.html

If your work is here too, tell me and I'll add you on the credits :)