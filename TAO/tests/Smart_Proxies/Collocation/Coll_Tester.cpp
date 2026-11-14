/**
 * @file Coll_Tester.cpp
 * @brief Implements the collocation test.
 * @author Nanbor Wang
 */

#include "Coll_Tester.h"
#include "Smart_Proxy_Impl.h"
#include <memory>

Collocation_Test::~Collocation_Test ()
{
  if (!CORBA::is_nil (this->root_poa_.in ()))
    {
      try
        {
          this->root_poa_->destroy (true, true);
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("Unable to destroy RootPOA");
        }
    }
}

int
Collocation_Test::init (int argc, ACE_TCHAR *argv[])
{
  // Initialize the ORB.
  this->orb_ = CORBA::ORB_init (argc, argv);

  // Get an Object reference to RootPOA.
  CORBA::Object_var obj = this->orb_->resolve_initial_references ("RootPOA");

  // Narrow the Object reference to a POA reference
  this->root_poa_ = PortableServer::POA::_narrow (obj.in ());

  // Get the POAManager of RootPOA
  this->poa_manager_ =
    this->root_poa_->the_POAManager ();

  // Activate the diamond servant and its base classes under RootPOA.
  PortableServer::ObjectId_var id =
    this->root_poa_->activate_object (std::addressof (this->top_servant_));
  id = this->root_poa_->activate_object (
    std::addressof (this->diamond_servant_));

  // We only care about the most derived class here.
  this->diamond_obj_ = this->root_poa_->id_to_reference (id.in ());

  id = this->root_poa_->activate_object (
    std::addressof (this->left_servant_));
  id = this->root_poa_->activate_object (
    std::addressof (this->right_servant_));

  CORBA::String_var str =
    this->orb_->object_to_string (this->diamond_obj_.in ());

  ACE_DEBUG ((LM_DEBUG, "Diamond Servant activated:\n %s\n",
              str.in ()));

  return 0;
}

int
Collocation_Test::test_narrow ()
{
  // Ensure that the smart proxy is the one which is used by registering
  // the user-defined factory. Its necessary to create one on the heap so
  // the lifetime of the factory object can be managed by the framework.
  Smart_Diamond_Top_Factory *factory {};
  ACE_NEW_RETURN (factory,
                  Smart_Diamond_Top_Factory,
                  -1);
  ACE_UNUSED_ARG (factory);

  Diamond::Top_var top =
    Diamond::Top::_narrow (this->diamond_obj_.in());

  Diamond::Left_var left =
    Diamond::Left::_narrow (this->diamond_obj_.in());

  Diamond::Right_var right =
    Diamond::Right::_narrow (this->diamond_obj_.in());

  Diamond::Buttom_var buttom =
    Diamond::Buttom::_narrow (this->diamond_obj_.in());

  CORBA::String_var str = top->shape ();
  ACE_DEBUG ((LM_DEBUG, "Calling top->shape: %s\n", str.in ()));

  str = left->shape ();
  ACE_DEBUG ((LM_DEBUG, "Calling left->shape: %s\n", str.in ()));

  CORBA::Long inout {2};
  CORBA::Long out {};
  CORBA::Long const result = left->combine (3, inout, out);
  if (result != 5 || inout != 5 || out != 5)
    ACE_ERROR_RETURN ((LM_ERROR, "left->combine returned unexpected values\n"), -1);

  str = right->shape ();
  ACE_DEBUG ((LM_DEBUG, "Calling right->shape: %s\n", str.in ()));

  str = buttom->shape ();
  ACE_DEBUG ((LM_DEBUG, "Calling buttom->shape: %s\n", str.in ()));

  return 0;
}

int
Collocation_Test::run ()
{
  this->poa_manager_->activate ();

  return this->test_narrow ();
}
