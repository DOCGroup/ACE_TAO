// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_union.h
//
// = DESCRIPTION
//    Extension of class AST_Union that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_UNION_H
#define BE_UNION_H

#include "be_type.h"
#include "be_scope.h"
#include "ast_union.h"

class AST_ConcreteType;
class be_visitor;

class be_union : public virtual AST_Union,
                 public virtual be_scope,
                 public virtual be_type
{
public:
  be_union (void);

  be_union (AST_ConcreteType *dt,
            UTL_ScopedName *n,
            bool local,
            bool abstract);

  virtual void redefine (AST_Structure *from);
  // Catch BE-specific member values before delegating to the base class.

  virtual bool has_duplicate_case_labels (void);
  // Do we have at least one member with multiple case labels?

  virtual void gen_ostream_operator (TAO_OutStream *os);
  // Overridden from class be_type.

  virtual void destroy (void);
  // Cleanup function.

  virtual int accept (be_visitor *visitor);
  // Visiting.

  bool gen_empty_default_label (void);
  // Decides whether a default switch case label in the generated copy
  // constructor, assignment operator, etc. is needed.

  unsigned long nlabels (void);
  // Returns total number of labels, useful when the union has
  // multiple case labels.

  // Narrowing.

  DEF_NARROW_FROM_DECL(be_union);
  DEF_NARROW_FROM_SCOPE(be_union);
};

#endif
