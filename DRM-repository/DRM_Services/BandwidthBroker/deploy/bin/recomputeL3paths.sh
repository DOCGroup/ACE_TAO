nohup bin/tr.sh access2 access3 192.168.130.2 >/dev/null &
nohup bin/tr.sh access2 access5 192.168.150.101 >/dev/null &
nohup bin/tr.sh access2 access6 192.168.160.2 >/dev/null &
nohup bin/tr.sh access2 access8 192.168.180.2 >/dev/null &
nohup bin/tr.sh access2 access9 192.168.190.2 >/dev/null &

nohup bin/tr.sh access3 access2 192.168.120.101 >/dev/null &
nohup bin/tr.sh access3 access5 192.168.150.101 >/dev/null &
nohup bin/tr.sh access3 access6 192.168.160.2 >/dev/null &
nohup bin/tr.sh access3 access8 192.168.180.2 >/dev/null &
nohup bin/tr.sh access3 access9 192.168.190.2 >/dev/null &

nohup bin/tr.sh access5 access2 192.168.120.101 > /dev/null &
nohup bin/tr.sh access5 access3 192.168.130.2 > /dev/null &
nohup bin/tr.sh access5 access6 192.168.160.2 > /dev/null &
nohup bin/tr.sh access5 access8 192.168.180.2 > /dev/null &
nohup bin/tr.sh access5 access9 192.168.190.2 > /dev/null &

nohup bin/tr.sh access6 access2 192.168.120.101 > /dev/null &
nohup bin/tr.sh access6 access3 192.168.130.2 > /dev/null &
nohup bin/tr.sh access6 access5 192.168.150.101 > /dev/null &
nohup bin/tr.sh access6 access8 192.168.180.2 > /dev/null &
nohup bin/tr.sh access6 access9 192.168.190.2 > /dev/null &

nohup bin/tr.sh access8 access2 192.168.120.101 > /dev/null &
nohup bin/tr.sh access8 access3 192.168.130.2 > /dev/null &
nohup bin/tr.sh access8 access5 192.168.150.101 > /dev/null &
nohup bin/tr.sh access8 access6 192.168.160.2 > /dev/null &
nohup bin/tr.sh access8 access9 192.168.190.2 > /dev/null &

nohup bin/tr.sh access9 access2 192.168.120.101 > /dev/null &
nohup bin/tr.sh access9 access3 192.168.130.2 > /dev/null &
nohup bin/tr.sh access9 access5 192.168.150.101 > /dev/null &
nohup bin/tr.sh access9 access6 192.168.160.2 > /dev/null &
nohup bin/tr.sh access9 access8 192.168.180.2 > /dev/null &

wait
echo All the routes found.
