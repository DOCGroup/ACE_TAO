#!/bin/sh -f
# This is the UNIX version of the one-button ACE tests.
# Contributed by Michael Rueger <m_rueger@SYSCOMP.DE>

IFS="|"
tmp=/tmp

# these patterns should not be included in log file
ERROR_MSGS="assertion failed|not supported|No such file or directory|Invalid argument|timeout|Bad file number"

# these patterns must be included in log file
SUCCESS_MSGS="starting|Ending"

run()
{
  echo running $1
  /bin/rm -f core

  ./$1
  status=$?

  if [ $status -ne 0 ]; then
    echo \"$1\" FAILED with exit status $status!!!!
  fi

  if [ -f core ]; then
    echo \"$1\" dumped core!!!!
  fi

  for i in $SUCCESS_MSGS; do
    grep $i log/$1.log >/dev/null
    if [ $? -eq 1 ]; then
      echo Error in log file no line with $i
    fi
  done

  for i in $ERROR_MSGS; do
    grep $i log/$1.log
  done
}

echo "Starting tests..."

run Time_Value_Test
run SString_Test
run Naming_Test			# uses Naming_Context, WString

run Handle_Set_Test		# uses Handle_Set
run Mem_Map_Test		# uses Mem_Map

run SV_Shared_Memory_Test	# uses SV_Shared_Memory, fork
run Shared_Memory_SV_Test	# uses Shared_Memory_SV, fork
run Shared_Memory_MM_Test	# uses Shared_Memory, fork, Thread_Manager
run Mutex_Test			# uses Process_Mutex

run Timer_Queue_Test		# uses Event_Handler, Timer_Queue

run Task_Test			# uses Thread_Manager, Task
run Thread_Manager_Test		# uses Thread_Manager, Task
run Thread_Pool_Test		# uses Thread_Manager, Task
run CPP_Test			# uses Thread_Manager, SAP
run TSS_Test			# uses Task, Mutex, Guard
run Reactors_Test		# uses Task, Mutex, Reactor
run Reactor_Timer_Test		# uses Event_Handler, Reactor
run Reader_Writer_Test		# uses Thread_Manager, Mutex

# ifdef ACE_HAS_STREAM_PIPES
run SPIPE_Test			# uses SPIPE_Acceptor/Connector, Thread_Manager
run UPIPE_SAP_Test		# uses UPIPE, Thread, Thread_Manager

run Barrier_Test		# uses Service_Config, Barrier
run Buffer_Stream_Test		# uses Service_Config, Module (Stream,Task, MQ)
run Priority_Buffer_Test	# uses Service_Config, MQ
run Recursive_Mutex_Test	# uses Service_Config, Recursive_Thread_Mutex

run Time_Service_Test		# uses libnet_svcs
run Tokens_Test

run Map_Manager_Test
run Message_Queue_Test

echo "Tests complete..."

/bin/rm -f ace_pipe_name pattern $tmp/ace_temp_file $tmp/ace_temp_file2 $tmp/ace_test_file $tmp/Naming_Test
echo; echo
ipcs

# EOF
