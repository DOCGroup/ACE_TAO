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
TAO_GIOP_Invocation::request_id (void)
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
    CORBA::Environment &ACE_TRY_ENV)
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

#if (TAO_HAS_RT_CORBA == 1)

ACE_INLINE TAO_Endpoint_Selection_State
TAO_GIOP_Invocation::get_endpoint_selection_state (void)
{
  return this->endpoint_selection_state_;
}

ACE_INLINE TAO_Stub *
TAO_GIOP_Invocation::get_stub (void)
{
  return this->stub_;
}

#endif /* TAO_HAS_RT_CORBA == 1 */

ACE_INLINE void
TAO_GIOP_Invocation::location_forward_i (TAO_Stub *stubobj,
                                         CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  /**
   * Add the profiles in the given stub object to the list of forward
   * profiles.
   */
  if (stubobj == 0)
    ACE_THROW (CORBA::INTERNAL ());

  // Modify the state as appropriate to include new forwarding profiles.
  this->endpoint_selector_->forward (this,
                                     stubobj->base_profiles (),
                                     ACE_TRY_ENV);
  ACE_CHECK;

  this->received_location_forward_ = 1;
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
