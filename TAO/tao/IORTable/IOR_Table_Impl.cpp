/**
 * @file IOR_Table_Impl.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 *
 */
#include "IOR_Table_Impl.h"

ACE_RCSID(IORTable, IOR_Table_Impl, "$Id$")

TAO_IOR_Table_Impl::TAO_IOR_Table_Impl (void)
{
}

char *
TAO_IOR_Table_Impl::find (const char *object_key,
                          CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        IORTable::NotFound
      ))
{
  ACE_CString key (object_key);
  ACE_CString ior;

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);
  if (this->map_.find (key, ior) == 0)
    {
      return CORBA::string_dup (ior.c_str ());
    }
  if (CORBA::is_nil (this->locator_.in ()))
    ACE_THROW_RETURN (IORTable::NotFound (), 0);

  return this->locator_->locate (object_key, ACE_TRY_ENV);
}

void
TAO_IOR_Table_Impl::bind (
        const char * object_key,
        const char * IOR,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        IORTable::AlreadyBound
      ))
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);
  if (this->map_.bind (object_key, IOR) != 0)
    ACE_THROW (IORTable::AlreadyBound ());
}

void
TAO_IOR_Table_Impl::rebind (
        const char * object_key,
        const char * IOR,
        CORBA::Environment &
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);
  this->map_.rebind (object_key, IOR);
}

void
TAO_IOR_Table_Impl::unbind (
        const char * object_key,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        IORTable::NotFound
      ))
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);
  if (this->map_.unbind (object_key) != 0)
    ACE_THROW (IORTable::NotFound ());
}

void
TAO_IOR_Table_Impl::set_locator (
        IORTable::Locator_ptr locator,
        CORBA::Environment &
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);
  this->locator_ = IORTable::Locator::_duplicate (locator);
}
