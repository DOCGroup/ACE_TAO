// // -*- C++ -*-
//
// $Id$

#include "Getter_Test_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/OS_NS_sys_time.h"

namespace CIAO_Getter_Test_Receiver_Impl
{
  //============================================================
  // Invoker_exec_i
  //============================================================
  Invoker_exec_i::Invoker_exec_i (Receiver_exec_i & callback)
    : callback_ (callback)
  {
  }

  Invoker_exec_i::~Invoker_exec_i (void)
  {
  }

  void
  Invoker_exec_i::start_timeout_get_one ()
  {
    this->callback_.timeout_get_one ();
  }

  void
  Invoker_exec_i::start_timeout_get_many ()
  {
    this->callback_.timeout_get_many ();
  }

  void
  Invoker_exec_i::start_get_one (const char * key,
                                 ::CORBA::Long fixed_key,
                                 ::CORBA::Long iteration)
  {
    this->callback_.start_get_one (key, fixed_key, iteration);
  }

  void
  Invoker_exec_i::start_get_many (::CORBA::Short keys, ::CORBA::Long iterations)
  {
    this->callback_.start_get_many (keys, iterations);
  }

  //============================================================
  // GetOneHandler
  //============================================================
  GetOneHandler::GetOneHandler (Receiver_exec_i &callback,
                                const char * key,
                                CORBA::Long fixed_key,
                                CORBA::Long iteration)
    : callback_ (callback),
      key_ (key),
      fixed_key_ (fixed_key),
      iteration_ (iteration)
  {
  }

  GetOneHandler::~GetOneHandler ()
  {
  }

  int
  GetOneHandler::handle_exception (ACE_HANDLE)
  {
    this->callback_.get_one (this->key_, this->fixed_key_, this->iteration_);
    return 0;
  }

  //============================================================
  // GetManyHandler
  //============================================================
  GetManyHandler::GetManyHandler (Receiver_exec_i &callback,
                                  CORBA::UShort keys,
                                  CORBA::Long iterations)
    : callback_ (callback),
      keys_ (keys),
      iterations_ (iterations)
  {
  }

  GetManyHandler::~GetManyHandler ()
  {
  }

  int
  GetManyHandler::handle_exception (ACE_HANDLE)
  {
    this->callback_.get_many (this->keys_, this->iterations_);
    return 0;
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  void
  Receiver_exec_i::start_get_one (const char * key,
                                  CORBA::Long fixed_key,
                                  CORBA::Long iteration)
  {
    GetOneHandler* rh = new  GetOneHandler (*this,
                                            CORBA::string_dup (key),
                                            fixed_key,
                                            iteration);
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->notify (rh);
  }

  void
  Receiver_exec_i::start_get_many (CORBA::Short keys,
                         CORBA::Long iterations)
  {
    GetManyHandler* rh = new  GetManyHandler (*this, keys, iterations);
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->notify (rh);
  }

  void
  Receiver_exec_i::get_many (CORBA::Short keys , CORBA::Long iterations)
  {
    // this is very hard to test in a controlled environment.
    // When data arrives in DDS, the waiting ends and the
    // Getter starts to read the data. The number of samples
    // read wil (almost) always be one.
    // On the other hand, when the user want to have all the
    // samples in DDS, one shouldn't use the wait method.
    // Since the spec is not clear about this, the test will
    // pass when at least one sample is returned.
    // Also, max_delivered_data cannot be tested since only
    // one sample is returned.
    DDS::Duration_t to;
    to.sec = 10;
    to.nanosec = 0;
    this->getter_->time_out (to);
    this->getter_->max_delivered_data (40);
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::get_many: "
                          "Start getting data from DDS: "
                          "#keys <%d> - #iterations <%d> with timeout: "
                          "sec <%d> - nanosec <%u>\n",
                          keys, iterations,
                          this->getter_->time_out ().sec,
                          this->getter_->time_out ().nanosec));

    GetterTestSeq gettertest_seq;
    ::CCM_DDS::ReadInfoSeq readinfo;
    bool const result = this->getter_->get_many (gettertest_seq, readinfo);
    if (result)
      {
        if (gettertest_seq.length () == 0)
          {
            ACE_ERROR ((LM_ERROR, "Receiver_exec_i::get_many: "
                                  "No data returned. "
                                  "number of samples: "
                                  "expected at least one - received <0>\n"));
          }
        for (CORBA::ULong i = 0; i < gettertest_seq.length (); ++i)
          {
            ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::get_many: "
                                  "Returned data : key <%C> - iteration <%d>\n",
                                  gettertest_seq[i].key.in (),
                                  gettertest_seq[i].iteration));
          }
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: GET MANY: "
                              "Time out occurred\n"));
      }
  }

  void
  Receiver_exec_i::get_one_fixed (CORBA::Long fixed_key, CORBA::Long iteration)
  {
    DDS::Duration_t to;
    to.sec = 5;
    to.nanosec = 0;
    this->fixed_->time_out (to);
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::get_one_fixed: "
                          "Start getting data from DDS: "
                          "key <%u> - iteration <%d> "
                          " with timeout: "
                          "sec <%u> - nanosec <%u>\n",
                          fixed_key, iteration,
                          this->getter_->time_out ().sec,
                          this->getter_->time_out ().nanosec));
    GetterFixed gettertest_info;
    ::CCM_DDS::ReadInfo readinfo;
    ACE_Time_Value tv = ACE_OS::gettimeofday ();
    bool result = this->fixed_->get_one (gettertest_info, readinfo);
    if (result)
      {
        ACE_Time_Value dur = ACE_OS::gettimeofday () - tv;
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::get_one_fixed: "
                               "get_one took <%#T>\n",
                               &dur));
        if (gettertest_info.key != fixed_key)
          {
            ACE_ERROR ((LM_ERROR, "ERROR FIXED: GET ONE: "
                                  "Expected key does "
                                  "not match received key: "
                                  "expected <%u> - received <%C>\n",
                                  fixed_key,
                                  gettertest_info.key));
          }
        if (gettertest_info.iteration != iteration)
          {
            ACE_ERROR ((LM_ERROR, "ERROR FIXED: GET ONE: "
                                  "Expected iteration does "
                                  "not match received iteration: "
                                  "expected <%d> - received <%d>\n",
                                  iteration,
                                  gettertest_info.iteration));
          }
        // check readinfo struct.
        if (readinfo.instance_handle.isValid)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR FIXED: GET MANY: ")
                    ACE_TEXT ("received instance handle should be invalid ")
                    ACE_TEXT ("for unkeyed data: ")
                    ACE_TEXT ("key <%u> - iteration <%u>\n"),
                    gettertest_info.key,
                    gettertest_info.iteration));
          }
        if (readinfo.source_timestamp.sec == 0 &&
            readinfo.source_timestamp.nanosec == 0)
          {
            ACE_ERROR ((LM_ERROR, "ERROR FIXED: READ ONE LAST: "
                                "source timestamp seems to be invalid (nil) "
                                "key <%u> - iteration <%d>\n",
                                gettertest_info.key,
                                gettertest_info.iteration));
          }
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::get_one_fixed: "
                              "Returned data : key <%u> - iteration <%d>\n",
                              gettertest_info.key,
                              gettertest_info.iteration));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR FIXED: GET ONE: "
                              "Time out while waiting for "
                              "key <%u> - iteration <%d>\n",
                              fixed_key,
                              iteration));
      }
  }

  void
  Receiver_exec_i::get_one_variable (const char * key, CORBA::Long iteration)
  {
    DDS::Duration_t to;
    to.sec = 5;
    to.nanosec = 0;
    this->getter_->time_out (to);
    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::get_one_variable: "
                          "Start getting data from DDS: "
                          "key <%C> - iteration <%d> "
                          " with timeout: "
                          "sec <%d> - nanosec <%u>\n",
                          key, iteration,
                          this->getter_->time_out ().sec,
                          this->getter_->time_out ().nanosec));
    GetterTest *gettertest_info = new GetterTest;
    ::CCM_DDS::ReadInfo readinfo;
    ACE_Time_Value tv = ACE_OS::gettimeofday ();
    bool result = this->getter_->get_one (gettertest_info, readinfo);
    if (result)
      {
        ACE_Time_Value dur = ACE_OS::gettimeofday () - tv;
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::get_one_variable: "
                               "get_one took <%#T>\n",
                               &dur));
        if (ACE_OS::strcmp (gettertest_info->key, key) != 0)
          {
            ACE_ERROR ((LM_ERROR, "ERROR VARIABLE: GET ONE: "
                                  "Expected key does "
                                  "not match received key: "
                                  "expected <%C> - received <%C>\n",
                                  key,
                                  gettertest_info->key.in ()));
          }
        if (gettertest_info->iteration != iteration)
          {
            ACE_ERROR ((LM_ERROR, "ERROR VARIABLE: GET ONE: "
                                  "Expected iteration does "
                                  "not match received iteration: "
                                  "expected <%d> - received <%d>\n",
                                  iteration,
                                  gettertest_info->iteration));
          }
        // check readinfo struct.
        if (readinfo.instance_handle.isValid)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR VARIABLE: GET MANY: ")
                    ACE_TEXT ("received instance handle should be invalid ")
                    ACE_TEXT ("for unkeyed data: ")
                    ACE_TEXT ("key <%C> - iteration <%u>\n"),
                    gettertest_info->key.in (),
                    gettertest_info->iteration));
          }
        if (readinfo.source_timestamp.sec == 0 &&
            readinfo.source_timestamp.nanosec == 0)
          {
            ACE_ERROR ((LM_ERROR, "ERROR VARIABLE: READ ONE LAST: "
                                "source timestamp seems to be invalid (nil) "
                                "key <%C> - iteration <%d>\n",
                                gettertest_info->key.in (),
                                gettertest_info->iteration));
          }
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::get_one_variable: "
                              "Returned data : key <%C> - iteration <%d>\n",
                              gettertest_info->key.in (),
                              gettertest_info->iteration));
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR VARIABLE: GET ONE: "
                              "Time out while waiting for "
                              "key <%C> - iteration <%d>\n",
                              key,
                              iteration));
      }
  }

  void
  Receiver_exec_i::get_one (const char * key,
                            CORBA::Long fixed_key,
                            CORBA::Long iteration)
  {
    this->get_one_variable (key, iteration);
    this->get_one_fixed (fixed_key, iteration);
  }

  void
  Receiver_exec_i::timeout_get_one_fixed ()
  {
    try
      {
        DDS::Duration_t to;
        to.sec = 1;
        to.nanosec = 0;
        this->fixed_->time_out (to);
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::timeout_get_one_fixed: "
                              "Start getting data from DDS: timeout: "
                              "sec <%d> - nanosec <%u>\n",
                              this->fixed_->time_out ().sec,
                              this->fixed_->time_out ().nanosec));
        GetterFixed gettertest_info;
        ::CCM_DDS::ReadInfo readinfo;
        bool result = this->fixed_->get_one (gettertest_info, readinfo);
        if (result)
          {
            ACE_ERROR ((LM_ERROR, "ERROR FIXED: TIMEOUT GET ONE: "
                                  "Returning true when get no data.\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::timeout_get_one_fixed: "
                                  "Expected to return no data.\n"));
          }
      }
    catch (const CCM_DDS::InternalError& )
      {
        ACE_ERROR ((LM_ERROR, "ERROR FIXED: TIMEOUT GET ONE: "
                               "Caught unexcepted InternalError "
                               "exception\n"));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR FIXED: TIMEOUT GET ONE:");
        ACE_ERROR ((LM_ERROR,
                "ERROR: Receiver_exec_i::timeout_get_one_fixed : Exception caught\n"));
      }
  }

  void
  Receiver_exec_i::timeout_get_one_variable ()
  {
    try
      {
        DDS::Duration_t to;
        to.sec = 1;
        to.nanosec = 0;
        this->getter_->time_out (to);
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::timeout_get_one_variable: "
                              "Start getting data from DDS: timeout: "
                              "sec <%d> - nanosec <%u>\n",
                              this->getter_->time_out ().sec,
                              this->getter_->time_out ().nanosec));
        GetterTest *gettertest_info = new GetterTest;
        ::CCM_DDS::ReadInfo readinfo;
        bool result = this->getter_->get_one (gettertest_info, readinfo);
        if (result)
          {
            ACE_ERROR ((LM_ERROR, "ERROR VARIABLE: TIMEOUT GET ONE: "
                                  "Returning true when get no data.\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::timeout_get_one_variable: "
                                  "Expected to return no data.\n"));
          }
      }
    catch (const CCM_DDS::InternalError& )
      {
        ACE_ERROR ((LM_ERROR, "ERROR VARIABLE: TIMEOUT GET ONE: "
                               "Caught unexcepted InternalError "
                               "exception\n"));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR VARIABLE: TIMEOUT GET ONE:");
        ACE_ERROR ((LM_ERROR,
                "ERROR: Receiver_exec_i::timeout_get_one_variable : Exception caught\n"));
      }
  }

  void
  Receiver_exec_i::timeout_get_one ()
  {
    this->timeout_get_one_variable ();
    this->timeout_get_one_fixed ();
  }

  void
  Receiver_exec_i::timeout_get_many ()
  {
    try
      {
        DDS::Duration_t to;
        to.sec = 1;
        to.nanosec = 0;
        this->getter_->time_out (to);
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::timeout_get_many: "
                              "Start getting data from DDS: timeout: "
                              "sec <%d> - nanosec <%u>\n",
                              this->getter_->time_out ().sec,
                              this->getter_->time_out ().nanosec));
        GetterTestSeq gettertest_seq;
        ::CCM_DDS::ReadInfoSeq readinfo;
        bool const result = this->getter_->get_many (gettertest_seq, readinfo);
        if (result)
          {
            ACE_ERROR ((LM_ERROR, "ERROR: TIMEOUT GET MANY: "
                                  "Returning true when get no data.\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::timeout_get_many: "
                                  "Expected to return no data.\n"));
          }
      }
    catch (const CCM_DDS::InternalError&)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: TIMEOUT GET MANY: "
                               "Caught unexcepted InternalError "
                               "exception\n"));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: TIMEOUT GET MANY:");
        ACE_ERROR ((LM_ERROR,
                      "ERROR: Receiver_exec_i::timeout_get_many : Exception caught\n"));
      }
  }

  ::Getter_Test::GetterTestConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    return ::Getter_Test::GetterTestConnector::CCM_Listener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_get_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_fixed_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_GetInvoker_ptr
  Receiver_exec_i::get_getter_invoke ()
  {
    return new Invoker_exec_i (*this);
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Getter_Test::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    this->getter_ = this->context_->get_connection_info_get_fresh_data ();
    this->fixed_ = this->context_->get_connection_info_fixed_fresh_data ();
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Getter_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
