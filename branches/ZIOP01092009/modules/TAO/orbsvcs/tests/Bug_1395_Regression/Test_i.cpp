// $Id$

#include "Test_i.h"

// In case this is a static build we have to force
// the ImR_Client to be linked.
#include "tao/ImR_Client/ImR_Client.h"

Test_i::Test_i (CORBA::ORB_ptr orb) :
  orb_ (CORBA::ORB::_duplicate(orb)),
   policies_ (1)
{
  try
    {
      CORBA::Object_var obj =
        orb_->resolve_initial_references ("RootPOA");
      this->root_poa_ =
        PortableServer::POA::_narrow (obj.in ());
      policies_.length(1);
      policies_[0] =
        root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);
      this->poa_mgr_ =
        this->root_poa_->the_POAManager ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception");

      ACE_ASSERT (0);  // Force termination!
    }
}

Test_i::~Test_i (void)
{
  this->policies_[0]->destroy ();
}

int
Test_i::try_and_create_POA (void)
{
  try
    {
      PortableServer::POA_var persistent_poa =
        root_poa_->create_POA ("MyPoa",
                               poa_mgr_.in (),
                               this->policies_);
    }
  catch (const CORBA::TRANSIENT&)
    {
      // A transient exception exception may be expected,
      // so we won't print out the word exception but we
      // will let the user know that a transient exception occurred
      ACE_DEBUG ((LM_DEBUG,
                  "CORBA::TRANSIENT in Test_i::try_and_create_POA\n"));
      return 1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("try_and_create_POA Exception ...");
      return 1;
    }

  return 0;
}

void
Test_i::shutdown (void)
{
   orb_->shutdown (0);
}
