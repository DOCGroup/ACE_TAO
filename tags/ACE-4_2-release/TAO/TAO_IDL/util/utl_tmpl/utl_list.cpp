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

#pragma ident "%@(#)utl_list.cc	1.34% %92/06/10% Sun Microsystems"

// utl_list.cc
//
// Implementation of generic single-linked lists

#include	<stddef.h>
#include	<utl_list.hh>
#include	<idl_bool.hh>

/*
 * Constructor(s)
 */

template<class L, class T>
UTL_List<L, T>::UTL_List(T *c, L *l)
	: pd_car_data(c),
	  pd_cdr_data(l) 
{
}

/*
 * Private operations
 */

// Compute list length
template<class L, class T>
long
UTL_List<L, T>::list_length(long n)
{
  if (pd_cdr_data == NULL)
    return n;
  else
    return pd_cdr_data->list_length(n+1);
}

/*
 * Public operations
 */

// Smash last cdr with l
template<class L, class T>
void
UTL_List<L, T>::nconc(L *l)
{
  if (pd_cdr_data == NULL)
    pd_cdr_data = l;
  else
    pd_cdr_data->nconc(l);
}

/*
 * Override this operation to copy lists of other types
 */
// Copy a list
template<class L, class T>
L *
UTL_List<L, T>::copy()
{
  if (pd_cdr_data == NULL)
    return new L(head(), NULL);
  return new L(head(), pd_cdr_data->copy());
}

// Get list item
template<class L, class T>
T *
UTL_List<L, T>::head()
{
  return pd_car_data;
}

// Get next list
template<class L, class T>
L *
UTL_List<L, T>::tail()
{
  return pd_cdr_data;
}

// Set list item
template<class L, class T>
void
UTL_List<L, T>::set_head(T *s)
{
  pd_car_data = s;
}

// Set next list
template<class L, class T>
void
UTL_List<L, T>::set_tail(L *l)
{
  pd_cdr_data = l;
}

// Compute list length
template<class L, class T>
long
UTL_List<L, T>::length()
{
  return list_length(1);
}

/*
 * Redefinition of inherited virtual operations
 */

// UTL_List active iterator

/*
 * Constructor
 */

template<class L, class T>
UTL_ListActiveIterator<L, T>::UTL_ListActiveIterator(UTL_List<L, T> *s)
		      : source(s)
{
}

/*
 * Private operations
 */

/*
 * Public operations
 */

// Is iterator done?
template<class L, class T>
long
UTL_ListActiveIterator<L, T>::is_done()
{
  return (source == NULL) ? I_TRUE : I_FALSE;
}

// Get current item
template<class L, class T>
T *
UTL_ListActiveIterator<L, T>::item()
{
  if (source == NULL)
    return NULL;
  return source->head();
}

// Advance to next item
template<class L, class T>
void
UTL_ListActiveIterator<L, T>::next()
{
  if (source != NULL)
    source = source->tail();
}

/*
 * Redefinition of inherited operations
 */

