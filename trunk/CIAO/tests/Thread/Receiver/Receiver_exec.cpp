// $Id$


#include "Receiver_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_Thread_Receiver_Impl
{
  //============================================================
  // Component Executor Implementation Class: Receiver_exec_i
  //============================================================

  Receiver_exec_i::Receiver_exec_i (const ACE_Thread_ID &thr)
    : iterations_ (3),
    thr_(0,0)
  {
    // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);
    this->thr_ = thr;

    char thr_org_str [65];
    thr.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Receiver: -4- Receiver_exec_i::Receiver_exec_i"
                              " Thread current <%C>, Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread mmmm
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::Receiver_exec_i Unexpected "
                              "thread switch from  <%C> to  <%C>\n",
                              thr_org_str,thr_cur_str));
      }
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
      // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Receiver: -10- Receiver_exec_i::~Receiver_exec_i"
                              " Thread current <%C>, Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::~Receiver_exec_i Unexpected "
                               "thread switch from  <%C> to  <%C>\n",
                               thr_org_str,thr_cur_str));
      }
  }

  // Supported operations and attributes.

  // Component attributes and port operations.

  void
  Receiver_exec_i::push_click_in (
    ::Thread::TimeOut * ev)
  {
    /* Your code here. */
    ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("Receiver - Informed by the Sender with message [%C]\n"),
          ev->data ()));

      // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::push_click_in "
                              "Thread current <%C>, Thread original <%C>\n",
                               thr_cur_str, thr_org_str));

    //compare current and original thread
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::push_click_in Unexpected thread"
                              " switch from  <%C> to  <%C>\n",
                              thr_org_str,thr_cur_str));
      }

    Thread::ReadMessage_var rev = this->context_->get_connection_read_message ();
    if (CORBA::is_nil (rev.in ()))
      throw CORBA::BAD_INV_ORDER ();
    for (CORBA::Short i = 0; i < this->iterations_; ++i)
      {
        CORBA::String_var str = rev->get_message ();
        ACE_DEBUG ((LM_DEBUG,
                  "Receiver - Got message from the sender [%C].\n",
                  str.in ()));
        ACE_Time_Value tv (1, 0);
        ACE_OS::sleep (tv);
      }
  }

  ::CORBA::Short
  Receiver_exec_i::iterations (void)
  {
    /* Your code here. */
    return this->iterations_;
  }

  void
  Receiver_exec_i::iterations (
    ::CORBA::Short iterations)
  {
    /* Your code here. */
    this->iterations_ = iterations;
  }

  // Operations from Components::SessionComponent.

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {

    // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Receiver: -5- Receiver_exec_i::"
                              "set_session_context Thread current <%C>,"
                              " Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::set_session_context "
                    "Unexpected thread switch from  <%C> to  <%C>\n",
                     thr_org_str,thr_cur_str));
      }

    this->context_ =
      ::Thread::CCM_Receiver_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
      // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Receiver: -6- Receiver_exec_i::"
                              "configuration_complete Thread current <%C>, "
                              "Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::configuration_complete "
                              "Unexpected thread switch from  <%C> to  <%C>\n",
                               thr_org_str,thr_cur_str));
      }
 }

  void
  Receiver_exec_i::ccm_activate (void)
  {
      // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Receiver: -7- Receiver_exec_i::ccm_activate "
                              "Thread current <%C>, Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::ccm_activate Unexpected thread"
                              " switch from  <%C> to  <%C>\n",
                              thr_org_str,thr_cur_str));
      }
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
     // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Receiver: -8- Receiver_exec_i::ccm_passivate"
                              " Thread current <%C>, Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::ccm_passivate Unexpected thread"
                              " switch from  <%C> to  <%C>\n",
                              thr_org_str,thr_cur_str));
      }
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
      // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Receiver: -9- Receiver_exec_i::ccm_remove "
                              "Thread current <%C>, Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::ccm_remove Unexpected thread"
                              " switch from  <%C> to  <%C>\n",
                              thr_org_str,thr_cur_str));
      }
  }
}

namespace CIAO_Thread_Receiver_Impl
{
  //============================================================
  // Home Executor Implementation Class: ReceiverHome_exec_i
  //============================================================

  ReceiverHome_exec_i::ReceiverHome_exec_i (const ACE_Thread_ID &thr)
  {
    // get current thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    thr.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Receiver: -2- ReceiverHome_exec_i::"
                              "ReceiverHome_exec_i Thread current <%C>,"
                              " Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(thr_cur != thr)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::ReceiverHome_exec_i "
                              "Unexpected thread switch from  <%C> to  <%C>\n",
                               thr_org_str,thr_cur_str));
      }
    this->thr_ = thr;
  }

  ReceiverHome_exec_i::~ReceiverHome_exec_i (void)
  {
    // get current thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Receiver: -11- ReceiverHome_exec_i::"
                              "~ReceiverHome_exec_i Thread current <%C>,"
                              " Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(thr_cur != this->thr_)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::~ReceiverHome_exec_i "
                              "Unexpected thread switch from  <%C> to  <%C>\n",
                              thr_org_str,thr_cur_str));
      }
  }

  // All operations and attributes.

  // Factory operations.

  // Finder operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  ReceiverHome_exec_i::create (void)
  {
    // get current thread
    ACE_Thread_ID thr;
    char thr_str [65];
    thr.to_string (thr_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);


    ACE_DEBUG ((LM_DEBUG, "Receiver: -3- ReceiverHome_exec_i::create "
                              "Thread current <%C>, Thread original <%C>\n",
                              thr_str, thr_org_str));
    if(this->thr_ != thr)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::create : Unexpected thread "
                              "switch from  <%C> to  <%C>\n",
                              thr_org_str,thr_str));
      }

    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      Receiver_exec_i (this->thr_),
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" RECEIVER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Thread_ReceiverHome_Impl (void)
  {
     //get current thread
     ACE_Thread_ID thr;
     char thr_str [65];
     thr.to_string (thr_str);
     ACE_DEBUG ((LM_DEBUG, "Receiver: -1- create_Thread_ReceiverHome_Impl "
                               "Thread <%C>\n", thr_str));

    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_NORETURN (
      retval,
      ReceiverHome_exec_i (thr));

    return retval;
  }
}

