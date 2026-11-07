/* -*- C++ -*- */
// Option manager for Event Server.

#ifndef DEVICE_OPTIONS_H
#define DEVICE_OPTIONS_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Profile_Timer.h"
#include "ace/Thread_Manager.h"
#include <string>

#if defined (ACE_HAS_THREADS)

class Options
{
public:
  Options () = default;
  ~Options () = default;
  Options (Options const &) = delete;
  Options (Options &&) = delete;
  Options &operator= (Options const &) = delete;
  Options &operator= (Options &&) = delete;
  void parse_args (int argc, ACE_TCHAR *argv[]);

  void   stop_timer ();
  void   start_timer ();

  void   thr_count (size_t count);
  size_t thr_count () const;

  void   initial_queue_length (size_t length);
  size_t initial_queue_length () const;

  void   high_water_mark (size_t size);
  size_t high_water_mark () const;

  void   low_water_mark (size_t size);
  size_t low_water_mark () const;

  void   message_size (size_t size);
  size_t message_size () const;

  void   iterations (size_t n);
  size_t iterations () const;

  void   t_flags (long flag);
  long   t_flags () const;

  void supplier_port (std::basic_string<ACE_TCHAR> const &port);
  std::basic_string<ACE_TCHAR> const &supplier_port () const;

  void consumer_port (std::basic_string<ACE_TCHAR> const &port);
  std::basic_string<ACE_TCHAR> const &consumer_port () const;

  void supplier_file (std::basic_string<ACE_TCHAR> const &file);
  std::basic_string<ACE_TCHAR> const &supplier_file () const;

  void consumer_file (std::basic_string<ACE_TCHAR> const &file);
  std::basic_string<ACE_TCHAR> const &consumer_file () const;

  bool debug () const;
  bool verbose () const;

  void   print_results ();

private:
  ACE_Profile_Timer itimer_;            // Time the process.
  size_t thr_count_ {4};
  long t_flags_ {THR_DETACHED};
  size_t high_water_mark_ {8 * 1024};
  size_t low_water_mark_ {1024};
  size_t message_size_ {128};
  size_t initial_queue_length_ {};
  size_t iterations_ {100000};
  bool debugging_ {};
  bool verbosity_ {};
  std::basic_string<ACE_TCHAR> consumer_port_ {ACE_TEXT ("-p 10000")};
  std::basic_string<ACE_TCHAR> supplier_port_ {ACE_TEXT ("-p 10001")};
  std::basic_string<ACE_TCHAR> consumer_file_ {ACE_TEXT ("-f/tmp/conupipe")};
  std::basic_string<ACE_TCHAR> supplier_file_ {ACE_TEXT ("-f/tmp/supupipe")};
};

extern Options options;

#include "Options.inl"
#endif /* ACE_HAS_THREADS */
#endif /* DEVICE_OPTIONS_H */
