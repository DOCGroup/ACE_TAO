//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    typecode_defn.cpp
//
// = DESCRIPTION
//    Visitor generating code for TypeCode definitions for types.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_typecode.h"

ACE_RCSID(be_visitor_typecode, typecode_defn, "$Id$")


// ******************************************************
// TypeCode Definitions
// ******************************************************

be_visitor_typecode_defn::be_visitor_typecode_defn (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_typecode_defn::~be_visitor_typecode_defn (void)
{
}

// the following needs to be done to deal with the most bizarre behavior of
// MSVC++ compiler
int
be_visitor_typecode_defn::gen_nested_namespace_begin (be_module *node)
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
be_visitor_typecode_defn::gen_nested_namespace_end (be_module *node)
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

int
be_visitor_typecode_defn::visit_type (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->indent ();
  // generate the typecode information here
  *os << "static const CORBA::Long _oc_" << node->flatname () << "[] ="
      << be_nl;
  *os << "{" << be_idt << "\n";
  // note that we just need the parameters here and hence we generate the
  // encapsulation for the parameters
  if (node->gen_encapsulation () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typecode_defn::"
                         "visit_type - "
                         "codegen for typecode failed\n"), -1);
    }
  *os << be_uidt << "};" << be_nl;

  *os << "static CORBA::TypeCode _tc_TAO_tc_" << node->flatname ()
      << " (";
  switch (node->node_type ())
    {
    case AST_Decl::NT_array:
      *os << "CORBA::tk_array";
      break;
    case AST_Decl::NT_enum:
      *os << "CORBA::tk_enum";
      break;
    case AST_Decl::NT_except:
      *os << "CORBA::tk_except";
      break;
    case AST_Decl::NT_interface:
      *os << "CORBA::tk_objref";
      break;
    case AST_Decl::NT_sequence:
      *os << "CORBA::tk_sequence";
      break;
    case AST_Decl::NT_struct:
      *os << "CORBA::tk_struct";
      break;
    case AST_Decl::NT_typedef:
      *os << "CORBA::tk_alias";
      break;
    case AST_Decl::NT_union:
      *os << "CORBA::tk_union";
      break;
    default:
      return -1; // error
    }

  *os << ", sizeof (_oc_" <<  node->flatname ()
      << "), (char *) &_oc_" << node->flatname ()
      << ", 0, sizeof (" << node->name () << "));" << be_nl;

  // is our enclosing scope a module? We need this check because for
  // platforms that support namespaces, the typecode must be declared
  // extern
  if (node->is_nested () &&
      node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
    {
      *os << "TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)" << be_nl;
      be_module *module = be_module::narrow_from_scope (node->defined_in ());
      if (!module || (this->gen_nested_namespace_begin (module) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_typecode_defn::visit_type - "
                             "Error parsing nested name\n"),
                            -1);
        }
      *os << "TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_"
          << node->local_name () << ", &_tc_TAO_tc_"
          << node->flatname () << ")" << be_nl;
      if (this->gen_nested_namespace_end (module) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_typecode_defn::visit_type - "
                             "Error parsing nested name\n"),
                            -1);
        }
    }
  else
    {
      // outermost scope.
      *os << "CORBA::TypeCode_ptr " << node->tc_name () << " = &_tc_TAO_tc_"
          <<  node->flatname () << ";\n\n";
    }
  return 0;
}

int
be_visitor_typecode_defn::visit_array (be_array *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_defn::visit_enum (be_enum *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_defn::visit_exception (be_exception *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_defn::visit_interface (be_interface *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_defn::visit_sequence (be_sequence *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_defn::visit_structure (be_structure *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_defn::visit_typedef (be_typedef *node)
{
  return this->visit_type (node);
}

int
be_visitor_typecode_defn::visit_union (be_union *node)
{
  return this->visit_type (node);
}
