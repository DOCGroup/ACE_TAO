// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Demux/CodeGen
//
// = FILENAME
//    demux.cpp
//
//    Main driver program for the code generator
//
// = AUTHOR
//
//    Aniruddha Gokhale
//
// ============================================================================

#include "codegen.h"

ACE_RCSID(CodeGen, demux, "$Id$")

int
main (int argc, char *argv [])
{
  Demux_Test_CodeGenerator demux_cg;

  if (demux_cg.init (argc, argv) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Main : cannot initialize the code generator\n"),
                        -1);
    }

  if (demux_cg.run () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Main : cannot invoke the code generator\n"),
                        -1);
    }

  return 0;
}
