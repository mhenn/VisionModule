modprobe i2c-dev
rmmod cy8c9540a
i2cset 0 0x20 0x29 0x04
i2cset 0 0x20 0x2C 0x02
modprobe cy8c9540a
