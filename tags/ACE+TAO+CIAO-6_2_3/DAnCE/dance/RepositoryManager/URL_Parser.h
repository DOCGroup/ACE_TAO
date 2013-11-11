/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    URL_Parser.h
 *
 *  $Id$
 *
 *  Parses a URL into its logical chunks
 *
 *  @author Stoyan Paunov
 */
//=============================================================================


#ifndef URL_PARSER_H
#define URL_PARSER_H

#include "ace/Singleton.h"  //for ACE_Singleton
#include "ace/Null_Mutex.h" //for ACE_Null_Mutex

// Forward declaration
class URL_Parser;

typedef ACE_Singleton <URL_Parser, ACE_Null_Mutex> TheURL_Parser;

class URL_Parser
{
public:

  friend class ACE_Singleton <URL_Parser, ACE_Null_Mutex>;

  /// Parses commandline arguments
  bool parse_args (int argc, ACE_TCHAR *argv[]);

  /// Return false on failure
  bool parseURL (ACE_TCHAR* url);

  void Error (void);

  /// Hostname to connect to
  ACE_TCHAR *hostname_;

  /// Port number to use
  u_short port_;

  /// Filename to upload/download
  ACE_TCHAR *filename_;

  /// Turns on verbosity
  bool debug_;

  /// destructor
  ~URL_Parser (void);

protected:
  /// protected constructor, singleton
  URL_Parser (void);
};



#endif /* URL_PARSER_H */
