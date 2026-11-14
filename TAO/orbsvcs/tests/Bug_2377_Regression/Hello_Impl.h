#include "HelloS.h"
#include "ace/Mutex.h"
#include <vector>

class MessageLog
{
public:
  explicit MessageLog (int expected);

  MessageLog (MessageLog const &) = delete;
  MessageLog (MessageLog &&) = delete;
  MessageLog &operator= (MessageLog const &) = delete;
  MessageLog &operator= (MessageLog &&) = delete;

  void register_message_send (int const message_num);
  void register_message_recv (int const message_num);
  int report_statistics () const;

private:
  std::vector<int> sent_;
  std::vector<int> rcvd_;
  int const expected_;
  int overdose_sent_;
  int overdose_received_;
  ACE_Mutex mutex_;
};

class Hello_Impl
  : public virtual POA_Test::Hello
{
public:
  Hello_Impl (CORBA::ORB_ptr orb, MessageLog &log);

  Hello_Impl (Hello_Impl const &) = delete;
  Hello_Impl (Hello_Impl &&) = delete;
  Hello_Impl &operator= (Hello_Impl const &) = delete;
  Hello_Impl &operator= (Hello_Impl &&) = delete;

  void say_hello (CORBA::Short count) override;

  void shutdown () override;

private:
  CORBA::ORB_var const orb_;

  MessageLog &logger_;
};
