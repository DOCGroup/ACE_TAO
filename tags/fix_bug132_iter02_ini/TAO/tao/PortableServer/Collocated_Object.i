// -*- C++ -*-
//
// $Id$


ACE_INLINE
TAO_Collocated_Object::TAO_Collocated_Object (TAO_Stub *p,
                                              CORBA::Boolean collocated,
                                              TAO_ServantBase *servant)
  : CORBA_Object (p, collocated, servant)
{
}
