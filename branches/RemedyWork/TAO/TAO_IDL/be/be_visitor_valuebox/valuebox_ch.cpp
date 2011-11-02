
//=============================================================================
/**
 *  @file    valuebox_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for valueboxes in the client header
 *
 *
 *  @author Gary Maxey
 */
//=============================================================================


// ******************************************************
// Valuebox visitor for client header
// ******************************************************

be_visitor_valuebox_ch::be_visitor_valuebox_ch (be_visitor_context *ctx)
  : be_visitor_valuebox (ctx)
{
}

be_visitor_valuebox_ch::~be_visitor_valuebox_ch (void)
{
}

int
be_visitor_valuebox_ch::visit_valuebox (be_valuebox *node)
{
  // Nothing to do if we are imported or code is already generated.
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  this->ctx_->node (node); // save the node

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2
      << "class " << node->local_name () << ";" ;

  // Generate the _var and _out template class declarations.
  *os << be_nl_2
      << "typedef" << be_idt_nl
      << "TAO_Value_Var_T<" << be_idt << be_idt_nl
      << node->local_name () << be_uidt_nl
      << ">" << be_uidt_nl
      << node->local_name () << "_var;" << be_uidt_nl << be_nl
      << "typedef" << be_idt_nl
      << "TAO_Value_Out_T<" << be_idt << be_idt_nl
      << node->local_name () << be_uidt_nl
      << ">" << be_uidt_nl
      << node->local_name () << "_out;" << be_uidt;

  *os << be_nl_2 << "class " << be_global->stub_export_macro ()
      << " " << node->local_name ();
  *os << be_idt_nl <<": public virtual ::CORBA::DefaultValueRefCountBase";

  *os << be_uidt << be_nl
      << "{" << be_nl
      << "public:" << be_idt;

  node->gen_stub_decls (os);

  // _downcast method
  *os << be_nl_2
      << "static " << node->local_name () << "* "
      << "_downcast ( ::CORBA::ValueBase *);" << be_nl;

  // _copy_value method
  *os << "::CORBA::ValueBase * _copy_value (void);" << be_nl_2;

  // repository id methods
  *os << "virtual const char* "
      << "_tao_obv_repository_id (void) const;"
      << be_nl_2
      << "virtual void "
      << "_tao_obv_truncatable_repo_ids (Repository_Id_List &ids) const;"
      << be_nl_2
      << "static const char* "
      << "_tao_obv_static_repository_id (void);" << be_nl_2;

  // unmarshal method
  *os << "static ::CORBA::Boolean _tao_unmarshal (" << be_idt << be_idt_nl
      << "TAO_InputCDR &," << be_nl
      << node->local_name () << " *&" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  if (be_global->tc_support ())
    {
      *os << "virtual ::CORBA::TypeCode_ptr _tao_type (void) const;"
          << be_nl_2;
    }

  be_type *bt = be_type::narrow_from_decl (node->boxed_type ());

  // Emit the type specific elements.  The visit_* methods in this
  // module do that work.
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_valuebox_ch::")
                         ACE_TEXT ("visit_valuebox - ")
                         ACE_TEXT ("type-specific valuebox")
                         ACE_TEXT (" code generation failed\n")),
                        -1);
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Generate the "protected" destructor

  *os << be_uidt_nl << be_nl << "protected:" << be_idt_nl;

  *os << "virtual ~" << node->local_name () << " (void);" << be_nl;

  // Methods for marshalling and unmarshalling the value
  *os << "virtual ::CORBA::Boolean "
      << "_tao_marshal_v (TAO_OutputCDR &) const;" << be_nl;
  *os << "virtual ::CORBA::Boolean "
      << "_tao_unmarshal_v (TAO_InputCDR &);" << be_nl;
  *os << "virtual ::CORBA::Boolean "
      << "_tao_match_formal_type (ptrdiff_t ) const;" << be_nl;

  // Private unimplemented default assignment operator
  *os << be_uidt_nl << "private:" << be_idt_nl;
  *os << "void operator= (const " << node->local_name () << " & val);"
      << be_nl;

  *os << be_uidt_nl
      << "};";

  // Generate typecode declaration.
  if (be_global->tc_support ())
    {
      be_visitor_context ctx (*this->ctx_);
      be_visitor_typecode_decl visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_valuebox_ch::")
                             ACE_TEXT ("visit_valuebox - ")
                             ACE_TEXT ("TypeCode declaration failed\n")),
                            -1);
        }
    }

  // Indicate that code is already generated for this node.
  node->cli_hdr_gen (true);

  return 0;
}


int
be_visitor_valuebox_ch::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl;

  this->emit_default_constructor();

  // Public constructor that takes a const array argument
  this->emit_constructor_one_arg (node, "", "const ", "");

  // Public copy constructor
  this->emit_copy_constructor ();

  *os << be_nl << "// assignment operator" << be_nl;

  // Public assignment operator that takes a const array argument
  this->emit_assignment (node, "", "const ", "");

  // Public accessor and modifier methods
  *os << "// Accessors and modifier" << be_nl;

  *os << "const " << node->full_name () << "_slice* "
      << "_value (void) const;" << be_nl;

  *os << node->full_name () << "_slice* " << "_value (void);" << be_nl;

  *os << "void" << " _value (const " << node->full_name () << " val);"
      << be_nl_2;

  // Overloaded subscript operators
  *os << "const " << node->full_name ()
      << "_slice & operator[] ( ::CORBA::ULong index) const;" << be_nl;

  *os << node->full_name ()
      << "_slice &  operator[] ( ::CORBA::ULong index);"
      << be_nl_2;

  // Explicit conversion functions
  *os << "const " << node->full_name ()
      << "_slice * _boxed_in (void) const;"
      << be_nl;

  *os << node->full_name () << "_slice * _boxed_inout (void);" << be_nl;

  *os << node->full_name () << "_slice * _boxed_out (void);" << be_nl
      << be_nl;

  // Member variable of underlying type;
  this->emit_boxed_member_var (node, "_var");

  return 0;
}


int
be_visitor_valuebox_ch::visit_enum (be_enum *node)
{
  return this->emit_for_predef_enum (node, "", false);
}


int
be_visitor_valuebox_ch::visit_interface (be_interface *node)
{
  return this->emit_for_predef_enum (node, "_ptr", false);
}


int
be_visitor_valuebox_ch::visit_predefined_type (be_predefined_type *node)
{
  return this->emit_for_predef_enum (
    node,
    "",
    node->pt () == AST_PredefinedType::PT_any);
}


int
be_visitor_valuebox_ch::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the node being visited by this be_visitor_valuebox_ch.
  be_decl * vb_node = this->ctx_->node ();

  if (node->anonymous ())
    { // Our sequence is anonymous so we must generate a declaration
      // for it.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // First generate the sequence declaration.
      be_visitor_sequence_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuebox_ch::"
                             "visit_sequence - "
                             "codegen failed\n"),
                            -1);
        }
    }

  // Retrieve the base type since we will need to do some code
  // generation for it.
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_ch::"
                         "visit_sequence - "
                         "Bad element type\n"),
                        -1);
    }

  // Indicate that this type has been used as a sequence element.
  bt->seen_in_sequence (true);

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  this->emit_default_constructor();

  if (node->unbounded ())
    {
      // Public constructor with one argument of type ULong
      *os << vb_node->local_name () << " ( ::CORBA::ULong max);" << be_nl;
    }

  // Public constructor for sequence with supplied buffer
  *os << vb_node->local_name () << " (" << be_idt << be_idt;

  if (node->unbounded ())
    {
      *os << be_nl << "::CORBA::ULong max,";
    }

  *os << be_nl << "::CORBA::ULong length," << be_nl;

  // Generate the base type for the buffer.
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH);
  be_visitor_sequence_buffer_type bt_visitor (&ctx);
  if (bt->accept (&bt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%N:%l) be_visitor_valuebox_ch::visit_sequence - "
                         "base type visit failed\n"),
                        -1);
    }

  *os << " * buf," << be_nl
      << "::CORBA::Boolean release = false" << be_uidt_nl << ");"
      << be_uidt_nl;

  // Public constructor with single argument of type const T&
  this->emit_constructor_one_arg (node, "", "const ", "&");

  // Public copy constructor
  this->emit_copy_constructor ();

  *os << be_nl << "// assignment operator" << be_nl;

  // Public assignment operator with one argument of type const T&
  this->emit_assignment (node, "", "const ", "&");

  // Public accessor and modifier methods
  this->emit_accessor_modifier (node);

  // Access to the boxed value for method signatures
  this->emit_boxed_access (node, "", "const ", "&", "*");

  // Generate accessors

  // Generate base type for sequence then remainder of operator []
  if (bt->accept (&bt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          " (%N:%l) be_visitor_valuebox_ch::"
                           "visit_sequence - "
                          "base type visit failed\n"),
                         -1);
    }
  *os << "& operator[] ( ::CORBA::ULong index);" << be_nl;

  // Generate base type for sequence then remainder of operator []
  *os << "const ";

  if (bt->accept (&bt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%N:%l) be_visitor_valuebox_ch::"
                         "visit_sequence - "
                         "base type visit failed\n"),
                        -1);
    }

  *os << "& operator[] ( ::CORBA::ULong index) const;" << be_nl_2
      << "::CORBA::ULong maximum (void) const;" << be_nl
      << "::CORBA::ULong length (void) const;" << be_nl
      << "void length ( ::CORBA::ULong len);" << be_nl_2;

  // Member variable of underlying type;
  this->emit_boxed_member_var (node, "_var");

  return 0;
}


int
be_visitor_valuebox_ch::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  const char *string_type;
  const char *char_type;

  if (node->node_type () == AST_Decl::NT_string)
    {
      string_type = "String";
      char_type = "char";
    }
  else if (node->node_type () == AST_Decl::NT_wstring)
    {
      string_type = "WString";
      char_type = "::CORBA::WChar";
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "(%N:%l) be_visitor_valuebox_ch::visit_string -"
                  "unexpected string node type=%d\n", node->node_type ()));
      return -1;
    }

  // Retrieve the node being visited by this be_visitor_valuebox_ch.
  be_decl * vb_node = this->ctx_->node ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  this->emit_default_constructor ();
  this->emit_constructor_one_arg (node, "", "", "");

  // Public constructor with one argument of type const char *
  this->emit_constructor_one_arg (node, "", "const ", "");

  // Public constructor with one argument of type const CORBA::String_var&
  *os << vb_node->local_name () << " (const ::CORBA::" << string_type
      << "_var& var);" << be_nl;

  // Public copy constructor
  this->emit_copy_constructor ();

  *os << "// assignment operators" << be_nl;

  // Public assignment operator with one argument of type char *
  this->emit_assignment (node, "", "", "");

  // Public assignment operator with one argument of type const char *
  this->emit_assignment (node, "", "const ", "");

  // Public assignment operator with one argument of type
  // const CORBA::String_var&
  *os << vb_node->local_name () << "& operator= (const ::CORBA::"
      << string_type << "_var& var);" << be_nl_2;

  *os << "// Accessor" << be_nl;

  // Accessor function takes no arguments and returns a const char *
  *os << "const " << node->full_name ()
      << " _value (void) const;" << be_nl_2;

  *os << "// Modifiers" << be_nl;

  // Modifier function with one argument of type char *
  *os << "void _value (" << node->full_name () << " val);" << be_nl;

  // Modifier function with one argument of type const char *
  *os << "void _value (const " << node->full_name () << " val);"
      << be_nl;

  // Modifier function with one argument of type const CORBA::String_var&
  *os << "void _value (const ::CORBA::" << string_type << "_var& var);"
      << be_nl_2;

  // Access to the boxed value for method signatures
  this->emit_boxed_access (node, "", "const ", "", "");

  // Overloaded subscript operators
  *os << "// Allows access and modification using a slot." << be_nl
      << char_type << " & operator[] ( ::CORBA::ULong slot);"
      << be_nl_2
      << "// Allows only accessing thru a slot." << be_nl
      << char_type << " operator[] ( ::CORBA::ULong slot) const;"
      << be_nl;

  // Member variable of underlying type;
  *os << be_uidt_nl << "private:" << be_idt_nl
      << "::CORBA::" << string_type << "_var" << " _pd_value;" << be_nl;

  return 0;
}


int
be_visitor_valuebox_ch::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  this->emit_default_constructor();

  // Public constructor with one argument of const T&
  this->emit_constructor_one_arg (node, "", "const ", "&");

  // Public copy constructor
  this->emit_copy_constructor ();

  *os << be_nl << "// Assignment operator" << be_nl;

  // Public assignment operator with one argument of type const T&
  this->emit_assignment (node, "", "const ", "&");

  // Public accessor and modifier methods
  this->emit_accessor_modifier (node);

  // Access to the boxed value for method signatures
  if (node->size_type() == AST_Type::FIXED)
    {
      this->emit_boxed_access (node, "", "const ", "&", "");
    }
  else
    {
      this->emit_boxed_access (node, "", "const ", "&", "*");
    }

  // Now generate the accessor and modifier functions for each struct
  // member.  These functions have the same signatures as
  // acessor and modifier functions for union members.

  AST_Decl *d = 0;
  AST_Field *field = 0;
  be_type *bt = 0;
  be_visitor_context ctx (*this->ctx_);

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
        si.next ())
    {
      d = si.item ();

      if (d == 0 || (field = AST_Field::narrow_from_decl (d)) == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) be_visitor_valuebox_ch::visit_structure -"
                      "bad node in this scope\n"));
          break;
        }

      bt = be_type::narrow_from_decl (field->field_type ());

      // Set the node to be visited
      ctx.node (be_decl::narrow_from_decl (d));

      // Create a visitor and use that to process the type.
      be_visitor_valuebox_field_ch visitor (&ctx);

      if (bt->accept (&visitor) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) be_visitor_valuebox_ch::visit_structure"
                      " - codegen for struct type failed\n"));
        }
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Member variable of underlying type;
  this->emit_boxed_member_var (node, "_var");

  return 0;
}


int
be_visitor_valuebox_ch::visit_typedef (be_typedef *node)
{
  // Make a decision based on the primitive base type.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%N:%l) be_visitor_valuebox_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  return 0;
}


int
be_visitor_valuebox_ch::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  this->emit_default_constructor();

  // Public constructor with single argument of type const T&
  this->emit_constructor_one_arg (node, "", "const ", "&");

  // Public copy constructor
  this->emit_copy_constructor ();

  *os << be_nl << "// Assignment operator" << be_nl;

  // Public assignment operator with one argument of type const T&
  this->emit_assignment (node, "", "const ", "&");

  // Public accessor and modifier methods
  this->emit_accessor_modifier (node);

  // Access to the boxed value for method signatures
  if (node->size_type() == AST_Type::FIXED)
    {
      this->emit_boxed_access (node, "", "const ", "&", "");
    }
  else
    {
      this->emit_boxed_access (node, "", "const ", "&", "*");
    }

  // Now generate the accessor and modifier functions for each union
  // member.

  AST_Decl *d = 0;
  AST_Field *field = 0;
  be_type *bt = 0;
  be_visitor_context ctx (*this->ctx_);

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
        si.next ())
    {
      d = si.item ();

      if (d == 0 || (field = AST_Field::narrow_from_decl (d)) == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) be_visitor_valuebox_ch::visit_union -"
                      "bad node in this scope\n"));
          break;
        }

      bt = be_type::narrow_from_decl (field->field_type ());

      // Set the node to be visited
      ctx.node (be_decl::narrow_from_decl (d));

      // Create a visitor and use that to process the type.
      be_visitor_valuebox_field_ch visitor (&ctx);

      if (bt->accept (&visitor) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) be_visitor_valuebox_ch::visit_union"
                      " - codegen for struct type failed\n"));
        }
    }

  // Retrieve the disriminant type.
  bt = be_type::narrow_from_decl (node->disc_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuebox_ch::visit_union - "
                         "bad disciminant type\n"),
                        -1);
    }

  // The discriminant type may have to be defined here if it was an enum
  // declaration inside of the union statement.

  be_visitor_union_discriminant_ch ud_visitor (&ctx);

  if (bt->accept (&ud_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_ch::"
                         " visit_union - "
                         "codegen for discriminant failed\n"),
                        -1);
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;


  // Member variable of underlying type;
  this->emit_boxed_member_var (node, "_var");

  return 0;
}


int
be_visitor_valuebox_ch::emit_for_predef_enum (be_type *node,
                                              const char * type_suffix,
                                              bool is_any)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  this->emit_default_constructor ();

  if (is_any)
    {
      this->emit_constructor_one_arg (node, "", "const ", "&");
    }
  else
    { // Public constructor with one argument of underlying type
      this->emit_constructor_one_arg (node, type_suffix, "", "");
    }

  // Public copy constructor
  this->emit_copy_constructor ();

  *os << be_nl << "// Assignment operator" << be_nl;

  if (is_any)
    { // Public assignment operator with one argument of type const T&
      this->emit_assignment (node, "", "const ::", "&");

      this->emit_accessor_modifier (node);

      // Access to the boxed value for method signatures
      this->emit_boxed_access (node, "", "const ::", "&", "*");

      // Member variable of underlying type;
      this->emit_boxed_member_var (node, "_var");
    }
  else
    { // Public assignment operator with one argument of underlying type
      this->emit_assignment (node, type_suffix, "", "");

      // Public accessor and modifier methods
      *os << "// Accessor and modifier" << be_nl;

      *os << node->full_name () << type_suffix
          << " _value (void) const;" << be_nl;

      *os << "void" << " _value (" << node->full_name () << type_suffix
          << " val);" << be_nl_2;

      // Access to the boxed value for method signatures
      this->emit_boxed_access (node, type_suffix, "", "", "");

      // Member variable of underlying type;
      this->emit_boxed_member_var (node, type_suffix);
    }

  return 0;
}

void
be_visitor_valuebox_ch::emit_default_constructor (void)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the node being visited by this be_visitor_valuebox_ch.
  be_decl * vb_node = this->ctx_->node ();

  *os << be_nl_2 << "// Constructors" << be_nl;

  // Public default constructor
  *os << vb_node->local_name () << " (void);" << be_nl;
}

void
be_visitor_valuebox_ch::emit_constructor_one_arg (be_decl *node,
                                                  const char * type_suffix,
                                                  const char * const_prefix,
                                                  const char * ref_modifier)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the node being visited by this be_visitor_valuebox_ch.
  be_decl * vb_node = this->ctx_->node ();

  // Public constructor with one argument of underlying type
  *os << vb_node->local_name () << " (" << const_prefix << node->full_name ()
      << type_suffix << ref_modifier << " val);" << be_nl;
}

void
be_visitor_valuebox_ch::emit_copy_constructor (void)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the node being visited by this be_visitor_valuebox_ch.
  be_decl * vb_node = this->ctx_->node ();

  // Public Copy constructor
  *os << vb_node->local_name ()
      << " (const " << vb_node->local_name () << "& val);" << be_nl;
}

void
be_visitor_valuebox_ch::emit_assignment (be_decl *node,
                                         const char * type_suffix,
                                         const char * const_prefix,
                                         const char * ref_modifier)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the node being visited by this be_visitor_valuebox_ch.
  be_decl * vb_node = this->ctx_->node ();

  *os << vb_node->local_name () << "& operator= (" << const_prefix
      << node->full_name () << type_suffix << ref_modifier << " val);"
      << be_nl_2;
}

void
be_visitor_valuebox_ch::emit_boxed_access (be_decl *node,
                                           const char * type_suffix,
                                           const char *const_prefix,
                                           const char *in_ref_modifier,
                                           const char *out_ref_modifier)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "// Access to the boxed value for method signatures" << be_nl;

  // Access to the boxed value for method signatures
  *os << const_prefix << node->full_name () << type_suffix << in_ref_modifier
      << " _boxed_in (void) const;" << be_nl
      << node->full_name () << type_suffix << "&"
      << " _boxed_inout (void);" << be_nl
      << node->full_name () << type_suffix << out_ref_modifier << "&"
      << " _boxed_out (void);" << be_nl;
}

void
be_visitor_valuebox_ch::emit_accessor_modifier (be_decl *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "// Accessors and modifier" << be_nl;

  // Public accessor method (const)
  *os << "const " << node->full_name () << "& _value (void) const;"
      << be_nl;

  // Public accessor method
  *os << node->full_name () << "& _value (void);" << be_nl;

  // Public modifier method
  *os << "void" << " _value (const " << node->full_name () << "& val);"
      << be_nl_2;
}


void
be_visitor_valuebox_ch::emit_boxed_member_var (be_decl *node,
                                               const char * type_suffix)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Member variable of underlying type;
  *os << be_uidt_nl << "private:" << be_idt_nl
      << node->full_name () << type_suffix << " _pd_value;" << be_nl;
}
