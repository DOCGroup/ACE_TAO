// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_sequence.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Sequence
//
// = AUTHOR
//    Aniruddha Gokhale and Carlos O'Ryan
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_sequence.h"

// Root visitor for client header
be_visitor_sequence_ch::be_visitor_sequence_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_ch::~be_visitor_sequence_ch (void)
{
}

int
be_visitor_sequence_ch::gen_base_sequence_class (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  // retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "Bad element type\n"), -1);
    }

  // generate the appropriate sequence type
  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF: // sequence of objrefs
      if (node->unbounded ())
        *os << "TAO_Unbounded_Object_Sequence<";
      else
        *os << "TAO_Bounded_Object_Sequence<";
      break;
    case be_sequence::MNG_STRING: // sequence of strings
      if (node->unbounded ())
        *os << "TAO_Unbounded_String_Sequence";
      else
        *os << "TAO_Bounded_String_Sequence";
      break;
    default: // not a managed type
      if (node->unbounded ())
        *os << "TAO_Unbounded_Sequence<";
      else
        *os << "TAO_Bounded_Sequence<";
      break;
    }

  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (bt->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "base type visit failed\n"),
                        -1);
    }
  delete visitor;

  // find out if the sequence is of a managed type and if it is bounded or not
  if (node->managed_type () == be_sequence::MNG_STRING)
    {
      if (!node->unbounded ())
        {
          *os << "<" << node->max_size () << ">";
        }
    }
  else
    {
      if (node->unbounded ())
        {
          *os << ">";
        }
      else
        {
          *os << ", " << node->max_size () << ">";
        }
    }
  return 0;
}

int be_visitor_sequence_ch::visit_sequence (be_sequence *node)
{
  be_type *bt;       // type node
  TAO_OutStream *os = this->ctx_->stream ();

  if (node->cli_hdr_gen () || node->imported ())
    return 0;

  // first create a name for ourselves
  if (node->create_name (this->ctx_->tdef ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "failed creating name\n"), -1);
    }

  // generate the ifdefined macro for the sequence type
  os->gen_ifdef_macro (node->flatname ());

  os->indent (); // start with the current indentation level

  // retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "Bad element type\n"), -1);
    }

  *os << "// *************************************************************"
      << be_nl
      << "// " << node->local_name () << be_nl
      << "// *************************************************************"
      << be_nl << be_nl;

  // generate a typedef to a parametrized sequence
  *os << "class " << idl_global->export_macro ()
      << " " << node->local_name () << " : public ";
  if (this->gen_base_sequence_class (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "codegen for base sequence class\n"), -1);
    }

  *os << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << node->local_name () << " (void); // default ctor" << be_nl;
  // for unbounded sequences, we have a different set of constructors
  if (node->unbounded ())
    {
      *os << node->local_name () << " (CORBA::ULong max); // uses max size"
          << be_nl;
    }

  *os << node->local_name () << " (" << be_idt_nl;
  if (node->unbounded ())
    {
      // unbounded seq takes this extra parameter
      *os << "CORBA::ULong max, " << be_nl;
    }
  *os << "CORBA::ULong length, " << be_nl;
  // generate the base type for the buffer
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "Bad visitor\n"), -1);
    }

  if (bt->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "base type visit failed\n"),
                        -1);
    }
  delete visitor;
  *os << " *buffer, " << be_nl
      << "CORBA::Boolean release=0" << be_uidt_nl
      << ");" << be_nl;
  *os << node->local_name () << " (const " << node->local_name ()
      << " &); // copy ctor" << be_nl;
  *os << "~" << node->local_name () << " (void); // dtor\n";

  os->decr_indent ();
  *os << "};" << be_nl;

  // define a _ptr type. This is just an extension for convenience
  *os << "typedef " << node->local_name () << " *"
      << node->local_name () << "_ptr;\n";

  // Generate the typecode decl
  if (node->is_nested ())
    {
      // we have a scoped name
      os->indent ();
      // is our enclosing scope a module? We need this check because for
      // platforms that support namespaces, the typecode must be declared
      // extern
      if (node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
        *os << "TAO_NAMESPACE_STORAGE_CLASS ";
      else
        *os << "static ";
      *os << "CORBA::TypeCode_ptr "
          << node->tc_name ()->last_component () << "_seq;\n\n";
    }
  else
    {
      // we are in the ROOT scope
      os->indent ();
      *os << "extern "
         << idl_global->export_macro ()
         << " CORBA::TypeCode_ptr "
         << node->tc_name ()->last_component () << ";\n\n";
    }


  os->gen_endif (); // endif macro

  // generate the ifdefined macro for the var type
  os->gen_ifdef_macro (node->flatname (), "_var");

  // generate the var and out types
  if (this->gen_var_defn (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "codegen for _var failed\n"), -1);
    }

  os->gen_endif ();

  // generate the ifdefined macro for the _out type
  os->gen_ifdef_macro (node->flatname (), "_out");

  if (this->gen_out_defn (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "codegen for _out failed\n"),
                        -1);
    }
  os->gen_endif ();
  node->cli_hdr_gen (1);

  return 0;
}

// generate the _var definition for ourself
int
be_visitor_sequence_ch::gen_var_defn (be_sequence *node)
{
  TAO_OutStream *os; // output stream
  char namebuf [NAMEBUFSIZE];  // names
  be_type *bt;  // base type

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (namebuf, "%s_var", node->local_name ()->get_string ());

  os = this->ctx_->stream ();

  *os << "// *************************************************************"
      << be_nl;
  *os << "// class " << node->name () << "_var" << be_nl;
  *os << "// *************************************************************\n\n";

  // retrieve base type
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "gen_var_defn - "
                         "Bad base type\n"),
                        -1);
    }

  // generate the var definition (always in the client header).
  // Depending upon the data type, there are some differences which we account
  // for over here.

  os->indent (); // start with whatever was our current indent level
  *os << "class " << idl_global->export_macro ()
      << " " << namebuf << be_nl;
  *os << "{" << be_nl;
  *os << "public:\n";
  os->incr_indent ();
  // default constr
  *os << namebuf << " (void); // default constructor" << be_nl;
  // constr
  *os << namebuf << " (" << node->local_name () << " *);" << be_nl;
  // copy constructor
  *os << namebuf << " (const " << namebuf <<
    " &); // copy constructor" << be_nl;
  // destructor
  *os << "~" << namebuf << " (void); // destructor" << be_nl;
  *os << be_nl;
  // assignment operator from a pointer
  *os << namebuf << " &operator= (" << node->local_name () << " *);" << be_nl;
  // assignment from _var
  *os << namebuf << " &operator= (const " << namebuf <<
    " &);" << be_nl;

  // arrow operator
  *os << node->local_name () << " *operator-> (void);" << be_nl;
  *os << "const " << node->local_name () << " *operator-> (void) const;" << be_nl;
  *os << be_nl;

  // other extra types (cast operators, [] operator, and others)

  // cast operator
  *os << "operator const " << node->local_name () << " &() const;" << be_nl;
  *os << "operator " << node->local_name () << " &();" << be_nl;
  *os << "operator " << node->local_name () << " &() const;" << be_nl;

  // overloaded [] operator. The const version is not required for sequences
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "gen_var_defn - "
                         "Bad visitor\n"), -1);
    }

  if (bt->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence::"
                         "gen_var_defn - "
                         "[] ret type gen failed\n"),
                        -1);
    }
  delete visitor;

  *os << "operator[] (CORBA::ULong index);" << be_nl;

  *os << "// in, inout, out, _retn " << be_nl;
  // the return types of in, out, inout, and _retn are based on the parameter
  // passing rules and the base type
  *os << "const " << node->local_name () << " &in (void) const;" << be_nl;
  *os << node->local_name () << " &inout (void);" << be_nl;
  *os << node->local_name () << " *&out (void);" << be_nl;
  *os << node->local_name () << " *_retn (void);" << be_nl;

  // generate an additional member function that returns the underlying pointer
  *os << node->local_name () << " *ptr (void) const;\n";

  *os << "\n";
  os->decr_indent ();

  // generate the private section
  *os << "private:\n";
  os->incr_indent ();
  *os << node->local_name () << " *ptr_;\n";

  os->decr_indent ();
  *os << "};\n\n";

  return 0;
}

// generate the _out definition
int
be_visitor_sequence_ch::gen_out_defn (be_sequence *node)
{
  TAO_OutStream *os; // output stream
  char namebuf [NAMEBUFSIZE];  // to hold the _out name
  be_type *bt;  // base type

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (namebuf, "%s_out", node->local_name ()->get_string ());
  os = this->ctx_->stream ();

  // retrieve base type
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "gen_out_defn - "
                         "Bad base type\n"),
                        -1);
    }

  // generate the out definition (always in the client header)
  os->indent (); // start with whatever was our current indent level

  *os << "class " << idl_global->export_macro () << " "
      << namebuf << be_nl;
  *os << "{" << be_nl;
  *os << "public:\n";
  os->incr_indent ();

  // No default constructor

  // constructor from a pointer
  *os << namebuf << " (" << node->local_name () << " *&);" << be_nl;
  // constructor from a _var &
  *os << namebuf << " (" << node->local_name () << "_var &);" << be_nl;
  // constructor from a _out &
  *os << namebuf << " (" << namebuf << " &);" << be_nl;
  // assignment operator from a _out &
  *os << namebuf << " &operator= (" << namebuf << " &);" << be_nl;
  // assignment operator from a pointer &, cast operator, ptr fn, operator
  // -> and any other extra operators
  // assignment
  *os << namebuf << " &operator= (" << node->local_name () << " *);" << be_nl;
  // operator ()
  *os << "operator " << node->local_name () << " *&();" << be_nl;
  // ptr fn
  *os << node->local_name () << " *&ptr (void);" << be_nl;
  // operator ->
  *os << node->local_name () << " *operator-> (void);" << be_nl;

  // overloaded [] operator only for sequence. The const version is not
  // required
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "gen_out_defn - "
                         "Bad visitor\n"), -1);
    }

  if (bt->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence::"
                         "gen_var_defn - "
                         "[] ret type gen failed\n"),
                        -1);
    }
  delete visitor;
  *os << "operator[] (CORBA::ULong index);" << be_nl;
  *os << "\n";
  os->decr_indent ();
  *os << "private:\n";
  os->incr_indent ();

  *os << node->local_name () << " *&ptr_;" << be_nl;
  *os << "// assignment from T_var not allowed" << be_nl;
  *os << "void operator= (const " << node->local_name () << "_var &);\n";

  os->decr_indent ();
  *os << "};\n\n";

  return 0;
}

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
  *os << fname << "::" << lname << " (" << fname <<
    " &p) // copy constructor" << be_nl;
  *os << "  : ptr_ (p.ptr_)" << be_nl;
  *os << "{}\n\n";

  // assignment operator from _out &
  os->indent ();
  *os << "ACE_INLINE " << fname << " &" << be_nl;
  *os << fname << "::operator= (" << fname <<
    " &p)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "this->ptr_ = p.ptr_;" << be_nl;
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

// ************************************************************
// Root visitor for client stub class
// ************************************************************

be_visitor_sequence_cs::be_visitor_sequence_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_cs::~be_visitor_sequence_cs (void)
{
}

// XXXASG - this method is same as that in the _ch visitor. So we need some
// abstraction for this

int
be_visitor_sequence_cs::gen_base_sequence_class (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  // retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "gen_base_sequence_class - "
                         "Bad element type\n"), -1);
    }

  // generate the appropriate sequence type
  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF: // sequence of objrefs
      if (node->unbounded ())
        *os << "TAO_Unbounded_Object_Sequence<";
      else
        *os << "TAO_Bounded_Object_Sequence<";
      break;
    case be_sequence::MNG_STRING: // sequence of strings
      if (node->unbounded ())
        *os << "TAO_Unbounded_String_Sequence";
      else
        *os << "TAO_Bounded_String_Sequence";
      break;
    default: // not a managed type
      if (node->unbounded ())
        *os << "TAO_Unbounded_Sequence<";
      else
        *os << "TAO_Bounded_Sequence<";
      break;
    }

  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (bt->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "base type visit failed\n"),
                        -1);
    }
  delete visitor;

  // find out if the sequence is of a managed type and if it is bounded or not
  if (node->managed_type () == be_sequence::MNG_STRING)
    {
      if (!node->unbounded ())
        {
          *os << "<" << node->max_size () << ">";
        }
    }
  else
    {
      if (node->unbounded ())
        {
          *os << ">";
        }
      else
        {
          *os << ", " << node->max_size () << ">";
        }
    }
  return 0;
}

int be_visitor_sequence_cs::visit_sequence (be_sequence *node)
{
  // generate the constructors
  be_type *bt;       // type node
  TAO_OutStream *os = this->ctx_->stream ();

  if (node->cli_stub_gen () || node->imported ())
    return 0;

  // generate the ifdefined macro for the sequence type
  os->gen_ifdef_macro (node->flatname ());

  os->indent (); // start with the current indentation level

  // retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "Bad element type\n"), -1);
    }

  *os << "// *************************************************************"
      << be_nl
      << "// " << node->name () << be_nl
      << "// *************************************************************"
      << be_nl << be_nl;

  // default constructor
  *os << node->name () << "::" << node->local_name () << " (void)" << be_nl
      << "{}" << be_nl;

  // for unbounded sequences, we have a different set of constructors
  if (node->unbounded ())
    {
      *os << node->name () << "::" << node->local_name ()
          << " (CORBA::ULong max) // uses max size" << be_nl
          << "  : ";
      // pass it to the base constructor
      if (this->gen_base_sequence_class (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_cs::"
                             "visit_sequence - "
                             "codegen for base sequence class\n"), -1);
        }
      *os << " (max)" << be_nl
          << "{}" << be_nl;
    }

  // constructor with the buffer
  *os << node->name () << "::" << node->local_name () << " (";
  if (node->unbounded ())
    {
      *os << "CORBA::ULong max, ";  // unbounded seq takes this extra parameter
    }
  *os << "CORBA::ULong length, ";
  // generate the base type for the buffer
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "Bad visitor\n"), -1);
    }

  if (bt->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "base type visit failed\n"),
                        -1);
    }
  delete visitor;
  *os << " *buffer, CORBA::Boolean release)" << be_nl
      << "  : ";
  // pass it to the base constructor
  if (this->gen_base_sequence_class (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "codegen for base sequence class\n"), -1);
    }
  *os << " (";
  if (node->unbounded ())
    {
      *os << "max, ";
    }
  *os << "length, buffer, release)" << be_nl
      << "{}" << be_nl;

  // copy constructor
  *os << node->name () << "::" << node->local_name ()
      << " (const " << node->local_name ()
      << " &seq) // copy ctor" << be_nl
      << "  : ";
  // pass it to the base constructor
  if (this->gen_base_sequence_class (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "codegen for base sequence class\n"), -1);
    }
  *os << " (seq)" << be_nl
      << "{}" << be_nl;

  // destructor
  *os << node->name () << "::~" << node->local_name ()
      << " (void) // dtor" << be_nl
      << "{}\n\n";

  // generate the typecode information here
  os->indent (); // start from current indentation level
  *os << "static const CORBA::Long _oc_" << node->flatname () << "_seq[] ="
      << be_nl;
  *os << "{\n";
  os->incr_indent (0);
  if (node->gen_encapsulation () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence::"
                         "gen_client_stubs - "
                         "codegen for encapsulation failed\n"),
                        -1);
    }

  os->decr_indent ();
  *os << "};" << be_nl;

  *os << "static CORBA::TypeCode _tc__tc_" << node->flatname ()
      << "_seq (CORBA::tk_sequence, sizeof (_oc_" <<  node->flatname ()
      << "_seq), (char *) &_oc_" << node->flatname ()
      << "_seq, CORBA::B_FALSE);" << be_nl;
  *os << "CORBA::TypeCode_ptr " << node->tc_name () << "_seq = &_tc__tc_"
      << node->flatname () << "_seq;\n\n";
  os->gen_endif ();

  return 0;
}

// ****************************************************************
// We have to generate the parameters for the template that implements
// each sequence type.
// ****************************************************************

be_visitor_sequence_base::
be_visitor_sequence_base (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_base::~be_visitor_sequence_base (void)
{
}

int
be_visitor_sequence_base::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  *os << node->name ();
  return 0;
}

// helper
int
be_visitor_sequence_base::visit_node (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQUENCE_BASE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << bt->name ();
  return 0;
}

int
be_visitor_sequence_base::visit_sequence (be_sequence *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_interface (be_interface *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_interface_fwd (be_interface_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_string (be_string *)
{
  // NO-OP, we have ad-hoc classes from strings.
  return 0;
}

int
be_visitor_sequence_base::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_enum (be_enum *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_exception (be_exception *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_array (be_array *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_sequence_base::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// ****************************************************************
// We have to generate the buffer type in the constructor
// ****************************************************************

be_visitor_sequence_buffer_type::
be_visitor_sequence_buffer_type (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_buffer_type::~be_visitor_sequence_buffer_type (void)
{
}

// helper
int
be_visitor_sequence_buffer_type::visit_node (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << bt->name ();
  return 0;
}

int
be_visitor_sequence_buffer_type::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  if (node->pt () == AST_PredefinedType::PT_pseudo)
    *os << node->name () << "_ptr";
  else
    *os << node->name ();
  return 0;
}

int
be_visitor_sequence_buffer_type::visit_sequence (be_sequence *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_buffer_type::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH)
    *os << node->nested_type_name (this->ctx_->scope (), "_ptr");
  else
    *os << node->name () << "_ptr";
  return 0;
}

int
be_visitor_sequence_buffer_type::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH)
    *os << node->nested_type_name (this->ctx_->scope (), "_ptr");
  else
    *os << node->name () << "_ptr";
  return 0;
}

int
be_visitor_sequence_buffer_type::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  *os << "char *";
  return 0;
}

int
be_visitor_sequence_buffer_type::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_buffer_type::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_buffer_type::visit_enum (be_enum *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_buffer_type::visit_exception (be_exception *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_buffer_type::visit_array (be_array *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_buffer_type::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_sequence_buffer_type::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// ***********************************************************
// sequence element type visitor for return types of [] methods
// ***********************************************************


be_visitor_sequence_elemtype::
be_visitor_sequence_elemtype (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_elemtype::~be_visitor_sequence_elemtype (void)
{
}

// helper
int
be_visitor_sequence_elemtype::visit_node (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ()) << " &";
  else
    *os << bt->name () << " &";
  return 0;
}

int
be_visitor_sequence_elemtype::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
        {
          *os << "TAO_Object_Manager<" << bt->nested_type_name
            (this->ctx_->scope ()) << "> ";
        }
      else
        *os << "TAO_Object_Manager<" << bt->name () << "> ";
      break;
    default:
      if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
	*os << bt->nested_type_name (this->ctx_->scope ()) << " &";
      else
	*os << bt->name () << " &";
    }
  return 0;
}

int
be_visitor_sequence_elemtype::visit_sequence (be_sequence *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;


  *os << "TAO_Object_Manager <"
      << bt->nested_type_name (this->ctx_->scope ())
      << " > ";

  return 0;
}

int
be_visitor_sequence_elemtype::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;


  *os << "TAO_Object_Manager <"
      << bt->nested_type_name (this->ctx_->scope ())
      << " > ";

  return 0;
}

int
be_visitor_sequence_elemtype::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "TAO_String_Manager ";
  return 0;
}

int
be_visitor_sequence_elemtype::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_enum (be_enum *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_exception (be_exception *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_array (be_array *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_sequence_elemtype::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// ***************************************************************************
// Sequence visitor for generating Any operator declarations in the client header
// ***************************************************************************

be_visitor_sequence_any_op_ch::be_visitor_sequence_any_op_ch
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_any_op_ch::~be_visitor_sequence_any_op_ch (void)
{
}

int
be_visitor_sequence_any_op_ch::visit_sequence (be_sequence *node)
{
  if (node->cli_hdr_any_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = tao_cg->client_header ();

  // generate the Any <<= and >>= operators
  os->indent ();
  *os << "void operator<<= (CORBA::Any &, const " << node->name ()
      << " &); // copying version" << be_nl;
  *os << "void operator<<= (CORBA::Any &, " << node->name ()
      << "*); // noncopying version" << be_nl;
  *os << "CORBA::Boolean operator>>= (const CORBA::Any &, "
      << node->name () << " *&);\n";

  node->cli_hdr_any_op_gen (1);
  return 0;
}

// ***************************************************************************
// Sequence visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_sequence_any_op_cs::be_visitor_sequence_any_op_cs
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_any_op_cs::~be_visitor_sequence_any_op_cs (void)
{
}

int
be_visitor_sequence_any_op_cs::visit_sequence (be_sequence *node)
{
  if (node->cli_stub_any_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = tao_cg->client_stubs ();

  // Any <<= and >>= operators
  os->indent ();
  *os << "void operator<<= (CORBA::Any &_tao_any, const "
      << node->name () << " &_tao_elem) // copying" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Environment _tao_env;" << be_nl
      << "_tao_any.replace (" << node->tc_name () << ", &"
      << "_tao_elem, 1, _tao_env);" << be_uidt_nl
      << "}" << be_nl;

  *os << "void operator<<= (CORBA::Any &_tao_any, "
      << node->name () << " *_tao_elem) // non copying" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Environment _tao_env;" << be_nl
      << "_tao_any.replace (" << node->tc_name () << ", "
      << "_tao_elem, 0, _tao_env);" << be_uidt_nl
      << "}" << be_nl;

  *os << "CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, "
      << node->name () << " *&_tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Environment _tao_env;" << be_nl
      << "if (!_tao_any.type ()->equal (" << node->tc_name ()
      << ", _tao_env)) return 0; // not equal" << be_nl
      << "if (_tao_any.any_owns_data ())" << be_nl
      << "{" << be_idt_nl
      << "ACE_NEW_RETURN (_tao_elem, " << node->name () << ", 0);"
      << be_nl
      << "TAO_InputCDR stream ((ACE_Message_Block *)_tao_any.value ());"
      << be_nl
      << "if (stream.decode (" << node->tc_name ()
      << ", _tao_elem, 0, _tao_env)" << be_nl
      << "  == CORBA::TypeCode::TRAVERSE_CONTINUE)" << be_nl
      << "{" << be_idt_nl
      << "((CORBA::Any *)&_tao_any)->replace (_tao_any.type (), "
      << "_tao_elem, 1, _tao_env);"
      << be_nl
      << "  return 1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl  // decode failed
      << "{" << be_idt_nl
      << "delete _tao_elem;" << be_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl  // else any does not own the data
      << "{" << be_idt_nl
      << "_tao_elem = (" << node->name () << " *)_tao_any.value ();"
      << be_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}\n\n";


  node->cli_stub_any_op_gen (1);
  return 0;
}
