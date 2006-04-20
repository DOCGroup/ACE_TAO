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
//    for types defined in Module's scope.
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_module, 
           serializer_op, 
           "$Id: serializer_op.cpp,v 1.2 1999/05/28 02:16:43 coryan Exp ")

// ***************************************************************************
// Module visitor for generating Serializer operator declarations in the client header
// and stub
// ***************************************************************************

be_visitor_module_serializer_op::be_visitor_module_serializer_op (be_visitor_context *ctx)
  : be_visitor_module (ctx)
{
}

be_visitor_module_serializer_op::~be_visitor_module_serializer_op (void)
{
}

int
be_visitor_module_serializer_op::visit_module (be_module *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module_serializer_op::visit_module - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  return 0;
}
