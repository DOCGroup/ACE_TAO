// -*- C++ -*-
//
// $Id$

#include "QueryFilter_Test_Receiver_exec.h"

#include "ace/OS_NS_unistd.h"
#include "ciao/Logger/Log_Macros.h"

#define QUERY "( (iteration > %0) AND (iteration < %1) )"

#define MIN_ITERATION_STR "2"
#define MIN_ITERATION 2

#define MAX_ITERATION_1_STR "5"
#define MAX_ITERATION_1 5

#define MAX_ITERATION_2_STR "7"
#define MAX_ITERATION_2 7


namespace CIAO_QueryFilter_Test_Receiver_Impl
{
  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Receiver_exec_i &callback)
    : callback_ (callback),
      has_run_ (false)
  {
  }

  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::ConnectorStatusListener
  void ConnectorStatusListener_exec_i::on_inconsistent_topic(
     ::DDS::Topic_ptr ,
     const DDS::InconsistentTopicStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr ,
     const DDS::RequestedIncompatibleQosStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr ,
     const DDS::SampleRejectedStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr ,
     const DDS::OfferedDeadlineMissedStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr ,
     const DDS::OfferedIncompatibleQosStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind  status_kind)
  {
    if (status_kind == ::DDS::DATA_ON_READERS_STATUS &&
        !this->has_run_ &&
        this->callback_.check_last ())
      {
        this->has_run_ = true;
        this->callback_.run ();
      }
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

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : iterations_ (10),
      keys_ (5),
      has_run_ (false),
      current_max_iteration_ (MAX_ITERATION_1)
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
        return queryfiltertest_info.iteration == MAX_ITERATION_1 - 1;
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
        printf ("READ_ALL\n");
        QueryFilterTest_Seq     *queryfiltertest_info_seq;
        ::CCM_DDS::ReadInfoSeq  *readinfo_seq;
        this->reader_->read_all (
                queryfiltertest_info_seq,
                readinfo_seq);

        for (CORBA::ULong it = 0; it < queryfiltertest_info_seq->length (); ++it)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ALL : ")
                ACE_TEXT ("sample received for <%C>: iteration <%u>\n"),
                (*queryfiltertest_info_seq)[it].symbol.in (),
                (*queryfiltertest_info_seq)[it].iteration));
            if ((*queryfiltertest_info_seq)[it].iteration <= MIN_ITERATION)
              {
                ACE_ERROR ((LM_ERROR, "ERROR: READ ALL: "
                                      "Didn't expect samples with iterations " 
                                      "<= %d\n",
                                      MIN_ITERATION));
              }
            if ((*queryfiltertest_info_seq)[it].iteration > MAX_ITERATION_1)
              {
                ACE_ERROR ((LM_ERROR, "ERROR: READ ALL: "
                                      "Didn't expect samples with iterations " 
                                      "> %d\n",
                                      this->current_max_iteration_));
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
  Receiver_exec_i::check_filter ()
  {
    CCM_DDS::QueryFilter * filter = this->reader_->filter ();
    //check query
    if (ACE_OS::strcmp (filter->query, QUERY) == 0)
      {
        CIAO_ERROR (1, (LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                                  "Unexpected query when retrieving filter: "
                                  "expected <%C> - received <%C>\n",
                                  QUERY, filter->query.in ()));
      }
    //check current parameters.
    if (filter->query_parameters.length () != 2)
      {
        CIAO_ERROR (1, (LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                                  "Unexpected number of parameters: "
                                  "expected <%d> - received <%d>\n",
                                  2, filter->query_parameters.length ()));
      }

    if (filter->query_parameters.length () >= 1)
      {
        if (ACE_OS::strcmp (filter->query_parameters[0], MIN_ITERATION_STR) == 0)
          {
            CIAO_ERROR (1, (LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                                      "Unexpected query when retrieving filter: "
                                      "expected <%C> - received <%C>\n",
                                      QUERY, filter->query.in ()));
          }
      }
    if (filter->query_parameters.length () >= 2)
      {
        if (ACE_OS::strcmp (filter->query_parameters[1], MAX_ITERATION_2_STR) == 0)
          {
            CIAO_ERROR (1, (LM_ERROR, "ERROR: Receiver_exec_i::check_filter - "
                                      "Unexpected query when retrieving filter: "
                                      "expected <%C> - received <%C>\n",
                                      QUERY, filter->query.in ()));
          }
      }
  }

  void
  Receiver_exec_i::run ()
  {
    this->has_run_ = true;
    ACE_OS::sleep (2);
    read_all ();
/*    CCM_DDS::QueryFilter filter;
    filter.query = CORBA::string_dup ("");
    filter.query_parameters.length (2);
    filter.query_parameters[0] = CORBA::string_dup (MIN_ITERATION_STR);
    filter.query_parameters[1] = CORBA::string_dup (MAX_ITERATION_2_STR);
    this->reader_->filter (filter);
    this->restarter_->restart_write ();
    this->current_max_iteration_ = MAX_ITERATION_2;
    ACE_OS::sleep (4);
    read_all ();
    check_filter ();*/
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

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_out_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (*this);
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
    this->reader_ = this->context_->get_connection_info_out_data();
    this->restarter_ = this->context_->get_connection_writer_restart ();
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    CCM_DDS::QueryFilter filter;
    filter.query = CORBA::string_dup (QUERY);
    filter.query_parameters.length (2);
    filter.query_parameters[0] = CORBA::string_dup (MIN_ITERATION_STR);
    filter.query_parameters[1] = CORBA::string_dup (MAX_ITERATION_1_STR);
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

