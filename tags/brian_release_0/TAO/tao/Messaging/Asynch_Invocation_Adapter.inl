// $Id$


namespace TAO
{
  ACE_INLINE void
  Asynch_Invocation_Adapter::invoke (
      TAO::Exception_Data *ex,
      unsigned long ex_count
      ACE_ENV_ARG_DECL)
  {
    Invocation_Adapter::invoke (ex, ex_count  ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

} // End namespace TAO
