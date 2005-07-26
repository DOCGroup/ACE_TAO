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
                ::Protocols::Sender_Controller::Test_Type test_type)
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

