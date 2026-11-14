/* -*- c++ -*- */
#ifndef JAWS_SERVER_H
#define JAWS_SERVER_H

#include "JAWS/Export.h"
#include "JAWS/Data_Block.h"
#include "JAWS/Policy.h"

class JAWS_IO_Handler_Factory;

class JAWS_Export JAWS_Server
{
public:
  JAWS_Server ();
  JAWS_Server (int argc, char *argv[]);

  void init (int argc, char *argv[]);
  int open (JAWS_Pipeline_Handler *ph, JAWS_Dispatch_Policy *dp = nullptr);

private:
  /// Parse command-line arguments.
  void parse_args (int argc, ACE_TCHAR *argv[]);

private:
  /// Ratio of asynchronous operations to threads.
  int ratio_;
  /// Port on which to listen.
  int port_;
  /// Concurrency strategy: 0 for thread pool, 1 for per-request threads.
  int concurrency_;
  /// I/O strategy: 0 for synchronous, 1 for asynchronous.
  int dispatch_;
  /// Number of worker threads.
  int nthreads_;
  /// Maximum number of worker threads.
  int maxthreads_;
  /// Thread creation flags.
  long flags_;

  JAWS_Default_Dispatch_Policy policy_;
};

#endif /* !defined (JAWS_SERVER_H) */
