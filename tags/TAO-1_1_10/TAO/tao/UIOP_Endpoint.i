// -*- C++ -*-
// $Id$

# if TAO_HAS_UIOP == 1

ACE_INLINE const ACE_UNIX_Addr &
TAO_UIOP_Endpoint::object_addr (void) const
{
  return this->object_addr_;
}

ACE_INLINE TAO_UIOP_Client_Connection_Handler *&
TAO_UIOP_Endpoint::hint (void)
{
  return this->hint_;
}

ACE_INLINE const char *
TAO_UIOP_Endpoint::rendezvous_point (void) const
{
  return this->object_addr_.get_path_name ();
}

ACE_INLINE CORBA::ULong
TAO_UIOP_Endpoint::hash (void)
{
  return ACE::hash_pjw (this->rendezvous_point ());
}

ACE_INLINE CORBA::Boolean
TAO_UIOP_Endpoint::is_equivalent (const TAO_UIOP_Endpoint *other_endpoint)
{
  return
    ACE_OS::strcmp (this->rendezvous_point (),
                    other_endpoint->rendezvous_point ()) == 0;
}

#endif  /* TAO_HAS_UIOP == 1 */
