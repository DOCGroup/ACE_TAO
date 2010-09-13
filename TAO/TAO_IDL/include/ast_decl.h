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

#include "utl_scoped_name.h"
#include "idl_narrow.h"

#include "ace/os_include/sys/os_types.h"
#include "ace/SString.h"

// This is for AIX w/IBM C++
class Identifier;

class UTL_Scope;
class UTL_String;
class ast_visitor;

// This class is needed (only for g++) to get around a bug in g++ which
// causes virtual operations to not be looked up correctly if an operation
// is defined in more than one virtual public base class. This class makes
// the hierarchy rooted in a single class, thereby eliminating the situation
// that causes the bug to appear

class TAO_IDL_FE_Export COMMON_Base
{
protected:
  COMMON_Base (bool local = false,
               bool abstract = false);

  virtual ~COMMON_Base (void);

public:
  // A no-op, overridden in the child classes.
  virtual void destroy (void);

  // Accessor needs to get overridden for a few types.
  virtual bool is_local (void);
  void is_local (bool val);

  bool is_abstract (void) const;
  void is_abstract (bool val);

protected:
  bool is_local_;
  bool is_abstract_;
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
      , NT_valuetype                // Denotes a valuetype
      , NT_valuetype_fwd            // Fwd declared valuetype
      , NT_const                    // Denotes a constant
      , NT_except                   // Denotes an exception
      , NT_attr                     // Denotes an attribute
      , NT_op                       // Denotes an operation
      , NT_argument                 // Denotes an op. argument
      , NT_union                    // Denotes a union
      , NT_union_fwd                // Fwd declared union
      , NT_union_branch             // Denotes a union branch
      , NT_struct                   // Denotes a structure
      , NT_struct_fwd               // Fwd declared struct
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
      , NT_factory                  // Denotes a OBV or home factory construct
      , NT_finder                   // Denotes a home finder construct
      , NT_component                // Denotes a CORBA component
      , NT_component_fwd            // Denotes a forward declared component
      , NT_home                     // Denotes a CORBA component home
      , NT_eventtype                // Denotes a CCM event source or sink
      , NT_eventtype_fwd            // Denotes a forward declared CCM event
      , NT_valuebox                 // Denotes a value box
      , NT_type                     // Template interface parameter
      , NT_fixed                    // Denotes (unsupported) fixed type
      , NT_porttype                 // Denotes a port type
      , NT_provides                 // Denotes a facet
      , NT_uses                     // Denotes a receptacle
      , NT_publishes                // Denotes an event source
      , NT_emits                    // Denotes a one-to-one event source
      , NT_consumes                 // Denotes an event sink
      , NT_ext_port                 // Denotes an extended port
      , NT_mirror_port              // Denotes a mirror port
      , NT_connector                // Denotes a CCM connector
      , NT_param_holder             // Denotes a template param placeholder
  };

  AST_Decl (NodeType type,
            UTL_ScopedName *n,
            bool anonymous = false);

  virtual ~AST_Decl (void);

  // Cleanup method.
  virtual void destroy (void);

  // If this decl has been found, some types need to be
  // moved onto their true definitions etc. Defaults to
  // NO adjustment.
  virtual AST_Decl *adjust_found (bool ignore_fwd, bool full_def_only);

  // Is this decl a forward declared type (default false)
  virtual bool is_fwd (void);

  // Data Accessors.

  bool imported (void);
  void set_imported (bool is_it);

  bool in_main_file (void);
  void set_in_main_file (bool is_it);

  UTL_Scope *defined_in (void);
  void set_defined_in (UTL_Scope *);

  NodeType node_type (void);

  long line (void);
  void set_line (long l);

  ACE_CString & file_name (void);
  void set_file_name (ACE_CString s);

  UTL_ScopedName *name (void);

  UTL_ScopedName *compute_name (const char *prefix,
                                const char *suffix);
  // Variation of the <name>. Computes scoped name string, applying
  // prefix and suffix to the local name component.

  void set_name (UTL_ScopedName *n);

  Identifier *local_name (void);
  void local_name (Identifier *id);

  Identifier *compute_local_name (const char *prefix,
                                  const char *sufix);
  // Apply prefix and suffix to the local name and return.

  virtual const char *full_name (void);
  // Return the stringified full name.

  virtual const char *flat_name (void);
  // Return the flattened full scoped name.

  const char *repoID (void);
  void repoID (char *value);
  // Accessors for the repository ID.

  const char *prefix (void);
  void prefix (const char *value);
  // Accessors for the repository ID prefix.

  const char *version (void);
  void version (char *value);
  // Accessors for the version_ member.

  bool anonymous (void) const;
  void anonymous (bool val);
  // Accessors for the anonymous_ member.

  bool typeid_set (void) const;
  void typeid_set (bool val);
  // Accessors for the typeid_set_ member.

  void set_id_with_typeid (char *value);
  // Called by a 'typeId' declaration.

  void set_prefix_with_typeprefix (const char *value);
  // Called by a 'type_prefix' declaration.

  // If there is _cxx_ in the beginning, we will remove that and keep
  // a copy of the original name. TAO IDL's front end adds _cxx_
  // prefix to the all the reserved keywords. But when we invoke the
  // operation remotely, we should be sending only the name with out
  // "_cxx_" prefix.
  Identifier *original_local_name (void);
  void original_local_name (Identifier *);

  // To be overridden by the subclasses interface, struct, union, and
  // the corresponding forward declaration classes.
  virtual bool is_defined (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL(AST_Decl);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Other operations

  // Return TRUE if "this" has "s" as an ancestor.
  bool has_ancestor (AST_Decl *s);

  // Return TRUE if "this" is a child of "s".
  bool is_child (AST_Decl *s);

  bool is_nested (void);
  // Determines if we are inside of a nested scope or not.

  UTL_ScopedName *last_referenced_as (void) const;
  void last_referenced_as (UTL_ScopedName *n);

  // Accessors for the prefix_scope_ member.
  UTL_Scope *prefix_scope (void);
  void prefix_scope (UTL_Scope *s);

  // Useful for GIOP to know if a wstring is being marshaled.
  virtual int contains_wstring (void);
  void contains_wstring (int val);

  // Additional checks when we think we have caught an IDL
  // masking scope error in a lookup, which starts simply with
  // a comparison of names in a scope masking queue.
  bool masking_checks (AST_Decl *mod);

protected:
  // These are not private because they're used by
  // be_predefined_type' constructor and can be called
  // from be_decl.

  char *repoID_;
  // Repository ID.

  char *flat_name_;
  // Flattened fully scoped name.

  int contains_wstring_;
  // If we are a scope, do we contain a wstring at some level?

protected:
  void dump_i (ACE_OSTREAM_TYPE &o, const char *s) const ;

  void compute_repoID (void);
  // Computes the repoID.

  void compute_full_name (void);
  // Computes the stringified scoped name.

  void compute_flat_name (void);
  // Compute the flattened fully scoped name.

  const char *node_type_to_string (NodeType nt);
  // Convert a NodeType to a string for dumping.

private:
  // Data

  bool pd_imported;
  // Imported?

  bool pd_in_main_file;
  // Defined in main file?

  UTL_Scope *pd_defined_in;
  // Scope.

  NodeType pd_node_type;
  // What kind of node.

  long pd_line;
  // Line defined in.

  ACE_CString pd_file_name;
  // What file defined in.

  UTL_ScopedName *pd_name;

  Identifier *pd_local_name;
  // Name in scope.

  Identifier *pd_original_local_name;
  // _cxx_ removed if any.

  char *full_name_;
  // Our full scoped name.

  char *prefix_;
  // The repository ID prefix.

  char *version_;
  // Set by #pragma version.

  bool anonymous_;
  // Are we an anonymous (no repo ID) type?

  bool typeid_set_;
  // Has our repo id been set by a typeId declaration?

  UTL_ScopedName *last_referenced_as_;
  // Temporary holder of the most recent way we were reference.
  // The top level component of this is added to pd_name_referenced.

  // The scope in which our prefix, if any, was assigned.
  UTL_Scope *prefix_scope_;

private:
  void compute_full_name (UTL_ScopedName *n);
  // Compute the full name of an AST node.

  void set_prefix_with_typeprefix_r (const char *value,
                                     UTL_Scope *appeared_in);
  // Non-top-level version of set_prefix_with_typeprefix.
};

#endif           // _AST_DECL_AST_DECL_HH
