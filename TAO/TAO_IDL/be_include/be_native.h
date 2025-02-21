
//=============================================================================
/**
 *  @file    be_native.h
 *
 *  The native IDL type
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef BE_NATIVE_H
#define BE_NATIVE_H

#include "be_exception.h"
#include "ast_native.h"

class be_visitor;

// Representation of "native" IDL type. It may be used as a
// return type, parameter type, or in an operation's
// exception list. This last usage creates special problems
// with both syntax checking and code generation. Letting
// this class inherit from be_exception (similar to the
// inheritance of AST_Native) is the most seamless
// way to handle it, and does not affect the other use cases.
class be_native : public virtual AST_Native,
                  public virtual be_exception
{
public:
  be_native (UTL_ScopedName *n);

  virtual ~be_native () {}

  /// Generate the typecode description.
  virtual int gen_typecode ();

  /// Return typecode size.
  virtual long tc_size ();

  /// Cleanup.
  virtual void destroy ();

  // Visiting.
  virtual int accept (be_visitor *visitor);
};

#endif /* BE_NATIVE_H */
