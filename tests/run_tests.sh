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

usage="usage: $0 <target>"
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

####
#### Process command line arguments.
####
target=
run_command=
chorus=
if [ $# -eq 1 ]; then
  target=$1
  if rsh $target help | head -1 | egrep CHORUS > /dev/null; then
    run_command=arun
    chorus=1
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
  if [ "$chorus" ]; then
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

if [ -x /bin/uname -a `uname -s` = 'LynxOS' ]; then
  LynxOS=1
fi

if [ -x /bin/uname -a `uname -s` = 'unicos' ]; then
  Unicos=1
fi

ace_version=`head -1 ../VERSION | sed 's/.*version \([0-9.]*\).*/\1/'`

if [ ! "$chorus" ]; then
  user=${LOGNAME:-`whoami`}
  start_test_resources=`ipcs | egrep $user`
fi # ! chorus

ACE_BUILD_COMPONENTS=`$ACE_ROOT/bin/ace_components --ace`
OTHER=`echo $ACE_BUILD_COMPONENTS | egrep ' Other '`
TOKEN=`echo $ACE_BUILD_COMPONENTS | egrep ' Token '`

echo "Starting ACE version $ace_version tests . . ."

mv -f "$compilation_log" "$compilation_log.bak" > /dev/null 2>&1

run Basic_Types_Test
test $chorus || run Env_Value_Test      # tests Env_Value_T and Process
run Atomic_Op_Test                      # tests Atomic_Op
run CDR_Test                            # tests ACE_OutputCDR and ACE_InputCDR
run Semaphore_Test                      # tests ACE_Thread_Semaphore
run TSS_Test                            # tests ACE_Task, ACE_Mutex, ACE_Guard
run Timeprobe_Test                      # tests ACE_Timeprobes
run Time_Value_Test                     # tests Time_Value
run High_Res_Timer_Test                 # tests High_Res_Timer
run SString_Test                        # tests ACE_CString and ACE_SString
run Collection_Test                     # tests ACE Collection classes
test $chorus || test $LynxOS || test $Unicos || run DLL_Test # tests ACE_DLL class
# Naming_Test: UNICOS fails due to feature not supported
test $OTHER && \
  (test $chorus || test $LynxOS || test $Unicos || run Naming_Test) # tests ACE_Naming_Context, ACE_WString

run Handle_Set_Test                     # tests ACE_Handle_Set
run OrdMultiSet_Test                    # tests ACE_Ordered_MultiSet
# Mem_Map_Test: UNICOS fails due to feature not supported
test $chorus || test $Unicos || run Mem_Map_Test        # tests ACE_Mem_Map

# SV_Shared_Memory_Test: UNICOS fails due to feature not supported
test $Unicos || run SV_Shared_Memory_Test               # tests ACE_SV_Shared_Memory, fork
# MM_Shared_Memory_Test: UNICOS fails due to feature not supported
test $chorus || test $Unicos || run MM_Shared_Memory_Test # tests ACE_Shared_Memory_MM

run Sigset_Ops_Test                     # tests ACE_sigset*() functions

run Timer_Queue_Test                    # tests ACE_Event_Handler, ACE_Timer_Queue
run MT_Reactor_Timer_Test               # tests ACE_Reactor's timer mechanism.

run SOCK_Connector_Test                 # tests ACE_SOCK_Connector

run Task_Test                           # tests ACE_Thread_Manager, ACE_Task
# Thread_Manager_Test: UNICOS fails due to no pthread_kill nor pthread_cancel
test $Unicos || run Thread_Manager_Test                 # tests ACE_Thread_Manager, ACE_Task
run Thread_Pool_Test                    # tests ACE_Thread_Manager, ACE_Task
run Future_Test                         # tests ACE_Thread_Manager, ACE_Task
run RB_Tree_Test                        # tests ACE_RB_Tree, ACE_RB_Tree_Iterator
run Reactors_Test                       # tests ACE_Task, ACE_Mutex, ACE_Reactor
run Reactor_Exceptions_Test             # tests ACE_Reactor and C++ exceptions
run Reactor_Notify_Test                 # tests ACE_Reactor's notify() method, ACE_Task
run Reactor_Timer_Test                  # tests ACE_Event_Handler, ACE_Reactor
# test $OTHER && run Thread_Pool_Reactor_Test            # tests ACE_TP_Reactor, ACE_Select_Reactor, ACE_Acceptor...
test $chorus || run Reactor_Performance_Test # tests ACE_Event_Handler, ACE_Reactor
run Notify_Performance_Test             # tests ACE_Event_Handler, ACE_Reactor
run Reader_Writer_Test                  # tests ACE_Thread_Manager, ACE_Mutex
test $chorus || run Priority_Reactor_Test # tests ACE_Priority_Reactor, ACE_Acceptor/ACE_Connector...
test $chorus || run SOCK_Test           # tests ACE_Thread_Manager, ACE_SOCK_SAP
test $chorus || run MT_SOCK_Test        # tests ACE_Thread_Manager, ACE_Acceptor/ACE_Connector

# ifdef ACE_HAS_STREAM_PIPES
run SPIPE_Test                          # tests ACE_SPIPE_Acceptor/ACE_Connector, ACE_Thread_Manager
run UPIPE_SAP_Test                      # tests ACE_UPIPE, ACE_Thread, ACE_Thread_Manager

run Barrier_Test                        # tests ACE_Service_Config, ACE_Barrier
run Buffer_Stream_Test                  # tests ACE_Service_Config, ACE_Stream (ACE_Module, ACE_Task, ACE_Message_Queue)
run Priority_Buffer_Test                # tests ACE_Service_Config, ACE_Message_Queue
run Dynamic_Priority_Test               # tests ACE_ACE_Message_Queue, ACE_Dynamic_Message_Queue
run Recursive_Mutex_Test                # tests ACE_Service_Config, ACE_Recursive_Thread_Mutex

# Time_Service_Test: UNICOS fails dlopen() - no shared libs on UNICOS
if [ -f ../netsvcs/servers/main ]; then
  test $TOKEN && (test $chorus || test $LynxOS || test $Unicos || run Time_Service_Test) # tests libnetsvcs
fi
# Tokens_Test: UNICOS fails dlopen() - no shared libs on UNICOS
test $TOKEN && (test $chorus || test $LynxOS || test $Unicos || run Tokens_Test) # tests ACE_Token

run Map_Manager_Test                    # tests ACE_Map Manager and ACE_Hash_Map_Manager + Forward and Reverse Map Iterators.
run Map_Test                            # tests ACE_Map + Forward and Reverse Map Iterators.
run Message_Queue_Notifications_Test    # tests ACE_Message_Queue + ACE_Reactor
test $chorus || run Message_Queue_Test  # tests ACE_Message_Queue + Forward and Reverse Message Queue Iterators.
test $chorus || run Simple_Message_Block_Test # tests ACE_Message_Block
test $chorus || run Message_Block_Test  # tests ACE_Message_Block and ACE_Message_Queue
test $chorus || run Pipe_Test           # tests ACE_Pipe
test $chorus || run Process_Mutex_Test  # tests ACE_Process_Mutex
test $chorus || run Thread_Mutex_Test   # tests ACE_Thread_Process_Mutex
test $chorus || run Process_Strategy_Test # tests ACE_ACE_Strategy_Acceptor
run Service_Config_Test                 # tests ACE_Service_Config
# Priority_Task_Test: UNICOS fails due to getprio - feature not supported
test $Unicos || run Priority_Task_Test                  # tests ACE_Task with priorities
run IOStream_Test                       # tests ACE_IOStream and ACE_SOCK_Stream
run Enum_Interfaces_Test                # tests ACE_ACE::get_ip_interfaces()
test $chorus || run Upgradable_RW_Test  # tests ACE_RW locks
test $chorus || run Conn_Test           # tests ACE_Thread_Manager, ACE_Acceptor/ACE_Connector, ACE_SOCK_SAP

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
