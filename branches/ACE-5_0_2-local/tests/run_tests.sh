#!/bin/sh
# $Id$
#
# This is the UNIX version of the one-button ACE tests.
# Contributed by Michael Rueger <m_rueger@SYSCOMP.DE>
#
# It also supports remote invocation on a CHORUS/ClassiX/MVME target.
# For that environment, these steps are required:
# 1) Create a "log" directory below the root mount point on the host.
# 2) Mount the "tests" directory on the target, and add it to the PATH
#    on the target.
# 3) cd to the directory that contains this script on the host.
# 4) Create a symlink to the "log" directory create in step 1) above.
# 5) ./run_tests.sh <target_hostname>

usage="usage: $0 [-p] <target>
       -p: purify tests"

purify=0
purify_with_old_gcc=0

####
#### Interpret command arguments.
####
for arg in "$@"; do
  case $arg in
     -p )  purify=1
           shift
           ;;

     -'?' ) echo $usage
           exit 0
           ;;

     -*) echo $0: unknown option $arg
         echo $usage
         exit 1
         ;;
  esac
done

IFS="|"
tmp=/tmp
compilation_log="log/compilations.log"
LD_LIBRARY_PATH=$ACE_ROOT/ace:${LD_LIBRARY_PATH:-/usr/lib}
export LD_LIBRARY_PATH

if [ -x /bin/uname -a `uname -s` = 'HP-UX' ]; then
  SHLIB_PATH=$ACE_ROOT/ace:${SHLIB_PATH:-/usr/lib}
  export SHLIB_PATH
fi

if [ -x /bin/uname -a `uname -s` = 'AIX' ]; then
  LIBPATH=$ACE_ROOT/ace:${LIBPATH:-/usr/lib:/lib}
  export LIBPATH
fi

if [ $purify -eq 1 ]; then
  if echo $PWD | egrep 'gcc|g++'; then
    purify_with_old_gcc=1
  fi
fi

####
#### Process command line arguments.
####
target=
run_command=
chorus=1
if [ $# -eq 1 ]; then
  target=$1
  if rsh $target help | head -1 | egrep CHORUS > /dev/null; then
    run_command=arun
    chorus=
  else
    #### Only support Chorus/MVME, for now.
    echo $0: host $1 does not appear to be Chorus/MVME:  not supported.
    exit 1
  fi
elif [ $# -ne 0 ]; then
  echo $usage
  exit 1
fi

run()
{
  /bin/rm -f core log/$1.log

  if [ ! -f "$1" ]; then
    echo 1>&2 "Making $1 . . ."
    remove_exe_after_test="true"
    make BIN="$1" >> "$compilation_log"
  else
    remove_exe_after_test=""
  fi

  echo "running $1"
  if [ -z "$chorus" ]; then
    #### Assumes that the PATH has been set on the target.
    rsh $target $run_command $1
  else
    ./$1
  fi
  status=$?

  if [ $status -ne 0 ]; then
    echo \"$1\" FAILED with exit status $status!!!!
  fi

  if [ -f core ]; then
    echo \"$1\" dumped core!!!!
  fi

  if [ -f log/$1.log ]; then
    sh ./run_tests.check log/$1.log
  else
    echo "No log file (log/$1.log) is present"
  fi

  if [ "$remove_exe_after_test" ]; then
    echo 1>&2 "Discarding $1"
    rm -f "$1" ".obj/$1.o"
  fi
}

LynxOS=1
if [ -x /bin/uname -a `uname -s` = 'LynxOS' ]; then
  LynxOS=
fi

Unicos=1
if [ -x /bin/uname -a `uname -s` = 'unicos' ]; then
  Unicos=
fi

ace_version=`head -1 ../VERSION | sed 's/.*version \([0-9.]*\).*/\1/'`

if [ ! "$chorus" ]; then
  user=${LOGNAME:-`whoami`}
  start_test_resources=`ipcs | egrep $user`
fi # ! chorus

ACE_BUILD_COMPONENTS=`$ACE_ROOT/bin/ace_components --ace`
OTHER=`echo $ACE_BUILD_COMPONENTS | egrep ' Other '`
TOKEN=`echo $ACE_BUILD_COMPONENTS | egrep ' Token '`

libDLL_Test=
if [ -f libDLL_Test.so ]; then
  libDLL_Test=1
fi

netsvcs_main=
if [ -f ../netsvcs/servers/main ]; then
  netsvcs_main=1
fi

# Testing for this flag always disables the test
DISABLED=

echo "Starting ACE version $ace_version tests . . ."

mv -f "$compilation_log" "$compilation_log.bak" > /dev/null 2>&1

# Limit the amount of memory required by the tests to 64Mb
ulimit -d 65536

while read i; do

  case $i in
    */*)
       p=`dirname $i | sed 's%/% \&\& test $%g'`;
       precond="\$$p";
       test=`basename $i`;
       ;;
    *)
       precond="";
       test=$i;
      ;;
  esac
 
#  echo =****= $precond ===== $test;

  if test -z "$precond"; then
    run $test;
  elif eval test $precond; then
    run $test;
  else
    echo Skipping $test on this platform;
  fi

done <run_tests.lst

echo "Finished ACE version $ace_version tests."

/bin/rm -f ace_pipe_name pattern \
           $tmp/ace_temp_file* \
           $tmp/ace_test_file \
           $tmp/Naming_Test*

if [ ! "$chorus" ]; then
  end_test_resources=`ipcs | egrep $user`
  if [ "$start_test_resources" != "$end_test_resources" ]; then
    echo WARNING: the ACE tests _may_ have leaked OS resources!
    ipcs
  fi
fi

# EOF
