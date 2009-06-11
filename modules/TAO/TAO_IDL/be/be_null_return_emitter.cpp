// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    be_null_return_emitter.cpp
//
// = DESCRIPTION
//    Emits 'return x;' for all CORBA types and ACE-supported
//    platforms.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "be_null_return_emitter.h"
#include "be_visitor_null_return_value.h"
#include "be_visitor_context.h"
#include "be_helper.h"
#include "be_typedef.h"
#include "be_structure.h"
#include "be_union.h"

ACE_RCSID (be, 
           be_null_return_emitter, 
           "$Id$")

be_null_return_emitter::be_null_return_emitter (
    be_visitor_context *ctx)
  : ctx_ (ctx),
    os_ (*ctx->stream ())
{
}

be_null_return_emitter::~be_null_return_emitter (void)
{
}

int
be_null_return_emitter::emit (be_type *node)
{
  AST_Decl::NodeType nt = node->node_type ();
  
  if (nt == AST_Decl::NT_typedef)
    {
      be_typedef *td = be_typedef::narrow_from_decl (node);
      nt = td->primitive_base_type ()->node_type ();
    }
    
  if (nt == AST_Decl::NT_struct || nt == AST_Decl::NT_union)
    {
      this->emit_struct_union (node);
      return 0;
    }
  
  os_ << "return ";
  
  be_visitor_null_return_value visitor (this->ctx_);
  
  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_null_return_emitter::")
                         ACE_TEXT ("emit(be_decl *) - accept() ")
                         ACE_TEXT ("failed for %C\n"),
                         node->full_name ()),
                        -1);
    }
    
  os_ << ";";
  
  return 0;
}

void
be_null_return_emitter::emit_struct_union (be_type *node)
{
  if (node->size_type () == AST_Type::FIXED)
    {
      os_ << "::" << node->full_name () << " retval;" << be_nl;
    }
    
  os_ << "return ";
    
  switch (node->size_type ())
    {
      case AST_Type::FIXED:
        os_ << "retval";
        break;
      case AST_Type::VARIABLE:
        os_ << "0";
        break;
      default:
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("be_null_return_emitter::")
                    ACE_TEXT ("emit_struct_union - ")
                    ACE_TEXT ("bad size type\n")));
    }
    
  os_ << ";";
}

