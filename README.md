# w1209
W1209 thermostat firmware using [stm8-tiny](https://github.com/ryanplusplus/stm8-tiny).

## Setup
- Copy `tools/Linux/udev/70-st-link.rules` to `/etc/udev/` and run `udevadm control --reload-rules`.

## Use
### Compile
```shell
make -f <target>.mk
```

### Clean
```shell
make -f <target>.mk clean
```

### Erase via SWIM
```shell
make -f <target>.mk erase
```

### Flash via SWIM
```shell
make -f <target>.mk upload
```
