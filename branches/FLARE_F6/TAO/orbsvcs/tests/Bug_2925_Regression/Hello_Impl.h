//
// $Id$
//

#include "HelloS.h"

class MessageLog
{
public:
  MessageLog (int num);
  ~MessageLog ();

  void register_message_recv (int message_num);
  int report_statistics ();

private:
  int *rcvd_;
  int expected_;
};

class Hello_Impl
  : public virtual POA_Test::Hello
{
public:
  // Constructor
  Hello_Impl (CORBA::ORB_ptr orb, MessageLog* logger);

  // The skeleton methods
  virtual void say_hello (CORBA::Short count);

  virtual void shutdown ();

private:
  CORBA::ORB_var orb_;

  MessageLog* logger_;
};
