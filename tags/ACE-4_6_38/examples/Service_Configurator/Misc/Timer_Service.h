/* -*- C++ -*- */
// $Id$

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Timer_Service_1 : public ACE_Service_Object
{
  // = TITLE
  //   Demonstrates a simple timer service that can be configured
  //   statically.
public:
  virtual int init (int argc, char *argv[]);
  // Initialization hook.

  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *);
  // Timeout hook.

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  // Close hook.

private:
  enum
  {
    TIMEOUT = 4, // Wait 4 seconds between timeouts
    MAX_TIMEOUTS = 10 // Don't timeout for more than 10 times.
  };

  int max_timeouts_;
  // Maximum number of timeouts before shutting down the test.

  int cur_timeouts_;
  // Current number of timeouts.
};

class Timer_Service_2 : public Timer_Service_1
{
};

// Declare both static and dynamic services.
ACE_STATIC_SVC_DECLARE (Timer_Service_1)
ACE_SVC_FACTORY_DECLARE (Timer_Service_1)
ACE_SVC_FACTORY_DECLARE (Timer_Service_2)

