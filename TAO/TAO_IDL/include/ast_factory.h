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

#ifndef _AST_FACTORY_AST_FACTORY_HH
#define _AST_FACTORY_AST_FACTORY_HH

#include "ast_decl.h"
#include "utl_scope.h"
#include "utl_scoped_name.h"
#include "utl_exceptlist.h"

#include <cstddef>

/// Represents an OBV or component home factory declaration.
class TAO_IDL_FE_Export AST_Factory : public virtual AST_Decl,
                                      public virtual UTL_Scope
{
public:
  AST_Factory () = delete;
  explicit AST_Factory (UTL_ScopedName *n);

  ~AST_Factory () override;

  /// Return the exception list.
  UTL_ExceptList *exceptions () const;

  /// Return the exception count.
  size_t n_exceptions () const;

  /// Return the count of arguments.
  virtual int argument_count ();

  /// Check whether an argument has a native type, for which stub code
  /// should not be generated.
  virtual bool has_native ();

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &o) override;

  // Cleanup function.
  void destroy () override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  // Method to add exceptions.
  UTL_ExceptList *be_add_exceptions (UTL_ExceptList *t);

  static AST_Decl::NodeType const NT;

protected:
  // Data.

  /// Exceptions raised.
  UTL_ExceptList_Owner pd_exceptions;

  /// Exception count.
  size_t pd_n_exceptions {};

  /// Number of arguments.
  int argument_count_ {-1};

  /// Whether any argument has a native type.
  bool has_native_ {};

private:
  // Operations.

  /// Count the number of arguments.
  int compute_argument_attr ();

  // Scope Management Protocol.

  friend class tao_yy::parser;
  AST_Argument *fe_add_argument (AST_Argument *a) override;
  UTL_NameList *fe_add_exceptions (UTL_NameList *e) override;
};

#endif           // _AST_FACTORY_AST_FACTORY_HH
