// $Id$

#include "Daemon_Impl.h"

#if !defined (__ACE_INLINE__)
# include "Daemon_Impl.inl"
#endif /* __ACE_INLINE__ */

CIAO::Daemon_Impl::~Daemon_Impl ()
{
  for (Iterator i = this->table_.begin ();
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

PortableServer::POA_ptr
CIAO::Daemon_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

char *
CIAO::Daemon_Impl::name (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->name_.in ());
}

CORBA::Object_ptr
CIAO::Daemon_Impl::get_service (const char * svc_name
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CIAO::NotFound))
{
  CORBA::Object_ptr found = CORBA::Object::_nil ();

  this->table_.find (svc_name, found);

  if (CORBA::is_nil (found))
    ACE_THROW_RETURN (CIAO::NotFound (), 0);

  return CORBA::Object::_duplicate (found);
}

void
CIAO::Daemon_Impl::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_SINGLE_ARG_PARAMETER);
}

int
CIAO::Daemon_Impl::bind (const char *id,
                         CORBA::Object_ptr obj)
{
  // Make sure that the supplied Object reference is valid,
  // i.e. not nil.
  if (id == 0 || CORBA::is_nil (obj))
    {
      errno = EINVAL;
      return -1;
    };

  CORBA::String_var name = CORBA::string_dup (id);
  CORBA::Object_var object = CORBA::Object::_duplicate (obj);

  int result = this->table_.bind (name.in (),
                                  object.in ());

  if (result == 0)
    {
      // Transfer ownership to the Object Table.
      (void) name._retn ();
      (void) object._retn ();
    }

  return result;
}

int
CIAO::Daemon_Impl::unbind (const char *id)
{
  Table::ENTRY *entry = 0;

  int result = this->table_.find (id, entry);

  if (result == 0)
    {
      // Deallocate the external ID and obtain the ORB core pointer
      // before unbinding the entry since the entry is deallocated
      // during the call to unbind().
      CORBA::string_free (ACE_const_cast (char *, entry->ext_id_));
      CORBA::Object_ptr obj = entry->int_id_;

      result = this->table_.unbind (entry);

      if (result != 0)
        return result;

      CORBA::release (obj);
    }

  return result;
}
