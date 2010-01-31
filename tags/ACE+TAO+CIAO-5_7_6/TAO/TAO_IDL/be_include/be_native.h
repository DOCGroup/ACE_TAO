// $Id$

// Id
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_native.h
//
// = DESCRIPTION
//    The native IDL type
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================


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
  be_native (void);
  // Default constructor.

  be_native (UTL_ScopedName *n);
  // Constructor that initializes its scoped name.

  virtual ~be_native (void) {}
  // Destructor

  virtual int gen_typecode (void);
  // Generate the typecode description.

  virtual long tc_size (void);
  // Return typecode size.
  
  virtual void destroy (void);
  // Cleanup.

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.

  DEF_NARROW_FROM_DECL(be_native);
};

#endif /* BE_NATIVE_H */
