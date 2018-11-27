#include "tao/Version.h"
#include "global_extern.h"
#include "be_extern.h"
#include "drv_extern.h"
#include "idl_version.h"

#include "ace/OS_NS_stdlib.h"

#include "utl_identifier.h"
#include "ast_generator.h"
#include "utl_scope.h"
#include "fe_declarator.h"
#include "ast_field.h"
#include "ast_annotation_decl.h"

void
BE_version ()
{
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("annotest_idl version ") ACE_TEXT (TAO_VERSION)
    ACE_TEXT ("\n")));
}

int
BE_init (int&, ACE_TCHAR*[])
{
  ACE_NEW_RETURN (be_global, BE_GlobalData, -1);

  idl_global->idl_version_.version (IDL_VERSION_4);

  return 0;
}

void
BE_post_init (char *[], long)
{
  // Get Root Scope
  UTL_Scope *root_scope = idl_global->scopes ().bottom ();

  /*
   * @annotation id {
   *   unsigned long value;
   * };
   */

  /*
  // Create Annoation
  Identifier *name = new Identifier("id");
  UTL_ScopedName *scoped_name = new UTL_ScopedName(name, 0);
  AST_Annotation_Decl *id_annotation = idl_global->gen ()->create_annotation_decl (scoped_name);
  root_scope->fe_add_structure (id_annotation);

  // Enter id scope
  idl_global->scopes ().push (id_annotation);

  // Set value member
  AST_Decl* type = root_scope->lookup_primitive_type (AST_Expression::EV_ulong); // unsigned long
  FE_Declarator *declarator = new FE_Declarator (
    new UTL_ScopedName(new Identifier("value"), 0), FE_Declarator::FD_simple, 0);
  AST_Field *field = idl_global->gen ()->create_field (
    declarator->compose (type), declarator->name (), AST_Field::vis_PRIVATE);
  id_annotation->fe_add_field (field);

  // Exit id scope
  idl_global->scopes ().pop ();
  */

  /*
  idl_global->eval (
    "struct MyStruct {"
    "  short value;"
    "};"
  );
  */
}
