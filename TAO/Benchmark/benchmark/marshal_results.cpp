// ============================================================================
//
// = LIBRARY
//    CORBA Benchmark/Marshal
// 
// = FILENAME
//    marshal_results.cpp
//
// = AUTHOR
//    Aniruddha Gokhale
// 
// ============================================================================

#include "benchmark/marshal_results.h"

#if !defined (__ACE_INLINE__)
#include "benchmark/marshal_results.i"
#endif /* __ACE_INLINE__ */

int 
CORBA_Marshal_Results::log_results (void)
{
  return 0;
}

int
CORBA_Marshal_Results::print_results (void)
{
  //  ACE_DEBUG ((LM_DEBUG, "CORBA_Marshal_Results::print_results\n"));

  return 0; // success
}
