ACE_INLINE
TAO_Client_Connection_Handler::TAO_Client_Connection_Handler (ACE_Thread_Manager *)
  : in_use_ (0)
{}

ACE_INLINE CORBA::Boolean
TAO_Client_Connection_Handler::in_use (void)
{
  return in_use_;
}

ACE_INLINE void
TAO_Client_Connection_Handler::in_use (CORBA::Boolean flag)
{
  in_use_ = flag;
}

ACE_INLINE CONNECTOR *
TAO_Client_Strategy_Factory::connector (void)
{
  return 0;
}

ACE_INLINE
TAO_Client_Strategy_Factory::TAO_Client_Strategy_Factory (void)
{}

ACE_INLINE
TAO_Client_Strategy_Factory::~TAO_Client_Strategy_Factory (void)
{}
