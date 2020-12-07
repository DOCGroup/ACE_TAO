/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

*/

// The generator protocol is explained in detail in the IDL CFE
// design document.
// The AST_Generator class provides operations to instantiate any
// of the AST nodes. It contains an operation for every constructor
// of every AST class.

#include "ast_root.h"
#include "ast_valuebox.h"
#include "ast_valuetype.h"
#include "ast_valuetype_fwd.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_component.h"
#include "ast_component_fwd.h"
#include "ast_home.h"
#include "ast_mirror_port.h"
#include "ast_connector.h"
#include "ast_template_module.h"
#include "ast_template_module_inst.h"
#include "ast_template_module_ref.h"
#include "ast_param_holder.h"
#include "ast_provides.h"
#include "ast_uses.h"
#include "ast_publishes.h"
#include "ast_emits.h"
#include "ast_consumes.h"
#include "ast_exception.h"
#include "ast_enum.h"
#include "ast_attribute.h"
#include "ast_union.h"
#include "ast_union_fwd.h"
#include "ast_union_branch.h"
#include "ast_enum_val.h"
#include "ast_array.h"
#include "ast_sequence.h"
#include "ast_string.h"
#include "ast_structure_fwd.h"
#include "ast_typedef.h"
#include "ast_native.h"
#include "ast_factory.h"
#include "ast_finder.h"
#include "ast_fixed.h"
#include "ast_annotation_appl.h"
#include "ast_annotation_member.h"

#include "utl_identifier.h"

#include "nr_extern.h"
#include "ace/OS_NS_wchar.h"

#include "ast_generator.h"

AST_PredefinedType *
AST_Generator::create_predefined_type (AST_PredefinedType::PredefinedType t,
                                       UTL_ScopedName *n)
{
  AST_PredefinedType *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_PredefinedType (t, n),
                  nullptr);

  return retval;
}

AST_Module *
AST_Generator::create_module (UTL_Scope *s,
                              UTL_ScopedName *n)
{
  AST_Module *retval = nullptr;

  // Check for another module of the same name in the scope.
  for (UTL_ScopeActiveIterator iter (s, UTL_Scope::IK_decls);
       !iter.is_done ();
       iter.next ())
    {
      // Can't just check node type here, since it could be a
      // template module or template module instantiation.
      AST_Module *m = dynamic_cast<AST_Module*> (iter.item ());
      if (m && m->local_name ()->compare (n->last_component ()))
        {
          // Create this new module with referance to the
          // "first" previous module found in scope.
          ACE_NEW_RETURN (retval, AST_Module (n, m), nullptr);
          retval->prefix (const_cast<char *> (m->prefix ()));
          return retval;
        }
    }

  // Since the scope didn't contain the same module name, it
  // doesn't mean that we haven't see it before. If the scope
  // is itself a module, and has been previously opened, any
  // of the previous openings may contain a previous opening
  // of the module we're creating.
  AST_Module *prev_module = dynamic_cast<AST_Module*> (s);
  if (prev_module)
    {
      while (!!(prev_module= prev_module->previous_opening ()))
        {
          for (UTL_ScopeActiveIterator iter (prev_module, UTL_Scope::IK_decls);
               !iter.is_done ();
               iter.next ())
            {
              AST_Module *m = dynamic_cast<AST_Module*> (iter.item ());
              if (m && m->local_name ()->compare (n->last_component ()))
                {
                  // Create this new module with referance to the
                  // "first" previous module found in scope.
                  ACE_NEW_RETURN (retval, AST_Module (n, m), nullptr);
                  return retval;
                }
            }
        }
    }

  // There is no previous module to this one
  ACE_NEW_RETURN (retval, AST_Module (n), nullptr);
  return retval;
}

AST_Root *
AST_Generator::create_root (UTL_ScopedName *n)
{
  AST_Root *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Root (n),
                  nullptr);

  return retval;
}

AST_Interface *
AST_Generator::create_interface (UTL_ScopedName *n,
                                 AST_Type **inherits,
                                 long n_inherits,
                                 AST_Interface **inherits_flat,
                                 long n_inherits_flat,
                                 bool is_local,
                                 bool is_abstract)
{
  AST_Interface *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Interface (n,
                                 inherits,
                                 n_inherits,
                                 inherits_flat,
                                 n_inherits_flat,
                                 is_local,
                                 is_abstract),
                  nullptr);

  return retval;
}

AST_InterfaceFwd *
AST_Generator::create_interface_fwd (UTL_ScopedName *n,
                                     bool is_local,
                                     bool is_abstract)
{
  AST_Interface
    *full_defn = this->create_interface (n,
                                         nullptr,
                                         -1,
                                         nullptr,
                                         0,
                                         is_local,
                                         is_abstract);
  AST_InterfaceFwd *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_InterfaceFwd (full_defn, n),
                  nullptr);

  full_defn->fwd_decl (retval);
  return retval;
}

AST_ValueType *
AST_Generator::create_valuetype (UTL_ScopedName *n,
                                 AST_Type **inherits,
                                 long n_inherits,
                                 AST_Type *inherits_concrete,
                                 AST_Interface **inherits_flat,
                                 long n_inherits_flat,
                                 AST_Type **supports_list,
                                 long n_supports,
                                 AST_Type *supports_concrete,
                                 bool is_abstract,
                                 bool is_truncatable,
                                 bool is_custom)
{
  AST_ValueType *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_ValueType (n,
                                 inherits,
                                 n_inherits,
                                 inherits_concrete,
                                 inherits_flat,
                                 n_inherits_flat,
                                 supports_list,
                                 n_supports,
                                 supports_concrete,
                                 is_abstract,
                                 is_truncatable,
                                 is_custom),
                  nullptr);

  // The following helps with OBV_ namespace generation.
  AST_Module *m = dynamic_cast<AST_Module*> (retval->defined_in ());
  if (m)
    {
      m->set_has_nested_valuetype ();
    }

  return retval;
}

AST_ValueTypeFwd *
AST_Generator::create_valuetype_fwd (UTL_ScopedName *n,
                                     bool is_abstract)
{
  AST_ValueType *full_defn = this->create_valuetype (n,
                                                     nullptr,
                                                     -1,
                                                     nullptr,
                                                     nullptr,
                                                     0,
                                                     nullptr,
                                                     0,
                                                     nullptr,
                                                     is_abstract,
                                                     false,
                                                     false);

  AST_ValueTypeFwd *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_ValueTypeFwd (full_defn,
                                    n),
                  nullptr);

  full_defn->fwd_decl (retval);
  return retval;
}

AST_EventType *
AST_Generator::create_eventtype (UTL_ScopedName *n,
                                 AST_Type **inherits,
                                 long n_inherits,
                                 AST_Type *inherits_concrete,
                                 AST_Interface **inherits_flat,
                                 long n_inherits_flat,
                                 AST_Type **supports_list,
                                 long n_supports,
                                 AST_Type *supports_concrete,
                                 bool is_abstract,
                                 bool is_truncatable,
                                 bool is_custom)
{
  AST_EventType *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_EventType (n,
                                 inherits,
                                 n_inherits,
                                 inherits_concrete,
                                 inherits_flat,
                                 n_inherits_flat,
                                 supports_list,
                                 n_supports,
                                 supports_concrete,
                                 is_abstract,
                                 is_truncatable,
                                 is_custom),
                  nullptr);

  // The following helps with OBV_ namespace generation.
  AST_Module *m = dynamic_cast<AST_Module*> (retval->defined_in ());

  if (m != nullptr)
    {
      m->set_has_nested_valuetype ();
    }

  return retval;
}

AST_EventTypeFwd *
AST_Generator::create_eventtype_fwd (UTL_ScopedName *n,
                                     bool is_abstract)
{
  AST_EventType *full_defn = this->create_eventtype (n,
                                                     nullptr,
                                                     -1,
                                                     nullptr,
                                                     nullptr,
                                                     0,
                                                     nullptr,
                                                     0,
                                                     nullptr,
                                                     is_abstract,
                                                     false,
                                                     false);

  AST_EventTypeFwd *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_EventTypeFwd (full_defn,
                                    n),
                  nullptr);

  full_defn->fwd_decl (retval);
  return retval;
}

AST_Component *
AST_Generator::create_component (UTL_ScopedName *n,
                                 AST_Component *base_component,
                                 AST_Type **supports_list,
                                 long n_supports,
                                 AST_Interface **supports_flat,
                                 long n_supports_flat)
{
  AST_Component *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Component (n,
                                 base_component,
                                 supports_list,
                                 n_supports,
                                 supports_flat,
                                 n_supports_flat),
                  nullptr);

  return retval;
}

AST_ComponentFwd *
AST_Generator::create_component_fwd (UTL_ScopedName *n)
{
  AST_Component *full_defn = this->create_component (n,
                                                     nullptr,
                                                     nullptr,
                                                     -1,
                                                     nullptr,
                                                     0);

  AST_ComponentFwd *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_ComponentFwd (full_defn,
                                    n),
                  nullptr);

  full_defn->fwd_decl (retval);
  return retval;
}

AST_Home *
AST_Generator::create_home (UTL_ScopedName *n,
                            AST_Home *base_home,
                            AST_Component *managed_component,
                            AST_Type *primary_key,
                            AST_Type **supports_list,
                            long n_supports,
                            AST_Interface **supports_flat,
                            long n_supports_flat)
{
  AST_Home *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Home (n,
                            base_home,
                            managed_component,
                            primary_key,
                            supports_list,
                            n_supports,
                            supports_flat,
                            n_supports_flat),
                  nullptr);

  return retval;
}

AST_Exception *
AST_Generator::create_exception (UTL_ScopedName *n,
                                 bool is_local,
                                 bool is_abstract)
{
  AST_Exception *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Exception (n,
                                 is_local,
                                 is_abstract),
                  nullptr);

  return retval;
}

AST_Structure *
AST_Generator::create_structure (UTL_ScopedName *n,
                                 bool is_local,
                                 bool is_abstract)
{
  AST_Structure *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Structure (n,
                                 is_local,
                                 is_abstract),
                  nullptr);

  return retval;
}

AST_StructureFwd *
AST_Generator::create_structure_fwd (UTL_ScopedName *n)
{
  AST_Structure *full_defn = this->create_structure  (n,
                                                      false,
                                                      false);
  AST_StructureFwd *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_StructureFwd (full_defn,
                                    n),
                  nullptr);

  full_defn->fwd_decl (retval);
  return retval;
}

AST_Enum *
AST_Generator::create_enum (UTL_ScopedName *n,
                            bool is_local,
                            bool is_abstract)
{
  AST_Enum *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Enum (n,
                            is_local,
                            is_abstract),
                  nullptr);

  return retval;
}

AST_Operation *
AST_Generator::create_operation (AST_Type *rt,
                                 AST_Operation::Flags fl,
                                 UTL_ScopedName *n,
                                 bool is_local,
                                 bool is_abstract)
{
  AST_Operation *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Operation (rt,
                                 fl,
                                 n,
                                 is_local,
                                 is_abstract),
                  nullptr);

  return retval;
}

AST_Field *
AST_Generator::create_field (AST_Type *ft,
                             UTL_ScopedName *n,
                             AST_Field::Visibility vis)
{
  AST_Field *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Field (ft,
                             n,
                             vis),
                  nullptr);

  return retval;
}

AST_Argument *
AST_Generator::create_argument (AST_Argument::Direction d,
                                AST_Type *ft,
                                UTL_ScopedName *n)
{
  AST_Argument *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Argument (d,
                                ft,
                                n),
                  nullptr);

  return retval;
}

AST_Attribute *
AST_Generator::create_attribute (bool ro,
                                 AST_Type *ft,
                                 UTL_ScopedName *n,
                                 bool is_local,
                                 bool is_abstract)
{
  AST_Attribute *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Attribute (ro,
                                 ft,
                                 n,
                                 is_local,
                                 is_abstract),
                  nullptr);

  return retval;
}

AST_Union *
AST_Generator::create_union (AST_ConcreteType *dt,
                             UTL_ScopedName *n,
                             bool is_local,
                             bool is_abstract)
{
  AST_Union *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Union (dt,
                             n,
                             is_local,
                             is_abstract),
                  nullptr);

  return retval;
}

AST_UnionFwd *
AST_Generator::create_union_fwd (UTL_ScopedName *n)
{
  AST_Union *full_defn = this->create_union (nullptr,
                                             n,
                                             false,
                                             false);
  AST_UnionFwd *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_UnionFwd (full_defn,
                                n),
                  nullptr);

  full_defn->fwd_decl (retval);
  return retval;
}

AST_UnionBranch *
AST_Generator::create_union_branch (UTL_LabelList *ll,
                                    AST_Type *ft,
                                    UTL_ScopedName *n)
{
  AST_UnionBranch *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_UnionBranch (ll,
                                   ft,
                                   n),
                  nullptr);

  return retval;
}

AST_UnionLabel *
AST_Generator::create_union_label (AST_UnionLabel::UnionLabel ul,
                                   AST_Expression *v)
{
  AST_UnionLabel *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_UnionLabel (ul,
                                  v),
                  nullptr);

  return retval;
}

AST_Constant *
AST_Generator::create_constant (AST_Expression::ExprType et,
                                AST_Expression *ev,
                                UTL_ScopedName *n)
{
  AST_Constant *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Constant (et,
                                ev,
                                n),
                  nullptr);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (UTL_ScopedName *n)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (n),
                  nullptr);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (AST_Expression *v,
                            AST_Expression::ExprType t)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (v,
                                  t),
                  nullptr);

  return retval;
}

AST_Expression  *
AST_Generator::create_expr (AST_Expression::ExprComb c,
                            AST_Expression *v1,
                            AST_Expression *v2)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (c,
                                  v1,
                                  v2),
                  nullptr);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::Long v)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (v),
                  nullptr);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::LongLong l)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (l),
                  nullptr);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::Boolean b)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (b),
                  nullptr);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::ULong v)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (v),
                  nullptr);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::ULongLong l)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (l),
                  nullptr);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::ULong v,
                            AST_Expression::ExprType t)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (v, t),
                  nullptr);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (UTL_String *s)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (s),
                  nullptr);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::Char c)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (c),
                  nullptr);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_OutputCDR::from_wchar wc)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (wc),
                  nullptr);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (char *s)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (s),
                  nullptr);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::Double d)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (d),
                  nullptr);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (const ACE_CDR::Fixed &f)
{
  AST_Expression *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Expression (f),
                  nullptr);

  return retval;
}

AST_EnumVal *
AST_Generator::create_enum_val (ACE_CDR::ULong v,
                                UTL_ScopedName *n)
{
  AST_EnumVal *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_EnumVal (v,
                               n),
                  nullptr);

  return retval;
}

AST_Array *
AST_Generator::create_array (UTL_ScopedName *n,
                             ACE_CDR::ULong ndims,
                             UTL_ExprList *dims,
                             bool is_local,
                             bool is_abstract)
{
  AST_Array *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Array (n,
                             ndims,
                             dims,
                             is_local,
                             is_abstract),
                  nullptr);

  return retval;
}

AST_Sequence *
AST_Generator::create_sequence (AST_Expression *ms,
                                AST_Type *bt,
                                UTL_ScopedName *n,
                                bool is_local,
                                bool is_abstract)
{
  AST_Sequence *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Sequence (ms,
                                bt,
                                n,
                                is_local,
                                is_abstract),
                  nullptr);

  return retval;
}

AST_String *
AST_Generator::create_string (AST_Expression *ms)
{
  Identifier id ("string");
  UTL_ScopedName n (&id,
                    nullptr);

  AST_String *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_String (AST_Decl::NT_string,
                              &n,
                              ms),
                  nullptr);

  return retval;
}

AST_String      *
AST_Generator::create_wstring (AST_Expression *ms)
{
  Identifier id (sizeof (ACE_CDR::WChar) == 1
                   ? "string"
                   : "wstring");
  UTL_ScopedName n (&id,
                    nullptr);
  AST_Decl::NodeType nt = sizeof (ACE_CDR::WChar) == 1
                            ? AST_Decl::NT_string
                            : AST_Decl::NT_wstring;

  AST_String *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_String (nt,
                              &n,
                              ms,
                              sizeof (ACE_OS::WChar)),
                  nullptr);

  return retval;
}

AST_Fixed *
AST_Generator::create_fixed (AST_Expression *digits,
                             AST_Expression *scale)
{
  Identifier id ("fixed");
  UTL_ScopedName name (&id, nullptr);
  AST_Fixed *retval = nullptr;
  ACE_NEW_RETURN (retval, AST_Fixed (&name, digits, scale), nullptr);
  return retval;
}

AST_Typedef *
AST_Generator::create_typedef (AST_Type *bt,
                               UTL_ScopedName *n,
                               bool is_local,
                               bool is_abstract)
{
  AST_Typedef *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Typedef (bt,
                               n,
                               is_local,
                               is_abstract),
                  nullptr);

  return retval;
}

AST_Native *
AST_Generator::create_native (UTL_ScopedName *n)
{
  AST_Native *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Native (n),
                  nullptr);

  return retval;
}

AST_Factory *
AST_Generator::create_factory (UTL_ScopedName *n)
{
  AST_Factory *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Factory (n),
                  nullptr);

  return retval;
}

AST_Finder *
AST_Generator::create_finder (UTL_ScopedName *n)
{
  AST_Finder *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Finder (n),
                  nullptr);

  return retval;
}

AST_ValueBox *
AST_Generator::create_valuebox (UTL_ScopedName *n,
                                AST_Type *boxed_type)
{
  AST_ValueBox *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_ValueBox (n, boxed_type),
                  nullptr);

  return retval;
}

AST_PortType *
AST_Generator::create_porttype (UTL_ScopedName *n)
{
  AST_PortType *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_PortType (n),
                  nullptr);

  return retval;
}

AST_Provides *
AST_Generator::create_provides (UTL_ScopedName *n,
                                AST_Type *provides_type)
{
  AST_Provides *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Provides (n,
                                provides_type),
                  nullptr);

  return retval;
}

AST_Uses *
AST_Generator::create_uses (UTL_ScopedName *n,
                            AST_Type *uses_type,
                            bool is_multiple)
{
  AST_Uses *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Uses (n,
                            uses_type,
                            is_multiple),
                  nullptr);

  return retval;
}

AST_Publishes *
AST_Generator::create_publishes (UTL_ScopedName *n,
                                 AST_Type *publishes_type)
{
  AST_Publishes *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Publishes (n,
                                 publishes_type),
                  nullptr);

  return retval;
}

AST_Emits *
AST_Generator::create_emits (UTL_ScopedName *n,
                             AST_Type *emits_type)
{
  AST_Emits *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Emits (n,
                             emits_type),
                  nullptr);

  return retval;
}
AST_Consumes *
AST_Generator::create_consumes (UTL_ScopedName *n,
                                AST_Type *consumes_type)
{
  AST_Consumes *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Consumes (n,
                                consumes_type),
                  nullptr);

  return retval;
}

AST_Extended_Port *
AST_Generator::create_extended_port (
  UTL_ScopedName *n,
  AST_PortType *porttype_ref)
{
  AST_Extended_Port *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Extended_Port (n,
                                     porttype_ref),
                  nullptr);

  return retval;
}

AST_Mirror_Port *
AST_Generator::create_mirror_port (
  UTL_ScopedName *n,
  AST_PortType *porttype_ref)
{
  AST_Mirror_Port *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Mirror_Port (n,
                                   porttype_ref),
                  nullptr);

  return retval;
}

AST_Connector *
AST_Generator::create_connector (
  UTL_ScopedName *n,
  AST_Connector *base_connector)
{
  AST_Connector *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Connector (n,
                                 base_connector),
                  nullptr);

  return retval;
}

AST_Template_Module *
AST_Generator::create_template_module (
  UTL_ScopedName *n,
  FE_Utils::T_PARAMLIST_INFO *template_params)
{
  AST_Template_Module *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Template_Module (n,
                                       template_params),
                  nullptr);

  return retval;
}

AST_Template_Module_Inst *
AST_Generator::create_template_module_inst (
  UTL_ScopedName *n,
  AST_Template_Module *ref,
  FE_Utils::T_ARGLIST *template_args)
{
  AST_Template_Module_Inst *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Template_Module_Inst (n,
                                            ref,
                                            template_args),
                  nullptr);

  return retval;
}

AST_Template_Module_Ref *
AST_Generator::create_template_module_ref (
  UTL_ScopedName *n,
  AST_Template_Module *ref,
  UTL_StrList *param_refs)
{
  AST_Template_Module_Ref *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Template_Module_Ref (n,
                                           ref,
                                           param_refs),
                  nullptr);

  return retval;
}

AST_Param_Holder *
AST_Generator::create_param_holder (
  UTL_ScopedName *parameter_name,
  FE_Utils::T_Param_Info *info)
{
  AST_Param_Holder *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  AST_Param_Holder (parameter_name,
                                    info),
                  nullptr);

  return retval;
}

AST_Annotation_Decl *
AST_Generator::create_annotation_decl (
  UTL_ScopedName *name)
{
  AST_Annotation_Decl *retval = nullptr;
  ACE_NEW_RETURN (retval, AST_Annotation_Decl (name), nullptr);
  return retval;
}

AST_Annotation_Appl *
AST_Generator::create_annotation_appl (
  UTL_ScopedName *name,
  AST_Annotation_Appl::Params *params)
{
  AST_Annotation_Appl *retval = nullptr;
  ACE_NEW_RETURN (retval, AST_Annotation_Appl (name, params), nullptr);
  return retval;
}

AST_Annotation_Member *
AST_Generator::create_annotation_member (
  AST_Expression::ExprType expr_type,
  AST_Type *type,
  UTL_ScopedName *name)
{
  AST_Annotation_Member *retval = nullptr;
  ACE_NEW_RETURN (retval, AST_Annotation_Member (expr_type, type, name), nullptr);
  return retval;
}
