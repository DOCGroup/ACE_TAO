// $Id$

#ifndef AMI_MyInterface_i_H
#define AMI_MyInterface_i_H

#include "AMIS.h"
#include "ace/Task.h"

namespace CCM_CORBA_AMI_MyInterface_Impl
{
  //============================================================
  // Implementation of the AMI CORBA INTERFACE reply handler
  //============================================================
  class AMI_MyInterface_reply_handler : public POA_CCM_AMI::AMI_MyInterfaceHandler
  {
    public:
      AMI_MyInterface_reply_handler (
        ::CCM_AMI::AMI_MyInterfaceCallback_ptr interface_callback);
      ~AMI_MyInterface_reply_handler (void);

      void
      do_something_with_something (
        CORBA::Float result);

      void
      do_something_with_something_excep (
        ::Messaging::ExceptionHolder * excep_holder);

    private:
      ::CCM_AMI::AMI_MyInterfaceCallback_var interface_callback_;
  };

  class AMI_MyInterface_i : public POA_CCM_AMI::MyInterface
  {
  public:
    /// ctor
    AMI_MyInterface_i (
      CORBA::ORB_ptr orb,
      ::CCM_AMI::MyInterface_ptr interface_receiver);

  CORBA::Float
  do_something_with_something (
    CORBA::Short something);

  private:
    CORBA::ORB_var orb_;
    ::CCM_AMI::MyInterface_var interface_receiver_;
  };

  // CORBA server which delivers the MyInterface interface
  class CORBA_MyInterface_server : public ACE_Task_Base
  {
    public:
      CORBA_MyInterface_server (
    ::CCM_AMI::MyInterface_ptr interface_receiver);
      virtual int svc (void);

    private:
    ::CCM_AMI::MyInterface_var interface_receiver_;
  };
}
#endif /* AMI_MyInterface_i_H */

