ACE_INLINE TAO_OA_Parameters *
TAO_Server_Connection_Handler::params (void)
{
  return this->params_;
}

ACE_INLINE void
TAO_Server_Connection_Handler::params (TAO_OA_Parameters *p)
{
  this->params_ = p;
}

