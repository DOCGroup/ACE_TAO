//
// $Id$
//

ACE_INLINE int
ECM_Federation::sender_local_addr (ACE_INET_Addr& addr)
{
  return this->sender_.get_local_addr (addr);
}


ACE_INLINE const char*
ECM_Federation::name (void) const
{
  return this->name_;
}

ACE_INLINE CORBA::UShort
ECM_Federation::mcast_port (void) const
{
  return this->mcast_port_;
}

ACE_INLINE int
ECM_Federation::supplier_types (void) const
{
  return this->supplier_types_;
}

ACE_INLINE const char*
ECM_Federation::supplier_name (CORBA::ULong i) const
{
  if (i < ACE_static_cast (ACE_CAST_CONST CORBA::ULong, this->supplier_types_))
    return this->supplier_names_[i];
  return 0;
}

ACE_INLINE CORBA::ULong
ECM_Federation::supplier_ipaddr (CORBA::ULong i) const
{
  if (i < ACE_static_cast (ACE_CAST_CONST CORBA::ULong, this->supplier_types_))
    return this->supplier_ipaddr_[i];
  return 0;
}

ACE_INLINE int
ECM_Federation::consumer_types (void) const
{
  return this->consumer_types_;
}

ACE_INLINE const char*
ECM_Federation::consumer_name (CORBA::ULong i) const
{
  if (i < ACE_static_cast (ACE_CAST_CONST CORBA::ULong, this->consumer_types_))
    return this->consumer_names_[i];
  return 0;
}

ACE_INLINE CORBA::ULong
ECM_Federation::consumer_ipaddr (CORBA::ULong i) const
{
  if (i < ACE_static_cast (ACE_CAST_CONST CORBA::ULong, this->consumer_types_))
    return this->consumer_ipaddr_[i];
  return 0;
}

ACE_INLINE const ECM_Federation*
ECM_Local_Federation::federation (void) const
{
  return this->federation_;
}

ACE_INLINE const char*
ECM_Local_Federation::name (void) const
{
  return this->federation_->name ();
}

ACE_INLINE CORBA::UShort
ECM_Local_Federation::mcast_port (void) const
{
  return this->federation_->mcast_port ();
}

ACE_INLINE int
ECM_Local_Federation::supplier_types (void) const
{
  return this->federation_->supplier_types ();
}

ACE_INLINE const char*
ECM_Local_Federation::supplier_name (CORBA::ULong i) const
{
  return this->federation_->supplier_name (i);
}

ACE_INLINE CORBA::ULong
ECM_Local_Federation::supplier_ipaddr (CORBA::ULong i) const
{
  return this->federation_->supplier_ipaddr (i);
}

ACE_INLINE int
ECM_Local_Federation::consumer_types (void) const
{
  return this->federation_->consumer_types ();
}

ACE_INLINE const char*
ECM_Local_Federation::consumer_name (CORBA::ULong i) const
{
  return this->federation_->consumer_name (i);
}

ACE_INLINE CORBA::ULong
ECM_Local_Federation::consumer_ipaddr (CORBA::ULong i) const
{
  return this->federation_->consumer_ipaddr (i);
}
