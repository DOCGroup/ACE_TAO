// -*- C++ -*-
// $Id$

ACE_INLINE
TAO_Private_Connection_Descriptor::
TAO_Private_Connection_Descriptor (TAO_Endpoint *endpoint,
                                   long object_id,
                                   CORBA::Boolean flag)
  : TAO_Connection_Descriptor_Interface (endpoint, flag),
    object_id_ (object_id)
{
}

