// $Id$

#include "AccountManager_i.h"
#include "Account_i.h"
#include "tao/debug.h"
#include "tao/Utils/ORB_Manager.h"

// Constructor
AccountManager_i::AccountManager_i (void) :
  orb_manager_ (0)
{
  // no-op
}

// Destructor
AccountManager_i::~AccountManager_i (void)
{
}

// Set the ORB pointer
void
AccountManager_i::orb (CORBA::ORB_ptr o)
{
  this->orb_ = CORBA::ORB::_duplicate (o);
}

void
AccountManager_i::poa (PortableServer::POA_ptr poa)
{
  this->poa_ = PortableServer::POA::_duplicate (poa);
}

void
AccountManager_i::set_orb_manager (TAO_ORB_Manager *orb_manager)
{
  this->orb_manager_ = orb_manager;
}

// Open an account for the given name.

Bank::Account_ptr
AccountManager_i::open (const char *name,
                        CORBA::Float initial_balance)
{
  // If name is already in the map, <find> will assign <result> to the
  // appropriate value.
  Account_i_var result;
  if (hash_map_.find (name, result) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("[SERVER] Process/Thread Id : (%P/%t) Opening Account (%C,%8.2f)\n"),
                  name,
                  initial_balance));

      Account_i *tmp = 0;
      ACE_NEW_THROW_EX (tmp,
                        Account_i (name,
                                   initial_balance),
                        CORBA::NO_MEMORY ());
      result = tmp;

      // Enter the new Account in the hash map. If the <bind> fails
      // throw an UNKNOWN exception. <result> may be valid but since
      // it is not properly bound, it's behaviour may be off, so
      // delete it to be safe.

      if (hash_map_.bind (name, result) == -1)
        {
          throw CORBA::UNKNOWN ();
        }
    }

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("[SERVER] Process/Thread Id : (%P/%t) Account already exists for %C\n"),
                name));
  // Generate an IOR for the result object and register it with the
  // POA.  In case the object already exists then the previously
  // generated IOR is returned.

  return result->_this ();
}

// Shutdown.
void
AccountManager_i::close (Bank::Account_ptr account)
{
  try
    {
      CORBA::String_var name = account->name ();

      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT ("[SERVER] Process/Thread Id : (%P/%t) Closing Account for %C\n"),
                 name.in ()));

      Account_i_var account;
      if (hash_map_.unbind (name.in (), account) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG((LM_DEBUG,
                       ACE_TEXT ("Unable to close account\n")));
        }

      if (account.is_nil ())
        {
          PortableServer::POA_var poa = account->_default_POA ();

          PortableServer::ObjectId_var id = poa->servant_to_id (account.in ());

          poa->deactivate_object (id.in ());
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unable to close Account\n");
    }
}

void
AccountManager_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n[SERVER] Process/Thread Id : (%P/%t) AccountManager_i is shutting down\n")));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}
