// $Id$

ACE_INLINE u_long
TAO_Servant_Hash::operator () (PortableServer::Servant servant) const
{
  return u_long (servant);
}

ACE_INLINE TAO_Stub *
TAO_Local_ServantBase::_create_stub (TAO_ENV_SINGLE_ARG_DECL)
{
#if 0
  PortableServer::ObjectId_var invalid_oid =
    PortableServer::string_to_ObjectId ("invalid");

  TAO_ObjectKey tmp_key (invalid_oid->length (),
                         invalid_oid->length (),
                         invalid_oid->get_buffer (),
                         0);

  // It is ok to use TAO_ORB_Core_instance here since the locality
  // constrained servant does not really register with a POA or get
  // exported remotely.
  //
  // The correct thing to do is to probably use ORB of the default
  // POA. The unfortunate part is that calling default_POA() requires
  // the creation of a local stub, hence causing a infinite loop.
  return TAO_ORB_Core_instance ()->orb ()->create_stub_object (tmp_key,
                                                               this->_interface_repository_id ()
                                                               TAO_ENV_ARG_PARAMETER);
#else
# if !defined (ACE_HAS_EXCEPTIONS)
  ACE_UNUSED_ARG (TAO_ENV_SINGLE_ARG_PARAMETER);
#endif
  return 0;
#endif
}
