// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Demux/CodeGen
//
// = FILENAME
//    servant_hdr.cpp
//
//    Generates the demux_test_i.h file that defines the implementation class
//    for the Demux_Test interface
//
// = AUTHOR
//
//    Aniruddha Gokhale
//
// ============================================================================


#include "codegen.h"

ACE_RCSID(CodeGen, servant_hdr, "$Id$")

// generate the implementation header
int
Demux_Test_CodeGenerator::gen_servant_hdr_code (void)
{
  // generate the implementation class
  FILE *fp;

  if ((fp = ACE_OS::fopen ("demux_test_i.h", "w")) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Demux_Test_CodeGenerator::gen_servant_hdr_code - "
                         "Failed to open file demux_test_i.h\n"),
                        -1);
    }

  ACE_OS::fprintf (fp, "#if !defined (TAO_DEMUX_TEST_I_H)\n");
  ACE_OS::fprintf (fp, "#define TAO_DEMUX_TEST_I_H\n\n");

  ACE_OS::fprintf (fp, "// Implementation of TAO's Demux_Test interface\n\n");

  // include file
  ACE_OS::fprintf (fp, "#include \"demux_testS.h\"\n\n");

  ACE_OS::fprintf (fp, "class Demux_Test_i : public POA_Demux_Test\n");
  ACE_OS::fprintf (fp, "{\n");

  ACE_OS::fprintf (fp, "public:\n");

  // ctor
  ACE_OS::fprintf (fp, "   Demux_Test_i (void);\n");
  ACE_OS::fprintf (fp, "   // ctor \n\n");
  
  // dtor
  ACE_OS::fprintf (fp, "   ~Demux_Test_i (void);\n");
  ACE_OS::fprintf (fp, "   // dtor \n\n");
  
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

      ACE_OS::fprintf (fp, "   void %s (CORBA::Environment &env = \n"
                       "            CORBA::Environment::"
                       "default_environment ());\n\n", 
                       str->c_str ());

      (void) iter.advance ();
    }

  ACE_OS::fprintf (fp, "};\n\n");

  ACE_OS::fprintf (fp, "#endif /* TAO_DEMUX_TEST_I_H */\n");
  ACE_OS::fclose (fp);

  return 0;
}  



