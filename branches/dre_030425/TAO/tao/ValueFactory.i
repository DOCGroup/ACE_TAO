// -*- C++ -*-
//
// $Id$

ACE_INLINE
void
CORBA::add_ref (CORBA::ValueFactoryBase * p)
{
  if (p != 0)
    {
      p->_add_ref ();
    }
}

ACE_INLINE
void
CORBA::remove_ref (CORBA::ValueFactoryBase * p)
{
  if (p != 0)
    {
      p->_remove_ref ();
    }
}

// ================================================================

ACE_INLINE
CORBA::ValueFactoryBase::ValueFactoryBase (void)
  : _tao_reference_count_ (1)
{
}

ACE_INLINE 
void
CORBA::ValueFactoryBase::_add_ref (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, 
             guard, 
             this->_tao_reference_count_lock_);
  ++_tao_reference_count_;
}

ACE_INLINE 
void
CORBA::ValueFactoryBase::_remove_ref (void)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, 
               guard, 
               this->_tao_reference_count_lock_);

    -- this->_tao_reference_count_;

    if (this->_tao_reference_count_ != 0)
      {
        return;
      }
  }

  delete this;
}

