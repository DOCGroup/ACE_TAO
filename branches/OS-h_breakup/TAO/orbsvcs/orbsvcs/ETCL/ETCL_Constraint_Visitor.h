/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    orbsvcs/ETCL
//
// = FILENAME
//    ECTL_Constraint_Visitor
//
// = AUTHOR
//    Carlos O'Ryan <coryan@cs.wustl.edu>
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ETCL_CONSTRAINT_VISITOR_H
#define TAO_ETCL_CONSTRAINT_VISITOR_H

#include "etcl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ETCL_Literal_Constraint;
class TAO_ETCL_Identifier;
class TAO_ETCL_Union_Value;
class TAO_ETCL_Union_Pos;
class TAO_ETCL_Component_Pos;
class TAO_ETCL_Component_Assoc;
class TAO_ETCL_Component_Array;
class TAO_ETCL_Special;
class TAO_ETCL_Component;
class TAO_ETCL_Dot;
class TAO_ETCL_Eval;
class TAO_ETCL_Default;
class TAO_ETCL_Exist;
class TAO_ETCL_Unary_Expr;
class TAO_ETCL_Binary_Expr;
class TAO_ETCL_Preference;

class TAO_ETCL_Export TAO_ETCL_Constraint_Visitor
{
public:
  virtual ~TAO_ETCL_Constraint_Visitor (void);
  // Destructor

  virtual int visit_literal (TAO_ETCL_Literal_Constraint *) = 0;
  virtual int visit_identifier (TAO_ETCL_Identifier *) = 0;
  virtual int visit_union_value (TAO_ETCL_Union_Value *) = 0;
  virtual int visit_union_pos (TAO_ETCL_Union_Pos *) = 0;
  virtual int visit_component_pos (TAO_ETCL_Component_Pos *) = 0;
  virtual int visit_component_assoc (TAO_ETCL_Component_Assoc *) = 0;
  virtual int visit_component_array (TAO_ETCL_Component_Array *) = 0;
  virtual int visit_special (TAO_ETCL_Special *) = 0;
  virtual int visit_component (TAO_ETCL_Component *) = 0;
  virtual int visit_dot (TAO_ETCL_Dot *) = 0;
  virtual int visit_eval (TAO_ETCL_Eval *) = 0;
  virtual int visit_default (TAO_ETCL_Default *) = 0;
  virtual int visit_exist (TAO_ETCL_Exist *) = 0;
  virtual int visit_unary_expr (TAO_ETCL_Unary_Expr *) = 0;
  virtual int visit_binary_expr (TAO_ETCL_Binary_Expr *) = 0;
  virtual int visit_preference (TAO_ETCL_Preference *) = 0;
};

#endif /* TAO_ETCL_CONSTRAINT_VISITOR_H */
