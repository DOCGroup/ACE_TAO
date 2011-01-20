// $Id$

#include "Common/CIF_Common.h"
#include <tests/CIF/Common/CIF_Common.h>
#include <tests/CIF/Component/CIF_ComponentC.h>

//============================================================
// test_subscribe_unsubscribe
//============================================================
::Components::EventConsumerBase_ptr
get_consumer (::Components::Events_ptr sink,
              const char * name)
{
  ::Components::EventConsumerBase_var consumer;
  try
    {
      consumer = sink->get_consumer (name);
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_unsubscribe - "
                            "Error: InvalidName exception caught "
                            "during get_consumer.\n"));
      return ::Components::EventConsumerBase::_nil ();
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Navigation test_provide_facet - "
                            "Error: Unknown exception caught "
                            "during get_consumer.\n"));
      return ::Components::EventConsumerBase::_nil ();
    }
  return consumer._retn ();
}


//============================================================
// test_subscribe_unsubscribe
//============================================================
int
test_subscribe_unsubscribe (::Components::Events_ptr source,
                            ::Components::Events_ptr sink)
{
  int ret = 0;
  ::Components::Cookie_var ck;
  ::Components::EventConsumerBase_var consumer =
    get_consumer (sink, "consume_do_something");
  if (::CORBA::is_nil (consumer.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_unsubscribe - "
                            "Error:Consumer seems nil.\n"));
      return 1;
    }

  try
    {
      ck = source->subscribe ("publish_do_something",
                              consumer.in ());
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_unsubscribe - "
                            "Error: InvalidName exception caught "
                            "during subscribe.\n"));
      return 1;
    }
  catch (const ::Components::InvalidConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_unsubscribe - "
                            "Error: InvalidConnection exception caught "
                            "during subscribe.\n"));
      return 1;
    }
  catch (const ::Components::ExceededConnectionLimit &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_unsubscribe - "
                            "Error: ExceededConnectionLimit exception caught "
                            "during subscribe.\n"));
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_unsubscribe - "
                            "Error: Unknown exception caught "
                            "during subscribe.\n"));
      return 1;
    }

  try
    {
      ::Components::EventConsumerBase_var safe_tmp =
        source->unsubscribe ("publish_do_something",
                            ck.in ());
      if (::CORBA::is_nil (safe_tmp.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Events test_subscribe_unsubscribe - "
                                "Error: disconnect: Returned object  "
                                "seems nil.\n"));
          return 1;
        }
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_unsubscribe - "
                            "Error: InvalidName exception caught "
                            "during unsubscribe.\n"));
      return 1;
    }
  catch (const ::Components::InvalidConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_unsubscribe - "
                            "Error: InvalidConnection exception caught "
                            "during unsubscribe.\n"));
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_unsubscribe - "
                            "Error: Unknown exception caught "
                            "during unsubscribe.\n"));
      return 1;
    }
  if (ret == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_subscribe_unsubscribe - "
                            "Test passed!\n"));
    }
  return ret;
}

//============================================================
// test_subscribe_invalid_name
//============================================================
int
test_subscribe_invalid_name (::Components::Events_ptr source,
                             ::Components::Events_ptr sink)
{
  ::Components::EventConsumerBase_var consumer =
    get_consumer (sink, "consume_do_something");
  if (::CORBA::is_nil (consumer.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_invalid_name - "
                            "Error:Consumer seems nil.\n"));
      return 1;
    }

  ::Components::Cookie_var ck;
  try
    {
      ck = source->subscribe ("publish_do_something_invalid_name",
                              consumer.in ());
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_subscribe_invalid_name - "
                            "Received expected InvalidName exception "
                            "during subscribe.\n"));
    }
  catch (const ::Components::InvalidConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_invalid_name - "
                            "Error: InvalidConnection exception caught "
                            "during subscribe.\n"));
      return 1;
    }
  catch (const ::Components::ExceededConnectionLimit &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_invalid_name - "
                            "Error: ExceededConnectionLimit exception caught "
                            "during subscribe.\n"));
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_invalid_name - "
                            "Error: Unknown exception caught "
                            "during subscribe.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Events test_subscribe_invalid_name - "
                        "Test passed!\n"));
  return 0;
}

//============================================================
// test_subscribe_invalid_name
//============================================================
int
test_subscribe_invalid_connection (::Components::Events_ptr source,
                                   ::Components::Events_ptr sink)
{
  ::Components::EventConsumerBase_var consumer =
    get_consumer (sink, "consume_do_something");
  if (::CORBA::is_nil (consumer.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_invalid_connection - "
                            "Error:Consumer seems nil.\n"));
      return 1;
    }

  ::Components::Cookie_var ck;
  try
    {
      ck = source->subscribe ("publish_do_something_else",
                              consumer.in ());
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_invalid_connection - "
                            "Error: No InvalidConnection exception caught.\n"));
      return 1;
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_invalid_connection - "
                            "Error: InvalidName exception caught "
                            "during subscribe.\n"));
    }
  catch (const ::Components::InvalidConnection &)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_subscribe_invalid_connection - "
                            "Received expected InvalidConnection exception "
                            "during subscribe.\n"));
      return 1;
    }
  catch (const ::Components::ExceededConnectionLimit &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_invalid_connection - "
                            "Error: ExceededConnectionLimit exception caught "
                            "during subscribe.\n"));
      return 1;
    }
  catch (const ::CORBA::Exception &e)
    {
      e._tao_print_exception ("Events test_subscribe_invalid_connection - ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_subscribe_invalid_connection - "
                            "Error: Unknown exception caught "
                            "during subscribe.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Events test_subscribe_invalid_connection - "
                        "Test passed!\n"));
  return 0;
}

int
run_test (::Components::Events_ptr source,
          ::Components::Events_ptr sink)
{
  int ret = 0;
  try
    {
       ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
       ret += test_subscribe_unsubscribe (source, sink);

       ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
       ret += test_subscribe_invalid_name (source, sink);

       ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
       ret += test_subscribe_invalid_connection (source, sink);

       //ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
       //ret += test_subscribe_exceededconnectionlimit (source, sink);
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events run_test - "
                            "Error: Unknown "
                            "exception during run_test\n"));
      ++ret;
    }
  return ret;

  return ret;
}

int
ACE_TMAIN (int argc,  ACE_TCHAR **argv)
{
  CIF_Common cmd;
  int ret = 0;
  try
    {
      if (cmd.init (argc, argv) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Error: Unable to initalize\n"),
                            1);

        }
      ::CORBA::Object_var prov = cmd.get_provider_cmp ();
      if (::CORBA::is_nil (prov.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to get provider component\n"),
                            1);
        }
      ::Components::Events_var source = cmd.get_events_interface (true);
      if (::CORBA::is_nil (source.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to get source event interface\n"),
                            1);
        }
      ::Components::Events_var sink = cmd.get_events_interface (false);
      if (::CORBA::is_nil (sink.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to get sink event interface\n"),
                            1);
        }
      ret = run_test (source.in (), sink.in ());
      //::Components::CCMObject_var prov_cmp = ::Components::CCMObject::_narrow (prov);

      cmd.shutdown ();
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Events main");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        "Error: Caught unknown exception\n"),
                        1);
    }

  ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
  ACE_DEBUG ((LM_DEBUG, "SUMMARY : \n"));
  if (ret != 0)
    {
      ACE_ERROR ((LM_ERROR, "\tEvents main - "
              " %d error(s) found during tests.\n",
              ret));
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "\tEvents main - "
              " No problems found during tests.\n"));
    }
  return ret;
}
