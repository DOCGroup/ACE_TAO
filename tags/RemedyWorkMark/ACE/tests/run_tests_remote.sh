#!/bin/sh
# $Id$
#
# This is the UNIX version of the one-button ACE tests.
# Contributed originally by Michael Rueger <m_rueger@SYSCOMP.DE> and
# modified substantially by the DOC group.
#
# It also supports remote invocation on a CHORUS/ClassiX/MVME target.
# For that environment, these steps are required:
# 1) Create a "log" directory below the root mount point on the host.
# 2) Mount the "tests" directory on the target, and add it to the PATH
#    on the target.
# 3) cd to the directory that contains this script on the host.
# 4) Create a symlink to the "log" directory create in step 1) above.
# 5) ./run_tests.sh <target_hostname>

if [ -x /bin/rm ]; then
  RM=/bin/rm
elif [ -x /usr/bin/rm ]; then
  RM=/usr/bin/rm
else
  echo "Can't find rm, aborting." 1>&2
  exit 1
fi

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

if [ ! "$ACE_ROOT" ]; then
  ACE_ROOT=..
  export ACE_ROOT
fi

# Some tests fork/exec copies of themselves (e.g. Pipe_Test). If execvp
# ends up getting used, the PATH has to include "." or the test won't work.
PATH=.:$PATH
export PATH

IFS="|"
tmp=/tmp
compilation_log="log/compilations.log"
shlib_suffix=".so"

LD_LIBRARY_PATH=$ACE_ROOT/ace:${LD_LIBRARY_PATH:-/usr/lib}
export LD_LIBRARY_PATH

#### If uname isn't on the user's PATH, store any string
#### in $sysname.
sysname=`uname -s 2>&1`

if [ $sysname = 'HP-UX' ]; then
  SHLIB_PATH=$ACE_ROOT/ace:${SHLIB_PATH:-/usr/lib}
  export SHLIB_PATH
  shlib_suffix=".sl"
fi

if [ $sysname = 'AIX' ]; then
  LIBPATH=$ACE_ROOT/ace:${LIBPATH:-/usr/lib:/lib}
  export LIBPATH
fi

if echo $sysname | grep -q CYGWIN; then
  shlib_suffix=".dll"
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
  if rsh $target help | head -1 | egrep -i CHORUS > /dev/null; then
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
  $RM -f core log/$1.log

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
if [ $sysname = 'LynxOS' ]; then
  LynxOS=
fi

Unicos=1
if [ $sysname = 'unicos' ]; then
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
if [ -f libDLL_Test$shlib_suffix ]; then
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

if [ $sysname != 'procnto' ]; then
  # Limit the amount of memory required by the tests to 64Mb.
  ulimit -d 65536
fi # ! procnto

# Redirection gets confused when rsh is involved (Chorus)
for i in `cat run_tests_remote.lst | tr "\012" "$IFS"`; do

  if [ "$i" != "" ]; then

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
      run $test
    elif eval test $precond; then
      run $test
    else
      if echo $precond | egrep '(DISABLED)|(OTHER)' > /dev/null; then :; else
        echo Skipping $test on this platform
      fi
    fi
  fi
done

echo "Finished ACE version $ace_version tests."

$RM -f ace_pipe_name pattern \
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
