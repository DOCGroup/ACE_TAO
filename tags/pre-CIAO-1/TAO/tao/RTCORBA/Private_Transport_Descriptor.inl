// -*- C++ -*-
// $Id$

ACE_INLINE
TAO_Private_Transport_Descriptor::
TAO_Private_Transport_Descriptor (TAO_Endpoint *endpoint,
                                  long object_id,
                                  CORBA::Boolean flag)
  : TAO_Transport_Descriptor_Interface (endpoint, flag),
    object_id_ (object_id)
{
}

