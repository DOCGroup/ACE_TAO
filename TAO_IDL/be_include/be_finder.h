
//=============================================================================
/**
 *  @file    be_finder.h
 *
 *  $Id$
 *
 *  Extension of class AST_Finder that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef BE_FINDER_H
#define BE_FINDER_H

#include "be_factory.h"

#include "ast_finder.h"

class be_finder : public virtual AST_Finder,
                  public virtual be_factory
{
public:
  be_finder (UTL_ScopedName *n);

  ~be_finder (void);

  /// Cleanup method.
  virtual void destroy (void);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_FROM_DECL (be_finder);
  DEF_NARROW_FROM_SCOPE (be_finder);
};

#endif // BE_FINDER_H
