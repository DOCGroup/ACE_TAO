/* -*- C++ -*- */
// $Id $

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/ImplRepo_Service
//
// = FILENAME
//    Options.h
//
// = DESCRIPTION
//    This class implements the Options container for the Implementation 
//    Repository.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#ifndef OPTIONS_H
#define OPTIONS_H

#include "ace/Singleton.h"

class ACE_Configuration;

class Options
{
  // = TITLE
  //   Maintains the global options.
  //
  // = DESCRIPTION
  //   This class is converted into a Singleton by the 
  //   <ACE_Singleton> template.
public:
  Options ();
  // Default Constructor

  ~Options();
  // dtor

  int parse_args (int argc, ASYS_TCHAR *argv[]);
  // Parse the command-line arguments and initialize the options.

  int debug (void) const;
  // Debug level for the IR.  
  // 0 - Quiet
  // 1 - Trace messages
  // 2 - Detailed messages

  FILE *output_file (void) const;
  // Returns the file where the IOR should be stored.

  ACE_Configuration* config();
  // Returns the configuration implementation object

private:
  unsigned int debug_;
  // Debug information

  FILE *ior_output_file_;
  // File where the IOR of the server object is stored.

  ACE_Configuration* config_;
  // the persistent configuration object
};

// Typedef an Options Singleton. 
typedef ACE_Singleton <Options, ACE_Null_Mutex> OPTIONS;

#endif /* OPTIONS_H */

