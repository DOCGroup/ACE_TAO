// $Id$

#include "SenderEC.h"
#include "Sender_exec_export.h"
#include "tao/LocalObject.h"

class Custom_Network_Priority_Mapping;

namespace CIDL_SenderImpl
{
  class SENDER_EXEC_Export SenderExec_i :
    public virtual SenderExec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:

    SenderExec_i (void);

    void start (CORBA::ULong iterations,
                CORBA::Short priority)
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

    SenderExec_Context_var context_;

    CORBA::ORB_var orb_;

    Custom_Network_Priority_Mapping *custom_network_priority_mapping_;

    CORBA::PolicyManager_var policy_manager_;

    CORBA::PolicyCurrent_var policy_current_;

    CORBA::PolicyList normal_policy_list_;

    CORBA::PolicyList diffserv_policy_list_;

    enum Priority_Level
      {
        OBJECT,
        THREAD,
        ORB
      };

    void change_network_priority (int enable_network_priority,
                                  Priority_Level level,
                                  DiffServ::test_var &test);

    void toggle (int &i);

  };

  class SENDER_EXEC_Export SenderHomeExec_i :
    public virtual SenderHomeExec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:

    ::Components::EnterpriseComponent_ptr create (void)
      throw (CORBA::SystemException,
             Components::CCMException);
  };

  extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createSenderHome_Impl (void);
}


