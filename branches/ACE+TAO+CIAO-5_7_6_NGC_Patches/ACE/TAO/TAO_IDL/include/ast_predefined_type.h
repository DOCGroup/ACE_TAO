// This may look like C, but it's really -*- C++ -*-
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

#ifndef _AST_PREDEFINED_TYPE_AST_PREDEFINED_TYPE_HH
#define _AST_PREDEFINED_TYPE_AST_PREDEFINED_TYPE_HH

#include "ast_concrete_type.h"

class ast_visitor;

class TAO_IDL_FE_Export AST_PredefinedType : public virtual AST_ConcreteType
{
public:
  // Enum for all the different predefined types.
  enum PredefinedType
    {
        PT_long         // Predefined type "long"
      , PT_ulong        // Predefined type "unsigned long"
      , PT_longlong     // Predefined type "long long"
      , PT_ulonglong    // Predefined type "unsigned long long"
      , PT_short        // Predefined type "short"
      , PT_ushort       // Predefined type "unsigned short"
      , PT_float        // Predefined type "float"
      , PT_double       // Predefined type "double"
      , PT_longdouble   // Predefined type "long double"
      , PT_char         // Predefined type "char"
      , PT_wchar        // Predefined type "CORBA::WChar"
      , PT_boolean      // Predefined type "boolean"
      , PT_octet        // Predefined type "octet"
      , PT_any          // Predefined type "CORBA::Any"
      , PT_object       // Predefined type "CORBA::Object"
      , PT_value        // Predefined type "CORBA::ValueBase"
      , PT_abstract     // Predefined type "CORBA::AbstractBase"
      , PT_void         // Predefined type "void"
      , PT_pseudo       // Predefined type for pseudo objects
    };

  // Operations.

  // Constructor(s).
  AST_PredefinedType (void);

  AST_PredefinedType (PredefinedType t,
                      UTL_ScopedName *n);

  virtual ~AST_PredefinedType (void);

  // Data Accessors.
  PredefinedType pt (void);

  // Narrowing

  DEF_NARROW_FROM_DECL(AST_PredefinedType);

  // AST Dumping
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

protected:
  virtual int compute_size_type (void);
  // Compute the size type if it is unknown.

private:
  // Data.

  const PredefinedType pd_pt;
  // The specific predefined type.
};

#endif           // _AST_PREDEFINED_TYPE_AST_PREDEFINED_TYPE_HH
