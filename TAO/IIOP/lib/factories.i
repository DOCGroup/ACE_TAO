ACE_INLINE
TAO_Client_Connection_Handler::TAO_Client_Connection_Handler(ACE_Thread_Manager* thrmgr)
  : in_use_(0)
{
}

ACE_INLINE
CORBA_Boolean
TAO_Client_Connection_Handler::in_use(void)
{
  return in_use_;
}

ACE_INLINE
void
TAO_Client_Connection_Handler::in_use(CORBA_Boolean flag)
{
  in_use_ = flag;
}

ACE_INLINE
TAO_Server_Factory::CREATION_STRATEGY*
TAO_Server_Factory::creation_strategy()
{
  return 0;
}

ACE_INLINE
TAO_Server_Factory::ACCEPT_STRATEGY*
TAO_Server_Factory::accept_strategy()
{
  return 0;
}

ACE_INLINE
TAO_Server_Factory::SCHEDULING_STRATEGY*
TAO_Server_Factory::scheduling_strategy()
{
  return 0;
}

#if defined(TAO_HAS_CLIENT_CONCURRENCY)
ACE_INLINE
TAO_Client_Factory::CONCURRENCY_STRATEGY*
TAO_Client_Factory::concurrency_strategy()
{
  return 0;
}
#endif

ACE_INLINE
TAO_Server_Factory::TAO_Server_Factory()
  : concurrency_strategy_(0),
    objtable_(0)
{
}

ACE_INLINE
void
TAO_Server_Factory::object_lookup_strategy(TAO_Object_Table *ot)
{
  this->objtable_ = ot;  // we assume ownership
}
