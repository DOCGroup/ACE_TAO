//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    constant_cs.cpp
//
// = DESCRIPTION
//    Visitor for code generation of Constant code in the client stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_constant.h"

// ********************************************************************
// Visitor implementation for the Constant type
// This one for the client stubs file
// ********************************************************************

be_visitor_constant_cs::be_visitor_constant_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_constant_cs::~be_visitor_constant_cs (void)
{
}

// visit the Constant_cs node and its scope
int
be_visitor_constant_cs::visit_constant (be_constant *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (!node->cli_stub_gen () && !node->imported ())
    {
      if (node->is_nested ())
        {
          if (node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
            {
              *os << "TAO_NAMESPACE_TYPE (const "
                  << node->exprtype_to_string () << ")" << be_nl;
              be_module *module = be_module::narrow_from_scope (node->defined_in ());
              if (!module || (this->gen_nested_namespace_begin (module) == -1))
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "be_visitor_constant_cs::visit_constant - "
                                     "Error parsing nested name\n"),
                                    -1);
                }
              *os << "TAO_NAMESPACE_DEFINE (const "
                  << node->exprtype_to_string () << ", "
                  << node->local_name () << ", "
                  << node->constant_value () << ")" << be_nl;
              if (this->gen_nested_namespace_end (module) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "be_visitor_constant_cs::visit_constant - "
                                     "Error parsing nested name\n"),
                                    -1);
                }
            }
          else
            {
              // for those constants not defined in the outer most scope, they get
              // assigned to their values in the impl file
              os->indent (); // start from whatever indentation level we were at
              *os << "const " << node->exprtype_to_string () << " "
                  << node->name () << " = " << node->constant_value ()
                  << ";\n\n";
            }
        }
      node->cli_stub_gen (I_TRUE);
    }
  return 0;
}

// the following needs to be done to deal with the most bizarre behavior of
// MSVC++ compiler
int
be_visitor_constant_cs::gen_nested_namespace_begin (be_module *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  UTL_IdListActiveIterator *i;

  i = new UTL_IdListActiveIterator (node->name ());
  while (!(i->is_done ()))
    {
      if (ACE_OS::strcmp (i->item ()->get_string (), "") != 0)
        {
          // leave the outermost root scope
          *os << "TAO_NAMESPACE_BEGIN (" << i->item ()->get_string ()
              << ")" << be_nl;
        }
      i->next ();
    }
  delete i;
  return 0;
}

// the following needs to be done to deal with the most bizarre behavior of
// MSVC++ compiler
int
be_visitor_constant_cs::gen_nested_namespace_end (be_module *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  UTL_IdListActiveIterator *i;

  i = new UTL_IdListActiveIterator (node->name ());
  while (!(i->is_done ()))
    {
      if (ACE_OS::strcmp (i->item ()->get_string (), "") != 0)
        {
          // leave the outermost root scope
          *os << "TAO_NAMESPACE_END" << be_nl;
        }
      i->next ();
    }
  delete i;
  return 0;
}
