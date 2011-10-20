// $Id$

#include "SenderEC.h"
#include "Sender_exec_export.h"
#include "tao/LocalObject.h"

class Custom_Network_Priority_Mapping;

namespace CIDL_SenderImpl
{
  class SENDER_EXEC_Export SenderExec_i :
    public virtual ::CIAO_Protocols_Sender_Impl::Sender_Exec,
    public virtual ::CORBA::LocalObject
  {
  public:

    SenderExec_i (void);

    void start (::CORBA::ULong iterations,
                ::CORBA::ULong invocation_rate,
                ::CORBA::Boolean count_missed_end_deadlines,
                ::CORBA::Boolean do_dump_history,
                ::CORBA::Boolean print_missed_invocations,
                ::CORBA::ULong message_size,
                ::CORBA::ULong test_protocol_tag,
                ::CORBA::Boolean print_statistics,
                ::CORBA::ULong number_of_connection_attempts,
                ::CORBA::Boolean enable_diffserv_code_points,
                ::CORBA::Short priority,
                ::Protocols::Sender_Controller::Test_Type test_type);

    void shutdown (void);

    void set_session_context (Components::SessionContext_ptr ctx);

    void ccm_activate (void);

    void ccm_passivate (void);

    void ccm_remove (void);

    void configuration_complete (void);

  protected:
    ::Protocols::CCM_Sender_Context_var context_;

    CORBA::ORB_var orb_;

  };

  class SENDER_EXEC_Export SenderHomeExec_i :
    public virtual ::CIAO_Protocols_Sender_Impl::SenderHome_Exec,
    public virtual ::CORBA::LocalObject
  {
  public:

    ::Components::EnterpriseComponent_ptr create (void);
  };

  extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  createSenderHome_Impl (void);
}
