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

#include "CdmwCifIDLVisitor.hpp"
#include "CdmwBEExtern.hpp"

#include "identifier_helper.h"
#include "be_sunsoft.h"
#include "be_extern.h"

#include "ast_component_fwd.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_home.h"
#include "ast_root.h"
#include "utl_string.h"
#include "utl_identifier.h"
#include "global_extern.h"
#include "nr_extern.h"

namespace Cdmw
{
   namespace CodeGenerator
   {
      CdmwCifIDLVisitor::CdmwCifIDLVisitor (void)
         : basic_visitor (),
           localIdl3 (false)
      {
      }

      CdmwCifIDLVisitor::~CdmwCifIDLVisitor (void)
      {
      }

      int
      CdmwCifIDLVisitor::visit_module (AST_Module *node)
      {
         if (node->imported () || this->can_skip_module (node))
         {
            return 0;
         }

         *os << be_nl << be_nl; 

         ACE_CString name;
         
         if (node->is_nested())
         {
             name = IdentifierHelper::try_escape (node->original_local_name ());
         }
         else
         {
             name = node->original_local_name()->get_string ();
         }

         if (node->is_nested())
         {
            *os << "module " << name.c_str() << be_nl
                << "{" << be_idt << be_nl;
         }
         else
         {
            *os << "module Cdmw_" << name.c_str() << be_nl
                << "{" << be_idt << be_nl;
         }

         if (this->visit_scope (node) != 0)
         {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "CdmwCifIDLVisitor::visit_module - "
                               "codegen for scope failed\n"),
                              -1);
         }

       *os << be_uidt_nl << be_nl;
       
       if (node->is_nested())
       {
         *os << "}; // end of module " << name.c_str ();
       }
       else
       {
         *os << "}; // end of module Cdmw_" << name.c_str ();
       }
             
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_interface (AST_Interface *node)
      {
         if (node->imported ())
         {
            return 0;
         }

         if (!node->is_local () && !node->is_abstract ())
         {
            *os << be_nl << be_nl
                << "interface "
                << IdentifierHelper::try_escape(node->original_local_name ()).c_str() 
                << " : "
                << IdentifierHelper::orig_sn (node->name ()).c_str ()
                << be_idt_nl
                << ", CosLifeCycle::LifeCycleObject " << be_uidt_nl
                << "{" << be_nl
                << "};";
         }

         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_interface_fwd (AST_InterfaceFwd *node)
      {
         if (node->imported ())
         {
            return 0;
         }

         if (!node->is_local () && !node->is_abstract ())
         {
            *os << be_nl << be_nl
                << "interface "
                << IdentifierHelper::try_escape(node->original_local_name ()).c_str()
                << ";";
         }
         
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_valuebox (AST_ValueBox *)
      {
         // Do not generate
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_valuetype (AST_ValueType *)
      {
         // Do not generate
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_valuetype_fwd (AST_ValueTypeFwd *)
      {
         // Do not generate
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_component (AST_Component *node)
      {
         if (node->imported ())
         {
            return 0;
         }
         
         UTL_Scope * scope = node->defined_in ();
         ACE_CString s_scope;
         
         if (scope != 0)
         {
            AST_Decl *dummy = ScopeAsDecl( scope);
            s_scope += IdentifierHelper::orig_sn (dummy->name ());
         }

         *os << be_nl << be_nl
             << "local interface CCM_" << node->original_local_name ()
             << "_SessionContext : " << s_scope.c_str() << "::CCM_"
             << node->original_local_name () << "_Context," << be_idt_nl
             << "CdmwComponents::SessionContextExt" << be_uidt_nl
             << "{" << be_nl
             << "};" << be_nl << be_nl
             << "interface "
             << IdentifierHelper::try_escape(node->original_local_name ()).c_str ()
             << " : "
             << IdentifierHelper::orig_sn (node->name ()).c_str ()
             << be_nl;

         // Inherit from facet interfaces.
         {
            ACE_Unbounded_Queue<AST_Component::port_description> &s =
               node->provides ();
            AST_Component::port_description *pd = 0;
            
            for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
                 ! iter.done ();
                 iter.advance ())
            {
               iter.next (pd);
               
               *os << ", "
                   << IdentifierHelper::orig_sn (pd->impl->name ()).c_str ()
                   << be_nl;
            }
         }
         
         // Inherit from Consumer interfaces.
         {
            ACE_Unbounded_Queue<AST_Component::port_description> &s =
               node->consumes ();
            AST_Component::port_description *pd = 0;

            for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
                 ! iter.done ();
                 iter.advance ())
            {
               iter.next (pd);
               
               *os << ", "
                   << IdentifierHelper::orig_sn (pd->impl->name (), true).c_str ()
                   << "Consumer" << be_nl;
            }
         }

         *os << "{" << be_nl
             << "};";

         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_component_fwd (AST_ComponentFwd *node)
      {
         if (node->imported ())
         {
            return 0;
         }

         *os << be_nl << be_nl
             << "local interface CCM_" << node->original_local_name ()
             << "_SessionContext;";

         *os << be_nl << be_nl
             << "interface "
             << IdentifierHelper::try_escape(node->original_local_name ()).c_str ()
             << ";";

         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_eventtype (AST_EventType *node)
      {
         if (node->imported ())
         {
            return 0;
         }

         if (this->visit_valuetype (node) != 0)
         {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "CdmwCifIDLVisitor::visit_eventtype - "
                               "codegen for valuetype failed\n"),
                              -1);
         }

         *os << be_nl << be_nl
             << "interface " << node->original_local_name () << "Consumer : "
             << IdentifierHelper::orig_sn (node->name (), true).c_str ()
             << "Consumer," << be_idt_nl 
             << "CosEventComm::PushConsumer," << be_nl
             << "CosLifeCycle::LifeCycleObject" << be_uidt_nl
             << "{" << be_nl
             << "};";

         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_eventtype_fwd (AST_EventTypeFwd *node)
      {
         if (node->imported ())
         {
            return 0;
         }

         if (this->visit_valuetype_fwd (node) != 0)
         {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "CdmwCifIDLVisitor::visit_eventtype_fwd - "
                               "codegen for valuetype_fwd failed\n"),
                              -1);
         }
         
         *os << be_nl << be_nl
             << "interface " << node->original_local_name () << "Consumer;";

         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_home (AST_Home *node)
      {
         if (node->imported ())
         {
            return 0;
         }

         *os << be_nl << be_nl
             << "interface "
             << IdentifierHelper::try_escape(node->original_local_name ()).c_str ()
             << " : "
             << IdentifierHelper::orig_sn (node->name ()).c_str ()
             << "," << be_idt_nl 
             << "Components::HomeConfiguration," << be_nl
             << "CdmwCcmCif::CCMHome," << be_nl
             << "CdmwCcmCif::KeylessCCMHome" << be_uidt_nl
             << "{" << be_nl
             << "};";
            
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_factory (AST_Factory *)
      {
         // Do not generate
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_structure (AST_Structure *)
      {
         // Do not generate
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_structure_fwd (AST_StructureFwd *)
      {
         // Do not generate
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_exception (AST_Exception *)
      {
         // Do not generate
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_expression (AST_Expression *)
      {
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_enum (AST_Enum *)
      {
         // Do not generate
         return 0;
      }


      int
      CdmwCifIDLVisitor::visit_union (AST_Union *)
      {
         // Do not generate
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_union_fwd (AST_UnionFwd *)
      {
         // Do not generate
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_union_branch (AST_UnionBranch *)
      {
         // Do not generate
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_union_label (AST_UnionLabel *)
      {
         // Do not generate
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_constant (AST_Constant *)
      {
         // Do not generate
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_enum_val (AST_EnumVal *)
      {
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_array (AST_Array *)
      {
         // Do not generate
         return 0;
      }

      int
      CdmwCifIDLVisitor::visit_sequence (AST_Sequence *)
      {
         // Do not generate
         return 0;
      }


      int
      CdmwCifIDLVisitor::visit_root (AST_Root *node)
      {
        int status = be_global->outfile_init (this->os,
                                              "Cdmw_",
                                              "_cif.idl",
                                              "_CDMW_IDL_CDMW_",
                                              "_IDL_");
                                                  
        if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("CdmwCifIDLVisitor::visit_root - ")
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
          
          // The included *_cif.idl file will pull these in, no need
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
            if (raw_filename.rfind ('/') == ACE_CString::npos)
            {
              filename =
                raw_filename.substr (0, raw_filename.rfind ('.'))
                + "_cif.idl";

              *os << be_nl
                  << "#include \"Cdmw_" << filename.c_str () << "\"";
            }
            else
            {
              ACE_CString filename_path =
                raw_filename.substr (0, raw_filename.rfind ('/'));
              ACE_CString filename_bis =
                raw_filename.substr (raw_filename.rfind ('/') + 1,
                                     raw_filename.length ());
              filename =
                filename_bis.substr (0, filename_bis.rfind ('.'))
                + "_cif.idl";
                
              *os << be_nl
                  << "#include \"" << filename_path.c_str () << "/Cdmw_"
                  << filename.c_str () << "\"";
            }
          }
        }

        ACE_CString fn (idl_global->stripped_filename ()->get_string ());
        fn = fn.substr (0, fn.rfind ('.'));
        fn += "_cif.idl";

        *os << be_nl << be_nl
            << "#include \"" << fn.c_str () << "\"";
        *os << be_nl
            << "#include \"CdmwCcmCif.idl\"";
        *os << be_nl
            << "#include \"CdmwComponents.idl\"";
             
        *os << be_nl << be_nl
             << "#pragma prefix  \"thalesgroup.com\"";

        if  (this->localIdl3)
        {
          if (this->visit_scope (node) != 0)
          {
             ACE_ERROR_RETURN ((LM_ERROR,
                                "CdmwCifIDLVisitor::visit_root - "
                                "codegen for scope failed\n"),
                               -1);
          }
        }
        
      *os << be_nl << be_nl
          << "#endif /* ifndef */" << be_nl << be_nl;

        return 0;
      }

      int
      CdmwCifIDLVisitor::visit_native (AST_Native *)
      {
         // Do not generate
         return 0;
      }

      bool
      CdmwCifIDLVisitor::scope_skip_type (AST_Decl *d)
      {
        AST_Decl::NodeType nt = d->node_type ();
        return (nt == AST_Decl::NT_pre_defined
                || nt == AST_Decl::NT_typedef);
      }
   } // End of CodeGenerator
} // End of Cdmw
