ACE_INLINE int
ROA_Parameters::using_threads()
{
  return using_threads_;
}

ACE_INLINE void
ROA_Parameters::using_threads(int i)
{
  using_threads_ = i;
}

ACE_INLINE void*
ROA_Parameters::context()
{
  return context_p_;
}

ACE_INLINE void
ROA_Parameters::context(void* p)
{
  context_p_ = p;
}

ACE_INLINE ROA_Parameters::UpcallFunc
ROA_Parameters::upcall()
{
  return upcall_;
}

ACE_INLINE void
ROA_Parameters::upcall(ROA_Parameters::UpcallFunc f)
{
  upcall_ = f;
}

ACE_INLINE ROA_Parameters::ForwardFunc
ROA_Parameters::forwarder()
{
  return forwarder_;
}

ACE_INLINE void
ROA_Parameters::forwarder(ROA_Parameters::ForwardFunc f)
{
  forwarder_ = f;
}

ACE_INLINE CORBA_BOA_ptr
ROA_Parameters::oa()
{
  return oa_;
}

ACE_INLINE void
ROA_Parameters::oa(CORBA_BOA_ptr anOA)
{
  oa_ = anOA;
}

ACE_INLINE u_int
ROA_Parameters::thread_flags()
{
  return thread_flags_;
}

ACE_INLINE void
ROA_Parameters::thread_flags(u_int f)
{
  thread_flags_ = f;
}

ACE_INLINE void
ROA_Parameters::addr(ACE_INET_Addr &addr)
{
  this->addr_ = addr;
}

ACE_INLINE ACE_INET_Addr
ROA_Parameters::addr()
{
  return this->addr_;
}

ACE_INLINE void
ROA_Parameters::demux_strategy(char* strategy)
{
  if (!ACE_OS::strcmp(strategy, "linear"))
    {
      this->demux_ = ROA_Parameters::TAO_LINEAR;
    }
  else if (!ACE_OS::strcmp(strategy, "dynamic_hash"))
    {
      this->demux_ = ROA_Parameters::TAO_DYNAMIC_HASH;
    }
  else if (!ACE_OS::strcmp(strategy, "user_def"))
    {
      this->demux_ = ROA_Parameters::TAO_USER_DEFINED;
    }
  else if (!ACE_OS::strcmp(strategy, "active_demux"))
    {
      this->demux_ = ROA_Parameters::TAO_ACTIVE_DEMUX;
    }
}

ACE_INLINE ROA_Parameters::DEMUX_STRATEGY
ROA_Parameters::demux_strategy()
{
  return this->demux_;
}

ACE_INLINE void
ROA_Parameters::tablesize(CORBA_ULong tblsize)
{
  this->tablesize_ = tblsize;
}

ACE_INLINE CORBA_ULong
ROA_Parameters::tablesize()
{
  return this->tablesize_;
}

ACE_INLINE
ROA_Factory::CREATION_STRATEGY*
ROA_Factory::creation_strategy()
{
  return 0;
}

ACE_INLINE
ROA_Factory::ACCEPT_STRATEGY*
ROA_Factory::accept_strategy()
{
  return 0;
}

ACE_INLINE
ROA_Factory::SCHEDULING_STRATEGY*
ROA_Factory::scheduling_strategy()
{
  return 0;
}

ACE_INLINE
ROA_Factory::CONCURRENCY_STRATEGY*
ROA_Factory::client_concurrency_strategy()
{
  return 0;
}
