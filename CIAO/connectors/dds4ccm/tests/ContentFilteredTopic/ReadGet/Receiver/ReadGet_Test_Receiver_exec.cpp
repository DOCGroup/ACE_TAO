// -*- C++ -*-
// $Id$

#include "ReadGet_Test_Receiver_exec.h"

#include "ace/OS_NS_unistd.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#define QUERY_GETTER_PORT "( (iteration > %0) AND (iteration < %1) )"
#define QUERY_READER_PORT "( (iteration < %0) OR (iteration > %1) )"

// initial values set in deploymentplan as well.
// Need to be the same
#define ITER1_VALUE1 "2"
#define ITER1_VALUE2 "5"

#define ITER2_VALUE1 "12"
#define ITER2_VALUE2 "25"

namespace CIAO_ReadGet_Test_Receiver_Impl
{
  //============================================================
  // read_action_Generator
  //============================================================
  read_action_Generator::read_action_Generator (Receiver_exec_i &callback, int run)
    : callback_ (callback),
      run_ (run)
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
    if (this->run_ == 1 || this->callback_.check_last ())
      {
        this->callback_.run (this->run_);
      }
    return 0;
  }

  //============================================================
  // Starter_exec_i
  //============================================================
  Starter_exec_i::Starter_exec_i (Receiver_exec_i & callback)
    : callback_ (callback)
  {
  }

  Starter_exec_i::~Starter_exec_i (void)
  {
  }

  void
  Starter_exec_i::set_reader_properties (CORBA::UShort nr_keys,
                                         CORBA::UShort nr_iterations)
  {
    this->callback_.keys (nr_keys);
    this->callback_.iterations (nr_iterations);
  }

  void
  Starter_exec_i::start_read (CORBA::UShort run)
  {
    this->callback_.start_read (run);
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : iterations_ (10),
      keys_ (5),
      has_run_ (false),
      run_tests_ (true),
      current_iter_value1_ (ACE_OS::atoi (ITER1_VALUE1)),
      current_iter_value2_ (ACE_OS::atoi (ITER1_VALUE2)),
      ticker_ (0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }


  bool
  Receiver_exec_i::check_last ()
  {
    try
      {
        QueryConditionTest queryfiltertest_info;
        ::CCM_DDS::ReadInfo readinfo;
        char key[100];
        ACE_OS::sprintf (key, "KEY_%d", this->keys_);
        queryfiltertest_info.symbol = CORBA::string_dup (key);
        this->get_reader_->read_one_last (
                queryfiltertest_info,
                readinfo,
                ::DDS::HANDLE_NIL);
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::check_last - "
                              "last iteration <%d> - <%d>\n",
                               queryfiltertest_info.iteration,
                               this->current_iter_value2_ - 1));
        return queryfiltertest_info.iteration >= this->current_iter_value2_ - 1;
      }
    catch (...)
      {
        // no need to catch. An error is given
        // when this example didn't run at all.
      }
    return false;
  }

  // Supported operations and attributes.
  void
  Receiver_exec_i::check_iter_on_reader_port (const QueryConditionTest & sample)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ALL ON READER PORT : ")
        ACE_TEXT ("sample received for <%C>: iteration <%u>\n"),
        sample.symbol.in (),
        sample.iteration));
    if (sample.iteration > this->current_iter_value1_ &&
        sample.iteration < this->current_iter_value2_ &&
        sample.iteration > 20) //don't count the first run...
      {
        ACE_ERROR ((LM_ERROR, "ERROR: READ ALL ON READER PORT : "
                              "Didn't expect samples between "
                              "%d and %d\n",
                              this->current_iter_value1_,
                              this->current_iter_value2_));
      }
  }

  void
  Receiver_exec_i::check_iter_on_getter_port (const QueryConditionTest & sample,
                                              const char * test)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%C ALL ON GETTER PORT : ")
        ACE_TEXT ("sample received for <%C>: iteration <%u>\n"),
        test,
        sample.symbol.in (),
        sample.iteration));
    if (sample.iteration <= ACE_OS::atoi (ITER1_VALUE1))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: %C ALL ON GETTER PORT : "
                              "Didn't expect samples with iterations "
                              "<= %d\n",
                              test,
                              this->current_iter_value1_));
      }
    if (sample.iteration > this->current_iter_value2_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: %C ALL ON GETTER PORT : "
                              "Didn't expect samples with iterations "
                              "> %d\n",
                              test,
                              this->current_iter_value2_));
      }
  }

  void
  Receiver_exec_i::read_all_on_reader_port (void)
  {
    if (::CORBA::is_nil (this->read_reader_))
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::get_all - "
                              "ERROR: No Reader on Reader port\n"));
        return;
      }
    QueryConditionTestSeq queryfiltertest_info_seq;
    ::CCM_DDS::ReadInfoSeq readinfo_seq;
    this->read_reader_->read_all (queryfiltertest_info_seq, readinfo_seq);
    if (queryfiltertest_info_seq.length () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::read_all : "
                              "No samples available in Reader!\n"));
      }
    for (CORBA::ULong it = 0; it < queryfiltertest_info_seq.length (); ++it)
      {
        this->check_iter_on_reader_port (queryfiltertest_info_seq[it]);
      }
  }

  void
  Receiver_exec_i::read_all_on_getter_port (void)
  {
    ACE_DEBUG ((LM_DEBUG, "=================================="
                          "==================================\n"));
    if (::CORBA::is_nil (this->get_reader_))
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::get_all - "
                              "ERROR: No Reader on Getter port\n"));
        return;
      }
    QueryConditionTestSeq queryfiltertest_info_seq;
    ::CCM_DDS::ReadInfoSeq readinfo_seq;
    this->get_reader_->read_all (queryfiltertest_info_seq, readinfo_seq);
    if (queryfiltertest_info_seq.length () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::read_all : "
                              "No samples available in Reader!\n"));
      }
    for (CORBA::ULong it = 0; it < queryfiltertest_info_seq.length (); ++it)
      {
        this->check_iter_on_getter_port (queryfiltertest_info_seq[it], "READ");
      }
  }

  void
  Receiver_exec_i::get_all_on_getter_port (void)
  {
    if (::CORBA::is_nil (this->get_getter_))
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::get_all - "
                              "ERROR: No Getter on Getter port\n"));
        return;
      }
    QueryConditionTest * qf_info = new QueryConditionTest;
    ::CCM_DDS::ReadInfo readinfo;
    bool result = this->get_getter_->get_one (qf_info, readinfo);
    if (result)
      {
        this->check_iter_on_getter_port (*qf_info, "GET");
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::get_all - "
                              "ERROR: time out when retrieving "
                              "first sample.\n"));
      }
    while (result)
      {
        result = this->get_getter_->get_one (qf_info, readinfo);
        if (result)
          {
            this->check_iter_on_getter_port (*qf_info, "GET");
          }
      }
  }

  void
  Receiver_exec_i::test_all ()
  {
    const char * test = "GET ALL";
    try
      {
        get_all_on_getter_port ();
        test = "READ ALL GETTER";
        read_all_on_getter_port ();
        test = "READ ALL READER";
        read_all_on_reader_port ();
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
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception (test);
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::test_all : Exception caught\n")));
      }
  }

  void
  Receiver_exec_i::check_filter ()
  {
    check_filter (true);
    check_filter (false);
  }

  void
  Receiver_exec_i::check_filter (bool check_reader)
  {
    ::CCM_DDS::QueryFilter * filter = 0;
    const char * port = "";
    check_reader ? port = "Reader" : port = "Getter";
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::check_filter <%C> - "
                          "checking filter\n", port));
    try
      {
        CORBA::Object_var cmp = this->get_getter_->_get_component ();
        if (::CORBA::is_nil (cmp.in ()))
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter <%C> - "
                                  "Unable to get component interface\n",
                                  port));
            throw ::CCM_DDS::InternalError ();
          }
        ::ReadGet_Test::QueryConditionTestConnector::CCM_DDS_State_var conn =
          ::ReadGet_Test::QueryConditionTestConnector::CCM_DDS_State::_narrow (cmp.in ());
        if (::CORBA::is_nil (conn.in ()))
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter <%C> - "
                                  "Unable to narrow connector interface\n",
                                  port));
            throw ::CCM_DDS::InternalError ();
          }

        if (check_reader)
          {
            // todo for marcel, we need an event connecto
            filter = conn->pull_observer_filter ();
          }
        else
          {
            filter = conn->pull_observer_filter ();
          }
        if (!filter)
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter <%C> - "
                                  "Filter is nil\n", port));
            return;
          }
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter <%C> - "
                              "caught InternalError exception: retval <%u>\n",
                              port,
                              ex.error_code));
        return;
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Receiver_exec_i::check_filter: ");
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter <%C> - "
                              "Exception caught\n",
                              port));
        return;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter <%C> - "
                              "caught unknown exception\n",
                              port));
        return;
      }

    //check query
    bool error = false;
    if (ACE_OS::strcmp (filter->expression, QUERY_GETTER_PORT) != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter <%C> - "
                              "Unexpected query when retrieving filter: "
                              "expected <%C> - received <%C>\n",
                              port, QUERY_GETTER_PORT, filter->expression.in ()));
        error = true;
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
        if (ACE_OS::atoi (filter->parameters[0]) != this->current_iter_value1_)
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter <%C> - "
                                  "Unexpected query when retrieving filter: "
                                  "expected <%C> - received <%C>\n",
                                  port, QUERY_GETTER_PORT, filter->expression.in ()));
            error = true;
          }
      }
    if (filter->parameters.length () >= 2)
      {
        if (ACE_OS::atoi (filter->parameters[1]) != this->current_iter_value2_)
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter <%C> - "
                                  "Unexpected query when retrieving filter: "
                                  "expected <%C> - received <%C>\n",
                                  port, QUERY_GETTER_PORT, filter->expression.in ()));
            error = true;
          }
      }
    if (!error)
      {
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::check_filter - "
                              "Passed check_filter test.\n"));
      }
  }

  void
  Receiver_exec_i::test_set_query_parameters ()
  {
    try
      {
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::test_set_query_parameters: Set parameters\n"));
        ::DDS::StringSeq parameters;
        parameters.length (2);
        parameters[0] = CORBA::string_dup (ITER2_VALUE1);
        parameters[1] = CORBA::string_dup (ITER2_VALUE2);
        this->get_filter_setting_->set_filter_parameters (parameters);
        this->read_filter_setting_->set_filter_parameters (parameters);

        this->current_iter_value1_ = ACE_OS::atoi (ITER2_VALUE1);
        this->current_iter_value2_ = ACE_OS::atoi (ITER2_VALUE2);
        ACE_DEBUG ((LM_DEBUG, "DONE Receiver_exec_i::test_set_query_parameters\n"));
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::test_set_query_parameters - "
                              "caught InternalError exception: retval <%u>\n",
                              ex.error_code));
        return;
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Receiver_exec_i::test_set_query_parameters: ");
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::test_set_query_parameters - "
                              "Exception caught\n"));
        return;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::test_set_query_parameters - "
                              "caught unknown exception\n"));
        return;
      }
  }

  void
  Receiver_exec_i::start_read (CORBA::UShort run)
  {
    this->ticker_ = new read_action_Generator (*this, run);
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
  Receiver_exec_i::run (CORBA::UShort run)
  {
    if (this->ticker_)
      {
        this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
        delete this->ticker_;
        this->ticker_ = 0;
      }
    this->has_run_ = true;
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::run - "
                          "Starting run number <%d>\n",
                          run));
    switch (run)
    {
      case 1:
        {
          check_filter ();
          this->restarter_->restart_write ();
        }
        break;
      case 2:
        {
          test_all ();
          check_filter ();
          test_set_query_parameters ();
          this->restarter_->restart_write ();
        }
        break;
      case 3:
        {
          test_all ();
          check_filter ();
        }
        break;
    }
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

  ::CORBA::Boolean
  Receiver_exec_i::run_tests (void)
  {
    return this->run_tests_;
  }

  void
  Receiver_exec_i::run_tests (::CORBA::Boolean run_tests)
  {
    this->run_tests_ = run_tests;
  }


  // Port operations.
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_get_port_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_read_port_status(void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_ReadGetStarter_ptr
  Receiver_exec_i::get_reader_start ()
  {
    return new Starter_exec_i (*this);
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::ReadGet_Test::CCM_Receiver_Context::_narrow (ctx);
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
    if (this->run_tests_)
      {
        this->read_reader_ = this->context_->get_connection_read_port_data ();
        this->get_reader_ = this->context_->get_connection_get_port_data ();
        this->get_getter_ = this->context_->get_connection_get_port_fresh_data ();
        this->get_filter_setting_ = this->context_->get_connection_get_port_filter_config ();
        this->read_filter_setting_ = this->context_->get_connection_read_port_filter_config ();

        if (CORBA::is_nil (this->get_filter_setting_.in ()) ||
            CORBA::is_nil (this->read_filter_setting_.in ()) ||
            CORBA::is_nil (this->read_reader_.in ()) ||
            CORBA::is_nil (this->get_reader_.in ()) ||
            CORBA::is_nil (this->get_getter_.in ()))
          {
            ACE_ERROR ((LM_ERROR, "Receiver_exec_i::ccm_activate : "
                                  "ERROR: Error while retrieving connections\n"
                                  "Filter settings getter <%@>\n"
                                  "Filter settings reader <%@>\n"
                                  "Reader on Reader port <%@>\n"
                                  "Reader on Getter port <%@>\n"
                                  "Getter on Getter port <%@>\n",
                                  this->get_filter_setting_.in (),
                                  this->read_filter_setting_.in (),
                                  this->read_reader_.in (),
                                  this->get_reader_.in (),
                                  this->get_getter_.in ()));
          }

        DDS::Duration_t to;
        to.sec = 5;
        to.nanosec = 0;
        if (! ::CORBA::is_nil (this->get_getter_))
          {
            this->get_getter_->time_out (to);
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Unable to set time out.\n"));
          }
        this->restarter_ = this->context_->get_connection_writer_restart ();
      }
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
    if (!this->has_run_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: ")
            ACE_TEXT ("Test did not run: Didn't receive ")
            ACE_TEXT ("the expected number of DATA_ON_READER ")
            ACE_TEXT ("events.\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Finished query filter test.\n")));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ReadGet_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

