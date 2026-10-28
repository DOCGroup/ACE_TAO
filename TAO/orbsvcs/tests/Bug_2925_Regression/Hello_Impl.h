
#include "HelloS.h"

class MessageLog
{
public:
  MessageLog (int num);
  ~MessageLog ();

  MessageLog (MessageLog const &) = delete;
  MessageLog (MessageLog &&) = delete;
  MessageLog &operator= (MessageLog const &) = delete;
  MessageLog &operator= (MessageLog &&) = delete;

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
  Hello_Impl (CORBA::ORB_ptr orb, MessageLog * logger);

  Hello_Impl (Hello_Impl const &) = delete;
  Hello_Impl (Hello_Impl &&) = delete;
  Hello_Impl &operator= (Hello_Impl const &) = delete;
  Hello_Impl &operator= (Hello_Impl &&) = delete;

  // The skeleton methods
  void say_hello (CORBA::Short count) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;

  MessageLog * logger_;
};
