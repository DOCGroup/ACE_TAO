//$Id$

ACE_INLINE
TAO_Connection_Descriptor_Interface::
    TAO_Connection_Descriptor_Interface (TAO_Endpoint *endpoint,
                                         CORBA::Boolean flag)
      : endpoint_ (endpoint),
        endpoint_from_heap_ (flag)
{
}

ACE_INLINE
TAO_Connection_Descriptor_Interface::
    TAO_Connection_Descriptor_Interface (void)
      : endpoint_ (0),
        endpoint_from_heap_ (0)
{
}


ACE_INLINE TAO_Endpoint *
TAO_Connection_Descriptor_Interface::endpoint (void)
{
  return this->endpoint_;
}
