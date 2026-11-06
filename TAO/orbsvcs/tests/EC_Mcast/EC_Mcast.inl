
ACE_INLINE int
ECM_Federation::sender_local_addr (ACE_INET_Addr& addr)
{
  return this->sender_->get_local_addr (addr);
}

ACE_INLINE char const*
ECM_Federation::name () const
{
  return this->name_.c_str ();
}

ACE_INLINE CORBA::UShort
ECM_Federation::mcast_port () const
{
  return this->mcast_port_;
}

ACE_INLINE int
ECM_Federation::supplier_types () const
{
  return static_cast<int> (this->supplier_names_.size ());
}

ACE_INLINE char const*
ECM_Federation::supplier_name (CORBA::ULong i) const
{
  if (i < this->supplier_names_.size ())
    return this->supplier_names_[i].c_str ();
  return nullptr;
}

ACE_INLINE CORBA::ULong
ECM_Federation::supplier_ipaddr (CORBA::ULong i) const
{
  if (i < this->supplier_ipaddr_.size ())
    return this->supplier_ipaddr_[i];
  return 0;
}

ACE_INLINE int
ECM_Federation::consumer_types () const
{
  return static_cast<int> (this->consumer_names_.size ());
}

ACE_INLINE char const*
ECM_Federation::consumer_name (CORBA::ULong i) const
{
  if (i < this->consumer_names_.size ())
    return this->consumer_names_[i].c_str ();
  return nullptr;
}

ACE_INLINE CORBA::ULong
ECM_Federation::consumer_ipaddr (CORBA::ULong i) const
{
  if (i < this->consumer_ipaddr_.size ())
    return this->consumer_ipaddr_[i];
  return 0;
}

ACE_INLINE ECM_Federation const*
ECM_Local_Federation::federation () const
{
  return this->federation_;
}

ACE_INLINE char const*
ECM_Local_Federation::name () const
{
  return this->federation_->name ();
}

ACE_INLINE CORBA::UShort
ECM_Local_Federation::mcast_port () const
{
  return this->federation_->mcast_port ();
}

ACE_INLINE int
ECM_Local_Federation::supplier_types () const
{
  return this->federation_->supplier_types ();
}

ACE_INLINE char const*
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
ECM_Local_Federation::consumer_types () const
{
  return this->federation_->consumer_types ();
}

ACE_INLINE char const*
ECM_Local_Federation::consumer_name (CORBA::ULong i) const
{
  return this->federation_->consumer_name (i);
}

ACE_INLINE CORBA::ULong
ECM_Local_Federation::consumer_ipaddr (CORBA::ULong i) const
{
  return this->federation_->consumer_ipaddr (i);
}
