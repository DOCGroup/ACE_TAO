// -*- C++ -*-
// $Id$

#include "AMI_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "AMI_MyFoo_i.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_Hello_AMI_AMI_AMI_Impl
{
  //============================================================
  // Worker thread to call "perform_work"
  //============================================================
  AMI_perform_work::AMI_perform_work (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  int AMI_perform_work::svc ()
  {
    try
      {
        printf ("AMI :\tPerform work started\n");
        while (1)
          {
            if (orb_->work_pending())
                orb_->perform_work();
            ACE_Time_Value tv (0, 1000);
            ACE_OS::sleep (tv);
          }
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Caught except:");
        return 1;
      }
    printf ("AMI :\t Exiting perform work");
    return 0;
  }


  //============================================================
  // Facet Executor Implementation Class: AMI_MyFoo_exec_i
  //============================================================
  AMI_MyFoo_exec_i::AMI_MyFoo_exec_i ()
  {
    // @@TODO:  This is all sorts of wrong.  We should be using the container ORB.

    //initialize AMI client
    int argc = 2;
    ACE_TCHAR **argv = new ACE_TCHAR *[argc];
    argv[0] = ACE::strnew (ACE_TEXT ("-ORBAMICollocation"));
    argv[1] = ACE::strnew (ACE_TEXT ("0"));
    CORBA::ORB_var orb =
      CORBA::ORB_init (argc, argv, ACE_TEXT ("AMI_foo_client"));

    // Activate POA to handle the call back.
    CORBA::Object_var poa_object =
      orb->resolve_initial_references("RootPOA");

    if (CORBA::is_nil (poa_object.in ()))
      printf ("POA is NIL!\n");

    PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (poa_object.in ());

    PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager ();

    poa_manager->activate ();
    AMI_perform_work *pw = new AMI_perform_work (orb.in ());
    pw->activate ();
  }

  AMI_MyFoo_exec_i::~AMI_MyFoo_exec_i (void)
  {
  }
  
  void 
  AMI_MyFoo_exec_i::provide_receiver (::Hello::MyFoo_ptr receiver_foo)
  {
    ami_foo_server_ = ::Hello::MyFoo::_duplicate (receiver_foo);    
    
    if (CORBA::is_nil (ami_foo_server_.in ()))
      {
        printf ("Server is NIL\n");
        throw CORBA::BAD_PARAM ();
      }
  }
  
  // Operations from ::CCM_AMI::AMI_ami_foo
  
  void
  AMI_MyFoo_exec_i::sendc_foo (
    ::Hello_AMI::AMI_MyFooCallback_ptr ami_handler,
    const char * in_str)
  {
    if (CORBA::is_nil (ami_foo_server_.in ()))
      {
        printf ("AMI(FOO) error: ami_foo_server_ reference is nil\n");
        return;
      }

    printf ("AMI (FOO) :\tsendc_foo <%s>\n", in_str);
    ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler*  handler =
        new ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler (ami_handler);
    //Create a reply handler the CORBA-way
    Hello::AMI_MyFooHandler_var the_handler_var = handler->_this ();
    printf ("AMI (FOO) :\tSending string <%s> to AMI CORBA server\n", in_str);
    ami_foo_server_->sendc_foo (the_handler_var.in (), in_str);
    printf ("AMI (FOO) : \tInvoked sendc_foo\n");
  }

  void
  AMI_MyFoo_exec_i::sendc_hello (
    ::Hello_AMI::AMI_MyFooCallback_ptr ami_handler)
  {
    printf ("AMI (FOO) :\tsendc_hello\n");

    ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler*  handler =
        new ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler (ami_handler);
    //Create a reply handler the CORBA-way
    Hello::AMI_MyFooHandler_var the_handler_var = handler->_this ();
    printf ("AMI (FOO) :\tCalling AMI CORBA server\n");
    ami_foo_server_->sendc_hello (the_handler_var.in ());
    printf ("AMI (FOO) : \tInvoked sendc_foo\n");
  }

  void
  AMI_MyFoo_exec_i::sendc_get_rw_attrib (
    ::Hello_AMI::AMI_MyFooCallback_ptr ami_handler)
  {
    printf ("AMI (FOO) :\tsendc_get_rw_attrib\n");
    ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler*  handler =
        new ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler (ami_handler);
    //Create a reply handler the CORBA-way
    Hello::AMI_MyFooHandler_var the_handler_var = handler->_this ();
    ami_foo_server_->sendc_get_rw_attrib (the_handler_var.in ());
    printf ("AMI (FOO) : \tInvoked sendc_get_rw_attrib\n");
  }

  void
  AMI_MyFoo_exec_i::sendc_set_rw_attrib (
    ::Hello_AMI::AMI_MyFooCallback_ptr ami_handler,
  CORBA::Short rw_attrib)
  {
    printf ("AMI (FOO) :\tsendc_set_rw_attrib\n");
    ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler*  handler =
        new ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler (ami_handler);
    //Create a reply handler the CORBA-way
    Hello::AMI_MyFooHandler_var the_handler_var = handler->_this ();
    printf ("AMI (FOO) : \tSet rw_attrib <%d>\n", rw_attrib);
    ami_foo_server_->sendc_set_rw_attrib (the_handler_var.in (), rw_attrib);
    printf ("AMI (FOO) : \tInvoked sendc_set_rw_attrib\n");
  }

  void
  AMI_MyFoo_exec_i::sendc_get_ro_attrib (
    ::Hello_AMI::AMI_MyFooCallback_ptr ami_handler)
  {
    printf ("AMI (FOO) :\tsendc_get_ro_attrib\n");
    ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler*  handler =
        new ::CCM_CORBA_AMI_MyFoo_Impl::AMI_MyFoo_reply_handler (ami_handler);
    //Create a reply handler the CORBA-way
    Hello::AMI_MyFooHandler_var the_handler_var = handler->_this ();
    ami_foo_server_->sendc_get_ro_attrib (the_handler_var.in ());
    printf ("AMI (FOO) : \tInvoked sendc_get_ro_attrib\n");
  }
  //============================================================
  // Component Executor Implementation Class: AMI_exec_i
  //============================================================

  AMI_exec_i::AMI_exec_i (void)
    : myfoo_ (0)
  {
  }

  AMI_exec_i::~AMI_exec_i (void)
  {
    delete myfoo_;
  }

  // Supported operations and attributes.

  // Component attributes.

  // Port operations.

  ::Hello_AMI::CCM_AMI_MyFoo_ptr
  AMI_exec_i::get_perform_asynch_my_foo (void)
  {
    this->myfoo_ = new AMI_MyFoo_exec_i ();
    return this->myfoo_;
  }
  // Operations from Components::SessionComponent.

  void
  AMI_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Hello_AMI_AMI::CCM_AMI_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  AMI_exec_i::configuration_complete (void)
  {
    receiver_foo_ =
      this->context_->get_connection_my_foo_receiver ();

    this->myfoo_->provide_receiver (receiver_foo_.in ());
  }

  void
  AMI_exec_i::ccm_activate (void)
  {
    ::CCM_CORBA_AMI_MyFoo_Impl::CORBA_MyFoo_server* foo_srv =
        new ::CCM_CORBA_AMI_MyFoo_Impl::CORBA_MyFoo_server (receiver_foo_.in ());
    printf ("AMI :\tStarting MyFoo CORBA server thread.\n");
    foo_srv->activate ();
  }

  void
  AMI_exec_i::ccm_passivate (void)
  {
    /* Your code here. */
  }

  void
  AMI_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_Hello_AMI_AMI_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_RETURN (
      retval,
      AMI_exec_i,
      ::Components::EnterpriseComponent::_nil ());

    return retval;
  }
}

namespace CIAO_Hello_AMI_AMI_AMI_Impl
{
  //============================================================
  // Home Executor Implementation Class: AMIHome_exec_i
  //============================================================

  AMIHome_exec_i::AMIHome_exec_i (void)
  {
  }

  AMIHome_exec_i::~AMIHome_exec_i (void)
  {
  }

  // All operations and attributes.

  // Factory operations.

  // Finder operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  AMIHome_exec_i::create (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      AMI_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C"  ::Components::HomeExecutorBase_ptr
  create_Hello_AMI_AMIHome_Impl (void)
  {
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      AMIHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

