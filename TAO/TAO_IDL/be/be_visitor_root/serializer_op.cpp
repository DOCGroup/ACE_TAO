
//=============================================================================
/**
 *  @file    serializer_op.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the TAO::DCPS::Serializer operators
 *  for types defined in Root's scope.
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================

// ***************************************************************************
// Root visitor for generating Serializer operator declarations in the client header
// and stub
// ***************************************************************************

be_visitor_root_serializer_op::be_visitor_root_serializer_op (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_serializer_op::~be_visitor_root_serializer_op (void)
{
}

int
be_visitor_root_serializer_op::visit_root (be_root *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_serializer_op::")
                         ACE_TEXT ("visit_root - codegen ")
                         ACE_TEXT ("for scope failed\n")),
                        -1);
    }

  return 0;
}
