// -*- C++ -*-
//
// $Id$

#include "QueryFilter_Test_Receiver_exec.h"

#include "ace/OS_NS_unistd.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ciao/Logger/Log_Macros.h"

#define QUERY "( (iteration > %0) AND (iteration < %1) )"

#define MIN_ITERATION_1 "2"
#define MAX_ITERATION_1 "5"

#define MIN_ITERATION_2 "12"
#define MAX_ITERATION_2 "15"


namespace CIAO_QueryFilter_Test_Receiver_Impl
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
      this->callback_.run (this->run_);
    return 0;
  }

  //============================================================
  // ReadHandler
  //============================================================
  ReadHandler::ReadHandler (Receiver_exec_i &callback,
                            CORBA::UShort run)
    : callback_ (callback),
      run_ (run)
  {
  }

  int
  ReadHandler::handle_exception (ACE_HANDLE)
  {
    this->callback_.run (this->run_);
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
      current_min_iteration_ (ACE_OS::atoi (MIN_ITERATION_1)),
      current_max_iteration_ (ACE_OS::atoi (MAX_ITERATION_1)),
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
        QueryFilterTest queryfiltertest_info;
        ::CCM_DDS::ReadInfo readinfo;
        char key[100];
        ACE_OS::sprintf (key, "KEY_%d", this->keys_);
        queryfiltertest_info.symbol = CORBA::string_dup (key);
        this->reader_->read_one_last (
                queryfiltertest_info,
                readinfo,
                ::DDS::HANDLE_NIL);
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::check_last - "
                              "last iteration <%d> - <%d>\n",
                               queryfiltertest_info.iteration,
                               current_max_iteration_ - 1));
        return queryfiltertest_info.iteration >= current_max_iteration_ - 1;
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
  Receiver_exec_i::read_all (void)
  {
    try
      {
        QueryFilterTest_Seq     *queryfiltertest_info_seq;
        ::CCM_DDS::ReadInfoSeq  *readinfo_seq;
        this->reader_->read_all (
                queryfiltertest_info_seq,
                readinfo_seq);

        for (CORBA::ULong it = 0; it < queryfiltertest_info_seq->length (); ++it)
          {
            if ((*readinfo_seq)[it].access_status == ::CCM_DDS::FRESH_INFO)
              {
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ALL : ")
                    ACE_TEXT ("sample received for <%C>: iteration <%u>\n"),
                    (*queryfiltertest_info_seq)[it].symbol.in (),
                    (*queryfiltertest_info_seq)[it].iteration));
                if ((*queryfiltertest_info_seq)[it].iteration <=
                      this->current_min_iteration_)
                  {
                    ACE_ERROR ((LM_ERROR, "ERROR: READ ALL: "
                                          "Didn't expect samples with iterations "
                                          "<= %d\n",
                                          this->current_min_iteration_));
                  }
                if ((*queryfiltertest_info_seq)[it].iteration >
                    this->current_max_iteration_)
                  {
                    ACE_ERROR ((LM_ERROR, "ERROR: READ ALL: "
                                          "Didn't expect samples with iterations "
                                          "> %d\n",
                                          this->current_max_iteration_));
                  }
              }
          }
      }
    catch (const CCM_DDS::NonExistent& ex)
      {
        for (CORBA::ULong i = 0; i < ex.indexes.length (); ++i)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ALL: ")
                  ACE_TEXT ("caught expected exception: index <%u>\n"),
                  ex.indexes[i]));
          }
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ALL: ")
              ACE_TEXT ("caught InternalError exception: retval <%u>\n"),
              ex.error_code));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: READ ALL: ");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::read_all : Exception caught\n")));
      }
  }

  void
  Receiver_exec_i::test_exception ()
  {
    CCM_DDS::QueryFilter * filter = 0;
    try
      {
        filter = this->reader_->filter ();
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::test_excep - "
                              "Expected InternalErr excep caught : retval <%u>\n",
                              ex.error_code));
        return;
      }
    catch (const CORBA::Exception& ex)
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
    CCM_DDS::QueryFilter * filter = 0;
    try
      {
        filter = this->reader_->filter ();
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                              "caught InternalError exception: retval <%u>\n",
                              ex.error_code));
        return;
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Receiver_exec_i::check_filter: ");
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                              "Exception caught\n"));
        return;
      }

    //check query
    bool error = false;
    if (ACE_OS::strcmp (filter->query, QUERY) != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                              "Unexpected query when retrieving filter: "
                              "expected <%C> - received <%C>\n",
                              QUERY, filter->query.in ()));
        error = true;
      }
    //check current parameters.
    if (filter->query_parameters.length () != 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                              "Unexpected number of parameters: "
                              "expected <%d> - received <%d>\n",
                              2, filter->query_parameters.length ()));
        error = true;
      }

    if (filter->query_parameters.length () >= 1)
      {
        if (ACE_OS::atoi (filter->query_parameters[0]) != this->current_min_iteration_)
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                                  "Unexpected query when retrieving filter: "
                                  "expected <%C> - received <%C>\n",
                                  QUERY, filter->query.in ()));
            error = true;
          }
      }
    if (filter->query_parameters.length () >= 2)
      {
        if (ACE_OS::atoi (filter->query_parameters[1]) != this->current_max_iteration_)
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                                  "Unexpected query when retrieving filter: "
                                  "expected <%C> - received <%C>\n",
                                  QUERY, filter->query.in ()));
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
        CCM_DDS::QueryFilter filter;
        filter.query = CORBA::string_dup ("na");
        filter.query_parameters.length (2);
        filter.query_parameters[0] = CORBA::string_dup (MIN_ITERATION_2);
        filter.query_parameters[1] = CORBA::string_dup (MAX_ITERATION_2);
        this->reader_->filter (filter);
        this->current_min_iteration_ = ACE_OS::atoi (MIN_ITERATION_2);
        this->current_max_iteration_ = ACE_OS::atoi (MAX_ITERATION_2);
        ACE_OS::sleep (2);
        this->restarter_->restart_write ();
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
  Receiver_exec_i::start (CORBA::UShort run)
  {
    ReadHandler *rh = new ReadHandler (*this, run);
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->notify (rh);
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
          read_all ();
          check_filter ();
          test_set_query_parameters ();
        }
        break;
      case 2:
        {
          read_all ();
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

  // Port operations.
  ::CCM_DDS::QueryFilterTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    return 0;
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return 0;
  }

  ::CCM_QueryFilterStarter_ptr
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
      ::QueryFilter_Test::CCM_Receiver_Context::_narrow (ctx);
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
    this->reader_ = this->context_->get_connection_info_out_data();
    this->restarter_ = this->context_->get_connection_writer_restart ();

    test_exception ();

    CCM_DDS::QueryFilter filter;
    filter.query = CORBA::string_dup (QUERY);
    filter.query_parameters.length (2);
    filter.query_parameters[0] = CORBA::string_dup (MIN_ITERATION_1);
    filter.query_parameters[1] = CORBA::string_dup (MAX_ITERATION_1);
    this->reader_->filter (filter);
}

  void
  Receiver_exec_i::ccm_passivate (void)
  {
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
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Finished query filter test.")));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_QueryFilter_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

