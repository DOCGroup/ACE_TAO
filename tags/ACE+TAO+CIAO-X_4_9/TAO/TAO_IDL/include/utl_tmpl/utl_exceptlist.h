// $Id$

/*
 *======================================================================
 *
 *Copyright 1992 Sun Microsystems, Inc.
 *
 *The Interface Definition Language Compiler Front End (CFE) is made
 *available for use provided that this legend is included on all media and
 *documentation and as a part of the software program in whole or part.
 *Users may copy and extend functionality (but may not remove
 *functionality) of the Interface Definition Language CFE without charge,
 *but are not authorized to license or distribute it to anyone else except
 *as part of a product or program developed by the user or with the express
 *written consent of Sun Microsystems, Inc.
 *
 *The names of Sun Microsystems, Inc. and any of its subsidiaries may not
 *be used in advertising or publicity pertaining to distribution of
 *Interface Definition Language CFE as permitted herein.
 *
 *The Interface Definition Language CFE may not be exported outside the
 *United States without first obtaining the appropriate government
 *approvals.
 *
 *INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES
 *OF ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND
 *FITNESS FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A
 *COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 *Interface Definition Language CFE is provided with no support and
 *without any obligation on the part of Sun Microsystems, Inc. or any of
 *its subsidiaries or affiliates to assist in its use, correction,
 *modification or enhancement.
 *
 *SUN MICROSYSTEMS, INC. OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL
 *HAVE NO LIABILITY WITH RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE
 *SECRETS OR ANY PATENTS BY INTERFACE DEFINITION LANGUAGE CFE OR ANY PART
 *THEREOF.
 *
 *IN NO EVENT WILL SUN MICROSYSTEMS, INC. OR ANY OF ITS SUBSIDIARIES OR
 *AFFILIATES BE LIABLE FOR ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL,
 *INDIRECT AND CONSEQUENTIAL DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE
 *POSSIBILITY OF SUCH DAMAGES.
 *
 *SunSoft, Inc.
 *2550 Garcia Avenue
 *Mountain View, California  94043
 *
 *
 *
 *
 *======================================================================
 */

#ifndef _UTL_EXCEPTLIST_UTL_EXCEPTLIST_HH
#define _UTL_EXCEPTLIST_UTL_EXCEPTLIST_HH

// utl_exceptlist.hh
//
// List of AST_Exception nodes

/*
** DEPENDENCIES: utl_list.hh, ast_exception.hh
**
** USE: Included from util.hh
*/

#include        "ace/stdcpp.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include        "idl_fwd.h"
#include        "utl_list.h"
#include        "utl_strlist.h"
#include        "utl_idlist.h"
#include        "utl_scoped_name.h"
#include        "utl_identifier.h"
#include        "utl_string.h"
#include        "ast_expression.h"
#include        "idl_bool.h"
#include        "utl_scope.h"
#include        "ast_decl.h"
#include        "ast_type.h"
#include        "ast_concrete_type.h"
#include        "ast_structure.h"
#include        "ast_exception.h"


class   UTL_ExceptList : public UTL_List<UTL_ExceptList, AST_Exception>
{
public:
  // Operations

  // Constructor(s)
  UTL_ExceptList(AST_Exception *s, UTL_ExceptList *cdr);
  virtual ~UTL_ExceptList() {}
};

// Active iterator for UTL_ExceptList
class   UTL_ExceptlistActiveIterator :
    public UTL_ListActiveIterator<UTL_ExceptList, AST_Exception>
{
public:
  // Operations

  // Constructor(s)
  UTL_ExceptlistActiveIterator(UTL_ExceptList *s);
  virtual ~UTL_ExceptlistActiveIterator() {}
};

#endif           // _UTL_EXCEPTLIST_UTL_EXCEPTLIST_HH
