// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_field.cpp
//
// = DESCRIPTION
//    Extension of class AST_Field that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_field.h"
#include "be_visitor.h"
#include "be_type.h"
#include "ace/Log_Msg.h"

ACE_RCSID (be, 
           be_field, 
           "$Id$")

be_field::be_field (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Field (),
    be_decl ()
{
}

be_field::be_field (AST_Type *ft,
                    UTL_ScopedName *n,
                    Visibility vis)
  : COMMON_Base (ft->is_local (),
                 ft->is_abstract ()),
    AST_Decl (AST_Decl::NT_field,
              n),
    AST_Field (ft,
               n,
               vis),
    be_decl (AST_Decl::NT_field,
             n)
{
}

int
be_field::accept (be_visitor *visitor)
{
  return visitor->visit_field (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_field, AST_Field, be_decl)
IMPL_NARROW_FROM_DECL (be_field)
