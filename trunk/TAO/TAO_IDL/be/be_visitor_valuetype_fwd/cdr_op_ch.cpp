//
// $Id$
//

// ================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_fwd_cdr_op_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for forward
//    declarations of valuetypes. This uses compiled marshaling.
//
// = AUTHOR
//    Boris Kolpackov <bosk@ipmce.ru>
//    base on code from Torsten Kuepper
//    based on code from  Aniruddha Gokhale & Carlos O'Ryan (cdr_op_ci.cpp)
//
// ================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype_fwd.h"

ACE_RCSID(be_visitor_valuetype_fwd, valuetype_fwd_cdr_op_ch, "$Id$")


be_visitor_valuetype_fwd_cdr_op_ch::be_visitor_valuetype_fwd_cdr_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_valuetype_fwd_cdr_op_ch::~be_visitor_valuetype_fwd_cdr_op_ch (void)
{
}

int
be_visitor_valuetype_fwd_cdr_op_ch::visit_valuetype_fwd (
    be_valuetype_fwd *node
  )
{
  AST_Interface *fd = node->full_definition ();
  be_valuetype *bfd = be_valuetype::narrow_from_decl (fd);

  // If this forward declared vt is defined later in the file,
  // the CDR operator declaration (along with the corresponding
  // declarations for members of the interface's scope) will be
  // generated then.
  if (fd->is_defined ())
    {
      return 0;
    }

  // Check if it's imported vt or CDR ops already generated.
  if (node->cli_hdr_cdr_op_gen () || node->imported ())
    {
      return 0;
    }

  // Generate helper functions declaration.
  if (bfd->gen_helper_header () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_ch::"
                         "visit_valuetype - "
                         "codegen for helper functions failed\n"), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // generate the CDR << and >> operator declarations (prototypes)

  //@@ Boris: Can I move this to be_valuetype? (as with _var, _out, etc?)

  *os << be_global->stub_export_macro ()
      << " CORBA::Boolean operator<< (TAO_OutputCDR &, const "
      << node->full_name () << " *);" << be_nl;

  *os << be_global->stub_export_macro ()
      << " CORBA::Boolean operator>> (TAO_InputCDR &, "
      << node->full_name () << " *&);" << be_nl;

  node->cli_hdr_cdr_op_gen (I_TRUE);

  return 0;
}
