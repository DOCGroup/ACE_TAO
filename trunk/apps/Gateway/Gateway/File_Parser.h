/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    File_Parser.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_FILE_PARSER)
#define _FILE_PARSER

#include "ace/OS.h"

class FP 
  // = TITLE
  //     This class serves as a namespace for the Return_Type
{
public:
  enum Return_Type 
  { 
    EOLINE,
    EOFILE,
    SUCCESS, 
    COMMENT, 
    ERROR
  };
};

template <class ENTRY>
class File_Parser
  // = TITLE
  //     Class used to parse the configuration file for the Consumer
  //     Map.
{
public:
  // = Open and Close the file specified
  int open (const char filename[]);
  int close (void);

  virtual FP::Return_Type read_entry (ENTRY &, int &line_number) = 0;
  // Implementations use protected methods to fill in the entry.

protected:
  FP::Return_Type getword (char buf[]);
  // Read the next ASCII word.

  FP::Return_Type getint (int &value);
  // Read the next integer.

  FP::Return_Type readword (char buf[]);
  int delimiter (char ch);
  int comments (char ch);
  int skipline (void);

  FILE *infile_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "File_Parser.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("File_Parser.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* _FILE_PARSER */
