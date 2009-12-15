// $Id$

#ifndef AMI_MyFoo_i_H
#define AMI_MyFoo_i_H

#include "AMIS.h"
#include "ace/Task.h"

namespace CCM_CORBA_AMI_MyFoo_Impl
{
  class AMI_MyFoo_reply_handler : public POA_Hello::AMI_MyFooHandler
  {
    public:
      AMI_MyFoo_reply_handler (
      ::Hello_AMI::AMI_MyFooCallback_ptr foo_callback);

      virtual ~AMI_MyFoo_reply_handler (void);

      virtual void foo (CORBA::Long result, const char * out_str);

      virtual void foo_excep (::Messaging::ExceptionHolder * excep_holder);

      virtual void hello (CORBA::Long answer);

      virtual void hello_excep (::Messaging::ExceptionHolder * excep_holder);

      virtual void get_rw_attrib (::CORBA::Short ami_return_val);

      virtual void get_rw_attrib_excep (
        ::Messaging::ExceptionHolder * excep_holder);

      void
      set_rw_attrib ();

      void
      set_rw_attrib_excep (
        ::Messaging::ExceptionHolder * excep_holder);

      void
      get_ro_attrib (
        ::CORBA::Short ami_return_val);

      void
      get_ro_attrib_excep (
        ::Messaging::ExceptionHolder * excep_holder);
    private:
      ::Hello_AMI::AMI_MyFooCallback_var foo_callback_;
  };
}
#endif /* AMI_MyFoo_i_H */

