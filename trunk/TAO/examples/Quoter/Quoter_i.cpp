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

#include "tao/corba.h"
#include "Quoter_i.h"

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

int Quoter_Factory_i::init (CORBA::Environment &ACE_TRY_ENV)
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
          this->poa_ptr_->activate_object (this->my_quoters_[i], ACE_TRY_ENV);
          ACE_TRY_CHECK;

          location_string = "_this";
          Stock::Quoter_var quoter_var = this->my_quoters_[i]->_this(ACE_TRY_ENV);
          ACE_TRY_CHECK;

          location_string = "CORBA::ORB::object_to_string";
          // Stringify the object reference and print it out.
          CORBA::String_var quoter_ior =
            TAO_ORB_Core_instance()->orb()->object_to_string (quoter_var.in (),
                                                              ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          ACE_TRY_ENV.print_exception (location_string);
          return -1;
        }
      ACE_ENDTRY;
    }

  // Everything is ok
  return 0;
}


// Return the quoter by the id <name>.

Stock::Quoter_ptr
Quoter_Factory_i::create_quoter (const char *name,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (name);

  this->next_quoter_ = (this->next_quoter_ + 1) % this->quoter_num_;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Quoter %d Created\n", this->next_quoter_));

  return my_quoters_[this->next_quoter_]->_this (ACE_TRY_ENV);
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
Quoter_i::get_quote (char const *stock_name,
                     class CORBA_Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (stock_name);
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  return 42;
}


// Make a copy of this object

CosLifeCycle::LifeCycleObject_ptr
Quoter_i::copy (CosLifeCycle::FactoryFinder_ptr there,
                const CosLifeCycle::Criteria &the_criteria,
                CORBA::Environment &ACE_TRY_ENV)
{
  const char *exception_message = "Null message";
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
      CosLifeCycle::Factories_ptr factories_ptr =
        there->find_factories (factoryKey, ACE_TRY_ENV);
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
          CosLifeCycle::GenericFactory::_narrow (generic_FactoryObj_ptr,
                                                 ACE_TRY_ENV);
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
                                                criteria,
                                                ACE_TRY_ENV);
          ACE_TRY_CHECK;

          exception_message = "While narrowing object";
          quoter_var = Stock::Quoter::_narrow (quoterObject_var.in(), ACE_TRY_ENV);
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

      CosLifeCycle::LifeCycleObject_ptr lifeCycleObject_ptr =
        CosLifeCycle::LifeCycleObject::_duplicate ((CosLifeCycle::LifeCycleObject_ptr) quoter_var.in());
  
      // Duplicate and return an object reference to the newly created Quoter.
      return lifeCycleObject_ptr;
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "Quoter_i::copy - %s\n", exception_message));
      ACE_TRY_ENV.print_exception ("SYS_EX");
      
      // Report a NoFactory exception back to the caller
      ACE_THROW (CosLifeCycle::NoFactory ());
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (CosLifeCycle::LifeCycleObject::_nil());
  
  return 0;
}


// Move this object using <there> and <the_criteria>

void
Quoter_i::move (CosLifeCycle::FactoryFinder_ptr there,
                const CosLifeCycle::Criteria &the_criteria,
                CORBA::Environment &ACE_TRY_ENV)
{
  const char *exception_message = "Null message";
  
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,"Quoter_i::move: being called\n"));

  ACE_TRY
    {
      // We need to have a Factory Finder
      if (CORBA::is_nil (there))
        {
          ACE_ERROR ((LM_ERROR,
                      "Quoter_i::move: No Factory Finder, don't know how to go on.\n"));
          
          ACE_THROW (CosLifeCycle::NoFactory ());
        }

      // We need to have access to the POA
      if (CORBA::is_nil (this->poa_var_.in()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Quoter_i::move: No access to the POA. Cannot move.\n"));
          ACE_THROW (CosLifeCycle::NotMovable ());
        }

      // Create a new Quoter over there
      exception_message = "While creating a new Quoter";
      CosLifeCycle::LifeCycleObject_var lifeCycleObject_var =
        this->copy (there, the_criteria, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (lifeCycleObject_var.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Quoter_i::move: Created Quoter is not valid.\n"));
          ACE_THROW (CosLifeCycle::NoFactory ());
        }

      // Set the POA, so that the requests will be forwarded to the new location

      // new location
      CORBA::Object_var forward_to_var = 
        CORBA::Object::_duplicate ((CORBA::Object_ptr) lifeCycleObject_var.in());

      if (!CORBA::is_nil (forward_to_var.in ()))
        {
          exception_message = "While servant_to_id";
          PortableServer::ObjectId_var oid = this->poa_var_->servant_to_id (this, ACE_TRY_ENV);
          ACE_TRY_CHECK;

          PortableServer::Servant servant = this->poa_var_->_servant ();

          if (servant == 0)
            {
              ACE_ERROR ((LM_ERROR,"Quoter_i::move: Could not find servant.\n"));
              ACE_THROW (CosLifeCycle::NotMovable());
            }

          void *ptr = servant->_downcast ("IDL:PortableServer/POA:1.0");
          POA_PortableServer::POA *poa = (POA_PortableServer::POA *) ptr;
          TAO_POA *tao_poa = ACE_dynamic_cast (TAO_POA *, poa);

          exception_message = "While forwarding object";
          tao_poa->forward_object (oid.in (),
                                   forward_to_var.in (),
                                   ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      else
        {
          ACE_ERROR ((LM_ERROR,"Quoter_i::move: forward_to refenence is nil.\n"));
          ACE_THROW (CosLifeCycle::NotMovable());
        }
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,"Quoter_i::move: %s\n", exception_message));
      ACE_TRY_ENV.print_exception ("SYS_EX");
      return;
    }
  ACE_ENDTRY;
}


// Removes the object.  Once we shut down the ORB we can call it a day.

void
Quoter_i::remove (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (ACE_TRY_ENV);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "I have been asked to shut down.\n"));

  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}
