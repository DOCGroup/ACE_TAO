// -*- C++ -*-
//
// $Id$

#include "ReadGet_Test_Receiver_exec.h"

#include "ace/OS_NS_unistd.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#define QUERY "( (iteration > %0) AND (iteration < %1) )"

#define MIN_ITERATION_1 "2"
#define MAX_ITERATION_1 "5"

#define MIN_ITERATION_2 "22"
#define MAX_ITERATION_2 "34"

//First run filtered in : 2 (iteration 3 and 4)
//Second run filtered in : 11 (iterations between 22 and 34)
#define SAMPLES_PER_KEY_GETTER (2 + 11)

// Reader also reads already read samples.
#define SAMPLES_PER_KEY_READER (2 + 11)

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
    : iterations_ (20),
      keys_ (5),
      current_min_iteration_ (ACE_OS::atoi (MIN_ITERATION_1)),
      current_max_iteration_ (ACE_OS::atoi (MAX_ITERATION_1)),
      ticker_ (0),
      samples_expected_getter_ (0),
      samples_received_getter_ (0),
      samples_expected_reader_ (0),
      samples_received_reader_ (0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }


  bool
  Receiver_exec_i::check_last ()
  {
    ::ReadGet_Test::QueryConditionTestConnector::Reader_var reader =
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
                              "last iteration <%02d> - <%02d>\n",
                               queryfiltertest_info.iteration,
                               this->current_max_iteration_ - 1));
        return queryfiltertest_info.iteration >= this->current_max_iteration_ - 1;
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
  Receiver_exec_i::check_iter (const QueryConditionTest & sample,
                               const char * test)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%C ALL : ")
        ACE_TEXT ("sample received for <%C>: iteration <%02u>\n"),
        test,
        sample.symbol.in (),
        sample.iteration));
    if (sample.iteration <= ACE_OS::atoi (MIN_ITERATION_1))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: %C ALL: "
                              "Didn't expect samples with iterations "
                              "<= %02d\n",
                              test,
                              this->current_min_iteration_));
      }
    if (sample.iteration > this->current_max_iteration_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: %C ALL: "
                              "Didn't expect samples with iterations "
                              "> %02d\n",
                              test,
                              this->current_max_iteration_));
      }
  }


  void
  Receiver_exec_i::read_all (void)
  {
    ::ReadGet_Test::QueryConditionTestConnector::Reader_var reader =
      this->context_->get_connection_get_port_data ();

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
        this->check_iter (queryfiltertest_info_seq[it], "READ");
      }
    this->samples_received_reader_ += queryfiltertest_info_seq.length ();
  }

  void
  Receiver_exec_i::get_all (void)
  {
    ::ReadGet_Test::QueryConditionTestConnector::Getter_var getter =
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
        this->check_iter (qf_info.in (), "GET");
        ++this->samples_received_getter_;
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
            this->check_iter (qf_info.in (), "GET");
            ++this->samples_received_getter_;
          }
      }
  }

  void
  Receiver_exec_i::check_all (void)
  {
    ::ReadGet_Test::QueryConditionTestConnector::Reader_var reader =
      this->context_->get_connection_check_port_data ();

    QueryConditionTestSeq queryfiltertest_info_seq;
    ::CCM_DDS::ReadInfoSeq readinfo_seq;
    reader->read_all (queryfiltertest_info_seq, readinfo_seq);

    for (CORBA::ULong it = 0; it < queryfiltertest_info_seq.length (); ++it)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\t\tCHECK ALL : ")
            ACE_TEXT ("sample received for <%C>: iteration <%02u> - ")
            ACE_TEXT ("access_status <%d> - instance_status <%d>\n"),
            queryfiltertest_info_seq[it].symbol.in (),
            queryfiltertest_info_seq[it].iteration,
            readinfo_seq[it].access_status,
            readinfo_seq[it].instance_status));
      }
    CORBA::ULong expected = 0;
    if (this->current_min_iteration_ == ACE_OS::atoi (MIN_ITERATION_1))
      {
        expected = this->iterations_ * this->keys_;
      }
    else if (this->current_min_iteration_ == ACE_OS::atoi (MIN_ITERATION_2))
      {
        expected = this->iterations_ * this->keys_ * 2;
      }
    if (queryfiltertest_info_seq.length () == expected)
      {
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::check_all - "
                    "OK : All samples received on the DDS_Read port: "
                    "expected <%02u> - received <%02u>\n",
                    expected,
                    queryfiltertest_info_seq.length ()));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::check_all - "
                    "ERROR: Unexpected number of samples received "
                    "on the DDS_Read port: "
                    "expected <%u> - received <%u>\n",
                    expected,
                    queryfiltertest_info_seq.length ()));
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
        // reading all samples on a different port.
         check_all ();
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
  Receiver_exec_i::test_exception ()
  {
    ::CCM_DDS::QueryFilter_var query;
    ::ReadGet_Test::QueryConditionTestConnector::Reader_var reader =
      this->context_->get_connection_get_port_data ();

    try
      {
        query = reader->query ();
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::test_excep - "
                              "Expected InternalErr excep caught : retval <%u>\n",
                              ex.error_code));
        return;
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Receiver_exec_i::test_exception: ");
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::test_exception - "
                              "Unexpected exception caught\n"));
        return;
      }
    ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::test_exception - "
                          "No exception caught while retrieving unset filter.\n"));
  }

  void
  Receiver_exec_i::check_filter ()
  {
    ::CCM_DDS::QueryFilter_var filter;
    ::ReadGet_Test::QueryConditionTestConnector::Reader_var reader =
      this->context_->get_connection_get_port_data ();
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
    if (ACE_OS::strcmp (filter->expression, QUERY) != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                              "Unexpected query when retrieving filter: "
                              "expected <%C> - received <%C>\n",
                              QUERY, filter->expression.in ()));
        error = true;
      }
    //check current parameters.
    if (filter->parameters.length () != 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                              "Unexpected number of parameters: "
                              "expected <%d> - received <%d>\n",
                              2, filter->parameters.length ()));
        error = true;
      }

    if (filter->parameters.length () >= 1)
      {
        if (ACE_OS::atoi (filter->parameters[0]) != this->current_min_iteration_)
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                                  "Unexpected param value: "
                                  "expected <%d> - received <%C>\n",
                                  this->current_min_iteration_,
                                  filter->parameters[0].in ()));
            error = true;
          }
      }
    if (filter->parameters.length () >= 2)
      {
        if (ACE_OS::atoi (filter->parameters[1]) != this->current_max_iteration_)
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                                  "Unexpected param value: "
                                  "expected <%d> - received <%C>\n",
                                  this->current_max_iteration_,
                                  filter->parameters[1].in ()));
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
        ::ReadGet_Test::QueryConditionTestConnector::Reader_var reader =
          this->context_->get_connection_get_port_data ();

        ::CCM_DDS::QueryFilter filter;
        filter.expression = CORBA::string_dup (QUERY);
        filter.parameters.length (2);
        filter.parameters[0] = CORBA::string_dup (MIN_ITERATION_2);
        filter.parameters[1] = CORBA::string_dup (MAX_ITERATION_2);
        reader->query (filter);
        this->current_min_iteration_ = ACE_OS::atoi (MIN_ITERATION_2);
        this->current_max_iteration_ = ACE_OS::atoi (MAX_ITERATION_2);

        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::test_set_query_parameters - "
                    "New query parameters set (<%C> - <%C>)!\n",
                    MIN_ITERATION_2, MAX_ITERATION_2));
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::test_set_query_parameters - "
                              "caught InternalError exception: retval <%u>\n",
                              ex.error_code));
        return;
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Receiver_exec_i::test_set_query_parameters: ");
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::test_set_query_parameters - "
                              "Exception caught\n"));
        return;
      }
  }

  void
  Receiver_exec_i::set_filter ()
  {
    ACE_DEBUG ((LM_DEBUG, "Set filter\n"));
    ::ReadGet_Test::QueryConditionTestConnector::Reader_var reader =
      this->context_->get_connection_get_port_data ();

    ::CCM_DDS::QueryFilter filter;
    filter.expression = CORBA::string_dup (QUERY);
    filter.parameters.length (2);
    filter.parameters[0] = CORBA::string_dup (MIN_ITERATION_1);
    filter.parameters[1] = CORBA::string_dup (MAX_ITERATION_1);
    reader->query (filter);
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
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::run - "
                          "Starting run number <%d>\n",
                          run));
    switch (run)
    {
      case 1:
        {
          this->test_exception ();
          this->set_filter ();
          this->restarter_->restart_write ();
        }
        break;
      case 2:
        {
          this->test_all ();
          check_filter ();
          this->test_set_query_parameters ();
          this->restarter_->restart_write ();
        }
        break;
      case 3:
        {
          this->test_all ();
          this->check_filter ();
        }
        break;
    }
  }

  void
  Receiver_exec_i::iterations (::CORBA::UShort iterations)
  {
    this->iterations_ = iterations;

  }

  void
  Receiver_exec_i::keys (::CORBA::UShort keys)
  {
    this->keys_ = keys;

    this->samples_expected_getter_ = (this->keys_ - 1) * SAMPLES_PER_KEY_GETTER;
    this->samples_expected_reader_ = (this->keys_) * SAMPLES_PER_KEY_READER;
  }

  // Port operations.
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_get_port_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_check_port_status (void)
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
    ::ReadGet_Test::QueryConditionTestConnector::Getter_var getter =
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
    this->restarter_ = this->context_->get_connection_writer_restart ();
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
    if (this->samples_received_getter_ != this->samples_expected_getter_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: READGET GETTER : "
                              "Unexpected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_getter_,
                              this->samples_received_getter_));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "READGET : GETTER : "
                              "Expected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_getter_,
                              this->samples_received_getter_));
      }
    if (this->samples_received_reader_ != this->samples_expected_reader_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: READGET READER : "
                              "Unexpected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_received_reader_,
                              this->samples_expected_reader_));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "READGET : READER : "
                              "Expected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_received_reader_,
                              this->samples_expected_reader_));
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

