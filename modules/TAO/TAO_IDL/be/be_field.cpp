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
#include "global_extern.h"
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
  // This covers valuetype fields as well, which is what we want.
  AST_Decl::NodeType nt =
    ft->unaliased_type ()->node_type ();
    
  if (nt == AST_Decl::NT_string || nt == AST_Decl::NT_wstring)
    {
      idl_global->string_member_seen_ = true;
    }
}

void
be_field::gen_member_ostream_operator (TAO_OutStream *os,
                                       const char *instance_name,
                                       bool accessor)
{
  be_type *ft = be_type::narrow_from_decl (this->field_type ());
  ft->gen_member_ostream_operator (os, instance_name, accessor);
}

int
be_field::accept (be_visitor *visitor)
{
  return visitor->visit_field (this);
}

void
be_field::destroy (void)
{
  this->be_decl::destroy ();
  this->AST_Field::destroy ();
}



IMPL_NARROW_FROM_DECL (be_field)
