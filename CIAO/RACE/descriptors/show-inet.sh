#!/bin/sh
/sbin/ifconfig | grep 250.100 -B 1 | gawk '{print $1,$2}'

