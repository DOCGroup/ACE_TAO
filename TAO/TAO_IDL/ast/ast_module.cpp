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

// AST_Modules denote IDL module declarations
// AST_Modules are subclasses of AST_Decl (they are not a type!) and
// of UTL_Scope.

#include "ast_module.h"
#include "ast_template_module_inst.h"
#include "ast_predefined_type.h"
#include "ast_valuebox.h"
#include "ast_valuetype.h"
#include "ast_valuetype_fwd.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_component.h"
#include "ast_component_fwd.h"
#include "ast_porttype.h"
#include "ast_connector.h"
#include "ast_home.h"
#include "ast_constant.h"
#include "ast_exception.h"
#include "ast_union.h"
#include "ast_union_fwd.h"
#include "ast_structure_fwd.h"
#include "ast_enum.h"
#include "ast_enum_val.h"
#include "ast_typedef.h"
#include "ast_native.h"
#include "ast_generator.h"
#include "ast_visitor.h"
#include "ast_extern.h"

#include "utl_err.h"
#include "utl_identifier.h"
#include "utl_indenter.h"

#include "global_extern.h"
#include "nr_extern.h"

AST_Decl::NodeType const
AST_Module::NT = AST_Decl::NT_module;

AST_Module::AST_Module (UTL_ScopedName *n)
 : AST_Decl (AST_Decl::NT_module,
             n),
   UTL_Scope (AST_Decl::NT_module),
   pd_has_nested_valuetype (0),
   from_inst_ (0)
{
}

AST_Module::~AST_Module (void)
{
}

// Dump this AST_Module node to the ostream o.
void
AST_Module::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "module ");
  this->local_name ()->dump (o);
  this->dump_i (o, " {\n");
  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);
  this->dump_i (o, "}");
}


// Involved in OBV_ namespace generation.
void
AST_Module::set_has_nested_valuetype (void)
{
  UTL_Scope *parent = this->defined_in ();

  if (!this->pd_has_nested_valuetype && parent)
    {
      AST_Module *pm = AST_Module::narrow_from_scope (parent);

      if (pm != 0)
        {
          pm->set_has_nested_valuetype ();
        }
    }

  this->pd_has_nested_valuetype = 1;
}

bool
AST_Module::has_nested_valuetype (void)
{
  return this->pd_has_nested_valuetype;
}

int
AST_Module::be_add_interface (AST_Interface *i,
                              AST_Interface *ix)
{
  // Add it to scope.
  this->add_to_scope (i,
                      ix);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (i,
                           false,
                           i->local_name (),
                           ix);

  return 0;
}

int
AST_Module::be_add_valuetype (AST_ValueType *v)
{
  // Add it to scope.
  this->add_to_scope (v);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (v,
                           false,
                           v->local_name ());

  return 0;
}

// Has this node been referenced here before?
bool
AST_Module::referenced (AST_Decl *e,
                        Identifier *id)
{
  bool refd = this->UTL_Scope::referenced (e, id);

  if (refd)
    {
      return true;
    }

  AST_Decl *d =
    this->look_in_prev_mods (e->local_name (), true);
  
  if (0 == d)
    {
      return false;
    }
    
  AST_Type *t = AST_Type::narrow_from_decl (d);
  return 0 == t || t->is_defined ();
}

void
AST_Module::add_to_previous (AST_Module *m)
{
  // This will eventually replace the individual declarations
  // list below.
  this->prev_mods_.insert (m);
  
  for (ACE_Unbounded_Set<AST_Module *>::CONST_ITERATOR i (
         m->prev_mods ());
       !i.done ();
       i.advance ())
    {
      AST_Module **mm = 0;
      i.next (mm);
      
      this->prev_mods_.insert (*mm);
    }
}

AST_Decl *
AST_Module::look_in_prev_mods (Identifier *e,
                               bool ignore_fwd)
{
  AST_Module **m = 0;
  AST_Decl *d = 0;
  AST_Decl *retval = 0;

  // If there are more than two openings of this module, we want
  // to get the last one - the one that will have the decls from
  // all the previous openings added to previous_.
  for (ACE_Unbounded_Set<AST_Module *>::CONST_ITERATOR iter (
         this->prev_mods_);
       !iter.done ();
       iter.advance ())
    {
      iter.next (m);
      
      for (UTL_ScopeActiveIterator i (*m, UTL_Scope::IK_decls);
           !i.is_done ();
           i.next ())
        {
          d = i.item ();

          if (ignore_fwd)
            {
              AST_Decl::NodeType nt = d->node_type ();

              if (nt == AST_Decl::NT_interface_fwd
                  || nt == AST_Decl::NT_eventtype_fwd
                  || nt == AST_Decl::NT_component_fwd
                  || nt == AST_Decl::NT_struct_fwd
                  || nt == AST_Decl::NT_union_fwd
                  || nt == AST_Decl::NT_valuetype_fwd)
                {
                  continue;
                }
            }

          if (e->case_compare (d->local_name ()))
            {
              retval = d;
            }
        }
    }
    
  return retval;
}

ACE_Unbounded_Set<AST_Module *> &
AST_Module::prev_mods (void)
{
  return this->prev_mods_;
}

void
AST_Module::destroy (void)
{
  this->UTL_Scope::destroy ();
  this->AST_Decl::destroy ();
}

int
AST_Module::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_module (this);
}

AST_Template_Module_Inst *
AST_Module::from_inst (void) const
{
  return this->from_inst_;
}

void
AST_Module::from_inst (AST_Template_Module_Inst *node)
{
  this->from_inst_ = node;
}

//================================================

AST_PredefinedType *
AST_Module::fe_add_predefined_type (AST_PredefinedType *t)
{
  return
    AST_PredefinedType::narrow_from_decl (
      this->fe_add_decl (t));
}

AST_Module *
AST_Module::fe_add_module (AST_Module *t)
{
  AST_Decl *d;
  AST_Module *m = 0;

  UTL_Scope *scope = t->defined_in ();
  const char *prefix_holder = 0;

  // If our prefix is empty, we check to see if an ancestor has one.
  while (ACE_OS::strcmp (t->prefix (), "") == 0 && scope != 0)
    {
      AST_Decl *parent = ScopeAsDecl (scope);
      prefix_holder = parent->prefix ();

      // We have reached global scope.
      if (prefix_holder == 0)
        {
          break;
        }

      t->prefix (const_cast<char *> (prefix_holder));
      scope = parent->defined_in ();
    }

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t)) != 0)
    {
      if (!can_be_redefined (d))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      this,
                                      d);
          return 0;
        }

      m = AST_Module::narrow_from_decl (d);

      // has_ancestor() returns TRUE if both nodes are the same.
      if (t != m)
        {
          if (t->has_ancestor (d))
            {
              idl_global->err ()->redefinition_in_scope (t,
                                                         d);
              return 0;
            }
        }

      const char *prev_prefix = d->prefix ();
      const char *this_prefix = t->prefix ();

      if (ACE_OS::strcmp (this_prefix, "") == 0)
        {
          t->prefix (const_cast<char *> (prev_prefix));
        }
      else
        {
          if (ACE_OS::strcmp (prev_prefix, "") == 0)
            {
              d->prefix (const_cast<char *> (this_prefix));
            }
        }
    }

  // If this node is not a reopened module, add it to scope and referenced.
  if (m == 0 || t != m)
    {
      // Add it to scope.
      this->add_to_scope (t);

      // Add it to set of locally referenced symbols.
      this->add_to_referenced (t,
                               false,
                               t->local_name ());
    }

  return t;
}

AST_Template_Module_Inst *
AST_Module::fe_add_template_module_inst (AST_Template_Module_Inst *t)
{
  return
    AST_Template_Module_Inst::narrow_from_decl (
      this->fe_add_ref_decl (t));
}

AST_Interface *
AST_Module::fe_add_interface (AST_Interface *t)
{
  return
    this->fe_add_full_intf_decl<AST_Interface> (t);
}

AST_ValueBox *
AST_Module::fe_add_valuebox (AST_ValueBox *t)
{
  return
    AST_ValueBox::narrow_from_decl (
      this->fe_add_decl (t));
}

AST_ValueType *
AST_Module::fe_add_valuetype (AST_ValueType *t)
{
  return
    this->fe_add_full_intf_decl<AST_ValueType> (t);
}

AST_EventType *
AST_Module::fe_add_eventtype (AST_EventType *t)
{
  return
    this->fe_add_full_intf_decl<AST_EventType> (t);
}

AST_Component *
AST_Module::fe_add_component (AST_Component *t)
{
  return
    this->fe_add_full_intf_decl<AST_Component> (t);
}

AST_Connector *
AST_Module::fe_add_connector (AST_Connector *t)
{
  return
    AST_Connector::narrow_from_decl (
      this->fe_add_decl (t));
}

AST_Home *
AST_Module::fe_add_home (AST_Home *t)
{
  return
    AST_Home::narrow_from_decl (
      this->fe_add_decl (t));
}

AST_InterfaceFwd *
AST_Module::fe_add_interface_fwd (AST_InterfaceFwd *t)
{
  return
    this->fe_add_fwd_intf_decl<AST_Interface> (t);
}

AST_ValueTypeFwd *
AST_Module::fe_add_valuetype_fwd (AST_ValueTypeFwd *t)
{
  return
    this->fe_add_fwd_intf_decl<AST_ValueType> (t);
}

AST_EventTypeFwd *
AST_Module::fe_add_eventtype_fwd (AST_EventTypeFwd *t)
{
  return
    this->fe_add_fwd_intf_decl<AST_EventType> (t);
}

AST_ComponentFwd *
AST_Module::fe_add_component_fwd (AST_ComponentFwd *t)
{
  return
    this->fe_add_fwd_intf_decl<AST_Component> (t);
}

AST_Constant *
AST_Module::fe_add_constant (AST_Constant *t)
{
  return
    AST_Constant::narrow_from_decl (
      this->fe_add_decl (t));
}

AST_Exception *
AST_Module::fe_add_exception (AST_Exception *t)
{
  return
    AST_Exception::narrow_from_decl (
      this->fe_add_decl (t));
}

AST_Union *
AST_Module::fe_add_union (AST_Union *t)
{
  return
    AST_Union::narrow_from_decl (
      this->fe_add_full_struct_type (t));
}

AST_UnionFwd *
AST_Module::fe_add_union_fwd (AST_UnionFwd *t)
{
  return
    AST_UnionFwd::narrow_from_decl (
      this->fe_add_fwd_struct_type (t));
}

AST_Structure *
AST_Module::fe_add_structure (AST_Structure *t)
{
  return this->fe_add_full_struct_type (t);
}

AST_StructureFwd *
AST_Module::fe_add_structure_fwd (AST_StructureFwd *t)
{
  return this->fe_add_fwd_struct_type (t);
}

AST_Enum *
AST_Module::fe_add_enum (AST_Enum *t)
{
  return
    AST_Enum::narrow_from_decl (
      this->fe_add_decl (t));
}

// Add an AST_EnumVal node (an enumerator) to this scope.
// This is done to conform to the C++ scoping rules which declare
// enumerators in the enclosing scope (in addition to declaring them
// in the enum itself).
AST_EnumVal *
AST_Module::fe_add_enum_val (AST_EnumVal *t)
{
  return
    AST_EnumVal::narrow_from_decl (
      this->fe_add_decl (t));
}

AST_Typedef *
AST_Module::fe_add_typedef (AST_Typedef *t)
{
  return
    AST_Typedef::narrow_from_decl (
      this->fe_add_ref_decl (t));
}

AST_Native *
AST_Module::fe_add_native (AST_Native *t)
{
  return
    AST_Native::narrow_from_decl (
      this->fe_add_decl (t));
}

AST_PortType *
AST_Module::fe_add_porttype (AST_PortType *t)
{
  return
    AST_PortType::narrow_from_decl (
      this->fe_add_decl (t));
}

IMPL_NARROW_FROM_DECL(AST_Module)
IMPL_NARROW_FROM_SCOPE(AST_Module)
