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

ACE_RCSID (be_visitor_sequence, 
           sequence_ch, 
           "$Id$")

// Root visitor for client header.
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
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "Bad element type\n"), 
                        -1);
    }

  *os << be_nl << "// TAO_IDL - Generated from "
      << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  os->gen_ifdef_AHETI ();

  // This is the instantiation branch.
  *os << node->instance_name ();

  os->gen_else_AHETI ();

  // Generate the appropriate sequence type.
  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
      if (node->unbounded ())
        {
          *os << "TAO_Unbounded_Object_Sequence<";
        }
      else
        {
          *os << "TAO_Bounded_Object_Sequence<";
        }

      break;
    case be_sequence::MNG_PSEUDO:
      if (node->unbounded ())
        {
          *os << "TAO_Unbounded_Pseudo_Sequence<";
        }
      else
        {
          *os << "TAO_Bounded_Pseudo_Sequence<";
        }

      break;
    case be_sequence::MNG_VALUE:
      if (node->unbounded ())
        {
          *os << "TAO_Unbounded_Valuetype_Sequence<";
        }
      else
        {
          *os << "TAO_Bounded_Valuetype_Sequence<";
        }

      break;
    case be_sequence::MNG_STRING:
      if (node->unbounded ())
        {
          *os << "TAO_Unbounded_String_Sequence";
        }
      else
        {
          *os << "TAO_Bounded_String_Sequence";
        }

      break;
    case be_sequence::MNG_WSTRING:
      if (node->unbounded ())
        {
          *os << "TAO_Unbounded_WString_Sequence";
        }
      else
        {
          *os << "TAO_Bounded_WString_Sequence";
        }

      break;
    default: // Not a managed type.
      if (bt->base_node_type () == AST_Decl::NT_array)
        {
          if (node->unbounded ())
            {
              *os << "TAO_Unbounded_Array_Sequence<";
            }
          else
            {
              *os << "TAO_Bounded_Array_Sequence<";
            }
        }
      else
        {
          if (node->unbounded ())
            {
              *os << "TAO_Unbounded_Sequence<";
            }
          else
            {
              *os << "TAO_Bounded_Sequence<";
            }
        }

      break;
    }

  be_visitor_context ctx (*this->ctx_);

  be_visitor_sequence_base_template_args visitor (&ctx,
                                                  node);

  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CH);

  if (bt->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "base type visit failed\n"),
                        -1);
    }

  // Find out if the sequence is of a managed type and if
  // it is bounded or not.
  if (node->managed_type () == be_sequence::MNG_STRING
      || node->managed_type () == be_sequence::MNG_WSTRING)
    {
      if (!node->unbounded ())
        {
          *os << "<" << node->max_size () << ">";
        }
    }
  else
    {
      // If we are a sequence of arrays, the template includes
      // a _var parameter.
      if (bt->base_node_type () == AST_Decl::NT_array)
        {
          *os << ", ";

          // So the call to nested_type_name will have "_var" suffix.
          ctx.sub_state (TAO_CodeGen::TAO_ARRAY_SEQ_CH_TEMPLATE_VAR);

          if (bt->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_sequence_ch::"
                                 "visit_sequence - "
                                 "base type visit failed\n"),
                                -1);
            }

          ctx.sub_state (TAO_CodeGen::TAO_SUB_STATE_UNKNOWN);
        }

      if (node->unbounded ())
        {
          *os << ">";
        }
      else
        {
          *os << ", " << node->max_size () << ">";
        }
    }

  os->gen_endif_AHETI ();

  return 0;
}


int
be_visitor_sequence_ch::instantiate_sequence (be_sequence *node)
{
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "gen_instantiate_template_name - "
                         "Bad element type\n"), 
                        -1);
    }

  // Generate the appropriate sequence type.
  switch (node->managed_type ())
    {
    case be_sequence::MNG_PSEUDO:
    case be_sequence::MNG_OBJREF:
    case be_sequence::MNG_VALUE:
      if (node->unbounded ())
        {
          this->gen_unbounded_obj_sequence (node);
        }
      else
        {
          this->gen_bounded_obj_sequence (node);
        }

      break;
    case be_sequence::MNG_STRING: // Sequence of strings.
      if (!node->unbounded ())
        {
          this->gen_bounded_str_sequence (node);
        }

      break;
    case be_sequence::MNG_WSTRING: // Sequence of wstrings.
      if (!node->unbounded ())
        {
          this->gen_bounded_wstr_sequence (node);
        }

      break;
    default: // Not a managed type.
      if (node->unbounded ())
	      {
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
		              predef =
                    be_predefined_type::narrow_from_decl (
                        alias->primitive_base_type ()
                      );
		            }
	          }
	        if (predef != 0)
	          {
	            if (predef->pt() != AST_PredefinedType::PT_octet)
                {
		              this->gen_unbounded_sequence (node);
                }
	          }
	        else
            {
	            this->gen_unbounded_sequence (node);
            }
	      }
      else
        {
          this->gen_bounded_sequence (node);
        }

      break;
    } // End of switch.

  return 0;
}


int be_visitor_sequence_ch::visit_sequence (be_sequence *node)
{
  be_type *bt = 0;
  TAO_OutStream *os = this->ctx_->stream ();

  if (node->defined_in () == 0)
    {
      // The node is a nested sequence, and has had no scope defined.
      node->set_defined_in (DeclAsScope (this->ctx_->scope ()));
    }

  // First create a name for ourselves.
  if (node->create_name (this->ctx_->tdef ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "failed creating name\n"), -1);
    }

  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  // Instantiation.

  if (this->instantiate_sequence (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "codegen. for the primitive type sequence\n"), 
                        -1);
    }

  // End of instantiation.

  // Generate the ifdefined macro for the sequence type.
  os->gen_ifdef_macro (node->flat_name ());

  // Retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());

  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "Bad element type\n"), -1);
    }

  *os << "class " << node->local_name () << ";" << be_nl;

  // No _var class for anonymous sequences.
  if (this->ctx_->tdef () != 0)
    {
      *os << "class " << node->local_name () << "_var;" << be_nl;
    }

  *os << be_nl;

  *os << "// *************************************************************"
      << be_nl
      << "// " << node->local_name () << be_nl
      << "// *************************************************************"
      << be_nl << be_nl;

  // Generate a typedef to a parametrized sequence.
  *os << "class " << be_global->stub_export_macro ()
      << " " << node->local_name () << " : public ";

  if (this->gen_base_sequence_class (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "codegen for base sequence class\n"),
                        -1);
    }

  *os << "{" << be_nl
      << "public:" << be_idt_nl
      << node->local_name () << " (void); // default ctor" << be_nl;

  // For unbounded sequences, we have a different set of constructors.
  if (node->unbounded ())
    {
      *os << node->local_name () << " (CORBA::ULong max); // uses max size"
          << be_nl;
    }

  *os << node->local_name () << " (" << be_idt << be_idt_nl;

  if (node->unbounded ())
    {
      // Unbounded seq takes this extra parameter.
      *os << "CORBA::ULong max, " << be_nl;
    }

  *os << "CORBA::ULong length, " << be_nl;

  // Generate the base type for the buffer.
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH);
  be_visitor_sequence_buffer_type bt_visitor (&ctx);

  if (bt->accept (&bt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "base type visit failed\n"),
                        -1);
    }

  *os << " *buffer, " << be_nl
      << "CORBA::Boolean release = 0" << be_uidt_nl
      << ");" << be_uidt_nl;
  *os << node->local_name () << " (const " << node->local_name ()
      << " &); // copy ctor" << be_nl;
  *os << "~" << node->local_name () << " (void);" << be_nl
      << "static void _tao_any_destructor (void*);" << be_nl;

  // Generate the _var_type typedef (only if we are not anonymous),
  // but we must protect against certain versions of g++.
  if (this->ctx_->tdef () != 0)
    {
      *os << "\n#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)"
          << be_nl;
      *os << "typedef " << node->local_name () << "_var _var_type;\n"
          << "#endif /* ! __GNUC__ || g++ >= 2.8 */" << be_nl << be_nl;
    }

  // TAO provides extensions for octet sequences, first find out if
  // the base type is an octet (or an alias for octet).
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
	        predef =
            be_predefined_type::narrow_from_decl (
                alias->primitive_base_type ()
              );
	      }
    }

  // Now generate the extension...
  if (predef != 0 && predef->pt () == AST_PredefinedType::PT_octet
      && node->unbounded ())
    {
      *os << "\n#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)" << be_nl
	        << node->local_name () << " (" << be_idt << be_idt_nl
	        << "CORBA::ULong length," << be_nl
	        << "const ACE_Message_Block* mb" << be_uidt_nl
	        << ")" << be_uidt_nl
	        << "  : " << node->instance_name ()
	        << " (length, mb) {}" << "\n"
	        << "#endif /* TAO_NO_COPY_OCTET_SEQUENCE == 1 */" << be_nl;
    }

  *os << be_uidt_nl << "};" << be_nl;

  os->gen_endif (); // Endif macro.

  // No _var or _out class for an anonymous (non-typedef'd) sequence.
  if (this->ctx_->tdef () != 0)
    {
      // Generate the ifdefined macro for the var type.
      os->gen_ifdef_macro (node->flat_name (), "_var");

      // Generate the var and out types.
      if (this->gen_var_defn (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_ch::"
                             "visit_sequence - "
                             "codegen for _var failed\n"), 
                            -1);
        }

      os->gen_endif ();

      // Generate the ifdefined macro for the _out type.
      os->gen_ifdef_macro (node->flat_name (), "_out");

      if (this->gen_out_defn (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_ch::"
                             "visit_sequence - "
                             "codegen for _out failed\n"),
                            -1);
        }

      os->gen_endif ();
    }

  node->cli_hdr_gen (1);

  return 0;
}

// Generate the _var definition for ourself.
int
be_visitor_sequence_ch::gen_var_defn (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  char namebuf [NAMEBUFSIZE];

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (namebuf,
                   "%s_var",
                   node->local_name ()->get_string ());

  *os << "// *************************************************************"
      << be_nl;
  *os << "// class " << node->name () << "_var" << be_nl;
  *os << "// *************************************************************"
      << be_nl << be_nl;

  // Retrieve base type.
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "gen_var_defn - "
                         "Bad base type\n"),
                        -1);
    }

  // Generate the var definition (always in the client header).
  // Depending upon the data type, there are some differences which we account
  // for over here.

  *os << "class " << be_global->stub_export_macro ()
      << " " << namebuf << be_nl;
  *os << "{" << be_nl;
  *os << "public:" << be_idt_nl;

  // Default constuctor.
  *os << namebuf << " (void);" << be_nl;

  // Constuctor.
  *os << namebuf << " (" << node->local_name () << " *);" << be_nl;

  // Copy constructor.
  *os << namebuf << " (const " << namebuf <<  " &);" << be_nl;

  // Fixed-size base types only.
  if (bt->size_type () == be_decl::FIXED)
    {
      *os << "// Fixed-size base types only." << be_nl;
      *os << namebuf << " (const " << node->local_name ()
          << " &);" << be_nl;
    }

  // Destructor.
  *os << "~" << namebuf << " (void);" << be_nl;
  *os << be_nl;

  // Assignment operator from a pointer.
  *os << namebuf << " &operator= ("
      << node->local_name () << " *);" << be_nl;

  // Assignment from _var.
  *os << namebuf << " &operator= (const " << namebuf << " &);" << be_nl;

  // Fixed-size base types only.
  if (bt->size_type () == be_decl::FIXED)
    {
      *os << "// Fixed-size base types only." << be_nl;
      *os << namebuf << " &operator= (const " << node->local_name ()
          << " &);" << be_nl;
    }

  // Arrow operator.
  *os << node->local_name () << " *operator-> (void);" << be_nl;
  *os << "const " << node->local_name ()
      << " *operator-> (void) const;" << be_nl;
  *os << be_nl;

  // Other extra types (cast operators, [] operator, and others).

  // Cast operator.
  *os << "operator const " << node->local_name () << " &() const;" << be_nl;
  *os << "operator " << node->local_name () << " &();" << be_nl;
  *os << "operator " << node->local_name () << " &() const;" << be_nl;

  if (bt->size_type () == be_decl::VARIABLE)
    {
      *os << "operator " << node->local_name ()
          << " *&(); // variable-size base types only" << be_nl;
    }

  *os << be_nl;

  // Overloaded [] operators.
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH);
  be_visitor_sequence_elemtype sr_visitor (&ctx);

  if (bt->accept (&sr_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence::"
                         "gen_var_defn - "
                         "[] ret type gen failed\n"),
                        -1);
    }

  *os << " operator[] (CORBA::ULong index);" << be_nl;

  AST_Decl::NodeType nt = bt->base_node_type ();
  AST_PredefinedType::PredefinedType pdt = AST_PredefinedType::PT_void;

  if (nt == AST_Decl::NT_pre_defined)
    {
      AST_PredefinedType *p = 0;
      be_typedef *btd = be_typedef::narrow_from_decl (bt);

      if (btd == 0)
        {
          p = AST_PredefinedType::narrow_from_decl (bt);
        }
      else
        {
          p = AST_PredefinedType::narrow_from_decl (
                  btd->primitive_base_type ()
                );
        }

      pdt = p->pt ();
    }

  // @@ (JP) Problems with constant instantiations of TAO_Object_Manager,
  // TAO_Pseudo_Object_Manager, TAO_SeqElem_WString_Manager and
  // TAO_SeqElem_String_Manager make these impossible right now [BUGID:676].
  if (nt != AST_Decl::NT_string
      && nt != AST_Decl::NT_wstring
      && nt != AST_Decl::NT_interface
      && nt != AST_Decl::NT_interface_fwd
      && nt != AST_Decl::NT_array
      && pdt != AST_PredefinedType::PT_pseudo)
    {
      *os << "const ";

      if (bt->accept (&sr_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_var_defn - "
                             "[] ret type gen failed\n"),
                            -1);
        }

      *os << " operator[] (CORBA::ULong index) const;" << be_nl;
    }

  *os << be_nl;

  *os << "// in, inout, out, _retn " << be_nl;

  // The return types of in, out, inout, and _retn are based on the parameter.
  // passing rules and the base type.
  *os << "const " << node->local_name () << " &in (void) const;" << be_nl;
  *os << node->local_name () << " &inout (void);" << be_nl;
  *os << node->local_name () << " *&out (void);" << be_nl;
  *os << node->local_name () << " *_retn (void);" << be_nl;

  // Generate an additional member function that
  // returns the underlying pointer.
  *os << node->local_name () << " *ptr (void) const;" << be_uidt_nl << be_nl;

  // Generate the private section.
  *os << "private:" << be_idt_nl;
  *os << node->local_name () << " *ptr_;" << be_uidt_nl;

  *os << "};" << be_nl << be_nl;

  return 0;
}

// Generate the _out definition.
int
be_visitor_sequence_ch::gen_out_defn (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  char namebuf [NAMEBUFSIZE];

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);
  ACE_OS::sprintf (namebuf,
                   "%s_out",
                   node->local_name ()->get_string ());

  // Retrieve base type.
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "gen_out_defn - "
                         "Bad base type\n"),
                        -1);
    }

  // Generate the out definition (always in the client header).
  *os << "class " << be_global->stub_export_macro () << " "
      << namebuf << be_nl;
  *os << "{" << be_nl;
  *os << "public:" << be_idt_nl;

  // No default constructor.

  // Constructor from a pointer.
  *os << namebuf << " (" << node->local_name () << " *&);" << be_nl;

  // Constructor from a _var &.
  *os << namebuf << " (" << node->local_name () << "_var &);" << be_nl;

  // Constructor from a _out &.
  *os << namebuf << " (const " << namebuf << " &);" << be_nl;

  // Assignment operator from a _out &.
  *os << namebuf << " &operator= (const " << namebuf << " &);" << be_nl;

  // Assignment operator from a pointer &, cast operator, ptr fn, operator
  // -> and any other extra operators.

  // Assignment.
  *os << namebuf << " &operator= (" << node->local_name () << " *);" << be_nl;

  // operator ().
  *os << "operator " << node->local_name () << " *&();" << be_nl;

  // ptr function.
  *os << node->local_name () << " *&ptr (void);" << be_nl;

  // operator ->.
  *os << node->local_name () << " *operator-> (void);" << be_nl;

  // Overloaded [] operator only for sequence. The const version is not
  // required
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH);
  be_visitor_sequence_elemtype sr_visitor (&ctx);

  if (bt->accept (&sr_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence::"
                         "gen_var_defn - "
                         "[] ret type gen failed\n"),
                        -1);
    }

  *os << " operator[] (CORBA::ULong index);" << be_uidt_nl << be_nl;
  *os << "private:" << be_idt_nl;
  *os << node->local_name () << " *&ptr_;" << be_nl;
  *os << "// Assignment from T_var not allowed." << be_nl;
  *os << "void operator= (const " << node->local_name ()
      << "_var &);" << be_uidt_nl;
  *os << "};" << be_nl << be_nl;

  return 0;
}
