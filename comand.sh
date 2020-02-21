#!/bin/bash

sudo rmmod driver.ko
make
sudo insmod driver.ko