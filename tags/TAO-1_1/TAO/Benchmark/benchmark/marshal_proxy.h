// $Id$

// ============================================================================
//
// = LIBRARY
//    CORBA Benchmark
//
// = FILENAME
//    marshal_proxy.h
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _CORBA_MARSHAL_PROXY_H_
#define _CORBA_MARSHAL_PROXY_H_

#include "benchmark/marshal_options.h"
#include "benchmark/marshal_results.h"

class CORBA_Marshal_Proxy
{
  // = TITLE
  //    Abstract handle for all CORBA_Marshal tests.
  // = DESCRIPTION
  //   The IMPL class is the CORBA specific class which does the real work. The
  //   IMPL class must support the run method
public:
  typedef CORBA_Marshal_Options OPTIONS;
  typedef CORBA_Marshal_Results RESULTS;

  CORBA_Marshal_Proxy (void);
  // constructor

  virtual ~CORBA_Marshal_Proxy (void);
  // destructor

  virtual int run (OPTIONS &, RESULTS &) = 0;
  // run the test

};

#if defined (__ACE_INLINE__)
#include "benchmark/marshal_proxy.i"
#endif /* __ACE_INLINE__ */

#endif
