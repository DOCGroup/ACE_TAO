//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    sequence_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequence in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, sequence_ch, "$Id$")


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


  // !! branching in either compile time template instantiation
  // or manual template instatiation
  os->gen_ifdef_AHETI();

  // this is the instantiation branch
  *os << node->instance_name ();

  os->gen_else_AHETI();

  // generate the appropriate sequence type
  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
      if (node->unbounded ())
        *os << "TAO_Unbounded_Object_Sequence<";
      else
        *os << "TAO_Bounded_Object_Sequence<";
      break;
    case be_sequence::MNG_PSEUDO:
      if (node->unbounded ())
        *os << "TAO_Unbounded_Pseudo_Sequence<";
      else
        *os << "TAO_Bounded_Pseudo_Sequence<";
      break;
    case be_sequence::MNG_STRING:
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
 
  os->gen_endif_AHETI();

  return 0;
}


int
be_visitor_sequence_ch::instantiate_sequence (be_sequence *node)
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


int be_visitor_sequence_ch::visit_sequence (be_sequence *node)
{
  be_type *bt;       // type node
  TAO_OutStream *os = this->ctx_->stream ();

  // first create a name for ourselves
  if (node->create_name (this->ctx_->tdef ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "failed creating name\n"), -1);
    }

  if (node->cli_hdr_gen () || node->imported ())
    return 0;

  // instantiation

  if (this->instantiate_sequence (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "codegen. for the primitive type sequence\n"), -1);
    }

  // end of instantiation

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

  *os << "{" << be_nl
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

  // TAO provides extensions for octet sequences, first find out if
  // the base type is an octet (or an alias for octet)
  be_predefined_type *predef = 0;
  if (bt->base_node_type () == AST_Type::NT_pre_defined)
    {
      be_typedef* alias = 
	be_typedef::narrow_from_decl (bt);

      if (alias == 0)
	{
	  predef = be_predefined_type::narrow_from_decl (bt);
	}
      else
	{
	  predef = be_predefined_type::narrow_from_decl
	    (alias->primitive_base_type ());
	}
    }
  // Now generate the extension...
  if (predef != 0 && predef->pt () == AST_PredefinedType::PT_octet
      && node->unbounded ())
    {
      *os << "\n"
	  << "#if defined(TAO_NO_COPY_OCTET_SEQUENCES)" << be_nl
	  << node->local_name () << " (" << be_idt << be_idt_nl
	  << "CORBA::ULong length," << be_nl
	  << "const ACE_Message_Block* mb" << be_uidt_nl
	  << ")" << be_uidt_nl
	  << "  : " << node->instance_name ()
	  << " (length, mb) {}" << "\n"
	  << "#endif /* TAO_NO_COPY_OCTET_SEQUENCE */\n\n";
    }

  os->decr_indent ();
  *os << "};" << be_nl;

  // define a _ptr type. This is just an extension for convenience
  *os << "typedef " << node->local_name () << " *"
      << node->local_name () << "_ptr;\n";

#if 0
  if (!this->ctx_->tdef ())
    {
      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_TYPECODE_DECL);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_ch::"
                             "visit_sequence - "
                             "TypeCode declaration failed\n"
                             ), -1);
        }
    }
#endif

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
  *os << namebuf << " (const " << namebuf << " &);" << be_nl;
  // assignment operator from a _out &
  *os << namebuf << " &operator= (const " << namebuf << " &);" << be_nl;
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
