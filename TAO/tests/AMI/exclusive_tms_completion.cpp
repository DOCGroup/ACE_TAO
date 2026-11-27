#include "tao/Exclusive_TMS.h"
#include "tao/ORB_Core.h"
#include "tao/Reply_Dispatcher.h"
#include "tao/Transport.h"
#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"

#include <memory>

class Test_Transport final : public TAO_Transport
{
public:
  explicit Test_Transport (TAO_ORB_Core *orb_core)
    : TAO_Transport (0, orb_core)
  {
  }

  ~Test_Transport () override = default;

  ssize_t send (iovec *,
                int,
                size_t &,
                ACE_Time_Value const * = nullptr) override
  {
    return 0;
  }

  ssize_t recv (char *,
                size_t,
                ACE_Time_Value const * = nullptr) override
  {
    return 0;
  }

  ACE_Event_Handler *event_handler_i () override
  {
    return nullptr;
  }

  int send_message (TAO_OutputCDR &,
                    TAO_Stub * = nullptr,
                    TAO_ServerRequest * = nullptr,
                    TAO_Message_Semantics = TAO_Message_Semantics (),
                    ACE_Time_Value * = nullptr) override
  {
    return 0;
  }

  Test_Transport (Test_Transport const &) = delete;
  Test_Transport (Test_Transport &&) = delete;
  Test_Transport &operator= (Test_Transport const &) = delete;
  Test_Transport &operator= (Test_Transport &&) = delete;

protected:
  TAO_Connection_Handler *connection_handler_i () override
  {
    return nullptr;
  }

  int send_request (TAO_Stub *,
                    TAO_ORB_Core *,
                    TAO_OutputCDR &,
                    TAO_Message_Semantics,
                    ACE_Time_Value *) override
  {
    return 0;
  }
};

class Test_Exclusive_TMS final : public TAO_Exclusive_TMS
{
public:
  explicit Test_Exclusive_TMS (TAO_Transport *transport)
    : TAO_Exclusive_TMS (transport)
  {
  }

  ~Test_Exclusive_TMS () override = default;

  bool request_sent () const
  {
    return this->request_sent_;
  }

  bool transport_idle () const
  {
    return this->transport_idle_;
  }

  Test_Exclusive_TMS (Test_Exclusive_TMS const &) = delete;
  Test_Exclusive_TMS (Test_Exclusive_TMS &&) = delete;
  Test_Exclusive_TMS &operator= (Test_Exclusive_TMS const &) = delete;
  Test_Exclusive_TMS &operator= (Test_Exclusive_TMS &&) = delete;
};

class Test_Reply_Dispatcher final : public TAO_Reply_Dispatcher
{
public:
  Test_Reply_Dispatcher () = default;
  ~Test_Reply_Dispatcher () override = default;

  int dispatch_reply (TAO_Pluggable_Reply_Params &) override
  {
    return 1;
  }

  void reply_timed_out () override
  {
  }

  void connection_closed () override
  {
  }

  Test_Reply_Dispatcher (Test_Reply_Dispatcher const &) = delete;
  Test_Reply_Dispatcher (Test_Reply_Dispatcher &&) = delete;
  Test_Reply_Dispatcher &operator= (Test_Reply_Dispatcher const &) = delete;
  Test_Reply_Dispatcher &operator= (Test_Reply_Dispatcher &&) = delete;
};

static ACE_Intrusive_Auto_Ptr<TAO_Reply_Dispatcher>
make_reply_dispatcher ()
{
  return ACE_Intrusive_Auto_Ptr<TAO_Reply_Dispatcher> (
    new Test_Reply_Dispatcher,
    false);
}

static void
test_reply_before_send (TAO_ORB_Core *orb_core)
{
  Test_Transport transport (orb_core);
  Test_Exclusive_TMS strategy (std::addressof (transport));
  CORBA::ULong const request_id = strategy.request_id ();
  ACE_Intrusive_Auto_Ptr<TAO_Reply_Dispatcher> const rd =
    make_reply_dispatcher ();

  ACE_TEST_ASSERT (strategy.bind_dispatcher (request_id, rd) == 0);
  ACE_TEST_ASSERT (strategy.unbind_dispatcher (request_id) == 0);

  // Simulate the AMI reply path while the sending thread is still active.
  ACE_TEST_ASSERT (strategy.idle_after_reply ());
  ACE_TEST_ASSERT (!strategy.request_sent ());
  ACE_TEST_ASSERT (!strategy.transport_idle ());

  ACE_TEST_ASSERT (strategy.idle_after_send ());
  ACE_TEST_ASSERT (strategy.request_sent ());
  ACE_TEST_ASSERT (strategy.transport_idle ());
  ACE_TEST_ASSERT (!strategy.idle_after_send ());
}

static void
test_send_before_reply (TAO_ORB_Core *orb_core)
{
  Test_Transport transport (orb_core);
  Test_Exclusive_TMS strategy (std::addressof (transport));
  CORBA::ULong const request_id = strategy.request_id ();
  ACE_Intrusive_Auto_Ptr<TAO_Reply_Dispatcher> const rd =
    make_reply_dispatcher ();

  ACE_TEST_ASSERT (strategy.bind_dispatcher (request_id, rd) == 0);

  ACE_TEST_ASSERT (!strategy.idle_after_send ());
  ACE_TEST_ASSERT (strategy.request_sent ());
  ACE_TEST_ASSERT (!strategy.transport_idle ());

  ACE_TEST_ASSERT (strategy.unbind_dispatcher (request_id) == 0);
  ACE_TEST_ASSERT (strategy.idle_after_reply ());
  ACE_TEST_ASSERT (strategy.transport_idle ());
  ACE_TEST_ASSERT (!strategy.idle_after_send ());
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      test_reply_before_send (orb->orb_core ());
      test_send_before_reply (orb->orb_core ());

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exclusive_TMS_Completion_Test");
      return 1;
    }

  return 0;
}
