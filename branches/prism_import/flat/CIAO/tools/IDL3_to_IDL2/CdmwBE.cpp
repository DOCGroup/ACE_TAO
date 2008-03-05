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

#include "CdmwBEExtern.hpp"
#include "InternalIDLVisitor.hpp"
#include "CdmwCifIDLVisitor.hpp"
#include "Config.hpp"

#include "checking_visitor.h"
#include "idl3_to_idl2_visitor.h"

#include "be_extern.h"
#include "be_helper.h"
#include "global_extern.h"
#include "ast_root.h"
#include "utl_string.h"


void 
CdmwBE_version (void)
{
  COG_INFO ("Cdmw_idl3_to_idl2, version " << Cdmw::CodeGenerator::BE_VERSION);
}

int
CdmwBE_init (int & /* argc */, char * /* argv */ [])
{
  // Initialize CARDAMOM BE global data object.
  ACE_NEW_RETURN (cdmw_be_global,
                  CdmwBEGlobalData,
                  -1);
                  
  // Set the base class pointer as well, so base class backend
  // internal calls can reference it.
  be_global = cdmw_be_global;

  idl_global->pass_orb_idl (true);
  return 0;
}

void
CdmwBE_post_init (const char * /* files */ [], long /* nfiles */)
{
}

// Clean up before exit, whether successful or not.
// Need not be exported since it is called only from this file.
void
CdmwBE_cleanup (void)
{
  idl_global->destroy ();
}

// Abort this run of the BE.
void
CdmwBE_abort (void)
{
   COG_FATAL("Fatal Error - Aborting\n");

   CdmwBE_cleanup ();

   ACE_OS::exit (1);
}

// Do the work of this BE. This is the starting point for code generation.
void
CdmwBE_produce (void)
{
   // Get the root node.
   AST_Decl *d = idl_global->root ();
   AST_Root *ast_root = AST_Root::narrow_from_decl (d);

   if (ast_root == 0)
   {
      COG_FATAL("No Root\n");
      CdmwBE_abort ();
   }
  
   // Check whether it is an IDL3 file.
   checking_visitor visitor0;
   
   if (visitor0.visit_root (ast_root) == -1)
   {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%N:%l) CdmwBE_produce -")
                  ACE_TEXT (" failed to accept visitor\n")));
   }
   
   // 1st: Generate Equivalent IDL.
   if (cdmw_be_global->gen_equivalent_idl ())
   {
     if (visitor0.is_idl3 ())
     {
       idl3_to_idl2_visitor visitor1;

       if (visitor1.visit_root (ast_root) == -1)
       { 
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%N:%l) CdmwBE_produce - failed to ")
                      ACE_TEXT ("accept equivalent IDL visitor\n")));
       }
     }
     else
     {
       TAO_OutStream *os = 0;
       int status = be_global->outfile_init (os,
                                             "",
                                             "_IDL2.idl",
                                             "_TAO_IDL_",
                                             "_IDL_");
      
       if (status == -1)
         {
           ACE_ERROR ((LM_ERROR,
                       ACE_TEXT ("(%N:%l) BE_produce - ")
                       ACE_TEXT ("failed to initialize output file\n")));

           delete os;
           os = 0;
           BE_abort ();
         }
        
       *os << be_nl << be_nl
           << "#include \"Components.idl\"" << be_nl
           << "#include \""
           << idl_global->stripped_filename ()->get_string ()
           << "\"" << be_nl << be_nl
           << "#endif /* ifndef */" << be_nl;

       delete os;
       os = 0;
     }
   }

   // 2nd: Generate Internal IDL.
   if (cdmw_be_global->gen_internal_idl ())
   {
     Cdmw::CodeGenerator::InternalIDLVisitor visitor2;
     visitor2.localIdl3 = visitor0.is_local_idl3 ();

     if (visitor2.visit_root (ast_root) == -1)
     { 
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%N:%l) CdmwBE_produce - failed to ")
                    ACE_TEXT ("accept internal IDL visitor\n")));
     }
   }

   // 3rd: Generate CDMW CIF IDL.
   if (cdmw_be_global->gen_cdmw_cif_idl ())
   {
      Cdmw::CodeGenerator::CdmwCifIDLVisitor visitor3;
      visitor3.localIdl3 = visitor0.is_local_idl3 ();

      if (visitor3.visit_root (ast_root) == -1)
      {
         ACE_ERROR ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) CdmwBE_produce - failed to ")
                     ACE_TEXT ("accept CdmwCif visitor\n")));
      }
   }

   // Clean up.
   CdmwBE_cleanup ();
}

