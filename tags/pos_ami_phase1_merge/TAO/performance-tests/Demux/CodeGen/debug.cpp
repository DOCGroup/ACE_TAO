// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Demux/CodeGen
//
// = FILENAME
//    debug.cpp
//
//    For debugging purposes
//
// = AUTHOR
//
//    Aniruddha Gokhale
//
// ============================================================================

#include "codegen.h"

ACE_RCSID(CodeGen, debug, "$Id$")

int
Demux_Test_CodeGenerator::print_names (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "*********** Begin Generated POA Names *********\n"));
  this->print_array (this->poa_array_);
  ACE_DEBUG ((LM_DEBUG,
              "*********** End Generated POA Names *********\n\n"));

  ACE_DEBUG ((LM_DEBUG,
              "*********** Begin Generated Object Names *********\n"));
  this->print_array (this->obj_array_);
  ACE_DEBUG ((LM_DEBUG,
              "*********** End Generated Object Names *********\n\n"));

  ACE_DEBUG ((LM_DEBUG,
              "*********** Begin Generated Operation Names *********\n"));
  this->print_array (this->op_array_);
  ACE_DEBUG ((LM_DEBUG,
              "*********** End Generated Operation Names *********\n\n"));

  return 0;
}

int
Demux_Test_CodeGenerator::print_array (ACE_Unbounded_Queue<ACE_CString> &arr)
{
  ACE_CString *str;
  ACE_Unbounded_Queue_Iterator <ACE_CString> iter (arr);

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

      ACE_DEBUG ((LM_DEBUG,
                  "\t%s\n",
                  str->c_str ()));
      (void) iter.advance ();
    }
  return 0; 	// not present
}

