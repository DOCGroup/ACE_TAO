// $Id$

#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Network_Priority_Mapping.h"
#include "Custom_Network_Priority_Mapping.h"
#include "Custom_Network_Priority_Mapping.cpp"
#include "Sender_exec.h"
#include "ace/OS_NS_unistd.h"

SenderImpl::SenderExec_i::SenderExec_i (void)
{
  ACE_DEBUG ((LM_DEBUG, "SenderImpl::SenderExec_i::SenderExec_i\n"));

  int argc = 0;
  char **argv = 0;

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      this->orb_ =
        CORBA::ORB_init (argc,
                         argv,
                         ""
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ()
                                 ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      RTCORBA::TCPProtocolProperties_var normal_tcp_protocol_properties =
        rt_orb->create_tcp_protocol_properties (ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                                                ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                                                1,
                                                0,
                                                1,
                                                0);

      RTCORBA::TCPProtocolProperties_var diffserv_tcp_protocol_properties =
        rt_orb->create_tcp_protocol_properties (ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                                                ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
                                                1,
                                                0,
                                                1,
                                                1);

      RTCORBA::ProtocolList protocols;
      protocols.length (1);
      protocols[0].protocol_type = 0;
      protocols[0].transport_protocol_properties =
        RTCORBA::ProtocolProperties::_duplicate (normal_tcp_protocol_properties.in ());
      protocols[0].orb_protocol_properties =
        RTCORBA::ProtocolProperties::_nil ();

      this->normal_policy_list_.length (1);
      this->normal_policy_list_[0] =
        rt_orb->create_client_protocol_policy (protocols);

      protocols[0].transport_protocol_properties =
        RTCORBA::ProtocolProperties::_duplicate (diffserv_tcp_protocol_properties.in ());

      this->diffserv_policy_list_.length (1);
      this->diffserv_policy_list_[0] =
        rt_orb->create_client_protocol_policy (protocols);

      object =
        this->orb_->resolve_initial_references ("ORBPolicyManager"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->policy_manager_ =
        CORBA::PolicyManager::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("PolicyCurrent"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->policy_current_ =
        CORBA::PolicyCurrent::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("NetworkPriorityMappingManager"
                                                ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      RTCORBA::NetworkPriorityMappingManager_var mapping_manager =
        RTCORBA::NetworkPriorityMappingManager::_narrow (object.in ());

      this->custom_network_priority_mapping_ =
        new Custom_Network_Priority_Mapping;

      mapping_manager->mapping (this->custom_network_priority_mapping_);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P|%t) Caught exception in SenderExec_i \n");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
SenderImpl::SenderExec_i::change_network_priority (int enable_network_priority,
                                                   Priority_Level level,
                                                   DiffServ::test_var &test)
{
  CORBA::PolicyList *policy_list = 0;

  if (enable_network_priority)
    policy_list = &this->diffserv_policy_list_;
  else
    policy_list = &this->normal_policy_list_;

  switch (level)
    {
      // Change policy at ORB level
    case ORB:
      {
        this->policy_manager_->set_policy_overrides (*policy_list,
                                                     CORBA::SET_OVERRIDE);

        break;
      }

      // Change policy at THREAD level
    case THREAD:
      {
        this->policy_current_->set_policy_overrides (*policy_list,
                                                     CORBA::SET_OVERRIDE);

        break;
      }

      // Change policy at OBJECT level
    case OBJECT:
      {
        CORBA::Object_var object =
          test->_set_policy_overrides (*policy_list,
                                       CORBA::SET_OVERRIDE);

        test = DiffServ::test::_narrow (object.in ());

        break;
      }
    }
}

void
SenderImpl::SenderExec_i::start (CORBA::ULong iterations,
                                 CORBA::Short priority)
  throw (CORBA::SystemException)
{
  ACE_DEBUG ((LM_DEBUG, "SenderImpl::SenderExec_i::start\n"));

  this->custom_network_priority_mapping_->corba_priority (priority);

  DiffServ::test_var test =
    this->context_->get_connection_reader ();

  int enable_network_priority = 1;

  for (CORBA::ULong i = 0; i < iterations; ++i)
    {
      this->toggle (enable_network_priority);

      switch (i)
        {
        case 0:
        case 1:
          this->change_network_priority (enable_network_priority, ORB, test);
          break;
        case 2:
        case 3:
          this->change_network_priority (enable_network_priority, THREAD, test);
          break;
        default:
          this->change_network_priority (enable_network_priority, OBJECT, test);
          break;
        }

      test->method ();
      ACE_OS::sleep (1);
    }
}

void
SenderImpl::SenderExec_i::shutdown (void)
  throw (CORBA::SystemException)
{
  ACE_DEBUG ((LM_DEBUG, "SenderImpl::SenderExec_i::shutdown\n"));
  this->orb_->shutdown ();
}

void
SenderImpl::SenderExec_i::toggle (int &i)
{
  if (i == 0)
    i = 1;
  else i = 0;
}

void
SenderImpl::SenderExec_i::set_session_context (Components::SessionContext_ptr ctx)
  throw (CORBA::SystemException,
         Components::CCMException)
{
  ACE_DEBUG ((LM_DEBUG, "SenderImpl::SenderExec_i::set_session_context\n"));

  this->context_ =
    SenderImpl::SenderExec_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    throw CORBA::INTERNAL ();
}

void
SenderImpl::SenderExec_i::ccm_activate (void)
  throw (CORBA::SystemException,
         Components::CCMException)
{
  ACE_DEBUG ((LM_DEBUG, "SenderImpl::SenderExec_i::ccm_activate\n"));
}

void
SenderImpl::SenderExec_i::ccm_passivate (void)
  throw (CORBA::SystemException,
         Components::CCMException)
{
  ACE_DEBUG ((LM_DEBUG, "SenderImpl::SenderExec_i::ccm_passivate\n"));
}

void
SenderImpl::SenderExec_i::ccm_remove (void)
  throw (CORBA::SystemException,
         Components::CCMException)
{
  ACE_DEBUG ((LM_DEBUG, "SenderImpl::SenderExec_i::ccm_remove\n"));
}


void
SenderImpl::SenderExec_i::ciao_preactivate (void)
  throw (CORBA::SystemException,
         Components::CCMException)
{
  ACE_DEBUG ((LM_DEBUG, "SenderImpl::SenderExec_i::ccm_preactivate\n"));
}

void
SenderImpl::SenderExec_i::ciao_postactivate (void)
  throw (CORBA::SystemException,
         Components::CCMException)
{
  ACE_DEBUG ((LM_DEBUG, "SenderImpl::SenderExec_i::ccm_postactivate\n"));
}

::Components::EnterpriseComponent_ptr
SenderImpl::SenderHomeExec_i::create (void)
  throw (CORBA::SystemException,
         Components::CCMException)
{
  ACE_DEBUG ((LM_DEBUG, "SenderImpl::SenderHome_exec::create\n"));
  return new SenderImpl::SenderExec_i;
}

extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
createSenderHome_Impl (void)
{
  ACE_DEBUG ((LM_DEBUG, "createSenderHome_Impl\n"));
  return new SenderImpl::SenderHomeExec_i;
}
