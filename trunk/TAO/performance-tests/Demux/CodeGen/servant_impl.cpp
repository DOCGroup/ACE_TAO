// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Demux/CodeGen
//
// = FILENAME
//    servant_impl.cpp
//
//    Generates the implementation of the Demux_Test interface in the
//    demux_test_i.cpp file.
//
// = AUTHOR
//
//    Aniruddha Gokhale
//
// ============================================================================


#include "codegen.h"

ACE_RCSID(CodeGen, servant_impl, "$Id$")

// generate the implementation 
int
Demux_Test_CodeGenerator::gen_servant_impl_code (void)
{
  // generate the implementation class
  FILE *fp;

  if ((fp = ACE_OS::fopen ("demux_test_i.cpp", "w")) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) Demux_Test_CodeGenerator::gen_servant_impl_code - "
                         "Failed to open file demux_test_i.cpp\n"),
                        -1);
    }

  ACE_OS::fprintf (fp, "// Implementation of TAO's Demux_Test interface\n\n");

  // include file
  ACE_OS::fprintf (fp, "#include \"demux_test_i.h\"\n\n");

  // ctor
  ACE_OS::fprintf (fp, "// ctor\n");
  ACE_OS::fprintf (fp, "Demux_Test_i::Demux_Test_i (void)\n");
  ACE_OS::fprintf (fp, "{}\n\n");
  
  // dtor
  ACE_OS::fprintf (fp, "// dtor\n");
  ACE_OS::fprintf (fp, "Demux_Test_i::~Demux_Test_i (void)\n");
  ACE_OS::fprintf (fp, "{}\n\n");
  
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

      ACE_OS::fprintf (fp, "void Demux_Test_i::%s (CORBA::Environment &env)\n",
                       str->c_str ());
      // if debugging desired
      ACE_OS::fprintf (fp, "{\n");
      ACE_OS::fprintf (fp, "   ACE_UNUSED_ARG (env);\n");

      if (this->debug_)
        {
          ACE_OS::fprintf (fp, "   ACE_DEBUG ((LM_DEBUG, "
                           "\"Inside Demux_Test_i::%s\\n\"));\n",
                           str->c_str ());
        }
      
      // if this is a shutdown method
      if (!ACE_OS::strcmp (str->c_str (), "shutdown"))
        {
          ACE_OS::fprintf (fp, 
                           "  TAO_ORB_Core_instance ()->orb ()->shutdown ();\n");
        }

      ACE_OS::fprintf (fp, "}\n\n");

      (void) iter.advance ();
    }

  ACE_OS::fclose (fp);

  return 0;
}  



