# What does this do?

This is an open source driver to control keyboard backlight on ROG GL553VE model laptops.
No more booting to Windows just to change the backlight color!!

# What devices does this support?

ROG GL553VE is the only device that this code has been tested on. 
However, you are free to test this driver with your laptop model.

For other models you can see this [project](https://github.com/wroberts/rogauracore).

# How do I use this?

Build the project with the following command:
```shell script
cmake --build /path/to/output/directory --target openauranb -- -j 6
```
Move to output directory and run:
```sh
$ sudo ./openauranb color_code
```
Argument color_code must be in hex format without the '#'.
For eg, If you wanted to make your backlight red (#FF0000), you would run:
```sh
$ sudo ./openauranb FF0000
```
You can use any color-picker to find hex code of your desired color.

# My laptop is not supported. What can I do?

Your best option is to install wireshark (or any URB capturing software) and study how
the colors are encoded in the packets. 
