// $Id$

/* -*- C++ -*- */
// URL_Parser.h, Stoyan

//============================================================================
//
// = FILENAME
//    URL_Parser.h
//
// = DESCRIPTION
//     parses a URL
//
// = AUTHOR
//    Stoyan Paunov
//
//============================================================================

#ifndef URL_PARSER_H
#define URL_PARSER_H

#include "ace/Get_Opt.h"

//#if !defined (ACE_LACKS_PRAGMA_ONCE)
//# pragma once
//#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ARGV.h"

class URL_Parser
{
public:
  static URL_Parser *instance (void);
  // Returns the singleton instance

  bool parse_args (int argc, ACE_TCHAR *argv[]);
  // parses commandline arguments

  bool parseURL (char* url);

  void exitOnError (void);

  ACE_TCHAR *hostname_;
  // Hostname to connect to

  u_short port_;
  // Port number to use

  ACE_TCHAR *filename_;
  // Filename to upload/download

  int debug_;
  // turns on verbosity

  //destructor
  ~URL_Parser (void);
protected:
  URL_Parser (void);
  // protected constructor, singleton

  static URL_Parser *instance_;
  // the singleton
};

#endif /* URL_PARSER_H */
