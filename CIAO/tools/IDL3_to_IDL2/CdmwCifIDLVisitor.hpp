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

#ifndef CDMW_CODE_GENERATOR_CIF_IDL_VISITOR_HPP
#define CDMW_CODE_GENERATOR_CIF_IDL_VISITOR_HPP

#include "basic_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ast_visitor.h"
#include "utl_scoped_name.h"

class UTL_ExceptList;

namespace Cdmw
{
   namespace CodeGenerator
   {
            //    This class generates the appropriate interfaces and local
            //    interfaces for lifecycle and context management
            //    considerations.
            //
      class CdmwCifIDLVisitor : public basic_visitor
      {
         public:

           /**
            * Purpose:
            * <p> Constructor.
            *
            *@param os The FmtOStringStream which will contain the code generated after the AST trip.
            */
    CdmwCifIDLVisitor (void);

            /**
             * Purpose:
             * <p> Destructor.
             */  
    virtual ~CdmwCifIDLVisitor (void);

            /**
             * Purpose:
             * <p> The visit_module operation is used to visit a module node.
             *
             *@param node The module node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_module (AST_Module *node);

            /**
             * Purpose:
             * <p> The visit_interface operation is used to visit an interface node.
             *
             *@param node The interface node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_interface (AST_Interface *node);

            /**
             * Purpose:
             * <p> The visit_interface_fwd operation is used to visit an interface fwd node.
             *
             *@param node The interface fwd node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_interface_fwd (AST_InterfaceFwd *node);

            /**
             * Purpose:
             * <p> The visit_valuebox operation is used to visit a valuebox node.
             *
             *@param node The valuebox node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_valuebox (AST_ValueBox *node);

            /**
             * Purpose:
             * <p> The visit_valuetype operation is used to visit a valuetype node.
             *
             *@param node The valuetype node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_valuetype (AST_ValueType *node);

            /**
             * Purpose:
             * <p> The visit_valuetype_fwd operation is used to visit a valuetype fwd node.
             *
             *@param node The valuetype fwd node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_valuetype_fwd (AST_ValueTypeFwd *node);

           /**
             * Purpose:
             * <p> The visit_component operation is used to visit a component node.
             *
             *@param node The component node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_component (AST_Component *node);

            /**
             * Purpose:
             * <p> The visit_component_fwd operation is used to visit a component node.
             *
             *@param node The component fwd node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_component_fwd (AST_ComponentFwd *node);

            /**
             * Purpose:
             * <p> The visit_eventtype operation is used to visit a eventtype node.
             *
             *@param node The eventtype node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_eventtype (AST_EventType *node);

            /**
             * Purpose:
             * <p> The visit_eventtype_fwd operation is used to visit a eventtype fwd node.
             *
             *@param node The eventtype fwd node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_eventtype_fwd (AST_EventTypeFwd *node);

            /**
             * Purpose:
             * <p> The visit_home operation is used to visit a home node.
             *
             *@param node The home node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_home (AST_Home *node);

            /**
             * Purpose:
             * <p> The visit_factory operation is used to visit a factory construct node.
             *
             *@param node The factory construct node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_factory (AST_Factory *node);

            /**
             * Purpose:
             * <p> The visit_structure operation is used to visit a structure node.
             *
             *@param node The structure node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_structure (AST_Structure *node);

           /**
             * Purpose:
             * <p> The visit_structure_fwd operation is used to visit a structure fwd node
             *
             *@param node The structure fwd node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_structure_fwd (AST_StructureFwd *node);

           /**
             * Purpose:
             * <p> The visit_exception operation is used to visit an exception  node.
             *
             *@param node The exception node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_exception (AST_Exception *node);

            /**
             * Purpose:
             * <p> The visit_expression operation is used to visit an expression node.
             *
             *@param node The expression node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_expression (AST_Expression *node);

            /**
             * Purpose:
             * <p> The visit_enum operation is used to visit an enum node.
             *
             *@param node The enum node.
             *@return The operation statusn, zero if the operation is successful.
             *
             */
    virtual int visit_enum (AST_Enum *node);

            /**
             * Purpose:
             * <p> The visit_union operation is used to visit an union node.
             *
             *@param node The union node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_union (AST_Union *node);

            /**
             * Purpose:
             * <p> The visit_union_fwd operation is used to visit an union fwd node.
             *
             *@param node The union fwd node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_union_fwd (AST_UnionFwd *node);

            /**
             * Purpose:
             * <p> The visit_union_branch operation is used to visit an union branch node
             *
             *@param node The union branch node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_union_branch (AST_UnionBranch *node);

            /**
             * Purpose:
             * <p> The visit_union_label operation is used to visit an union label node.
             *
             *@param node The union label node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_union_label (AST_UnionLabel *node);

            /**
             * Purpose: <p> The visit_constant operation is used to visit
             * a constant fwd node.
             *
             *@param node The constant node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_constant (AST_Constant *node);

            /**
             * Purpose: <p> The visit_enum_val operation is used to visit
             * an enum val node.
             *
             *@param node The enum val node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_enum_val (AST_EnumVal *node);

            /**
             * Purpose:
             * <p> The visit_array operation is used to visit an array node.
             *
             *@param node The array node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_array (AST_Array *node);

            /**
             * Purpose:
             * <p> The visit_sequence operation is used to visit a sequence node.
             *
             *@parIdl3am node The sequence node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_sequence (AST_Sequence *node);

             /**
              * Purpose: <p> The visit_root operation is used to visit
              * a root node
              *
              *@param node The root node.
              *@return The operation status, zero if the operation is successful.
              *
              */
    virtual int visit_root (AST_Root *node);

            /**
             * Purpose: <p> The visit_native operation is used to visit
             * a native node
             *
             *@param node The native node.
             *@return The operation status, zero if the operation is successful.
             *
             */
    virtual int visit_native (AST_Native *node);

            // local IDL3.
    bool localIdl3;
    
    virtual bool scope_skip_type (AST_Decl *d);
      };
   } // End of CodeGenerator
} // End of Cdmw

#endif // CDMW_CODE_GENERATOR_CIF_IDL_VISITOR_HPP
