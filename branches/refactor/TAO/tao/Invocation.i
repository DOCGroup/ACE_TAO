// -*- C++ -*-
//
// $Id$

ACE_INLINE TAO_Service_Context &
TAO_GIOP_Invocation::request_service_context (void)
{
  return this->op_details_.request_service_context ();
}

ACE_INLINE TAO_Service_Context &
TAO_GIOP_Invocation::reply_service_context (void)
{
  return this->op_details_.reply_service_context ();
}

ACE_INLINE CORBA::ULong
TAO_GIOP_Invocation::request_id (void) const
{
  return this->op_details_.request_id ();
}

ACE_INLINE const char *
TAO_GIOP_Invocation::operation (void)
{
  return this->op_details_.opname ();
}

ACE_INLINE TAO_OutputCDR &
TAO_GIOP_Invocation::out_stream (void)
{
  return this->out_stream_;
}

ACE_INLINE void
TAO_GIOP_Invocation::restart_flag (CORBA::Boolean flag)
{
  // Set the flag
  this->restart_flag_ = flag;
}

ACE_INLINE CORBA::Object_ptr
TAO_GIOP_Invocation::forward_reference (void)
{
  return CORBA::Object::_duplicate (this->forward_reference_.in ());
}

ACE_INLINE CORBA::Boolean
TAO_GIOP_Invocation::received_location_forward (void) const
{
  return this->received_location_forward_;
}

ACE_INLINE void
TAO_GIOP_Invocation::init_inconsistent_policies (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_NEW_THROW_EX (this->inconsistent_policies_,
                    CORBA::PolicyList (0),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
}

ACE_INLINE CORBA::PolicyList *
TAO_GIOP_Invocation::get_inconsistent_policies (void)
{
  return this->inconsistent_policies_._retn ();
}

ACE_INLINE TAO_Stub *
TAO_GIOP_Invocation::stub (void)
{
  return this->stub_;
}

ACE_INLINE TAO_Endpoint *
TAO_GIOP_Invocation::endpoint (void)
{
  return this->endpoint_;
}

ACE_INLINE void
TAO_GIOP_Invocation::endpoint (TAO_Endpoint *ep)
{
  this->endpoint_ = ep;
}


ACE_INLINE TAO_Profile *
TAO_GIOP_Invocation::profile (void)
{
  return this->profile_;
}


ACE_INLINE void
TAO_GIOP_Invocation::profile (TAO_Profile *p)
{
  this->profile_ = p;
}

ACE_INLINE TAO_ORB_Core *
TAO_GIOP_Invocation::orb_core (void)
{
  return this->orb_core_;
}

ACE_INLINE TAO_Transport *&
TAO_GIOP_Invocation::transport (void)
{
  return this->transport_;
}

ACE_INLINE CORBA::PolicyList_var&
TAO_GIOP_Invocation::inconsistent_policies (void)
{
  return this->inconsistent_policies_;
}

ACE_INLINE ACE_Time_Value *
TAO_GIOP_Invocation::max_wait_time (void)
{
  return this->max_wait_time_;
}

ACE_INLINE CORBA::ULong&
TAO_GIOP_Invocation::profile_index (void)
{
  return this->profile_index_;
}

// ****************************************************************

ACE_INLINE
TAO_GIOP_Synch_Invocation::TAO_GIOP_Synch_Invocation (
    TAO_Stub *stub,
    const char *operation,
    CORBA::ULong opname_len,
    CORBA::Boolean argument_flag,
    TAO_ORB_Core *orb_core,
    int byte_order
  )
  : TAO_GIOP_Invocation (stub,
                         operation,
                         opname_len,
                         argument_flag,
                         orb_core,
       byte_order),
    rd_ (orb_core,
         this->op_details_.reply_service_info   ())
{
}

ACE_INLINE TAO_InputCDR &
TAO_GIOP_Synch_Invocation::inp_stream (void)
{
  this->transport_->assign_translators(&this->rd_.reply_cdr(),0);
  return this->rd_.reply_cdr ();
}

// ****************************************************************

ACE_INLINE
TAO_GIOP_Twoway_Invocation::TAO_GIOP_Twoway_Invocation (
    TAO_Stub *stub,
    const char *operation,
    CORBA::ULong opname_len,
    CORBA::Boolean argument_flag,
    TAO_ORB_Core *orb_core,
    int byte_order
  )
  : TAO_GIOP_Synch_Invocation (stub,
                               operation,
                               opname_len,
                               argument_flag,
                               orb_core,
                               byte_order)
{
}

ACE_INLINE
TAO_GIOP_Twoway_Invocation::~TAO_GIOP_Twoway_Invocation (void)
{
}

// ****************************************************************

ACE_INLINE
TAO_GIOP_Oneway_Invocation::~TAO_GIOP_Oneway_Invocation (void)
{
}

ACE_INLINE Messaging::SyncScope
TAO_GIOP_Oneway_Invocation::sync_scope (void)
{
  return this->sync_scope_;
}

// *********************************************************************

ACE_INLINE
TAO_GIOP_Locate_Request_Invocation::
TAO_GIOP_Locate_Request_Invocation (TAO_Stub *stub,
                                    TAO_ORB_Core *orb_core)
  : TAO_GIOP_Synch_Invocation (stub,
                               0,
                               0,
                               0,
                               orb_core)
{
}

ACE_INLINE
TAO_GIOP_Locate_Request_Invocation::~TAO_GIOP_Locate_Request_Invocation (void)
{
}
