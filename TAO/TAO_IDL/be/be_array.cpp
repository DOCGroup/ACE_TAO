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

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

ACE_RCSID(be, be_array, "$Id$")

be_array::be_array (void)
  :  tao_name_ (0)
{
}

be_array::be_array (UTL_ScopedName *n,
                    unsigned long ndims,
                    UTL_ExprList *dims,
                    idl_bool local,
                    idl_bool abstract)
  : AST_Array (n, 
               ndims, 
               dims, 
               local, 
               abstract),
    AST_Decl (AST_Decl::NT_array, 
              n, 
              I_TRUE),
    COMMON_Base (local, 
                 abstract),
    tao_name_ (0)
{
}

be_array::~be_array (void)
{
  if (this->tao_name_ == 0)
    delete [] tao_name_;
}

// create a name for ourselves
const char*
be_array::tao_name (void)
{
  if (this->tao_name_ != 0)
    return this->tao_name_;

  be_type *bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::"
                         "tao_name - "
                         "bad base type\n"),
                        0);
    }

  const char prefix[] = "_tc_tao_array_";

  int l = (ACE_OS::strlen (bt->local_name ()->get_string ())
           + ACE_OS::strlen (this->local_name ()->get_string ()) + 1
           + sizeof (prefix)
           + 5 * this->n_dims ());

  ACE_NEW_RETURN (this->tao_name_, char[l], 0);

  ACE_OS::sprintf (this->tao_name_, "%s%s_%s",
                   prefix, this->local_name ()->get_string (),
                   bt->local_name ()->get_string());

  for (unsigned int i = 0; i < this->n_dims (); ++i)
    {
      AST_Expression *expr = this->dims ()[i]; // retrieve the ith

      // dimension value
      if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "tao_name - "
                             "bad array dimension\n"),
                            0);
        }

      if (expr->ev ()->et != AST_Expression::EV_ulong)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "tao_name - "
                             "bad dimension value\n"),
                            0);
        }

      char buf[16];
      ACE_OS::sprintf (buf, "_%4.4x", ((int) expr->ev ()->u.ulval));
      ACE_OS::strcat (this->tao_name_, buf);
    }
  return this->tao_name_;
}

int
be_array::create_name (void)
{
  char namebuf [NAMEBUFSIZE];
  unsigned long i;
  UTL_ScopedName *n = NULL;
  be_decl *scope; // scope in which we are defined

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);  // reset the buffer
  // retrieve the base type
  // the name always starts this way

  be_type *bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::"
                         "create_name - "
                         "bad base type\n"),
                        0);
    }

  ACE_OS::sprintf (namebuf, "_tao_array_%s", bt->local_name ()->get_string ());
  // now append dimensions
  for (i = 0; i < this->n_dims (); i++)
    {
      AST_Expression *expr = this->dims ()[i]; // retrieve the ith

      // dimension value
      if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "create_name - "
                             "bad array dimension\n"),
                            -1);
        }
      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          ACE_OS::sprintf (namebuf, "%s_%d", namebuf, ((int)expr->ev ()->u.ulval));
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

  // now see if we have a fully scoped name and if so, generate one
  scope = be_scope::narrow_from_scope (this->defined_in ())->decl ();
  if (scope)
    {
      // make a copy of the enclosing scope's  name
      n = (UTL_ScopedName *)scope->name ()->copy () ;

      // add our local name as the last component
      n->nconc (new UTL_ScopedName (new Identifier (ACE_OS::strdup (namebuf)),
                                    NULL));
      // set the fully scoped name
      this->set_name (n);
    }
  else
    {
      // We better be not here because we must be inside some scope,
      // atleast the ROOT scope.
      return -1;
    }
  return 0;
}

// Code generation

int
be_array::gen_dimensions (TAO_OutStream *os, 
                          unsigned short slice)
{
   // Loop index.
   unsigned long i;

  // Print our dimensions.
  for (i = (slice ? 1 : 0); i < this->n_dims (); i++)
    {
      // Retrieve the ith.
      AST_Expression *expr = this->dims ()[i];

      // Dimension value.
      if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
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

// compute the size type of the node in question
int
be_array::compute_size_type (void)
{
  be_type *type = be_type::narrow_from_decl (this->base_type ());
  if (!type)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::compute_size_type - "
                         "bad base type\n"), -1);
    }

  // our size type is the same as our type
  this->size_type (type->size_type ());

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
