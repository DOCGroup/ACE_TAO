
//=============================================================================
/**
 *  @file    ami_exception_holder_operation_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Operation in the stubs file.
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_ami_exception_holder_operation_cs::
be_visitor_operation_ami_exception_holder_operation_cs (
    be_visitor_context *ctx
  )
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_exception_holder_operation_cs::
~be_visitor_operation_ami_exception_holder_operation_cs (void)
{
}

int
be_visitor_operation_ami_exception_holder_operation_cs::visit_operation (
    be_operation *
  )
{
  return 0;
}
