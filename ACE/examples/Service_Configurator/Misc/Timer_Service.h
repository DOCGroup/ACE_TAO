// -*- C++ -*-
#include "ace/svc_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Reactor.h"
#include "ace/Service_Object.h"


class ACE_Svc_Export Timer_Service_1 : public ACE_Service_Object
{
  // = TITLE
  //   Demonstrates a simple timer service that can be configured
  //   statically.
public:
  Timer_Service_1 ();
  // Default constructor.

  int init (int argc, ACE_TCHAR *argv[]) override;
  // Initialization hook.

  int handle_timeout (const ACE_Time_Value &,
                      const void *) override;
  // Timeout hook.

  int handle_close (ACE_HANDLE, ACE_Reactor_Mask) override;
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

  int max_timeouts_ = MAX_TIMEOUTS;
  // Maximum number of timeouts before shutting down the test.

  int cur_timeouts_ = 0;
  // Current number of timeouts.
};

class ACE_Svc_Export Timer_Service_2 : public Timer_Service_1
{
public:
  Timer_Service_2 ();
  // Default constructor.
};

class ACE_Svc_Export Timer_Service_3 : public Timer_Service_1
{
public:
  Timer_Service_3 ();
  // Default constructor.
};

// Declare both static and dynamic services.
ACE_STATIC_SVC_DECLARE_EXPORT (ACE_Svc, Timer_Service_1)
ACE_SVC_FACTORY_DECLARE (Timer_Service_1)
ACE_SVC_FACTORY_DECLARE (Timer_Service_2)
ACE_SVC_FACTORY_DECLARE (Timer_Service_3)
