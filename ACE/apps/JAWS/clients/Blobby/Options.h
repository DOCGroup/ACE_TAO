/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Options.h
 *
 *   Options is an Singleton for blobby
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
#include <string>

class Options
{
public:
  /// Returns the singleton instance
  static Options *instance ();

  Options (Options const &) = delete;
  Options (Options &&) = delete;
  Options &operator= (Options const &) = delete;
  Options &operator= (Options &&) = delete;

  /// parses commandline arguments
  void parse_args (int argc, ACE_TCHAR *argv[]);

  /// Hostname to connect to
  std::basic_string<ACE_TCHAR> hostname_;

  /// Port number to use
  u_short port_ {ACE_DEFAULT_HTTP_SERVER_PORT};

  /// Filename to upload/download
  std::basic_string<ACE_TCHAR> filename_;

  /// number of bytes to read/write
  int length_ {};

  /// offset to read/write
  int offset_ {};

  /// "r" means download (read), and "w" means upload (write).
  char operation_ {'r'};

  /// turns on verbosity
  bool debug_ {};

protected:
  Options () = default;
  // protected constructor, singleton
};

#endif /* ACE_BLOBBY_OPTIONS_H */
