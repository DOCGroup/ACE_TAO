#!/bin/bash
#$Id$

exec_cmd=
service_pid=
reset_flag=
server_name=
use_imr=1
while getopts "c:e:p:r:s:" OPTION
do
    case $OPTION in
        c)
            use_imr=$OPTARG
            echo "use_imr = $use_imr";
            ;;
        e)
            exec_cmd=$OPTARG
            ;;
        p)
            service_pid=$OPTARG
            ;;
        r)
            reset_flag=$OPTARG
            ;;
        s)
            server_name=$OPTARG
    esac
done


if [ 0 -ne $reset_flag ]; then
    dead=`ps -p $service_pid | grep -l $service_pid; echo $?`
    while [ 1 -ne $dead ]; do
        echo "restart: $service_pid is not dead yet"
        sleep 1
        dead=`ps -p $service_pid | grep -l $service_pid; echo $?`
    done
    if [ 0 -ne $use_imr ]; then
#        tao_imr shutdown $server_name
#        status=$?
#        echo shutdown returned $status
        echo restart: calling tao_imr start
        tao_imr start $server_name
        status=$?
        echo restart: tao_imr start returned $status
        exit;
    fi
fi

echo restart: running $exec_cmd
$exec_cmd &
exec_cmd_pid=$!
echo "restart: pid is $exec_cmd_pid, waiting"

wait $exec_cmd_pid
exec_cmd_status=$?
echo "restart: status is $exec_cmd_status"

if [[ 0 != $exec_cmd_status ]]; then
    ./restart.sh -e "$exec_cmd" -c $use_imr -p $$ -r 1 -s $server_name &
fi
