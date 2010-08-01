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

#define ITER2_VALUE1 "22"
#define ITER2_VALUE2 "34"

// Samples received on the getter port:
// 2 during the first run.
// 12 during the second run
// Beware that the last key (key no 5) is
// not read by the getter since this one is
// already read during check_last
// (see constructor Receiver_exec_i).
#define SAMPLES_PER_KEY_GETTER (2 + 11)


// Samples received on the Reader port depend on the
// number of iterations send. The sender informs the
// Receiver about this number by invoking set_reader_properties.
// The number of expected samples on the receiver port are set
// when the iterations are set (see Receiver_exec_i::iterations).
// Besides this, this test also reads on the reader port. This'll
// result in the same amount of samples of the read on the getter
// port.


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
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Checking if last sample ")
                          ACE_TEXT ("is available in DDS...\n")));
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
      has_run_ (false),
      current_iter_value1_ (ACE_OS::atoi (ITER1_VALUE1)),
      current_iter_value2_ (ACE_OS::atoi (ITER1_VALUE2)),
      ticker_ (0),
      samples_expected_reader_ (0),
      samples_received_reader_ (0),
      samples_expected_getter_ (0),
      samples_received_getter_ (0)
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
        ::ReadGet_Test::QueryConditionTestConnector::Reader_var get_reader =
          this->context_->get_connection_get_port_data ();
        if (::CORBA::is_nil (get_reader.in ()))
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_last - ")
                                  ACE_TEXT ("Reader of DDS_Get is nil.\n")));
            return false;
          }
        QueryConditionTest queryfiltertest_info;
        ::CCM_DDS::ReadInfo readinfo;
        char key[10];
        ACE_OS::sprintf (key, "KEY_%d", this->keys_);
        queryfiltertest_info.symbol = CORBA::string_dup (key);
        get_reader->read_one_last (
                queryfiltertest_info,
                readinfo,
                ::DDS::HANDLE_NIL);
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver_exec_i::check_last - ")
                              ACE_TEXT ("last iteration <%d> - <%d>\n"),
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
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: READ ALL ON READER PORT : ")
                              ACE_TEXT ("Didn't expect samples between ")
                              ACE_TEXT ("%d and %d\n"),
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
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: %C ALL ON GETTER PORT : ")
                              ACE_TEXT ("Didn't expect samples with iterations ")
                              ACE_TEXT ("<= %d\n"),
                              test,
                              this->current_iter_value1_));
      }
    if (sample.iteration > this->current_iter_value2_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: %C ALL ON GETTER PORT : ")
                              ACE_TEXT ("Didn't expect samples with iterations ")
                              ACE_TEXT ("> %d\n"),
                              test,
                              this->current_iter_value2_));
      }
  }

  void
  Receiver_exec_i::read_all_on_reader_port (void)
  {
    ::ReadGet_Test::QueryConditionTestConnector::Reader_var read_reader =
      this->context_->get_connection_read_port_data ();

    if (::CORBA::is_nil (read_reader))
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Receiver_exec_i::get_all - ")
                              ACE_TEXT ("ERROR: No Reader on Reader port\n")));
        return;
      }
    QueryConditionTestSeq queryfiltertest_info_seq;
    ::CCM_DDS::ReadInfoSeq readinfo_seq;
    read_reader->read_all (queryfiltertest_info_seq, readinfo_seq);
    if (queryfiltertest_info_seq.length () == 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : Receiver_exec_i::read_all : ")
                              ACE_TEXT ("No samples available in Reader!\n")));
      }
    for (CORBA::ULong it = 0; it < queryfiltertest_info_seq.length (); ++it)
      {
        this->check_iter_on_reader_port (queryfiltertest_info_seq[it]);
      }
    this->samples_received_reader_ += queryfiltertest_info_seq.length ();
  }

  void
  Receiver_exec_i::read_all_on_getter_port (void)
  {
    ::ReadGet_Test::QueryConditionTestConnector::Reader_var get_reader =
      this->context_->get_connection_get_port_data ();

    if (::CORBA::is_nil (get_reader))
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::get_all - ")
                              ACE_TEXT ("No Reader on Getter port\n")));
        return;
      }
    QueryConditionTestSeq queryfiltertest_info_seq;
    ::CCM_DDS::ReadInfoSeq readinfo_seq;
    get_reader->read_all (queryfiltertest_info_seq, readinfo_seq);
    if (queryfiltertest_info_seq.length () == 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : Receiver_exec_i::read_all : ")
                              ACE_TEXT ("No samples available in Reader!\n")));
      }
    for (CORBA::ULong it = 0; it < queryfiltertest_info_seq.length (); ++it)
      {
        this->check_iter_on_getter_port (queryfiltertest_info_seq[it], "READ");
      }
    this->samples_received_reader_ += queryfiltertest_info_seq.length ();
  }

  void
  Receiver_exec_i::get_all_on_getter_port (void)
  {
    ::ReadGet_Test::QueryConditionTestConnector::Getter_var get_getter =
      this->context_->get_connection_get_port_fresh_data ();

    if (::CORBA::is_nil (get_getter))
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::get_all - ")
                              ACE_TEXT ("No Getter on Getter port\n")));
        return;
      }
    //set time_out on getter
    DDS::Duration_t to;
    to.sec = 5;
    to.nanosec = 0;
    get_getter->time_out (to);

    QueryConditionTest_var qf_info;;
    ::CCM_DDS::ReadInfo readinfo;
    CORBA::Boolean result = get_getter->get_one (qf_info.out (), readinfo);
    if (result)
      {
        this->check_iter_on_getter_port (*qf_info, "GET");
        ++this->samples_received_getter_;
      }
    else
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::get_all - ")
                              ACE_TEXT ("time out when retrieving ")
                              ACE_TEXT ("first sample.\n")));
      }
    while (result)
      {
        result = get_getter->get_one (qf_info.out (), readinfo);
        if (result)
          {
            this->check_iter_on_getter_port (*qf_info, "GET");
            ++this->samples_received_getter_;
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
        ACE_DEBUG ((LM_DEBUG, "=================================="
                              "==================================\n"));

        test = "READ ALL GETTER";
        read_all_on_getter_port ();
        ACE_DEBUG ((LM_DEBUG, "=================================="
                              "==================================\n"));

        test = "READ ALL READER";
        read_all_on_reader_port ();
        ACE_DEBUG ((LM_DEBUG, "=================================="
                              "==================================\n"));
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
  Receiver_exec_i::check_filter ()
  {


    this->check_filter_getter ();
    this->check_filter_reader ();
  }

  void
  Receiver_exec_i::check_filter_getter (void)
  {
    ::CCM_DDS::QueryFilter * filter = 0;
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver_exec_i::check_filter <GETTER> - ")
                          ACE_TEXT ("checking filter\n")));

    try
      {
        ::ReadGet_Test::QueryConditionTestConnector::Getter_var get_getter =
          this->context_->get_connection_get_port_fresh_data ();
        if (::CORBA::is_nil (get_getter.in ()))
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <GETTER> - ")
                                  ACE_TEXT ("Unable to get getter interface\n")));
            throw ::CCM_DDS::InternalError ();
          }
        CORBA::Object_var cmp = get_getter->_get_component ();
        if (::CORBA::is_nil (cmp.in ()))
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <GETTER> - ")
                                  ACE_TEXT ("Unable to get component interface\n")));
            throw ::CCM_DDS::InternalError ();
          }
        ::ReadGet_Test::QueryConditionTestConnector::CCM_DDS_State_var conn =
          ::ReadGet_Test::QueryConditionTestConnector::CCM_DDS_State::_narrow (cmp.in ());
        if (::CORBA::is_nil (conn.in ()))
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <GETTER> - ")
                                  ACE_TEXT ("Unable to narrow connector interface\n")));
            throw ::CCM_DDS::InternalError ();
          }

        filter = conn->pull_observer_filter ();
        if (!filter)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <GETTER> - ")
                                  ACE_TEXT ("Filter is nil\n")));
            return;
          }
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <GETTER> - ")
                              ACE_TEXT ("caught InternalError exception: retval <%u>\n"),
                              ex.error_code));
        return;
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Receiver_exec_i::check_filter: ");
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <GETTER> - ")
                              ACE_TEXT ("Exception caught\n")));
        return;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <GETTER> - ")
                              ACE_TEXT ("caught unknown exception\n")));
        return;
      }

    //check query
    bool error = false;
    if (ACE_OS::strcmp (filter->expression, QUERY_GETTER_PORT) != 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <GETTER> - ")
                              ACE_TEXT ("Unexpected query when retrieving filter: ")
                              ACE_TEXT ("expected <%C> - received <%C>\n"),
                              QUERY_GETTER_PORT, filter->expression.in ()));
        error = true;
      }
    //check current parameters.
    if (filter->parameters.length () != 2)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <GETTER> - ")
                              ACE_TEXT ("Unexpected number of parameters: ")
                              ACE_TEXT ("expected <%d> - received <%d>\n"),
                              2, filter->parameters.length ()));
        error = true;
      }

    if (filter->parameters.length () >= 1)
      {
        if (ACE_OS::atoi (filter->parameters[0]) != this->current_iter_value1_)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <GETTER> - ")
                                  ACE_TEXT ("Unexpected param value: ")
                                  ACE_TEXT ("expected <%d> - received <%C>\n"),
                                  this->current_iter_value1_,
                                  filter->parameters[0].in ()));
            error = true;
          }
      }
    if (filter->parameters.length () >= 2)
      {
        if (ACE_OS::atoi (filter->parameters[1]) != this->current_iter_value2_)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <GETTER> - ")
                                  ACE_TEXT ("Unexpected param value: ")
                                  ACE_TEXT ("expected <%d> - received <%C>\n"),
                                  this->current_iter_value2_,
                                  filter->parameters[1].in ()));
            error = true;
          }
      }
    if (!error)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver_exec_i::check_filter <GETTER> - ")
                              ACE_TEXT ("Passed check_filter test.\n")));
      }
  }

  void
  Receiver_exec_i::check_filter_reader (void)
  {
    ::CCM_DDS::QueryFilter * filter = 0;
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver_exec_i::check_filter <READER> - ")
                          ACE_TEXT ("checking filter\n")));

    try
      {
        ::ReadGet_Test::QueryConditionTestConnector::Reader_var reader =
          this->context_->get_connection_read_port_data ();
        if (::CORBA::is_nil (reader.in ()))
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <READER> - ")
                                  ACE_TEXT ("Unable to get getter interface\n")));
            throw ::CCM_DDS::InternalError ();
          }
        CORBA::Object_var cmp = reader->_get_component ();
        if (::CORBA::is_nil (cmp.in ()))
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <READER> - ")
                                  ACE_TEXT ("Unable to get component interface\n")));
            throw ::CCM_DDS::InternalError ();
          }
        ::ReadGet_Test::QueryConditionTestConnector::CCM_DDS_State_var conn =
          ::ReadGet_Test::QueryConditionTestConnector::CCM_DDS_State::_narrow (cmp.in ());
        if (::CORBA::is_nil (conn.in ()))
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <READER> - ")
                                  ACE_TEXT ("Unable to narrow connector interface\n")));
            throw ::CCM_DDS::InternalError ();
          }

        filter = conn->passive_observer_filter ();
        if (!filter)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <READER> - ")
                                  ACE_TEXT ("Filter is nil\n")));
            return;
          }
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <READER> - ")
                              ACE_TEXT ("caught InternalError exception: retval <%u>\n"),
                              ex.error_code));
        return;
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Receiver_exec_i::check_filter: ");
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <READER> - ")
                              ACE_TEXT ("Exception caught\n")));
        return;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <READER> - ")
                              ACE_TEXT ("caught unknown exception\n")));
        return;
      }

    //check query
    bool error = false;
    if (ACE_OS::strcmp (filter->expression, QUERY_READER_PORT) != 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <READER> - ")
                              ACE_TEXT ("Unexpected query when retrieving filter: ")
                              ACE_TEXT ("expected <%C> - received <%C>\n"),
                              QUERY_READER_PORT, filter->expression.in ()));
        error = true;
      }
    //check current parameters.
    if (filter->parameters.length () != 2)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <READER> - ")
                              ACE_TEXT ("Unexpected number of parameters: ")
                              ACE_TEXT ("expected <%d> - received <%d>\n"),
                              2, filter->parameters.length ()));
        error = true;
      }

    if (filter->parameters.length () >= 1)
      {
        if (ACE_OS::atoi (filter->parameters[0]) != this->current_iter_value1_)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <READER> - ")
                                  ACE_TEXT ("Unexpected param value: ")
                                  ACE_TEXT ("expected <%d> - received <%C>\n"),
                                  this->current_iter_value1_,
                                  filter->parameters[0].in ()));
            error = true;
          }
      }
    if (filter->parameters.length () >= 2)
      {
        if (ACE_OS::atoi (filter->parameters[1]) != this->current_iter_value2_)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::check_filter <READER> - ")
                                  ACE_TEXT ("Unexpected param value: ")
                                  ACE_TEXT ("expected <%d> - received <%C>\n"),
                                  this->current_iter_value2_,
                                  filter->parameters[1].in ()));
            error = true;
          }
      }
    if (!error)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver_exec_i::check_filter <READER> - ")
                              ACE_TEXT ("Passed check_filter test.\n")));
      }
  }

  void
  Receiver_exec_i::test_set_query_parameters ()
  {
    try
      {
        ::CCM_DDS::ContentFilterSetting_var get_filter_setting =
          this->context_->get_connection_get_port_filter_config ();
        if (::CORBA::is_nil (get_filter_setting.in ()))
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::test_set_query_parameters - ")
                                  ACE_TEXT ("Filter config interface for Getter port is nil.\n")));
            throw ::CCM_DDS::InternalError ();
          }
        ::CCM_DDS::ContentFilterSetting_var read_filter_setting =
          this->context_->get_connection_read_port_filter_config ();
        if (::CORBA::is_nil (read_filter_setting.in ()))
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::test_set_query_parameters - ")
                                  ACE_TEXT ("Filter config interface for Reader port is nil.\n")));
            throw ::CCM_DDS::InternalError ();
          }

        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver_exec_i::test_set_query_parameters: Set parameters\n")));
        ::DDS::StringSeq parameters;
        parameters.length (2);
        parameters[0] = CORBA::string_dup (ITER2_VALUE1);
        parameters[1] = CORBA::string_dup (ITER2_VALUE2);
        get_filter_setting->set_filter_parameters (parameters);
        read_filter_setting->set_filter_parameters (parameters);

        this->current_iter_value1_ = ACE_OS::atoi (ITER2_VALUE1);
        this->current_iter_value2_ = ACE_OS::atoi (ITER2_VALUE2);
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("DONE Receiver_exec_i::test_set_query_parameters\n")));
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::test_set_query_parameters - ")
                              ACE_TEXT ("caught InternalError exception: retval <%u>\n"),
                              ex.error_code));
        return;
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Receiver_exec_i::test_set_query_parameters: ");
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::test_set_query_parameters - ")
                              ACE_TEXT ("Exception caught\n")));
        return;
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::test_set_query_parameters - ")
                              ACE_TEXT ("caught unknown exception\n")));
        return;
      }
  }

  void
  Receiver_exec_i::start_read (CORBA::UShort run)
  {
    ACE_NEW_THROW_EX (this->ticker_,
                      read_action_Generator (*this, run),
                      ::CORBA::INTERNAL ());
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                                          this->ticker_,
                                          0,
                                          ACE_Time_Value(1, 0),
                                          ACE_Time_Value(1, 0)) == -1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to schedule Timer\n")));
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

    ReadGetRestarter_var restarter = this->context_->get_connection_writer_restart ();
    if (::CORBA::is_nil (restarter.in ()))
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver_exec_i::run - ")
                              ACE_TEXT ("Unable to start run since restarter ")
                              ACE_TEXT ("is nil.\n")));
        return;
      }

    this->has_run_ = true;

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver_exec_i::run - ")
                          ACE_TEXT ("Starting run number <%d>\n"),
                          run));
    switch (run)
    {
      case 1:
        {
          check_filter ();
          restarter->restart_write ();
        }
        break;
      case 2:
        {
          test_all ();
          check_filter ();
          test_set_query_parameters ();
          restarter->restart_write ();
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

  void
  Receiver_exec_i::iterations (::CORBA::UShort iterations)
  {
    this->iterations_ = iterations;
    // during first iteration, 2,3,4 and 5 are filtered out
    // during the second iteration 22-34 are fitered out but the
    // iterations of the first run are also received during the
    // second run
    // The last addition is from the read action on the getter port.
    // During the first run, two samples are received. The second run 
    // includes the two samples from the first run and 11 samples (22-34)
    // from the second run.
    this->samples_expected_reader_ =
      ((this->iterations_ - 4) * this->keys_ * 2) +
      ((this->iterations_ - 13) * this->keys_) +
      ((2 + 2 + 11) * this->keys_);
  }

  void
  Receiver_exec_i::keys (::CORBA::UShort keys)
  {
    this->keys_ = keys;
    this->samples_expected_getter_ = (keys_ - 1) * SAMPLES_PER_KEY_GETTER;
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
  Receiver_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::ReadGet_Test::CCM_Receiver_Context::_narrow (ctx);
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
    if (this->samples_received_reader_ != this->samples_expected_reader_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: READGET READER: "
                              "Unexpected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_reader_,
                              this->samples_received_reader_));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "READGET READER: "
                              "Expected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_reader_,
                              this->samples_received_reader_));
      }
    if (this->samples_received_getter_ != this->samples_expected_getter_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: READGET GETTER: "
                              "Unexpected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_getter_,
                              this->samples_received_getter_));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "READGET GETTER: "
                              "Expected number of samples received: "
                              "expected <%d> - received <%d>\n",
                              this->samples_expected_getter_,
                              this->samples_received_getter_));
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

