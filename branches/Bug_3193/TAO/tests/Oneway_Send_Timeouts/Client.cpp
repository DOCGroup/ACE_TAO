
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"

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
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) ~Client>\n"));
}

bool
Client::init (int argc, ACE_TCHAR* argv[])
{
  try {
    orb_ = CORBA::ORB_init (argc, argv, "Client");

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
  ACE_Get_Opt get_opts (argc, argv, "b:k:1::f:", 0);
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
        test_ior = get_opts.opt_arg ();
        break;
      case 'b':
        back_ior = get_opts.opt_arg ();
        break;
      case 'f':
        flush_strategy = get_opts.opt_arg ();
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

  try {
    if (one_way_test_)
      {
        ACE_Time_Value tv (0);
        bool flooded = false;
        bool test_condition_met = false;
        if (flush_strategy_ == BLOCKING) {
          test_obj_->sleep (tv.sec(), tv.msec());
          test_condition_met = true;
        }
        else {
          flooded = this->flood_connection (tv);
          if (flooded) {
            test_condition_met = true;
          }
        }

        if (test_condition_met) {
          if (!this->test_oneway_timeout (flooded, !flooded)) {
            ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: Client::run> "
                        "test_oneway_timeout failed.\n"));
            status = false;
          }

          management_->unsleep ();
        }

        // flush out the channel
        test_obj_->dummy_two_way ();

        // now run same test without the transport flooded.
        if (!this->test_oneway_timeout (false, false)) {
            ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: Client::run> "
                        "test_oneway_timeout 2 failed.\n"));
            status = false;
        }
      }

    test_obj_->shutdown ();

    orb_->shutdown (1);
    orb_->destroy ();
    orb_ = CORBA::ORB::_nil();
  }
  catch( CORBA::Exception& ex) {
    ACE_ERROR ((LM_ERROR, "(%P|%t) Client::run> Caught CORBA::Exception %s"
                , ex._info().c_str()));
    status = false;
  }

  return status;
}

bool
Client::test_oneway_timeout (bool flooded, bool simulate_flooding)
{
  bool status = true;

  std::auto_ptr<char> tmp;
  char* msg = new char [6000000];
  tmp.reset (msg);
  ACE_OS::memset (msg,'A',5999999);
  msg[5999999] = 0;


  // Timeout with SYNC_SCOPE SYNC_NONE
  try {
    if (simulate_flooding) {
      test_obj_none_timeout_->dummy_one_way (msg);
    }
    else {
      test_obj_none_timeout_->dummy_one_way ("SYNC_NONE");
    }
    if (!simulate_flooding) {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) No Timeout for SYNC_NONE\n"));
    }
    else {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: Timeout expected for SYNC_NONE.\n"));
    }
  }
  catch (CORBA::TIMEOUT&) {
    if (simulate_flooding) {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Expected timeout received for SYNC_NONE.\n"));
    }
    else {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Unexpected "
                  "timeout exception with synch scope SYNC_NONE.\n"));
      status = false;
    }
  }


  // Timeout with TAO specific SYNC_SCOPE SYNC_EAGER_BUFFERING
  try {
    if (simulate_flooding) {
      management_->unsleep ();
      test_obj_->sleep (0, 0);
      test_obj_eager_timeout_->dummy_one_way (msg);
    }
    else {
      test_obj_eager_timeout_->dummy_one_way ("SYNC_EAGER_BUFFERING");
    }
    if (!simulate_flooding) {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) No Timeout for SYNC_EAGER_BUFFERING\n"));
    }
    else {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) No Timeout for SYNC_EAGER_BUFFERING\n"));
    }
  }
  catch (CORBA::TIMEOUT&) {
    if (simulate_flooding) {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Expected timeout received for SYNC_EAGER_BUFFERING\n"));
    }
    else {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Unexpected "
                  "timeout exception with synch scope SYNC_EAGER_BUFFERING.\n"));
      status = false;
    }
  }

  // Timeout with TAO specific SYNC_SCOPE SYNC_DELAYED_BUFFERING
  try {
    if (simulate_flooding) {
      management_->unsleep ();
      test_obj_->sleep (0, 0);
      test_obj_delayed_timeout_->dummy_one_way (msg);
    }
    else {
      test_obj_delayed_timeout_->dummy_one_way ("SYNC_DELAYED_BUFFERING");
    }
    if (!simulate_flooding) {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) No Timeout for SYNC_DELAYED_BUFFERING\n"));
    }
    else {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) No Timeout for SYNC_DELAYED_BUFFERING\n"));
    }
  }
  catch (CORBA::TIMEOUT&) {
    if (simulate_flooding) {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Expected timeout received for SYNC_DELAYED_BUFFERING\n"));
    }
    else {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Unexpected "
                "timeout exception with synch scope SYNC_DELAYED_BUFFERING.\n"));
      status = false;
    }
  }

  // Timeout with SYNC_SCOPE SYNC_WITH_TRANSPORT
  try {
    if (simulate_flooding) {
      management_->unsleep ();
      test_obj_->sleep (0, 0);
      test_obj_transport_timeout_->dummy_one_way (msg);
    }
    else {
      test_obj_transport_timeout_->dummy_one_way ("SYNC_WITH_TRANSPORT");
    }
    if (flooded || simulate_flooding) {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Expected "
                  "timeout not received for synch scope SYNC_WITH_TRANSPORT.\n"
                  ));
      status = false;
    } else {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) No Timeout for SYNC_WITH_TRANSPORT\n"));
    }
  }
  catch (CORBA::TIMEOUT&) {
    if (!flooded && !simulate_flooding) {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> No timeout "
                  "expected for synch scope SYNC_WITH_TRANSPORT.\n"));
      status = false;
    } else {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Timeout for SYNC_WITH_TRANSPORT\n"));
    }
  }


  // Timeout with default SYNC_SCOPE SYNC_WITH_SERVER
  try {
    if (simulate_flooding) {
      management_->unsleep ();
      test_obj_->sleep (0, 0);
      test_obj_server_timeout_->dummy_one_way (msg);
    }
    else {
      test_obj_server_timeout_->dummy_one_way ("SYNC_WITH_SERVER");
    }
    if (flooded || simulate_flooding) {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Expected "
                  "timeout not received for SYNC_SCOPE SYNC_WITH_SERVER.\n"));
      status = false;
    } else {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) No Timeout for SYNC_WITH_SERVER.\n"));
    }
  }
  catch (CORBA::TIMEOUT&) {
    if (!flooded && !simulate_flooding) {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> No timeout "
                  "expected for synch scope SYNC_WITH_SERVER.\n"));
      status = false;
    } else {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Timeout for SYNC_WITH_SERVER\n"));
    }
  }


  // Timeout with default SYNC_SCOPE (SYNC_WITH_TARGET)
  try {
    if (simulate_flooding) {
      management_->unsleep ();
      test_obj_->sleep (0, 0);
      test_obj_target_timeout_->dummy_one_way (msg);
    }
    else {
      test_obj_target_timeout_->dummy_one_way ("SYNC_WITH_TARGET");
    }
    if (flooded || simulate_flooding) {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> Expected "
                  "timeout not received for SYNC_SCOPE SYNC_WITH_TARGET.\n"));
      status = false;
    } else {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) No Timeout for SYNC_WITH_TARGET.\n"));
    }
  }
  catch (CORBA::TIMEOUT&) {
    if (!flooded && !simulate_flooding) {
      ACE_ERROR ((LM_ERROR, "(%P|%t) ERROR: test_oneway_timeout> No timeout "
                  "expected for synch scope SYNC_WITH_TARGET.\n"));
      status = false;
    } else {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Timeout for SYNC_WITH_TARGET\n"));
    }
  }

  return status;
}

bool
Client::flood_connection (ACE_Time_Value& tv)
{
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

  std::auto_ptr<char> tmp;
  char* msg = new char [2000000];
  tmp.reset (msg);
  ACE_OS::memset (msg,'A',1999999);
  msg[1999999] = 0;

  test_obj_->sleep (tv.sec(), tv.msec());
  mod_test_obj->dummy_one_way (msg);

  orb_->perform_work ();

  //ACE_OS::sleep (ACE_Time_Value (5000));

  return true;
}
