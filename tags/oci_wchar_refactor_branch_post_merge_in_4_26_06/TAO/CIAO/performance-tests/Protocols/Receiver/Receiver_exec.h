// $Id$

#include "ReceiverEC.h"
#include "Receiver_exec_export.h"
#include "tao/LocalObject.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sample_History.h"
#include "ace/Array.h"

namespace CIDL_ReceiverImpl
{
  class RECEIVER_EXEC_Export ReceiverExec_i :
    public virtual ReceiverExec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:

    ReceiverExec_i (void);

    ::Protocols::CCM_test_ptr get_writer (void)
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
    public virtual Protocols::CCM_test,
    public virtual TAO_Local_RefCounted_Object
  {
  public:

    test_i (void);

    void start_test (CORBA::Long session_id,
                     const char *protocol,
                     CORBA::ULong invocation_rate,
                     CORBA::ULong message_size,
                     CORBA::ULong iterations)
      throw (CORBA::SystemException);

    void end_test (void)
      throw (CORBA::SystemException);

    void oneway_sync (void)
      throw (CORBA::SystemException);

    void twoway_sync (void)
      throw (CORBA::SystemException);

    void oneway_method (CORBA::Long session_id,
                        CORBA::ULong iteration,
                        const Protocols::test::octets &payload)
      throw (CORBA::SystemException);

    void twoway_method (CORBA::Long &session_id,
                        CORBA::ULong &iteration,
                        Protocols::test::octets &payload)
      throw (CORBA::SystemException);

  private:
    typedef ACE_Array_Base<CORBA::Boolean> Invocations;
    Invocations invocations_received_;
    ACE_hrtime_t time_of_last_call_;
    ACE_hrtime_t test_start_;
    ACE_hrtime_t test_end_;
    CORBA::Boolean first_invocation_;
    ACE_Sample_History *inter_arrival_times_;
    CORBA::ULong iterations_;
    CORBA::ULong number_of_invocations_received_;
    CORBA::Long session_id_;
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

  extern "C" RECEIVER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createReceiverHome_Impl (void);
}



