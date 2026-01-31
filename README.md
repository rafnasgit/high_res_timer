High Resolution Timer
----------------------

This module provides a high resolution timer.
By default, set to 1 second expiry, however could be changed easily by changing the source file.

Tested on Kubuntu 25.10 release which had 6.17.0-8 kernel.

Usage
------
cd src

make

sudo insmod spot_high_res_timer.ko

Check kernel logs
------------------
sudo dmesg -W
