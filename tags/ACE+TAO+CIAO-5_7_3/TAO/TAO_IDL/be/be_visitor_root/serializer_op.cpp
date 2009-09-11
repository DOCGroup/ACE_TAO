//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    serializer_op.cpp
//
// = DESCRIPTION
//    Visitor generating code for the TAO::DCPS::Serializer operators 
//    for types defined in Root's scope.
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_root, 
           serializer_op, 
           "$Id$")

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
                         "(%N:%l) be_visitor_root_serializer_op::visit_root - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  return 0;
}
