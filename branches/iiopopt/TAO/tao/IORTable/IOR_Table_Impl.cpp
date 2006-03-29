/**
 * @file IOR_Table_Impl.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 *
 */

#include "tao/IORTable/IOR_Table_Impl.h"
#include "ace/Guard_T.h"

ACE_RCSID (IORTable,
           IOR_Table_Impl,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IOR_Table_Impl::TAO_IOR_Table_Impl (void)
{
}

char *
TAO_IOR_Table_Impl::find (const char *object_key
                          ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        IORTable::NotFound
      ))
{
  // We don't want the lock held during locate, so make it go out
  // of scope before then.
  {
    ACE_CString key (object_key);
    ACE_CString ior;

    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);
    if (this->map_.find (key, ior) == 0)
      {
        return CORBA::string_dup (ior.c_str ());
      }
    if (CORBA::is_nil (this->locator_.in ()))
      ACE_THROW_RETURN (IORTable::NotFound (), 0);
  }

  return this->locator_->locate (object_key ACE_ENV_ARG_PARAMETER);
}

void
TAO_IOR_Table_Impl::bind (
        const char * object_key,
        const char * IOR
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        IORTable::AlreadyBound
      ))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  if (this->map_.bind (object_key, IOR) != 0)
    ACE_THROW (IORTable::AlreadyBound ());
}

void
TAO_IOR_Table_Impl::rebind (
        const char * object_key,
        const char * IOR
        ACE_ENV_ARG_DECL_NOT_USED
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->map_.rebind (object_key, IOR);
}

void
TAO_IOR_Table_Impl::unbind (
        const char * object_key
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        IORTable::NotFound
      ))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  if (this->map_.unbind (object_key) != 0)
    ACE_THROW (IORTable::NotFound ());
}

void
TAO_IOR_Table_Impl::set_locator (
        IORTable::Locator_ptr locator
        ACE_ENV_ARG_DECL_NOT_USED
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->locator_ = IORTable::Locator::_duplicate (locator);
}

TAO_END_VERSIONED_NAMESPACE_DECL
