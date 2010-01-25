// $Id$

#include "AMI_MyFoo_i.h"
#include "connectors/ami4ccm/ami4ccm/ami4ccm.h"

namespace CCM_CORBA_AMI_MyFoo_Impl
{
  //============================================================
  // Implementation of the AMI CORBA FOO reply handler
  //============================================================
  AMI_MyFoo_reply_handler::AMI_MyFoo_reply_handler (::Hello::AMI_MyFooCallback_ptr foo_callback)
  : foo_callback_ (::Hello::AMI_MyFooCallback::_duplicate (foo_callback))
  {
  }

  AMI_MyFoo_reply_handler::~AMI_MyFoo_reply_handler ()
  {
  }

  // FOO methods
  void
  AMI_MyFoo_reply_handler::foo (
    CORBA::Long result,
    const char * out_str)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Foo Reply Handler::foo\n");
    this->foo_callback_->foo (result, out_str);
    this->_remove_ref ();
  }

  void
  AMI_MyFoo_reply_handler::foo_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Foo Reply Handler::foo_excep\n");
    ::CCM_AMI::ExceptionHolder_i holder (excep_holder);
    this->foo_callback_->foo_excep (&holder);
    this->_remove_ref ();
  }

  // HELLO methods
  void
  AMI_MyFoo_reply_handler::hello (
    CORBA::Long answer)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::hello\n");
    this->foo_callback_->hello (answer);
    this->_remove_ref ();
  }

  void
  AMI_MyFoo_reply_handler::hello_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::hello_excep\n");
    ::CCM_AMI::ExceptionHolder_i holder (excep_holder);
    this->foo_callback_->hello_excep (&holder);
    this->_remove_ref ();
  }

  //GET rw_attrib Reply Handler
  void
  AMI_MyFoo_reply_handler::get_rw_attrib (
    ::CORBA::Short ami_return_val)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::get_rw_atrrib\n");
    this->foo_callback_->get_rw_attrib (ami_return_val);
    this->_remove_ref ();
  }

  void
  AMI_MyFoo_reply_handler::get_rw_attrib_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::get_rw_attrib_excep\n");
    ::CCM_AMI::ExceptionHolder_i holder (excep_holder);
    this->foo_callback_->get_rw_attrib_excep (&holder);
    this->_remove_ref ();
  }

  //SET rw_attrib Reply Handler
  void
  AMI_MyFoo_reply_handler::set_rw_attrib ()
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::set_rw_attrib\n");
    this->foo_callback_->set_rw_attrib ();
    this->_remove_ref ();
  }

  void
  AMI_MyFoo_reply_handler::set_rw_attrib_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::set_rw_attrib_excep\n");
    ::CCM_AMI::ExceptionHolder_i holder (excep_holder);
    this->foo_callback_->set_rw_attrib_excep (&holder);
    this->_remove_ref ();
  }

  //ro_attrib Reply Handler
  void
  AMI_MyFoo_reply_handler::get_ro_attrib (
    ::CORBA::Short ami_return_val)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::get_ro_attrib\n");
    this->foo_callback_->get_ro_attrib (ami_return_val);
    this->_remove_ref ();
  }

  void
  AMI_MyFoo_reply_handler::get_ro_attrib_excep (
    ::Messaging::ExceptionHolder * excep_holder)
  {
    printf ("AMI CORBA (FOO) :\tMyFoo Hello Reply Handler::get_ro_attrib_excep\n");
    ::CCM_AMI::ExceptionHolder_i holder (excep_holder);
    this->foo_callback_->get_ro_attrib_excep (&holder);
    this->_remove_ref ();
  }
}
