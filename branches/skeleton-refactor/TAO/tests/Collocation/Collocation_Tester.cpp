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
Collocation_Test::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->root_poa_->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->orb_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

int
Collocation_Test::init (int argc, char *argv[] ACE_ENV_ARG_DECL)
{
  // Initialize the ORB.
  this->orb_ = CORBA::ORB_init (argc, argv, 0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  int result = this->parse_args (argc, argv);
  if (result != 0)
    return result;

  // Get an Object reference to RootPOA.
  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Narrow the Object reference to a POA reference
  this->root_poa_ =
    PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Get the POAManager of RootPOA
  this->poa_manager_ =
    this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Activate the diamond servant and its base classes under RootPOA.
  PortableServer::ObjectId_var id =
    this->root_poa_->activate_object (&this->top_servant_
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

//    // We only care about the most derived class here.
//    this->diamond_obj_ = this->diamond_servant_._this (ACE_ENV_SINGLE_ARG_PARAMETER);
//    ACE_CHECK_RETURN (-1);

  id =
    this->root_poa_->activate_object (&this->diamond_servant_
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // We only care about the most derived class here.
  this->diamond_obj_ = this->root_poa_->id_to_reference (id.in ()
                                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  id =
    this->root_poa_->activate_object (&this->left_servant_
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  id =
    this->root_poa_->activate_object (&this->right_servant_
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::String_var str =
    this->orb_->object_to_string (this->diamond_obj_.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

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
Collocation_Test::test_narrow (ACE_ENV_SINGLE_ARG_DECL)
{
  Diamond::Top_var top =
    Diamond::Top::_narrow (this->diamond_obj_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  Diamond::Left_var left =
    Diamond::Left::_narrow (this->diamond_obj_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  Diamond::Right_var right =
    Diamond::Right::_narrow (this->diamond_obj_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  Diamond::Buttom_var buttom =
    Diamond::Buttom::_narrow (this->diamond_obj_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::String_var str = top->shape (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  ACE_DEBUG ((LM_DEBUG, "Calling top->shape: %s\n", str.in ()));

  str = left->shape (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  ACE_DEBUG ((LM_DEBUG, "Calling left->shape: %s\n", str.in ()));

  str = right->shape (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  ACE_DEBUG ((LM_DEBUG, "Calling right->shape: %s\n", str.in ()));

  str = buttom->shape (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  ACE_DEBUG ((LM_DEBUG, "Calling buttom->shape: %s\n", str.in ()));

  return 0;
}

int
Collocation_Test::run (ACE_ENV_SINGLE_ARG_DECL)
{
  this->poa_manager_->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->test_narrow (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}
