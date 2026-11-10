#include "HelloS.h"
#include "ace/Mutex.h"
#include <cstddef>
#include <vector>

class MessageLog
{
public:
  explicit MessageLog (int num);

  MessageLog (MessageLog const &) = delete;
  MessageLog (MessageLog &&) = delete;
  MessageLog &operator= (MessageLog const &) = delete;
  MessageLog &operator= (MessageLog &&) = delete;

  void register_message_recv (int message_num);
  int report_statistics ();

private:
  std::vector<int> received_;
  ACE_Mutex mutex_;
};

class Hello_Impl
  : public virtual POA_Test::Hello
{
public:
  Hello_Impl (CORBA::ORB_ptr orb, MessageLog &logger);

  Hello_Impl (Hello_Impl const &) = delete;
  Hello_Impl (Hello_Impl &&) = delete;
  Hello_Impl &operator= (Hello_Impl const &) = delete;
  Hello_Impl &operator= (Hello_Impl &&) = delete;

  void say_hello (CORBA::Short count) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;

  MessageLog &logger_;
};
