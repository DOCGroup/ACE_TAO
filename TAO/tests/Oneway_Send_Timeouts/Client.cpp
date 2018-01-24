#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"

#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_strings.h"

#include "Client.h"
#include "TestC.h"

Client::Client (int argc, ACE_TCHAR* argv[])
  : init_ (false), one_way_test_ (false)
{
  if (this->init (argc, argv)) {
    init_ = true;
  }
}

Client::~Client ()
{
  if (!CORBA::is_nil (orb_.in())) {
    orb_->shutdown (1);
    orb_->destroy ();
    orb_ = CORBA::ORB::_nil();
  }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) ~Client>\n"));
}

bool
Client::init (int argc, ACE_TCHAR* argv[])
{
  try {
    orb_ = CORBA::ORB_init (argc, argv, "Client");
    if (CORBA::is_nil (orb_.in())) {
      ACE_ERROR ((LM_ERROR, "Client::init> ORB initialization failed.\n"));
      return false;
    }

    if (!this->parse_args (argc, argv)) {
      return false;
    }

    // Timeout
    TimeBase::TimeT timeout = 5 * 1000000; // 5 seconds
    CORBA::Any any_object;
    any_object <<= timeout;

    CORBA::PolicyList policy_list (2);
    policy_list.length (2);
    policy_list[0] =
      orb_->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                           any_object);


    // Timeout with SYNC_SCOPE SYNC_WITH_TRANSPORT
    Messaging::SyncScope sync_with_transport = Messaging::SYNC_WITH_TRANSPORT;
    CORBA::Any sync_with_transport_any;
    sync_with_transport_any <<= sync_with_transport;
    policy_list[1] = orb_->create_policy
      (Messaging::SYNC_SCOPE_POLICY_TYPE, sync_with_transport_any);
    CORBA::Object_var obj = test_obj_->_set_policy_overrides
      (policy_list, CORBA::SET_OVERRIDE);
    test_obj_transport_timeout_ = Test::_narrow (obj.in ());
    policy_list[1]->destroy ();



    // Timeout with SYNC_SCOPE SYNC_NONE
    Messaging::SyncScope sync_none = Messaging::SYNC_NONE;
    CORBA::Any sync_none_any;
    sync_none_any <<= sync_none;
    policy_list[1] = orb_->create_policy
      (Messaging::SYNC_SCOPE_POLICY_TYPE, sync_none_any);
    // Apply the policy at the object level
    obj = test_obj_->_set_policy_overrides
      (policy_list, CORBA::SET_OVERRIDE);
    test_obj_none_timeout_ = Test::_narrow (obj.in ());
    policy_list[1]->destroy ();


    // Timeout with TAO specific SYNC_SCOPE SYNC_EAGER_BUFFERING
    //Messaging::SyncScope eager_buffering = TAO::SYNC_EAGER_BUFFERING;
    Messaging::SyncScope eager_buffering = Messaging::SYNC_NONE;
    CORBA::Any eager_any;
    eager_any <<= eager_buffering;
    policy_list[1] = orb_->create_policy
      (Messaging::SYNC_SCOPE_POLICY_TYPE, eager_any);
    obj = test_obj_->_set_policy_overrides
      (policy_list, CORBA::SET_OVERRIDE);
    test_obj_eager_timeout_ = Test::_narrow (obj.in ());
    policy_list[1]->destroy ();


    // Timeout with TAO specific SYNC_SCOPE SYNC_DELAYED_BUFFERING
    Messaging::SyncScope delayed_buffering = TAO::SYNC_DELAYED_BUFFERING;
    CORBA::Any delayed_any;
    delayed_any <<= delayed_buffering;
    policy_list[1] = orb_->create_policy
      (Messaging::SYNC_SCOPE_POLICY_TYPE, delayed_any);
    obj = test_obj_->_set_policy_overrides
      (policy_list, CORBA::SET_OVERRIDE);
    test_obj_delayed_timeout_ = Test::_narrow (obj.in ());
    policy_list[1]->destroy ();


    // Timeout with default SYNC_SCOPE SYNC_WITH_SERVER
    Messaging::SyncScope sync_with_server = Messaging::SYNC_WITH_SERVER;
    CORBA::Any sync_with_server_any;
    sync_with_server_any <<= sync_with_server;
    policy_list[1] = orb_->create_policy
      (Messaging::SYNC_SCOPE_POLICY_TYPE, sync_with_server_any);
    obj = test_obj_->_set_policy_overrides
      (policy_list, CORBA::SET_OVERRIDE);
    test_obj_server_timeout_ = Test::_narrow (obj.in ());
    policy_list[1]->destroy ();


    // Timeout with default SYNC_SCOPE (SYNC_WITH_TARGET)
    Messaging::SyncScope sync_with_target = Messaging::SYNC_WITH_TARGET;
    CORBA::Any sync_with_target_any;
    sync_with_target_any <<= sync_with_target;
    policy_list[1] = orb_->create_policy
      (Messaging::SYNC_SCOPE_POLICY_TYPE, sync_with_target_any);
    // Apply the policy at the object level
    obj = test_obj_->_set_policy_overrides
      (policy_list, CORBA::SET_OVERRIDE);
    test_obj_target_timeout_ = Test::_narrow (obj.in ());

    policy_list[0]->destroy ();
    policy_list[1]->destroy ();
    policy_list.length(0);

  }
  catch( CORBA::Exception& ex) {
    ACE_ERROR ((LM_ERROR, "(%P|%t) Client::Init> Caught CORBA::Exception %s"
                , ex._info().c_str()));
    return false;
  }

  return true;
}

bool
Client::parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:k:1::f:"), 0);
  int c;
  std::string test_ior;
  std::string back_ior;
  std::string flush_strategy ("lf");

  while ((c = get_opts ()) != -1) {
    switch (c)
      {
      case '1':
        one_way_test_ = true;
        break;
      case 'k':
        test_ior = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
        break;
      case 'b':
        back_ior = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
        break;
      case 'f':
        flush_strategy = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR, "Invalid option \'-%c\'\n", c)
                          , false);
      }
  }

  CORBA::Object_var obj = orb_->string_to_object (test_ior.c_str());
  test_obj_ = Test::_narrow (obj.in ());

  obj = orb_->string_to_object (back_ior.c_str());
  management_ = Test::_narrow (obj.in ());

  if (ACE_OS::strcasecmp ("lf", flush_strategy.c_str()) == 0) {
    flush_strategy_ = LF;
  }
  else if (ACE_OS::strcasecmp ("blocking", flush_strategy.c_str()) == 0) {
    flush_strategy_ = BLOCKING;
  }
  else if (ACE_OS::strcasecmp ("reactive", flush_strategy.c_str()) == 0) {
    flush_strategy_ = REACTIVE;
  }

  return true;
}

bool
Client::run ()
{
  bool status = true;

  if (CORBA::is_nil (orb_.in())) {
      ACE_ERROR ((LM_ERROR, "Client::run> nil ORB found.\n"));
      return false;
    }

  try {
    try {
      if (one_way_test_)
        {
          if (!this->test_oneway_timeout (true)) {
            ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: Client::run> "
                        "test_oneway_timeout failed.\n"));
            status = false;
          }
        }

      // now run same test without the transport flooded.
      if (!this->test_oneway_timeout (false)) {
        ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: Client::run> "
                    "test_oneway_timeout 2 failed.\n"));
        status = false;
      }

      test_obj_->shutdown ();
    }
    catch( CORBA::Exception& ex) {
      management_->unsleep (); // remote side could be asleep
      ACE_ERROR ((LM_ERROR, "(%P|%t) Client::run> Caught during test logic CORBA::Exception %s"
                  , ex._info().c_str()));
      status = false;
    }
  }
  catch( CORBA::Exception& ex) {
    ACE_ERROR ((LM_ERROR, "(%P|%t) Client::run> Caught during test shutdown CORBA::Exception %s"
                , ex._info().c_str()));
    status = false;
  }

  return status;
}

bool
Client::test_oneway_timeout (bool flood)
{
  bool status = true;

  ACE_Auto_Array_Ptr<char> tmp (new char [6000000]);
  char* msg = tmp.get();

  ACE_OS::memset (msg,'A',5999999);
  msg[5999999] = 0;

  test_obj_->dummy_two_way (); // connection establishment

  ACE_Time_Value tv (0);
  if (flood && !this->flood_connection(tv)) {
    ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> flooding failed.\n"));
  }

  // Timeout with SYNC_SCOPE SYNC_NONE
  try {
    std::string scope_name ("SYNC_NONE");
    ACE_OS::strncpy (msg, scope_name.c_str(), scope_name.length());
    test_obj_none_timeout_->dummy_one_way (msg);

    if (flood && flush_strategy_ == BLOCKING) {
      // block flushing gives a oneway SYNCH_WITH_TRANSPORT semantics
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: A Timeout was expected for SYNC_NONE.\n"));
    }
    else {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) As expected no Timeout received for SYNC_NONE\n"));
    }
  }
  catch (const CORBA::TIMEOUT&) {
    if (flood && flush_strategy_ == BLOCKING) {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) As expected a timeout was received for SYNC_NONE.\n"));
    }
    else {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Unexpected "
                  "timeout exception with synch scope SYNC_NONE.\n"));
      status = false;
    }
  }


  // Timeout with TAO specific SYNC_SCOPE SYNC_EAGER_BUFFERING
  try {
    std::string scope_name ("SYNC_EAGER_BUFFERING");
    ACE_OS::strncpy (msg, scope_name.c_str(), scope_name.length());

    /* BLOCKed flushing has SYNCH_WITH_TRANSPORT semantics. With flooding turned on
        you would have received a TIMEOUT in the previous test (SYNC_NONE). Even without
        flooding there is a chance to get back a TIMEOUT. The TIMEOUT has the side-effect
        closing out the connection. Therefore when flush_strategy is set to BLOCK we want
        to re-establish connection before each test. With flooding turned on we need
        to first unsleep the test server, re-establish connection and put it back to sleep.
        With flooding=0 we simple re-establish connection. This trick is performed at beginning
        of every test for flush_strategy == BLOCKING.
     */
    if (flush_strategy_ == BLOCKING) {
      if (flood) {
        management_->unsleep ();
        test_obj_->sleep (0, 0); // rebuild connection and put server thread to sleep
      }
      else {
        // else simply re-establish connection
        test_obj_->dummy_two_way ();
      }
    }

    test_obj_eager_timeout_->dummy_one_way (msg);

    if (flood && flush_strategy_ == BLOCKING) {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: A Timeout was expected for SYNC_EAGER_BUFFERING\n"));
    }
    else {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) As expected no Timeout received for SYNC_EAGER_BUFFERING\n"));
    }
  }
  catch (const CORBA::TIMEOUT&) {
    if (flood && flush_strategy_ == BLOCKING) {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) As expected a timeout was received for SYNC_EAGER_BUFFERING\n"));
    }
    else {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Unexpected "
                  "timeout exception with synch scope SYNC_EAGER_BUFFERING.\n"));
      status = false;
    }
  }

  // Timeout with TAO specific SYNC_SCOPE SYNC_DELAYED_BUFFERING
  try {
    std::string scope_name ("SYNC_DELAYED_BUFFERING");
    ACE_OS::strncpy (msg, scope_name.c_str(), scope_name.length());

    if (flush_strategy_ == BLOCKING) {
      if (flood) {
        management_->unsleep ();
        test_obj_->sleep (0, 0);
      }
      else {
        test_obj_->dummy_two_way ();
      }
    }

    test_obj_delayed_timeout_->dummy_one_way (msg);

    if (flood && flush_strategy_ == BLOCKING) {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: A Timeout was expected for SYNC_DELAYED_BUFFERING\n"));
    }
    else {
      test_obj_delayed_timeout_->dummy_one_way ("SYNC_DELAYED_BUFFERING");
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) As expected no Timeout received for SYNC_DELAYED_BUFFERING\n"));
    }
  }
  catch (const CORBA::TIMEOUT&) {
    if (flood && flush_strategy_ == BLOCKING) {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Expected timeout received for SYNC_DELAYED_BUFFERING\n"));
    }
    else {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Unexpected "
                "timeout exception with synch scope SYNC_DELAYED_BUFFERING.\n"));
      status = false;
    }
  }

  /* Cleanup queue before the synchronous tests. We don't want the test
      results affected by leftovers from previous runs.
   */
  ACE_Time_Value tv_tmp (1);
  orb_->run (tv_tmp);

  // Timeout with SYNC_SCOPE SYNC_WITH_TRANSPORT
  try {
    std::string scope_name ("SYNC_WITH_TRANSPORT");
    ACE_OS::strncpy (msg, scope_name.c_str(), scope_name.length());

    if (flush_strategy_ == BLOCKING) {
      if (flood) {
        management_->unsleep ();
        test_obj_->sleep (0, 0);
      }
      else {
        test_obj_->dummy_two_way ();
      }
    }

    test_obj_transport_timeout_->dummy_one_way (msg);

    if (flood) {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Expected "
                  "timeout not received for synch scope SYNC_WITH_TRANSPORT.\n"
                  ));
      status = false;
    }
    else {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) As expected no Timeout received for SYNC_WITH_TRANSPORT\n"));
    }
  }
  catch (const CORBA::TIMEOUT&) {
    if (flood) {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Expected Timeout received for SYNC_WITH_TRANSPORT\n"));
    }
    else {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Unexpected "
                  "timeout exception with synch scope SYNC_WITH_TRANSPORT.\n"));
      status = false;
    }
  }

// Timeout with default SYNC_SCOPE SYNC_WITH_SERVER
  try {
    std::string scope_name ("SYNC_WITH_SERVER");
    ACE_OS::strncpy (msg, scope_name.c_str(), scope_name.length());

    if (flush_strategy_ == BLOCKING) {
      if (flood) {
        management_->unsleep ();
        test_obj_->sleep (0, 0);
      }
      else {
        test_obj_->dummy_two_way ();
      }
    }

    test_obj_server_timeout_->dummy_one_way (msg);

    if (flood) {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Expected "
                  "timeout not received for SYNC_SCOPE SYNC_WITH_SERVER.\n"));
      status = false;
    }
    else {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) As expected no Timeout received for SYNC_WITH_SERVER\n"));
    }
  }
  catch (const CORBA::TIMEOUT&) {
    if (flood) {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Expected Timeout received for SYNC_WITH_SERVER\n"));
    }
    else {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Unexpected "
                  "timeout exception with synch scope SYNC_WITH_SERVER.\n"));
      status = false;
    }
  }


  // Timeout with default SYNC_SCOPE (SYNC_WITH_TARGET)
  try {
    std::string scope_name ("SYNC_WITH_TARGET");
    ACE_OS::strncpy (msg, scope_name.c_str(), scope_name.length());

    if (flush_strategy_ == BLOCKING) {
      if (flood) {
        management_->unsleep ();
        test_obj_->sleep (0, 0);
      }
      else {
        test_obj_->dummy_two_way ();
      }
    }

    test_obj_target_timeout_->dummy_one_way (msg);

    if (flood) {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Expected "
                  "timeout not received for SYNC_SCOPE SYNC_WITH_TARGET.\n"));
      status = false;
    }
    else {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) As expected no Timeout received for SYNC_WITH_TARGET\n"));
    }
  }
  catch (const CORBA::TIMEOUT&) {
    if (flood) {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Expected Timeout received for SYNC_WITH_TARGET\n"));
    }
    else {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Unexpected "
                  "timeout exception with synch scope SYNC_WITH_TARGET.\n"));
      status = false;
    }
  }

  if (flood) {
    management_->unsleep ();
  }

  return status;
}

bool
Client::flood_connection (ACE_Time_Value& tv)
{
  // Block flushing currently blocks even on SYNC_DELAYED_BUFFERING
  //  so we can't use it to flood connections.

  // Set the policy value.
  // SYNC_DELAYED_BUFFERING is used to ensure that the tcp buffer gets filled before
  //  buffering starts.
  Messaging::SyncScope sync_scope = TAO::SYNC_DELAYED_BUFFERING;
  //Messaging::SyncScope sync_scope = Messaging::SYNC_NONE;
  CORBA::Any sync_scope_any;
  sync_scope_any <<= sync_scope;

  CORBA::PolicyList policy_list (1);
  policy_list.length (1);
  policy_list[0] = orb_->create_policy
    (Messaging::SYNC_SCOPE_POLICY_TYPE, sync_scope_any);
  // Apply the policy at the object level
  CORBA::Object_var obj = test_obj_->_set_policy_overrides
    (policy_list, CORBA::SET_OVERRIDE);
  Test_var mod_test_obj = Test::_narrow (obj.in ());

  policy_list[0]->destroy ();
  policy_list.length(0);

  ACE_Auto_Array_Ptr<char> tmp (new char [2000000]);
  char* msg = tmp.get();

  ACE_OS::memset (msg,'A',1999999);
  msg[1999999] = 0;

  test_obj_->sleep (static_cast<CORBA::Long>(tv.sec())
                    , static_cast<CORBA::Long>(tv.msec()));

  /* BLOCK flush startegy always has SYNC_WITH_TRANSPORT semantics.
      Trying to flood a BLOCKed flushing connection can lead to a TIMEOUT
      exception being thrown. This will close out the connection and
      the whole flooding attempt fails. Therefore in BLOCK flushing case
      don't attempt to flood (unless BLOCK flush accepts SYNC_WITH_TRANSPORT
      semantics).
   */
  if (flush_strategy_ != BLOCKING)
    {
      mod_test_obj->dummy_one_way (msg);

      // attempt again to flood connection.
      ACE_Time_Value tv_tmp (2);
      orb_->perform_work (tv_tmp);
    }

  return true;
}
