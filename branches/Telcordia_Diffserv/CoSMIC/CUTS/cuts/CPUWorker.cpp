// $Id$

#include "cuts/CPUWorker.h"
#include "ace/ACE.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/CPUWorker.inl"
#endif

#define CUTS_CPU_CYCLES 25

CUTS_ACTION_TABLE_BEGIN (CUTS_CPU_Worker, CUTS_Worker)
  CUTS_ACTION_TABLE_ENTRY("Run_Processor", CUTS_CPU_Worker::Run_Processor);
CUTS_ACTION_TABLE_END (CUTS_CPU_Worker)

//
// CUTS_CPU_Worker
//
CUTS_CPU_Worker::CUTS_CPU_Worker (void)
: visits_ (0)
{

}

//
// ~CUTS_CPU_Worker
//
CUTS_CPU_Worker::~CUTS_CPU_Worker (void)
{

}

//
// process
//
void CUTS_CPU_Worker::process (void)
{
  ++ this->visits_;

  for (int cycle = 0; cycle < CUTS_CPU_CYCLES; cycle ++)
  {
    ACE::is_prime (16127, 2, 16127 / 2);
  }
}
