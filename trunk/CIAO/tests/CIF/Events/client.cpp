// $Id$

#include Common/CIF_Common.h
#include Component/CIF_ComponentC.h

#include <ccm/CCM_EventsC.h>

#if !defined (CCM_NOEVENT)
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
      ACE_ERROR ((LM_ERROR, "Events get_consumer - "
                            "Error: InvalidName exception caught "
                            "during get_consumer.\n"));
      return ::Components::EventConsumerBase::_nil ();
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Navigation get_consumer - "
                            "Error: Unknown exception caught "
                            "during get_consumer.\n"));
      return ::Components::EventConsumerBase::_nil ();
    }
  return consumer._retn ();
}
#endif

#if !defined (CCM_NOEVENT)
//============================================================
// test_get_consumer
//============================================================
int
test_get_consumer (::Components::Events_ptr sink)
{
  try
    {
      ::Components::EventConsumerBase_var consumer =
        sink->get_consumer ("consume_do_something");
      if (::CORBA::is_nil (consumer.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Events test_get_consumer - "
                                "Error: returned consumer "
                                "seems nil.\n"));
          return 1;
        }
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_consumer - "
                            "Error: InvalidName exception caught "
                            "during get_consumer.\n"));
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_consumer - "
                            "Error: Unknown exception caught "
                            "during get_consumer.\n"));
      return 1;
    }
  return 0;
}
#endif

#if !defined (CCM_NOEVENT)
//============================================================
// test_get_consumer_invalid_name
//============================================================
int
test_get_consumer_invalid_name (::Components::Events_ptr sink)
{
  try
    {
      ::Components::EventConsumerBase_var consumer =
        sink->get_consumer ("consume_do_something_invalid_name");
      ACE_ERROR ((LM_ERROR, "Events test_get_consumer_invalid_name - "
                            "Error: No InvalidName exception raised.\n"));
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_get_consumer_invalid_name - "
                            "Received InvalidName exception "
                            "during get_consumer.\n"));
      ACE_DEBUG ((LM_DEBUG, "Events test_get_consumer_invalid_name - "
                            "Test passed!\n"));
      return 0;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Navigation test_get_consumer - "
                            "Error: Unknown exception caught "
                            "during get_consumer.\n"));
    }
  return 1;
}
#endif

#if !defined (CCM_NOEVENT)
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
#endif

#if !defined (CCM_NOEVENT)
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
#endif

#if !defined (CCM_NOEVENT)
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
      ACE_DEBUG ((LM_DEBUG, "Events test_subscribe_invalid_connection - "
                            "Test passed!\n"));
      return 0;
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
#endif

#if !defined (CCM_NOEVENT)
//============================================================
// connect_consumer
//============================================================
int
connect_consumer (::Components::Events_ptr source,
                  ::Components::Events_ptr sink,
                  const char * name)
{
  ::Components::EventConsumerBase_var consumer =
    get_consumer (sink, "consume_do_something");
  if (::CORBA::is_nil (consumer.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Events connect_consumer - "
                            "Error:Consumer seems nil.\n"));
      return 1;
    }
  try
    {
      source->connect_consumer (name,
                                consumer.in ());
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events connect_consumer - "
                            "Error: InvalidName exception caught "
                            "during connect_consumer.\n"));
    }
  catch (const ::Components::InvalidConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Events connect_consumer - "
                            "Error: InvalidConnection exception caught "
                            "during connect_consumer.\n"));
      return 1;
    }
  catch (const ::Components::AlreadyConnected &)
    {
      ACE_ERROR ((LM_ERROR, "Events connect_consumer - "
                            "Error: AlreadyConnected exception caught "
                            "during connect_consumer.\n"));
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events connect_consumer - "
                            "Error: Unknown exception caught "
                            "during connect_consumer.\n"));
      return 1;
    }
  return 0;
}
#endif

#if !defined (CCM_NOEVENT)
//============================================================
// disconnect_consumer
//============================================================
int
disconnect_consumer (::Components::Events_ptr source,
                    const char * name)
{
  try
    {
      ::Components::EventConsumerBase_var obj =
        source->disconnect_consumer (name);
      if (::CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Events disconnect_consumer - "
                                "Error: Returned object from disconnect_consumer "
                                "seems nil.\n"));
          return 1;
        }
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events disconnect_consumer - "
                            "Error: InvalidName exception caught "
                            "during disconnect_consumer.\n"));
      return 1;
    }
  catch (const ::Components::NoConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Events disconnect_consumer - "
                            "Error: NoConnection exception caught "
                            "during disconnect_consumer.\n"));
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events disconnect_consumer - "
                            "Error: Unknown exception caught "
                            "during disconnect_consumer.\n"));
      return 1;
    }
  return 0;
}
#endif

#if !defined (CCM_NOEVENT)
//============================================================
// test_connect_disconnect_consumer
//============================================================
int
test_connect_disconnect_consumer (::Components::Events_ptr source,
                                  ::Components::Events_ptr sink)
{
  if (connect_consumer (source, sink, "emit_do_something") == 0)
    {
      if (disconnect_consumer (source, "emit_do_something") == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Events test_connect_disconnect_consumer - "
                                "Test passed!\n"));
          return 0;
        }
    }
  return 1;
}
#endif

#if !defined (CCM_NOEVENT)
//============================================================
// test_connect_consumer_invalid_name
//============================================================
int
test_connect_consumer_invalid_name (::Components::Events_ptr source,
                                    ::Components::Events_ptr sink)
{
  ::Components::EventConsumerBase_var consumer =
    get_consumer (sink, "consume_do_something");
  if (::CORBA::is_nil (consumer.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_invalid_name - "
                            "Error:Consumer seems nil.\n"));
      return 1;
    }
  try
    {
      source->connect_consumer ("emit_do_something_invalid_name",
                                consumer.in ());
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_invalid_name - "
                            "Error: No InvalidName exception received.\n"));
      disconnect_consumer (source, "emit_do_something");
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_connect_consumer_invalid_name - "
                            "Received InvalidName exception "
                            "during connect_consumer.\n"));
    }
  catch (const ::Components::InvalidConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_invalid_name - "
                            "Error: InvalidConnection exception caught "
                            "during connect_consumer.\n"));
      return 1;
    }
  catch (const ::Components::AlreadyConnected &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_invalid_name - "
                            "Error: AlreadyConnected exception caught "
                            "during connect_consumer.\n"));
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_invalid_name - "
                            "Error: Unknown exception caught "
                            "during connect_consumer.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Events test_connect_consumer_invalid_name - "
                        "Test passed!\n"));
  return 0;
}
#endif

#if !defined (CCM_NOEVENT)
//============================================================
// test_connect_consumer_already_connected
//============================================================
int
test_connect_consumer_already_connected (::Components::Events_ptr source,
                                        ::Components::Events_ptr sink)
{
  ::Components::EventConsumerBase_var consumer =
    get_consumer (sink, "consume_do_something");
  if (::CORBA::is_nil (consumer.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_already_connected - "
                            "Error:Consumer seems nil.\n"));
      return 1;
    }
  try
    {
      source->connect_consumer ("emit_do_something",
                                consumer.in ());
      source->connect_consumer ("emit_do_something",
                                consumer.in ());
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_already_connected - "
                            "Error: No AlreadyConnected exception received.\n"));
      disconnect_consumer (source, "emit_do_something");
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_already_connected - "
                            "Error: InvalidName exception caught "
                            "during connect_consumer.\n"));
      return 1;
    }
  catch (const ::Components::InvalidConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_already_connected - "
                            "Error: InvalidConnection exception caught "
                            "during connect_consumer.\n"));
      return 1;
    }
  catch (const ::Components::AlreadyConnected &)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_connect_consumer_already_connected - "
                            "Received AlreadyConnected exception "
                            "during connect_consumer.\n"));
      disconnect_consumer (source, "emit_do_something");
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_already_connected - "
                            "Error: Unknown exception caught "
                            "during connect_consumer.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Events test_connect_consumer_already_connected - "
                        "Test passed!\n"));
  return 0;
}
#endif

#if !defined (CCM_NOEVENT)
//============================================================
// test_connect_consumer_invalid_connection
//============================================================
int
test_connect_consumer_invalid_connection (::Components::Events_ptr source,
                                          ::Components::Events_ptr sink)
{
  ::Components::EventConsumerBase_var consumer =
    get_consumer (sink, "consume_do_something");
  if (::CORBA::is_nil (consumer.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_invalid_connection - "
                            "Error:Consumer seems nil.\n"));
      return 1;
    }
  try
    {
      source->connect_consumer ("emit_do_something_else",
                                consumer.in ());
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_invalid_connection - "
                            "Error: No InvalidConnection exception received.\n"));
      disconnect_consumer (source, "emit_do_something_else");
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_invalid_connection - "
                            "Error: InvalidName exception caught "
                            "during connect_consumer.\n"));
      return 1;
    }
  catch (const ::Components::InvalidConnection &)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_connect_consumer_invalid_connection - "
                            "Received InvalidConnection exception "
                            "during connect_consumer.\n"));
    }
  catch (const ::Components::AlreadyConnected &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_invalid_connection - "
                            "Error: AlreadyConnected exception caught"
                            "during connect_consumer.\n"));
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_connect_consumer_invalid_connection - "
                            "Error: Unknown exception caught "
                            "during connect_consumer.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Events test_connect_consumer_invalid_connection - "
                        "Test passed!\n"));
  return 0;
}
#endif

#if !defined (CCM_NOEVENT)
//============================================================
// test_disconnect_consumer_invalid_name
//============================================================
int
test_disconnect_consumer_invalid_name (::Components::Events_ptr source,
                                      ::Components::Events_ptr sink)
{
  try
    {
      if (connect_consumer (source, sink, "emit_do_something") == 0)
        {
          ::Components::EventConsumerBase_var obj =
            source->disconnect_consumer ("emit_do_something_invalid_name");
          ACE_ERROR ((LM_ERROR, "Events test_disconnect_consumer_invalid_name - "
                                "Error: No InvalidName exception received.\n"));
          return 1;
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "Events test_disconnect_consumer_invalid_name - "
                                "Error: Unable to connect to consumer.\n"));
          return 1;
        }
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_disconnect_consumer_invalid_name - "
                            "Received InvalidName exception "
                            "during disconnect_consumer.\n"));
      disconnect_consumer (source, "emit_do_something");
    }
  catch (const ::Components::NoConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_disconnect_consumer_invalid_name - "
                            "Error: NoConnection exception caught "
                            "during disconnect_consumer.\n"));
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_disconnect_consumer_invalid_name - "
                            "Error: Unknown exception caught "
                            "during connect_consumer.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Events test_disconnect_consumer_invalid_name - "
                        "Test passed!\n"));
  return 0;
}
#endif

#if !defined (CCM_NOEVENT)
//============================================================
// test_disconnect_consumer_no_connection
//============================================================
int
test_disconnect_consumer_no_connection (::Components::Events_ptr source)
{
  try
    {
      ::Components::EventConsumerBase_var obj =
        source->disconnect_consumer ("emit_do_something");
      ACE_ERROR ((LM_ERROR, "Events test_disconnect_consumer_no_connection - "
                            "Error: No NoConnection exception received.\n"));
      return 1;
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_disconnect_consumer_no_connection - "
                            "Error: InvalidName exception caught "
                            "during disconnect_consumer.\n"));
      return 1;
    }
  catch (const ::Components::NoConnection &)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_disconnect_consumer_no_connection - "
                            "Received NoConnection exception "
                            "during disconnect_consumer.\n"));
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_disconnect_consumer_no_connection - "
                            "Error: Unknown exception caught "
                            "during connect_consumer.\n"));
      return 1;
    }
  ACE_DEBUG ((LM_DEBUG, "Events test_disconnect_consumer_no_connection - "
                        "Test passed!\n"));
  return 0;
}
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
//============================================================
// test_get_all_consumers
//============================================================
int
test_get_all_consumers (::Components::Events_ptr sink)
{
  int ret = 0;
  try
    {
      ::Components::ConsumerDescriptions_var cds =
        sink->get_all_consumers ();
      if (cds->length () != 2)
        {
          ACE_ERROR ((LM_ERROR, "Events test_get_all_consumers - "
                                "Error: Unexpected number of ConsumerDescriptions: "
                                "expected <2> - received <%d>.\n",
                                cds->length ()));
          ++ret;
        }
      for (::CORBA::ULong i = 0UL; i < cds->length (); ++i)
        {
          if (::ACE_OS::strcmp (cds[i]->name (), "consume_do_something") == 0 ||
              ::ACE_OS::strcmp (cds[i]->name (), "consume_do_something_else") == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Events test_get_all_consumers - "
                                    "Correct consumer description found <%C>\n",
                                    cds[i]->name ()));
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "Events test_get_all_consumers - "
                                    "Error Incorrect consumer description found <%C>\n",
                                    cds[i]->name ()));
              ++ret;
            }
          if (::CORBA::is_nil (cds[i]->consumer ()))
            {
              ACE_ERROR ((LM_ERROR, "Events test_get_all_consumers - "
                                    "Error: Consumer of %C seems nil.\n",
                                    cds[i]->name ()));
              ++ret;
            }
        }
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_all_consumers. Error: ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_all_consumers - "
                            "Error: Unknown exception caught "
                            "during get_all_consumers.\n"));
      return 1;
    }
  if (ret == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_get_all_consumers - "
                            "Test passed!\n"));
    }
  return ret;
}
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
//============================================================
// test_get_named_consumers
//============================================================
int
test_get_named_consumers (::Components::Events_ptr sink)
{
  int ret = 0;
  try
    {
      ::Components::NameList_var names;
      ACE_NEW_THROW_EX (names,
                        ::Components::NameList,
                        CORBA::NO_MEMORY ());
      names->length (1);
      (*names)[0] = CORBA::string_dup ("consume_do_something_else");
      ::Components::ConsumerDescriptions_var cds =
        sink->get_named_consumers (names);
      if (cds->length () != 1)
        {
          ACE_ERROR ((LM_ERROR, "Events test_get_named_consumers <1> - "
                                "Error: Unexpected number of ConsumerDescriptions: "
                                "expected <1> - received <%d>.\n",
                                cds->length ()));
          ++ret;
        }
      for (::CORBA::ULong i = 0UL; i < cds->length (); ++i)
        {
          if (::ACE_OS::strcmp (cds[i]->name (), "consume_do_something_else") == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Events test_get_named_consumers <1> - "
                                    "Correct consumer description found <%C>\n",
                                    cds[i]->name ()));
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "Events test_get_named_consumers <1> - "
                                    "Error Incorrect consumer description found <%C>\n",
                                    cds[i]->name ()));
              ++ret;
            }
          if (::CORBA::is_nil (cds[i]->consumer ()))
            {
              ACE_ERROR ((LM_ERROR, "Events test_get_named_consumers <1> - "
                                    "Error: Consumer of %C seems nil.\n",
                                    cds[i]->name ()));
              ++ret;
            }
        }

      names->length (2);
      (*names)[0] = CORBA::string_dup ("consume_do_something");
      (*names)[1] = CORBA::string_dup ("consume_do_something_else");
      cds = sink->get_named_consumers (names);
      if (cds->length () != 2)
        {
          ACE_ERROR ((LM_ERROR, "Events test_get_named_consumers <2> - "
                                "Error: Unexpected number of ConsumerDescriptions: "
                                "expected <2> - received <%d>.\n",
                                cds->length ()));
          ++ret;
        }
      for (::CORBA::ULong i = 0UL; i < cds->length (); ++i)
        {
          if (::ACE_OS::strcmp (cds[i]->name (), "consume_do_something") == 0 ||
              ::ACE_OS::strcmp (cds[i]->name (), "consume_do_something_else") == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Events test_get_named_consumers <2> - "
                                    "Correct consumer description found <%C>\n",
                                    cds[i]->name ()));
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "Events test_get_named_consumers <2> - "
                                    "Error Incorrect consumer description found <%C>\n",
                                    cds[i]->name ()));
              ++ret;
            }
          if (::CORBA::is_nil (cds[i]->consumer ()))
            {
              ACE_ERROR ((LM_ERROR, "Events test_get_named_consumers <2> - "
                                    "Error: Consumer of %C seems nil.\n",
                                    cds[i]->name ()));
              ++ret;
            }
        }
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_named_consumers - "
                            "Error: InvalidName exception caught "
                            "during get_named_consumers.\n"));
      return 1;
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_named_consumers. Error: ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_named_consumers - "
                            "Error: Unknown exception caught "
                            "during get_named_consumers.\n"));
      return 1;
    }
  if (ret == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_get_named_consumers - "
                            "Test passed!\n"));
    }
  return ret;
}
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
//============================================================
// test_consumer_of_consumer
//============================================================
int
test_consumer_of_consumer (::Components::Events_ptr source,
                           ::Components::Events_ptr sink)
{
  int ret = 0;

  try
    {
      connect_consumer (source, sink, "emit_do_something");
      ::Components::NameList_var names;
      ACE_NEW_THROW_EX (names,
                        ::Components::NameList,
                        CORBA::NO_MEMORY ());
      names->length (1);
      (*names)[0] = CORBA::string_dup ("consume_do_something");
      ::Components::ConsumerDescriptions_var cds =
        sink->get_named_consumers (names);
      if (cds->length () != 1)
        {
          ACE_ERROR ((LM_ERROR, "Events test_consumer_of_consumer - "
                                "Error: Unexpected number of consumer "
                                "descriptions found. expected <1> - "
                                "received <%u>\n",
                                cds->length ()));
          return 1;
        }
      if (::CORBA::is_nil (cds[0UL]->consumer()))
        {
          ACE_ERROR ((LM_ERROR, "Events test_consumer_of_consumer - "
                                "Error: Consumer of consumer description "
                                "seems to be nil.\n"));
          return 1;
        }
      disconnect_consumer (source, "emit_do_something");
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_consumer_of_consumer - "
                            "Error: Unexpected InvalidName exception caught"
                            "during get_named_consumers.\n"));
      ++ret;
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_consumer_of_consumer . Error: ");
      ++ret;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_consumer_of_consumer - "
                            "Error: Unknown exception caught "
                            "during get_named_consumers.\n"));
      ++ret;
    }
  if (ret == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_get_named_consumers - "
                            "Test passed!\n"));
    }
  return ret;
}
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
//============================================================
// test_get_named_consumers_invalid_name
//============================================================
int
test_get_named_consumers_invalid_name (::Components::Events_ptr sink)
{
  int ret = 0;

  ::Components::NameList_var names;
  ACE_NEW_THROW_EX (names,
                    ::Components::NameList,
                    CORBA::NO_MEMORY ());
  ::Components::ConsumerDescriptions_var cds;

  try
    {
      names->length (1);
      (*names)[0] = CORBA::string_dup ("consume_do_something_invalid_name");
      cds = sink->get_named_consumers (names);
      ACE_ERROR ((LM_ERROR, "Events test_get_named_consumers_invalid_name <1> - "
                            "Error: No InvalidName exception caught "
                            "during get_named_consumers\n"));
      ++ret;
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_get_named_consumers_invalid_name <1> - "
                            "Received InvalidName exception "
                            "during get_named_consumers.\n"));
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_named_consumers_invalid_name <1> . Error: ");
      ++ret;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_named_consumers_invalid_name  <1> - "
                            "Error: Unknown exception caught "
                            "during get_named_consumers.\n"));
      ++ret;
    }

  try
    {
      names->length (2);
      (*names)[0] = CORBA::string_dup ("consume_do_something");
      (*names)[1] = CORBA::string_dup ("consume_do_something_else_invalid_name");
      cds = sink->get_named_consumers (names);
      ACE_ERROR ((LM_ERROR, "Events test_get_named_consumers_invalid_name <2> - "
                            "Error: No InvalidName exception caught "
                            "during get_named_consumers\n"));
      ++ret;
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_get_named_consumers_invalid_name <2> - "
                            "Received InvalidName exception "
                            "during get_named_consumers.\n"));
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_named_consumers_invalid_name <2>. Error: ");
      ++ret;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_named_consumers_invalid_name <2> - "
                            "Error: Unknown exception caught "
                            "during get_named_consumers.\n"));
      ++ret;
    }
  if (ret == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_get_named_consumers_invalid_name - "
                            "Test passed!\n"));
    }
  return ret;
}
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
//============================================================
// test_get_all_emitters
//============================================================
int
test_get_all_emitters (::Components::Events_ptr source)
{
  int ret = 0;

  try
    {
      ::Components::EmitterDescriptions_var eds =
        source->get_all_emitters ();
      if (eds->length () != 2)
        {
          ACE_ERROR ((LM_ERROR, "Events test_get_all_emitters - "
                                "Error: Unexpected number of Emitter "
                                "descriptions received. expected - "
                                "received <%u>\n",
                                eds->length ()));
          ++ret;
        }
      for (::CORBA::ULong i = 0UL; i < eds->length (); ++i)
        {
          if (::ACE_OS::strcmp (eds[i]->name (), "emit_do_something") == 0 ||
              ::ACE_OS::strcmp (eds[i]->name (), "emit_do_something_else") == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Events test_get_all_emitters - "
                                    "Correct emitter description found <%C>\n",
                                    eds[i]->name ()));
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "Events test_get_all_emitters - "
                                    "Error Incorrect emitter description found <%C>\n",
                                    eds[i]->name ()));
              ++ret;
            }
        }
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_all_emitters. Error: ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_all_emitters - "
                            "Error: Unknown exception caught "
                            "during get_all_emitters.\n"));
      return 1;
    }
  if (ret == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_get_all_emitters - "
                            "Test passed!\n"));
    }
  return ret;
}
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
//============================================================
// test_get_named_emitters
//============================================================
int
test_get_named_emitters (::Components::Events_ptr source)
{
  int ret = 0;
  try
    {
      ::Components::NameList_var names;
      ACE_NEW_THROW_EX (names,
                        ::Components::NameList,
                        CORBA::NO_MEMORY ());
      names->length (1);
      (*names)[0] = CORBA::string_dup ("emit_do_something_else");
      ::Components::EmitterDescriptions_var eds =
        source->get_named_emitters (names);
      if (eds->length () != 1)
        {
          ACE_ERROR ((LM_ERROR, "Events test_get_named_emitters <1> - "
                                "Error: Unexpected number of ConsumerDescriptions: "
                                "expected <1> - received <%d>.\n",
                                eds->length ()));
          ++ret;
        }
      for (::CORBA::ULong i = 0UL; i < eds->length (); ++i)
        {
          if (::ACE_OS::strcmp (eds[i]->name (), "emit_do_something_else") == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Events test_get_named_emitters <1> - "
                                    "Correct emitter description found <%C>\n",
                                    eds[i]->name ()));
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "Events test_get_named_emitters <1> - "
                                    "Error Incorrect emitter description found <%C>\n",
                                    eds[i]->name ()));
              ++ret;
            }
        }

      names->length (2);
      (*names)[0] = CORBA::string_dup ("emit_do_something");
      (*names)[1] = CORBA::string_dup ("emit_do_something_else");
      eds = source->get_named_emitters (names);
      if (eds->length () != 2)
        {
          ACE_ERROR ((LM_ERROR, "Events test_get_named_emitters <2> - "
                                "Error: Unexpected number of EmitterDescriptions: "
                                "expected <2> - received <%d>.\n",
                                eds->length ()));
          ++ret;
        }
      for (::CORBA::ULong i = 0UL; i < eds->length (); ++i)
        {
          if (::ACE_OS::strcmp (eds[i]->name (), "emit_do_something") == 0 ||
              ::ACE_OS::strcmp (eds[i]->name (), "emit_do_something_else") == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Events test_get_named_emitters <2> - "
                                    "Correct emitter description found <%C>\n",
                                    eds[i]->name ()));
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "Events test_get_named_emitters <2> - "
                                    "Error Incorrect emitter description found <%C>\n",
                                    eds[i]->name ()));
              ++ret;
            }
        }
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_named_emitters - "
                            "Error: InvalidName exception caught "
                            "during get_named_emitters.\n"));
      return 1;
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_named_emitters. Error: ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_named_emitters - "
                            "Error: Unknown exception caught "
                            "during get_named_emitters.\n"));
      return 1;
    }
  if (ret == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_get_named_emitters - "
                            "Test passed!\n"));
    }
  return ret;
}
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
//============================================================
// test_consumer_of_emitter
//============================================================
int
test_consumer_of_emitter (::Components::Events_ptr source,
                          ::Components::Events_ptr sink)
{
  int ret = 0;
  try
    {
      connect_consumer (source, sink, "emit_do_something");
      ::Components::NameList_var names;
      ACE_NEW_THROW_EX (names,
                        ::Components::NameList,
                        CORBA::NO_MEMORY ());
      names->length (1);
      (*names)[0] = CORBA::string_dup ("emit_do_something");
      ::Components::EmitterDescriptions_var eds =
        source->get_named_emitters (names);
      if (eds->length () != 1)
        {
          ACE_ERROR ((LM_ERROR, "Events test_consumer_of_emitter - "
                                "Error: Unexpected number of ConsumerDescriptions: "
                                "expected <1> - received <%d>.\n",
                                eds->length ()));
          return 1;
        }
      if (::CORBA::is_nil (eds[0UL]->consumer ()))
        {
          ACE_ERROR ((LM_ERROR, "Events test_consumer_of_emitter - "
                                "Error: Consumer of Emmitter description "
                                "seems nil.\n"));
        }
      disconnect_consumer (source, "emit_do_something");
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_consumer_of_emitter - "
                            "Error: InvalidName exception caught "
                            "during get_named_emitters.\n"));
      return 1;
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_consumer_of_emitter. Error: ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_consumer_of_emitter - "
                            "Error: Unknown exception caught "
                            "during get_named_emitters.\n"));
      return 1;
    }
  if (ret == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_consumer_of_emitter - "
                            "Test passed!\n"));
    }
  return ret;
}
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
//============================================================
// test_get_named_emitters_invalid_name
//============================================================
int
test_get_named_emitters_invalid_name (::Components::Events_ptr source)
{
  try
    {
      ::Components::NameList_var names;
      ACE_NEW_THROW_EX (names,
                        ::Components::NameList,
                        CORBA::NO_MEMORY ());
      names->length (2);
      (*names)[0] = CORBA::string_dup ("emit_do_something");
      (*names)[1] = CORBA::string_dup ("emit_do_something_else_invalid_name");
      ::Components::EmitterDescriptions_var eds =
        source->get_named_emitters (names);
      ACE_ERROR ((LM_ERROR, "Events test_get_named_emitters_invalid_name - "
                            "Error: No InvalidName exception caught "
                            "during get_named_emitters\n"));
      return 1;
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_get_named_emitters_invalid_name - "
                            "Received InvalidName exception "
                            "during get_named_emitters.\n"));
      ACE_DEBUG ((LM_DEBUG, "Events test_get_named_emitters_invalid_name - "
                            "Test passed!\n"));
      return 0;
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_named_emitters_invalid_name. Error: ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_named_emitters_invalid_name - "
                            "Error: Unknown exception caught "
                            "during get_named_emitters.\n"));
      return 1;
    }
  return 1;
}
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
//============================================================
// test_get_all_publishers
//============================================================
int
test_get_all_publishers (::Components::Events_ptr source)
{
  int ret = 0;

  try
    {
      ::Components::PublisherDescriptions_var pds =
        source->get_all_publishers ();
      if (pds->length () != 2)
        {
          ACE_ERROR ((LM_ERROR, "Events test_get_all_publishers - "
                                "Error: Unexpected number of Emitter "
                                "descriptions received. expected - "
                                "received <%u>\n",
                                pds->length ()));
          ++ret;
        }
      for (::CORBA::ULong i = 0UL; i < pds->length (); ++i)
        {
          if (::ACE_OS::strcmp (pds[i]->name (), "publish_do_something") == 0 ||
              ::ACE_OS::strcmp (pds[i]->name (), "publish_do_something_else") == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Events test_get_all_publishers - "
                                    "Correct publisher description found <%C>\n",
                                    pds[i]->name ()));
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "Events test_get_all_publishers - "
                                    "Error Incorrect publisher description found <%C>\n",
                                    pds[i]->name ()));
              ++ret;
            }
        }
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_all_publishers. Error: ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_all_publishers - "
                            "Error: Unknown exception caught "
                            "during get_all_publishers.\n"));
      return 1;
    }
  if (ret == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_get_all_publishers - "
                            "Test passed!\n"));
    }
  return ret;
}
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
//============================================================
// test_get_named_publishers
//============================================================
int
test_get_named_publishers (::Components::Events_ptr source)
{
  int ret = 0;
  try
    {
      ::Components::NameList_var names;
      ACE_NEW_THROW_EX (names,
                        ::Components::NameList,
                        CORBA::NO_MEMORY ());
      names->length (1);
      (*names)[0] = CORBA::string_dup ("publish_do_something_else");
      ::Components::PublisherDescriptions_var pds =
        source->get_named_publishers (names);
      if (pds->length () != 1)
        {
          ACE_ERROR ((LM_ERROR, "Events test_get_named_publishers <1> - "
                                "Error: Unexpected number of PublisherDescriptions: "
                                "expected <1> - received <%d>.\n",
                                pds->length ()));
          ++ret;
        }
      for (::CORBA::ULong i = 0UL; i < pds->length (); ++i)
        {
          if (::ACE_OS::strcmp (pds[i]->name (), "publish_do_something_else") == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Events test_get_named_publishers <1> - "
                                    "Correct publisher description found <%C>\n",
                                    pds[i]->name ()));
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "Events test_get_named_publishers <1> - "
                                    "Error Incorrect publisher description found <%C>\n",
                                    pds[i]->name ()));
              ++ret;
            }
        }

      names->length (2);
      (*names)[0] = CORBA::string_dup ("publish_do_something");
      (*names)[1] = CORBA::string_dup ("publish_do_something_else");
      pds = source->get_named_publishers (names);
      if (pds->length () != 2)
        {
          ACE_ERROR ((LM_ERROR, "Events test_get_named_publishers <2> - "
                                "Error: Unexpected number of PublisherDescriptions: "
                                "expected <2> - received <%d>.\n",
                                pds->length ()));
          ++ret;
        }
      for (::CORBA::ULong i = 0UL; i < pds->length (); ++i)
        {
          if (::ACE_OS::strcmp (pds[i]->name (), "publish_do_something") == 0 ||
              ::ACE_OS::strcmp (pds[i]->name (), "publish_do_something_else") == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Events test_get_named_publishers <2> - "
                                    "Correct publisher description found <%C>\n",
                                    pds[i]->name ()));
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "Events test_get_named_publishers <2> - "
                                    "Error Incorrect publisher description found <%C>\n",
                                    pds[i]->name ()));
              ++ret;
            }
        }
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_named_publishers - "
                            "Error: InvalidName exception caught "
                            "during get_named_publishers.\n"));
      return 1;
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_named_publishers. Error: ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_named_publishers - "
                            "Error: Unknown exception caught "
                            "during get_named_publishers.\n"));
      return 1;
    }
  if (ret == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_get_named_publishers - "
                            "Test passed!\n"));
    }
  return ret;
}
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
//============================================================
// test_get_named_publishers_invalid_name
//============================================================
int
test_get_named_publishers_invalid_name (::Components::Events_ptr source)
{
  try
    {
      ::Components::NameList_var names;
      ACE_NEW_THROW_EX (names,
                        ::Components::NameList,
                        CORBA::NO_MEMORY ());
      names->length (2);
      (*names)[0] = CORBA::string_dup ("publish_do_something");
      (*names)[1] = CORBA::string_dup ("publish_do_something_else_invalid_name");
      ::Components::PublisherDescriptions_var pds =
        source->get_named_publishers (names);
      ACE_ERROR ((LM_ERROR, "Events test_get_named_publishers_invalid_name - "
                            "Error: No InvalidName exception caught "
                            "during get_named_publishers\n"));
      return 1;
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_get_named_publishers_invalid_name - "
                            "Received InvalidName exception "
                            "during get_named_publishers.\n"));
      ACE_DEBUG ((LM_DEBUG, "Events test_get_named_publishers_invalid_name - "
                            "Test passed!\n"));
      return 0;
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_get_named_publishers_invalid_name. Error: ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_get_named_publishers_invalid_name - "
                            "Error: Unknown exception caught "
                            "during get_named_publishers.\n"));
      return 1;
    }
  return 1;
}
#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
//============================================================
// test_get_named_publishers_invalid_name
//============================================================
int
test_consumers_of_publishers (::Components::Events_ptr source,
                              ::Components::Events_ptr sink)
{
  int ret = 0;
  try
    {
      ::Components::EventConsumerBase_var consumer1 =
        get_consumer (sink, "consume_do_something");
      ::Components::EventConsumerBase_var consumer2 =
        get_consumer (sink, "consume_do_something");
      if (::CORBA::is_nil (consumer1.in ()) ||
          ::CORBA::is_nil (consumer1.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Events test_consumers_of_publishers - "
                                "Error: One of the consumers seems nil.\n"));
          return 1;
        }
      ::Components::Cookie_var ck1 = source->subscribe ("publish_do_something",
                                                      consumer1.in ());
      ::Components::Cookie_var ck2 = source->subscribe ("publish_do_something",
                                                      consumer2.in ());
      ::Components::NameList_var names;
      ACE_NEW_THROW_EX (names,
                        ::Components::NameList,
                        CORBA::NO_MEMORY ());
      names->length (1);
      (*names)[0] = CORBA::string_dup ("publish_do_something");
      ::Components::PublisherDescriptions_var pds =
        source->get_named_publishers (names);

      if (pds->length () != 1)
        {
          ACE_ERROR ((LM_ERROR, "Events test_consumers_of_publishers - "
                                "Error: Unexpected number of PublisherDescriptions: "
                                "expected <1> - received <%d>.\n",
                                pds->length ()));
          return 1;
        }
      ::CORBA::ULong consumers = pds[0UL]->consumers ().length ();
      if (consumers != 2)
        {
          ACE_ERROR ((LM_ERROR, "Events test_consumers_of_publishers - "
                                "Error: Unexpected number of PublisherDescriptions: "
                                "expected <2> - received <%d>.\n",
                                pds[0UL]->consumers().length ()));
          ++ret;
        }
      for (::CORBA::ULong consumer = 0; consumer < consumers; ++consumer)
        {
          ::Components::EventConsumerBase_var unsub =
            source->unsubscribe ("publish_do_something",
                                 pds[0UL]->consumers ()[consumer]->ck ());
        }
    }
  catch (const ::Components::InvalidName &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_consumers_of_publishers - "
                            "Error: InvalidName exception caught\n"));
      return 1;
    }
  catch (const ::Components::InvalidConnection &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_consumers_of_publishers - "
                            "Error: InvalidConnection exception caught\n"));
      return 1;
    }
  catch (const ::Components::ExceededConnectionLimit &)
    {
      ACE_ERROR ((LM_ERROR, "Events test_consumers_of_publishers - "
                            "Error: ExceededConnectionLimit exception caught\n"));
      return 1;
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception ("test_consumers_of_publishers. Error: ");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Events test_consumers_of_publishers - "
                            "Error: Unknown exception caught "
                            "during get_named_publishers.\n"));
      return 1;
    }
  if (ret == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test_consumers_of_publishers - "
                            "Test passed!\n"));
    }
  return ret;
}
#endif

int
run_test (::Components::Events_ptr source,
          ::Components::Events_ptr sink)
{
  int ret = 0;
  try
    {

#if !defined (CCM_NOEVENT)
      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_get_consumer (sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_get_consumer_invalid_name (sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_subscribe_unsubscribe (source, sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_subscribe_invalid_name (source, sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_subscribe_invalid_connection (source, sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_connect_disconnect_consumer (source, sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_connect_consumer_invalid_name (source, sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_connect_consumer_already_connected (source, sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_connect_consumer_invalid_connection (source, sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_disconnect_consumer_invalid_name (source, sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_disconnect_consumer_no_connection (source);

#endif

#if !defined (CCM_LW) && !defined (CCM_NOEVENT)
      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_get_all_consumers (sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_get_named_consumers (sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_consumer_of_consumer (source, sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_get_named_consumers_invalid_name (sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_get_all_emitters (source);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_get_named_emitters (source);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_consumer_of_emitter (source, sink);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_get_named_emitters_invalid_name (source);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_get_all_publishers (source);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_get_named_publishers (source);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_get_named_publishers_invalid_name (source);

      ACE_DEBUG ((LM_DEBUG, "\n\n===============================\n"));
      ret += test_consumers_of_publishers (source, sink);
#else
  ACE_UNUSED_ARG (sink);
  ACE_UNUSED_ARG (source);
#endif

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
  ACE_DEBUG ((LM_DEBUG, "SUMMARY : "));
  if (ret != 0)
    {
      ACE_ERROR ((LM_ERROR, "Events main - "
              " %d error(s) found during tests.\n\n",
              ret));
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "Events main - "
              " No problems found during tests.\n\n"));
    }
  return ret;
}
