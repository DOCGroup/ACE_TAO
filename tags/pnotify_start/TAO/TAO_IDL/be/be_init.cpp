// $Id$

#include "ast_predefined_type.h"
#include "utl_identifier.h"
#include "global_extern.h"
#include "be_extern.h"
#include "../../tao/Version.h"

TAO_IDL_BE_Export void
BE_version (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "%s %s\n",
              ACE_TEXT ("TAO_IDL_BE, version"),
              ACE_TEXT (TAO_VERSION)));
}

TAO_IDL_BE_Export int
BE_init (int &, char *[])
{
  return 0;
}

TAO_IDL_BE_Export void
BE_post_init (const char *[], long)
{
  Identifier id ("void");
  UTL_ScopedName n (&id,
                    0);
  AST_Decl *d =
    idl_global->scopes ().bottom ()->lookup_primitive_type (
                                          AST_Expression::EV_void
                                        );
  be_global->void_type (AST_PredefinedType::narrow_from_decl (d));
  id.destroy ();
}

