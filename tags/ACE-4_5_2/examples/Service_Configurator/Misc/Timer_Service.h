/* -*- C++ -*- */
// $Id$

#include "ace/Service_Config.h"

class Timer_Service : public ACE_Service_Object
{
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

ACE_STATIC_SVC_DECLARE (Timer_Service)

