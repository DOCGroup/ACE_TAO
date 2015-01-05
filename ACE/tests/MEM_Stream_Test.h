/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    MEM_Stream_Test.h
 *
 *  $Id$
 *
 * This file has the class definitions needed for template generation in
 * MEM_Stream_Test.cpp.  They have to be in a separate file so AIX xlC can
 * find them at auto-instantiate time.
 *
 *
 *  @author Steve Huston <shuston@riverace.com>
 */
//=============================================================================


#ifndef ACE_TESTS_MEM_STREAM_TEST_H
#define ACE_TESTS_MEM_STREAM_TEST_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/MEM_Stream.h"
#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch_Traits.h"

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)

/**
 * @class Echo_Handler
 *
 * @brief Simple class for reading in the data and then sending it back
 */
class Echo_Handler : public ACE_Svc_Handler<ACE_MEM_STREAM, ACE_SYNCH>
{
public:
  /// The Svc_Handler callbacks.
  Echo_Handler (ACE_Thread_Manager *thr_mgr = 0);
  virtual int open (void *);
  static void reset_handler (void);
  virtual int handle_input (ACE_HANDLE h);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);
  virtual int svc (void);

private:
  ACE_TCHAR name_[MAXPATHLEN];
  u_short connection_;
};

#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS */

#endif /* ACE_TESTS_MEM_STREAM_TEST_H */
