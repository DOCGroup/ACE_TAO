#include "../Sequence_Unit_Tests/test_macros.h"

#include <ast_fixed.h>
#include <ast_typedef.h>
#include <fe_extern.h>
#include <utl_identifier.h>

#include <cstdlib>
#include <memory>

int test_fixed_typedef ()
{
  Identifier id_fixed ("fixed");
  UTL_ScopedName name_fixed (&id_fixed, 0);
  std::unique_ptr<AST_Expression> digits = std::make_unique<AST_Expression> (22);
  std::unique_ptr<AST_Expression> scale = std::make_unique<AST_Expression> (2);
  std::unique_ptr<AST_Fixed> fixed =
    std::make_unique<AST_Fixed> (&name_fixed, digits.get (), scale.get ());
  digits.release ();
  scale.release ();

  Identifier id_typedef ("Myfixed");
  UTL_ScopedName name_typedef (&id_typedef, 0);
  AST_Typedef td (fixed.get (), &name_typedef, false, false);
  fixed.release ();
  CHECK (td.owns_base_type ());
  td.destroy ();
  return EXIT_SUCCESS;
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  FE_init ();
  int status = EXIT_SUCCESS;
  status += test_fixed_typedef ();
  return status;
}
