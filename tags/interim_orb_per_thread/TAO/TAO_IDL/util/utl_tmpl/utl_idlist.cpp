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

#pragma ident "%@(#)utl_idlist.cc	1.34% %92/06/10% Sun Microsystems"

// utl_idlist.cc
//
// Implementation of a list of utl_string nodes

#include	<idl.hh>
#include	<idl_extern.hh>

/*
 * Constructor(s)
 */

UTL_IdList::UTL_IdList(Identifier *s, UTL_IdList *cdr)
	  : UTL_List<UTL_IdList, Identifier>(s, cdr)
{
}

/*
 * Private operations
 */

/*
 * Public operations
 */

// Get last item of this list
Identifier *
UTL_IdList::last_component()
{
  if (tail() == NULL)
    return head();
  return tail()->last_component();
}

// AST Dumping
void
UTL_IdList::dump(ostream &o)
{
  UTL_IdListActiveIterator	*i = new UTL_IdListActiveIterator(this);
  long				first = I_TRUE;
  long				second = I_FALSE;

  while (!(i->is_done())) {
    if (!first)
      o << "::";
    else if (second)
      first = second = I_FALSE;
    i->item()->dump(o);
    if (first) {
      if (strcmp(i->item()->get_string(), "::") != 0)
	first = I_FALSE;
      else
	second = I_TRUE;
    }
    i->next();
  }
}

/*
 * Redefinition of inherited virtual operations
 */

// UTL_IdList active iterator

/*
 * Constructor
 */

UTL_IdListActiveIterator::UTL_IdListActiveIterator(UTL_IdList *s)
			: UTL_ListActiveIterator<UTL_IdList, Identifier>(s)
{
}

/*
 * Private operations
 */

/*
 * Public operations
 */

/*
 * Redefinition of inherited virtual operations
 */
