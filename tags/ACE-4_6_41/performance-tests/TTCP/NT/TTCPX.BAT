@echo off
:start
ttcp -s -t -v -D -p5006 -b8192 -l1024 -n2000 %1
sleep 5
ttcp -s -t -v -D -p5006 -b8192 -l2048 -n2000 %1
sleep 5
ttcp -s -t -v -D -p5006 -b8192 -l4096 -n2000 %1
sleep 5
ttcp -s -t -v -D -p5006 -b8192 -l8192 -n2000 %1
sleep 5
ttcp -s -t -v -D -p5006 -b8192 -l16384 -n2000 %1
sleep 5
ttcp -s -t -v -D -p5006 -b8192 -l32768 -n2000 %1
sleep 5
ttcp -s -t -v -D -p5006 -b8192 -l65536 -n2000 %1
sleep 5
ttcp -s -t -v -D -p5006 -b8192 -l131072 -n2000 %1
