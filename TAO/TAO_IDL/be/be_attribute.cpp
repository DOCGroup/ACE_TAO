
//=============================================================================
/**
 *  @file    be_attribute.cpp
 *
 *  Extension of class AST_Attribute that provides additional means for C++
 *  mapping.
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_attribute.h"
#include "be_type.h"
#include "be_visitor.h"
#include "be_util.h"

#include "global_extern.h"

be_attribute::be_attribute (bool ro,
                            AST_Type *ft,
                            UTL_ScopedName *n,
                            bool local,
                            bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_attr,
              n),
    AST_Field (AST_Decl::NT_attr,
               ft,
               n),
    AST_Attribute (ro,
                   ft,
                   n,
                   local,
                   abstract),
    be_decl (AST_Decl::NT_attr,
             n),
    be_field (ft,
              n)
{
  if (!this->imported () && !this->is_local ())
    {
      // For the return types of the two operations
      // generated from this attribute.
      be_util::set_arg_seen_bit (dynamic_cast<be_type*> (ft));
    }
}

be_type *
be_attribute::field_type () const
{
  return dynamic_cast<be_type*>  (this->AST_Attribute::field_type ());
}

int
be_attribute::accept (be_visitor *visitor)
{
  return visitor->visit_attribute (this);
}

void
be_attribute::destroy ()
{
  this->be_decl::destroy ();
  this->AST_Attribute::destroy ();
}
