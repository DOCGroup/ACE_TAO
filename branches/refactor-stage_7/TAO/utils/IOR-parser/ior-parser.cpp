// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/utils/IOR-parser
//
// = FILENAME
//    ior-parser.cpp
//
// = DESCRIPTION
//    Main loop of the IOR Parser.
//
// = AUTHORS
//    Priya Narasimhan <priya@lambda.ece.ucsb.edu>
//
// ============================================================================

#include "ior-handler.h"
#include "ace/Log_Msg.h"

IorHandler parser;

int 
main (int argc, char **argv)
{
  if (argc <= 1 || argc >= 3)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "USAGE: parseIor <IOR filename>\n"),
                       1);

  parser.readIorFromFile (argv[1]);
  return 0;
}





