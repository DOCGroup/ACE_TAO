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

ACE_INLINE ROA_ptr
ROA_Parameters::oa()
{
  return oa_;
}

ACE_INLINE void
ROA_Parameters::oa(ROA_ptr anOA)
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

