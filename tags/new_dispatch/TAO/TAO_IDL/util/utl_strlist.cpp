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

#pragma ident "%@(#)utl_strlist.cc	1.34% %92/06/10% Sun Microsystems"

// utl_strlist.cc
//
// Implementation of a list of utl_string nodes

// NOTE: This list class only works correctly because we use single public
//       inheritance, as opposed to multiple inheritance or public virtual.
//	 It relies on a type-unsafe cast from UTL_List to subclasses, which
//	 will cease to operate correctly if you use either multiple or
//	 public virtual inheritance.
//
//	 For portability reasons we have decided to provide both this and
//	 an implementation of the list classes in terms of templates. If
//	 your compiler supports templates, please use the files in the
//	 include/utl_tmpl and util/utl_tmpl directories instead of the
//	 files by the same names in the include and util directories.

#include	"idl.h"
#include	"idl_extern.h"

/*
 * Constructor(s)
 */

UTL_StrList::UTL_StrList(String *s, UTL_StrList *cdr)
	   : pd_car_data(s),
	     UTL_List(cdr)
{
}

/*
 * Private operations
 */

/*
 * Public operations
 */

// Get list item
String *
UTL_StrList::head()
{
  return pd_car_data;
}

// Set list item
void
UTL_StrList::set_head(String *s)
{
  pd_car_data = s;
}

// Get last item of this list
String *
UTL_StrList::last_component()
{
  if (tail() == NULL)
    return pd_car_data;
  return ((UTL_StrList *) tail())->last_component();
}

// Copy a list
UTL_List *
UTL_StrList::copy()
{
  if (tail() == NULL)
    return new UTL_StrList(head(), NULL);
  return new UTL_StrList(head(), (UTL_StrList *) tail()->copy());
}

// AST Dumping
void
UTL_StrList::dump(ostream &o)
{
  char			      *s;
  UTL_StrlistActiveIterator *i = new UTL_StrlistActiveIterator(this);
  idl_bool		       first = I_TRUE;
  idl_bool		       second = I_FALSE;

  while (!(i->is_done())) {
    if (!first)
      o << "::";
    else if (second)
      first = second = I_FALSE;
    s = i->item()->get_string();
    o << s;
    if (first) {
      if (strcmp(s, "::") != 0)
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

// UTL_StrList active iterator

/*
 * Constructor
 */

UTL_StrlistActiveIterator::UTL_StrlistActiveIterator(UTL_StrList *s)
			   : UTL_ListActiveIterator(s)
{
}

/*
 * Private operations
 */

/*
 * Public operations
 */

// Get current item
String *
UTL_StrlistActiveIterator::item()
{
  if (source == NULL)
    return NULL;
  return ((UTL_StrList *) source)->head();
}

/*
 * Redefinition of inherited virtual operations
 */
