/**
 * @file IOR_Table_Impl.cpp
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "tao/IORTable/IOR_Table_Impl.h"
#include "tao/PortableServer/ForwardRequestC.h"
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
TAO::Objref_Traits <TAO_IOR_Table_Impl>::nil ()
{
  return TAO_IOR_Table_Impl::_nil ();
}

::CORBA::Boolean
TAO::Objref_Traits <TAO_IOR_Table_Impl>::marshal (TAO_IOR_Table_Impl_ptr const,
                                                  TAO_OutputCDR &)
{
  return false;
}

char *
TAO_IOR_Table_Impl::find (char const *object_key)
{
  // We don't want the lock held during locate, so make it go out
  // of scope before then.
  {
    std::string const key (object_key);

    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);
    Map::const_iterator const iter = this->map_.find (key);
    if (iter != this->map_.end ())
      {
        return CORBA::string_dup (iter->second.c_str ());
      }
    if (CORBA::is_nil (this->locator_.in ()))
      throw IORTable::NotFound ();
  }

  return this->locator_->locate (object_key);
}

void
TAO_IOR_Table_Impl::bind (char const *object_key, char const *IOR)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  if (!this->map_.emplace (object_key, IOR).second)
    throw IORTable::AlreadyBound ();
}

void
TAO_IOR_Table_Impl::rebind (char const *object_key, char const *IOR)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->map_[object_key] = IOR;
}

void
TAO_IOR_Table_Impl::unbind (char const *object_key)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  if (this->map_.erase (object_key) == 0)
    throw IORTable::NotFound ();
}

void
TAO_IOR_Table_Impl::set_locator (IORTable::Locator_ptr locator)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->locator_ = IORTable::Locator::_duplicate (locator);
}

TAO_END_VERSIONED_NAMESPACE_DECL
