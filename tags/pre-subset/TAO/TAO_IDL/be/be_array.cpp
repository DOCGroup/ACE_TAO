// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_array.cpp
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

#include "be_array.h"
#include "be_codegen.h"
#include "be_scope.h"
#include "be_helper.h"
#include "be_visitor.h"
#include "utl_identifier.h"
#include "idl_defines.h"
#include "ace/Log_Msg.h"

ACE_RCSID (be, 
           be_array, 
           "$Id$")

be_array::be_array (void)
{
}

be_array::be_array (UTL_ScopedName *n,
                    unsigned long ndims,
                    UTL_ExprList *dims,
                    idl_bool local,
                    idl_bool abstract)
  : be_type (AST_Decl::NT_array,
             n),
    be_decl (AST_Decl::NT_array,
             n),
    AST_Array (n,
               ndims,
               dims,
               local,
               abstract),
    AST_Decl (AST_Decl::NT_array,
              n,
              I_TRUE),
    COMMON_Base (local,
                 abstract)
{
}

be_array::~be_array (void)
{
}

int
be_array::create_name (void)
{
  char namebuf [NAMEBUFSIZE];
  unsigned long i;
  UTL_ScopedName *n = 0;
  be_decl *scope;

  ACE_OS::memset (namebuf, 
                  '\0', 
                  NAMEBUFSIZE);
  // Retrieve the base type.
  // The name always starts this way.
  be_type *bt = be_type::narrow_from_decl (this->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::"
                         "create_name - "
                         "bad base type\n"),
                        0);
    }

  ACE_OS::sprintf (namebuf, 
                   "_tao_array_%s", 
                   bt->local_name ()->get_string ());

  // Now append dimensions.
  for (i = 0; i < this->n_dims (); ++i)
    {
      AST_Expression *expr = this->dims ()[i];

      // Dimension value.
      if ((expr == 0) || ((expr != 0) && (expr->ev () == 0)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "create_name - "
                             "bad array dimension\n"),
                            -1);
        }
      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          ACE_OS::sprintf (namebuf, 
                           "%s_%d", 
                           namebuf, 
                           ((int)expr->ev ()->u.ulval));
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "create_name - "
                             "bad dimension value\n"),
                            -1);
        }
    }

  // Now see if we have a fully scoped name and if so, generate one.
  scope = be_scope::narrow_from_scope (this->defined_in ())->decl ();

  if (scope)
    {
      // Make a copy of the enclosing scope's  name.
      n = (UTL_ScopedName *)scope->name ()->copy ();

      Identifier *id = 0;
      ACE_NEW_RETURN (id,
                      Identifier (ACE_OS::strdup (namebuf)),
                      -1);

      UTL_ScopedName *sn = 0;
      ACE_NEW_RETURN (sn,
                      UTL_ScopedName (id,
                                      0),
                      -1);

      // Add our local name as the last component.
      n->nconc (sn);
      // Set the fully scoped name.
      this->set_name (n);
    }
  else
    {
      // We better be not here because we must be inside some scope,
      // at least the ROOT scope.
      return -1;
    }

  return 0;
}

// Code generation.

int
be_array::gen_dimensions (TAO_OutStream *os,
                          unsigned short slice)
{
   // Loop index.
   unsigned long i;

  // Print our dimensions.
  for (i = (slice ? 1 : 0); i < this->n_dims (); ++i)
    {
      // Retrieve the ith.
      AST_Expression *expr = this->dims ()[i];

      // Dimension value.
      if ((expr == 0) || ((expr != 0) && (expr->ev () == 0)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_dimensions - "
                             "bad array dimension\n"),
                            -1);
        }

      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          *os << "[" << ((int)expr->ev ()->u.ulval) << "]";
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_dimensions - "
                             "bad dimension value\n"),
                            -1);
        }
    }

  return 0;
}

int
be_array::accept (be_visitor *visitor)
{
  return visitor->visit_array (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_array, AST_Array, be_type)
IMPL_NARROW_FROM_DECL (be_array)
