ACE_INLINE ACE_Reactor* ACE_ROA::reactor() {  return &theReactor; }

ACE_INLINE int  ACE_ROA::usingThreads()      { return usingThreads_; }
ACE_INLINE void ACE_ROA::usingThreads(int i) { usingThreads_ = i; }

ACE_INLINE void* ACE_ROA::context()        { return context_p; }
ACE_INLINE void  ACE_ROA::context(void* p) { context_p = p; }

ACE_INLINE ACE_ROA::UpcallFunc ACE_ROA::upcall() { return theUpcall; }
ACE_INLINE void ACE_ROA::upcall(ACE_ROA::UpcallFunc f) { theUpcall = f; }

ACE_INLINE TCP_OA_ptr ACE_ROA::oa() { return theOA; }
ACE_INLINE void ACE_ROA::oa(TCP_OA_ptr anOA) { theOA = anOA; }
