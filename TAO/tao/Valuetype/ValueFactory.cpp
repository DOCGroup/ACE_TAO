#include "ValueFactory.h"

#if !defined (__ACE_INLINE__)
# include "ValueFactory.inl"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (tao,
           ValueFactory,
           "$Id$")

// Static operations in namespace CORBA.

void
CORBA::add_ref (CORBA::ValueFactoryBase *val)
{
  if (val)
    {
      val->_add_ref ();
    }
}

void
CORBA::remove_ref (CORBA::ValueFactoryBase *val)
{
  if (val)
    {
      val->_remove_ref ();
    }
}

// ===========================================================

CORBA::ValueFactoryBase::ValueFactoryBase (void)
  : _tao_reference_count_ (1)
{
}

CORBA::ValueFactoryBase::~ValueFactoryBase (void)
{
}

void
CORBA::ValueFactoryBase::_add_ref (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, 
             guard, 
             this->_tao_reference_count_lock_);

  ++this->_tao_reference_count_;
}

void
CORBA::ValueFactoryBase::_remove_ref (void)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, 
               guard, 
               this->_tao_reference_count_lock_);

    --this->_tao_reference_count_;

    if (this->_tao_reference_count_ != 0)
      {
        return;
      }
  }

  delete this;
}

// No-op. This should never be called, but it can't be pure virtual.
CORBA::AbstractBase *
CORBA::ValueFactoryBase::create_for_unmarshal_abstract (void)
{
  return 0;
}

