// $Id$

namespace TAO
{
  ACE_INLINE void
  ARDB_Refcount_Functor::operator () (
      TAO_Asynch_Reply_Dispatcher_Base *ardb
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (())
  {
    (void) ardb->decr_refcount (ACE_ENV_SINGLE_ARG_PARAMETER);
  }
}
