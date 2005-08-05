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

#ifndef _UTL_NAMELIST_UTL_NAMELIST_HH
#define _UTL_NAMELIST_UTL_NAMELIST_HH

// utl_namelist.hh
//
// List of UTL_ScopedName nodes

/*
** DEPENDENCIES: utl_list.hh, utl_scoped_name.hh
**
** USE: Included from util.hh
*/

#include        "ace/stdcpp.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include        "idl_fwd.h"
#include        "utl_list.h"
#include        "utl_idlist.h"
#include        "utl_scoped_name.h"
#include        "utl_identifier.h"
#include        "utl_string.h"

class   UTL_NameList : public UTL_List<UTL_NameList, UTL_ScopedName>
{
public:
  // Operations

  // Constructor(s)
  UTL_NameList(UTL_ScopedName *s, UTL_NameList *cdr);
  virtual ~UTL_NameList() {}
};

// Active iterator for UTL_NameList
class   UTL_NamelistActiveIterator :
    public UTL_ListActiveIterator<UTL_NameList, UTL_ScopedName>
{
public:
  // Operations

  // Constructor(s)
  UTL_NamelistActiveIterator(UTL_NameList *source);
  virtual ~UTL_NamelistActiveIterator() {}
};

#endif           // _UTL_NAMELIST_UTL_NAMELIST_HH
