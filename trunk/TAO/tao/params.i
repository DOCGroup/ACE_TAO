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

ACE_INLINE CORBA::BOA_ptr
TAO_OA_Parameters::oa (void)
{
  return oa_;
}

ACE_INLINE void
TAO_OA_Parameters::oa (CORBA::BOA_ptr anOA)
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
TAO_OA_Parameters::demux_strategy (TAO_Demux_Strategy strategy)
{
  this->demux_ = strategy;      // Trust that the value is valid!
}

//ACE_INLINE TAO_OA_Parameters::TAO_Demux_Strategy
ACE_INLINE TAO_Demux_Strategy
TAO_OA_Parameters::demux_strategy (void)
{
  return this->demux_;
}

ACE_INLINE void
TAO_OA_Parameters::tablesize (CORBA::ULong tblsize)
{
  this->tablesize_ = tblsize;
}

ACE_INLINE CORBA::ULong
TAO_OA_Parameters::tablesize (void)
{
  return this->tablesize_;
}

ACE_INLINE void 
TAO_OA_Parameters::userdef_lookup_strategy (TAO_Object_Table *&ot)
{
  // provide a way for user defined object key lookup strategies to be plugged
  // in
  ot_ = ot;
  ot = 0;  // we own it now
}

ACE_INLINE TAO_Object_Table *
TAO_OA_Parameters::userdef_lookup_strategy (void)
{
  // return the lookup strategy
  return ot_;
}

ACE_INLINE TAO_OA_Parameters *&
TAO_OA_Parameters::instance_i (void)
{
  return TAO_OA_Parameters::instance_;
}

ACE_INLINE ACE_SYNCH_MUTEX &
TAO_OA_Parameters::singleton_lock_i (void)
{
  return TAO_OA_Parameters::ace_singleton_lock_;
}
