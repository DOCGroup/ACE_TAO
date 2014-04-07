// $Id$

#include "SharedIntf_i.h"
#include "ace/Log_Priority.h"
#include "ace/OS_NS_unistd.h"

#include "tao/Messaging/Messaging.h"
#include "tao/PolicyC.h"
#include "tao/AnyTypeCode/Any.h"

Test_Idl_SharedIntf_i::Test_Idl_SharedIntf_i(CORBA::ORB_ptr orb)
  : upper_ior (),
    orb_ (orb)
{
}

Test_Idl_SharedIntf_i::~Test_Idl_SharedIntf_i(void) {}

void
Test_Idl_SharedIntf_i::set_upper (const char *ior)
{
  this->upper_ior = CORBA::string_dup (ior);
}

void
Test_Idl_SharedIntf_i::do_upcall (void)
{
   ACE_DEBUG((LM_DEBUG,"(%P|%t) Test_Idl_SharedIntf::do_upcall called\n"));

   CORBA::Object_var obj =
     orb_->resolve_initial_references ("ORBPolicyManager");

   CORBA::PolicyManager_var policy_manager_ =
     CORBA::PolicyManager::_narrow (obj.in ());

   TimeBase::TimeT timeout = 10000 * 100;

   CORBA::Any any_orb;
   any_orb <<= timeout;

   CORBA::PolicyList policy_list (1);
   policy_list.length (1);
   policy_list[0] =
     orb_->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                          any_orb);

   policy_manager_->set_policy_overrides (policy_list,
                                          CORBA::SET_OVERRIDE);


   obj = this->orb_->string_to_object (this->upper_ior.in());
   Test_Idl::SharedIntf_var upper = Test_Idl::SharedIntf::_narrow (obj.in());
   upper->ping ();
   ACE_DEBUG((LM_DEBUG,"(%P|%t) Test_Idl_SharedIntf::do_upcall returning\n"));
}

void Test_Idl_SharedIntf_i::ping ()
{
   ACE_DEBUG((LM_DEBUG,"(%P|%t) Test_Idl_SharedIntf::ping - called\n"));
}

void Test_Idl_SharedIntf_i::farewell ()
{
  ACE_DEBUG((LM_INFO,"(%P|%t) farewell begins\n"));
  // if (!CORBA::is_nil (this->upper_.in ()))
  //   {
  //     this->upper_->farewell ();
  //   }
  this->orb_->shutdown ();
  ACE_DEBUG((LM_INFO,"(%P|%t) farewell completes\n"));
}

