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

#ifndef _UTL_LABELLIST_UTL_LABELLIST_HH
#define _UTL_LABELLIST_UTL_LABELLIST_HH

#pragma ident "%@(#)utl_labellist.hh	1.27% %92/06/10% Sun Microsystems"

// utl_labellist.hh
//
// List of AST_UnionBranch nodes

/*
** DEPENDENCIES: utl_list.hh, ast_union_label.hh
**
** USE: Included from util.hh
*/

#include	<iostream.h>
#include	"idl_fwd.h"
#include	"utl_list.h"
#include	"ast_union_label.h"


class	UTL_LabelList : public UTL_List<UTL_LabelList, AST_UnionLabel>
{
public:
  // Operations

  // Constructor(s)
  UTL_LabelList(AST_UnionLabel *s, UTL_LabelList *cdr);
  virtual ~UTL_LabelList() {}
};

// Active iterator for UTL_LabelList
class	UTL_LabellistActiveIterator :
    public UTL_ListActiveIterator<UTL_LabelList, AST_UnionLabel>
{
public:
  // Operations

  // Constructor(s)
  UTL_LabellistActiveIterator(UTL_LabelList *source);
  virtual ~UTL_LabellistActiveIterator() {}
};

#endif           // _UTL_LABELLIST_UTL_LABELLIST_HH
