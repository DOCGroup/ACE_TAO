#!/bin/sh -f
# This is the UNIX version of the one-button ACE tests.
# Contributed by Michael Rueger <m_rueger@SYSCOMP.DE>

IFS="|"
tmp=/tmp

run()
{
  echo running $1
  /bin/rm -f core log/$1.log

  ./$1
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

echo "Starting tests..."

run Atomic_Op_Test                      # uses Atomic_Op
run TSS_Test                            # uses Task, Mutex, Guard
run Time_Value_Test
run SString_Test
run Naming_Test                         # uses Naming_Context, WString

run Handle_Set_Test                     # uses Handle_Set
run Mem_Map_Test                        # uses Mem_Map

run SV_Shared_Memory_Test               # uses SV_Shared_Memory, fork
run MM_Shared_Memory_Test

run Sigset_Ops_Test                     # uses sigset*() functions

run Mutex_Test                          # uses Process_Mutex

run Timer_Queue_Test                    # uses Event_Handler, Timer_Queue

run SOCK_Connector_Test                 # uses SOCK_Connector

run Task_Test                           # uses Thread_Manager, Task
run Thread_Manager_Test                 # uses Thread_Manager, Task
run Thread_Pool_Test                    # uses Thread_Manager, Task
run Future_Test                         # uses Thread_Manager, Task
run Reactors_Test                       # uses Task, Mutex, Reactor
run Reactor_Exceptions_Test             # uses Reactor and C++ exceptions
run Reactor_Notify_Test                 # uses Reactor's notify() method, Task
run Reactor_Timer_Test                  # uses Event_Handler, Reactor
run Reactor_Performance_Test            # uses Event_Handler, Reactor
run Notify_Performance_Test             # uses Event_Handler, Reactor
run Reader_Writer_Test                  # uses Thread_Manager, Mutex
run Priority_Reactor_Test               # uses Priority_Reactor, Acceptor/Connector...
run SOCK_Test                           # uses Thread_Manager, SOCK_SAP
run MT_SOCK_Test                        # uses Thread_Manager, Acceptor/Connector

# ifdef ACE_HAS_STREAM_PIPES
run SPIPE_Test                          # uses SPIPE_Acceptor/Connector, Thread_Manager
run UPIPE_SAP_Test                      # uses UPIPE, Thread, Thread_Manager

run Barrier_Test                        # uses Service_Config, Barrier
run Buffer_Stream_Test                  # uses Service_Config, Module (Stream,Task, Message_Queue)
run Priority_Buffer_Test                # uses Service_Config, Message_Queue
run Recursive_Mutex_Test                # uses Service_Config, Recursive_Thread_Mutex

run Time_Service_Test                   # uses libnetsvcs
run Tokens_Test

run Map_Manager_Test                    # uses Map Manager and Hash Map Manager + Forward and Reverse Map Iterators.
run Message_Queue_Notifications_Test    # uses Message_Queue + Reactor.
run Message_Queue_Test                  # uses Message_Queue + Forward and Reverse Message Queue Iterators.
run Simple_Message_Block_Test           # uses Message_Block
run Message_Block_Test                  # uses Message_Block and Message_Queue
run Pipe_Test                           # uses Pipe
run Process_Mutex_Test                  # uses Process_Mutex
run Process_Strategy_Test               # uses ACE_Strategy_Acceptor
run Service_Config_Test                 # uses Service_Config
run Priority_Task_Test                  # uses Task with priorities
run IOStream_Test                       # uses ACE_IOStream and ACE_SOCK_Stream
run Enum_Interfaces_Test                # uses ACE::get_ip_interfaces()
run Conn_Test                           # uses Thread_Manager, Acceptor/Connector, SOCK_SAP

echo "Tests complete..."

/bin/rm -f ace_pipe_name pattern $tmp/ace_temp_file $tmp/ace_temp_file2 $tmp/ace_test_file $tmp/Naming_Test*
echo; echo
ipcs

# EOF
