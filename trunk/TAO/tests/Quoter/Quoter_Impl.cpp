// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Quoter
//
// = FILENAME
//    Quoter_Impl.cpp
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#include "tao/corba.h"
#include "Quoter_Impl.h"


// Constructor.  Create all the quoter factories.

Quoter_Factory_Impl::Quoter_Factory_Impl (size_t num)
  : my_quoters_ (0),
    quoter_num_ (num),
    next_quoter_ (0)
    
{
  ACE_NEW(this->my_quoters_, Quoter_Impl *[num]);
  
  for (size_t i = 0; i < num; i++)
    ACE_NEW(this->my_quoters_[i], Quoter_Impl("x"));
}


// Destructor

Quoter_Factory_Impl::~Quoter_Factory_Impl (void)
{
  for (size_t i = 0; i < this->quoter_num_; i++)
    delete this->my_quoters_[i];
}


// Return the quoter by the id <name>.

Stock::Quoter_ptr
Quoter_Factory_Impl::create_quoter (const char *name,
                                    CORBA::Environment &env)
{
  ACE_UNUSED_ARG (name);

  this->next_quoter_ = (this->next_quoter_ + 1) % this->quoter_num_;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Quoter %d Created\n", this->next_quoter_));

  return my_quoters_[this->next_quoter_]->_this (env);
}


// Constructor

Quoter_Impl::Quoter_Impl (const char *name, 
			  const unsigned char use_LifeCycle_Service, 
			  PortableServer::POA_ptr poa)
  : use_LifeCycle_Service_ (use_LifeCycle_Service),
    poa_ (poa)
{
  ACE_UNUSED_ARG (name);
}


// Destructor

Quoter_Impl::~Quoter_Impl (void)
{
}


// Returns the current quote for the stock <stock_name>.
// For now, just return 42.  It was a good day on Wall Street.

CORBA::Long
Quoter_Impl::get_quote (char const *stock_name,
                        class CORBA_Environment &env)
{
  ACE_UNUSED_ARG (stock_name);
  ACE_UNUSED_ARG (env);

  return 42;
}


// Make a copy of this object

CosLifeCycle::LifeCycleObject_ptr
Quoter_Impl::copy (CosLifeCycle::FactoryFinder_ptr there,
                   const CosLifeCycle::Criteria &the_criteria,
                   CORBA::Environment &_env_there)
{
  TAO_TRY
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
      CosLifeCycle::Factories_ptr factories_ptr =
        there->find_factories (factoryKey, _env_there);

      // Only a NoFactory exception might have occured, so if it
      // occured, then go immediately back.
      if (_env_there.exception() != 0)
	{
	  // _env_there contains already the exception.            
	  ACE_ERROR ((LM_ERROR,
		      "Quoter::copy: Exception occured while trying to find a factory.\n"));
	  
	  return CosLifeCycle::LifeCycleObject::_nil();
	}
      
      // Now it is known that there is at least one factory.
      Stock::Quoter_var quoter_var;
      
      for (u_int i = 0; i < factories_ptr->length (); i++)
      {
        // Get the first object reference to a factory.
        CORBA::Object_ptr generic_FactoryObj_ptr = (*factories_ptr)[i];
        
        // Narrow it to a Quoter Factory.
        CosLifeCycle::GenericFactory_var generic_Factory_var =
          CosLifeCycle::GenericFactory::_narrow (generic_FactoryObj_ptr,
                                          TAO_TRY_ENV);
        TAO_CHECK_ENV;
	  
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
	  
	      CORBA::Object_var quoterObject_var = 
	          generic_Factory_var->create_object (genericFactoryName,
		  		                          		        criteria,
			  			                                  TAO_TRY_ENV);
	      TAO_CHECK_ENV;
	  
	      quoter_var = Stock::Quoter::_narrow (quoterObject_var.in(), TAO_TRY_ENV);     
	      TAO_CHECK_ENV;
	  
        if (CORBA::is_nil (quoter_var.in ()))
        {
          // If we had already our last chance, then give up.
          if (i == factories_ptr->length ())
          {
            _env_there.exception (new CosLifeCycle::NoFactory (factoryKey));
            ACE_ERROR ((LM_ERROR,
                        "Quoter::copy: Last factory did not work. \n"
                        "No more factories are available. I give up.\n"));
            return CosLifeCycle::LifeCycleObject::_nil();		  
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
      // Duplicate and eturn an object reference to the newly created Quoter.
      return lifeCycleObject_ptr;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
      // Report a NoFactory exception back to the caller
      _env_there.exception (new CosLifeCycle::NoFactory ());
      return CosLifeCycle::LifeCycleObject::_nil();
    }
  TAO_ENDTRY;
  return CosLifeCycle::LifeCycleObject::_nil();
}


// Move this object using <there> and <the_criteria>

void
Quoter_Impl::move (CosLifeCycle::FactoryFinder_ptr there,
                   const CosLifeCycle::Criteria &the_criteria,
                   CORBA::Environment &_env_there)
{
  ACE_DEBUG ((LM_DEBUG,"Quoter_Impl::move: being called\n"));
  
  TAO_TRY
    {
      // We need to have a Factory Finder
      if (CORBA::is_nil (there))
	{
	  ACE_ERROR ((LM_ERROR,
		      "Quoter_Impl::move: No Factory Finder, don't know how to go on.\n"));
	  _env_there.exception (new CosLifeCycle::NoFactory ());
	  return;	  
	}

      // We need to have access to the POA
      if (CORBA::is_nil (poa_))
	{
	  ACE_ERROR ((LM_ERROR,
		      "Quoter_Impl::move: No access to the POA. Cannot move.\n"));
	  _env_there.exception (new CosLifeCycle::NotMovable ());
	  return;	  
	}

      // Create a new Quoter over there
      CosLifeCycle::LifeCycleObject_var lifeCycleObject_var =       
	this->copy (there, the_criteria, _env_there);

      if (_env_there.exception () != 0)
	{
	  ACE_ERROR ((LM_ERROR,
		      "Quoter_Impl::move: Exception while creating new Quoter.\n"));
	  // The exception is already contained in the right environment
	  return;
	}

      if (CORBA::is_nil (lifeCycleObject_var.in ()))
	{
	  ACE_ERROR ((LM_ERROR,
		      "Quoter_Impl::move: Created Quoter is not valid.\n"));
	  _env_there.exception (new CosLifeCycle::NoFactory ());
	  return;
	}
            
      // Set the POA, so that the requests will be forwarded to the new location
      
      // new location
      CORBA::Object_var forward_to_var = (CORBA::Object_ptr) lifeCycleObject_var.in();
      
      if (!CORBA::is_nil (forward_to_var.in ()))
	{
	  PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this, TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	  
	  PortableServer::Servant servant = this->poa_->_servant ();

	  if (servant == 0)
	    {
	      ACE_ERROR ((LM_ERROR,"Quoter_Impl::move: Could not find servant.\n"));
	      _env_there.exception (new CosLifeCycle::NotMovable());
	      return;
	    }
	  
	  void *ptr = servant->_downcast ("IDL:PortableServer/POA:1.0");
	  POA_PortableServer::POA *poa = (POA_PortableServer::POA *) ptr;
	  TAO_POA *tao_poa = ACE_dynamic_cast (TAO_POA *, poa);
	  
	  tao_poa->forward_object (oid.in (),
				   forward_to_var.in (),
				   TAO_TRY_ENV);
	  TAO_CHECK_ENV;  
	}
      else
	{
	  ACE_ERROR ((LM_ERROR,"Quoter_Impl::move: forward_to refenence is nil.\n"));
	  _env_there.exception (new CosLifeCycle::NotMovable());
	  return;
	}
      
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
      return;
    }
  TAO_ENDTRY;   
}


// Removes the object.  Once we shut down the ORB we can call it a day.

void
Quoter_Impl::remove (CORBA::Environment &_tao_environment)
{
  ACE_UNUSED_ARG (_tao_environment);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "I have been asked to shut down.\n"));
	      
  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}
