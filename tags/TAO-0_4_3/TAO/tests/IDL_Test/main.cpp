// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/IDL_Test
//
// = FILENAME
//    main.cpp
//
// = DESCRIPTION
//    This test isn't for functionality, but only for IDL compiler
//    and C++ compiler errors and warnings
//
// = AUTHORS
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include "ace/Log_Msg.h"

int 
main (int, char *[])
{
  ACE_DEBUG ((LM_DEBUG,
              "IDL_Test build completed without errors\n"
              "Check compiler output for warnings\n"));

  return 0;
}
