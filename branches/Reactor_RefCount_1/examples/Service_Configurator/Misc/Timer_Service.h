// -*- C++ -*-
//
// $Id$

#include "ace/svc_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Service_Object.h"


class ACE_Svc_Export Timer_Service_1 : public ACE_Service_Object
{
  // = TITLE
  //   Demonstrates a simple timer service that can be configured
  //   statically.
public:
  Timer_Service_1 (void);
  // Default constructor.

  virtual int init (int argc, ACE_TCHAR *argv[]);
  // Initialization hook.

  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *);
  // Timeout hook.

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  // Close hook.

protected:
  ACE_TCHAR name_[BUFSIZ];
  // Keep track of which service this is!

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

class ACE_Svc_Export Timer_Service_2 : public Timer_Service_1
{
public:
  Timer_Service_2 (void);
  // Default constructor.
};

class ACE_Svc_Export Timer_Service_3 : public Timer_Service_1
{
public:
  Timer_Service_3 (void);
  // Default constructor.
};

// Declare both static and dynamic services.
ACE_STATIC_SVC_DECLARE_EXPORT (ACE_Svc, Timer_Service_1)
ACE_SVC_FACTORY_DECLARE (Timer_Service_1)
ACE_SVC_FACTORY_DECLARE (Timer_Service_2)
ACE_SVC_FACTORY_DECLARE (Timer_Service_3)
