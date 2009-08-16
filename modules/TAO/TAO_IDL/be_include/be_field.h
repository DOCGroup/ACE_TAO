// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_field.h
//
// = DESCRIPTION
//    Extension of class AST_Field that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_FIELD_H
#define BE_FIELD_H

#include "be_decl.h"
#include "ast_field.h"

class be_visitor;
class TAO_OutStream;

class be_field : public virtual AST_Field,
                 public virtual be_decl
{
public:
  be_field (void);

  be_field (AST_Type *ft,
            UTL_ScopedName *n,
            Visibility vis = vis_NA);
            
  // Generate debugging ostream operator for members.
  void gen_member_ostream_operator (TAO_OutStream *os,
                                    const char *instance_name,
                                    bool accessor = false);

  // Visiting.
  virtual int accept (be_visitor *visitor);
  
  // Cleanup.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (be_field);
};

#endif
