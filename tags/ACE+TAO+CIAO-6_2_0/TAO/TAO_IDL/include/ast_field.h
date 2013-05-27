// -*- C++ -*-
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

#ifndef _AST_FIELD_AST_FIELD_HH
#define _AST_FIELD_AST_FIELD_HH

// Representation of a generic field.

// Used as member in structures, exceptions and unions, and as a base
// type for attributes and arguments to operations
//
// NOTE: There are two constructors here, the first is used when field
// is used directly inside a structure, the second when it is used as
// the base type for attributes and operations.

#include "ast_decl.h"
#include "utl_scoped_name.h"

class AST_Type;

class TAO_IDL_FE_Export AST_Field : public virtual AST_Decl
{
public:
  enum Visibility
    {
      vis_NA,
      vis_PUBLIC,
      vis_PRIVATE
    };

  AST_Field (AST_Type *field_type,
             UTL_ScopedName *n,
             Visibility vis = vis_NA);

  AST_Field (AST_Decl::NodeType nt,
             AST_Type *field_type,
             UTL_ScopedName *n,
             Visibility vis = vis_NA);

  virtual ~AST_Field (void);

  // Data Accessors.
  AST_Type *field_type (void) const;
  Visibility visibility (void) const;

  // Are we or do we contain a wstring?
  virtual int contains_wstring (void);

  // Determine this bit of state after we have
  // be added to our aggregate type and before
  // we are destroyed.
  void set_recursive_flag (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (AST_Field);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

  static AST_Decl::NodeType const NT;

protected:
  // Data.

  AST_Type *ref_type_;
  // Base type for field.

  Visibility visibility_;
  // Used with valuetype and eventtype fields.

  bool owns_base_type_;
  // If our field type is anonymous array or sequence, we're
  // responsible for destroying it.
};

#endif           // _AST_FIELD_AST_FIELD_HH
