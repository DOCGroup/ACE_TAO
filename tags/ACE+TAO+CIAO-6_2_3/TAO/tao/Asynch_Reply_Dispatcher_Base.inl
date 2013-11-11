// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ACE_INLINE void
  ARDB_Refcount_Functor::operator () (
      TAO_Asynch_Reply_Dispatcher_Base *ardb) throw ()
  {
    (void) ardb->intrusive_remove_ref (ardb);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
