// -*- C++ -*-
//
// $Id$

#include "Getter_Test_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

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
                                     CORBA::Long iteration)
  {
    this->callback_.start_get_one (key, iteration);
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
                                CORBA::Long iteration)
    : callback_ (callback),
      key_ (key),
      iteration_ (iteration)
  {
  }

  GetOneHandler::~GetOneHandler ()
  {
  }

  int
  GetOneHandler::handle_exception (ACE_HANDLE)
  {
    this->callback_.get_one (this->key_, this->iteration_);
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
                                          CORBA::Long iteration)
  {
    GetOneHandler* rh = new  GetOneHandler (*this, CORBA::string_dup (key), iteration);
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
    CIAO_DEBUG ((LM_DEBUG, CLINFO "Receiver_exec_i::get_many: "
                                  "Start getting data from DDS: "
                                  "#keys <%d> - #iterations <%d> with timeout: "
                                  "sec <%d> - nanosec <%u>\n",
                                  keys, iterations,
                                  this->getter_->time_out ().sec,
                                  this->getter_->time_out ().nanosec));

    GetterTest_Seq *gettertest_seq;
    ::CCM_DDS::ReadInfoSeq *readinfo;
    bool result = this->getter_->get_many (gettertest_seq, readinfo);
    if (result)
      {
        if (gettertest_seq->length () == 0)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "Receiver_exec_i::get_many: "
                                          "No data returned. "
                                          "number of samples: "
                                          "expected at least one - received <0>\n"));
          }
        for (CORBA::ULong i = 0; i < gettertest_seq->length (); ++i)
          {
            CIAO_DEBUG ((LM_DEBUG, CLINFO "Receiver_exec_i::get_many: "
                                          "Returned data : key <%C> - iteration <%d>\n",
                                          (*gettertest_seq)[i].key.in (),
                                          (*gettertest_seq)[i].iteration));
          }
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, CLINFO "ERROR: GET MANY: "
                                      "Time out occurred\n"));
      }
  }

  void 
  Receiver_exec_i::get_one (const char * key, CORBA::Long iteration)
  {
    DDS::Duration_t to;
    to.sec = 5;
    to.nanosec = 0;
    this->getter_->time_out (to);
    CIAO_DEBUG ((LM_DEBUG, CLINFO "Receiver_exec_i::get_one: "
                                  "Start getting data from DDS: "
                                  "key <%C> - iteration <%d> "
                                  " with timeout: "
                                  "sec <%d> - nanosec <%u>\n",
                                  key, iteration,
                                  this->getter_->time_out ().sec,
                                  this->getter_->time_out ().nanosec));
    GetterTest *gettertest_info = new GetterTest;
    ::CCM_DDS::ReadInfo readinfo;
    bool result = this->getter_->get_one (gettertest_info, readinfo);
    if (result)
      {
        if (ACE_OS::strcmp (gettertest_info->key, key) != 0)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "ERROR: GET ONE: "
                                          "Expected key does "
                                          "not match received key: "
                                          "expected <%C> - received <%C>\n",
                                          key,
                                          gettertest_info->key.in ()));
          }
        if (gettertest_info->iteration != iteration)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "ERROR: GET ONE: "
                                          "Expected iteration does "
                                          "not match received iteration: "
                                          "expected <%d> - received <%d>\n",
                                          iteration,
                                          gettertest_info->iteration));
          }
        // check readinfo struct.
        if (readinfo.instance_handle.isValid)
          {
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: GET MANY: ")
                    ACE_TEXT ("received instance handle should be invalid ")
                    ACE_TEXT ("for unkeyed data: ")
                    ACE_TEXT ("key <%C> - iteration <%u>\n"),
                    gettertest_info->key.in (),
                    gettertest_info->iteration));
          }
        if (readinfo.source_timestamp.sec == 0 &&
            readinfo.source_timestamp.nanosec == 0)
          {
            CIAO_ERROR ((LM_ERROR, "ERROR: READ ONE LAST: "
                                "source timestamp seems to be invalid (nil) "
                                "key <%C> - iteration <%d>\n",
                                gettertest_info->key.in (),
                                gettertest_info->iteration));
          }
        CIAO_DEBUG ((LM_DEBUG, CLINFO "Receiver_exec_i::get_one: "
                                      "Returned data : key <%C> - iteration <%d>\n",
                                      gettertest_info->key.in (),
                                      gettertest_info->iteration));
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, CLINFO "ERROR: GET ONE: "
                                      "Time out while waiting for "
                                      "key <%C> - iteration <%d>\n",
                                      key,
                                      iteration));
      }
  }

  void
  Receiver_exec_i::timeout_get_one ()
  {
    try
      {
        DDS::Duration_t to;
        to.sec = 1;
        to.nanosec = 0;
        this->getter_->time_out (to);
        CIAO_DEBUG ((LM_DEBUG, CLINFO "Receiver_exec_i::timeout_get_one: "
                                      "Start getting data from DDS: timeout: "
                                      "sec <%d> - nanosec <%u>\n",
                                      this->getter_->time_out ().sec,
                                      this->getter_->time_out ().nanosec));
        GetterTest *gettertest_info = new GetterTest;
        ::CCM_DDS::ReadInfo readinfo;
        bool result = this->getter_->get_one (gettertest_info, readinfo);
        if (result)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "ERROR: TIMEOUT GET ONE: "
                                          "Returning true when get no data.\n"));
          }
        else
          {
            CIAO_DEBUG ((LM_DEBUG, CLINFO "Receiver_exec_i::timeout_get_one: "
                                          "Expected to return no data.\n"));
          }
      }
    catch (const CCM_DDS::InternalError& )
      {
        CIAO_ERROR ((LM_ERROR, "ERROR: TIMEOUT GET ONE: "
                               "Caught unexcepted InternalError "
                               "exception\n"));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: TIMEOUT GET ONE:");
        CIAO_ERROR ((LM_ERROR,
                "ERROR: Receiver_exec_i::timeout_get_one : Exception caught\n"));
      }
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
        CIAO_DEBUG ((LM_DEBUG, CLINFO "Receiver_exec_i::timeout_get_many: "
                                      "Start getting data from DDS: timeout: "
                                      "sec <%d> - nanosec <%u>\n",
                                      this->getter_->time_out ().sec,
                                      this->getter_->time_out ().nanosec));
        GetterTest_Seq *gettertest_seq;
        ::CCM_DDS::ReadInfoSeq *readinfo;
        bool result = this->getter_->get_many (gettertest_seq, readinfo);
        if (result)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "ERROR: TIMEOUT GET MANY: "
                                          "Returning true when get no data.\n"));
          }
        else
          {
            CIAO_DEBUG ((LM_DEBUG, CLINFO "Receiver_exec_i::timeout_get_many: "
                                          "Expected to return no data.\n"));
          }
      }
    catch (const CCM_DDS::InternalError& )
      {
        CIAO_ERROR ((LM_ERROR, "ERROR: TIMEOUT GET MANY: "
                               "Caught unexcepted InternalError "
                               "exception\n"));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: TIMEOUT GET MANY:");
        CIAO_ERROR ((LM_ERROR,
                      "ERROR: Receiver_exec_i::timeout_get_many : Exception caught\n"));
      }
  }

  ::CCM_DDS::GetterTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    return 0;
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_get_status (void)
  {
    return 0;
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
