/* -*- C++ -*- */
// Option manager for performance tests.

#ifndef _PERFORMANCE_TEST_OPTIONS_H
#define _PERFORMANCE_TEST_OPTIONS_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <string>
#include <vector>

#include "ace/Profile_Timer.h"
#include "ace/Log_Msg.h"
#include "ace/Thread_Manager.h"
#include "ace/svc_export.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Performance_Test_Options
{
public:
  Performance_Test_Options ();
  Performance_Test_Options (Performance_Test_Options const &) = delete;
  Performance_Test_Options (Performance_Test_Options &&) = delete;
  Performance_Test_Options &operator= (Performance_Test_Options const &) = delete;
  Performance_Test_Options &operator= (Performance_Test_Options &&) = delete;
  void   parse_args (int argc, ACE_TCHAR *argv[]);

  void   init ();

  void   start_timer ();
  void   stop_timer ();

  void   thr_count (size_t count);
  size_t thr_count () const;

  void pipe_addr (std::basic_string<ACE_TCHAR> const &pipe);
  std::basic_string<ACE_TCHAR> const &pipe_addr () const;

  void mapped_file (std::basic_string<ACE_TCHAR> const &filename);
  std::basic_string<ACE_TCHAR> const &mapped_file () const;

  void service_entry (std::basic_string<ACE_TCHAR> const &service_entry);
  std::basic_string<ACE_TCHAR> const &service_entry () const;

  void   sleep_time (size_t count);
  size_t sleep_time () const;

  void   logical_connections (size_t count);
  size_t logical_connections () const;

  void   physical_connections (size_t count);
  size_t physical_connections () const;

  void   consecutive_ports (size_t count);
  size_t consecutive_ports () const;

  void   initial_queue_length (size_t length);
  size_t initial_queue_length () const;

  void   high_water_mark (size_t size);
  size_t high_water_mark () const;

  void   low_water_mark (size_t size);
  size_t low_water_mark () const;

  void   msg_size (size_t size);
  size_t msg_size () const;

  void   iterations (size_t n);
  size_t iterations () const;

  void   n_lwps (size_t n);
  size_t n_lwps () const;

  void   t_flags (long flag);
  long   t_flags () const;

  size_t count () const;

  bool debug () const;
  bool verbose () const;
  bool do_checksum () const;
  bool do_generate () const;
  bool do_ack () const;
  bool do_delete () const;
  bool do_eager_exit () const;
  bool do_print_summary () const;
  bool do_udp () const;
  bool do_xdr () const;
  bool do_zero_copy () const;
  void   print_results ();

  std::vector<int> thr_work_count = std::vector<int> (10000);
  int thr_wc_size {10000};  // Max number of threads.

private:
  ACE_Profile_Timer _itimer;            // Keep track of time.
  std::basic_string<ACE_TCHAR> _service_entry; // Name of the shared object file and shared object.
  std::basic_string<ACE_TCHAR> _mapped_file;   // Name of the mapped file.
  std::basic_string<ACE_TCHAR> _pipe_addr {ACE_DEFAULT_RENDEZVOUS}; // Name of the STREAM pipe.
  size_t _sleep_time {100}; // Time to sleep.
  size_t _n_lwps {};
  size_t _thr_count {4}; // Number of threads to spawn.
  long _t_flags {}; // Flags to thr_create().
  size_t _high_water_mark {8 * 1024}; // ACE_Queue high water mark.
  size_t _low_water_mark {1024}; // ACE_Queue low water mark.
  size_t _msg_size {128}; // Size of a message.
  size_t _initial_queue_length {};
  size_t _logical_connections {1};
  size_t _physical_connections {1};
  size_t _iterations {100000}; // Number of iterations to run the test program.
  bool _generate {};
  bool _udp {};
  bool _debugging {};
  bool _verbosity {};
  bool _ack {true};
  bool _checksum {true};
  bool _xdr {true};
  bool _free_memory {true};
  bool _zero_copy {};
  bool _print_summary {};
  size_t _consecutive_ports {1};
  bool _eager_exit {};
};

// Make this available to any code that wants to see it!
extern ACE_Svc_Export Performance_Test_Options performance_test_options;

#include "Performance_Test_Options.inl"
#endif /* ACE_HAS_THREADS */
#endif /* _PERFORMANCE_TEST_OPTIONS_H */
