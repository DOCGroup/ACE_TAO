// $Id$

#include "AccountManager_i.h"
#include "Account_i.h"
#include "tao/debug.h"

ACE_RCSID(AccountManager, AccountManager_i, "$Id$")

// Constructor

AccountManager_i::AccountManager_i (void)
{
  // no-op
}

AccountManager_i::AccountManager_i (AccountManager_i &foo)
  : POA_Bank::AccountManager (foo)
{
  // old g++ fooler.
}

// Destructor

AccountManager_i::~AccountManager_i (void)
{
  // no-op
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
                        CORBA::Float initial_balance
                        TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
      ACE_CHECK_RETURN (Bank::Account::_nil ());

      // Enter the new Account in the hash map. If the <bind> fails
      // throw an UNKNOWN exception. <result> may be valid but since
      // it is not properly bound, it's behaviour may be off, so
      // delete it to be safe.

      if (hash_map_.bind (name, result) == -1)
        {
          delete result;
          ACE_THROW_RETURN (CORBA::UNKNOWN (),
                            Bank::Account::_nil ());
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
AccountManager_i::close (Bank::Account_ptr account
                         TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      CORBA::String_var name =
        CORBA::string_dup (account->name (TAO_ENV_SINGLE_ARG_PARAMETER));

      ACE_TRY_CHECK;

      if (hash_map_.unbind (name.in ()) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG((LM_DEBUG,
                       "Unable to close account\n"));
        }
      else if (TAO_debug_level > 0)
        ACE_DEBUG((LM_DEBUG,
                   "[SERVER] Process/Thread Id : (%P/%t) Closing Account for %s\n",
                   (char *) name));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unable to close Account\n");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
AccountManager_i::shutdown (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "\n[SERVER] Process/Thread Id : (%P/%t) %s\n",
              "AccountManager_i is shutting down"));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Manager<ACE_CString,Account_i *,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, Account_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString,Account_i *,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, Account_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_CString, Account_i *>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, Account_i *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, Account_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, Account_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,Account_i *,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, Account_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,Account_i *,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, Account_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, Account_i *>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, Account_i *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, Account_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, Account_i *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
