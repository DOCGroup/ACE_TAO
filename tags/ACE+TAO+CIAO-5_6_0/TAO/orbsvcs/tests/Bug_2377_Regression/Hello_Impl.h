//
// $Id$
//

#include "HelloS.h"
#include "ace/Mutex.h"

class MessageLog
{
public:
  MessageLog (int num, bool sprs = false);
  ~MessageLog ();

  void register_message_send (int message_num);
  void register_message_recv (int message_num);
  int report_statistics ();

private:
  int *sent_;
  int *rcvd_;
  int expected_;
  int overdoseS_;
  int overdoseR_;
  bool supress_;
  ACE_Mutex mutex_;
};

class Hello_Impl
  : public virtual POA_Test::Hello
{
public:
  // Constructor
  Hello_Impl (CORBA::ORB_ptr orb, MessageLog *log);

  // The skeleton methods
  virtual void say_hello (CORBA::Short count);

  virtual void shutdown ();

private:
  CORBA::ORB_var orb_;

  MessageLog *logger_;
};
