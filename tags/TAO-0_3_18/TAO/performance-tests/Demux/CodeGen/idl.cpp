// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Demux/CodeGen
//
// = FILENAME
//    idl.cpp
//
//    Generates the IDL definition for the Demux_Test
//
// = AUTHOR
//
//    Aniruddha Gokhale
//
// ============================================================================

#include "codegen.h"

ACE_RCSID(CodeGen, idl, "$Id$")

// this method generates the IDL interface with all the methods in it
int
Demux_Test_CodeGenerator::gen_idl_interface (void)
{
  FILE *fp;

  if ((fp = ACE_OS::fopen ("demux_test.idl", "w")) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Demux_Test_CodeGenerator::gen_idl_interface - "
                         "Failed to open file demux_test.idl\n"),
                        -1);
    }

  ACE_OS::fprintf (fp, "// Interface for TAO's Demux_Test\n\n");
  ACE_OS::fprintf (fp, "interface Demux_Test\n");
  ACE_OS::fprintf (fp, "{\n");
  
  // go thru all the operation names
  ACE_CString *str;
  ACE_Unbounded_Queue_Iterator <ACE_CString> iter (this->op_array_);

  // start with the first element
  if (!iter.first ())
    {
      // queue is empty
      return 0;
    }
  while (!iter.done ())
    {
      // grab the next element waiting to be grabbed
      (void) iter.next (str);

      // only the shutdown method is always oneway
      if (!ACE_OS::strcmp (str->c_str (), "shutdown"))
        ACE_OS::fprintf (fp, "   oneway void shutdown ();\n");
      else
        ACE_OS::fprintf (fp, "   %svoid %s ();\n", 
                         (this->oneway_ ? "oneway ": ""),
                         str->c_str ());

      (void) iter.advance ();
    }

  ACE_OS::fprintf (fp, "};\n");
  ACE_OS::fclose (fp);

  return 0;
}
