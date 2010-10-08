// $Id$

#include "SimpleAttribute_exec.h"
#include "ace/Log_Msg.h"

namespace CIAO_Simple_SimpleAttribute_Impl
{
  //==================================================================
  // Component Executor Implementation Class:   SimpleAttribute_exec_i
  //==================================================================

  SimpleAttribute_exec_i::SimpleAttribute_exec_i (void)
    : str_ (""),
      long_(-1),
      short_(-1)
  {
  }

  SimpleAttribute_exec_i::~SimpleAttribute_exec_i (void)
  {
  }

  // Supported or inherited operations.

  // Attribute operations.

  char *
  SimpleAttribute_exec_i::str_attr ()
  {
    // Your code here.
    return CORBA::string_dup (str_.c_str ());
  }

  void
  SimpleAttribute_exec_i::str_attr (const char * str)
  {
    ACE_TRACE ("SimpleAttribute_exec_i::str_attr");
    // Your code here.
    this->str_ = str;
  }

  ::CORBA::Long
  SimpleAttribute_exec_i::long_attr ()
  {
    ACE_TRACE ("SimpleAttribute_exec_i::long_attr");
    // Your code here.
    return long_;
  }

  void
  SimpleAttribute_exec_i::long_attr (::CORBA::Long long_attr )
  {
    ACE_TRACE ("SimpleAttribute_exec_i::long_attr");
    // Your code here.
    this->long_ = long_attr;
  }

  ::CORBA::Short
  SimpleAttribute_exec_i::short_attr ()
  {
    // Your code here.
    return short_;
  }

  void
  SimpleAttribute_exec_i::short_attr (::CORBA::Short short_attr )
  {
    ACE_TRACE ("SimpleAttribute_exec_i::short_attr");
    // Your code here.
    this->short_ = short_attr;
  }

  // Port operations.

  // Operations from Components::SessionComponent

  void
  SimpleAttribute_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Simple::CCM_SimpleAttribute_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  SimpleAttribute_exec_i::configuration_complete ()
  {
    // Your code here.
  }

  void
  SimpleAttribute_exec_i::ccm_activate ()
  {
    // Your code here.
    if (this->str_ == "" ||
        this->long_ == -1 ||
        this->short_ == -1)
      {
        ACE_ERROR ((LM_ERROR, "SimpleAttribute_exec_i::ccm_activate - "
                     "Error: Attribute values didin't initialize correctly!\n"));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "SimpleAttribute_exec_i::ccm_activate - "
                     "Test passed!  String attribute is %s\n",
                     this->str_.c_str ()));
      }

  }

  void
  SimpleAttribute_exec_i::ccm_passivate ()
  {
    // Your code here.
  }

  void
  SimpleAttribute_exec_i::ccm_remove ()
  {
    // Your code here.
  }

  //==================================================================
  // Home Executor Implementation Class:   SimpleAttributeHome_exec_i
  //==================================================================

  SimpleAttributeHome_exec_i::SimpleAttributeHome_exec_i (void)
    : str_ (""),
      long_(-1),
      short_(-1)
  {
  }

  SimpleAttributeHome_exec_i::~SimpleAttributeHome_exec_i (void)
  {
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.


  char *
  SimpleAttributeHome_exec_i::str_attr ()
  {
    // Your code here.
    return CORBA::string_dup (str_.c_str ());
  }

  void
  SimpleAttributeHome_exec_i::str_attr (const char * str)
  {
    ACE_TRACE ("SimpleAttributeHome_exec_i::str_attr");
    // Your code here.
    this->str_ = str;
  }

  ::CORBA::Long
  SimpleAttributeHome_exec_i::long_attr ()
  {
    ACE_TRACE ("SimpleAttributeHome_exec_i::long_attr");
    // Your code here.
    return long_;
  }

  void
  SimpleAttributeHome_exec_i::long_attr (::CORBA::Long long_attr )
  {
    ACE_TRACE ("SimpleAttributeHome_exec_i::long_attr");
    // Your code here.
    this->long_ = long_attr;
  }

  ::CORBA::Short
  SimpleAttributeHome_exec_i::short_attr ()
  {
    // Your code here.
    return short_;
  }

  void
  SimpleAttributeHome_exec_i::short_attr (::CORBA::Short short_attr )
  {
    ACE_TRACE ("SimpleAttributeHome_exec_i::short_attr");
    // Your code here.
    this->short_ = short_attr;
  }

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  SimpleAttributeHome_exec_i::create ()
  {
    if (this->str_ == "" ||
        this->long_ == -1 ||
        this->short_ == -1)
      {
        ACE_ERROR ((LM_ERROR, "SimpleAttributeHom_exec_i::create - "
                     "Error: Attribute values didin't initialize correctly!\n"));
      }
    else
      {
        ACE_DEBUG ((LM_EMERGENCY, "SimpleAttributeHome_exec_i::create - "
                     "Test passed!  String attribute is %s\n",
                     this->str_.c_str ()));
      }

    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      SimpleAttribute_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SIMPLEATTRIBUTE_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleAttributeHome_Impl (void)
  {
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      SimpleAttributeHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

