#!/bin/sh -f
# This is the UNIX version of the one-button ACE tests.
# Contributed by Michael Rueger <mike@SYSCOMP.DE>

run()
{
  echo running $1

  ./$1

  if [ ! $? ]; then
    echo exit status failed - exiting
    exit 1
  fi

  if [ -e core ]; then
    echo dumped core - exiting
    exit 1
  fi

  grep "^starting.* at " log/$1.log >/dev/null

  if [ ! $? ]; then
    echo start comment not in log file - exiting
    exit 1
  fi

  grep "^Exiting.* at " log/$1.log >/dev/null

  if [ ! $? ]; then
    echo Exiting comment not in log file - exiting
    exit 1
  fi
}

echo "Starting tests..."
rm -f core

run Barrier_Test
run Buffer_Stream_Test
run CPP_Test
run Handle_Set_Test
run Mem_Map_Test
run Mutex_Test
run Naming_Test
run Priority_Buffer_Test
run Reactors_Test
run Reactor_Timer_Test
run Reader_Writer_Test
run Recursive_Mutex_Test
run Shared_Memory_MM_Test
run Shared_Memory_SV_Test
run SPIPE_Test
run SString_Test
run SV_Shared_Memory_Test
run Task_Test
run Thread_Manager_Test
run Thread_Pool_Test
run Timer_Queue_Test
run Time_Service_Test
run Time_Value_Test
run TSS_Test
run UPIPE_SAP_Test

echo "Tests complete..."

