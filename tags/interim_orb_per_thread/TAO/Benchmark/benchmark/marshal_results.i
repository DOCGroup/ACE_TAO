
// ============================================================================
//
// = LIBRARY
//    CORBA Benchmark/Marshal
// 
// = FILENAME
//    marshal_results.i
//
// = AUTHOR
//    Aniruddha Gokhale
// 
// ============================================================================

ACE_INLINE
CORBA_Marshal_Results::CORBA_Marshal_Results (void)
  : filename_ (0),
    elapsed_time_ (0)
{
}

ACE_INLINE
CORBA_Marshal_Results::~CORBA_Marshal_Results (void)
{
}

ACE_INLINE int
CORBA_Marshal_Results::start_timer (void)
{
  return this->timer_.start ();
}

ACE_INLINE int
CORBA_Marshal_Results::stop_timer (void)
{
  return this->timer_.stop ();
}

ACE_INLINE ACE_Profile_Timer::ACE_Elapsed_Time &
CORBA_Marshal_Results::elapsed_time (void)
{
  (void) this->timer_.elapsed_time (this->et_);
  return this->et_;
}

ACE_INLINE ACE_Profile_Timer::Rusage
CORBA_Marshal_Results::elapsed_rusage (void)
{
  (void) this->timer_.elapsed_rusage (this->rusage_);
  return this->rusage_;
}

ACE_INLINE void
CORBA_Marshal_Results::filename (const char *fname)
{
  this->filename_ = fname;
}


