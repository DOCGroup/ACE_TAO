// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_array.h
//
// = DESCRIPTION
//    Extension of class AST_Array that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_ARRAY_H
#define BE_ARRAY_H

#include "be_type.h"
#include "ast_array.h"

class UTL_ExprList;
class TAO_OutStream;
class be_visitor;

class be_array : public virtual AST_Array,
                 public virtual be_type
{
public:
  be_array (void);

  be_array (UTL_ScopedName *n,
            unsigned long ndims,
            UTL_ExprList *dims,
            bool local,
            bool abstract);

  ~be_array (void);

  int gen_dimensions (TAO_OutStream *os,
                      unsigned short slice = 0);
  // Generate dimensions. If slice == 1, generate dimensions for the slice
  // definition.
  
  virtual void gen_ostream_operator (TAO_OutStream *os);
  virtual void gen_member_ostream_operator (TAO_OutStream *os,
                                            const char *instance_name,
                                            bool accessor = false);
  // Overridden from class be_type.

  // Visiting.
  virtual int accept (be_visitor *visitor);
  
  // Cleanup.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (be_array);

protected:
  virtual int create_name (void);
  // Create a name for us.

  virtual void compute_tc_name (void);
};

#endif
