//$Id$

ACE_INLINE
TAO_Transport_Descriptor_Interface::
    TAO_Transport_Descriptor_Interface (TAO_Endpoint *endpoint,
                                        CORBA::Boolean flag)
      : endpoint_ (endpoint),
        bidir_flag_ (0),
        endpoint_from_heap_ (flag)
{
}

ACE_INLINE
TAO_Transport_Descriptor_Interface::
    TAO_Transport_Descriptor_Interface (void)
      : endpoint_ (0),
        bidir_flag_ (0),
        endpoint_from_heap_ (0)
{
}


ACE_INLINE TAO_Endpoint *
TAO_Transport_Descriptor_Interface::endpoint (void)
{
  return this->endpoint_;
}


ACE_INLINE void
TAO_Transport_Descriptor_Interface::set_bidir_flag (CORBA::Boolean flag)
{
  this->bidir_flag_ = flag;
}
