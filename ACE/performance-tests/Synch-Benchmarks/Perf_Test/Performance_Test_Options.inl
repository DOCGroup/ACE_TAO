/* -*- C++ -*- */
/* Option manager for ustreams */

#include "ace/Get_Opt.h"

// Since this is only included in the .h file, these should be
// remain inline, not ACE_INLINE.
// FUZZ: disable check_for_inline


inline bool
Performance_Test_Options::do_print_summary () const
{
  return this->_print_summary;
}

inline bool
Performance_Test_Options::do_udp () const
{
  return this->_udp;
}

inline void
Performance_Test_Options::start_timer ()
{
  this->_itimer.start ();
}

inline void
Performance_Test_Options::stop_timer ()
{
  this->_itimer.stop ();
}

inline bool
Performance_Test_Options::do_generate () const
{
  return this->_generate;
}

inline bool
Performance_Test_Options::do_ack () const
{
  return this->_ack;
}

inline bool
Performance_Test_Options::do_eager_exit () const
{
  return this->_eager_exit;
}

inline bool
Performance_Test_Options::do_zero_copy () const
{
  return this->_zero_copy;
}

inline bool
Performance_Test_Options::do_checksum () const
{
  return this->_checksum;
}

inline bool
Performance_Test_Options::do_delete () const
{
  return this->_free_memory;
}

inline bool
Performance_Test_Options::do_xdr () const
{
  return this->_xdr;
}

inline void
Performance_Test_Options::n_lwps (size_t count)
{
  this->_n_lwps = count;
}

inline size_t
Performance_Test_Options::n_lwps () const
{
  return this->_n_lwps;
}

inline void
Performance_Test_Options::pipe_addr (std::basic_string<ACE_TCHAR> const &pipe)
{
  this->_pipe_addr = pipe;
}

inline std::basic_string<ACE_TCHAR> const &
Performance_Test_Options::pipe_addr () const
{
  return this->_pipe_addr;
}

inline void
Performance_Test_Options::service_entry (std::basic_string<ACE_TCHAR> const &service_entry)
{
  this->_service_entry = service_entry;
}

inline std::basic_string<ACE_TCHAR> const &
Performance_Test_Options::service_entry () const
{
  return this->_service_entry;
}

inline void
Performance_Test_Options::mapped_file (std::basic_string<ACE_TCHAR> const &filename)
{
  this->_mapped_file = filename;
}

inline std::basic_string<ACE_TCHAR> const &
Performance_Test_Options::mapped_file () const
{
  return this->_mapped_file;
}

inline void
Performance_Test_Options::sleep_time (size_t count)
{
  this->_sleep_time = count;
}

inline size_t
Performance_Test_Options::sleep_time () const
{
  return this->_sleep_time;
}

inline void
Performance_Test_Options::thr_count (size_t count)
{
  this->_thr_count = count;
}

inline size_t
Performance_Test_Options::thr_count () const
{
  return this->_thr_count;
}

inline void
Performance_Test_Options::consecutive_ports (size_t count)
{
  this->_consecutive_ports = count;
}

inline size_t
Performance_Test_Options::consecutive_ports () const
{
  return this->_consecutive_ports;
}

inline void
Performance_Test_Options::logical_connections (size_t count)
{
  this->_logical_connections = count;
}

inline size_t
Performance_Test_Options::logical_connections () const
{
  return this->_logical_connections;
}

inline void
Performance_Test_Options::physical_connections (size_t count)
{
  this->_physical_connections = count;
}

inline size_t
Performance_Test_Options::physical_connections () const
{
  return this->_physical_connections;
}

inline void
Performance_Test_Options::initial_queue_length (size_t length)
{
  this->_initial_queue_length = length;
}

inline size_t
Performance_Test_Options::initial_queue_length () const
{
  return this->_initial_queue_length;
}

inline void
Performance_Test_Options::high_water_mark (size_t size)
{
  this->_high_water_mark = size;
}

inline size_t
Performance_Test_Options::high_water_mark () const
{
  return this->_high_water_mark;
}

inline void
Performance_Test_Options::low_water_mark (size_t size)
{
  this->_low_water_mark = size;
}

inline size_t
Performance_Test_Options::low_water_mark () const
{
  return this->_low_water_mark;
}

inline void
Performance_Test_Options::msg_size (size_t size)
{
  this->_msg_size = size;
}

inline size_t
Performance_Test_Options::msg_size () const
{
  return this->_msg_size;
}

inline void
Performance_Test_Options::iterations (size_t n)
{
  this->_iterations = n;
}

inline size_t
Performance_Test_Options::iterations () const
{
  return this->_iterations;
}

inline void
Performance_Test_Options::t_flags (long flag)
{
  this->_t_flags |= flag;
}

inline long
Performance_Test_Options::t_flags () const
{
  return this->_t_flags;
}

inline bool
Performance_Test_Options::debug () const
{
  return this->_debugging;
}

inline bool
Performance_Test_Options::verbose () const
{
  return this->_verbosity;
}
