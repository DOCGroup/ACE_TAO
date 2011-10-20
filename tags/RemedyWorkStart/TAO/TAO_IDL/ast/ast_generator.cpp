// $Id$

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

#include "utl_identifier.h"

#include "nr_extern.h"
#include "ace/OS_NS_wchar.h"

#include "ast_generator.h"

AST_PredefinedType *
AST_Generator::create_predefined_type (AST_PredefinedType::PredefinedType t,
                                       UTL_ScopedName *n)
{
  AST_PredefinedType *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_PredefinedType (t, n),
                  0);

  return retval;
}

AST_Module *
AST_Generator::create_module (UTL_Scope *s,
                              UTL_ScopedName *n)
{
  AST_Module *retval = 0;

  // Check for another module of the same name in the scope.
  for (UTL_ScopeActiveIterator iter (s, UTL_Scope::IK_decls);
       !iter.is_done ();
       iter.next ())
    {
      // Can't just check node type here, since it could be a
      // template module or template module instantiation.
      AST_Module *m = AST_Module::narrow_from_decl (iter.item ());
      if (m && m->local_name ()->compare (n->last_component ()))
        {
          // Create this new module with referance to the
          // "first" previous module found in scope.
          ACE_NEW_RETURN (retval, AST_Module (n, m), 0);
          retval->prefix (const_cast<char *> (m->prefix ()));
          return retval;
        }
    }

  // Since the scope didn't contain the same module name, it
  // doesn't mean that we haven't see it before. If the scope
  // is itself a module, and has been previously opened, any
  // of the previous openings may contain a previous opening
  // of the module we're creating.
  //  AST_Module *prev_module = AST_Module::narrow_from_scope (s);
  AST_Module *prev_module = AST_Module::narrow_from_scope (s);
  if (prev_module)
    {
      while (!!(prev_module= prev_module->previous_opening ()))
        {
          for (UTL_ScopeActiveIterator iter (prev_module, UTL_Scope::IK_decls);
               !iter.is_done ();
               iter.next ())
            {
              AST_Module *m = AST_Module::narrow_from_decl (iter.item ());
              if (m && m->local_name ()->compare (n->last_component ()))
                {
                  // Create this new module with referance to the
                  // "first" previous module found in scope.
                  ACE_NEW_RETURN (retval, AST_Module (n, m), 0);
                  return retval;
                }
            }
        }
    }

  // There is no previous module to this one
  ACE_NEW_RETURN (retval, AST_Module (n), 0);
  return retval;
}

AST_Root *
AST_Generator::create_root (UTL_ScopedName *n)
{
  AST_Root *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Root (n),
                  0);

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
  AST_Interface *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Interface (n,
                                 inherits,
                                 n_inherits,
                                 inherits_flat,
                                 n_inherits_flat,
                                 is_local,
                                 is_abstract),
                  0);

  return retval;
}

AST_InterfaceFwd *
AST_Generator::create_interface_fwd (UTL_ScopedName *n,
                                     bool is_local,
                                     bool is_abstract)
{
  AST_Interface
    *full_defn = this->create_interface (n,
                                         0,
                                         -1,
                                         0,
                                         0,
                                         is_local,
                                         is_abstract);
  AST_InterfaceFwd *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_InterfaceFwd (full_defn, n),
                  0);

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
  AST_ValueType *retval = 0;
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
                  0);

  // The following helps with OBV_ namespace generation.
  AST_Module *m = AST_Module::narrow_from_scope (retval->defined_in ());
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
                                                     0,
                                                     -1,
                                                     0,
                                                     0,
                                                     0,
                                                     0,
                                                     0,
                                                     0,
                                                     is_abstract,
                                                     false,
                                                     false);

  AST_ValueTypeFwd *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_ValueTypeFwd (full_defn,
                                    n),
                  0);

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
  AST_EventType *retval = 0;
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
                  0);

  // The following helps with OBV_ namespace generation.
  AST_Module *m = AST_Module::narrow_from_scope (retval->defined_in ());

  if (m != 0)
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
                                                     0,
                                                     -1,
                                                     0,
                                                     0,
                                                     0,
                                                     0,
                                                     0,
                                                     0,
                                                     is_abstract,
                                                     false,
                                                     false);

  AST_EventTypeFwd *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_EventTypeFwd (full_defn,
                                    n),
                  0);

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
  AST_Component *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Component (n,
                                 base_component,
                                 supports_list,
                                 n_supports,
                                 supports_flat,
                                 n_supports_flat),
                  0);

  return retval;
}

AST_ComponentFwd *
AST_Generator::create_component_fwd (UTL_ScopedName *n)
{
  AST_Component *full_defn = this->create_component (n,
                                                     0,
                                                     0,
                                                     -1,
                                                     0,
                                                     0);

  AST_ComponentFwd *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_ComponentFwd (full_defn,
                                    n),
                  0);

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
  AST_Home *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Home (n,
                            base_home,
                            managed_component,
                            primary_key,
                            supports_list,
                            n_supports,
                            supports_flat,
                            n_supports_flat),
                  0);

  return retval;
}

AST_Exception *
AST_Generator::create_exception (UTL_ScopedName *n,
                                 bool is_local,
                                 bool is_abstract)
{
  AST_Exception *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Exception (n,
                                 is_local,
                                 is_abstract),
                  0);

  return retval;
}

AST_Structure *
AST_Generator::create_structure (UTL_ScopedName *n,
                                 bool is_local,
                                 bool is_abstract)
{
  AST_Structure *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Structure (n,
                                 is_local,
                                 is_abstract),
                  0);

  return retval;
}

AST_StructureFwd *
AST_Generator::create_structure_fwd (UTL_ScopedName *n)
{
  AST_Structure *full_defn = this->create_structure  (n,
                                                      false,
                                                      false);
  AST_StructureFwd *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_StructureFwd (full_defn,
                                    n),
                  0);

  full_defn->fwd_decl (retval);
  return retval;
}

AST_Enum *
AST_Generator::create_enum (UTL_ScopedName *n,
                            bool is_local,
                            bool is_abstract)
{
  AST_Enum *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Enum (n,
                            is_local,
                            is_abstract),
                  0);

  return retval;
}

AST_Operation *
AST_Generator::create_operation (AST_Type *rt,
                                 AST_Operation::Flags fl,
                                 UTL_ScopedName *n,
                                 bool is_local,
                                 bool is_abstract)
{
  AST_Operation *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Operation (rt,
                                 fl,
                                 n,
                                 is_local,
                                 is_abstract),
                  0);

  return retval;
}

AST_Field *
AST_Generator::create_field (AST_Type *ft,
                             UTL_ScopedName *n,
                             AST_Field::Visibility vis)
{
  AST_Field *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Field (ft,
                             n,
                             vis),
                  0);

  return retval;
}

AST_Argument *
AST_Generator::create_argument (AST_Argument::Direction d,
                                AST_Type *ft,
                                UTL_ScopedName *n)
{
  AST_Argument *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Argument (d,
                                ft,
                                n),
                  0);

  return retval;
}

AST_Attribute *
AST_Generator::create_attribute (bool ro,
                                 AST_Type *ft,
                                 UTL_ScopedName *n,
                                 bool is_local,
                                 bool is_abstract)
{
  AST_Attribute *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Attribute (ro,
                                 ft,
                                 n,
                                 is_local,
                                 is_abstract),
                  0);

  return retval;
}

AST_Union *
AST_Generator::create_union (AST_ConcreteType *dt,
                             UTL_ScopedName *n,
                             bool is_local,
                             bool is_abstract)
{
  AST_Union *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Union (dt,
                             n,
                             is_local,
                             is_abstract),
                  0);

  return retval;
}

AST_UnionFwd *
AST_Generator::create_union_fwd (UTL_ScopedName *n)
{
  AST_Union *full_defn = this->create_union (0,
                                             n,
                                             false,
                                             false);
  AST_UnionFwd *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_UnionFwd (full_defn,
                                n),
                  0);

  full_defn->fwd_decl (retval);
  return retval;
}

AST_UnionBranch *
AST_Generator::create_union_branch (UTL_LabelList *ll,
                                    AST_Type *ft,
                                    UTL_ScopedName *n)
{
  AST_UnionBranch *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_UnionBranch (ll,
                                   ft,
                                   n),
                  0);

  return retval;
}

AST_UnionLabel *
AST_Generator::create_union_label (AST_UnionLabel::UnionLabel ul,
                                   AST_Expression *v)
{
  AST_UnionLabel *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_UnionLabel (ul,
                                  v),
                  0);

  return retval;
}

AST_Constant *
AST_Generator::create_constant (AST_Expression::ExprType et,
                                AST_Expression *ev,
                                UTL_ScopedName *n)
{
  AST_Constant *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Constant (et,
                                ev,
                                n),
                  0);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (UTL_ScopedName *n)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (n),
                  0);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (AST_Expression *v,
                            AST_Expression::ExprType t)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (v,
                                  t),
                  0);

  return retval;
}

AST_Expression  *
AST_Generator::create_expr (AST_Expression::ExprComb c,
                            AST_Expression *v1,
                            AST_Expression *v2)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (c,
                                  v1,
                                  v2),
                  0);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::Long v)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (v),
                  0);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::LongLong l)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (l),
                  0);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::Boolean b)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (b),
                  0);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::ULong v)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (v),
                  0);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::ULongLong l)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (l),
                  0);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::ULong v,
                            AST_Expression::ExprType t)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (v, t),
                  0);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (UTL_String *s)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (s),
                  0);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::Char c)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (c),
                  0);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_OutputCDR::from_wchar wc)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (wc),
                  0);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (char *s)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (s),
                  0);

  return retval;
}

AST_Expression *
AST_Generator::create_expr (ACE_CDR::Double d)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (d),
                  0);

  return retval;
}

AST_EnumVal *
AST_Generator::create_enum_val (ACE_CDR::ULong v,
                                UTL_ScopedName *n)
{
  AST_EnumVal *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_EnumVal (v,
                               n),
                  0);

  return retval;
}

AST_Array *
AST_Generator::create_array (UTL_ScopedName *n,
                             ACE_CDR::ULong ndims,
                             UTL_ExprList *dims,
                             bool is_local,
                             bool is_abstract)
{
  AST_Array *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Array (n,
                             ndims,
                             dims,
                             is_local,
                             is_abstract),
                  0);

  return retval;
}

AST_Sequence *
AST_Generator::create_sequence (AST_Expression *ms,
                                AST_Type *bt,
                                UTL_ScopedName *n,
                                bool is_local,
                                bool is_abstract)
{
  AST_Sequence *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Sequence (ms,
                                bt,
                                n,
                                is_local,
                                is_abstract),
                  0);

  return retval;
}

AST_String *
AST_Generator::create_string (AST_Expression *ms)
{
  Identifier id ("string");
  UTL_ScopedName n (&id,
                    0);

  AST_String *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_String (AST_Decl::NT_string,
                              &n,
                              ms),
                  0);

  return retval;
}

AST_String      *
AST_Generator::create_wstring (AST_Expression *ms)
{
  Identifier id (sizeof (ACE_CDR::WChar) == 1
                   ? "string"
                   : "wstring");
  UTL_ScopedName n (&id,
                    0);
  AST_Decl::NodeType nt = sizeof (ACE_CDR::WChar) == 1
                            ? AST_Decl::NT_string
                            : AST_Decl::NT_wstring;

  AST_String *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_String (nt,
                              &n,
                              ms,
                              sizeof (ACE_OS::WChar)),
                  0);

  return retval;
}

AST_Typedef *
AST_Generator::create_typedef (AST_Type *bt,
                               UTL_ScopedName *n,
                               bool is_local,
                               bool is_abstract)
{
  AST_Typedef *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Typedef (bt,
                               n,
                               is_local,
                               is_abstract),
                  0);

  return retval;
}

AST_Native *
AST_Generator::create_native (UTL_ScopedName *n)
{
  AST_Native *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Native (n),
                  0);

  return retval;
}

AST_Factory *
AST_Generator::create_factory (UTL_ScopedName *n)
{
  AST_Factory *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Factory (n),
                  0);

  return retval;
}

AST_Finder *
AST_Generator::create_finder (UTL_ScopedName *n)
{
  AST_Finder *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Finder (n),
                  0);

  return retval;
}

AST_ValueBox *
AST_Generator::create_valuebox (UTL_ScopedName *n,
                                AST_Type *boxed_type)
{
  AST_ValueBox *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_ValueBox (n, boxed_type),
                  0);

  return retval;
}

AST_PortType *
AST_Generator::create_porttype (UTL_ScopedName *n)
{
  AST_PortType *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_PortType (n),
                  0);

  return retval;
}

AST_Provides *
AST_Generator::create_provides (UTL_ScopedName *n,
                                AST_Type *provides_type)
{
  AST_Provides *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Provides (n,
                                provides_type),
                  0);

  return retval;
}

AST_Uses *
AST_Generator::create_uses (UTL_ScopedName *n,
                            AST_Type *uses_type,
                            bool is_multiple)
{
  AST_Uses *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Uses (n,
                            uses_type,
                            is_multiple),
                  0);

  return retval;
}

AST_Publishes *
AST_Generator::create_publishes (UTL_ScopedName *n,
                                 AST_Type *publishes_type)
{
  AST_Publishes *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Publishes (n,
                                 publishes_type),
                  0);

  return retval;
}

AST_Emits *
AST_Generator::create_emits (UTL_ScopedName *n,
                             AST_Type *emits_type)
{
  AST_Emits *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Emits (n,
                             emits_type),
                  0);

  return retval;
}
AST_Consumes *
AST_Generator::create_consumes (UTL_ScopedName *n,
                                AST_Type *consumes_type)
{
  AST_Consumes *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Consumes (n,
                                consumes_type),
                  0);

  return retval;
}

AST_Extended_Port *
AST_Generator::create_extended_port (
  UTL_ScopedName *n,
  AST_PortType *porttype_ref)
{
  AST_Extended_Port *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Extended_Port (n,
                                     porttype_ref),
                  0);

  return retval;
}

AST_Mirror_Port *
AST_Generator::create_mirror_port (
  UTL_ScopedName *n,
  AST_PortType *porttype_ref)
{
  AST_Mirror_Port *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Mirror_Port (n,
                                   porttype_ref),
                  0);

  return retval;
}

AST_Connector *
AST_Generator::create_connector (
  UTL_ScopedName *n,
  AST_Connector *base_connector)
{
  AST_Connector *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Connector (n,
                                 base_connector),
                  0);

  return retval;
}

AST_Template_Module *
AST_Generator::create_template_module (
  UTL_ScopedName *n,
  FE_Utils::T_PARAMLIST_INFO *template_params)
{
  AST_Template_Module *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Template_Module (n,
                                       template_params),
                  0);

  return retval;
}

AST_Template_Module_Inst *
AST_Generator::create_template_module_inst (
  UTL_ScopedName *n,
  AST_Template_Module *ref,
  FE_Utils::T_ARGLIST *template_args)
{
  AST_Template_Module_Inst *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Template_Module_Inst (n,
                                            ref,
                                            template_args),
                  0);

  return retval;
}

AST_Template_Module_Ref *
AST_Generator::create_template_module_ref (
  UTL_ScopedName *n,
  AST_Template_Module *ref,
  UTL_StrList *param_refs)
{
  AST_Template_Module_Ref *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Template_Module_Ref (n,
                                           ref,
                                           param_refs),
                  0);

  return retval;
}

AST_Param_Holder *
AST_Generator::create_param_holder (
  UTL_ScopedName *parameter_name,
  FE_Utils::T_Param_Info *info)
{
  AST_Param_Holder *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Param_Holder (parameter_name,
                                    info),
                  0);

  return retval;
}

