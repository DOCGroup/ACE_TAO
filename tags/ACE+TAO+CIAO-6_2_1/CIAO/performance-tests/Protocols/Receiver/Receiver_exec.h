// $Id$

#include "ReceiverEC.h"
#include "Receiver_exec_export.h"
#include "tao/LocalObject.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sample_History.h"
#include "ace/Containers_T.h"

namespace CIDL_ReceiverImpl
{
  class RECEIVER_EXEC_Export ReceiverExec_i :
    public virtual ::CIAO_Protocols_Receiver_Impl::Receiver_Exec,
    public virtual ::CORBA::LocalObject
  {
  public:

    ReceiverExec_i (void);

    ::Protocols::CCM_test_ptr get_writer (void);

    void shutdown (void);

    void set_session_context (Components::SessionContext_ptr ctx);

    void ccm_activate (void);

    void ccm_passivate (void);

    void ccm_remove (void);

    void configuration_complete (void);

  protected:

    ::Protocols::CCM_Receiver_Context_var context_;

    CORBA::ORB_var orb_;

  };

  class RECEIVER_EXEC_Export test_i :
    public virtual Protocols::CCM_test,
    public virtual ::CORBA::LocalObject
  {
  public:

    test_i (void);

    void start_test (CORBA::Long session_id,
                     const char *protocol,
                     CORBA::ULong invocation_rate,
                     CORBA::ULong message_size,
                     CORBA::ULong iterations);

    void end_test (void);

    void oneway_sync (void);

    void twoway_sync (void);

    void oneway_method (CORBA::Long session_id,
                        CORBA::ULong iteration,
                        const Protocols::test::octets &payload);

    void twoway_method (CORBA::Long &session_id,
                        CORBA::ULong &iteration,
                        Protocols::test::octets &payload);

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
    public virtual ::CIAO_Protocols_Receiver_Impl::ReceiverHome_Exec,
    public virtual ::CORBA::LocalObject
  {
  public:

    ::Components::EnterpriseComponent_ptr create (void);
  };

  extern "C" RECEIVER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createReceiverHome_Impl (void);
}
