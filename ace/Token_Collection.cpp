// Token_Collection.cpp
// $Id$


#define ACE_BUILD_DLL
#include "ace/Token_Collection.h"
#include "ace/Log_Msg.h"

#if !defined (__INLINE__)
#include "ace/Token_Collection.i"
#endif /* __INLINE__ */

ACE_Token_Collection::ACE_Token_Collection (int debug,
					    const char *name)
: debug_ (debug)
{
  ACE_TRACE ("ACE_Token_Collection::ACE_Token_Collection");

  if (name == 0)
    name = "no name";

  int n = ACE_OS::strlen (name) + 1;

  if (n >= ACE_MAXTOKENNAMELEN)
    n = ACE_MAXTOKENNAMELEN - 1;

  ACE_OS::strncpy (this->name_, (char *) name, n);
  this->name_[ACE_MAXTOKENNAMELEN - 1] = '\0';
}

int
ACE_Token_Collection::insert (ACE_Token_Proxy &new_token)
{
  ACE_TRACE ("ACE_Token_Collection::insert");

  TOKEN_NAME name (new_token.name ());

  // Check if the new_proxy is already in the list.
  if (collection_.find (name) == 1)
    // One already exists, so fail.
    return -1;

  // Clone the new token.
  ACE_Token_Proxy *temp = new_token.clone ();

  if (collection_.bind (name, temp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "bind failed\n"), -1);
  return 0;
}

int
ACE_Token_Collection::extract (const char *token_name, ACE_Token_Proxy *&proxy)
{
  ACE_TRACE ("ACE_Token_Collection::extract");
  TOKEN_NAME name (token_name);
  return collection_.unbind (token_name, proxy);
}

ACE_Token_Proxy *
ACE_Token_Collection::is_member (const char *token_name)
{
  ACE_TRACE ("ACE_Token_Collection::is_member");
  TOKEN_NAME name (token_name);
  ACE_Token_Proxy *temp;
  // Get the token from the collection.
  return collection_.find (name, temp) == -1 ? 0 : temp;
}

int
ACE_Token_Collection::is_member (const ACE_Token_Proxy &token)
{
  ACE_TRACE ("ACE_Token_Collection::is_member");
  TOKEN_NAME token_name (token.name ());
  return collection_.find (token_name) == 0;
}

int 
ACE_Token_Collection::acquire (int notify,
			       void (*sleep_hook)(void *),
			       ACE_Synch_Options &options)
{
  ACE_TRACE ("ACE_Token_Collection::acquire");

  COLLECTION_ITERATOR iterator (collection_);

  for (COLLECTION_ENTRY *temp = 0;
       iterator.next (temp) != 0;
       iterator.advance ())
    {
      if (debug_)
	ACE_DEBUG ((LM_DEBUG, "collection acquiring %s\n",
		    temp->int_id_->name ()));
      if (temp->int_id_->acquire (notify, sleep_hook, options) == -1)
	{
	  int error = errno;
	  this->release ();
	  errno = error;
	  ACE_RETURN (-1);
	}
    }

  return 0;
}

int 
ACE_Token_Collection::acquire (const char *token_name,
			       int notify,
			       void (*sleep_hook)(void *),
			       ACE_Synch_Options &options)
{
  ACE_TRACE ("ACE_Token_Collection::acquire");
  TOKEN_NAME name (token_name);
  ACE_Token_Proxy *temp;
  // Get the token from the collection.
  int result = collection_.find (name, temp);
  // did we find it?
  if (result == -1)
    return result;
  // perform the operation
  return temp->acquire (notify, sleep_hook, options);
}


int 
ACE_Token_Collection::tryacquire (const char *token_name,
				  void (*sleep_hook)(void *))
{
  ACE_TRACE ("ACE_Token_Collection::tryacquire");
  TOKEN_NAME name (token_name);
  ACE_Token_Proxy *temp;
  // Get the token from the collection.
  int result = collection_.find (name, temp);
  // did we find it?
  if (result == -1)
    return result;

  // perform the operation
  return temp->tryacquire (sleep_hook);
}

int 
ACE_Token_Collection::tryacquire (void (*sleep_hook)(void *))
{
  ACE_TRACE ("ACE_Token_Collection::tryacquire");

  COLLECTION_ITERATOR iterator (collection_);

  for (COLLECTION_ENTRY *temp = 0;
       iterator.next (temp) != 0;
       iterator.advance ())
    {
      if (debug_)
	ACE_DEBUG ((LM_DEBUG, "collection acquiring %s\n",
		    temp->int_id_->name ()));
      // We will fail if _any_ token is not free.
      if (temp->int_id_->tryacquire (sleep_hook) == -1)
	return -1;
    }

  return 0;
}

int 
ACE_Token_Collection::renew (int requeue_position,
			     ACE_Synch_Options &options)
{
  ACE_TRACE ("ACE_Token_Collection::renew");

  COLLECTION_ITERATOR iterator (collection_);

  for (COLLECTION_ENTRY *temp = 0;
       iterator.next (temp) != 0;
       iterator.advance ())
    {
      if (debug_)
	ACE_DEBUG ((LM_DEBUG, "collection renewing %s\n",
		    temp->int_id_->name ()));
      if (temp->int_id_->renew (requeue_position, options) == -1)
	return -1;
    }

  return 0;
}

int 
ACE_Token_Collection::renew (const char *token_name,
			     int requeue_position,
			     ACE_Synch_Options &options)
{
  ACE_TRACE ("ACE_Token_Collection::renew");
  TOKEN_NAME name (token_name);
  ACE_Token_Proxy *temp;

  // Get the token from the collection.
  int result = collection_.find (name, temp);

  // Did we find it?
  if (result == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p %s\n",
		       "not in collection ",
		       token_name), -1);
  // perform the operation
  return temp->renew (requeue_position, options);
}

int 
ACE_Token_Collection::release (ACE_Synch_Options &)

{
  ACE_TRACE ("ACE_Token_Collection::release");
  COLLECTION_ITERATOR iterator (collection_);

  for (COLLECTION_ENTRY *temp = 0;
       iterator.next (temp) != 0;
       iterator.advance ())
    {
      if (debug_)
	ACE_DEBUG ((LM_DEBUG, "collection releasing %s\n",
		    temp->int_id_->name ()));
      temp->int_id_->release ();
    }

  return 0;
}

int 
ACE_Token_Collection::release (const char *token_name,
			       ACE_Synch_Options &options)
{
  ACE_TRACE ("ACE_Token_Collection::release");
  TOKEN_NAME name (token_name);
  ACE_Token_Proxy *temp;
  // get the token from the collection
  int result = collection_.find (name, temp);
  // did we find it?
  if (result != 0)
    return result;
  // perform the operation
  return temp->release (options);
}

ACE_Token_Collection::~ACE_Token_Collection (void)
{
  ACE_TRACE ("ACE_Token_Collection::~ACE_Token_Collection");
  COLLECTION_ITERATOR iterator (collection_);

  for (COLLECTION_ENTRY *temp = 0;
       iterator.next (temp) != 0;
       iterator.advance ())
    {
      delete temp->int_id_;
      // The ext_id_'s delete themselves when the array of
      // COLLECTION_ENTRYs goes away.
    }
}


// This method doesn't mean anything for a collection.
ACE_Token_Proxy *
ACE_Token_Collection::clone (void) const
{
  ACE_TRACE ("ACE_Token_Collection::clone");
  return (ACE_Token_Proxy *) 0;
}

// This method doesn't mean anything for a collection.
ACE_Tokens *
ACE_Token_Collection::create_token (const char *)
{
  ACE_TRACE ("ACE_Token_Collection::create_token");
  return (ACE_Tokens *) 0;
}

void
ACE_Token_Collection::dump (void) const
{
  ACE_TRACE ("ACE_Token_Collection::dump");
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "ACE_Token_Collection::dump:\n"
			" debug_ = %d\n", debug_));
  ACE_DEBUG ((LM_DEBUG, "collection_\n"));
  collection_.dump ();
  ACE_DEBUG ((LM_DEBUG, "base:\n"));
  ACE_Token_Proxy::dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));    
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Map_Manager<ACE_Token_Name, ACE_Token_Proxy *, ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_Token_Name, ACE_Token_Proxy *, ACE_Null_Mutex>;
template class ACE_Map_Entry<ACE_Token_Name, ACE_Token_Proxy *>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
