/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Op_Dcl_Scope
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ============================================================================

#ifndef TAO_PSDL_OP_DCL_SCOPE_H
#define TAO_PSDL_OP_DCL_SCOPE_H

#include "ace/pre.h"

#include "PSDL_Scope.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_PSDL_Export TAO_PSDL_Op_Dcl_Scope : public TAO_PSDL_Scope
{
 public:

  TAO_PSDL_Op_Dcl_Scope (TAO_PSDL_Scope *parent_scope);

  int add_const_decl (ACE_CString identifier,
                      ACE_CString identifier_type);

  int add_member_decl (ACE_CString identifier,
                       ACE_CString identifier_type);

  void dump (CORBA::ULong depth);

  TAO_PSDL_Scope *parent_scope (void);

  Scope_Map *scope_map (void);

protected:

  Scope_Map scope_map_;
  TAO_PSDL_Scope *parent_scope_;
};

#include "ace/post.h"

#endif /* TAO_PSDL_OP_DCL_SCOPE_H */
