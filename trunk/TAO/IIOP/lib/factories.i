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

ACE_INLINE
TAO_Server_Factory::CONCURRENCY_STRATEGY*
TAO_Server_Factory::client_concurrency_strategy()
{
  return 0;
}

ACE_INLINE
TAO_Server_Factory::TAO_Server_Factory()
  : server_concurrency_strategy_(0),
    objtable_(0),
    client_concurrency_strategy_(0)
{
}

ACE_INLINE
void
TAO_Server_Factory::object_lookup_strategy(TAO_Object_Table *ot)
{
  this->objtable_ = ot;  // we assume ownership
}
