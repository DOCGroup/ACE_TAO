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
//    Darrell Brunsch
//
// ============================================================================

#include "tao/corba.h"
#include "Quoter_Impl.h"
//#include "QuoterC.h"

// Constructor

Quoter_Factory_Impl::Quoter_Factory_Impl (void)
{
}

// Destructor

Quoter_Factory_Impl::~Quoter_Factory_Impl (void)
{
}

Stock::Quoter_ptr
Quoter_Factory_Impl::create_quoter (const char *name,
                                    CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,
              "Quoter Created\n"));
  ACE_UNUSED_ARG (name);
  return my_quoter_._this (env);
}

// Constructor

Quoter_Impl::Quoter_Impl (const char *)
{
}

// Destructor

Quoter_Impl::~Quoter_Impl (void)
{
}

CORBA::Long
Quoter_Impl::get_quote (char const *stock_name,
                        class CORBA_Environment &env)
{
  ACE_UNUSED_ARG (stock_name);
  ACE_UNUSED_ARG (env);

  return 42;
}

// Shutdown.

void Quoter_Impl::destroy (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  ACE_DEBUG ((LM_DEBUG,
              "%s",
              "I have been asked to shut down "));

  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}

CosLifeCycle::LifeCycleObject_ptr
Quoter_Impl::copy (CosLifeCycle::FactoryFinder_ptr there,
                   const CosLifeCycle::Criteria &the_criteria,
                   CORBA::Environment &_env_there)
{
  TAO_TRY
    {
      // The name of the Generic Factory
      CosLifeCycle::Key factoryKey (2);  // max = 2
      
      // future enhancement
      /* if (this->useLifeCycleService_ == 1)
	{
	  // use the LifeCycle Service
	  factoryKey.length(1);
	  factoryKey[0].id = CORBA::string_dup ("Life_Cycle_Service");
	}
      else*/
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
                        "Quoter::copy: Factory did not create the Quoter properly.\n"),
                        0);

          // Try to get a Quoter created by this factory.
          // and duplicate the pointer to it
          quoter_var = Stock::Quoter::_duplicate (
            quoter_Factory_var->create_quoter ("quoter_copied", TAO_TRY_ENV));

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

void
Quoter_Impl::move (CosLifeCycle::FactoryFinder_ptr there,
                   const CosLifeCycle::Criteria &the_criteria,
                   CORBA::Environment &_env_there)
{
  // for later
  // this->copy (there, the_criteria, _env_there);

  // the move operation is not implemented yet, because of the issue,
  // that the object reference has to stay the same. But if it has
  // to stay the same this object. the old object, has to forward
  // further calls.

  _env_there.exception (new CosLifeCycle::NotMovable());
}

void
Quoter_Impl::remove (CORBA::Environment &_tao_environment)
{
}
