// $Id$

#include "PSDL_Scope.h"
#include "PSDL_Extern.h"
#include "Dump_Visitor.h"

int main (int argc, char *argv [])
{
  void *result_ptr = 0;

  TAO_PSDL_Extern psdl_extern;

  if (argc < 2)
    psdl_extern.TAO_PSDL_Extern_yyin (stdin);
  else
    psdl_extern.TAO_PSDL_Extern_yyin (ACE_OS::fopen (argv[1], "r"));

  ACE_OS::strtok (argv[1], ".");

  TAO_PSDL_Scope::instance ()->set_stub_prefix (argv[1]);
  TAO_PSDL_Scope::instance ()->set_root_scope ();

  // The syntax is checked and ASTs are build.
  int result = psdl_extern.TAO_PSDL_Extern_yyparse (result_ptr);

  if (result != 0)
  {
    ACE_DEBUG ((LM_ERROR,
                "Error in yyparse\n"));
    return -1;
  }

  TAO_PSDL_Node *tree_node = psdl_extern.TAO_PSDL_Extern_yyval ();

  // The following segment of the code is for printing out a parse
  // tree. To-Do: have a parse option so that
  // the parse tree will be printed out when that option is used.
  // {@@
  /*Dump_Visitor visitor (0);

  int result_visitor = tree_node->accept (&visitor);

  if (result_visitor != 0)
    ACE_DEBUG ((LM_DEBUG,
                "Error in creating the parse tree\n"));
  // @@}
  */

  // This segment is responsible for generating code
  // for the stubs.
  // {@@
  TAO_PSDL_Node_Visitor node_visitor;

  result = tree_node->accept (&node_visitor);

  if (result != 0)
    {
      ACE_DEBUG ((LM_ERROR,
                  "Error in writing the stubs\n"));
      return -1;
    }
  // @@}

  return 0;
}
