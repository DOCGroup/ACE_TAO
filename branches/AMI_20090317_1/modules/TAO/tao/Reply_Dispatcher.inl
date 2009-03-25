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

TAO_END_VERSIONED_NAMESPACE_DECL
