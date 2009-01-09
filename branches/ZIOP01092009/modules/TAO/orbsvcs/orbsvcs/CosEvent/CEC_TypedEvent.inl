// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_CEC_TypedEvent::TAO_CEC_TypedEvent (void)
{
}

ACE_INLINE
TAO_CEC_TypedEvent::TAO_CEC_TypedEvent (CORBA::NVList_ptr list,
                                        const char * operation)
  :  list_ (list),
     operation_ (operation)
{
}

ACE_INLINE
TAO_CEC_TypedEvent&
TAO_CEC_TypedEvent::operator= (const TAO_CEC_TypedEvent& other)
{
  this->list_ = CORBA::NVList::_duplicate (other.list_);
  this->operation_ = CORBA::string_dup (other.operation_);

  return *this;
}

TAO_END_VERSIONED_NAMESPACE_DECL
