/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Struct_Scope
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ============================================================================

#ifndef TAO_PSDL_STRUCT_SCOPE_H
#define TAO_PSDL_STRUCT_SCOPE_H

#include "ace/pre.h"

#include "PSDL_Scope.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_PSDL_Export TAO_PSDL_Struct_Scope : public TAO_PSDL_Scope
{
 public:

  TAO_PSDL_Struct_Scope (TAO_PSDL_Scope *parent_scope);

  int add_module (ACE_CString identifier);

  int add_struct (ACE_CString identifier);

  int add_typedef (ACE_CString identifier,
                   ACE_CString identifier_type);

  int add_const_decl (ACE_CString identifier,
                      ACE_CString identifier_type);

  int add_except_decl (ACE_CString identifier,
                       ACE_CString identifier_type);

  void dump (CORBA::ULong depth);

  int find (const ACE_CString &identifier_name,
            ACE_CString &identifier_type);

  int find (const ACE_CString &identifier_name);

  TAO_PSDL_Scope *parent_scope (void);

  Scope_Map *scope_map (void);

  ACE_CString identifier_type (void);
  ACE_CString module_name (void);
  ACE_CString interface_name (void);

private:

  Scope_Map struct_scope_;
  TAO_PSDL_Scope *parent_scope_;
  ACE_CString identifier_type_;
  ACE_CString module_name_;
  ACE_CString interface_name_;
};

#include "ace/post.h"

#endif /* TAO_PSDL_STRUCT_SCOPE_H */
