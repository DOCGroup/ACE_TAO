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

#ifndef _UTL_LIST_UTL_LIST_HH
#define _UTL_LIST_UTL_LIST_HH

#pragma ident "%@(#)utl_list.hh	1.33% %92/06/10% Sun Microsystems"

// utl_list.hh
//
// General single-linked lists

/*
** DEPENDENCIES: NONE
**
** USE: Included from util.hh
*/

// Forward declare active iterator for UTL_List
template<class L, class T> class UTL_ListActiveIterator;

template<class L, class T>
class	UTL_List
{
public:
  // Operations

  // Constructor(s)
  UTL_List(T *c, L *l);
  virtual ~UTL_List() {}

  // Smash last cdr pointer in "this" with l
  void			nconc(L *l);

  // Copy the list starting at "this"
  L			*copy();

  // Get list element
  T			*head();

  // Get next list
  L			*tail();

  // Set the list element
  void			set_head(T *t);

  // Set next list
  void			set_tail(L *l);

  // How long is this list?
  long			length();

private:
  // Data
  T			*pd_car_data;	// The next list
  L			*pd_cdr_data;	// The next list

  // Operations
  long			list_length(long n); // How long is it?

  // Friend class
  friend class		UTL_ListActiveIterator<L, T>; // Friend class

};

// Active iterator for lists
template<class L, class T>
class	UTL_ListActiveIterator
{
public:
  // Operations

  // Constructor(s)
  UTL_ListActiveIterator(UTL_List<L, T> *s);	
  virtual ~UTL_ListActiveIterator() {}

  // Get current item
  T			*item();

  // Get next list
  virtual void		next();

  // Are we at the end of this list?
  virtual long		is_done();

protected:
  // Data
  UTL_List<L, T>		*source;		// On what to iterate?
};

#endif           // _UTL_LIST_UTL_LIST_HH
