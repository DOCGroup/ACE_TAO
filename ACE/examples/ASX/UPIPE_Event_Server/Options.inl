/* -*- C++ -*- */
// Option manager for ustreams.

// Since this is only included in Options.h these should stay
// inline, not ACE_INLINE.
// FUZZ: disable check_for_inline


inline void
Options::supplier_port (const ACE_TCHAR *port)
{
  this->supplier_port_ = port;
}

inline const ACE_TCHAR *
Options::supplier_port ()
{
  return this->supplier_port_;
}

inline void
Options::supplier_file (const ACE_TCHAR *file)
{
  this->supplier_file_ = file;
}

inline const ACE_TCHAR *
Options::supplier_file ()
{
  return this->supplier_file_;
}

inline void
Options::consumer_file (const ACE_TCHAR *file)
{
  this->consumer_file_ = file;
}

inline const ACE_TCHAR *
Options::consumer_file ()
{
  return this->consumer_file_;
}

inline void
Options::consumer_port (const ACE_TCHAR *port)
{
  this->consumer_port_ = port;
}

inline const ACE_TCHAR *
Options::consumer_port ()
{
  return this->consumer_port_;
}

inline void
Options::start_timer ()
{
  this->itimer_.start ();
}

inline void
Options::stop_timer ()
{
  this->itimer_.stop ();
}

inline void
Options::thr_count (size_t count)
{
  this->thr_count_ = count;
}

inline size_t
Options::thr_count ()
{
  return this->thr_count_;
}

inline void
Options::initial_queue_length (size_t length)
{
  this->initial_queue_length_ = length;
}

inline size_t
Options::initial_queue_length ()
{
  return this->initial_queue_length_;
}

inline void
Options::high_water_mark (size_t size)
{
  this->high_water_mark_ = size;
}

inline size_t
Options::high_water_mark ()
{
  return this->high_water_mark_;
}

inline void
Options::low_water_mark (size_t size)
{
  this->low_water_mark_ = size;
}

inline size_t
Options::low_water_mark ()
{
  return this->low_water_mark_;
}

inline void
Options::message_size (size_t size)
{
  this->message_size_ = size;
}

inline size_t
Options::message_size ()
{
  return this->message_size_;
}

inline void
Options::iterations (size_t n)
{
  this->iterations_ = n;
}

inline size_t
Options::iterations ()
{
  return this->iterations_;
}

inline void
Options::t_flags (long flag)
{
  this->t_flags_ |= flag;
}

inline long
Options::t_flags ()
{
  return this->t_flags_;
}

inline int
Options::debug ()
{
  return this->debugging_;
}

inline int
Options::verbose ()
{
  return this->verbosity_;
}

