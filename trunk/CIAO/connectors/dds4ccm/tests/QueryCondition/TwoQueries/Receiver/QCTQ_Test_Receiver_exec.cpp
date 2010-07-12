// -*- C++ -*-
//
// $Id$

#include "QCTQ_Test_Receiver_exec.h"

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

// Reader also reads already read samples.
#define SAMPLES_PER_KEY_READER (2 + 11)

namespace CIAO_QCTQ_Test_Receiver_Impl
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
    if (this->callback_.check_last ())
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
      samples_expected_ (0),
      samples_received_ (0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }


  bool
  Receiver_exec_i::check_last ()
  {
    ::QCTQ_Test::QueryConditionTestConnector::Reader_var reader =
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

  CORBA::ULong
  Receiver_exec_i::read_all (void)
  {
    ::QCTQ_Test::QueryConditionTestConnector::Reader_var reader =
      this->context_->get_connection_get_port_data ();

    QueryConditionTestSeq queryfiltertest_info_seq;
    ::CCM_DDS::ReadInfoSeq readinfo_seq;
    reader->read_all (queryfiltertest_info_seq, readinfo_seq);
    if (queryfiltertest_info_seq.length () == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::read_all : "
                              "No samples available in Reader!\n"));
      }
    if (this->current_max_iteration_ != 0 && this->current_max_iteration_ != 0)
      {
        for (CORBA::ULong it = 0; it < queryfiltertest_info_seq.length (); ++it)
          {
            this->check_iter (queryfiltertest_info_seq[it], "READ");
          }
      }
    return queryfiltertest_info_seq.length ();
  }

  CORBA::ULong
  Receiver_exec_i::test_all (void)
  {
    const char * test = "READ ALL";
    try
      {
        return read_all ();
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
    return 0;
  }

  void
  Receiver_exec_i::check_filter (CORBA::UShort run)
  {
    ::CCM_DDS::QueryFilter_var filter;
    ::QCTQ_Test::QueryConditionTestConnector::Reader_var reader =
      this->context_->get_connection_get_port_data ();
    try
      {
        filter = reader->query ();
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        if (run == 3)
          {
            ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::check_filter - "
                                  "caught expected InternalEr excep: retval <%u>\n",
                                  ex.error_code));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                                  "caught InternalError exception: retval <%u>\n",
                                  ex.error_code));
          }
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
                              "Passed check_filter test for run <%d>.\n",
                              run));
      }
  }

  void
  Receiver_exec_i::set_filter (CORBA::UShort run)
  {
    ACE_DEBUG ((LM_DEBUG, "Set filter for run <%d>\n", run));
    ::QCTQ_Test::QueryConditionTestConnector::Reader_var reader =
      this->context_->get_connection_get_port_data ();

    ::CCM_DDS::QueryFilter filter;
    filter.expression = CORBA::string_dup (QUERY);
    filter.parameters.length (2);
    if (run == 1)
      {
        filter.parameters[0] = CORBA::string_dup (MIN_ITERATION_1);
        filter.parameters[1] = CORBA::string_dup (MAX_ITERATION_1);
        this->current_min_iteration_ = ACE_OS::atoi (MIN_ITERATION_1);
        this->current_max_iteration_ = ACE_OS::atoi (MAX_ITERATION_1);
      }
    else if (run == 2)
      {
        filter.parameters[0] = CORBA::string_dup (MIN_ITERATION_2);
        filter.parameters[1] = CORBA::string_dup (MAX_ITERATION_2);
        this->current_min_iteration_ = ACE_OS::atoi (MIN_ITERATION_2);
        this->current_max_iteration_ = ACE_OS::atoi (MAX_ITERATION_2);
      }
    else if (run == 3)
      {
        filter.expression = CORBA::string_dup ("");
        filter.parameters.length (2);
        filter.parameters[0] = CORBA::string_dup ("0");
        filter.parameters[1] = CORBA::string_dup ("0");
        this->current_min_iteration_ = 0;
        this->current_max_iteration_ = 0;
      }
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
          this->samples_received_ += this->test_all ();
          this->check_filter (run);
          //set filter for the next run
          this->set_filter (run + 1);
          this->restarter_->restart_write ();
        }
        break;
      case 2:
        {
          this->samples_received_ += this->test_all ();
          check_filter (run);
          //set filter for the next run
          this->set_filter (run + 1);
          this->restarter_->restart_write ();
        }
        break;
      case 3:
        {
          CORBA::ULong all = this->test_all ();
          if (all != static_cast <CORBA::ULong>(run * this->keys_ * this->iterations_))
            {
              ACE_ERROR ((LM_ERROR, "Receiver_exec_i::run - "
                                    "ERROR: Did not receive the expected "
                                    "number of samples: "
                                    "expected <%u> - received <%u>\n",
                                    run * this->keys_ * this->iterations_,
                                    all));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::run - "
                                    "Receive the expected number of samples: "
                                    "expected <%u> - received <%u>\n",
                                    run * this->keys_ * this->iterations_,
                                    all));
            }
          this->check_filter (run);
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

    this->samples_expected_ = (this->keys_) * SAMPLES_PER_KEY_READER;
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

  ::CCM_TwoQueriesStarter_ptr
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
      ::QCTQ_Test::CCM_Receiver_Context::_narrow (ctx);
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
    //set filter for the first run
    this->set_filter (1);
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
    if (this->samples_received_ != this->samples_expected_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: READGET READER : "
                              "Unexpected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_received_,
                              this->samples_expected_));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "READGET : READER : "
                              "Expected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_received_,
                              this->samples_expected_));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_QCTQ_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

