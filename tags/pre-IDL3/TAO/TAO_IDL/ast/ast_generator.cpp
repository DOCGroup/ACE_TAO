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

#include "idl.h"
#include "idl_extern.h"

#include "ace/config-all.h"

ACE_RCSID(ast, ast_generator, "$Id$")

// Public operations.

// Construct an AST_PredefinedType node (a predefined type).
AST_PredefinedType *
AST_Generator::create_predefined_type (AST_PredefinedType::PredefinedType t,
                                       UTL_ScopedName *n)
{
  AST_PredefinedType *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_PredefinedType (t,
                                      n),
                  0);

  return retval;
}

// Construct an AST_Module node (a module).
AST_Module *
AST_Generator::create_module (UTL_Scope *s,
                              UTL_ScopedName *n)
{
  // We create this first so if we find a module with the
  // same name from an included file, we can add its
  // members to the new module's scope.
  AST_Module *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Module (n),
                  0);

  AST_Decl *d = 0;
  AST_Module *m = 0;

  UTL_ScopeActiveIterator iter (s,
                                UTL_Scope::IK_decls);

  // Check for another module of the same name in this scope.
  while (!iter.is_done ())
    {
      d = iter.item ();

      if (d->node_type () == AST_Decl::NT_module)
        {
          // Does it have the same name as the one we're
          // supposed to create.
          if (d->local_name ()->compare (n->last_component ()))
            {
              m = AST_Module::narrow_from_decl (d);

              // Get m's previous_ member, plus all it's decls,
              // into the new modules's previous_ member.
              retval->add_to_previous (m);
            }
        }

      iter.next ();
    }

  // If this scope is itself a module, and has been previously
  // opened, the previous opening may contain a previous opening
  // of the module we're creating.
  d = ScopeAsDecl (s);
  AST_Decl::NodeType nt = d->node_type ();

  if (nt == AST_Decl::NT_module || nt == AST_Decl::NT_root)
    {
      m = AST_Module::narrow_from_decl (d);

      // AST_Module::previous_ is a set, so it contains each
      // entry only once, but previous_ will contain the decls
      // from all previous openings. See comment in
      // AST_Module::add_to_previous() body.
      d = m->look_in_previous (n->last_component ());

      if (d != 0)
        {
          if (d->node_type () == AST_Decl::NT_module)
            {
              m = AST_Module::narrow_from_decl (d);

              retval->add_to_previous (m);
            }
        }
    }

  // If we are opening module CORBA, we must add the predefined
  // types TypeCode, TCKind and maybe ValueBase.
  if (!ACE_OS::strcmp (retval->local_name ()->get_string (), "CORBA"))
    {
      retval->add_CORBA_members ();
    }

  return retval;
}

// Construct an AST_Root node (a node representing the root of an AST).
AST_Root *
AST_Generator::create_root (UTL_ScopedName *n)
{
  AST_Root *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Root (n),
                  0);

  return retval;
}

//     Construct an AST_Interface node (an interface).
AST_Interface *
AST_Generator::create_interface (UTL_ScopedName *n,
                                 AST_Interface **ih,
                                 long nih,
                                 AST_Interface **ih_flat,
                                 long nih_flat,
                                 idl_bool l,
                                 idl_bool a)
{
  AST_Interface *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Interface (n,
                                 ih,
                                 nih,
                                 ih_flat,
                                 nih_flat,
                                 l,
                                 a),
                  0);

  return retval;
}

// Construct an AST_InterfaceFwd node (a node representing the forward
// declaration of an interface).
AST_InterfaceFwd *
AST_Generator::create_interface_fwd (UTL_ScopedName *n,
                                     idl_bool local,
                                     idl_bool abstract)
{
  AST_InterfaceFwd *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_InterfaceFwd (this->create_interface (n,
                                                            0,
                                                            -1,
                                                            0,
                                                            0,
                                                            local,
                                                            abstract),
                                    n),
                  0);

  return retval;
}

// Create an AST_Interface node which is a valuetype.
AST_Interface *
AST_Generator::create_valuetype (UTL_ScopedName *n,
                                 AST_Interface **ih,
                                 long nih)
{
  AST_Interface *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Interface (n,
                                 ih,
                                 nih,
                                 0,
                                 0,
                                 0,
                                 0),
                  0);

  // Valuetypes are represented as be_valuetype derived from be_interface,
  // which derives from AST_Interface. If you construct a backend which
  // utilizes only the AST_... classes, you must instantiate an object that
  // returns true from AST_Interface::is_valuetype().
  // (@@@ (JP) implemented 2000/10/4)
  retval->set_valuetype ();

  // The following helps with OBV_ namespace generation.
  AST_Module *m = AST_Module::narrow_from_scope (retval->defined_in ());

  if (m != 0)
    {
      m->set_has_nested_valuetype ();
    }

  return retval;
}

// Create an AST_InterfaceFwd node whose full_definition
// member is a valuetype.
AST_InterfaceFwd *
AST_Generator::create_valuetype_fwd (UTL_ScopedName *n)
{
  // See note in create_valuetype().
  // Dummy placeholder must return true from is_valuetype().

  AST_Interface *dummy = this->create_valuetype (n,
                                                 0,
                                                 -1);

  AST_InterfaceFwd *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_InterfaceFwd (dummy,
                                    n),
                  0);

  return retval;
}


// Construct an AST_Exception node (an exception).
AST_Exception *
AST_Generator::create_exception (UTL_ScopedName *n,
                                 idl_bool local,
                                 idl_bool abstract)
{
  AST_Exception *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Exception (n,
                                 local,
                                 abstract),
                  0);

  return retval;
}

// Construct an AST_Structure node (a struct).
AST_Structure *
AST_Generator::create_structure (UTL_ScopedName *n,
                                 idl_bool local,
                                 idl_bool abstract)
{
  AST_Structure *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Structure (n,
                                 local,
                                 abstract),
                  0);

  return retval;
}

// Construct an AST_Enum node (an enum).
AST_Enum *
AST_Generator::create_enum (UTL_ScopedName *n,
                            idl_bool local,
                            idl_bool abstract)
{
  AST_Enum *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Enum (n,
                            local,
                            abstract),
                  0);

  return retval;
}

// Construct an AST_Operation node (an operation on an interface).
AST_Operation *
AST_Generator::create_operation (AST_Type *rt,
                                 AST_Operation::Flags fl,
                                 UTL_ScopedName *n,
                                 idl_bool local,
                                 idl_bool abstract)
{
  AST_Operation *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Operation (rt,
                                 fl,
                                 n,
                                 local,
                                 abstract),
                  0);

  return retval;
}

// Construct an AST_Field node (a field in a struct, union or exception).
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

// Construct an AST_Argument node (an argument to an operation).
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

// Construct an AST_Attribute node (an attribute).
AST_Attribute *
AST_Generator::create_attribute (idl_bool ro,
                                 AST_Type *ft,
                                 UTL_ScopedName *n,
                                 idl_bool local,
                                 idl_bool abstract)
{
  AST_Attribute *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Attribute (ro,
                                 ft,
                                 n,
                                 local,
                                 abstract),
                  0);

  return retval;
}

// Construct an AST_Union node (a union).
AST_Union *
AST_Generator::create_union (AST_ConcreteType *dt,
                             UTL_ScopedName *n,
                             idl_bool local,
                             idl_bool abstract)
{
  AST_Union *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Union (dt,
                             n,
                             local,
                             abstract),
                  0);

  return retval;
}

// Construct an AST_UnionBranch node (a branch in a union).
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

// Construct an AST_UnionLabel node (a label in a union).
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

// Construct an AST_Constant node (a constant).
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

// Construct an AST_Expression node denoting a symbolic name.
AST_Expression *
AST_Generator::create_expr (UTL_ScopedName *n)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (n),
                  0);

  return retval;
}

// Construct an AST_Expression denoting a coercion.
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

// Construct an AST_Expression node denoting a binary combinator.
// of two other AST_Expression nodes
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

// Construct an AST_Expression node denoting a long integer.
AST_Expression *
AST_Generator::create_expr (long v)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (v),
                  0);

  return retval;
}

// Construct an AST_Expression node denoting a long integer being used.
// as a boolean
AST_Expression *
AST_Generator::create_expr (long v,
                            AST_Expression::ExprType t)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (v,
                                  t),
                  0);

  return retval;
}

// Construct an AST_Expression node denoting an unsigned long integer.
AST_Expression *
AST_Generator::create_expr (unsigned long v)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (v),
                  0);

  return retval;
}

// Construct an AST_Expression node denoting a string (a char *
// encapsulated as String).
AST_Expression *
AST_Generator::create_expr (UTL_String *s)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (s),
                  0);

  return retval;
}

// Construct an AST_Expression node denoting a character.
AST_Expression *
AST_Generator::create_expr (char c)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (c),
                  0);

  return retval;
}

// Construct an AST_Expression node denoting a wide character.
AST_Expression *
AST_Generator::create_expr (ACE_OutputCDR::from_wchar wc)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (wc),
                  0);

  return retval;
}

// Construct an AST_Expression node denoting a wide string.
AST_Expression *
AST_Generator::create_expr (char *s)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (s),
                  0);

  return retval;
}

// Construct an AST_Expression node denoting a 64-bit floating point number.
AST_Expression *
AST_Generator::create_expr (double d)
{
  AST_Expression *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Expression (d),
                  0);

  return retval;
}

// Construct an AST_EnumVal node (an enumerator).
AST_EnumVal *
AST_Generator::create_enum_val (unsigned long v,
                                UTL_ScopedName *n)
{
  AST_EnumVal *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_EnumVal (v,
                               n),
                  0);

  return retval;
}

// Construct an AST_Array node (an array type or field modifier).
AST_Array *
AST_Generator::create_array (UTL_ScopedName *n,
                             unsigned long ndims,
                             UTL_ExprList *dims,
                             idl_bool local,
                             idl_bool abstract)
{
  AST_Array *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Array (n,
                             ndims,
                             dims,
                             local,
                             abstract),
                  0);

  return retval;
}

// Construct an AST_Sequence node (a sequence type definition).
AST_Sequence *
AST_Generator::create_sequence (AST_Expression *ms,
                                AST_Type *bt,
                                idl_bool local,
                                idl_bool abstract)
{
  AST_Sequence *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Sequence (ms,
                                bt,
                                local,
                                abstract),
                  0);

  return retval;
}

// Construct an AST_String node (a string type definition).
AST_String *
AST_Generator::create_string (AST_Expression *ms)
{
  AST_String *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_String (ms),
                  0);

  return retval;
}

// Construct an AST_String node denoting a wide string type definition.
AST_String      *
AST_Generator::create_wstring (AST_Expression *ms)
{
  AST_String *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_String (ms,
                              sizeof (ACE_OS::WChar)),
                  0);

  return retval;
}

// Construct an AST_Typedef node (a typedef).
AST_Typedef *
AST_Generator::create_typedef (AST_Type *bt,
                               UTL_ScopedName *n,
                               idl_bool local,
                               idl_bool abstract)
{
  AST_Typedef *retval = 0;
  ACE_NEW_RETURN (retval,
                  AST_Typedef (bt,
                               n,
                               local,
                               abstract),
                  0);

  return retval;
}

// Construct an AST_Native node.
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
