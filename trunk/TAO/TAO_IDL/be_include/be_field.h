
//=============================================================================
/**
 *  @file    be_field.h
 *
 *  $Id$
 *
 *  Extension of class AST_Field that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

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
  be_field (AST_Type *ft,
            UTL_ScopedName *n,
            Visibility vis = vis_NA);

  // Non-virtual override of frontend method.
  be_type *field_type (void) const;

  // Generate debugging ostream operator for members.
  void gen_member_ostream_operator (TAO_OutStream *os,
                                    const char *instance_name,
                                    bool use_underscore,
                                    bool accessor = false);

  // Accessors for the member.
  bool port_name_prefixed (void) const;
  void port_name_prefixed (bool val);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (be_field);

private:
  // If the name of a provides or uses node has already been
  // prefixed with the (mirror)port name(s), we don't want
  // to prefix it again.
  bool port_name_prefixed_;
};

#endif
