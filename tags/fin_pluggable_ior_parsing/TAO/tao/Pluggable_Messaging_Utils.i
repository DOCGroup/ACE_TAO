// -*- C++ -*-

//$Id$

ACE_INLINE
TAO_Pluggable_Reply_Params::TAO_Pluggable_Reply_Params (void)
  :svc_ctx_ (0),
   request_id_ (0),
   reply_status_ (0),
#if (TAO_HAS_MINIMUM_CORBA == 0)
   params_ (0),
#endif /*TAO_HAS_MINIMUM_CORBA*/
   service_context_ (0)


{

}


ACE_INLINE void 
TAO_Pluggable_Reply_Params::service_context_notowned (IOP::ServiceContextList *svc)
{
  this->service_context_ = svc;
}

ACE_INLINE IOP::ServiceContextList &
TAO_Pluggable_Reply_Params::service_context_notowned (void)
{
  return *this->service_context_;
}



