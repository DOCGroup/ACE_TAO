// -*- C++ -*-
// $Id$

# if TAO_HAS_UIOP == 1

ACE_INLINE const ACE_UNIX_Addr &
TAO_UIOP_Endpoint::object_addr (void) const
{
  return this->object_addr_;
}

/*ACE_INLINE TAO_UIOP_Client_Connection_Handler *&
TAO_UIOP_Endpoint::hint (void)
{
  return this->hint_;
}
*/

ACE_INLINE const char *
TAO_UIOP_Endpoint::rendezvous_point (void) const
{
  return this->object_addr_.get_path_name ();
}


#endif  /* TAO_HAS_UIOP == 1 */
