/// @file Connection_Cache.cpp

#include "ace/INet/ConnectionCache.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include <new>
#include <stdexcept>

#if defined (ACE_HAS_THREADS)
# include "ace/OS_NS_stdlib.h"
# include "ace/OS_NS_sys_time.h"
# include "ace/Semaphore.h"
# include <thread>

thread_local ACE_Semaphore *lookup_observer {};
#endif

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
#if defined (ACE_HAS_THREADS)
    if (lookup_observer != nullptr)
      {
        lookup_observer->release ();
        lookup_observer = nullptr;
      }
#endif
    return dynamic_cast<Test_Key const *> (std::addressof (other)) != nullptr;
  }
};

class Test_Connection : public ACE::INet::ConnectionHolder {};

class Failing_Key : public Test_Key
{
public:
  ACE::INet::ConnectionKey *duplicate () const override { return nullptr; }
};

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

#if defined (ACE_HAS_THREADS)
class Blocking_Factory : public ACE::INet::ConnectionFactory
{
public:
  Blocking_Factory (ACE_Semaphore &entered, ACE_Semaphore &proceed)
    : entered_ (entered), proceed_ (proceed) {}

  ACE::INet::ConnectionHolder *create_connection (ACE::INet::ConnectionKey const &) const override
  {
    entered_.release ();
    proceed_.acquire ();
    return nullptr;
  }

private:
  ACE_Semaphore &entered_;
  ACE_Semaphore &proceed_;
};
#endif

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  Test_Key const key;
  Failing_Key const failing_key;
  Test_Factory const fail (Test_Factory::Result::fail);
  Test_Factory const exception (Test_Factory::Result::exception);
  Test_Factory const success (Test_Factory::Result::success);
  ACE::INet::ConnectionCache cache;
  ACE::INet::ConnectionHolder *connection {};
  bool copy_failed {};
  try
    {
      cache.claim_connection (failing_key, connection, success, false);
    }
  catch (std::bad_alloc const &)
    {
      copy_failed = true;
    }
  if (!copy_failed || cache.has_connection (key))
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cache key duplication failure was not reported\n")), 1);
    }
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
      || connection == nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Could not create connection after factory failure\n")), 1);
    }
  bool assignment_failed {};
  try
    {
      cache.release_connection (failing_key, connection);
    }
  catch (std::bad_alloc const &)
    {
      assignment_failed = true;
    }
  ACE::INet::ConnectionHolder *other {};
  if (!assignment_failed || cache.claim_connection (key, other, success, false)
      || !cache.release_connection (key, connection)
      || !cache.claim_connection (key, connection, success, false)
      || !cache.close_connection (key, connection))
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Could not retry connection after factory failure\n")), 1);
    }
#if defined (ACE_HAS_THREADS)
  ACE::INet::ConnectionCache concurrent_cache;
  ACE_Semaphore factory_entered (0);
  ACE_Semaphore finish_factory (0);
  ACE_Semaphore waiter_lookup (0);
  ACE_Semaphore waiter_finished (0);
  Blocking_Factory const blocked (factory_entered, finish_factory);
  bool creator_result {};
  bool waiter_result {};
  std::thread creator ([&] ()
    {
      ACE::INet::ConnectionHolder *created {};
      creator_result = concurrent_cache.claim_connection (key, created, blocked);
    });
  ACE_Time_Value deadline = ACE_OS::gettimeofday () + ACE_Time_Value (5);
  if (factory_entered.acquire (deadline) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Factory was not called\n")));
      ACE_OS::exit (1);
    }
  std::thread waiter ([&] ()
    {
      lookup_observer = std::addressof (waiter_lookup);
      ACE::INet::ConnectionHolder *claimed {};
      waiter_result = concurrent_cache.claim_connection (key, claimed, success)
        && concurrent_cache.close_connection (key, claimed);
      waiter_finished.release ();
    });
  deadline = ACE_OS::gettimeofday () + ACE_Time_Value (5);
  if (waiter_lookup.acquire (deadline) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Waiting caller did not reach the cache\n")));
      ACE_OS::exit (1);
    }
  finish_factory.release ();
  creator.join ();
  deadline = ACE_OS::gettimeofday () + ACE_Time_Value (5);
  if (waiter_finished.acquire (deadline) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Waiting caller was not awakened\n")));
      ACE_OS::exit (1);
    }
  waiter.join ();
  if (creator_result || !waiter_result)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Waiting caller could not retry after factory failure\n")), 1);
    }
#endif
  return 0;
}
