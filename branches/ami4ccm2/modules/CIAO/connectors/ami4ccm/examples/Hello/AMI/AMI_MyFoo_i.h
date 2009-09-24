// $Id$

#ifndef AMI_MyFoo_i_H
#define AMI_MyFoo_i_H

#include "AMIS.h"
#include "ace/Task.h"

namespace CCM_CORBA_AMI_MyFoo_Impl
{
  //============================================================
  // Implementation of the AMI CORBA FOO reply handler
  //============================================================
  class AMI_MyFoo_reply_handler : public POA_CCM_AMI::AMI_MyFooHandler
  {
    public:
      AMI_MyFoo_reply_handler (
        ::CCM_AMI::AMI_MyFoo_callback_ptr foo_callback);
      ~AMI_MyFoo_reply_handler (void);

      void
      foo (
        CORBA::Long result,
        const char * out_str);

      void
      foo_excep (
        ::Messaging::ExceptionHolder * excep_holder);

      void
      hello (
        CORBA::Long answer);

      void
      hello_excep (
        ::Messaging::ExceptionHolder * excep_holder);
      
      void 
      get_rw_attrib (
        ::CORBA::Short ami_return_val);
      
      void 
      get_rw_attrib_excep (
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
      ::CCM_AMI::AMI_MyFoo_callback_var foo_callback_;
  };

  class AMI_MyFoo_i : public POA_CCM_AMI::MyFoo
  {
  public:
    /// ctor
    AMI_MyFoo_i (
      CORBA::ORB_ptr orb,
      ::CCM_AMI::MyFoo_ptr foo_receiver);

    // The AMI methods.
    CORBA::Long
    foo (
      const char * in_str,
      CORBA::String_out out_str);

    void
    hello (
      CORBA::Long_out answer);
  
    CORBA::Short
    rw_attrib (void);

    void
    rw_attrib (
      CORBA::Short rw_attrib);
  
    CORBA::Short
    ro_attrib (void);

  private:
    CORBA::ORB_var orb_;
    ::CCM_AMI::MyFoo_var foo_receiver_;
  };

  // CORBA server which delivers the MyFoo interface
  class CORBA_MyFoo_server : public ACE_Task_Base
  {
    public:
      CORBA_MyFoo_server (
    ::CCM_AMI::MyFoo_ptr foo_receiver);
      virtual int svc (void);

    private:
    ::CCM_AMI::MyFoo_var foo_receiver_;
  };
}
#endif /* AMI_MyFoo_i_H */

