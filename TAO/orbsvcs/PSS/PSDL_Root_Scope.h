/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Root_Scope
//
// = DESCRIPTION
//    Derived class from PSDL_Scope. This is for the Root Scope.
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ============================================================================

#ifndef TAO_PSDL_ROOT_SCOPE_H
#define TAO_PSDL_ROOT_SCOPE_H

#include /**/ "ace/pre.h"

#include "PSDL_Scope.h"
#include "PSDL_Module_Scope.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_PSDL_Export TAO_PSDL_Root_Scope : public TAO_PSDL_Scope
{
public:

  TAO_PSDL_Root_Scope (void);

  ~TAO_PSDL_Root_Scope (void);

  /// Methods to be able to add the respective types to the Root
  /// Scope.
  int add_module (ACE_CString identifier);

  int add_interface (ACE_CString identifier);

  int add_struct (ACE_CString identifier);

  int add_typedef (ACE_CString identifier,
                   ACE_CString identifier_type);

  int add_const_decl (ACE_CString identifier,
                      ACE_CString identifier_type);

  int add_except_decl (ACE_CString identifier,
                       ACE_CString identifier_type);

  int add_op_dcl (ACE_CString identifier);

  void dump (CORBA::ULong depth);

  int find (const ACE_CString &identifier_name,
            ACE_CString &identifier_type);

  int find (const ACE_CString &identifier_name);

  int get_module_name (const ACE_CString &identifier_name,
                       ACE_CString &module_name);

  int get_interface_name (const ACE_CString &identifier_name,
                          ACE_CString &interface_name);

  /// Public methods to return the scopes needed.
  TAO_PSDL_Scope *parent_scope (void);

  Scope_Map *scope_map (void);

protected:

  Scope_Map root_scope_map_;
};

#include /**/ "ace/post.h"

#endif /* TAO_PSDL_ROOT_SCOPE_H */
