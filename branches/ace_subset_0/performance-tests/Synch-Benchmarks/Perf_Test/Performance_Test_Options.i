/* -*- C++ -*- */
// $Id$

/* Option manager for ustreams */

#include "ace/Get_Opt.h"

// Since this is only included in the .h file, these should be 
// remain inline, not ACE_INLINE.
// FUZZ: disable check_for_inline


inline int
Performance_Test_Options::do_print_summary (void)
{
  return this->_print_summary;
}

inline int
Performance_Test_Options::do_udp (void)
{
  return this->_udp;
}

inline void
Performance_Test_Options::start_timer (void)
{
  this->_itimer.start ();
}

inline void
Performance_Test_Options::stop_timer (void)
{
  this->_itimer.stop ();
}

inline int
Performance_Test_Options::do_generate (void)
{
  return this->_generate;
}

inline int
Performance_Test_Options::do_ack (void)
{
  return this->_ack;
}

inline int
Performance_Test_Options::do_eager_exit (void)
{
  return this->_eager_exit;
}

inline int
Performance_Test_Options::do_zero_copy (void)
{
  return this->_zero_copy;
}

inline int
Performance_Test_Options::do_checksum (void)
{
  return this->_checksum;
}

inline int
Performance_Test_Options::do_delete (void)
{
  return this->_free_memory;
}

inline int
Performance_Test_Options::do_xdr (void)
{
  return this->_xdr;
}

inline void
Performance_Test_Options::n_lwps (size_t count)
{
  this->_n_lwps = count;
}

inline size_t
Performance_Test_Options::n_lwps (void)
{
  return this->_n_lwps;
}

inline void
Performance_Test_Options::pipe_addr (char *pipe)
{
  this->_pipe_addr = pipe;
}

inline char *
Performance_Test_Options::pipe_addr (void)
{
  return this->_pipe_addr;
}

inline void
Performance_Test_Options::service_entry (char *pipe)
{
  this->_service_entry = pipe;
}

inline char *
Performance_Test_Options::service_entry (void)
{
  return this->_service_entry;
}

inline void
Performance_Test_Options::mapped_file (char *filename)
{
  this->_mapped_file = filename;
}

inline char *
Performance_Test_Options::mapped_file (void)
{
  return this->_mapped_file;
}

inline void
Performance_Test_Options::sleep_time (size_t count)
{
  this->_sleep_time = count;
}

inline size_t
Performance_Test_Options::sleep_time (void)
{
  return this->_sleep_time;
}

inline void
Performance_Test_Options::thr_count (size_t count)
{
  this->_thr_count = count;
}

inline size_t
Performance_Test_Options::thr_count (void)
{
  return this->_thr_count;
}

inline void
Performance_Test_Options::consecutive_ports (size_t count)
{
  this->_consecutive_ports = count;
}

inline size_t
Performance_Test_Options::consecutive_ports (void)
{
  return this->_consecutive_ports;
}

inline void
Performance_Test_Options::logical_connections (size_t count)
{
  this->_logical_connections = count;
}

inline size_t
Performance_Test_Options::logical_connections (void)
{
  return this->_logical_connections;
}

inline void
Performance_Test_Options::physical_connections (size_t count)
{
  this->_physical_connections = count;
}

inline size_t
Performance_Test_Options::physical_connections (void)
{
  return this->_physical_connections;
}

inline void
Performance_Test_Options::initial_queue_length (size_t length)
{
  this->_initial_queue_length = length;
}

inline size_t
Performance_Test_Options::initial_queue_length (void)
{
  return this->_initial_queue_length;
}

inline void
Performance_Test_Options::high_water_mark (size_t size)
{
  this->_high_water_mark = size;
}

inline size_t
Performance_Test_Options::high_water_mark (void)
{
  return this->_high_water_mark;
}

inline void
Performance_Test_Options::low_water_mark (size_t size)
{
  this->_low_water_mark = size;
}

inline size_t
Performance_Test_Options::low_water_mark (void)
{
  return this->_low_water_mark;
}

inline void
Performance_Test_Options::msg_size (size_t size)
{
  this->_msg_size = size;
}

inline size_t
Performance_Test_Options::msg_size (void)
{
  return this->_msg_size;
}

inline void
Performance_Test_Options::iterations (size_t n)
{
  this->_iterations = n;
}

inline size_t
Performance_Test_Options::iterations (void)
{
  return this->_iterations;
}

inline void
Performance_Test_Options::t_flags (long flag)
{
  this->_t_flags |= flag;
}

inline long
Performance_Test_Options::t_flags (void)
{
  return this->_t_flags;
}

inline int
Performance_Test_Options::debug (void)
{
  return this->_debugging;
}

inline int
Performance_Test_Options::verbose (void)
{
  return this->_verbosity;
}
