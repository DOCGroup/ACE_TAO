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

#ifndef _AST_DECL_AST_DECL_HH
#define _AST_DECL_AST_DECL_HH

// Rock bottom of AST class hierarchy
//
// This class is inherited by all classes which represent named entities
// in IDL. It implements the line and file recording mechanism and also
// records the type of the node. This may be useful for BEs to be able
// to distinguish the real type of a node given only a superclass.

/*
** DEPENDENCIES: utl_scope.hh, utl_scoped_name.hh, utl_strlist.hh
**
** USE: Included from ast.hh
*/

#include "idl_fwd.h"
#include "idl_narrow.h"
#include "idl_bool.h"
#include "utl_scoped_name.h"
#include "utl_string.h"

// This is for AIX w/IBM C++
class Identifier;

// This class is needed (only for g++) to get around a bug in g++ which
// causes virtual operations to not be looked up correctly if an operation
// is defined in more than one virtual public base class. This class makes
// the hierarchy rooted in a single class, thereby eliminating the situation
// that causes the bug to appear

class TAO_IDL_FE_Export COMMON_Base
{
public:

  COMMON_Base (idl_bool local = I_FALSE,
               idl_bool abstract = I_FALSE);

  virtual idl_bool is_local (void);
  virtual idl_bool is_abstract (void);

  // A no-op, overridden in the child classes.
  virtual void destroy (void);

        // Narrowing.
  DEF_NARROW_METHODS0(COMMON_Base);

protected:
  idl_bool is_local_;
  idl_bool is_abstract_;
};

class TAO_IDL_FE_Export AST_Decl : public virtual COMMON_Base
{
public:
  // Enum defining the different kinds of AST nodes.
  enum NodeType {
        NT_module                   // Denotes a module
      , NT_root                     // Denotes the root of AST
      , NT_interface                // Denotes an interface
      , NT_interface_fwd            // Fwd declared interface
      , NT_const                    // Denotes a constant
      , NT_except                   // Denotes an exception
      , NT_attr                     // Denotes an attribute
      , NT_op                       // Denotes an operation
      , NT_argument                 // Denotes an op. argument
      , NT_union                    // Denotes a union
      , NT_union_branch             // Denotes a union branch
      , NT_struct                   // Denotes a structure
      , NT_field                    // Denotes a field in structure
      , NT_enum                     // Denotes an enumeration
      , NT_enum_val                 // Denotes an enum. value
      , NT_string                   // Denotes an IDL string
      , NT_wstring                  // Denotes an IDL wstring
      , NT_array                    // Denotes an IDL array
      , NT_sequence                 // Denotes an IDL sequence
      , NT_typedef                  // Denotes a typedef
      , NT_pre_defined              // Denotes a predefined type
      , NT_native                   // Denotes a native type
                                    // dependent on the programming
                                    // language
      , NT_factory                  // Denotes a OBV factory construct
  };

  // Operations.

  // Constructor(s).
  AST_Decl (void);

  AST_Decl (NodeType type,
            UTL_ScopedName *n,
            idl_bool anonymous = I_FALSE);

  virtual ~AST_Decl (void);

  // Data Accessors.

  idl_bool imported (void);

  void set_imported (idl_bool is_it);

  idl_bool in_main_file (void);

  void set_in_main_file (idl_bool is_it);

  UTL_Scope *defined_in (void);

  void set_defined_in (UTL_Scope *);

  NodeType node_type (void);

  long line (void);

  void set_line (long l);

  UTL_String *file_name (void);

  void set_file_name (UTL_String *s);

  UTL_ScopedName *name (void);

  UTL_ScopedName *compute_name (const char *prefix,
                                const char *suffix);
  // Variation of the <name>. Computes scoped name string, applying
  // prefix and suffix to the local name component.

  void set_name (UTL_ScopedName *n);

  Identifier *local_name (void);

  Identifier *compute_local_name (const char *prefix,
                                  const char *sufix);
  // Apply prefix and suffix to the local name and return.

  virtual const char *full_name (void);
  // Return the stringified full name.

  const char *repoID (void);
  void repoID (char *value);
  // Accessors for the repository ID.

  const char *prefix (void);
  void prefix (char *value);
  // Accessors for the repository ID prefix.

  const char *version (void);
  void version (char *value);
  // Accessors for the version member.

  idl_bool anonymous (void) const;
  // Are we an anonymous (no repo ID) type?

  // If there is _cxx_ in the beginning, we will remove that and keep
  // a copy of the original name. TAO IDL's front end adds _cxx_
  // prefix to the all the reserved keywords. But when we invoke the
  // operation remotely, we should be sending only the name with out
  // "_cxx_" prefix.
  //

  Identifier *original_local_name (void);
  // Get.

  void original_local_name (Identifier *);
  // Set.

  idl_bool added (void);

  void set_added (idl_bool is_it);

  // Narrowing.
  DEF_NARROW_METHODS0(AST_Decl);
  DEF_NARROW_FROM_DECL(AST_Decl);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Cleanup method.
  virtual void destroy (void);

  // Other operations

  // Return TRUE if "this" has "s" as an ancestor.
  idl_bool has_ancestor (AST_Decl *s);

  // Return TRUE if "this" is a child of "s".
  idl_bool is_child (AST_Decl *s);

protected:
  // These are not private because they're used by
  // be_predefined_type' constructor.

  char *repoID_;
  // Repository ID.

  void compute_repoID (void);
  // Computes the repoID.

  void compute_full_name (void);
  // Computes the stringified scoped name.

private:
  // Data

  idl_bool pd_imported;
  // Imported?

  idl_bool pd_in_main_file;
  // Defined in main file?

  UTL_Scope *pd_defined_in;
  // Scope.

  NodeType pd_node_type;
  // What kind of node.

  long pd_line;
  // Line defined in.

  UTL_String *pd_file_name;
  // What file defined in.

  UTL_ScopedName *pd_name;
  // As given.

  Identifier *pd_local_name;
  // Name in scope.

  Identifier *pd_original_local_name;
  // _cxx_ removed if any.

  idl_bool pd_added;
  // Already added.

  char *full_name_;
  // Our full scoped name.

  char *prefix_;
  // The repository ID prefix.

  char *version_;
  // Set by #pragma version.

  idl_bool anonymous_;
  // Are we an anonymous (no repo ID) type?

  // Operations

  // Compute the full name of an AST node.
  void compute_full_name (UTL_ScopedName *n);
};

#endif           // _AST_DECL_AST_DECL_HH
