/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Options.h
 *
 *  $Id$
 *
 *   Options is an Singleton for blobby
 *
 *
 *  @author Prashant Jain and Sumedh Mungee
 */
//=============================================================================


#ifndef ACE_BLOBBY_OPTIONS_H
#define ACE_BLOBBY_OPTIONS_H

#include "Blob.h"
#include "Blob_Handler.h"
#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ARGV.h"

class Options
{
public:
  /// Returns the singleton instance
  static Options *instance (void);

  /// parses commandline arguments
  void parse_args (int argc, ACE_TCHAR *argv[]);

  /// Hostname to connect to
  ACE_TCHAR *hostname_;

  /// Port number to use
  u_short port_;

  /// Filename to upload/download
  ACE_TCHAR *filename_;

  /// number of bytes to read/write
  int length_;

  /// offset to read/write
  int offset_;

  /// "r" means download (read), and "w" means upload (write).
  char operation_;

  /// turns on verbosity
  int debug_;

protected:
  Options (void);
  // protected constructor, singleton

  /// the singleton
  static Options *instance_;
};

#endif /* ACE_BLOBBY_OPTIONS_H */
