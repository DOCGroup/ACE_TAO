//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    sequence_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequence in the client inline file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, sequence_ci, "$Id$")


// ***********************************************************
// sequence visitor for inline generation
// ***********************************************************


be_visitor_sequence_ci::be_visitor_sequence_ci (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_ci::~be_visitor_sequence_ci (void)
{
}

int
be_visitor_sequence_ci::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (node->cli_inline_gen () || node->imported ())
    return 0;

  // instantiation

  if (this->instantiate_sequence (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
                         "visit_sequence - "
                         "codegen. for the primitive type sequence\n"), 
                        -1);
    }

  // end of instantiation
  
  // generate the ifdefined macro for the sequence type
  os->gen_ifdef_macro (node->flatname ());

  // all we do is generate the _var and _out implementations
  if (this->gen_var_impl (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
                         "visit_sequence - "
                         "codegen for _var failed\n"), -1);
    }

  if (this->gen_out_impl (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
                         "visit_sequence - "
                         "codegen for _out failed\n"), -1);
    }

  // generate the endif macro for the sequence type
  os->gen_endif ();
  node->cli_inline_gen (1);

  return 0;
}

int
be_visitor_sequence_ci::instantiate_sequence (be_sequence *node)
{
  be_type *bt;

  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "gen_instantiate_template_name - "
                         "Bad element type\n"), -1);
    }

  // generate the appropriate sequence type
  switch (node->managed_type ())
    {
    case be_sequence::MNG_PSEUDO:
    case be_sequence::MNG_OBJREF:
      if (node->unbounded ())
        this->gen_unbounded_obj_sequence (node);
      else
        this->gen_bounded_obj_sequence (node);
      break;
    case be_sequence::MNG_STRING: // sequence of strings
      if (!node->unbounded ())
        this->gen_bounded_str_sequence (node);
      // else 
      //   inheriting from the right class is enough
      break;
    default: // not a managed type
      if (node->unbounded ())
	{
#if 1
	  // TAO provides extensions for octet sequences, first find out
	  // if the base type is an octet (or an alias for octet)
	  be_predefined_type *predef = 0;
	  if (bt->base_node_type () == AST_Type::NT_pre_defined)
	    {
	      be_typedef* alias = 
		be_typedef::narrow_from_decl (bt);
	      
	      if (alias == 0)
		{
		  predef =
		    be_predefined_type::narrow_from_decl (bt);
		}
	      else
		{
		  predef = be_predefined_type::narrow_from_decl
		    (alias->primitive_base_type ());
		}
	    }
	  if (predef != 0)
	    {
	      if (predef->pt() != AST_PredefinedType::PT_octet)
		this->gen_unbounded_sequence (node);
	    }
	  else
	    this->gen_unbounded_sequence (node);
#else
	  // @@ This needs to be fixed. (Michael)
	  be_predefined_type * bpt = 
	    be_predefined_type::narrow_from_decl (node->base_type());
	  if (bpt)
	    {
	      if (bpt->pt() != AST_PredefinedType::PT_octet)
		this->gen_unbounded_sequence (node);
	    }
	  else
	    this->gen_unbounded_sequence (node);
#endif
	}
      else
        this->gen_bounded_sequence (node);
      break;
    }

  return 0;
}

int
be_visitor_sequence_ci::gen_var_impl (be_sequence *node)
{
  TAO_OutStream *os; // output stream
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _var names
  be_type *bt;  // base type


  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_var", node->fullname ());

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_var", node->local_name ()->get_string ());

  os  = this->ctx_->stream ();

  // retrieve base type
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
                         "visit_sequence - "
                         "Bad element type\n"), -1);
    }

  // generate the var implementation in the inline file
  os->indent (); // start with whatever was our current indent level

  *os << "// *************************************************************"
      << be_nl;
  *os << "// Inline operations for class " << fname << be_nl;
  *os << "// *************************************************************\n\n";

  // default constr
  *os << "ACE_INLINE" << be_nl
      << fname << "::" << lname
      << " (void) // default constructor" << be_nl
      << "  " << ": ptr_ (0)" << be_nl
      << "{}\n\n";

  // constr from a _ptr
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::" << lname << " (" << node->name () << " *p)" << be_nl;
  *os << "  : ptr_ (p)" << be_nl;
  *os << "{}\n\n";

  // copy constructor
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::" << lname << " (const " << fname <<
    " &p) // copy constructor" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "if (p.ptr_)" << be_nl;
  *os << "  this->ptr_ = new " << node->name () << "(*p.ptr_);" << be_nl;
  *os << "else" << be_nl;
  *os << "  this->ptr_ = 0;\n";
  os->decr_indent ();
  *os << "}\n\n";

  // destructor
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::~" << lname << " (void) // destructor" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "delete this->ptr_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  // assignment operator from a pointer
  os->indent ();
  *os << "ACE_INLINE " << fname << " &" << be_nl;
  *os << fname << "::operator= (" << node->name () <<
    " *p)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "delete this->ptr_;" << be_nl;
  *os << "this->ptr_ = p;" << be_nl;
  *os << "return *this;\n";
  os->decr_indent ();
  *os << "}\n\n";

  // assignment operator from _var
  os->indent ();
  *os << "ACE_INLINE " << fname << " &" << be_nl;
  *os << fname << "::operator= (const " << fname <<
    " &p) // deep copy" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "if (this != &p)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "delete this->ptr_;" << be_nl;
  *os << "this->ptr_ = new " << node->name () << " (*p.ptr_);\n";
  os->decr_indent ();
  *os << "}" << be_nl;
  *os << "return *this;\n";
  os->decr_indent ();
  *os << "}\n\n";

  // two arrow operators
  os->indent ();
  *os << "ACE_INLINE const " << node->name () << " *" << be_nl;
  *os << fname << "::operator-> (void) const" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return this->ptr_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  os->indent ();
  *os << "ACE_INLINE " << node->name () << " *" << be_nl;
  *os << fname << "::operator-> (void)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return this->ptr_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  // other extra methods - 3 cast operator ()
  os->indent ();
  *os << "ACE_INLINE " << be_nl;
  *os << fname << "::operator const " << node->name () <<
    " &() const // cast" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return *this->ptr_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  os->indent ();
  *os << "ACE_INLINE " << be_nl;
  *os << fname << "::operator " << node->name () << " &() // cast " << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return *this->ptr_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  os->indent ();
  *os << "ACE_INLINE " << be_nl;
  *os << fname << "::operator " << node->name () << " &() const// cast " << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return *this->ptr_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  // operator []
  os->indent ();
  *os << "ACE_INLINE ";

  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQELEM_RETTYPE_CI);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
                         "gen_var_impl - "
                         "Bad visitor\n"), -1);
    }

  if (bt->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
                         "gen_var_impl - "
                         "[] ret type gen failed\n"),
                        -1);
    }
  delete visitor;

  *os << be_nl;
  *os << fname << "::operator[] (CORBA::ULong index)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return this->ptr_->operator[] (index);\n";
  os->decr_indent ();
  *os << "}\n\n";

  // in, inout, out, and _retn
  os->indent ();
  *os << "ACE_INLINE const " << node->name () << " &" << be_nl;
  *os << fname << "::in (void) const" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return *this->ptr_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  os->indent ();
  *os << "ACE_INLINE " << node->name () << " &" << be_nl;
  *os << fname << "::inout (void)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return *this->ptr_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  os->indent ();
  *os << "// mapping for variable size " << be_nl;
  *os << "ACE_INLINE " << node->name () << " *&" << be_nl;
  *os << fname << "::out (void)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "delete this->ptr_;" << be_nl;
  *os << "this->ptr_ = 0;" << be_nl;
  *os << "return this->ptr_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  os->indent ();
  *os << "ACE_INLINE " << node->name () << " *" << be_nl;
  *os << fname << "::_retn (void)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << node->name () << " *tmp = this->ptr_;" << be_nl;
  *os << "this->ptr_ = 0;" << be_nl;
  *os << "return tmp;\n";
  os->decr_indent ();
  *os << "}\n\n";

  // the additional ptr () member function
  os->indent ();
  *os << "ACE_INLINE " << node->name () << " *" << be_nl;
  *os << fname << "::ptr (void) const" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return this->ptr_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  return 0;
}

int
be_visitor_sequence_ci::gen_out_impl (be_sequence *node)
{
  TAO_OutStream *os; // output stream
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _out names
  be_type *bt;  // base type


  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_out", node->fullname ());

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_out", node->local_name ()->get_string ());

  os = this->ctx_->stream ();

  // retrieve base type
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
                         "gen_out_impl - "
                         "Bad element type\n"), -1);
    }

  // generate the out implementation in the inline file

  os->indent (); // start with whatever was our current indent level

  *os << "// *************************************************************"
      << be_nl;
  *os << "// Inline operations for class " << fname << be_nl;
  *os << "// *************************************************************\n\n";

  // constr from a pointer
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::" << lname << " (" << node->name () << " *&p)" << be_nl;
  *os << "  : ptr_ (p)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "this->ptr_ = 0;\n";
  os->decr_indent ();
  *os << "}\n\n";

  // constructor from _var &
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::" << lname << " (" << node->name () <<
    "_var &p) // constructor from _var" << be_nl;
  *os << "  : ptr_ (p.out ())" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "delete this->ptr_;" << be_nl;
  *os << "this->ptr_ = 0;\n";
  os->decr_indent ();
  *os << "}\n\n";

  // copy constructor
  os->indent ();
  *os << "ACE_INLINE" << be_nl;
  *os << fname << "::" << lname << " (const " << fname <<
    " &p) // copy constructor" << be_nl;
  *os << "  : ptr_ (ACE_const_cast (" << fname 
      << "&,p).ptr_)" << be_nl;
  *os << "{}\n\n";

  // assignment operator from _out &
  os->indent ();
  *os << "ACE_INLINE " << fname << " &" << be_nl;
  *os << fname << "::operator= (const " << fname <<
    " &p)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "this->ptr_ = ACE_const_cast (" << fname 
      << "&,p).ptr_;" << be_nl;
  *os << "return *this;\n";
  os->decr_indent ();
  *os << "}\n\n";

  // assignment from _var is not allowed by a private declaration

  // assignment operator from pointer
  os->indent ();
  *os << "ACE_INLINE " << fname << " &" << be_nl;
  *os << fname << "::operator= (" << node->name () <<
    " *p)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "this->ptr_ = p;" << be_nl;
  *os << "return *this;\n";
  os->decr_indent ();
  *os << "}\n\n";

  // other extra methods - cast operator ()
  os->indent ();
  *os << "ACE_INLINE " << be_nl;
  *os << fname << "::operator " << node->name () <<
    " *&() // cast" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return this->ptr_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  // ptr function
  os->indent ();
  *os << "ACE_INLINE " << node->name () << " *&" << be_nl;
  *os << fname << "::ptr (void) // ptr" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return this->ptr_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  // operator ->
  os->indent ();
  *os << "ACE_INLINE " << node->name () << " *" << be_nl;
  *os << fname << "::operator-> (void)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return this->ptr_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  // sequence has an additional method
  os->indent ();
  *os << "ACE_INLINE ";

  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQELEM_RETTYPE_CI);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
                         "gen_out_impl - "
                         "Bad visitor\n"), -1);
    }

  if (bt->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ci::"
                         "gen_out_impl - "
                         "[] ret type gen failed\n"),
                        -1);
    }
  delete visitor;

  *os << be_nl;
  *os << fname << "::operator[] (CORBA::ULong index)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return this->ptr_->operator[] (index);\n";
  os->decr_indent ();
  *os << "}\n\n";

  return 0;
}
