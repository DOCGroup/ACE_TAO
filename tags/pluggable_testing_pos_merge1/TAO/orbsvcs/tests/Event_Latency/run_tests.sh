#! /bin/sh
# $Id$
#
# Runs Event_Latency test.

usage="Usage: $0 [-m <messages>] [-s <suppliers>]"
messages=1000
suppliers=1

########
######## Interpret command arguments.
########
skip_arg=0
for arg in "$@"; do
  if [ $skip_arg -eq 0 ]; then
    case $arg in
       -m ) messages=$2
            skip_arg=1
            ;;
       -s ) suppliers=$2
            skip_arg=1
            ;;
       -* ) echo undefined option: $arg
            echo $usage
            exit 1
            ;;
       *  ) echo $usage
            exit 1
            ;;
     esac
 else
   skip_arg=0
 fi
 shift
done

whoami=`whoami`

#### Get the user name
if [ "$LOGNAME" ]; then
  #### LOGNAME is preserved across su
  login=$LOGNAME
else
  #### whoami returns the users login, which changes across su
  login=$whoami
fi

run ()
#### Run the process in the RT class, if root on Solaris.
{
  if [ `uname -s` = 'SunOS'  -a  "$whoami" = 'root' ]; then
    LD_BIND_NOW=1 priocntl -e -c RT -p 0 $@
  else
    $@
  fi
}


########
######## Clean up on normal or abnormal termination.
########
trap '../start_services clean' 0 1 2 3 15


########
######## Start NameService, etc.
########
run ../start_services
NameService=`cat /tmp/nameserviceior_$login`
export NameService


########
######## Run the Event_Latency test.
########
./Event_Latency -j -t 50 -m $messages -s $suppliers -c $suppliers
