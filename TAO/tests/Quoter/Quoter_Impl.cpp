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

Quoter_Impl::Quoter_Impl (const char *name, const unsigned char use_LifeCycle_Service)
  : use_LifeCycle_Service_ (use_LifeCycle_Service)
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
      
      if (this->useLifeCycleService_ == 1)
	{
	  // use the LifeCycle Service
	  factoryKey.length(1);
	  factoryKey[0].id = CORBA::string_dup ("Life_Cycle_Service");
	}
      else // which means this->useLifeCycleService_ == 0, default setting
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
          return CosLifeCycle::LifeCycleObject::_nil();
        }
      
      // Now it is known that there is at least one factory.
      Stock::Quoter_var quoter_var;
      
      for (u_int i = 0; i < factories_ptr->length (); i++)
        {
          // Get the first object reference to a factory.
          CORBA::Object_ptr quoter_FactoryObj_ptr = (*factories_ptr)[i];
	  
          // Narrow it to a Quoter Factory.
          Stock::Quoter_Factory_var quoter_Factory_var =
            Stock::Quoter_Factory::_narrow (quoter_FactoryObj_ptr,
                                            TAO_TRY_ENV);
          TAO_CHECK_ENV;
	  
          if (CORBA::is_nil (quoter_Factory_var.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
			       "Quoter::copy: Narrow failed. Factory is not valid.\n"),
			      0);
	  
          // Try to get a Quoter created by this factory.
          // and duplicate the pointer to it
          quoter_var = Stock::Quoter::_duplicate (
						  quoter_Factory_var->create_quoter (
										     "quoter_copied",
										     TAO_TRY_ENV));
	  
          // @@ mk1: The create_quoter should return an exception
          TAO_CHECK_ENV;
	  
          if (CORBA::is_nil (quoter_var.in ()))
            {
              // If we had already our last chance, then give up.
              if (i == factories_ptr->length ())
                {
                  _env_there.exception (new CosLifeCycle::NoFactory (factoryKey));
                  return CosLifeCycle::LifeCycleObject::_nil();
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "Quoter::copy: Factory did not create the Quoter properly.\n"));
                  // Else tell what's wrong and try the next factory.
                }
            }
          else
            break;
	  // if succeeded in creating a new Quoter over there, then stop trying
        }

      // Return an object reference to the newly created Quoter.
      return (CosLifeCycle::LifeCycleObject_ptr) quoter_var.ptr();
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
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
      // Get the location over there as a CORBA::Object_var
      
      // The name of the Generic Factory
      CosLifeCycle::Key factoryKey (2);  // max = 2
      
      if (this->useLifeCycleService_ == 1)
	    {
	      // use the LifeCycle Service
	      factoryKey.length(1);
	      factoryKey[0].id = CORBA::string_dup ("Life_Cycle_Service");
	    }
      else // which means this->useLifeCycleService_ == 0, default setting
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
	        _env_there.exception (new CosLifeCycle::NotMovable());
          // _env_there contains already the exception.
          return CosLifeCycle::LifeCycleObject::_nil();
        }
      
      // Now it is known that there is at least one factory.
      Stock::Quoter_var quoter_var;
      
      for (u_int i = 0; i < factories_ptr->length (); i++)
        {
          // Get the first object reference to a factory.
          CORBA::Object_ptr quoter_FactoryObj_ptr = (*factories_ptr)[i];
	  
          // Narrow it to a Quoter Factory.
          Stock::Quoter_Factory_var quoter_Factory_var =
            Stock::Quoter_Factory::_narrow (quoter_FactoryObj_ptr,
                                            TAO_TRY_ENV);
          TAO_CHECK_ENV;
	  
          if (CORBA::is_nil (quoter_Factory_var.in ()))
	    {
	      env_there.exception (new );
	      ACE_ERROR_RETURN ((LM_ERROR,
				 "Quoter::move: Narrow failed. Factory is not valid.\n"),
				0);
	    }
	  
	  // Try to get a Quoter created by this factory.
	  // and duplicate the pointer to it
	  quoter_var = Stock::Quoter::_duplicate (
						  quoter_Factory_var->create_quoter (
										     "quoter_moved", 
										     TAO_TRY_ENV));
      
	  // @@ mk1: The create_quoter should return an exception
	  TAO_CHECK_ENV;
	  
	  if (CORBA::is_nil (quoter_var.in ()))
	    {
              // If we had already our last chance, then give up.
	      if (i == factories_ptr->length ())
		{
		  _env_there.exception (new CosLifeCycle::NoFactory (factoryKey));
		  return CosLifeCycle::LifeCycleObject::_nil();
		}
	      else
		{
		  (ew CosLifeCycle::NotMovable());
		  ACE_ERROR ((LM_ERROR,
			      "Quoter::copy: Factory did not create the Quoter properly.\n"));
		  // Else tell what's wrong and try the next factory.
		}
	    }
	  else
	    break;
	  // if succeeded in creating a new Quoter over there, then stop trying
	}
      
      // Return an object reference to the newly created Quoter.
      
      CORBAL::Object_var forward_to = (CORBA::Object_var) quoter_var;
      
      if (!CORBA::is_nil (forward_to.in ()))
	{
	  PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this, env);
	  
	  if (env.exception () != 0)
	    return;
	  
	  PortableServer::Servant servant = this->poa_->_servant ();
	  if (servant == 0)
	    {
	      _env_there.exception (new CosLifeCycle::NotMovable());
	      return;
	    }
	  
	  void *ptr = servant->_downcast ("IDL:PortableServer/POA:1.0");
	  POA_PortableServer::POA *poa = (POA_PortableServer::POA *) ptr;
	  TAO_POA *tao_poa = ACE_dynamic_cast (TAO_POA *, poa);
	  
	  tao_poa->forward_object (oid.in (),
				   forward_to.in (),
				   TAO_TRY_ENV);
	  TAO_CHECK_ENV
	    }
      else
	{
	  ACE_DEBUG ((LM_DEBUG,"::move: forward_to refenence is nil.\n"));
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
  
  // for later
  // this->copy (there, the_criteria, _env_there);
  
  // the move operation is not implemented yet, because of the issue,
  // that the object reference has to stay the same. But if it has
  // to stay the same this object, the old object, has to forward
  // further calls.
  
  
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









