// -*- C++ -*-
#include "ace/svc_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Reactor.h"
#include "ace/Service_Object.h"
#include <string>

class ACE_Svc_Export Timer_Service_1 : public ACE_Service_Object
{
  /// Demonstrates a simple timer service that can be configured statically.
public:
  Timer_Service_1 ();
  Timer_Service_1 (Timer_Service_1 const &) = delete;
  Timer_Service_1 (Timer_Service_1 &&) = delete;
  Timer_Service_1 &operator= (Timer_Service_1 const &) = delete;
  Timer_Service_1 &operator= (Timer_Service_1 &&) = delete;

  /// Initialization hook.
  int init (int argc, ACE_TCHAR *argv[]) override;

  /// Timeout hook.
  int handle_timeout (const ACE_Time_Value &,
                      const void *) override;

  /// Close hook.
  int handle_close (ACE_HANDLE, ACE_Reactor_Mask) override;

protected:
  /// Identifies this service instance.
  std::basic_string<ACE_TCHAR> name_ { ACE_TEXT ("Timer_Service_1") };

private:
  /// Seconds between timeouts.
  static constexpr int TIMEOUT { 4 };
  /// Maximum number of timeouts.
  static constexpr int MAX_TIMEOUTS { 10 };

  /// Maximum number of timeouts before shutting down the test.
  int max_timeouts_ { MAX_TIMEOUTS };

  /// Current number of timeouts.
  int cur_timeouts_ {};
};

class ACE_Svc_Export Timer_Service_2 : public Timer_Service_1
{
public:
  Timer_Service_2 ();
  Timer_Service_2 (Timer_Service_2 const &) = delete;
  Timer_Service_2 (Timer_Service_2 &&) = delete;
  Timer_Service_2 &operator= (Timer_Service_2 const &) = delete;
  Timer_Service_2 &operator= (Timer_Service_2 &&) = delete;
};

class ACE_Svc_Export Timer_Service_3 : public Timer_Service_1
{
public:
  Timer_Service_3 ();
  Timer_Service_3 (Timer_Service_3 const &) = delete;
  Timer_Service_3 (Timer_Service_3 &&) = delete;
  Timer_Service_3 &operator= (Timer_Service_3 const &) = delete;
  Timer_Service_3 &operator= (Timer_Service_3 &&) = delete;
};

// Declare both static and dynamic services.
ACE_STATIC_SVC_DECLARE_EXPORT (ACE_Svc, Timer_Service_1)
ACE_SVC_FACTORY_DECLARE (Timer_Service_1)
ACE_SVC_FACTORY_DECLARE (Timer_Service_2)
ACE_SVC_FACTORY_DECLARE (Timer_Service_3)
