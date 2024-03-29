ACE_INLINE size_t
Baseline_Test_Base::iteration ()
{
  return this->iteration_;
}

ACE_INLINE int
Baseline_Test_Base::yield_method ()
{
  return this->yield_method_;
}

ACE_INLINE int
Baseline_Test_Options::test_try_lock ()
{
  return this->test_try_lock_;
}

ACE_INLINE size_t
Baseline_Test_Options::current_iteration ()
{
  return this->current_iteration_;
}

ACE_INLINE void
Baseline_Test_Options::start_inc_timer ()
{
  this->timer.start_incr ();
}

ACE_INLINE void
Baseline_Test_Options::stop_inc_timer ()
{
  this->timer.stop_incr ();
}

ACE_INLINE int
Baseline_Test_Options::inc_loop_counter ()
{
  return (++this->current_iteration_ < this->total_iteration_);
}
