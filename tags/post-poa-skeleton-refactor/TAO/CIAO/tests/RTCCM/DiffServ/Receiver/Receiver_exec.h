// $Id$

#include "ReceiverEC.h"
#include "Receiver_exec_export.h"
#include "tao/LocalObject.h"

namespace ReceiverImpl
{
  class RECEIVER_EXEC_Export ReceiverExec_i :
    public virtual ReceiverExec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:

    ReceiverExec_i (void);

    ::DiffServ::CCM_test_ptr get_writer (void)
      throw (CORBA::SystemException);

    void shutdown (void)
      throw (CORBA::SystemException);

    void set_session_context (Components::SessionContext_ptr ctx)
      throw (CORBA::SystemException,
             Components::CCMException);

    void ccm_activate (void)
      throw (CORBA::SystemException,
             Components::CCMException);

    void ccm_passivate (void)
      throw (CORBA::SystemException,
             Components::CCMException);

    void ccm_remove (void)
      throw (CORBA::SystemException,
             Components::CCMException);

    void ciao_preactivate (void)
      throw (CORBA::SystemException,
             Components::CCMException);

    void ciao_postactivate (void)
      throw (CORBA::SystemException,
             Components::CCMException);

  protected:

    ReceiverExec_Context_var context_;

    CORBA::ORB_var orb_;

  };

  class RECEIVER_EXEC_Export test_i :
    public virtual DiffServ::CCM_test,
    public virtual TAO_Local_RefCounted_Object
  {
  public:

    void method (void)
      throw (CORBA::SystemException);

  };

  class RECEIVER_EXEC_Export ReceiverHomeExec_i :
    public virtual ReceiverHomeExec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:

    ::Components::EnterpriseComponent_ptr create (void)
      throw (CORBA::SystemException,
             Components::CCMException);
  };

}

extern "C" RECEIVER_EXEC_Export ::Components::HomeExecutorBase_ptr
createReceiverHome_Impl (void);
