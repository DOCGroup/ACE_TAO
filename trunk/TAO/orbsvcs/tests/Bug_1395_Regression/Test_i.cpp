// $Id$

#include "Test_i.h"

Test_i::Test_i(CORBA::ORB_ptr orb) :
   orb_(CORBA::ORB::_duplicate(orb)),
   policies_(1)
{
   CORBA::Object_var obj = orb_->resolve_initial_references("RootPOA");
   root_poa_ = PortableServer::POA::_narrow(obj.in());
   policies_.length(1);
   policies_[0] = root_poa_->create_lifespan_policy(PortableServer::PERSISTENT);
   poa_mgr_ = root_poa_->the_POAManager();    
}

Test_i::~Test_i()
{
   policies_[0]->destroy();
}

int
Test_i::try_and_create_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
   ACE_TRY
   {
      PortableServer::POA_var persistent_poa = 
         root_poa_->create_POA( "MyPoa", poa_mgr_.in(), policies_);
      ACE_TRY_CHECK;
      return 0;
   }
   ACE_CATCHANY 
   {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "try_and_create_POA Exception ...");
      return 1; 
   }
   ACE_ENDTRY;
}

void 
Test_i::shutdown(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
   orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
