// $Id$

#include "AccountManager_i.h"
#include "Account_i.h"
#include "tao/debug.h"
#include "tao/Utils/ORB_Manager.h"

ACE_RCSID(AccountManager, AccountManager_i, "$Id$")

// Constructor

AccountManager_i::AccountManager_i (void)
{
  // no-op
}

// Destructor

AccountManager_i::~AccountManager_i (void)
{
  for (MAP_MANAGER_TYPE::ITERATOR iter = this->hash_map_.begin ();
       iter != this->hash_map_.end ();
       ++iter)
    {
      delete (*iter).int_id_;
    }
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
  this->poa_ = poa;
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
  Account_i *result = 0;

  // If name is already in the map, <find> will assign <result> to the
  // appropriate value.

  if (hash_map_.find (name, result) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[SERVER] Process/Thread Id : (%P/%t) Opening Account (%s,%8.2f)\n",
                  name,
                  initial_balance));

      ACE_NEW_THROW_EX (result,
                        Account_i (name,
                                   initial_balance),
                        CORBA::NO_MEMORY ());

      // Enter the new Account in the hash map. If the <bind> fails
      // throw an UNKNOWN exception. <result> may be valid but since
      // it is not properly bound, it's behaviour may be off, so
      // delete it to be safe.

      if (hash_map_.bind (name, result) == -1)
        {
          delete result;
          throw CORBA::UNKNOWN ();
        }
    }
  else if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "[SERVER] Process/Thread Id : (%P/%t) Account already exists for %s\n",
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
                 "[SERVER] Process/Thread Id : (%P/%t) Closing Account for %s\n",
                 name.in ()));

      Account_i *account = 0;
      if (hash_map_.unbind (name.in (), account) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG((LM_DEBUG,
                       "Unable to close account\n"));
        }

      if (account)
        {
          PortableServer::POA_var poa = account->_default_POA ();

          PortableServer::ObjectId_var id = poa->servant_to_id (account);

          poa->deactivate_object (id.in ());

          delete account;
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
              "\n[SERVER] Process/Thread Id : (%P/%t) %s\n",
              "AccountManager_i is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

