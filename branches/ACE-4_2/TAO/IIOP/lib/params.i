ACE_INLINE
TAO_OA_Parameters::TAO_OA_Parameters (void)
  : using_threads_ (0),
    thread_flags_ (THR_NEW_LWP),
    context_p_ (0),
    upcall_ (0),
    forwarder_ (0),
    oa_ (0)
{
}

ACE_INLINE int
TAO_OA_Parameters::using_threads (void)
{
  return using_threads_;
}

ACE_INLINE void
TAO_OA_Parameters::using_threads (int i)
{
  using_threads_ = i;
}

ACE_INLINE void *
TAO_OA_Parameters::context (void)
{
  return context_p_;
}

ACE_INLINE void
TAO_OA_Parameters::context (void* p)
{
  context_p_ = p;
}

ACE_INLINE TAO_OA_Parameters::UpcallFunc
TAO_OA_Parameters::upcall (void)
{
  return upcall_;
}

ACE_INLINE void
TAO_OA_Parameters::upcall (TAO_OA_Parameters::UpcallFunc f)
{
  upcall_ = f;
}

ACE_INLINE TAO_OA_Parameters::ForwardFunc
TAO_OA_Parameters::forwarder (void)
{
  return forwarder_;
}

ACE_INLINE void
TAO_OA_Parameters::forwarder (TAO_OA_Parameters::ForwardFunc f)
{
  forwarder_ = f;
}

ACE_INLINE CORBA_BOA_ptr
TAO_OA_Parameters::oa (void)
{
  return oa_;
}

ACE_INLINE void
TAO_OA_Parameters::oa (CORBA_BOA_ptr anOA)
{
  oa_ = anOA;
}

ACE_INLINE u_int
TAO_OA_Parameters::thread_flags (void)
{
  return thread_flags_;
}

ACE_INLINE void
TAO_OA_Parameters::thread_flags (u_int f)
{
  thread_flags_ = f;
}

ACE_INLINE void
TAO_OA_Parameters::addr (ACE_INET_Addr &addr)
{
  this->addr_ = addr;
}

ACE_INLINE ACE_INET_Addr
TAO_OA_Parameters::addr (void)
{
  return this->addr_;
}

ACE_INLINE void
TAO_OA_Parameters::demux_strategy (DEMUX_STRATEGY strategy)
{
  this->demux_ = strategy;      // Trust that the value is valid!
}

ACE_INLINE
void TAO_OA_Parameters::demux_strategy (const char* strategy)
{
  // Determine the demux strategy based on the given name
  if (!ACE_OS::strcmp (strategy, "linear"))
    {
      this->demux_ = TAO_LINEAR;
    }
  else if (!ACE_OS::strcmp (strategy, "dynamic_hash"))
    {
      this->demux_ = TAO_DYNAMIC_HASH;
    }
  else if (!ACE_OS::strcmp (strategy, "user_def"))
    {
      this->demux_ = TAO_USER_DEFINED;
    }
  else if (!ACE_OS::strcmp (strategy, "active_demux"))
    {
      this->demux_ = TAO_ACTIVE_DEMUX;
    }
  else
    {
      // Provide fallback!
      this->demux_ = TAO_LINEAR;
    }
}

ACE_INLINE TAO_OA_Parameters::DEMUX_STRATEGY
TAO_OA_Parameters::demux_strategy (void)
{
  return this->demux_;
}

ACE_INLINE void
TAO_OA_Parameters::tablesize (CORBA_ULong tblsize)
{
  this->tablesize_ = tblsize;
}

ACE_INLINE CORBA_ULong
TAO_OA_Parameters::tablesize (void)
{
  return this->tablesize_;
}


