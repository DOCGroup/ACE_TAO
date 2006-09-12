#!/bin/sh

bin/initdb.sh
sleep 4
export GUI=-G

java imp.ConfigServer $GUI localhost
