// -*- C++ -*-
//
// $Id$


ACE_INLINE
TAO_Collocated_Object::TAO_Collocated_Object (TAO_Stub *p,
                                              CORBA::Boolean collocated,
                                              TAO_ServantBase *servant)
  : ACE_NESTED_CLASS (CORBA, Object) (p, collocated, servant)
{
}
