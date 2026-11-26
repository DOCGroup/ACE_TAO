/// @file Connection_Cache.cpp

#include "ace/INet/ConnectionCache.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include <stdexcept>

class Test_Key : public ACE::INet::ConnectionKey
{
public:
  Test_Key () = default;
  Test_Key (Test_Key const &) = default;
  u_long hash () const override { return 1; }
  ACE::INet::ConnectionKey *duplicate () const override { return new Test_Key (*this); }
protected:
  bool equal (ACE::INet::ConnectionKey const &other) const override
  {
    return dynamic_cast<Test_Key const *> (std::addressof (other)) != nullptr;
  }
};

class Test_Connection : public ACE::INet::ConnectionHolder {};

class Test_Factory : public ACE::INet::ConnectionFactory
{
public:
  enum class Result { fail, exception, success };
  explicit Test_Factory (Result result) : result_ (result) {}
  ACE::INet::ConnectionHolder *create_connection (ACE::INet::ConnectionKey const &) const override
  {
    if (result_ == Result::exception)
      {
        throw std::runtime_error ("test factory failure");
      }
    return result_ == Result::success ? new Test_Connection : nullptr;
  }
private:
  Result result_;
};

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  Test_Key const key;
  Test_Factory const fail (Test_Factory::Result::fail);
  Test_Factory const exception (Test_Factory::Result::exception);
  Test_Factory const success (Test_Factory::Result::success);
  ACE::INet::ConnectionCache cache;
  ACE::INet::ConnectionHolder *connection {};
  if (cache.claim_connection (key, connection, fail, false) || cache.has_connection (key))
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Failed factory left a cache entry\n")), 1);
    }
  bool caught {};
  try
    {
      cache.claim_connection (key, connection, exception, false);
    }
  catch (std::runtime_error const &)
    {
      caught = true;
    }
  if (!caught || cache.has_connection (key))
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Throwing factory left a cache entry\n")), 1);
    }
  if (!cache.claim_connection (key, connection, success, false)
      || connection == nullptr || !cache.release_connection (key, connection)
      || !cache.claim_connection (key, connection, success, false)
      || !cache.close_connection (key, connection))
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Could not retry connection after factory failure\n")), 1);
    }
  return 0;
}
