// $Id$

#include "Test_i.h"

Test_i::Test_i (CORBA::ORB_ptr orb) :
  orb_ (CORBA::ORB::_duplicate(orb)),
   policies_ (1)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Object_var obj =
        orb_->resolve_initial_references ("RootPOA"
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->root_poa_ =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      policies_.length(1);
      policies_[0] =
        root_poa_->create_lifespan_policy (PortableServer::PERSISTENT
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->poa_mgr_ =
        this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception");

      ACE_ASSERT (0);  // Force termination!
    }
  ACE_ENDTRY;
}

Test_i::~Test_i (void)
{
  this->policies_[0]->destroy ();
}

int
Test_i::try_and_create_POA (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      PortableServer::POA_var persistent_poa =
        root_poa_->create_POA ("MyPoa",
                               poa_mgr_.in (),
                               this->policies_
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "try_and_create_POA Exception ...");
      return 1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (1);

  return 0;
}

void
Test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
   orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
