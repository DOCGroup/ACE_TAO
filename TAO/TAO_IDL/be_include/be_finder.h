
//=============================================================================
/**
 *  @file    be_finder.h
 *
 *  Extension of class AST_Finder that provides additional means for C++
 *  mapping.
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

  ~be_finder ();

  /// Cleanup method.
  virtual void destroy ();

  // Visiting.
  virtual int accept (be_visitor *visitor);
};

#endif // BE_FINDER_H
