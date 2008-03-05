/* ===================================================================== */
/*
 * This file is part of CARDAMOM (R) which is jointly developed by THALES
 * and SELEX-SI. It is derivative work based on PERCO Copyright (C) THALES 
 * 2000-2003. All rights reserved.
 * Copyright (C) THALES 2004-2005. All rights reserved.
 * Copyright (C) SELEX-SI and THALES 2006. All rights reserved.
 * 
 * CARDAMOM is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * CARDAMOM is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Library General Public
 * License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with CARDAMOM; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/* ===================================================================== */
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

#include "InternalIDLVisitor.hpp"
#include "CdmwBEExtern.hpp"

#include "identifier_helper.h"
#include "be_sunsoft.h"
#include "be_extern.h"

#include "ast_attribute.h"
#include "ast_component_fwd.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_home.h"
#include "ast_operation.h"
#include "ast_root.h"
#include "utl_string.h"
#include "utl_exceptlist.h"
#include "utl_identifier.h"
#include "global_extern.h"
#include "nr_extern.h"

namespace Cdmw
{
  namespace CodeGenerator
  {
    InternalIDLVisitor::InternalIDLVisitor (void)
      : basic_visitor (),
        localIdl3 (false)
    {
    }

    InternalIDLVisitor::~InternalIDLVisitor (void)
    {
    }

    int
    InternalIDLVisitor::visit_module (AST_Module *node)
    {
      if (node->imported () || this->can_skip_module (node))
      {
        return 0;
      }

      ACE_CString name =
        IdentifierHelper::try_escape (node->original_local_name ());

      *os << be_nl << be_nl
          << "module " << name.c_str () << be_nl
          << "{" << be_idt;

       if (this->visit_scope (node) != 0)
       {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "InternalIDLVisitor::visit_module - "
                             "codegen for scope failed\n"),
                            -1);
       }

       *os << be_uidt_nl
           << "}; // end of module " << name.c_str ();
      
       return 0;
    }

    int
    InternalIDLVisitor::visit_interface (AST_Interface *node)
    {        

       if (node->imported ())
       {
          return 0;
       }

       if (!node->is_local () && !node->is_abstract ())
       {
          *os << be_nl << be_nl
              << "local interface CCM_"
              << node->original_local_name () << " : " 
              << IdentifierHelper::try_escape (node->original_local_name ()).c_str()
              << "{" << be_nl
              << "};";
       }

       return 0;
    }

    int
    InternalIDLVisitor::visit_interface_fwd (AST_InterfaceFwd *node)
    {
       if (node->imported ())
       {
          return 0;
       }
       
       if (!node->is_local () && !node->is_abstract ())
       {
          *os << be_nl << be_nl
              << "local interface CCM_"
              << node->original_local_name () << ";";
       }

       return 0;
    }

    int
    InternalIDLVisitor::visit_valuebox (AST_ValueBox *)
    {
       // Do not generate
       return 0;
    }

    int
    InternalIDLVisitor::visit_valuetype (AST_ValueType *)
    {
       // Do not generate
       return 0;
    }

    int
    InternalIDLVisitor::visit_valuetype_fwd (AST_ValueTypeFwd *)
    {
       // Do not generate
       return 0;
    }

    int
    InternalIDLVisitor::visit_component (AST_Component *node)
    {
       if (node->imported ())
       {
          return 0;
       }

       this->gen_main_executor (node);
       this->gen_monolithic_executor (node);
       this->gen_context (node);

       return 0;
    }

    int
    InternalIDLVisitor::visit_component_fwd (AST_ComponentFwd *node)
    {
       if (node->imported ())
       {
          return 0;
       }
       
       *os << be_nl << be_nl
           << "local interface CCM_"
           << node->original_local_name () << "_Executor;" << be_nl
           << "local interface CCM_"
           << node->original_local_name () << ";" << be_nl
           << "local interface CCM_"
           << node->original_local_name () << "_Context;" << be_nl;

       return 0;
    }

    int
    InternalIDLVisitor::visit_eventtype (AST_EventType *node)
    {
       if (node->imported ())
       {
          return 0;
       }
       
       *os << be_nl << be_nl
          << "local interface CCM_"
          << node->original_local_name () << "Consumer" << be_nl
          << "{" << be_idt_nl
          << "void push (in "
          << IdentifierHelper::try_escape (node->original_local_name ()).c_str()
          << " ev);" << be_uidt_nl
          << "};";
          
       return 0;
    }

    int
    InternalIDLVisitor::visit_eventtype_fwd (AST_EventTypeFwd *node)
    {
       if (node->imported ())
       {
          return 0;
       }
       
       *os << be_nl << be_nl
           << "local interface CCM_" << node->original_local_name ()
           << "Consumer;" << be_nl;

       return 0;
    }

    int
    InternalIDLVisitor::visit_home (AST_Home *node)
    {
       if (node->imported ())
       {
          return 0;
       }

       this->gen_home_implicit_executor (node);
       this->gen_home_explicit_executor (node);
       this->gen_home_main_executor (node);

       return 0;
    }


    int
    InternalIDLVisitor::visit_factory  (AST_Factory *)
    {
       // Do not generate
       return 0;
    }

    int
    InternalIDLVisitor::visit_structure (AST_Structure *)
    {
       // Do not generate
       return 0;
    }

    int
    InternalIDLVisitor::visit_structure_fwd (AST_StructureFwd *)
    {
       // Do not generate
       return 0;
    }

    int
    InternalIDLVisitor::visit_exception (AST_Exception *)
    {
       // Do not generate
       return 0;
    }

    int
    InternalIDLVisitor::visit_expression (AST_Expression *)
    {
       return 0;
    }

    int
    InternalIDLVisitor::visit_enum (AST_Enum *)
    {
       // Do not generate
       return 0;
    }


    int
    InternalIDLVisitor::visit_union (AST_Union *)
    {
       // Do not generate
       return 0;
    }

    int
    InternalIDLVisitor::visit_union_fwd (AST_UnionFwd *)
    {
       // Do not generate
       return 0;
    }

    int
    InternalIDLVisitor::visit_union_branch (AST_UnionBranch *)
    {
       // Do not generate
       return 0;
    }

    int
    InternalIDLVisitor::visit_union_label (AST_UnionLabel *)
    {
       // Do not generate
       return 0;
    }

    int
    InternalIDLVisitor::visit_constant (AST_Constant *)
    {
       // Do not generate
       return 0;
    }

    int
    InternalIDLVisitor::visit_enum_val (AST_EnumVal *)
    {
       return 0;
    }

    int
    InternalIDLVisitor::visit_root (AST_Root *node)
    {
      int status = be_global->outfile_init (this->os,
                                            "",
                                            "_cif.idl",
                                            "_CDMW_IDL_",
                                            "_IDL_");
                                                
      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("InternalIDLVisitor::visit_root - ")
                             ACE_TEXT ("failed to initialize output file\n")),
                            -1);
        }

      ACE_CString filename;

      for (size_t i = 0; i < idl_global->n_included_idl_files (); ++i)
      {
        if (i == 0)
        {
          *os << be_nl;
        }

        ACE_CString raw_filename =
          idl_global->included_idl_files ()[i];
        
        // The included *_IDL2.idl file will pull these in, no need
        // to repeat them.
        if (raw_filename == "orb.idl" || raw_filename == "Components.idl")
        {
          continue;
        }
        
        bool excluded_file_found =
          this->match_excluded_file (raw_filename.c_str ());

        if (raw_filename.find (".pidl") != ACE_CString::npos
            || excluded_file_found)
        {
          filename = raw_filename;
        }
        else
        {
          filename =
            raw_filename.substr (0, raw_filename.rfind ('.')) + "_cif.idl";
        }

        *os << be_nl
            << "#include \"" << filename.c_str () << "\"";
      }

      ACE_CString fn (idl_global->stripped_filename ()->get_string ());
      fn = fn.substr (0, fn.rfind ('.'));
      fn += "_IDL2.idl";

      *os << be_nl
          << "#include \"" << fn.c_str () << "\"";

      const char *pfix = node->prefix ();

      if (ACE_OS::strcmp (pfix, "") != 0)
      {
        *os << be_nl << be_nl
            << "#pragma prefix \"" << pfix << "\"";
      }

      if (this->localIdl3)
      {
        if (this->visit_scope (node) != 0)
        {
           ACE_ERROR_RETURN ((LM_ERROR,
                              "InternalIDLVisitor::visit_root - "
                              "codegen for scope failed\n"),
                             -1);
        }
      }

      *os << be_nl << be_nl
          << "#endif /* ifndef */" << be_nl << be_nl;

      return 0;
    }

    int
    InternalIDLVisitor::visit_native (AST_Native *)
    {
       // Do not generated
       return 0;
    }

    bool
    InternalIDLVisitor::scope_skip_type (AST_Decl *d)
    {
      AST_Decl::NodeType nt = d->node_type ();
      return (nt == AST_Decl::NT_pre_defined
              || nt == AST_Decl::NT_typedef);
    }

    void
    InternalIDLVisitor::gen_context (AST_Component *node)
    {
      AST_Component *base = node->base_component ();

      *os << be_nl << be_nl
          << "local interface CCM_" << node->original_local_name ()
          << "_Context";

      ACE_CString s_base_context;
       
       if (base != 0) 
       {
          UTL_Scope * base_scope = base->defined_in ();
          
          if (base_scope != 0)
          {
             AST_Decl *dummy = ScopeAsDecl(base_scope);
             s_base_context = "::";
             s_base_context += IdentifierHelper::orig_sn (dummy->name());
             s_base_context += "::";
          }
          
          s_base_context += "CCM_";
          s_base_context += base->original_local_name ()->get_string();
          s_base_context += "_Context";
       } 
       else
       {
          s_base_context = "Components::CCMContext";
       }
       
       *os << " : " << s_base_context.c_str()
           << be_nl
           << "{" << be_idt_nl;
          
       // Generate additional operations for receptacles and event sources
       {
          ACE_Unbounded_Queue<AST_Component::port_description> &s =
             node->uses ();
          AST_Component::port_description *pd = 0;

          for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
               ! iter.done ();
               iter.advance ())
          {
             iter.next (pd);

             *os << be_nl << be_nl;
             
             Identifier * id__ =
              IdentifierHelper::original_local_name (pd->id);

             if (pd->is_multiple)
             {
                *os << IdentifierHelper::try_escape (node->original_local_name ()).c_str () 
                    << "::" << id__ << "Connections get_connections_"
                    << id__ << " ();";
             }
             else
             {
                *os << IdentifierHelper::orig_sn (pd->impl->name ()).c_str ()
                    << " get_connection_" << id__ << " ();";
             }
             
             id__->destroy ();
             delete id__;
             id__ = 0;
          }
       }
       // Publishes
       {

          ACE_Unbounded_Queue<AST_Component::port_description> &s =
             node->publishes ();
          AST_Component::port_description *pd = 0;

          for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
               ! iter.done ();
               iter.advance ())
          {
             iter.next (pd);
             Identifier * id__ =
              IdentifierHelper::original_local_name (pd->id);
        
             *os << be_nl << be_nl
                 << "void push_" << id__
                 << " (in "
                 << IdentifierHelper::orig_sn (pd->impl->name ()).c_str ()
                 << " ev);";
              
             id__->destroy ();    
             delete id__;
             id__ = 0;
          }
       }
       // Emits
       {
          ACE_Unbounded_Queue<AST_Component::port_description> &s =
             node->emits ();
          AST_Component::port_description *pd = 0;

          for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
               ! iter.done ();
               iter.advance ())
          {
             iter.next (pd);
             Identifier * id__ =
               IdentifierHelper::original_local_name (pd->id);
  
             *os << be_nl << be_nl
                 << "void push_" << id__
                 << " (in "
                 << IdentifierHelper::orig_sn (pd->impl->name ()).c_str ()
                 << " ev);";
  
             id__->destroy ();
             delete id__;
             id__ = 0;
          }
       }

       *os << be_uidt_nl
           << "};";
    }

    void
    InternalIDLVisitor::gen_main_executor (AST_Component *node)
    {
       AST_Component *base = node->base_component ();

       *os << be_nl << be_nl
           << "local interface CCM_" << node->original_local_name ()
           << "_Executor";

       ACE_CString s_base_context;
       
       if (base != 0) 
       {
          UTL_Scope * base_scope = base->defined_in ();
          
          if (base_scope != 0)
          {
             AST_Decl *dummy = ScopeAsDecl(base_scope);
             s_base_context += IdentifierHelper::orig_sn (dummy->name ()).c_str ();
             s_base_context += "::";
          }
          
          s_base_context += "CCM_";
          s_base_context += base->original_local_name ()->get_string ();
          s_base_context += "_Executor";
       } 
       else
       {
          s_base_context = "Components::EnterpriseComponent";
       }
       
       *os << " : " << s_base_context.c_str();
       
       // Any supported interfaces?         
       long nsupports = node->n_supports ();

       for (long i = 0; i < nsupports; ++i)
       {
          *os << be_nl
              << ", "
              << IdentifierHelper::orig_sn (node->supports ()[i]->name ()).c_str ();
       }
       
       *os << be_nl
           << "{" << be_idt_nl;

       // Generate attributes declared by the component
       {
          // Any attributes and operations?
          for (UTL_ScopeActiveIterator decl_iter (node, UTL_Scope::IK_decls);
               ! decl_iter.is_done ();
               decl_iter.next ())
          {
             AST_Decl *d = decl_iter.item ();
             AST_Attribute * attr = AST_Attribute::narrow_from_decl(d);
             
             if (attr != 0 && d->ast_accept (this) != 0)
             {
                ACE_ERROR ((LM_ERROR,
                            "InternalIDLVisitor::gen_home_explicit_executor - "
                            "codegen for scope failed\n"));
                return;
             }
          }
       }

       *os << be_uidt_nl
           << "};";
    }

    void
    InternalIDLVisitor::gen_monolithic_executor (AST_Component *node)
    {
       *os << be_nl << be_nl
           << "local interface CCM_"
           << node->original_local_name () << " : CCM_"
           << node->original_local_name () << "_Executor" << be_nl
           << "{" << be_idt_nl;
           
       // Facet ports
       {

          ACE_Unbounded_Queue<AST_Component::port_description> &s =
             node->provides ();
          AST_Component::port_description *pd = 0;

          for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
               ! iter.done ();
               iter.advance ())
          {
             iter.next (pd);
             Identifier * id__ =
               IdentifierHelper::original_local_name (pd->id);

             // CCM_<type> get_<name> ();
             
             ACE_CString s_impl;
             UTL_Scope * base_scope = pd->impl->defined_in ();
             
             if (base_scope != 0)
             {
                AST_Decl *dummy = ScopeAsDecl(base_scope);
                s_impl += IdentifierHelper::orig_sn (dummy->name ()).c_str ();
                s_impl += "::";
                s_impl += "CCM_";
                s_impl += pd->impl->original_local_name ()->get_string();
             } 
             else
             {
                s_impl += "CCM_";
                s_impl += pd->impl->original_local_name ()->get_string();
             }

             *os << be_nl << be_nl
                 << s_impl.c_str () << " get_" << id__ << " ();";
                
             id__->destroy ();   
             delete id__;
             id__ = 0;
          }
       }
       // Consumer ports
       {
          ACE_Unbounded_Queue<AST_Component::port_description> &s =
             node->consumes ();
          AST_Component::port_description *pd = 0;
          
          for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
               ! iter.done ();
               iter.advance ())
          {
             iter.next (pd);
             
             Identifier * id__ =
              IdentifierHelper::original_local_name (pd->id);
        
             *os << be_nl << be_nl
                 << "void push_" << id__ << " (in "
                 << IdentifierHelper::orig_sn (pd->impl->name ()).c_str ()
                 << " ev );";
    
             id__->destroy ();
             delete id__;
             id__ = 0;
          }        
       }
       
       *os << be_uidt_nl
           << "};";
    }

    void 
    InternalIDLVisitor::gen_home_implicit_executor (AST_Home *node)
    {
       AST_ValueType *key = node->primary_key ();

       *os << be_nl << be_nl
           << "local interface CCM_"
           << node->original_local_name () << "Implicit" << be_nl
           << "{"<< be_idt_nl;

       if (key != 0)
       {
          ACE_CString key_name =
            IdentifierHelper::orig_sn (key->name ());
       
          *os << "Components::EnterpriseComponent "
              << "create (in " << key_name.c_str () << " key)" << be_idt_nl
              << "raises (Components::CCMException);" << be_uidt_nl
              << "Components::EnterpriseComponent "
              << "find_by_primary_key (in " << key_name.c_str ()
              << " key)" << be_idt_nl
              << "raises (Components::CCMException);" << be_uidt_nl
              << "void remove (in " << key_name.c_str ()
              << " key)" << be_idt_nl
              << "raises (Components::CCMException);" << be_uidt_nl;
       } 
       else
       {
          *os << "Components::EnterpriseComponent create()" << be_idt_nl
              << "raises (Components::CCMException);" << be_uidt_nl;
       }

       *os << be_uidt_nl
           << "};";
    }
    
    void 
    InternalIDLVisitor::gen_home_explicit_executor (AST_Home *node)
    {
       ACE_CString explicit_name ("CCM_");
       explicit_name += node->original_local_name ()->get_string ();
       explicit_name += "Explicit";

       *os << be_nl << be_nl
           << "local interface " << explicit_name.c_str () << " : ";

       AST_Home *base = node->base_home ();

       if (base == 0)
       {
          *os << "Components::HomeExecutorBase";
       }
       else
       {
          ACE_CString s_base_home;
          UTL_Scope * base_scope = base->defined_in ();
          
          if (base_scope != 0)
          {
             AST_Decl *dummy = ScopeAsDecl(base_scope);
             s_base_home += IdentifierHelper::orig_sn (dummy->name ()).c_str ();
             s_base_home += "::";
          }
          
          s_base_home += "CCM_";
          s_base_home += base->original_local_name ()->get_string ();
          s_base_home += "Explicit";
          
          *os << s_base_home.c_str ();
       }

       *os << be_idt_nl;
       
       AST_Interface **parents = node->inherits ();

       for (long i = 0; i < node->n_inherits (); ++i)
       {
          *os << ", "
              << IdentifierHelper::orig_sn (parents[i]->name ()).c_str ()
              << be_nl;
       }
       
       *os << be_uidt_nl
           << "{" << be_idt_nl;

       // Any attributes and operations?
       for (UTL_ScopeActiveIterator decl_iter (node, UTL_Scope::IK_decls);
            ! decl_iter.is_done ();
            decl_iter.next ())
       {
          AST_Decl *d = decl_iter.item ();
          
          // (JP) This is a hack, used only to make a version comparison
          // work down the line when the container's version is fetched.
          // The explicit home node is created in the idl3_to_idl2 visitor
          // and is only/ a temporary one - that visitor wasn't designed
          // for subsequent visitors to use the same AST.
          d->set_defined_in (node);
          
          if (d->ast_accept (this) != 0)
          {
             ACE_ERROR ((
               LM_ERROR,
               ACE_TEXT ("InternalIDLVisitor::gen_home_explicit_executor - ")
               ACE_TEXT ("codegen for scope failed\n")));
               
             return;
          }

       }
       // Generate factory operations
       {
          *os << be_nl;
          
          AST_Operation **item = 0;

          for (ACE_Unbounded_Queue_Iterator<AST_Operation *> i (node->factories ());
               ! i.done ();
               i.advance ())
          {
             i.next (item);

             *os << "Components::EnterpriseComponent "
                 << IdentifierHelper::try_escape((*item)->original_local_name ()).c_str()
                 << " (";
                  
             this->gen_params (*item, (*item)->argument_count ());
             
             *os << ")";

             UTL_ExceptList *exceps = (*item)->exceptions ();

             if (exceps != 0 && exceps->length () > 0)
             {
                this->gen_exception_list (exceps, "", true);
             }
             
             *os << ";" << be_nl;
          }
       }

       // Generate finder operations
       { 
          *os << be_nl;
          
          AST_Operation **item = 0;

          for (ACE_Unbounded_Queue_Iterator<AST_Operation *> i (node->finders ());
               ! i.done ();
               i.advance ())
          {
             i.next (item);

             *os << "Components::EnterpriseComponent "
                  << IdentifierHelper::try_escape((*item)->original_local_name ()).c_str()
                  << " (";
                  
             this->gen_params (*item, (*item)->argument_count ());
             
             *os << ")";

             UTL_ExceptList *exceps = (*item)->exceptions ();

             if (exceps != 0 && exceps->length () > 0)
             {
                this->gen_exception_list (exceps, "", true);
             }
             
             *os << ";";
          }
       }
       
       *os << be_uidt_nl
           << "};";
    }
    
    void 
    InternalIDLVisitor::gen_home_main_executor (AST_Home *node)
    {
       *os << be_nl << be_nl
           << "local interface CCM_"
           << node->original_local_name () << " : " << be_idt_nl
           << "CCM_" << node->original_local_name ()
           << "Explicit," << be_nl
           << "CCM_" << node->original_local_name ()
           << "Implicit" << be_uidt_nl
           << "{" << be_nl
           << "};";
    }
  } // End of CodeGenerator
} // End of Cdmw
