// -*- C++ -*-
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Configurable_Refcount::TAO_Configurable_Refcount (
                      TAO_Configurable_Refcount::Lock_Type type,
                      unsigned long value)
 : type_ (type),
   null_refcount_ (value),
   mutex_refcount_ (value)
{
}

ACE_INLINE unsigned long
TAO_Configurable_Refcount::increment (void)
{
  switch(this->type_)
    {
    case TAO_NULL_LOCK:
      return this->null_refcount_++;
    case TAO_THREAD_LOCK:
    default:
      return this->mutex_refcount_++;
    }
}

ACE_INLINE unsigned long
TAO_Configurable_Refcount::decrement (void)
{
  switch(this->type_)
    {
    case TAO_NULL_LOCK:
      return --this->null_refcount_;
    case TAO_THREAD_LOCK:
    default:
      return --this->mutex_refcount_;
    }
}

ACE_INLINE unsigned long
TAO_Configurable_Refcount::value (void) const
{
  switch(this->type_)
    {
    case TAO_NULL_LOCK:
      return this->null_refcount_.value ();
    case TAO_THREAD_LOCK:
    default:
      return this->mutex_refcount_.value ();
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
