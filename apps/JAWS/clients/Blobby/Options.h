/* -*- C++ -*- */
// $Id$

//============================================================================
//
// = LIBRARY
//    JAWS
// 
// = FILENAME
//    Options.h
//
// = DESCRIPTION
//     Options is an Singleton for blobby
//
// = AUTHOR
//    Prashant Jain and Sumedh Mungee
// 
//============================================================================

#if !defined (ACE_BLOBBY_OPTIONS_H)
#define ACE_BLOBBY_OPTIONS_H

#include "Blob.h"
#include "Blob_Handler.h"
#include "ace/Get_Opt.h"
#include "ace/ARGV.h"

class Options 
{
public:
  static Options *instance (void);
  // Returns the singleton instance

  void parse_args (int argc, char *argv[]);
  // parses commandline arguments

  char *hostname_;
  // Hostname to connect to

  u_short port_;
  // Port number to use 

  char *filename_;
  // Filename to upload/download

  int length_;
  // number of bytes to read/write

  int offset_;
  // offset to read/write

  char operation_;
  // "r" means download (read), and "w" means upload (write).

  int debug_;
  // turns on verbosity

protected:
  Options (void);
  // protected constructor, singleton

  static Options *instance_;
  // the singleton
};

#endif /* ACE_BLOBBY_OPTIONS_H */
