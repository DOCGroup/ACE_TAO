
//=============================================================================
/**
 *  @file    be_factory.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Factory that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================

#include "be_factory.h"
#include "be_visitor.h"

#include "ast_exception.h"
#include "ast_argument.h"

#include "utl_err.h"
#include "utl_exceptlist.h"

#include "global_extern.h"

be_factory::be_factory (UTL_ScopedName *n)
  : COMMON_Base (1,
                 0), //@@ Always local, never abstract
    AST_Decl (AST_Decl::NT_factory,
              n),
    UTL_Scope (AST_Decl::NT_factory),
    AST_Factory (n),
    be_scope (AST_Decl::NT_factory),
    be_decl (AST_Decl::NT_factory,
             n)
{
}

be_factory::~be_factory (void)
{
}

void
be_factory::destroy (void)
{
  // Call the destroy methods of our base classes.
  this->be_scope::destroy ();
  this->be_decl::destroy ();

  this->AST_Factory::destroy ();
}

int
be_factory::accept (be_visitor *visitor)
{
  return visitor->visit_factory (this);
}

AST_Argument *
be_factory::be_add_argument (AST_Argument *arg)
{
  this->add_to_scope (arg);
  this->add_to_referenced (arg,
                           0,
                           0);
  return arg;
}

IMPL_NARROW_FROM_DECL (be_factory)
IMPL_NARROW_FROM_SCOPE (be_factory)
