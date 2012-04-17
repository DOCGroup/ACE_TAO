
//=============================================================================
/**
 *  @file    Reactor_Fairness_Test.h
 *
 *  $Id$
 *
 *  This file includes template class definitions for the
 *  Reactor_Fairness_Test.cpp program.  This class gets its own
 *  header file to work around AIX C++ compiler "features" related
 *  to template instantiation...
 *
 *
 *  @author Steve Huston <shuston@riverace.com>
 */
//=============================================================================


#ifndef ACE_TESTS_REACTOR_FAIRNESS_TEST_H
#define ACE_TESTS_REACTOR_FAIRNESS_TEST_H

#include "ace/SOCK_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Svc_Handler.h"

/**
 * @class Read_Handler
 *
 * @brief Simple class for reading in the data
 */
class Read_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  Read_Handler () : chunks_in (0) {}

  /// The Svc_Handler callbacks.
  virtual int handle_input (ACE_HANDLE h);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

private:
  unsigned int chunks_in;
};

#endif /* ACE_TESTS_REACTOR_FAIRNESS_TEST_H */
