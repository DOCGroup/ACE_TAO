// -*- C++ -*-
//
// $Id$

#include "Different_Test_Receiver_exec.h"

#include "ace/OS_NS_unistd.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#define QUERY_GETTER "( (iteration > %0) AND (iteration < %1) )"
#define QUERY_READER "( (iteration < %0) OR (iteration > %1) )"

#define MIN_ITERATION "6"
#define MAX_ITERATION "9"

namespace CIAO_Different_Test_Receiver_Impl
{
  //============================================================
  // read_action_Generator
  //============================================================
  read_action_Generator::read_action_Generator (Receiver_exec_i &callback)
    : callback_ (callback)
  {
  }

  read_action_Generator::~read_action_Generator ()
  {
  }

  int
  read_action_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    ACE_DEBUG ((LM_DEBUG, "Checking if last sample "
                          "is available in DDS...\n"));
    if (this->callback_.check_last ())
      {
        this->callback_.run ();
      }
    return 0;
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : iterations_ (15),
      keys_ (5),
      ticker_ (0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }


  bool
  Receiver_exec_i::check_last ()
  {
    ::Different_Test::QueryConditionTestConnector::Reader_var reader =
      this->context_->get_connection_get_port_data ();

    try
      {
        QueryConditionTest queryfiltertest_info;
        ::CCM_DDS::ReadInfo readinfo;
        char key[10];
        ACE_OS::sprintf (key, "KEY_%d", this->keys_);
        queryfiltertest_info.symbol = CORBA::string_dup (key);
        reader->read_one_last (
                queryfiltertest_info,
                readinfo,
                ::DDS::HANDLE_NIL);
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::check_last - "
                              "last iteration <%d> - <%d>\n",
                               queryfiltertest_info.iteration,
                               this->iterations ()));
        return queryfiltertest_info.iteration == this->iterations ();
      }
    catch (const ::CCM_DDS::InternalError &)
      {
      }
    catch (const ::CCM_DDS::NonExistent &)
      {
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::check_last: "
                              "ERROR: Unexpected exception caught\n"));
      }
    return false;
  }

  // Supported operations and attributes.
  void
  Receiver_exec_i::check_iter_reader (const QueryConditionTest & sample)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ALL : ")
        ACE_TEXT ("sample received for <%C>: iteration <%u>\n"),
        sample.symbol.in (),
        sample.iteration));
    if (sample.iteration > ACE_OS::atoi (MIN_ITERATION) &&
        sample.iteration < ACE_OS::atoi (MAX_ITERATION))
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ALL : ")
                              ACE_TEXT ("Didn't expect samples between ")
                              ACE_TEXT ("%C and %C\n"),
                              MIN_ITERATION,
                              MAX_ITERATION));
      }
  }

  void
  Receiver_exec_i::check_iter_getter (const QueryConditionTest & sample)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("GET ALL : ")
        ACE_TEXT ("sample received for <%C>: iteration <%u>\n"),
        sample.symbol.in (),
        sample.iteration));
    if (sample.iteration <= ACE_OS::atoi (MIN_ITERATION))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: GET ALL: "
                              "Didn't expect samples with iterations "
                              "<= %C\n",
                              MIN_ITERATION));
      }
    if (sample.iteration > ACE_OS::atoi (MAX_ITERATION))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: GET ALL: "
                              "Didn't expect samples with iterations "
                              "> %C\n",
                              MAX_ITERATION));
      }
  }


  void
  Receiver_exec_i::read_all (void)
  {
    ::Different_Test::QueryConditionTestConnector::Reader_var reader =
      this->context_->get_connection_read_port_data ();

    QueryConditionTestSeq queryfiltertest_info_seq;
    ::CCM_DDS::ReadInfoSeq readinfo_seq;
    reader->read_all (queryfiltertest_info_seq, readinfo_seq);
    if (queryfiltertest_info_seq.length () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::read_all : "
                              "No samples available in Reader!\n"));
      }
    for (CORBA::ULong it = 0; it < queryfiltertest_info_seq.length (); ++it)
      {
        this->check_iter_reader (queryfiltertest_info_seq[it]);
      }
  }

  void
  Receiver_exec_i::get_all (void)
  {
    ::Different_Test::QueryConditionTestConnector::Getter_var getter =
      this->context_->get_connection_get_port_fresh_data ();

    if (::CORBA::is_nil (getter.in ()))
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::get_all - "
                              "ERROR: No Getter\n"));
      }
    QueryConditionTest_var qf_info;
    ::CCM_DDS::ReadInfo readinfo;
    CORBA::Boolean result = getter->get_one (qf_info.out (), readinfo);
    if (result)
      {
        this->check_iter_getter (qf_info.in ());
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::get_all - "
                              "ERROR: time out when retrieving "
                              "first sample.\n"));
      }
    while (result)
      {
        result = getter->get_one (qf_info.out (), readinfo);
        if (result)
          {
            this->check_iter_getter (qf_info.in ());
          }
      }
  }

  void
  Receiver_exec_i::test_all ()
  {
    const char * test = "GET ALL";
    try
      {
        get_all ();
        test = "READ ALL";
        read_all ();
      }
    catch (const CCM_DDS::NonExistent& ex)
      {
        for (CORBA::ULong i = 0; i < ex.indexes.length (); ++i)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ERROR %C: ")
                  ACE_TEXT ("caught expected exception: index <%u>\n"),
                  test,
                  ex.indexes[i]));
          }
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: %C: ")
              ACE_TEXT ("caught InternalError exception: retval <%u>\n"),
              test,
              ex.error_code));
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception (test);
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::test_all : Exception caught\n")));
      }
  }

  void
  Receiver_exec_i::check_filters (void)
  {
    ::Different_Test::QueryConditionTestConnector::Reader_var get_reader =
      this->context_->get_connection_get_port_data ();
    if (! ::CORBA::is_nil (get_reader.in ()))
      {
        this->check_filter (get_reader.in (), true);
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "Unable to retrieve pointer to the reader.\n"));
      }

    ::Different_Test::QueryConditionTestConnector::Reader_var read_reader =
      this->context_->get_connection_read_port_data ();
    if (! ::CORBA::is_nil (read_reader.in ()))
      {
        this->check_filter (read_reader.in (), false);
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "Unable to retrieve pointer to the reader.\n"));
      }
  }

  void
  Receiver_exec_i::check_filter (
    ::Different_Test::QueryConditionTestConnector::Reader_ptr reader,
    bool check_getter)
  {
    ::CCM_DDS::QueryFilter_var filter;
    try
      {
        filter = reader->query ();
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                              "caught InternalError exception: retval <%u>\n",
                              ex.error_code));
        return;
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Receiver_exec_i::check_filter: ");
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                              "Exception caught\n"));
        return;
      }

    //check query
    bool error = false;

    const char * query = "";
    const char * port = "";
    if (check_getter)
      {
        query = QUERY_GETTER;
        port = "GETTER";
      }
    else
      {
        query = QUERY_READER;
        port = "READER";
      }

    if (ACE_OS::strcmp (filter->expression, query) != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter <%C> - "
                              "Unexpected query when retrieving filter: "
                              "expected <%C> - received <%C>\n",
                              port, query, filter->expression.in ()));
        error = true;
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::check_filter <%C> - "
                              "Query expression seems correct : "
                              "expected <%C> - received <%C>\n",
                              port, query, filter->expression.in ()));
      }
    //check current parameters.
    if (filter->parameters.length () != 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter <%C> - "
                              "Unexpected number of parameters: "
                              "expected <%d> - received <%d>\n",
                              port, 2, filter->parameters.length ()));
        error = true;
      }

    if (filter->parameters.length () >= 1)
      {
        if (ACE_OS::atoi (filter->parameters[0]) != ACE_OS::atoi(MIN_ITERATION))
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter <%C> - "
                                  "Unexpected param value: "
                                  "expected <%d> - received <%C>\n",
                                  port,
                                  MIN_ITERATION,
                                  filter->parameters[0].in ()));
            error = true;
          }
      }
    if (filter->parameters.length () >= 2)
      {
        if (ACE_OS::atoi (filter->parameters[1]) != ACE_OS::atoi (MAX_ITERATION))
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter <%C> - "
                                  "Unexpected query when retrieving filter: "
                                  "expected <%d> - received <%C>\n",
                                  port,
                                  MAX_ITERATION,
                                  filter->parameters[1].in ()));
            error = true;
          }
      }
    if (!error)
      {
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::check_filter <%C> - "
                              "Passed check_filter test.\n",
                              port));
      }
  }

  void
  Receiver_exec_i::set_filters ()
  {
    ACE_DEBUG ((LM_DEBUG, "Set filters\n"));

    try
      {
        ::Different_Test::QueryConditionTestConnector::Reader_var get_reader =
          this->context_->get_connection_get_port_data ();

        ::Different_Test::QueryConditionTestConnector::Reader_var read_reader =
          this->context_->get_connection_read_port_data ();

        ::CCM_DDS::QueryFilter get_filter;
        get_filter.expression = CORBA::string_dup (QUERY_GETTER);
        get_filter.parameters.length (2);
        get_filter.parameters[0] = CORBA::string_dup (MIN_ITERATION);
        get_filter.parameters[1] = CORBA::string_dup (MAX_ITERATION);
        get_reader->query (get_filter);
        ACE_DEBUG ((LM_DEBUG, "Filter set for the getter.\n"));


        ::CCM_DDS::QueryFilter read_filter;
        read_filter.expression = CORBA::string_dup (QUERY_READER);
        read_filter.parameters.length (2);
        read_filter.parameters[0] = CORBA::string_dup (MIN_ITERATION);
        read_filter.parameters[1] = CORBA::string_dup (MAX_ITERATION);
        read_reader->query (read_filter);
        ACE_DEBUG ((LM_DEBUG, "Filter set for the reader.\n"));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::set_filters () - "
                    "Unexpected exception caught.\n"));
      }
  }

  void
  Receiver_exec_i::start_read (void)
  {
    ACE_DEBUG ((LM_DEBUG, "Start reading by starting a timer.\n"));

    this->ticker_ = new read_action_Generator (*this);
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                                          this->ticker_,
                                          0,
                                          ACE_Time_Value(1, 0),
                                          ACE_Time_Value(1, 0)) == -1)
      {
        ACE_ERROR ((LM_ERROR, "Unable to schedule Timer\n"));
      }
  }

  void
  Receiver_exec_i::run (void)
  {
    if (this->ticker_)
      {
        this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
        delete this->ticker_;
        this->ticker_ = 0;
      }
    this->test_all ();
  }

  ::CORBA::UShort
  Receiver_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Receiver_exec_i::iterations (::CORBA::UShort iterations)
  {
    this->iterations_ = iterations;
  }

  ::CORBA::UShort
  Receiver_exec_i::keys (void)
  {
    return this->keys_;
  }

  void
  Receiver_exec_i::keys (::CORBA::UShort keys)
  {
    this->keys_ = keys;
  }

  // Port operations.
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_get_port_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_read_port_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Different_Test::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ::Different_Test::QueryConditionTestConnector::Getter_var getter =
      this->context_->get_connection_get_port_fresh_data ();
    DDS::Duration_t to;
    to.sec = 5;
    to.nanosec = 0;
    if (! ::CORBA::is_nil (getter))
      {
        getter->time_out (to);
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Unable to set time out.\n"));
      }

    this->set_filters ();
    this->check_filters ();
    this->start_read ();
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
    if (this->ticker_)
      {
        this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
        delete this->ticker_;
        this->ticker_ = 0;
      }
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
/*    if (!this->has_run_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: ")
            ACE_TEXT ("Test did not run: Didn't receive ")
            ACE_TEXT ("the expected number of DATA_ON_READER ")
            ACE_TEXT ("events.\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Finished query filter test.\n")));
      }*/
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Different_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

