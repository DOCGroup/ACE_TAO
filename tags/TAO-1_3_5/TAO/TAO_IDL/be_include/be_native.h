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

#include "be_type.h"
#include "ast_native.h"

class be_visitor;

// Representation of "native" IDL type added by the POA spec.
class be_native : public virtual AST_Native,
                  public virtual be_type
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

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.
  DEF_NARROW_METHODS2(be_native, AST_Native, be_type);
  DEF_NARROW_FROM_DECL(be_native);
};

#endif /* BE_NATIVE_H */
