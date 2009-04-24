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
Collocation_Test::init (int argc, ACE_TCHAR *argv[])
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

  // Activate the diamond servant and its base classes under RootPOA.
  PortableServer::ObjectId_var id =
    this->root_poa_->activate_object (&this->top_servant_);

//    // We only care about the most derived class here.
//    this->diamond_obj_ = this->diamond_servant_._this ();

  id =
    this->root_poa_->activate_object (&this->diamond_servant_);

  // We only care about the most derived class here.
  this->diamond_obj_ = this->root_poa_->id_to_reference (id.in ());

  id =
    this->root_poa_->activate_object (&this->left_servant_);

  id =
    this->root_poa_->activate_object (&this->right_servant_);

  CORBA::String_var str =
    this->orb_->object_to_string (this->diamond_obj_.in ());

  ACE_DEBUG ((LM_DEBUG, "Diamond Servant activated:\n %C\n",
              str.in()));

  return 0;


}

int
Collocation_Test::parse_args (int /*argc*/,
                              ACE_TCHAR *[] /*argv*/)
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
  ACE_DEBUG ((LM_DEBUG, "Calling top->shape: %C\n", str.in ()));

  str = left->shape ();
  ACE_DEBUG ((LM_DEBUG, "Calling left->shape: %C\n", str.in ()));

  str = right->shape ();
  ACE_DEBUG ((LM_DEBUG, "Calling right->shape: %C\n", str.in ()));

  str = buttom->shape ();
  ACE_DEBUG ((LM_DEBUG, "Calling buttom->shape: %C\n", str.in ()));

  return 0;
}

int
Collocation_Test::run (void)
{
  this->poa_manager_->activate ();

  this->test_narrow ();

  return 0;
}
