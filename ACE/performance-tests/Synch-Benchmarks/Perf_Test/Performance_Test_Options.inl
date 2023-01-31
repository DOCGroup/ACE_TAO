/* -*- C++ -*- */
/* Option manager for ustreams */

#include "ace/Get_Opt.h"

// Since this is only included in the .h file, these should be
// remain inline, not ACE_INLINE.
// FUZZ: disable check_for_inline


inline int
Performance_Test_Options::do_print_summary ()
{
  return this->_print_summary;
}

inline int
Performance_Test_Options::do_udp ()
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

inline int
Performance_Test_Options::do_generate ()
{
  return this->_generate;
}

inline int
Performance_Test_Options::do_ack ()
{
  return this->_ack;
}

inline int
Performance_Test_Options::do_eager_exit ()
{
  return this->_eager_exit;
}

inline int
Performance_Test_Options::do_zero_copy ()
{
  return this->_zero_copy;
}

inline int
Performance_Test_Options::do_checksum ()
{
  return this->_checksum;
}

inline int
Performance_Test_Options::do_delete ()
{
  return this->_free_memory;
}

inline int
Performance_Test_Options::do_xdr ()
{
  return this->_xdr;
}

inline void
Performance_Test_Options::n_lwps (size_t count)
{
  this->_n_lwps = count;
}

inline size_t
Performance_Test_Options::n_lwps ()
{
  return this->_n_lwps;
}

inline void
Performance_Test_Options::pipe_addr (ACE_TCHAR *pipe)
{
  this->_pipe_addr = pipe;
}

inline ACE_TCHAR *
Performance_Test_Options::pipe_addr ()
{
  return this->_pipe_addr;
}

inline void
Performance_Test_Options::service_entry (ACE_TCHAR *pipe)
{
  this->_service_entry = pipe;
}

inline ACE_TCHAR *
Performance_Test_Options::service_entry ()
{
  return this->_service_entry;
}

inline void
Performance_Test_Options::mapped_file (ACE_TCHAR *filename)
{
  this->_mapped_file = filename;
}

inline ACE_TCHAR *
Performance_Test_Options::mapped_file ()
{
  return this->_mapped_file;
}

inline void
Performance_Test_Options::sleep_time (size_t count)
{
  this->_sleep_time = count;
}

inline size_t
Performance_Test_Options::sleep_time ()
{
  return this->_sleep_time;
}

inline void
Performance_Test_Options::thr_count (size_t count)
{
  this->_thr_count = count;
}

inline size_t
Performance_Test_Options::thr_count ()
{
  return this->_thr_count;
}

inline void
Performance_Test_Options::consecutive_ports (size_t count)
{
  this->_consecutive_ports = count;
}

inline size_t
Performance_Test_Options::consecutive_ports ()
{
  return this->_consecutive_ports;
}

inline void
Performance_Test_Options::logical_connections (size_t count)
{
  this->_logical_connections = count;
}

inline size_t
Performance_Test_Options::logical_connections ()
{
  return this->_logical_connections;
}

inline void
Performance_Test_Options::physical_connections (size_t count)
{
  this->_physical_connections = count;
}

inline size_t
Performance_Test_Options::physical_connections ()
{
  return this->_physical_connections;
}

inline void
Performance_Test_Options::initial_queue_length (size_t length)
{
  this->_initial_queue_length = length;
}

inline size_t
Performance_Test_Options::initial_queue_length ()
{
  return this->_initial_queue_length;
}

inline void
Performance_Test_Options::high_water_mark (size_t size)
{
  this->_high_water_mark = size;
}

inline size_t
Performance_Test_Options::high_water_mark ()
{
  return this->_high_water_mark;
}

inline void
Performance_Test_Options::low_water_mark (size_t size)
{
  this->_low_water_mark = size;
}

inline size_t
Performance_Test_Options::low_water_mark ()
{
  return this->_low_water_mark;
}

inline void
Performance_Test_Options::msg_size (size_t size)
{
  this->_msg_size = size;
}

inline size_t
Performance_Test_Options::msg_size ()
{
  return this->_msg_size;
}

inline void
Performance_Test_Options::iterations (size_t n)
{
  this->_iterations = n;
}

inline size_t
Performance_Test_Options::iterations ()
{
  return this->_iterations;
}

inline void
Performance_Test_Options::t_flags (long flag)
{
  this->_t_flags |= flag;
}

inline long
Performance_Test_Options::t_flags ()
{
  return this->_t_flags;
}

inline int
Performance_Test_Options::debug ()
{
  return this->_debugging;
}

inline int
Performance_Test_Options::verbose ()
{
  return this->_verbosity;
}
