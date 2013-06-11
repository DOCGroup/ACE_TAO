/* -*- C++ -*- */
// $Id$

/* Option manager for ustreams */

// Since this is only included in Options.h these should stay
// inline, not ACE_INLINE.
// FUZZ: disable check_for_inline

inline void
Options::supplier_port (u_short port)
{
  this->supplier_port_ = port;
}

inline u_short
Options::supplier_port (void)
{
  return this->supplier_port_;
}

inline void
Options::consumer_port (u_short port)
{
  this->consumer_port_ = port;
}

inline u_short
Options::consumer_port (void)
{
  return this->consumer_port_;
}

inline void
Options::start_timer (void)
{
  this->itimer_.start ();
}

inline void
Options::stop_timer (void)
{
  this->itimer_.stop ();
}

inline void
Options::thr_count (size_t count)
{
  this->thr_count_ = count;
}

inline size_t
Options::thr_count (void)
{
  return this->thr_count_;
}

inline void
Options::initial_queue_length (size_t length)
{
  this->initial_queue_length_ = length;
}

inline size_t
Options::initial_queue_length (void)
{
  return this->initial_queue_length_;
}

inline void
Options::high_water_mark (size_t size)
{
  this->high_water_mark_ = size;
}

inline size_t
Options::high_water_mark (void)
{
  return this->high_water_mark_;
}

inline void
Options::low_water_mark (size_t size)
{
  this->low_water_mark_ = size;
}

inline size_t
Options::low_water_mark (void)
{
  return this->low_water_mark_;
}

inline void
Options::message_size (size_t size)
{
  this->message_size_ = size;
}

inline size_t
Options::message_size (void)
{
  return this->message_size_;
}

inline void
Options::iterations (size_t n)
{
  this->iterations_ = n;
}

inline size_t
Options::iterations (void)
{
  return this->iterations_;
}

inline void
Options::t_flags (long flag)
{
  this->t_flags_ |= flag;
}

inline long
Options::t_flags (void)
{
  return this->t_flags_;
}

inline int
Options::debug (void)
{
  return this->debugging_;
}

inline int
Options::verbose (void)
{
  return this->verbosity_;
}

