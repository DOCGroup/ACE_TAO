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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IOR_Table_Impl_ptr
TAO::Objref_Traits <TAO_IOR_Table_Impl>::duplicate (TAO_IOR_Table_Impl_ptr p)
{
  if (!::CORBA::is_nil (p))
    {
      p->_add_ref ();
    }

  return p;
}

void
TAO::Objref_Traits <TAO_IOR_Table_Impl>::release (TAO_IOR_Table_Impl_ptr p)
{
  ::CORBA::release (p);
}

TAO_IOR_Table_Impl_ptr
TAO::Objref_Traits <TAO_IOR_Table_Impl>::nil (void)
{
  return TAO_IOR_Table_Impl::_nil ();
}

::CORBA::Boolean
TAO::Objref_Traits <TAO_IOR_Table_Impl>::marshal (const TAO_IOR_Table_Impl_ptr,
                                                  TAO_OutputCDR &)
{
  return false;
}

TAO_IOR_Table_Impl::TAO_IOR_Table_Impl (void)
{
}

char *
TAO_IOR_Table_Impl::find (const char *object_key)
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
      throw IORTable::NotFound ();
  }

  return this->locator_->locate (object_key);
}

void
TAO_IOR_Table_Impl::bind (const char * object_key, const char * IOR)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  if (this->map_.bind (object_key, IOR) != 0)
    throw IORTable::AlreadyBound ();
}

void
TAO_IOR_Table_Impl::rebind (const char * object_key, const char * IOR)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->map_.rebind (object_key, IOR);
}

void
TAO_IOR_Table_Impl::unbind (const char * object_key)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  if (this->map_.unbind (object_key) != 0)
    throw IORTable::NotFound ();
}

void
TAO_IOR_Table_Impl::set_locator (IORTable::Locator_ptr locator)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->locator_ = IORTable::Locator::_duplicate (locator);
}

TAO_END_VERSIONED_NAMESPACE_DECL
