ACE_INLINE
OA_Parameters::OA_Parameters()
  : using_threads_(0),
    thread_flags_(THR_NEW_LWP),
    context_p_(0),
    upcall_(0),
    forwarder_(0),
    oa_(0)
{
}

ACE_INLINE
int
OA_Parameters::using_threads()
{
  return using_threads_;
}

ACE_INLINE
void
OA_Parameters::using_threads(int i)
{
  using_threads_ = i;
}

ACE_INLINE
void*
OA_Parameters::context()
{
  return context_p_;
}

ACE_INLINE
void
OA_Parameters::context(void* p)
{
  context_p_ = p;
}

ACE_INLINE
OA_Parameters::UpcallFunc
OA_Parameters::upcall()
{
  return upcall_;
}

ACE_INLINE
void
OA_Parameters::upcall(OA_Parameters::UpcallFunc f)
{
  upcall_ = f;
}

ACE_INLINE
OA_Parameters::ForwardFunc
OA_Parameters::forwarder()
{
  return forwarder_;
}

ACE_INLINE
void
OA_Parameters::forwarder(OA_Parameters::ForwardFunc f)
{
  forwarder_ = f;
}

ACE_INLINE
CORBA_BOA_ptr
OA_Parameters::oa()
{
  return oa_;
}

ACE_INLINE
void
OA_Parameters::oa(CORBA_BOA_ptr anOA)
{
  oa_ = anOA;
}

ACE_INLINE
u_int
OA_Parameters::thread_flags()
{
  return thread_flags_;
}

ACE_INLINE
void
OA_Parameters::thread_flags(u_int f)
{
  thread_flags_ = f;
}

ACE_INLINE
void
OA_Parameters::addr(ACE_INET_Addr &addr)
{
  this->addr_ = addr;
}

ACE_INLINE
ACE_INET_Addr
OA_Parameters::addr()
{
  return this->addr_;
}

ACE_INLINE
void
OA_Parameters::demux_strategy(DEMUX_STRATEGY strategy)
{
  this->demux_ = strategy;      // Trust that the value is valid!
}

ACE_INLINE
void
OA_Parameters::demux_strategy(char* strategy)
{
  if (!ACE_OS::strcmp(strategy, "linear"))
    {
      this->demux_ = TAO_LINEAR;
    }
  else if (!ACE_OS::strcmp(strategy, "dynamic_hash"))
    {
      this->demux_ = TAO_DYNAMIC_HASH;
    }
  else if (!ACE_OS::strcmp(strategy, "user_def"))
    {
      this->demux_ = TAO_USER_DEFINED;
    }
  else if (!ACE_OS::strcmp(strategy, "active_demux"))
    {
      this->demux_ = TAO_ACTIVE_DEMUX;
    }
  else
    {
      // Provide fallback!
      this->demux_ = TAO_LINEAR;
    }
}

ACE_INLINE
OA_Parameters::DEMUX_STRATEGY
OA_Parameters::demux_strategy()
{
  return this->demux_;
}

ACE_INLINE
void
OA_Parameters::tablesize(CORBA_ULong tblsize)
{
  this->tablesize_ = tblsize;
}

ACE_INLINE
CORBA_ULong
OA_Parameters::tablesize()
{
  return this->tablesize_;
}


