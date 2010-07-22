// -*- C++ -*-
//
// $Id$

#include "QCTQ_Test_Receiver_exec.h"

#include "ace/OS_NS_unistd.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"

#include "dds4ccm/impl/dds4ccm_conf.h"
#include "dds4ccm/impl/Utils.h"

#define QUERY "( (iteration > %0) AND (iteration < %1) )"

#define MIN_ITERATION_1 "2"
#define MAX_ITERATION_1 "5"

#define MIN_ITERATION_2 "7"
#define MAX_ITERATION_2 "9"

#define MIN_ITERATION_3 "68"
#define MAX_ITERATION_3 "77"

// Reader also reads already read samples.
#define SAMPLES_PER_KEY_GETTER (2 + 1 + +17 + 8)

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
  Starter_exec_i::set_reader_properties (::CORBA::UShort nr_keys,
                                         ::CORBA::UShort nr_iterations)
  {
    this->callback_.keys (nr_keys);
    this->callback_.iterations (nr_iterations);
  }

  void
  Starter_exec_i::start_read (::CORBA::UShort run)
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
        queryfiltertest_info.symbol = ::CORBA::string_dup (key);
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
                               ::CORBA::UShort run)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("GET ALL : ")
        ACE_TEXT ("sample received for <%C>: iteration <%02u>\n"),
        sample.symbol.in (),
        sample.iteration));
    if (run == 3)
      {
        // We need to receive all UNread samples. Therefor we should
        // receive all samples except the ones between
        // MIN_ITERATION_1 and MAX_ITERATION_1 and between
        // MIN_ITERATION_2 and MAX_ITERATION_2
        if ((sample.iteration > ACE_OS::atoi (MIN_ITERATION_1)  &&
             sample.iteration < ACE_OS::atoi (MAX_ITERATION_1)) ||
            (sample.iteration > ACE_OS::atoi (MIN_ITERATION_2)  &&
             sample.iteration < ACE_OS::atoi (MAX_ITERATION_2)))
          {
            ACE_ERROR ((LM_ERROR, "ERROR: GET ALL: "
                                  "Didn't except samples between "
                                  "<%02d> and <%02d> and between "
                                  "<%02d> and <%02d>\n",
                                  ACE_OS::atoi (MIN_ITERATION_1),
                                  ACE_OS::atoi (MAX_ITERATION_1),
                                  ACE_OS::atoi (MIN_ITERATION_2),
                                  ACE_OS::atoi (MAX_ITERATION_2)));
          }
      }
    else
      {
        if (sample.iteration <= current_min_iteration_)
          {
            ACE_ERROR ((LM_ERROR, "ERROR: GET ALL: "
                                  "Didn't expect samples with iterations "
                                  "<= %02d\n",
                                  this->current_min_iteration_));
          }
        if (sample.iteration > this->current_max_iteration_)
          {
            ACE_ERROR ((LM_ERROR, "ERROR: GET ALL: "
                                  "Didn't expect samples with iterations "
                                  "> %02d\n",
                                  this->current_max_iteration_));
          }
      }
  }

  ::CORBA::ULong
  Receiver_exec_i::get_all (::CORBA::UShort run)
  {
    ::QCTQ_Test::QueryConditionTestConnector::Getter_var getter =
      this->context_->get_connection_get_port_fresh_data ();
    ::CORBA::ULong samples_received = 0;

    if (::CORBA::is_nil (getter.in ()))
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::get_all - "
                              "ERROR: No Getter\n"));
      }
    QueryConditionTest_var qf_info;
    ::CCM_DDS::ReadInfo readinfo;
    ::CORBA::Boolean result = getter->get_one (qf_info.out (), readinfo);
    if (result)
      {
        this->check_iter (qf_info.in (), run);
        ++samples_received;
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
            this->check_iter (qf_info.in (), run);
            ++samples_received;
          }
      }
    return samples_received;
  }

  ::CORBA::ULong
  Receiver_exec_i::test_all (::CORBA::UShort run)
  {
    try
      {
        return get_all (run);
      }
    catch (const CCM_DDS::NonExistent& ex)
      {
        for (::CORBA::ULong i = 0; i < ex.indexes.length (); ++i)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ERROR test_all <%d>: ")
                  ACE_TEXT ("caught expected exception: index <%u>\n"),
                  run,
                  ex.indexes[i]));
          }
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: test_all <%d>: ")
              ACE_TEXT ("caught InternalError exception: retval <%u>\n"),
              run,
              ex.error_code));
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("test_all");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::test_all <%d> : Exception caught\n"),
          run));
      }
    return 0;
  }

  void
  Receiver_exec_i::check_filter (::CORBA::UShort run)
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
  Receiver_exec_i::set_filter (::CORBA::UShort run)
  {
    ACE_DEBUG ((LM_DEBUG, "Set filter for run <%d>\n", run));
    ::QCTQ_Test::QueryConditionTestConnector::Reader_var reader =
      this->context_->get_connection_get_port_data ();

    try
      {
        ::CCM_DDS::QueryFilter filter;
        run != 3 ? filter.expression = ::CORBA::string_dup (QUERY) :
                   filter.expression = ::CORBA::string_dup ("");
        filter.parameters.length (2);
        if (run == 1)
          {
            filter.parameters[0] = ::CORBA::string_dup (MIN_ITERATION_1);
            filter.parameters[1] = ::CORBA::string_dup (MAX_ITERATION_1);
            this->current_min_iteration_ = ACE_OS::atoi (MIN_ITERATION_1);
            this->current_max_iteration_ = ACE_OS::atoi (MAX_ITERATION_1);
          }
        else if (run == 2)
          {
            filter.parameters[0] = ::CORBA::string_dup (MIN_ITERATION_2);
            filter.parameters[1] = ::CORBA::string_dup (MAX_ITERATION_2);
            this->current_min_iteration_ = ACE_OS::atoi (MIN_ITERATION_2);
            this->current_max_iteration_ = ACE_OS::atoi (MAX_ITERATION_2);
          }
        else if (run == 3)
          {
            filter.parameters[0] = ::CORBA::string_dup ("0");
            filter.parameters[1] = ::CORBA::string_dup ("0");
            this->current_min_iteration_ = (run - 1) * this->iterations_;
            this->current_max_iteration_ = run * this->iterations_;
          }
        else if (run == 4)
          {
            filter.parameters[0] = ::CORBA::string_dup (MIN_ITERATION_3);
            filter.parameters[1] = ::CORBA::string_dup (MAX_ITERATION_3);
            this->current_min_iteration_ = ACE_OS::atoi (MIN_ITERATION_3);
            this->current_max_iteration_ = ACE_OS::atoi (MAX_ITERATION_3);
          }
        reader->query (filter);
      }
    catch (const ::CCM_DDS::InternalError &ex)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::set_filter - "
                    "ERROR: Unexpected InternalError exception caught "
                    "with <%C> as error\n.",
                    ::CIAO::DDS4CCM::translate_retcode (ex.error_code)));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::set_filter - "
                    "ERROR: Unexpected exception caught.\n"));
      }
  }

  void
  Receiver_exec_i::start_read (::CORBA::UShort run)
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
  Receiver_exec_i::run (::CORBA::UShort run)
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
          this->samples_received_ += this->test_all (run);
          this->check_filter (run);
          //set filter for the next run
          this->set_filter (run + 1);
          this->restarter_->restart_write ();
        }
        break;
      case 2:
        {
          this->samples_received_ += this->test_all (run);
          check_filter (run);
          //set filter for the next run
          this->set_filter (run + 1);
          this->restarter_->restart_write ();
        }
        break;
      case 3:
        {
          this->samples_received_ += this->test_all (run);
          this->check_filter (run);
          //set filter for the next run
          this->set_filter (run + 1);
          this->restarter_->restart_write ();
        }
        break;
      case 4:
        {
          this->samples_received_ += this->test_all (run);
        }
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

    this->samples_expected_ = (this->keys_ - 1) * SAMPLES_PER_KEY_GETTER;
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
    ::QCTQ_Test::QueryConditionTestConnector::Getter_var getter =
      this->context_->get_connection_get_port_fresh_data ();
    DDS::Duration_t to;
    to.sec = 5; to.nanosec = 0;
    getter->time_out (to);
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
        ACE_ERROR ((LM_ERROR, "ERROR: READGET GETTER : "
                              "Unexpected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_,
                              this->samples_received_));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "READGET : GETTER : "
                              "Expected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_,
                              this->samples_received_));
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

