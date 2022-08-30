
//=============================================================================
/**
 *  @file    be_string.h
 *
 *  Extension of class AST_String that provides additional means for C++
 *  mapping.
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#ifndef BE_STRING_H
#define BE_STRING_H

#include "be_type.h"
#include "ast_string.h"

class AST_Expression;
class be_visitor;

class be_string : public virtual AST_String,
                  public virtual be_type
{
public:
  be_string (AST_Decl::NodeType nt,
             UTL_ScopedName *n,
             AST_Expression *v,
             long width);

  // Overridden from class be_type.
  void gen_member_ostream_operator (TAO_OutStream *os,
                                   const char *instance_name,
                                   bool use_underscore,
                                   bool accessor = false) override;

  // Visiting.
  int accept (be_visitor *visitor) override;

  // Cleanup.
  void destroy () override;

protected:
  /// Overridden method to compute typecode name.
  void compute_tc_name () override;
};

#endif
