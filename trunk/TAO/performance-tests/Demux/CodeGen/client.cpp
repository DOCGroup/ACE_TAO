// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Demux/CodeGen
//
// = FILENAME
//    client.cpp
//
//    Generates the client-side include code
//
// = AUTHOR
//
//    Aniruddha Gokhale
//
// ============================================================================

#include "codegen.h"

ACE_RCSID(CodeGen, client, "$Id$")

// Generate the client side include file "client.i". This file has a table of
// function pointers to the the different operations of the
// interface. Depending on the strategy chosen, we make appropriate calls

int Demux_Test_CodeGenerator::gen_client_incl_code (void)
{
  FILE *fp;
  // client include code

  int i;
  if ((fp = ACE_OS::fopen ("demux_test_client.i", "w")) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "CodeGen::gen-client_incl_code - "
                         "Failed to open demux_test_client.i "
                         "file for writing\n"),
                         -1);
    }


  ACE_CString *str;
  ACE_Unbounded_Queue_Iterator <ACE_CString> iter (this->op_array_);

  // start with the first element
  (void)iter.first ();
  while (!iter.done ())
    {
      // grab the next element waiting to be grabbed
      (void) iter.next (str);
      
      ACE_OS::fprintf (fp,
                       "static void %s (Demux_Test_ptr objref, \n"
                       "                CORBA::Environment &env)\n",
                       str->c_str ());
      ACE_OS::fprintf (fp, "{\n");
      ACE_OS::fprintf (fp, "  objref->%s (env);\n", str->c_str ());
      ACE_OS::fprintf (fp, "}\n\n");
      (void) iter.advance ();
    }

  // now generate the method that initilaizes the operation database
  ACE_OS::fprintf (fp, "int Demux_Test_Client::init_operation_db (void)\n");
  ACE_OS::fprintf (fp, "{\n");

  i = 0;
  // start with the first element
  (void)iter.first ();
  while (!iter.done ())
    {
      // grab the next element waiting to be grabbed
      (void) iter.next (str);
      
      ACE_OS::fprintf (fp, "   this->op_db_[%d].op_ = %s;\n",
                       i, str->c_str ());
      (void) iter.advance ();
      i++;
    }

  ACE_OS::fprintf (fp, "   return 0;\n");
  ACE_OS::fprintf (fp, "}\n\n");
  return 0;
}

