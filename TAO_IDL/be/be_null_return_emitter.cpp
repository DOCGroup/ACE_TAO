
//=============================================================================
/**
 *  @file    be_null_return_emitter.cpp
 *
 *  $Id$
 *
 *  Emits 'return x;' for all CORBA types and ACE-supported
 *  platforms.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#include "be_null_return_emitter.h"
#include "be_visitor_null_return_value.h"
#include "be_visitor_context.h"
#include "be_helper.h"
#include "be_typedef.h"
#include "be_structure.h"
#include "be_union.h"

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
