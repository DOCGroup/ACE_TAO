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

void
Collocation_Test::shutdown ()
{
  this->root_poa_->destroy (1, 1);
  this->orb_->destroy ();
}

int
Collocation_Test::init (int argc, ACE_TCHAR *argv[])
{
  // Initialize the ORB.
  this->orb_ = CORBA::ORB_init (argc, argv);

  int const result = this->parse_args (argc, argv);
  if (result != 0)
    return result;

  // Get an Object reference to RootPOA.
  CORBA::Object_var const obj {this->orb_->resolve_initial_references ("RootPOA")};

  // Narrow the Object reference to a POA reference
  this->root_poa_ =
    PortableServer::POA::_narrow (obj.in ());

  // Get the POAManager of RootPOA
  this->poa_manager_ =
    this->root_poa_->the_POAManager ();

  // Activate the diamond servant and its base classes under RootPOA.
  PortableServer::ObjectId_var id {this->root_poa_->activate_object (&this->top_servant_)};

  id =
    this->root_poa_->activate_object (&this->diamond_servant_);

  // We only care about the most derived class here.
  this->diamond_obj_ = this->root_poa_->id_to_reference (id.in ());

  id =
    this->root_poa_->activate_object (&this->left_servant_);

  id =
    this->root_poa_->activate_object (&this->right_servant_);

  CORBA::String_var const str =
    this->orb_->object_to_string (this->diamond_obj_.in ());

  ACE_DEBUG ((LM_DEBUG, "Diamond Servant activated:\n %C\n",
              str.in ()));

  return 0;

}

int
Collocation_Test::parse_args (int /*argc*/,
                              ACE_TCHAR *[] /*argv*/)
{
  return 0;
}

int
Collocation_Test::test_narrow ()
{
  Diamond::Top_var const top {Diamond::Top::_narrow (this->diamond_obj_.in ())};

  Diamond::Left_var const left {Diamond::Left::_narrow (this->diamond_obj_.in ())};

  Diamond::Right_var const right {Diamond::Right::_narrow (this->diamond_obj_.in ())};

  Diamond::Buttom_var const buttom {Diamond::Buttom::_narrow (this->diamond_obj_.in ())};

  CORBA::String_var const top_shape {top->shape ()};
  ACE_DEBUG ((LM_DEBUG, "Calling top->shape: %C\n", top_shape.in ()));

  CORBA::String_var const left_shape {left->shape ()};
  ACE_DEBUG ((LM_DEBUG, "Calling left->shape: %C\n", left_shape.in ()));

  CORBA::String_var const right_shape {right->shape ()};
  ACE_DEBUG ((LM_DEBUG, "Calling right->shape: %C\n", right_shape.in ()));

  CORBA::String_var const buttom_shape {buttom->shape ()};
  ACE_DEBUG ((LM_DEBUG, "Calling buttom->shape: %C\n", buttom_shape.in ()));

  return 0;
}

int
Collocation_Test::run ()
{
  this->poa_manager_->activate ();

  this->test_narrow ();

  return 0;
}
