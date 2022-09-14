#include "../Sequence_Unit_Tests/test_macros.h"

#include <ast_fixed.h>
#include <ast_typedef.h>
#include <fe_extern.h>
#include <utl_identifier.h>

#include <cstdlib>

int test_fixed_typedef ()
{
  Identifier id_fixed ("fixed");
  UTL_ScopedName name_fixed (&id_fixed, 0);
  AST_Expression digits (22), scale (2);
  AST_Fixed fixed (&name_fixed, &digits, &scale);

  Identifier id_typedef ("Myfixed");
  UTL_ScopedName name_typedef (&id_typedef, 0);
  AST_Typedef td (&fixed, &name_typedef, false, false);
  CHECK (td.owns_base_type ());
  return EXIT_SUCCESS;
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  FE_init ();
  int status = EXIT_SUCCESS;
  status += test_fixed_typedef ();
  return status;
}
