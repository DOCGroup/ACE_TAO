/* -*- c++ -*- */
// $Id$

#if !defined (JAWS_SERVER_H)
#define JAWS_SERVER_H

#include "JAWS/Data_Block.h"

class JAWS_IO_Handler_Factory;

class JAWS_Server
{
public:
  JAWS_Server (void);
  JAWS_Server (int argc, char *argv[]);

  void init (int argc, char *argv[]);
  int open (JAWS_Pipeline_Handler *ph);

private:
  void parse_args (int argc, char *argv[]);
  // Parse arguments

private:
  int port_;            // port to listen on
  int concurrency_;     // 0 => pool, 1 => per request
  int dispatch_;        // 0 => synch, 1 => asynch
  int nthreads_;        // number of threads
  int maxthreads_;      // maximum number of threads
  long flags_;          // thread creation flags

  JAWS_IO_Handler_Factory *factory_;
};


#endif /* !defined (JAWS_SERVER_H) */
