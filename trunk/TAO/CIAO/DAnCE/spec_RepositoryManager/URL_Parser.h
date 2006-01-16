
/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    URL_Parser.h
 *
 *  $Id$
 *
 *  Parses a URL
 *
 *
 *  @author Stoyan Paunov
 */
//=============================================================================


#ifndef URL_PARSER_H
#define URL_PARSER_H

#include "ace/Get_Opt.h"
#include "ace/ARGV.h"

class URL_Parser
{
public:
  /// Returns the singleton instance
  static URL_Parser *instance (void);

  /// parses commandline arguments
  bool parse_args (int argc, ACE_TCHAR *argv[]);

  bool parseURL (char* url);

  void exitOnError (void);

  /// Hostname to connect to
  ACE_TCHAR *hostname_;

  /// Port number to use
  u_short port_;

  /// Filename to upload/download
  ACE_TCHAR *filename_;

  /// turns on verbosity
  int debug_;

  //destructor
  ~URL_Parser (void);
protected:
  URL_Parser (void);
  // protected constructor, singleton

  /// the singleton
  static URL_Parser *instance_;
};

#endif /* URL_PARSER_H */
