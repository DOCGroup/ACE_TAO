// -*- C++ -*-

#include "LB_Redirect_Table.h"

#include "tao/Object.h"
#include "tao/Exception.h"
#include "tao/Environment.h"
#include "tao/CORBA_String.h"
#include "tao/debug.h"

ACE_RCSID (LoadBalancing,
           LB_Redirect_Table,
           "$Id$")

// ****************************************************************

TAO_LB_Redirect_Table::TAO_LB_Redirect_Table (void)
  : table_ ()
{
}

TAO_LB_Redirect_Table::~TAO_LB_Redirect_Table (void)
{
  // Must explicitly call destroy() in the destructor since not all
  // applications will invoke ORB::shutdown() or ORB::destroy().
  this->destroy ();
}

void
TAO_LB_Redirect_Table::register_redirect (
  const char *type_id,
  CORBA::Object_ptr redirect_to,
  CORBA::Environment &ACE_TRY_ENV)
{
  // The RepositoryId should never be zero since it is not possible to
  // send a NULL string as an "in" argument.
  if (ACE_OS_String::strlen (id) == 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  if (CORBA::is_nil (obj))
    ACE_THROW (CORBA::BAD_PARAM ());

  int result = this->bind (type_id, redirect_to);

  if (result == 1)
    {
      if (TAO_debug_level > 1)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) LB_Redirect_Table::register_redirect:\n"
                    "  Could not register duplicate object <%s> with "
                    "the LB_Redirect_Table\n",
                    type_id));

      ACE_THROW (CORBA::INV_OBJREF ());
    }

  if (result == -1)
    {
      if (TAO_debug_level > 1)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) LB_Redirect_Table::register_redirect:\n"
                    "  Could not register redirect object <%s> with "
                    "the LB_Redirect_Table\n",
                    type_id));

      ACE_THROW (CORBA::INTERNAL ());
    }
}

void
TAO_LB_Redirect_Table::find_redirect (
  PortableInterceptor::ServerRequestInfo_ptr ri,
  CORBA::Environment &ACE_TRY_ENV)
{
  // This could become very slow if there are many type of targets
  // being redirected since we're doing a linear search using string
  // comparisons.
  //
  // We do things this way to avoid the memory allocation that would
  // occur if we were to just use the
  // ServerRequestInfo::target_most_derived_interface() method.  At
  // some point, there is a threshold reached where the cost of the
  // iteration through the redirect table is more expensive than the
  // allocation incurred by calling
  // ServerRequestInfo::target_most_derived_interface().  That
  // threshold is approached as the number of redirects in the table
  // becomes "large."  For now, we optimize for the common case and
  // assume that only a few types of targets (i.e. objects/servants
  // with different RepositoryIds) exist at the current location.
  //
  // Note that if no redirects are registered, then this method
  // basically becomes a no-op.

  for (Table::iterator i = this->table_.begin ();
       i != this->table_.end ();
       ++i)
    {
      // Compare the target's RepositoryId with the registered
      // redirects.
      CORBA::Boolean matched =
        ri->target_is_a ((*i).ext_id_, ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (matched)
        ACE_THROW (PortableInterceptor::ForwardRequest (
                     (*i).int_id_,
                     0 /* non-permanent forward */));
    }
}

int
TAO_LB_Redirect_Table::remove_redirect (const char *type_id,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  Table::ENTRY *entry = 0;

  int result = this->table_.find (type_id, entry);

  if (result == 0)
    {
      // Deallocate the external ID and obtain the ORB core pointer
      // before unbinding the entry since the entry is deallocated
      // during the call to unbind().
      CORBA::string_free (ACE_const_cast (char *, entry->ext_id_));
      CORBA::Object_ptr redirect = entry->int_id_;

      result = this->table_.unbind (entry);

      if (result != 0)
        return result;

      CORBA::release (redirect);
    }

  return result;
}

void
TAO_LB_Redirect_Table::destroy (void)
{
  for (Table::iterator i = this->table_.begin ();
       i != this->table_.end ();
       ++i)
    {
      // Deallocate the id.
      CORBA::string_free (ACE_const_cast (char *, (*i).ext_id_));

      // Release the Object.
      CORBA::release ((*i).int_id_);
    }

  this->table_.unbind_all ();
}

int
TAO_LB_Redirect_Table::bind (const char *id,
                             CORBA::Object_ptr obj)
{
  // Make sure that the supplied Object reference is valid,
  // i.e. not nil.
  if (id == 0 || CORBA::is_nil (obj))
    {
      errno = EINVAL;
      return -1;
    };

  CORBA::String_var type_id = CORBA::string_dup (id);
  CORBA::Object_var redirect = CORBA::Object::_duplicate (obj);

  int result = this->table_.bind (type_id.in (),
                                  redirect.in ());

  if (result == 0)
    {
      // Transfer ownership to the Object Table.
      (void) type_id._retn ();
      (void) redirect._retn ();
    }

  return result;
}
