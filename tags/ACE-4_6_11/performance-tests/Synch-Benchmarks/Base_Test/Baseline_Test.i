// $Id$

int
Baseline_Options::test_try_lock (void)
{
  return this->test_try_lock_;
}

size_t
Baseline_Options::multiply_factor (void)
{
  return this->multiply_factor_;
}

size_t
Baseline_Options::iteration (void)
{
  return this->iteration_;
}

int
Baseline_Options::add_time (ACE_Profile_Timer::ACE_Elapsed_Time &et)
{
  this->real_ += et.real_time;
  this->system_ += et.system_time;
  this->user_ += et.user_time;
  return 0;
}

int
Baseline_Options::inc_loop_counter (void)
{
  this->total_iteration_ += this->multiply_factor_;
  return (this->total_iteration_ >= this->iteration_);
}
