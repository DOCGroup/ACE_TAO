//$Id$
ACE_INLINE
TAO_Fault_Tolerance_Service::TAO_Fault_Tolerance_Service (void)
  : ft_service_callback_ (0),
    ft_object_id_ (),
    ft_object_retention_id_ (-1),
    ft_object_retention_id_lock_ (0)
{
}



ACE_INLINE TAO_Service_Callbacks *
TAO_Fault_Tolerance_Service::service_callback (void)
{
  return this->ft_service_callback_;
}

ACE_INLINE const ACE_CString &
TAO_Fault_Tolerance_Service::client_id (void)
{
  return this->ft_object_id_;
}

ACE_INLINE void
TAO_Fault_Tolerance_Service::client_id (const char *id)
{
  this->ft_object_id_ = id;
}

ACE_INLINE CORBA::Long
TAO_Fault_Tolerance_Service::retention_id (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->ft_object_retention_id_lock_,
                            0));

  return ++this->ft_object_retention_id_;
}
