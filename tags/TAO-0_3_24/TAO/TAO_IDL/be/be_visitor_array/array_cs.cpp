//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    array_cs.cpp
//
// = DESCRIPTION
//    Visitor for code generation of Arrays in the client stubs
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_array.h"

ACE_RCSID(be_visitor_array, array_cs, "$Id$")


// ************************************************************************
//  visitor for array declaration in client stubs
// ************************************************************************

be_visitor_array_cs::be_visitor_array_cs (be_visitor_context *ctx)
  : be_visitor_array (ctx)
{
}

be_visitor_array_cs::~be_visitor_array_cs (void)
{
}

int be_visitor_array_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_type *bt;  // base type
  unsigned long i;

  // nothing to do if we are imported or code is already generated
  if (node->imported () || (node->cli_stub_gen ()))
    return 0;

  this->ctx_->node (node); // save the array node

  // retrieve the type
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_array_ci::"
                         "visit_array - "
                         "Bad base type\n"),
                        -1);
    }

  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local names of the var
  // save the node's local name and full name in a buffer for quick use later
  // on 
  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  if (this->ctx_->tdef ())
    {
      // typedefed node
      ACE_OS::sprintf (fname, "%s", node->fullname ());
      ACE_OS::sprintf (lname, "%s", 
                       node->local_name ()->get_string ());
    }
  else
    {
      // for anonymous arrays ...
      // we have to generate a name for us that has an underscope prepended to
      // our local name. This needs to be inserted after the parents's name
      if (node->is_nested ())
        {
          be_decl *parent = be_scope::narrow_from_scope (node->defined_in ())->decl ();
          ACE_OS::sprintf (fname, "%s::_%s", parent->fullname (), 
                           node->local_name ()->get_string ());
          ACE_OS::sprintf (lname, "_%s", 
                           node->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (fname, "_%s", node->fullname ());
          ACE_OS::sprintf (lname, "_%s", 
                           node->local_name ()->get_string ());
        }
    }

  // dup method
  os->indent ();
  *os << fname << "_slice *" << be_nl
      << fname << "_dup (const " << fname
      << "_slice *_tao_src_array)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << fname << "_slice *_tao_dup_array = " << fname
      << "_alloc ();" << be_nl;
  *os << "if (!_tao_dup_array) return (" << fname
      << "_slice *)0;" << be_nl;
  *os << fname << "_copy (_tao_dup_array, _tao_src_array);" << be_nl;
  *os << "return _tao_dup_array;" << be_uidt_nl;
  *os << "}\n\n";

  // copy method
  os->indent ();
  *os << "void " << be_nl;
  *os << fname << "_copy (" << fname << "_slice * _tao_to, "
      << "const " << fname << "_slice *_tao_from)" << be_nl;
  *os << "{" << be_idt_nl;
  *os << "// copy each individual elements" << be_nl;

  // generate nested loops for as many dimensions as there are
  for (i = 0; i < node->n_dims (); i++)
    {
      // retrieve the ith dimension value
      AST_Expression *expr = node->dims ()[i];
      // dimension value
      if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cs::"
                             "visit_array - "
                             "bad array dimension\n"),
                            -1);
        }
      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          // generate a loop for each dimension
          *os << "for (CORBA::ULong i" << i << " = 0; i" << i << " < "
              << expr->ev ()->u.ulval << "; i" << i << "++)" << be_idt_nl;
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cs::"
                             "visit_array - "
                             "bad array dimension value\n"),
                            -1);
        }
    }

  // now generate code such that every element of the array gets assigned
  // inside the innermost level of the  nested loops generated above
  be_array *primitive_type = 0;
  if (bt->node_type () == AST_Decl::NT_typedef)
    {
      // base type of the array node is a typedef. We need to make sure that
      // this typedef is not to another array type. If it is, then we cannot
      // assign an array to another. We will have to invoke the underlying
      // array type's copy method for every array dimension.
      be_typedef *tdef = be_typedef::narrow_from_decl (bt);
      // check if the base type of the typedef node is an Array node
      primitive_type = be_array::narrow_from_decl (tdef->base_type ());
    }
  if (primitive_type)
    {
      // the base type is not a typedef to a possibly another array type. In
      // such a case, assign each element.

      *os << "// call the underlying _copy" << be_nl;
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_cs::"
                             "visit_array - "
                             "base type decl failed\n"),
                            -1);
        }

      *os << "_copy (_tao_to";
      for (i = 0; i < node->n_dims (); i++)
        {
          *os << "[i" << i << "]";
        }
      *os << ", ";
      *os << "_tao_from";  // generate the rvalue
      for (i = 0; i < node->n_dims (); i++)
        {
          *os << "[i" << i << "]";
        }
      *os << ");";
    }
  else
    {
      // the base type is not a typedef to a possibly another array type. In
      // such a case, assign each element.

      *os << "_tao_to"; // generate the lvalue
      for (i = 0; i < node->n_dims (); i++)
        {
          *os << "[i" << i << "]";
        }
      *os << " = ";
      *os << "_tao_from";  // generate the rvalue
      for (i = 0; i < node->n_dims (); i++)
        {
          *os << "[i" << i << "]";
        }
      *os << ";";
    }
  for (i = 0; i < node->n_dims (); i++)
    {
      // decrement indentation as many times as the number of dimensions
      *os << be_uidt;
    }
  *os << be_uidt_nl << "}\n\n";

#if 0 
  // typecode for anonymous arrays is not required since we do not generate the
  // Any operators for it and it cannot be used as a type

  // is this a typedefined array? if so, then let the typedef deal with
  // generation of the typecode
  if (!this->ctx_->tdef ())
    {
      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      be_visitor_context ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DEFN);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_cs::"
                             "visit_array - "
                             "TypeCode definition failed\n"
                             ), -1);
        }
      delete visitor;
    }
#endif /* 0 */

  node->cli_stub_gen (1);

  return 0;
}
