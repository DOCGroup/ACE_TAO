// CORBA_Ref.cpp
// $Id$

#if !defined (ACE_CORBA_REF_C)
#define ACE_CORBA_REF_C

#define ACE_BUILD_DLL
#include "ace/CORBA_Ref.h"

#if !defined (__ACE_INLINE__)
#include "ace/CORBA_Ref.i"
#endif /* __ACE_INLINE__ */

template<class CORBA_REF>
ACE_CORBA_Ref<CORBA_REF>::ACE_CORBA_Ref (void)
  : ref_ (0)
{
  ACE_TRACE ("ACE_CORBA_Ref<CORBA_REF>::ACE_CORBA_Ref");
}

template<class CORBA_REF>
ACE_CORBA_Ref<CORBA_REF>::ACE_CORBA_Ref (CORBA_REF *ref)
{
  ACE_TRACE ("ACE_CORBA_Ref<CORBA_REF>::ACE_CORBA_Ref");
  if (ref != 0)
    ref_ = ref->_duplicate ();
  else
    ref_ = 0;
}

template<class CORBA_REF> CORBA_REF *
ACE_CORBA_Ref<CORBA_REF>::operator= (CORBA_REF *ref)
{
  ACE_TRACE ("ACE_CORBA_Ref<CORBA_REF>::operator=");
  if (ref_ != 0)
    ref_->_release ();
  if (ref == 0)
    {
      ref_ = 0;
      return 0;
    }
  else
    return ref_ = ref->_duplicate ();
}

template<class CORBA_REF>
ACE_CORBA_Ref<CORBA_REF>::operator CORBA_REF * (void) const
{
  ACE_TRACE ("ACE_CORBA_Ref<CORBA_REF>::operator CORBA_REF *");
  ACE_ASSERT (ref_ != 0);
  return ref_; 
}

template<class CORBA_REF> CORBA_REF *
ACE_CORBA_Ref<CORBA_REF>::operator-> (void) const
{
  ACE_TRACE ("ACE_CORBA_Ref<CORBA_REF>::operator->");
  ACE_ASSERT (ref_ != 0);
  return ref_;
}

template<class CORBA_REF> int
ACE_CORBA_Ref<CORBA_REF>::operator== (CORBA_REF *rhs) const
{
  ACE_TRACE ("ACE_CORBA_Ref<CORBA_REF>::operator==");
  // pointer comparison.
  return ref_ == rhs;
}

template<class CORBA_REF> int
ACE_CORBA_Ref<CORBA_REF>::operator!= (CORBA_REF *rhs) const
{
  ACE_TRACE ("ACE_CORBA_Ref<CORBA_REF>::operator!=");
  // pointer comparison.
  return ref_ != rhs;
}

template<class CORBA_REF>
ACE_CORBA_Ref<CORBA_REF>::~ACE_CORBA_Ref ()
{
  ACE_TRACE ("ACE_CORBA_Ref<CORBA_REF>::~ACE_CORBA_Ref");
  if (ref_ != 0)
    ref_->_release ();
}

#endif /* ACE_CORBA_REF_C */
