// -*- C++ -*-
//
// $Id$

# if TAO_HAS_UIOP == 1

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE const ACE_UNIX_Addr &
TAO_UIOP_Endpoint::object_addr (void) const
{
  return this->object_addr_;
}

ACE_INLINE const char *
TAO_UIOP_Endpoint::rendezvous_point (void) const
{
  return this->object_addr_.get_path_name ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_UIOP == 1 */
