
//=============================================================================
/**
 *  @file    Quoter_i.cpp
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================


#include "Quoter_i.h"

#include <new>
#include <utility>

#include "tao/debug.h"
#include "tao/ORB_Core.h"

// Constructor.  Create all the quoter factories.

Quoter_Factory_i::Quoter_Factory_i (size_t num, PortableServer::POA_ptr poa_ptr)
  : poa_var_ (PortableServer::POA::_duplicate (poa_ptr)),
    quoter_num_ (num)
{
}

// Initialize everything in the factory

int Quoter_Factory_i::init ()
{
  if (this->quoter_num_ == 0 || this->quoter_num_ > this->quoters_.max_size ())
    return -1;

  try
    {
      this->quoters_.reserve (this->quoter_num_);
    }
  catch (std::bad_alloc const&)
    {
      return -1;
    }

  for (size_t i = 0; i < this->quoter_num_; i++)
    {
      Quoter_i *quoter = nullptr;
      ACE_NEW_RETURN (quoter,
                      Quoter_i (false, this->poa_var_.in ()),
                      -1);
      std::unique_ptr<Quoter_i> quoter_owner (quoter);
      this->quoters_.push_back (std::move (quoter_owner));
      Quoter_i *const servant = this->quoters_.back ().get ();

      char const *location_string = "POA::activate";
      try
        {
          this->poa_var_->activate_object (servant);

          location_string = "_this";
          Stock::Quoter_var quoter_var = servant->_this ();

          location_string = "CORBA::ORB::object_to_string";
          // Stringify the object reference and print it out.
          CORBA::String_var quoter_ior =
            TAO_ORB_Core_instance()->orb()->object_to_string (quoter_var.in ());
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception (location_string);
          return -1;
        }
    }

  // Everything is ok
  return 0;
}


// Return the quoter by the id <name>.

Stock::Quoter_ptr
Quoter_Factory_i::create_quoter (char const *)
{
  if (this->quoters_.empty ())
    throw CORBA::INTERNAL ();

  this->next_quoter_ = (this->next_quoter_ + 1) % this->quoter_num_;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Quoter %d Created\n", this->next_quoter_));

  return this->quoters_[this->next_quoter_].get ()->_this ();
}


// Constructor

Quoter_i::Quoter_i (bool use_LifeCycle_Service,
                    PortableServer::POA_ptr poa_ptr)
  : use_LifeCycle_Service_ (use_LifeCycle_Service),
    poa_var_ (PortableServer::POA::_duplicate (poa_ptr))
{
}


// Returns the current quote for the stock <stock_name>.
// For now, just return 42.  It was a good day on Wall Street.

CORBA::Long
Quoter_i::get_quote (char const *)
{
  return 42;
}


// Make a copy of this object

CosLifeCycle::LifeCycleObject_ptr
Quoter_i::copy (CosLifeCycle::FactoryFinder_ptr there,
                CosLifeCycle::Criteria const &/*the_criteria*/)
{
  char const *exception_message = "Null message";
  CosLifeCycle::LifeCycleObject_ptr lifeCycleObject_ptr =
    CosLifeCycle::LifeCycleObject::_nil ();
  try
    {
      // The name of the Generic Factory
      CosLifeCycle::Key factoryKey (2);  // max = 2

      if (this->use_LifeCycle_Service_)
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
      CosLifeCycle::Factories_var factories =
        there->find_factories (factoryKey);
      if (factories.ptr () == nullptr)
        throw CosLifeCycle::NoFactory (factoryKey);

      // Now it is known that there is at least one factory.
      Stock::Quoter_var quoter_var;

      for (CORBA::ULong i = 0; i < factories->length (); i++)
      {
        // Get the first object reference to a factory.
        CORBA::Object_ptr const generic_FactoryObj_ptr = (*factories)[i];

        // Narrow it to a Quoter Factory.
        exception_message = "While narrowing.\n";
        CosLifeCycle::GenericFactory_var generic_Factory_var =
          CosLifeCycle::GenericFactory::_narrow (generic_FactoryObj_ptr);

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
                                                criteria);

          exception_message = "While narrowing object";
          quoter_var = Stock::Quoter::_narrow (quoterObject_var.in());

          if (CORBA::is_nil (quoter_var.in ()))
            {
              // If we had already our last chance, then give up.
              if (i + 1 == factories->length ())
                {
                  ACE_ERROR ((LM_ERROR,
                              "Quoter::copy: Last factory did not work.\n"
                              "No more factories are available. I give up.\n"));
                  throw CosLifeCycle::NoFactory (factoryKey);
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
        CosLifeCycle::LifeCycleObject::_duplicate (
          static_cast<CosLifeCycle::LifeCycleObject_ptr> (quoter_var.in ()));
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_ERROR ((LM_ERROR, "Quoter_i::copy - %C\n", exception_message));
      ex._tao_print_exception ("Exception");

      // Report a NoFactory exception back to the caller
      throw CosLifeCycle::NoFactory ();
    }


  return lifeCycleObject_ptr;
}


// Move this object using <there> and <the_criteria>

void
Quoter_i::move (CosLifeCycle::FactoryFinder_ptr /* there */,
                CosLifeCycle::Criteria const & /* the_criteria */)
{
  ACE_ERROR ((LM_ERROR,
              "Quoter_i::move: The Quoter object is not movable!"));

  throw CosLifeCycle::NotMovable();
}


// Removes the object.  Once we shut down the ORB we can call it a day.

void
Quoter_i::remove ()
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "I have been asked to shut down.\n"));

  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}
