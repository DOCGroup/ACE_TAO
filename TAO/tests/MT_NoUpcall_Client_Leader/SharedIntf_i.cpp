#include "SharedIntf_i.h"
#include "ace/Log_Priority.h"

Test_Idl_SharedIntf_i::Test_Idl_SharedIntf_i(CORBA::ORB_ptr orb) 
  : orb_ (orb) 
{
}

Test_Idl_SharedIntf_i::~Test_Idl_SharedIntf_i(void) {}

void Test_Idl_SharedIntf_i::ping (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
   ACE_THROW_SPEC (( CORBA::SystemException ))
{
   ACE_DEBUG((LM_DEBUG,"    (%P|%t) Test_Idl_SharedIntf::ping - sleeping for 5 seconds\n"));
   ACE_OS::sleep(5);
   ACE_DEBUG((LM_DEBUG,"    (%P|%t) Test_Idl_SharedIntf::ping - waking up\n"));
}

void Test_Idl_SharedIntf_i::farewell (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC (( CORBA::SystemException))
{
  ACE_DEBUG((LM_INFO,"(%P|%t) farewell begins\n"));
  this->orb_->shutdown ();
  ACE_DEBUG((LM_INFO,"(%P|%t) farewell completes\n"));
}
    
