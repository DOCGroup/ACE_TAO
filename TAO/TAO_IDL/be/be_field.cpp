
//=============================================================================
/**
 *  @file    be_field.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Field that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_field.h"
#include "be_visitor.h"
#include "be_type.h"
#include "global_extern.h"
#include "ace/Log_Msg.h"

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
             n),
    port_name_prefixed_ (false)
{
  // This covers valuetype fields as well, which is what we want.
  AST_Decl::NodeType nt =
    ft->unaliased_type ()->node_type ();

  if (nt == AST_Decl::NT_string || nt == AST_Decl::NT_wstring)
    {
      idl_global->string_member_seen_ = true;
    }
}

be_type *
be_field::field_type (void) const
{
  return
    be_type::narrow_from_decl  (
      this->AST_Field::field_type ());
}

void
be_field::gen_member_ostream_operator (TAO_OutStream *os,
                                       const char *instance_name,
                                       bool use_underscore,
                                       bool accessor)
{
  be_type *ft = be_type::narrow_from_decl (this->field_type ());
  ft->gen_member_ostream_operator (os,
                                   instance_name,
                                   use_underscore,
                                   accessor);
}

bool
be_field::port_name_prefixed (void) const
{
  return this->port_name_prefixed_;
}

void
be_field::port_name_prefixed (bool val)
{
  this->port_name_prefixed_ = val;
}

int
be_field::accept (be_visitor *visitor)
{
  return visitor->visit_field (this);
}

void
be_field::destroy (void)
{
  // AST_Field::destroy() is called by another path.
  this->be_decl::destroy ();
}

IMPL_NARROW_FROM_DECL (be_field)
