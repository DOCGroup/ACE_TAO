// $Id$

namespace TAO
{
  ACE_INLINE void
  ARDB_Refcount_Functor::operator () (
      TAO_Asynch_Reply_Dispatcher_Base *ardb)
    ACE_THROW_SPEC (())
  {
    (void) ardb->decr_refcount ();
  }
}
