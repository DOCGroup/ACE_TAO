ACE_INLINE TAO_OA_Parameters *
TAO_OA_Connection_Handler::params (void)
{
  return params_;
}

ACE_INLINE void
TAO_OA_Connection_Handler::params (TAO_OA_Parameters *p)
{
  params_ = p;
}

