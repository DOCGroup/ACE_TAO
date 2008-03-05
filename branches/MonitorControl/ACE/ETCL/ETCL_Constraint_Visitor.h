// -*- C++ -*-
//=============================================================================
/**
 * @file ETCL_Constraint_Visitor.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@cs.wustl.edu>
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef ETCL_CONSTRAINT_VISITOR_H
#define ETCL_CONSTRAINT_VISITOR_H

#include /**/ "ace/pre.h"

#include "etcl_export.h"
#include "ETCL_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Versioned_Namespace.h"

BEGIN_VERSIONED_NAMESPACE_MACRO

class ETCL_Literal_Constraint;
class ETCL_Identifier;
class ETCL_Union_Value;
class ETCL_Union_Pos;
class ETCL_Component_Pos;
class ETCL_Component_Assoc;
class ETCL_Component_Array;
class ETCL_Special;
class ETCL_Component;
class ETCL_Dot;
class ETCL_Eval;
class ETCL_Default;
class ETCL_Exist;
class ETCL_Unary_Expr;
class ETCL_Binary_Expr;
class ETCL_Preference;

class ETCL_EXPORT_MACRO ETCL_Constraint_Visitor
{
public:
  virtual ~ETCL_Constraint_Visitor (void);

  virtual int visit_literal (ETCL_Literal_Constraint *) = 0;
  virtual int visit_identifier (ETCL_Identifier *) = 0;
  virtual int visit_union_value (ETCL_Union_Value *) = 0;
  virtual int visit_union_pos (ETCL_Union_Pos *) = 0;
  virtual int visit_component_pos (ETCL_Component_Pos *) = 0;
  virtual int visit_component_assoc (ETCL_Component_Assoc *) = 0;
  virtual int visit_component_array (ETCL_Component_Array *) = 0;
  virtual int visit_special (ETCL_Special *) = 0;
  virtual int visit_component (ETCL_Component *) = 0;
  virtual int visit_dot (ETCL_Dot *) = 0;
  virtual int visit_eval (ETCL_Eval *) = 0;
  virtual int visit_default (ETCL_Default *) = 0;
  virtual int visit_exist (ETCL_Exist *) = 0;
  virtual int visit_unary_expr (ETCL_Unary_Expr *) = 0;
  virtual int visit_binary_expr (ETCL_Binary_Expr *) = 0;
  virtual int visit_preference (ETCL_Preference *) = 0;
};

END_VERSIONED_NAMESPACE_MACRO

#include /**/ "ace/post.h"

#endif // ETCL_CONSTRAINT_VISITOR_H
