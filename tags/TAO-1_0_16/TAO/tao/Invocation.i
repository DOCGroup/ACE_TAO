// This may look like C, but it's really -*- C++ -*-
//
// $Id$
//

ACE_INLINE IOP::ServiceContextList &
TAO_GIOP_Invocation::service_info (void)
{
  return this->service_info_;
}

ACE_INLINE CORBA::ULong
TAO_GIOP_Invocation::request_id (void)
{
  return this->request_id_;
}

ACE_INLINE TAO_OutputCDR &
TAO_GIOP_Invocation::out_stream (void)
{
  return this->out_stream_;
}

// ****************************************************************

ACE_INLINE
TAO_GIOP_Twoway_Invocation::
TAO_GIOP_Twoway_Invocation (TAO_Stub *stub,
                            const char *operation,
			    CORBA::ULong opname_len,
                            TAO_ORB_Core *orb_core)
  : TAO_GIOP_Invocation (stub, operation, opname_len, orb_core),
    rd_ (orb_core, this->service_info_)
{
}

ACE_INLINE TAO_InputCDR &
TAO_GIOP_Twoway_Invocation::inp_stream (void)
{
  return this->rd_.reply_cdr ();
}

// ****************************************************************

ACE_INLINE TAO::SyncScope
TAO_GIOP_Oneway_Invocation::sync_scope (void)
{
  return this->sync_scope_;
}

// *********************************************************************

ACE_INLINE
TAO_GIOP_Locate_Request_Invocation::
TAO_GIOP_Locate_Request_Invocation (TAO_Stub *stub,
                                    TAO_ORB_Core *orb_core)
  : TAO_GIOP_Invocation (stub, 0, 0, orb_core),
    rd_ (orb_core, this->service_info_)
{
}

ACE_INLINE TAO_InputCDR &
TAO_GIOP_Locate_Request_Invocation::inp_stream (void)
{
  return this->rd_.reply_cdr ();
}

