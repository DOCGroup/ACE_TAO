ACE_INLINE
TAO_Server_Strategy_Factory::TAO_Server_Strategy_Factory (void)
{}

ACE_INLINE
TAO_Server_Strategy_Factory::~TAO_Server_Strategy_Factory(void)
{}

ACE_INLINE CREATION_STRATEGY *
TAO_Server_Strategy_Factory::creation_strategy (void)
{
  return 0;
}

ACE_INLINE ACCEPT_STRATEGY *
TAO_Server_Strategy_Factory::accept_strategy (void)
{
  return 0;
}

ACE_INLINE CONCURRENCY_STRATEGY *
TAO_Server_Strategy_Factory::concurrency_strategy (void)
{
  return 0;
}

ACE_INLINE SCHEDULING_STRATEGY *
TAO_Server_Strategy_Factory::scheduling_strategy (void)
{
  return 0;
}

ACE_INLINE TAO_Object_Table *
TAO_Server_Strategy_Factory::object_lookup_strategy (void)
{
  return 0;
}
