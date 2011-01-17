// $Id$


#include "Sender_exec.h"
#include "ace/OS_NS_time.h"

namespace CIAO_Thread_Sender_Impl
{
  //============================================================
  // Facet Executor Implementation Class: ReadMessage_exec_i
  //============================================================

  ReadMessage_exec_i::ReadMessage_exec_i (Sender_exec_i& component)
    : component_ (component)
  {
    // get current thread
    ACE_Thread_ID thr;
    char thr_str [65];
    thr.to_string (thr_str);

    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec_i::ReadMessage_exec_i, Thread <%C>\n", thr_str));

    if (component_.thr_.id () == 0)
      {
        ACE_ERROR ((LM_ERROR,
                "Sender_exec_i::ReadMessage_exec_i, Error thread  = 0\n"));
      }

    //compare current thread with original thread
    if (component_.thr_ != thr)
      {
        char thr_org [65];
        component_.thr_.to_string (thr_org);
        ACE_ERROR ((LM_ERROR,
               "Sender_exec_i::ReadMessage_exec_i, Error unexpected "
               "thread switch, Thread org. <%C>, Thread current <%C>\n",
                thr_org, thr_str));
      }
  }

  ReadMessage_exec_i::~ReadMessage_exec_i ()
  {
  }

  // Operations from ::Thread::ReadMessage
  char *
  ReadMessage_exec_i::get_message (void)
  {
    // get current thread
    ACE_Thread_ID thr;
    char thr_str [65];
    thr.to_string (thr_str);

    //compare current thread with original thread
    if (component_.thr_ != thr)
      {
        char thr_org [65];
        component_.thr_.to_string (thr_org);
        ACE_ERROR ((LM_ERROR,
               "Sender_exec_i::ReadMessage_exec_i, Error unexpected "
               "thread switch, Thread org. <%C>, Thread current <%C>\n",
                thr_org, thr_str));
      }

    ACE_DEBUG ((LM_DEBUG,
                "Sender returning message: [%C], Thread ID [%C}\n",
                component_.message_.in (), thr_str));
    return CORBA::string_dup (component_.message_.in ());
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i (const ACE_Thread_ID &thr)
    : thr_ (0,0),
      message_(CORBA::string_dup ("Hello World"))
  {
    // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);
    this->thr_ = thr;

    char thr_org_str [65];
    thr.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Sender: -4- Sender_exec_i::Sender_exec_i"
                              " Thread current <%C>, Thread original <%C>\n",
                              thr_cur_str, thr_org_str));
    //compare current and original thread mmmm
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Sender_exec_i::Sender_exec_i Unexpected "
                              "thread switch from  <%C> to  <%C>\n",
                              thr_org_str,thr_cur_str));
      }
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
    // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Sender: -10- Sender_exec_i::~Sender_exec_i"
                              " Thread current <%C>, Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Sender_exec_i::~Sender_exec_i Unexpected "
                               "thread switch from  <%C> to  <%C>\n",
                               thr_org_str,thr_cur_str));
      }
  }

  // Supported operations and attributes.
  void
  Sender_exec_i::start (void)
  {
#if !defined (CCM_NOEVENT)
    Thread::TimeOut_var event = new OBV_Thread::TimeOut;
    event->data ("start sending");
    ACE_DEBUG ((LM_DEBUG, "Sender initiates the process.\n"));
    this->context_->push_click_out (event.in ());
#endif
  }

  // Component attributes and port operations.
  ::Thread::CCM_ReadMessage_ptr
  Sender_exec_i::get_push_message (void)
  {
    ACE_Thread_ID thr;
    char thr_str [65];
    thr.to_string (thr_str);
    if (this->thr_ != thr)
      {
        char thr_org [65];
        this->thr_.to_string (thr_org);
        ACE_ERROR ((LM_ERROR,
                "Sender_exec_i::get_push_message, Error thread switch:"
                " Thread org. <%C>, Thread current <%C>\n",
                thr_org, thr_str));
      }

    ACE_DEBUG ((LM_DEBUG,
                "Sender_exec.i::get_push_message called, Thread ,%C>\n",
                thr_str));
    return ( new ReadMessage_exec_i (*this) );
  }

  // Operations from Components::SessionComponent.

  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
   // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Sender: -5- Sender_exec_i::"
                              "set_session_context Thread current <%C>,"
                              " Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Sender_exec_i::set_session_context "
                    "Unexpected thread switch from  <%C> to  <%C>\n",
                     thr_org_str,thr_cur_str));
      }
    this->context_ =
      ::Thread::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
   // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Sender: -6- Sender_exec_i::"
                              "configuration_complete Thread current <%C>,"
                              " Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Sender_exec_i::configuration_complete "
                    "Unexpected thread switch from  <%C> to  <%C>\n",
                     thr_org_str,thr_cur_str));
      }
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Sender: -7- Sender_exec_i::"
                              "ccm_activate Thread current <%C>,"
                              " Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Sender_exec_i::ccm_activate "
                    "Unexpected thread switch from  <%C> to  <%C>\n",
                     thr_org_str,thr_cur_str));
      }
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
     // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Sender: -8- Sender_exec_i::"
                              "ccm_passivate Thread current <%C>,"
                              " Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Sender_exec_i::ccm_passivate "
                    "Unexpected thread switch from  <%C> to  <%C>\n",
                     thr_org_str,thr_cur_str));
      }
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
     // get curent thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Sender: -9- Sender_exec_i::"
                              "ccm_remove Thread current <%C>,"
                              " Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(this->thr_ != thr_cur)
      {
        ACE_ERROR ((LM_ERROR, "Sender_exec_i::ccm_remove "
                    "Unexpected thread switch from  <%C> to  <%C>\n",
                     thr_org_str,thr_cur_str));
      }
  }
}

namespace CIAO_Thread_Sender_Impl
{
  //============================================================
  // Home Executor Implementation Class: SenderHome_exec_i
  //============================================================

  SenderHome_exec_i::SenderHome_exec_i (const ACE_Thread_ID &thr)
  {
    // get current thread
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    thr.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Sender: -2- SenderHome_exec_i::"
                              "SenderHome_exec_i Thread current <%C>,"
                              " Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(thr_cur != thr)
      {
        ACE_ERROR ((LM_ERROR, "Sender_exec_i::SenderHome_exec_i "
                              "Unexpected thread switch from  <%C> to  <%C>\n",
                               thr_org_str,thr_cur_str));
      }
    this->thr_ = thr;
  }

  SenderHome_exec_i::~SenderHome_exec_i (void)
  {
    ACE_Thread_ID thr_cur;
    char thr_cur_str [65];
    thr_cur.to_string (thr_cur_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);

    ACE_DEBUG ((LM_DEBUG, "Sender: -11- SenderHome_exec_i::"
                              "~SenderHome_exec_i Thread current <%C>,"
                              " Thread original <%C>\n",
                              thr_cur_str, thr_org_str));

    //compare current and original thread
    if(thr_cur != this->thr_)
      {
        ACE_ERROR ((LM_ERROR, "Sender_exec_i::~SenderHome_exec_i "
                              "Unexpected thread switch from  <%C> to  <%C>\n",
                              thr_org_str,thr_cur_str));
      }
  }

  // All operations and attributes.

  // Factory operations.

  // Finder operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  SenderHome_exec_i::create (void)
  {
   // get current thread
    ACE_Thread_ID thr;
    char thr_str [65];
    thr.to_string (thr_str);

    char thr_org_str [65];
    this->thr_.to_string (thr_org_str);


    ACE_DEBUG ((LM_DEBUG, "Sender: -3- SenderHome_exec_i::create "
                              "Thread current <%C>, Thread original <%C>\n",
                              thr_str, thr_org_str));
    if(this->thr_ != thr)
      {
        ACE_ERROR ((LM_ERROR, "Sender_exec_i::create : Unexpected thread "
                              "switch from  <%C> to  <%C>\n",
                              thr_org_str,thr_str));
      }
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) creating SenderHome\n"));
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      Sender_exec_i (this->thr_),
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Thread_SenderHome_Impl (void)
  {

    //get current thread
     ACE_Thread_ID thr;
     char thr_str [65];
     thr.to_string (thr_str);
     ACE_DEBUG ((LM_DEBUG, "Senderr: -1- create_Thread_SenderHome_Impl "
                               "Thread <%C>\n", thr_str));


    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_NORETURN (
      retval,
      SenderHome_exec_i (thr));

    return retval;
  }
}

