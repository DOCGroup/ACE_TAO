/* -*- C++ -*- */
// $Id$

/* Option manager for ustreams */

#if !defined (_OPTIONS_H)
#define _OPTIONS_H

#include "ace/OS.h"
#include "ace/Profile_Timer.h"
#include "ace/Log_Msg.h"
#include "ace/Thread_Manager.h"

#if defined (ACE_HAS_THREADS)

class Options
{
public:
  Options (void);
  void   parse_args (int argc, char *argv[]);

  void   init (void);

  void   start_timer (void);
  void   stop_timer (void);
  
  void   thr_count (size_t count);
  size_t thr_count (void);

  void   pipe_addr (char pipe[]);
  char   *pipe_addr (void);

  void   mapped_file (char filename[]);
  char   *mapped_file (void);

  void   service_entry (char *service_entry);
  char   *service_entry (void);

  void   sleep_time (size_t count);
  size_t sleep_time (void);

  void   logical_connections (size_t count);
  size_t logical_connections (void);

  void   physical_connections (size_t count);
  size_t physical_connections (void);

  void   consecutive_ports (size_t count);
  size_t consecutive_ports (void);

  void   initial_queue_length (size_t length);
  size_t initial_queue_length (void);

  void   high_water_mark (size_t size);
  size_t high_water_mark (void);

  void   low_water_mark (size_t size);
  size_t low_water_mark (void);

  void   msg_size (size_t size);
  size_t msg_size (void);

  void   iterations (size_t n);
  size_t iterations (void);

  void   n_lwps (size_t n);
  size_t n_lwps (void);

  void   t_flags (long flag);
  long   t_flags (void);

  size_t count (void);

  int    debug (void);
  int    verbose (void);
  int    do_checksum (void);
  int    do_generate (void);
  int    do_ack (void);
  int    do_delete (void);
  int    do_eager_exit (void);
  int    do_print_summary (void);
  int    do_udp (void);
  int    do_xdr (void);
  int    do_zero_copy (void);
  void   print_results (void);

  ACE_Atomic_Op<ACE_Thread_Mutex, size_t> msg_count;        /* Keep track of number of messages atomically */
  int *thr_work_count;  /* Count activity per-thread */
  int thr_wc_size;	     /* Max number of threads */

private:
  ACE_Profile_Timer _itimer;	        /* Keep track of time */
  char		*_service_entry;        /* Name of the shared object file and shared object */
  char		*_mapped_file;          /* Name of the mapped file */
  char		*_pipe_addr;            /* Name of the STREAM pipe */
  size_t	_sleep_time;            /* Time to sleep */
  size_t	_n_lwps;                /* Number of LWPs */
  size_t	_thr_count;             /* Number of threads to spawn */
  long		_t_flags;               /* Flags to thr_create() */
  size_t	_high_water_mark;       /* ACE_Queue high water mark */
  size_t	_low_water_mark;        /* ACE_Queue low water mark */
  size_t	_msg_size;              /* Size of a message */
  size_t	_initial_queue_length;  /* Initial number of items in the queue */
  size_t	_logical_connections;   /* Number of logical connections */
  size_t	_physical_connections;  /* Number of physical connections */
  size_t	_iterations;            /* Number of iterations to run the test program */
  int		_generate;              /* Generate the data */
  int		_udp;                   /* Use UDP format */
  int		_debugging;             /* Extra debugging info */
  int		_verbosity;             /* Extra verbose messages */
  int		_ack;                   /* Do an acknowledgement */
  int		_checksum;              /* Is checksumming enabled? */
  int		_xdr;                   /* Is xdr conversion enabled? */
  int		_free_memory;           /* Are we freeing up memory? */
  int		_zero_copy;             /* Implement a zero-copy driver? */
  int		_print_summary;         /* Print a summary of the results only */
  int		_consecutive_ports;     /* Number of consecutive messages from same port */
  int		_eager_exit;            /* Exit eagerly, without cleaning up */
};

/* Make this available to any code that wants to see it! */
extern Options options;

#include "Options.i"
#endif /* ACE_HAS_THREADS */
#endif /* _OPTIONS_H */
