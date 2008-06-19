// $Id$

//============================================================================
//
//  =FILENAME
//     Collocation_Test.h
//
//  =DESCRIPTION
//     Server class to perform testing of TAO's collocation mechanism.
//
//  =AUTHOR
//     Nanbor Wang
//
//=============================================================================

#include "Collocation_Tester.h"

Collocation_Test::Collocation_Test (void)
{
  // no-op.
}

void
Collocation_Test::shutdown (void)
{
  this->root_poa_->destroy (1, 1);
  this->orb_->destroy ();
}

int
Collocation_Test::init (int argc, char *argv[])
{
  // Initialize the ORB.
  this->orb_ = CORBA::ORB_init (argc, argv);

  int result = this->parse_args (argc, argv);
  if (result != 0)
    return result;

  // Get an Object reference to RootPOA.
  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("RootPOA");

  // Narrow the Object reference to a POA reference
  this->root_poa_ =
    PortableServer::POA::_narrow (obj.in ());

  // Get the POAManager of RootPOA
  this->poa_manager_ =
    this->root_poa_->the_POAManager ();

  // Create our own child POA
  // Empty sequence means all default policies
  int nPolicies = 0;
  CORBA::PolicyList policies;

  policies.length(1);
  policies[nPolicies++] =
    this->root_poa_->create_implicit_activation_policy(PortableServer::IMPLICIT_ACTIVATION);

  obj = this->root_poa_->create_POA("child", this->poa_manager_.in(), policies);
  PortableServer::POA_var childPOA = PortableServer::POA::_narrow (obj.in ());

  // Destroy the Policy objects (before we may exit as a result of failures)
  for (CORBA::ULong i = 0; i < policies.length(); ++i)
    {
      policies[i]->destroy();
    }

  // Activate the diamond servant and its base classes under childPOA.
  PortableServer::ObjectId_var id =
    childPOA->activate_object (&this->top_servant_);

//    // We only care about the most derived class here.
//    this->diamond_obj_ = this->diamond_servant_._this ();

  id =
    childPOA->activate_object (&this->diamond_servant_);

  // We only care about the most derived class here.
  this->diamond_obj_ = childPOA->id_to_reference (id.in ());

  id =
    childPOA->activate_object (&this->left_servant_);

  id =
    childPOA->activate_object (&this->right_servant_);


  CORBA::String_var str =
    this->orb_->object_to_string (this->diamond_obj_.in ());

  ACE_DEBUG ((LM_DEBUG, "Diamond Servant activated:\n %s\n",
              str.in()));

  return 0;


}

int
Collocation_Test::parse_args (int /*argc*/,
                              char *[] /*argv*/)
{
  return 0;
}

int
Collocation_Test::test_narrow (void)
{
  Diamond::Top_var top =
    Diamond::Top::_narrow (this->diamond_obj_.in ());

  Diamond::Left_var left =
    Diamond::Left::_narrow (this->diamond_obj_.in ());

  Diamond::Right_var right =
    Diamond::Right::_narrow (this->diamond_obj_.in ());

  Diamond::Buttom_var buttom =
    Diamond::Buttom::_narrow (this->diamond_obj_.in ());

  CORBA::String_var str = top->shape ();
  ACE_DEBUG ((LM_DEBUG, "Calling top->shape: %s\n", str.in ()));

  str = left->shape ();
  ACE_DEBUG ((LM_DEBUG, "Calling left->shape: %s\n", str.in ()));

  str = right->shape ();
  ACE_DEBUG ((LM_DEBUG, "Calling right->shape: %s\n", str.in ()));

  str = buttom->shape ();
  ACE_DEBUG ((LM_DEBUG, "Calling buttom->shape: %s\n", str.in ()));

  return 0;
}

int
Collocation_Test::run (void)
{
  this->poa_manager_->activate ();

  this->test_narrow ();

  return 0;
}
