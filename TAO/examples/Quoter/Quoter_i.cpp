// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Quoter
//
// = FILENAME
//    Quoter_i.cpp
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#include "Quoter_i.h"
#include "tao/PortableServer/POA.h"
#include "tao/corba.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"

ACE_RCSID(Quoter, Quoter_i, "$Id$")

// Constructor.  Create all the quoter factories.

Quoter_Factory_i::Quoter_Factory_i (size_t num, PortableServer::POA_ptr poa_ptr)
  : poa_ptr_ (poa_ptr),
    my_quoters_ (0),
    quoter_num_ (num),
    next_quoter_ (0)
{
  // Nothing
}


// Destructor

Quoter_Factory_i::~Quoter_Factory_i (void)
{
  for (size_t i = 0; i < this->quoter_num_; i++)
    delete this->my_quoters_[i];
  delete [] this->my_quoters_;
}


// Initialize everything in the factory

int Quoter_Factory_i::init (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_NEW_RETURN (this->my_quoters_,
                  Quoter_i *[this->quoter_num_],
                  -1);

  for (size_t i = 0; i < this->quoter_num_; i++)
    {
      ACE_NEW_RETURN (this->my_quoters_[i],
                      Quoter_i("x",  // name
                               0,    // don't use the LifeCycle_Service
                               this->poa_ptr_), // a reference to the poa
                      -1);

      const char *location_string = "POA::activate";
      ACE_TRY
        {
          this->poa_ptr_->activate_object (this->my_quoters_[i] ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          location_string = "_this";
          Stock::Quoter_var quoter_var = this->my_quoters_[i]->_this(ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          location_string = "CORBA::ORB::object_to_string";
          // Stringify the object reference and print it out.
          CORBA::String_var quoter_ior =
            TAO_ORB_Core_instance()->orb()->object_to_string (quoter_var.in ()
                                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, location_string);
          return -1;
        }
      ACE_ENDTRY;
    }

  // Everything is ok
  return 0;
}


// Return the quoter by the id <name>.

Stock::Quoter_ptr
Quoter_Factory_i::create_quoter (const char *
                                 ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Stock::Invalid_Quoter))
{
  this->next_quoter_ = (this->next_quoter_ + 1) % this->quoter_num_;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Quoter %d Created\n", this->next_quoter_));

  return my_quoters_[this->next_quoter_]->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}


// Constructor

Quoter_i::Quoter_i (const char *name,
                    const unsigned char use_LifeCycle_Service,
                    PortableServer::POA_ptr poa_ptr)
  : use_LifeCycle_Service_ (use_LifeCycle_Service),
    poa_var_ (PortableServer::POA::_duplicate (poa_ptr))
{
  ACE_UNUSED_ARG (name);
}


// Destructor

Quoter_i::~Quoter_i (void)
{
  // Nothing
}


// Returns the current quote for the stock <stock_name>.
// For now, just return 42.  It was a good day on Wall Street.

CORBA::Long
Quoter_i::get_quote (char const *
                     ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Stock::Invalid_Stock,
                       Stock::Invalid_Quoter))
{
  return 42;
}


// Make a copy of this object

CosLifeCycle::LifeCycleObject_ptr
Quoter_i::copy (CosLifeCycle::FactoryFinder_ptr there,
                const CosLifeCycle::Criteria &/*the_criteria*/
                ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosLifeCycle::NoFactory,
                       CosLifeCycle::NotCopyable,
                       CosLifeCycle::InvalidCriteria,
                       CosLifeCycle::CannotMeetCriteria))
{
  const char *exception_message = "Null message";
  CosLifeCycle::LifeCycleObject_ptr lifeCycleObject_ptr =
CosLifeCycle::LifeCycleObject::_nil ();
  ACE_TRY
    {
      // The name of the Generic Factory
      CosLifeCycle::Key factoryKey (2);  // max = 2

      if (this->use_LifeCycle_Service_ == 1)
        {
          // use the LifeCycle Service
          factoryKey.length(1);
          factoryKey[0].id = CORBA::string_dup ("Life_Cycle_Service");
        }
      else
        {
          // use a Generic Factory
          factoryKey.length(2);
          factoryKey[0].id = CORBA::string_dup ("IDL_Quoter");
          factoryKey[1].id = CORBA::string_dup ("Quoter_Generic_Factory");
        }

      // Find an appropriate factory over there.
      exception_message = "While trying to find a factory.\n";
      CosLifeCycle::Factories *factories_ptr =
        there->find_factories (factoryKey ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Now it is known that there is at least one factory.
      Stock::Quoter_var quoter_var;

      for (u_int i = 0; i < factories_ptr->length (); i++)
      {
        // Get the first object reference to a factory.
        CORBA::Object_ptr generic_FactoryObj_ptr = (*factories_ptr)[i];

        // Narrow it to a Quoter Factory.
        exception_message = "While narrowing.\n";
        CosLifeCycle::GenericFactory_var generic_Factory_var =
          CosLifeCycle::GenericFactory::_narrow (generic_FactoryObj_ptr
                                                 ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (CORBA::is_nil (generic_Factory_var.in ()))
          {
            ACE_ERROR ((LM_ERROR,
                       "Quoter::copy: Narrow failed. Generic Factory is not valid.\n"));
            return CosLifeCycle::LifeCycleObject::_nil();
          }

          CosLifeCycle::Key genericFactoryName (1);  // max = 1
          genericFactoryName.length(1);
          genericFactoryName[0].id = CORBA::string_dup ("Quoter_Factory");

          CosLifeCycle::Criteria criteria(1);
          criteria.length (1);
          criteria[0].name = CORBA::string_dup ("filter");
          criteria[0].value <<= CORBA::string_dup ("name=='Quoter_Generic_Factory'");

          exception_message = "While creating an object";
          CORBA::Object_var quoterObject_var =
            generic_Factory_var->create_object (genericFactoryName,
                                                criteria
                                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          exception_message = "While narrowing object";
          quoter_var = Stock::Quoter::_narrow (quoterObject_var.in() ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (quoter_var.in ()))
            {
              // If we had already our last chance, then give up.
              if (i == factories_ptr->length ())
                {
                  ACE_ERROR ((LM_ERROR,
                              "Quoter::copy: Last factory did not work. \n"
                              "No more factories are available. I give up.\n"));
                  ACE_THROW_RETURN (CosLifeCycle::NoFactory (factoryKey),
                                    CosLifeCycle::LifeCycleObject::_nil());
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "Quoter::copy: Factory did not create the Quoter properly.\n"));
                  // Tell what's wrong and try the next factory.
                }
            }
          else
            // if succeeded in creating a new Quoter over there, then stop trying
            break;
        }

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Quoter copied\n"));

      lifeCycleObject_ptr =
        CosLifeCycle::LifeCycleObject::_duplicate ((CosLifeCycle::LifeCycleObject_ptr) quoter_var.in());
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Quoter_i::copy - %s\n", exception_message));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception");

      // Report a NoFactory exception back to the caller
      ACE_TRY_THROW (CosLifeCycle::NoFactory ());
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (CosLifeCycle::LifeCycleObject::_nil());

  return lifeCycleObject_ptr;
}


// Move this object using <there> and <the_criteria>

void
Quoter_i::move (CosLifeCycle::FactoryFinder_ptr /* there */,
                const CosLifeCycle::Criteria & /* the_criteria */
                ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosLifeCycle::NoFactory,
                       CosLifeCycle::NotMovable,
                       CosLifeCycle::InvalidCriteria,
                       CosLifeCycle::CannotMeetCriteria))
{
  ACE_ERROR ((LM_ERROR,
              "Quoter_i::move: The Quoter object is not movable!"));

  ACE_THROW (CosLifeCycle::NotMovable());
}


// Removes the object.  Once we shut down the ORB we can call it a day.

void
Quoter_i::remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosLifeCycle::NotRemovable))
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "I have been asked to shut down.\n"));

  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}
