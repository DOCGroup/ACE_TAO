// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ACE_INLINE void
  ARDB_Refcount_Functor::operator () (
      TAO_Reply_Dispatcher *ardb) throw ()
  {
    (void) ardb->decr_refcount ();
  }
}

ACE_INLINE GIOP::LocateStatusType
TAO_Reply_Dispatcher::locate_reply_status (void) const
{
  return this->locate_reply_status_;
}

ACE_INLINE GIOP::ReplyStatusType
TAO_Reply_Dispatcher::reply_status (void) const
{
  return this->reply_status_;
}

ACE_INLINE int 
TAO_Reply_Dispatcher::dispatch_reply (TAO_Pluggable_Reply_Params &params)
{
  return 0;
}

ACE_INLINE void 
TAO_Reply_Dispatcher::reply_timed_out (void)
{
  return;
}

ACE_INLINE void 
TAO_Reply_Dispatcher::connection_closed (void)
{
  return;
}

long
TAO_Reply_Dispatcher::schedule_timer (CORBA::ULong request_id,
                                             const ACE_Time_Value &max_wait_time)
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
