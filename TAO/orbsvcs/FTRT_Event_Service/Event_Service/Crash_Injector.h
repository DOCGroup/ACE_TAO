// $Id$
#ifndef CRASH_INJECTOR_H
#define CRASH_INJECTOR_H
#include "ace/Service_Config.h"

#include "ace/Service_Object.h"
#include "ace/SString.h"

#define CRASH_Export
#define CRASH

class Crash_Injector : public ACE_Service_Object
{
public:
  Crash_Injector();
  ~Crash_Injector();

  static Crash_Injector* instance();

  virtual int init (int argc, ACE_TCHAR* []);

  void check_on_receive_request(const char* operation);
  void check_on_sending_reply(const char* operation);

  bool work_done() { return done_; }
private:
  void crash_timeout();

  enum CRASH_TYPE {
    CRASH_IMMEDIATELY,
    CRASH_TIMEOUT,
    CRASH_AFTER_REPLY
  };

  CRASH_TYPE crash_type_;
  ACE_CString operation_;
  int time_to_crash_in_ms_;
  int crash_after_number_of_invocation_;
  int num_of_invocation_;
  bool done_;
};


ACE_STATIC_SVC_DECLARE_EXPORT (CRASH, Crash_Injector)
ACE_STATIC_SVC_REQUIRE(Crash_Injector)
ACE_FACTORY_DECLARE (CRASH, Crash_Injector)

#endif
