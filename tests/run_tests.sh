#!/bin/sh -f
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

####
#### Process command line arguments.
####
target=
run_command=
chorus=
if [ $# -eq 1 ]; then
  target=$1
  if rsh $target help | head -1 | egrep CHORUS/ClassiX > /dev/null; then
    run_command=arun
    chorus=1
  else
    #### Only support Chorus/MVME, for now.
    echo $0: host $1 does not appear to be Chorus/MVME:  not supported.
  fi
elif [ $# -ne 0 ]; then
  echo $usage
  exit 1
fi

run()
{
  echo running $1
  /bin/rm -f core log/$1.log

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
    ./run_tests.check log/$1.log
  else
    echo "No log file (log/$1.log) is present"
  fi
}

if [ -x /bin/uname -a `uname -s` = 'LynxOS' ]; then
  LynxOS=1
fi

echo "Starting tests..."

run Basic_Types_Test
test $chorus || run Env_Value_Test      # uses Env_Value_T and Process
run Atomic_Op_Test                      # uses Atomic_Op
run Semaphore_Test                      # uses ACE_Thread_Semaphore
run TSS_Test                            # uses Task, Mutex, Guard
run Timeprobe_Test
run Time_Value_Test
run SString_Test
test $chorus || test $LynxOS || run Naming_Test # uses Naming_Context, WString

run Handle_Set_Test                     # uses Handle_Set
run OrdMultiSet_Test                    # uses Ordered_MultiSet
test $chorus || run Mem_Map_Test        # uses Mem_Map

run SV_Shared_Memory_Test               # uses SV_Shared_Memory, fork
test $chorus || run MM_Shared_Memory_Test

run Sigset_Ops_Test                     # uses sigset*() functions

test $LynxOS || run Timer_Queue_Test    # uses Event_Handler, Timer_Queue

run SOCK_Connector_Test                 # uses SOCK_Connector

run Task_Test                           # uses Thread_Manager, Task
run Thread_Manager_Test                 # uses Thread_Manager, Task
run Thread_Pool_Test                    # uses Thread_Manager, Task
run Future_Test                         # uses Thread_Manager, Task
run Reactors_Test                       # uses Task, Mutex, Reactor
run Reactor_Exceptions_Test             # uses Reactor and C++ exceptions
run Reactor_Notify_Test                 # uses Reactor's notify() method, Task
run Reactor_Timer_Test                  # uses Event_Handler, Reactor
test $chorus || test $LynxOS || run Reactor_Performance_Test # uses Event_Handler, Reactor
run Notify_Performance_Test             # uses Event_Handler, Reactor
run Reader_Writer_Test                  # uses Thread_Manager, Mutex
test $chorus || test $LynxOS || run Priority_Reactor_Test # uses Priority_Reactor, Acceptor/Connector...
test $chorus || run SOCK_Test           # uses Thread_Manager, SOCK_SAP
test $chorus || run MT_SOCK_Test        # uses Thread_Manager, Acceptor/Connector

# ifdef ACE_HAS_STREAM_PIPES
run SPIPE_Test                          # uses SPIPE_Acceptor/Connector, Thread_Manager
run UPIPE_SAP_Test                      # uses UPIPE, Thread, Thread_Manager

run Barrier_Test                        # uses Service_Config, Barrier
run Buffer_Stream_Test                  # uses Service_Config, Module (Stream,Task, Message_Queue)
run Priority_Buffer_Test                # uses Service_Config, Message_Queue
run Dynamic_Priority_Test               # uses ACE_Message_Queue, ACE_Dynamic_Message_Queue
run Recursive_Mutex_Test                # uses Service_Config, Recursive_Thread_Mutex

test $chorus || test $LynxOS || run Time_Service_Test # uses libnetsvcs
test $chorus || test $LynxOS || run Tokens_Test

run Map_Manager_Test                    # uses Map Manager and Hash Map Manager + Forward and Reverse Map Iterators.
run Message_Queue_Notifications_Test    # uses Message_Queue + Reactor.
test $chorus || run Message_Queue_Test  # uses Message_Queue + Forward and Reverse Message Queue Iterators.
test $chorus || run Simple_Message_Block_Test # uses Message_Block
test $chorus || run Message_Block_Test  # uses Message_Block and Message_Queue
test $chorus || run Pipe_Test           # uses Pipe
test $chorus || run Process_Mutex_Test  # uses Process_Mutex
test $chorus || run Thread_Mutex_Test   # uses Thread_Process_Mutex
test $chorus || run Process_Strategy_Test # uses ACE_Strategy_Acceptor
run Service_Config_Test                 # uses Service_Config
run Priority_Task_Test                  # uses Task with priorities
run IOStream_Test                       # uses ACE_IOStream and ACE_SOCK_Stream
run Enum_Interfaces_Test                # uses ACE::get_ip_interfaces()
test $chorus || run Conn_Test           # uses Thread_Manager, Acceptor/Connector, SOCK_SAP

echo "Tests complete..."

/bin/rm -f ace_pipe_name pattern $tmp/ace_temp_file $tmp/ace_temp_file2 $tmp/ace_test_file $tmp/Naming_Test*
echo; echo
ipcs

# EOF
