ACE_INLINE void ROA_Parameters::reactor(ACE_Reactor* i) { reactor_ = i; }
ACE_INLINE ACE_Reactor*  ROA_Parameters::reactor()
{
  if (reactor_ == 0) reactor_ = ACE_Service_Config::reactor();
  return reactor_;
}

ACE_INLINE int  ROA_Parameters::usingThreads()      { return usingThreads_; }
ACE_INLINE void ROA_Parameters::usingThreads(int i) { usingThreads_ = i; }

ACE_INLINE void* ROA_Parameters::context()        { return context_p_; }
ACE_INLINE void  ROA_Parameters::context(void* p) { context_p_ = p; }

ACE_INLINE ROA_Parameters::UpcallFunc ROA_Parameters::upcall() { return upcall_; }
ACE_INLINE void ROA_Parameters::upcall(ROA_Parameters::UpcallFunc f) { upcall_ = f; }

ACE_INLINE ROA_Parameters::ForwardFunc ROA_Parameters::forwarder() { return forwarder_; }
ACE_INLINE void ROA_Parameters::forwarder(ROA_Parameters::ForwardFunc f) { forwarder_ = f; }

ACE_INLINE TCP_OA_ptr ROA_Parameters::oa() { return oa_; }
ACE_INLINE void ROA_Parameters::oa(TCP_OA_ptr anOA) { oa_ = anOA; }

ACE_INLINE unsigned int ROA_Parameters::threadFlags() { return threadFlags_; }
ACE_INLINE void ROA_Parameters::threadFlags(unsigned int f) { threadFlags_ = f; }
