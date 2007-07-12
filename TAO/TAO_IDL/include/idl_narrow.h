// $Id$

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

#ifndef _IDL_NARROW_IDL_NARROW_HH
#define _IDL_NARROW_IDL_NARROW_HH

// idl_narrow.hh
//
// Implement IDL narrowing mechanism. Not intended to be read by humans..
//
// Logic:
//
// The DEF_xxx macros are used in class declarations.
// The IMPL_xxx macros are used in class implementations.
//
// The DEF_xxx macros come in two flavors. The first one, DEF_NARROW_METHODSn,
// defines the narrowing mechanism. It declares a static class function whose
// address is used as the class identifier. Then, it declares a narrow op
// with an implementation suitable for narrowing from exactly 'n' superclasses.
// The names of the superclass types are given as arguments. The second flavor
// implements specific narrowing mechanisms from AST_decl and from UTL_scope.
// These macros are named DEF_NARROW_FROM_DECL and DEF_NARROW_FROM_SCOPE
// respectively.
//
// The IMPL_xxx macros also come in two flavors, corresponding to the two
// flavors of DEF_xxx macros. The IMPL_NARROW_METHODSn macros implement the
// narrowing mechanism for classes which must be narrowed from 'n' ancestors.
// The IMPL_NARROW_FROM_DECL and IMPL_NARROW_FROM_SCOPE macros implement
// the narrowing mechanisms for narrowing from instances of AST_decl and
// UTL_scope.
//
// IMPL_NARROW_METHODSn where n is 0,1,2 and 3 are provided.
// DEF_NARROW_METHODSn where n is 0,1,2 and 3 are provided.

extern char * type_id_to_string (long);

#define DEF_NARROW_METHODS0(TYPE)
#define IMPL_NARROW_METHODS0(TYPE)

#define DEF_NARROW_METHODS1(TYPE,PARENT)
#define IMPL_NARROW_METHODS1(TYPE,PARENT)

#define DEF_NARROW_METHODS2(TYPE,PARENT1,PARENT2)
#define IMPL_NARROW_METHODS2(TYPE,PARENT1,PARENT2)

#define DEF_NARROW_METHODS3(TYPE,PARENT1,PARENT2,PARENT3)
#define IMPL_NARROW_METHODS3(TYPE,PARENT1,PARENT2,PARENT3)

#define DEF_NARROW_METHODS4(TYPE,PARENT1,PARENT2,PARENT3,PARENT4)
#define IMPL_NARROW_METHODS4(TYPE,PARENT1,PARENT2,PARENT3,PARENT4)

#define DEF_NARROW_METHODS5(TYPE,PARENT1,PARENT2,PARENT3,PARENT4,PARENT5)
#define IMPL_NARROW_METHODS5(TYPE,PARENT1,PARENT2,PARENT3,PARENT4,PARENT5)

#define DEF_NARROW_FROM_DECL(TYPE)              \
static  TYPE* narrow_from_decl(AST_Decl *t)

#define IMPL_NARROW_FROM_DECL(TYPE)             \
TYPE *                                          \
TYPE::narrow_from_decl (AST_Decl *t) {          \
  return dynamic_cast<TYPE *> (t);              \
}

#define DEF_NARROW_FROM_SCOPE(TYPE)             \
static TYPE * narrow_from_scope(UTL_Scope *t)

#define IMPL_NARROW_FROM_SCOPE(TYPE)            \
TYPE *                                          \
TYPE::narrow_from_scope (UTL_Scope *t) {        \
  return dynamic_cast<TYPE *> (t);              \
}

#endif  // _IDL_NARROW_IDL_NARROW_HH
