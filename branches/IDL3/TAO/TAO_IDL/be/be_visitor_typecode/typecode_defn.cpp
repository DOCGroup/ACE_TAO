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

ACE_RCSID (be_visitor_typecode, 
           typecode_defn, 
           "$Id$")

// This is an implementation of C++ "scoped lock" idiom in order to
// avoid repetitive code.
class Scoped_Compute_Queue_Guard
{
public:
  Scoped_Compute_Queue_Guard (be_visitor_typecode_defn* );
  ~Scoped_Compute_Queue_Guard (void);

private:
  be_visitor_typecode_defn* customer_;
};

Scoped_Compute_Queue_Guard::Scoped_Compute_Queue_Guard (
    be_visitor_typecode_defn* customer)
    :customer_ (customer)
{
  if (customer_ != 0)
    {
      // Reset the compute queue to set the stage for computing our
      // encapsulation length.
      customer_->queue_reset (customer_->compute_queue_);
    }
}

Scoped_Compute_Queue_Guard::~Scoped_Compute_Queue_Guard (void)
{
  if (customer_ != 0)
    {
      // Reset the compute queue since we must not affect computation of other
      // nodes.
      customer_->queue_reset (customer_->compute_queue_);
    }
}

// ******************************************************
// TypeCode Definitions
// ******************************************************

be_visitor_typecode_defn::be_visitor_typecode_defn (be_visitor_context *ctx)
    : be_visitor_scope (ctx),
      computed_tc_size_ (0),
      computed_encap_len_ (0),
      computed_scope_encap_len_ (0),
      tc_offset_ (0),
      index_ (-1)

{
}

be_visitor_typecode_defn::~be_visitor_typecode_defn (void)
{
  this->queue_reset (this->tc_queue_);
  this->queue_reset (this->compute_queue_);
}

int
be_visitor_typecode_defn::visit_members (AST_Structure *node)
{
  this->elem_number_ = 0;
  AST_Field **member_ptr = 0;
  size_t count = node->nfields ();
  be_decl *enclosing = be_decl::narrow_from_decl (node);

  for (size_t i = 0; i < count; ++i)
    {
      node->field (member_ptr, i);

      be_decl *bd = be_decl::narrow_from_decl (*member_ptr);

      // Set the node to be visited.
      this->ctx_->node (bd);

      // Reset this with every iteration - it might be changed
      // when it comes around again.
      this->ctx_->scope (enclosing);

      this->elem_number_++;

      // Do any pre processing using the next item info.
      if (this->pre_process (bd) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_typecode_defn::visit_members - "
              "pre processing failed\n"
            ),
            -1
          );
        }

      // Send the visitor.
      if (bd == 0 || bd->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_typecode_defn::visit_members - "
              "codegen for scope failed\n"
            ),
            -1
          );

        }

      // Do any post processing using this item info.
      if (this->post_process (bd) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_typecode_defn::visit_members - "
              "post processing failed\n"
            ),
            -1
          );
        }
    }

  return 0;
}

int
be_visitor_typecode_defn::visit_members (be_valuetype *node)
{
  this->elem_number_ = 0;

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl *d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typecode_defn::visit_members - "
                             "bad node in this scope\n"), 
                            -1);
        }

      AST_Field *field = AST_Field::narrow_from_decl (d);

      if (!field)
        {
          continue;
        }

      be_decl *bd = be_decl::narrow_from_decl (d);

      // Set the scope node as "node" in which the code is being
      // generated so that elements in the node's scope can use it
      // for code generation
      this->ctx_->scope (node->decl ());

      // Set the node to be visited.
      this->ctx_->node (bd);
      this->elem_number_++;

      // Do any pre processing using the next item info.
      if (this->pre_process (bd) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typecode_defn::"
                             "visit_members - "
                             "pre processing failed\n"
                             ), 
                            -1);
        }

      // Send the visitor.
      if (bd == 0 || bd->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typecode_defn::"
                             "visit_members - "
                             "codegen for scope failed\n"
                             ), -1);
        }

      // Do any post processing using this item info.
      if (this->post_process (bd) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typecode_defn::"
                             "visit_members - "
                             "post processing failed\n"
                             ), -1);
        }
    }

  return 0;
}


// The following needs to be done to deal until the MSVC compiler's broken
// handling of namespaces is fixed (hopefully forthcoming in version 7).
int
be_visitor_typecode_defn::gen_nested_namespace_begin (be_module *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  char *item_name = 0;

  for (UTL_IdListActiveIterator i (node->name ()); !i.is_done (); i.next ())
    {
      item_name = i.item ()->get_string ();

      if (ACE_OS::strcmp (item_name, "") != 0)
        {
          // Leave the outermost root scope.
          *os << "TAO_NAMESPACE_BEGIN (" << item_name
              << ")" << be_nl;
        }
    }

  return 0;
}

// The following needs to be done to deal until the MSVC compiler's broken
// handling of namespaces is fixed (hopefully forthcoming in version 7).
int
be_visitor_typecode_defn::gen_nested_namespace_end (be_module *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  for (UTL_IdListActiveIterator i (node->name ()); !i.is_done (); i.next ())
    {
      if (ACE_OS::strcmp (i.item ()->get_string (), "") != 0)
        {
          // Leave the outermost root scope.
          *os << "TAO_NAMESPACE_END" << be_nl;
        }
    }

  return 0;
}

// The visit methods will be called for the top-level node whose typecode is
// being generated.

int
be_visitor_typecode_defn::visit_type (be_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // reset the queue
  this->queue_reset (this->tc_queue_);
  this->tc_offset_ = 0;

  // Insert node into tc_queue_ in case the node is involved in
  // some form of recursion.
  if (this->queue_insert (this->tc_queue_, node, this->tc_offset_) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typecode_defn::"
                         "visit_type - "
                         "queue insert failed\n"),
                        -1);
    }

  os->indent (); // start from current indentation level

  // Generate the typecode information here
  *os << "static const CORBA::Long _oc_";

  // Flat name generation.
  *os << node->flat_name ();

  *os << "[] =" << be_nl;
  *os << "{" << be_idt << "\n";

  // Add the sizeof the enum tk_* and the encap length that we do not put into
  // this array but which will exist in the CDR buffer.

  this->tc_offset_ = 4 + 4;

  // Note that we just need the parameters here and hence we generate the
  // encapsulation for the parameters.
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION);

  if (node->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typecode_defn::"
                         "visit_type - "
                         "codegen for typecode encapsulation failed\n"),
                        -1);
    }
  *os << be_uidt << "};" << be_nl << be_nl;

  // Type code definition.
  *os << "static CORBA::TypeCode _tc_TAO_tc_";

  // Flat name generation.
  *os << node->flat_name ();

  *os << " (" << be_idt << be_idt_nl;

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
      {
        // Yet another fruit of interface being a valuetype sometimes :-(
        AST_Interface* iface = AST_Interface::narrow_from_decl (node);
        if (iface != 0 && iface->is_valuetype ())
          {
            *os << "CORBA::tk_value";
          }
        else
          {
            *os << "CORBA::tk_objref";
          }
       break;
      }
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

  *os << "," << be_nl
      << "sizeof (_oc_";

  // Flat name generation.
  *os <<  node->flat_name ();

  *os << ")," << be_nl
      << "(char *) &_oc_";

  // Flat name generation.
  *os <<  node->flat_name ();

  // Name generation.
  *os << "," << be_nl
      << "0," << be_nl
      << "sizeof (" << node->name () << ")" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  // Is our enclosing scope a module? We need this check because for
  // platforms that support namespaces, the typecode must be declared
  // extern.
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
      *os << "TAO_NAMESPACE_DEFINE (::CORBA::TypeCode_ptr, _tc_";

      // Local name generation.
      *os << node->local_name ();

      *os << ", &_tc_TAO_tc_";

      // Flat name generation.
      *os << node->flat_name ();

      *os << ")" << be_nl;

      if (this->gen_nested_namespace_end (module) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_typecode_defn::visit_type - "
                             "Error parsing nested name\n"),
                            -1);
        }

      *os << be_nl;
    }
  else
    {
      // outermost scope.
      *os << "::CORBA::TypeCode_ptr ";

      // Tc name generation.
      *os << node->tc_name ();

      *os << " =" << be_idt_nl
          << "&_tc_TAO_tc_";

      // Flat name generation.
      *os <<  node->flat_name ();

      *os << ";" << be_uidt_nl << "\n";
    }

  return 0;
}

int
be_visitor_typecode_defn::visit_array (be_array *node)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE:
      return this->visit_type (node);
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED:
      return this->gen_typecode (node);

    case TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION:
      return this->gen_encapsulation (node);

    case TAO_CodeGen::TAO_TC_DEFN_TC_SIZE:
      this->computed_tc_size_ = this->compute_tc_size (node);
      return ((this->computed_tc_size_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN:
      this->computed_encap_len_ = this->compute_encap_length (node);
      return ((this->computed_encap_len_ > 0) ? 0 : -1);
    default:
      // error
      break;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                     ACE_TEXT ("visit - bad sub state ")
                     ACE_TEXT ("in visitor context\n")),
                    -1);
}

int
be_visitor_typecode_defn::visit_enum (be_enum *node)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE:
      return this->visit_type (node);
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED:
      return this->gen_typecode (node);

    case TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION:
      return this->gen_encapsulation (node);

    case TAO_CodeGen::TAO_TC_DEFN_TC_SIZE:
      this->computed_tc_size_ = this->compute_tc_size (node);
      return ((this->computed_tc_size_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN:
      this->computed_encap_len_ = this->compute_encap_length (node);
      return ((this->computed_encap_len_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE:
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN:
      return this->visit_scope (node);
    default:
      // error
      break;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                     ACE_TEXT ("visit - bad sub state ")
                     ACE_TEXT ("in visitor context\n")),
                    -1);
}

int
be_visitor_typecode_defn::visit_enum_val (be_enum_val *node)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE:
      return this->gen_encapsulation (node);
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN:
      this->computed_encap_len_ = this->compute_encap_length (node);
      return ((this->computed_encap_len_ > 0) ? 0 : -1);
    default:
      // error
      break;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                     ACE_TEXT ("visit - bad sub state ")
                     ACE_TEXT ("in visitor context\n")),
                    -1);
}

int
be_visitor_typecode_defn::visit_exception (be_exception *node)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE:
      return this->visit_type (node);
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED:
      return this->gen_typecode (node);

    case TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION:
      return this->gen_encapsulation (node);

    case TAO_CodeGen::TAO_TC_DEFN_TC_SIZE:
      this->computed_tc_size_ = this->compute_tc_size (node);
      return ((this->computed_tc_size_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN:
      this->computed_encap_len_ = this->compute_encap_length (node);
      return ((this->computed_encap_len_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE:
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN:
      return this->visit_members (node);
    default:
      // error
      break;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                     ACE_TEXT ("visit - bad sub state ")
                     ACE_TEXT ("in visitor context\n")),
                    -1);
}

int
be_visitor_typecode_defn::visit_field (be_field *node)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE:
      return this->gen_encapsulation (node);
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN:
      this->computed_encap_len_ = this->compute_encap_length (node);
      return ((this->computed_encap_len_ > 0) ? 0 : -1);
    default:
      // error
      break;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                     ACE_TEXT ("visit - bad sub state ")
                     ACE_TEXT ("in visitor context\n")),
                    -1);
}

int
be_visitor_typecode_defn::visit_interface (be_interface *node)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE:
      return this->visit_type (node);

    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED:
      return this->gen_typecode (node);

    case TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION:
      return this->gen_encapsulation (node);

    case TAO_CodeGen::TAO_TC_DEFN_TC_SIZE:
      this->computed_tc_size_ = this->compute_tc_size (node);
      return ((this->computed_tc_size_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN:
      this->computed_encap_len_ = this->compute_encap_length (node);
      return ((this->computed_encap_len_ > 0) ? 0 : -1);
    default:
      // error
      break;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                     ACE_TEXT ("visit - bad sub state ")
                     ACE_TEXT ("in visitor context\n")),
                    -1);
}

int
be_visitor_typecode_defn::visit_interface_fwd (be_interface_fwd *)
{
  // nothing to do
  return 0;
}

int
be_visitor_typecode_defn::visit_predefined_type (be_predefined_type *node)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE:
      // top level typecodes are defined in the CORBA library. If we show up
      // here, then it is an error
      break;
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED:
      return this->gen_typecode (node);

    case TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION:
      return this->gen_encapsulation (node);

    case TAO_CodeGen::TAO_TC_DEFN_TC_SIZE:
      this->computed_tc_size_ = this->compute_tc_size (node);
      return ((this->computed_tc_size_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN:
      this->computed_encap_len_ = this->compute_encap_length (node);
      return ((this->computed_encap_len_ > 0) ? 0 : -1);
    default:
      // error
      break;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                     ACE_TEXT ("visit - bad sub state ")
                     ACE_TEXT ("in visitor context\n")),
                    -1);
}

int
be_visitor_typecode_defn::visit_sequence (be_sequence *node)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE:
      return this->visit_type (node);
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED:
      return this->gen_typecode (node);

    case TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION:
      return this->gen_encapsulation (node);

    case TAO_CodeGen::TAO_TC_DEFN_TC_SIZE:
      this->computed_tc_size_ = this->compute_tc_size (node);
      return ((this->computed_tc_size_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN:
      this->computed_encap_len_ = this->compute_encap_length (node);
      return ((this->computed_encap_len_ > 0) ? 0 : -1);
    default:
      // error
      break;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                     ACE_TEXT ("visit - bad sub state ")
                     ACE_TEXT ("in visitor context\n")),
                    -1);
}

int
be_visitor_typecode_defn::visit_string (be_string *node)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE:
      // top level typecode for string is not permitted. It has to be a
      // typedefed string
      break;
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED:
      return this->gen_typecode (node);

    case TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION:
      return this->gen_encapsulation (node);

    case TAO_CodeGen::TAO_TC_DEFN_TC_SIZE:
      this->computed_tc_size_ = this->compute_tc_size (node);
      return ((this->computed_tc_size_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN:
      this->computed_encap_len_ = this->compute_encap_length (node);
      return ((this->computed_encap_len_ > 0) ? 0 : -1);
    default:
      // error
      break;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                     ACE_TEXT ("visit - bad sub state ")
                     ACE_TEXT ("in visitor context\n")),
                    -1);
}

int
be_visitor_typecode_defn::visit_structure (be_structure *node)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE:
      return this->visit_type (node);
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED:
      return this->gen_typecode (node);

    case TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION:
      return this->gen_encapsulation (node);

    case TAO_CodeGen::TAO_TC_DEFN_TC_SIZE:
      this->computed_tc_size_ = this->compute_tc_size (node);
      return ((this->computed_tc_size_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN:
      this->computed_encap_len_ = this->compute_encap_length (node);
      return ((this->computed_encap_len_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE:
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN:
      return this->visit_members (node);
    default:
      // error
      break;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                     ACE_TEXT ("visit - bad sub state ")
                     ACE_TEXT ("in visitor context\n")),
                    -1);
}

int
be_visitor_typecode_defn::visit_typedef (be_typedef *node)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE:
      return this->visit_type (node);
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED:
      return this->gen_typecode (node);

    case TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION:
      return this->gen_encapsulation (node);

    case TAO_CodeGen::TAO_TC_DEFN_TC_SIZE:
      this->computed_tc_size_ = this->compute_tc_size (node);
      return ((this->computed_tc_size_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN:
      this->computed_encap_len_ = this->compute_encap_length (node);
      return ((this->computed_encap_len_ > 0) ? 0 : -1);
    default:
      // error
      break;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                     ACE_TEXT ("visit - bad sub state ")
                     ACE_TEXT ("in visitor context\n")),
                    -1);
}

int
be_visitor_typecode_defn::visit_union (be_union *node)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE:
      return this->visit_type (node);
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED:
      return this->gen_typecode (node);

    case TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION:
      return this->gen_encapsulation (node);

    case TAO_CodeGen::TAO_TC_DEFN_TC_SIZE:
      this->computed_tc_size_ = this->compute_tc_size (node);
      return ((this->computed_tc_size_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN:
      this->computed_encap_len_ = this->compute_encap_length (node);
      return ((this->computed_encap_len_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE:
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN:
      return this->visit_members (node);
    default:
      // error
      break;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                     ACE_TEXT ("visit - bad sub state ")
                     ACE_TEXT ("in visitor context\n")),
                    -1);
}

int
be_visitor_typecode_defn::visit_valuetype (be_valuetype *node)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE:
      return this->visit_type (node);
    case TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED:
      return this->gen_typecode (node);

    case TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION:
      return this->gen_encapsulation (node);

    case TAO_CodeGen::TAO_TC_DEFN_TC_SIZE:
      this->computed_tc_size_ = this->compute_tc_size (node);
      return ((this->computed_tc_size_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN:
      this->computed_encap_len_ = this->compute_encap_length (node);
      return ((this->computed_encap_len_ > 0) ? 0 : -1);
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE:
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN:
      return this->visit_members (node);
    default:
      // error
      break;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                     ACE_TEXT ("visit - bad sub state ")
                     ACE_TEXT ("in visitor context\n")),
                    -1);
}

int
be_visitor_typecode_defn::visit_union_branch (be_union_branch *node)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE:
      return this->gen_encapsulation (node);
    case TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN:
      this->computed_encap_len_ = this->compute_encap_length (node);
      return ((this->computed_encap_len_ > 0) ? 0 : -1);
    default:
      // error
      break;
    }

  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                     ACE_TEXT ("visit - bad sub state ")
                     ACE_TEXT ("in visitor context\n")),
                    -1);
}

//  methods that actually produce the typecode and the encapsulations


int
be_visitor_typecode_defn::gen_typecode (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  // we do not do typecode optimization for anonymous arrays

  *os << "CORBA::tk_array, // typecode kind" << be_nl;
  // size of the enum
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  {
    Scoped_Compute_Queue_Guard guard (this);

    // emit the encapsulation length
    this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
    if (node->accept (this) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%N:%l) - be_visitor_typecode_defn")
                           ACE_TEXT ("gen_typecode (array) - ")
                           ACE_TEXT ("Failed to get encap length\n")),
                          -1);
      }
  }

  *os << this->computed_encap_len_ << ", // encapsulation length"
      << be_idt << "\n";
  // size of the encap length
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // now emit the encapsulation
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION);
  if (node->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                         ACE_TEXT ("gen_typecode - ")
                         ACE_TEXT ("gen_encapsulation failed for array\n")),
                        -1);
    }

  *os << be_uidt << "\n";

  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_type *bt; // base type
  unsigned long i;

  os->indent (); // start from the current indentation level

  // retrieve the base type
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::gen_encapsulation - "
                         "bad base type\n"),
                        -1);
    }

  {
    Scoped_Compute_Queue_Guard guard (this);

    // compute the typecode size
    this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TC_SIZE);
    if (bt->accept (this) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%N:%l) - be_visitor_typecode_defn")
                           ACE_TEXT ("gen_encapsulation (array) - ")
                           ACE_TEXT ("Failed to get typecode size\n")),
                          -1);
      }
  }

  for (i = 0; i < (node->n_dims () - 1); i++)
    {
      unsigned long rem_encap_len;

      *os << "TAO_ENCAP_BYTE_ORDER, // byte order" << be_nl;
      // size of the encapsulation byte order flag. Although it is 1 byte, the
      // aligned size is 4 bytes
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
      *os << "CORBA::tk_array, // typecode kind" << be_nl;
      // size of the enum
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
      rem_encap_len
        = (node->n_dims () - (i + 1)) * (4 + 4)
        + (node->n_dims () - (i + 2)) * (4 + 4)
        + this->computed_tc_size_;
      *os << rem_encap_len << ", // encapsulation length" << be_idt_nl;
      // size of the encap length
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
    }

  // now generate the typecode of the base type
  *os << "TAO_ENCAP_BYTE_ORDER, // byte order\n";
  // size of the encapsulation byte order flag. Although it is 1 byte, the
  // aligned size is 4 bytes
  this->tc_offset_ += sizeof (ACE_CDR::ULong);
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED);
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::gen_encapsulation - "
                         "base type typecode gen failed\n"),
                        -1);
    }

  os->indent ();
  // now reduce the indentation appropriately
  for (i = (node->n_dims () - 1); i > 0; i--)
    {
      *os << node->dims ()[i] << "," << be_uidt_nl;
      // size of the dimension which is a 4 byte quantity
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
    }
  *os << node->dims ()[0] << ",\n";
  // size of the dimension which is a 4 byte quantity
  this->tc_offset_ += sizeof (ACE_CDR::ULong);
  return 0;
}

int
be_visitor_typecode_defn::gen_typecode (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  // check if we are repeated
  const be_visitor_typecode_defn::QNode *qnode =
    this->queue_lookup (this->tc_queue_, node);
  if (qnode)
    {
      // we are repeated, so we must generate an indirection here
      *os << "0xffffffff, // indirection" << be_nl;
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
      // the offset must point to the tc_kind value of the first occurrence of
      // this type
      os->print ("0x%x, // negative offset (%ld)\n",
                 (qnode->offset - this->tc_offset_),
                 (qnode->offset - this->tc_offset_));
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
    }
  else
    {
      if (be_global->opt_tc ())
        {
          if (this->queue_insert (this->tc_queue_, node, this->tc_offset_) == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_typecode_defn::"
                                 "visit_type - "
                                 "queue insert failed\n"),
                            -1);
            }
        }

      *os << "CORBA::tk_enum, // typecode kind" << be_nl;
      // size of the enum
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      {
        Scoped_Compute_Queue_Guard guard (this);

        // emit the encapsulation length
        this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
        if (node->accept (this) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%N:%l) - be_visitor_typecode_defn")
                               ACE_TEXT ("gen_typecode (enum) - ")
                               ACE_TEXT ("Failed to get encap length\n")),
                              -1);
          }
      }

      *os << this->computed_encap_len_ << ", // encapsulation length"
          << be_idt << "\n";
      // size of the encap length
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      // now emit the encapsulation
      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION);
      if (node->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::gen_typecode (enum) - ")
                             ACE_TEXT ("failed to generate encapsulation\n")),
                            -1);
        }

      *os << be_uidt << "\n";

    }
  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  *os << "TAO_ENCAP_BYTE_ORDER, // byte order" << be_nl;
  // size of the encapsulation byte order flag. Although it is 1 byte, the
  // aligned size is 4 bytes
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // generate repoID
  this->gen_repoID (node);

  // generate name
  os->indent ();
  this->gen_name (node);

  // generate the member count
  os->indent ();
  *os << node->member_count () << ", // member count\n";
  // size of the member length
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // hand over to the scope to generate the typecode for elements
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE);
  if (node->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::gen_encapsulation (enum) - ")
                         ACE_TEXT ("cannot generate typecode for members\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_enum_val *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  // generate name
  this->gen_name (node);

  return 0;
}

int
be_visitor_typecode_defn::gen_typecode (be_exception *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  // check if we are repeated
  const be_visitor_typecode_defn::QNode *qnode =
    this->queue_lookup (this->tc_queue_, node);
  if (qnode)
    {
      // we are repeated, so we must generate an indirection here
      *os << "0xffffffff, // indirection" << be_nl;
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
      // the offset must point to the tc_kind value of the first occurrence of
      // this type
      os->print ("0x%x, // negative offset (%ld)\n",
                 (qnode->offset - this->tc_offset_),
                 (qnode->offset - this->tc_offset_));
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
    }
  else
    {
      // Insert node into tc_queue_ in case the node is involved in
      // some form of recursion.
      if (this->queue_insert (this->tc_queue_, node, this->tc_offset_) == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typecode_defn::"
                           "visit_type - "
                           "queue insert failed\n"),
                          -1);
      }

      *os << "CORBA::tk_except, // typecode kind" << be_nl;
      // size of the enum
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      {
        Scoped_Compute_Queue_Guard guard (this);

      // emit the encapsulation length
        this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
        if (node->accept (this) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%N:%l) - be_visitor_typecode_defn")
                               ACE_TEXT ("gen_typecode (exception) - ")
                               ACE_TEXT ("Failed to get encap length\n")),
                              -1);
          }
      }

      *os << this->computed_encap_len_ << ", // encapsulation length"
          << be_idt << "\n";
      // size of the encap length
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      // now emit the encapsulation
      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION);

      if (node->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::gen_typecode (exception) - ")
                             ACE_TEXT ("failed to generate encapsulation\n")),
                            -1);
        }

      *os << be_uidt << "\n";
    }
  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_exception *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  *os << "TAO_ENCAP_BYTE_ORDER, // byte order" << be_nl;
  // size of the encapsulation byte order flag. Although it is 1 byte, the
  // aligned size is 4 bytes
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // generate repoID
  this->gen_repoID (node);

  // generate name
  os->indent ();
  this->gen_name (node);

  // generate the member count
  os->indent ();
  *os << node->nfields () << ", // member count\n";
  // size of the member count
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // hand over to the scope to generate the typecode for elements
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE);

  // Set the scope node as "node" in which the code is being
  // generated so that elements in the node's scope can use it
  // for code generation.
  this->ctx_->scope (node);

  if (node->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::gen_encapsulation (exception) - ")
                         ACE_TEXT ("cannot generate typecode for members\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_field *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_type *bt;  // our type node

  os->indent (); // start from whatever indentation level we were at

  // generate name
  this->gen_name (node);

  // hand over code generation to our type node
  bt = be_type::narrow_from_decl (node->field_type ());
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED);
  if (!bt || bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::gen_encapsulation (field) - ")
                         ACE_TEXT ("failed to generate typecode\n")),
                        -1);
    }
  // revert the state to what it was before because we may be dealing with
  // subsequent fields for which we have to be in the "gen scope"
  // substate
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE);

  if (node->visibility() != AST_Field::vis_NA)
    {
      // generate visibility marker

      // Even though visibility marker is UShort it seems that
      // it would always be aligned on ULong boundary.
      ACE_CDR::ULong visibility =
        node->visibility() == AST_Field::vis_PRIVATE ? 0 : 1;

      os->indent (); // start from current indentation level
      *os << visibility << ", // data memeber visibility marker"
          << "\n\n";

      this->tc_offset_ += sizeof (ACE_CDR::ULong);
    }

  return 0;
}

int
be_visitor_typecode_defn::gen_typecode (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  // check if we are repeated
  const be_visitor_typecode_defn::QNode *qnode =
    this->queue_lookup (this->tc_queue_, node);
  if (qnode)
    {
      // we are repeated, so we must generate an indirection here
      *os << "0xffffffff, // indirection" << be_nl;
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
      // the offset must point to the tc_kind value of the first occurrence of
      // this type
      os->print ("0x%x, // negative offset (%ld)\n",
                 (qnode->offset - this->tc_offset_),
                 (qnode->offset - this->tc_offset_));
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
    }
  else
    {
      if (this->queue_insert (this->tc_queue_, node, this->tc_offset_) == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typecode_defn::"
                           "visit_type - "
                           "queue insert failed\n"),
                          -1);
      }

      *os << "CORBA::tk_objref, // typecode kind" << be_nl;
      // size of the enum
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      {
        Scoped_Compute_Queue_Guard guard (this);

        // emit the encapsulation length
        this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
        if (node->accept (this) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%N:%l) - be_visitor_typecode_defn")
                               ACE_TEXT ("gen_typecode (interface) - ")
                               ACE_TEXT ("Failed to get encap length\n")),
                              -1);
          }
      }

      *os << this->computed_encap_len_ << ", // encapsulation length"
          << be_idt << "\n";
      // size of the encap length
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      // now emit the encapsulation
      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION);
      if (node->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::gen_typecode (interface) - ")
                             ACE_TEXT ("failed to generate encapsulation\n")),
                            -1);
        }
      *os << be_uidt << "\n";
    }
  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  *os << "TAO_ENCAP_BYTE_ORDER, // byte order" << be_nl;
  // size of the encapsulation byte order flag. Although it is 1 byte, the
  // aligned size is 4 bytes
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // generate repoID.
  this->gen_repoID (node);

  // generate name.
  os->indent ();
  this->gen_name (node);

  return 0;
}

int
be_visitor_typecode_defn::gen_typecode (be_interface_fwd *)
{
  // nothing to do here
  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_interface_fwd *)
{
  // nothing to be done
  return 0;
}

int
be_visitor_typecode_defn::gen_typecode (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from the current indentation level

  // size of the enum
  this->tc_offset_ += sizeof (ACE_CDR::ULong);
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_void:
      *os << "CORBA::tk_void,\n\n";
      break;
    case AST_PredefinedType::PT_short:
      *os << "CORBA::tk_short,\n\n";
      break;
    case AST_PredefinedType::PT_ushort:
      *os << "CORBA::tk_ushort,\n\n";
      break;
    case AST_PredefinedType::PT_long:
      *os << "CORBA::tk_long,\n\n";
      break;
    case AST_PredefinedType::PT_ulong:
      *os << "CORBA::tk_ulong,\n\n";
      break;
    case AST_PredefinedType::PT_longlong:
      *os << "CORBA::tk_longlong,\n\n";
      break;
    case AST_PredefinedType::PT_ulonglong:
      *os << "CORBA::tk_ulonglong,\n\n";
      break;
    case AST_PredefinedType::PT_float:
      *os << "CORBA::tk_float,\n\n";
      break;
    case AST_PredefinedType::PT_double:
      *os << "CORBA::tk_double,\n\n";
      break;
    case AST_PredefinedType::PT_longdouble:
      *os << "CORBA::tk_longdouble,\n\n";
      break;
    case AST_PredefinedType::PT_boolean:
      *os << "CORBA::tk_boolean,\n\n";
      break;
    case AST_PredefinedType::PT_char:
      *os << "CORBA::tk_char,\n\n";
      break;
    case AST_PredefinedType::PT_octet:
      *os << "CORBA::tk_octet,\n\n";
      break;
    case AST_PredefinedType::PT_any:
      *os << "CORBA::tk_any,\n\n";
      break;
    case AST_PredefinedType::PT_wchar:
      *os << "CORBA::tk_wchar,\n\n";
      break;
    case AST_PredefinedType::PT_pseudo:
      {
        if (!ACE_OS::strcmp (node->local_name ()->get_string (), "TypeCode"))
          *os << "CORBA::tk_TypeCode,\n\n";
        else
          if (!ACE_OS::strcmp (node->local_name ()->get_string (), "Object"))
            {
              // check if we are repeated
              const be_visitor_typecode_defn::QNode *qnode =
                this->queue_lookup (this->tc_queue_, node);
              if (qnode)
                {
                  // we are repeated, so we must generate an indirection here
                  *os << "0xffffffff, // indirection" << be_nl;
                  this->tc_offset_ += sizeof (ACE_CDR::ULong);
                  // the offset must point to the tc_kind value of the first occurrence of
                  // this type
                  os->print ("0x%x, // negative offset (%ld)\n",
                             (qnode->offset - this->tc_offset_),
                             (qnode->offset - this->tc_offset_));
                  this->tc_offset_ += sizeof (ACE_CDR::ULong);
                }
              else
                {
                  // Insert node into tc_queue_ in case the node is involved in
                  // some form of recursion.
                  if (this->queue_insert (this->tc_queue_,
                                          node,
                                          this->tc_offset_) == 0)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "(%N:%l) be_visitor_typecode_defn::"
                                       "visit_type - "
                                       "queue insert failed\n"),
                                      -1);
                  }

                  *os << "CORBA::tk_objref," << be_nl;

                  {
                    Scoped_Compute_Queue_Guard guard (this);

                    // emit the encapsulation length
                    this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
                    if (node->accept (this) == -1)
                      {
                        ACE_ERROR_RETURN ((
                            LM_ERROR,
                            ACE_TEXT ("(%N:%l) - be_visitor_typecode_defn")
                            ACE_TEXT ("gen_typecode (predefined) - ")
                            ACE_TEXT ("Failed to get encap length\n")), -1);
                      }
                  }

                  *os << this->computed_encap_len_
                      << ", // encapsulation length" << be_idt << "\n";
                  // size of the encap length
                  this->tc_offset_ += sizeof (ACE_CDR::ULong);

                  // now emit the encapsulation
                  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION);
                  if (node->accept (this) == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                                         ACE_TEXT ("::gen_typecode (predefined objref) - ")
                                         ACE_TEXT ("failed to generate encapsulation\n")),
                                        -1);
                    }
                  *os << be_uidt << "\n";
                }
            }
      }
      break;
    }
  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_predefined_type *node)
{
  // this one is valid only for "Object"
  if (!ACE_OS::strcmp (node->local_name ()->get_string (), "Object"))
    {
      TAO_OutStream *os = this->ctx_->stream (); // output stream

      os->indent (); // start from whatever indentation level we were at

      *os << "TAO_ENCAP_BYTE_ORDER, // byte order" << be_nl;
      // size of the encapsulation byte order flag. Although it is 1 byte, the
      // aligned size is 4 bytes
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      // generate repoID
      this->gen_repoID (node);

      // generate name
      os->indent ();
      this->gen_name (node);
    }
  return 0;
}

int
be_visitor_typecode_defn::gen_typecode (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  // no typecode optimization for anonymous sequences

  *os << "CORBA::tk_sequence, // typecode kind" << be_nl;
  // size of the enum
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  {
    Scoped_Compute_Queue_Guard guard (this);

    // emit the encapsulation length
    this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
    if (node->accept (this) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%N:%l) - be_visitor_typecode_defn")
                           ACE_TEXT ("gen_typecode (sequence) - ")
                           ACE_TEXT ("Failed to get encap length\n")),
                          -1);
      }
  }

  *os << this->computed_encap_len_ << ", // encapsulation length"
      << be_idt << "\n";
  // size of the encap length
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

      // now emit the encapsulation
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION);
  if (node->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::gen_typecode (sequence) - ")
                         ACE_TEXT ("failed to generate encapsulation\n")),
                        -1);
    }
  *os << be_uidt << "\n";

  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_type *bt; // base type

  os->indent ();
  *os << "TAO_ENCAP_BYTE_ORDER, // byte order\n";
  // size of the encapsulation byte order flag. Although it is 1 byte, the
  // aligned size is 4 bytes
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // emit typecode of element type
  bt = be_type::narrow_from_decl (node->base_type ());
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED);
  if (!bt || bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::gen_encapsulation (sequence) - ")
                         ACE_TEXT ("failed to generate typecode\n")),
                        -1);
    }

  //  emit the sequence bounds (0 if unbounded)
  os->indent ();
  *os << node->max_size () << ",\n";
  // size of the bound length
  this->tc_offset_ += sizeof (ACE_CDR::ULong);
  return 0;
}

int
be_visitor_typecode_defn::gen_typecode (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  // no typecode optimizations for anonymous strings

  os->indent (); // start from the current indentation level

  // emit the enumeration
  if (node->width () == (long) sizeof (char))
    {
      *os << "CORBA::tk_string, " << be_nl;
    }
  else
    {
      *os << "CORBA::tk_wstring, " << be_nl;
    }

  // size of the enum
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // emit the string bounds (0 if unbounded)
  *os << node->max_size () << ", // string length\n";
  // size of the bounds
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_string *)
{
  // nothing to be done here
  return 0;
}

int
be_visitor_typecode_defn::gen_typecode (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  // check if we are repeated
  const be_visitor_typecode_defn::QNode *qnode =
    this->queue_lookup (this->tc_queue_, node);
  if (qnode)
    {
      // we are repeated, so we must generate an indirection here
      *os << "0xffffffff, // indirection" << be_nl;
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
      // the offset must point to the tc_kind value of the first occurrence of
      // this type
      os->print ("0x%x, // negative offset (%ld)\n",
                 (qnode->offset - this->tc_offset_),
                 (qnode->offset - this->tc_offset_));
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
    }
  else
    {
      // Insert node into tc_queue_ in case the node is involved in
      // some form of recursion.
      if (this->queue_insert (this->tc_queue_, node, this->tc_offset_) == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typecode_defn::"
                           "visit_type - "
                           "queue insert failed\n"),
                          -1);
      }

      *os << "CORBA::tk_struct, // typecode kind" << be_nl;
      // size of the enum
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      {
        Scoped_Compute_Queue_Guard guard (this);

        // emit the encapsulation length
        this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
        if (node->accept (this) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%N:%l) - be_visitor_typecode_defn")
                               ACE_TEXT ("gen_typecode (struct) - ")
                               ACE_TEXT ("Failed to get encap length\n")),
                              -1);
          }
      }

      *os << this->computed_encap_len_ << ", // encapsulation length"
          << be_idt << "\n";
      // size of the encap length
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      // now emit the encapsulation
      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION);
      if (node->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::gen_typecode (structure) - ")
                             ACE_TEXT ("failed to generate encapsulation\n")),
                            -1);
        }
      *os << be_uidt << "\n";
    }
  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  *os << "TAO_ENCAP_BYTE_ORDER, // byte order" << be_nl;
  // size of the encapsulation byte order flag. Although it is 1 byte, the
  // aligned size is 4 bytes
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // generate repoID
  this->gen_repoID (node);

  // generate name
  os->indent ();
  this->gen_name (node);

  // generate the member count
  os->indent ();
  *os << node->nfields () << ", // member count\n";
  // size of the member count
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // hand over to the scope to generate the typecode for elements
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE);

  if (node->accept (this) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_structure: cannot generate typecode for members\n"));
      return -1;
    }

  return 0;
}

int
be_visitor_typecode_defn::gen_typecode (be_typedef *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  // check if we are repeated
  const be_visitor_typecode_defn::QNode *qnode =
    this->queue_lookup (this->tc_queue_, node);
  if (qnode)
    {
      // we are repeated, so we must generate an indirection here
      *os << "0xffffffff, // indirection" << be_nl;
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
      // the offset must point to the tc_kind value of the first occurrence of
      // this type
      os->print ("0x%x, // negative offset (%ld)\n",
                 (qnode->offset - this->tc_offset_),
                 (qnode->offset - this->tc_offset_));
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
    }
  else
    {
      // Insert node into tc_queue_ in case the node is involved in
      // some form of recursion.
      if (this->queue_insert (this->tc_queue_, node, this->tc_offset_) == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typecode_defn::"
                           "visit_type - "
                           "queue insert failed\n"),
                          -1);
      }

      *os << "CORBA::tk_alias, // typecode kind for typedefs" << be_nl;
      // size of the enum
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      {
        Scoped_Compute_Queue_Guard guard (this);

        // emit the encapsulation length
        this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
        if (node->accept (this) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%N:%l) - be_visitor_typecode_defn")
                               ACE_TEXT ("gen_typecode (typedef) - ")
                               ACE_TEXT ("Failed to get encap length\n")),
                              -1);
          }
      }

      *os << this->computed_encap_len_ << ", // encapsulation length"
          << be_idt << "\n";
      // size of the encap length
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      // now emit the encapsulation
      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION);
      if (node->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::gen_typecode (typedef) - ")
                             ACE_TEXT ("failed to generate encapsulation\n")),
                            -1);
        }

      *os << be_uidt << "\n";
    }
  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_typedef *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_type *bt; // base type

  os->indent (); // start from whatever indentation level we were at

  *os << "TAO_ENCAP_BYTE_ORDER, // byte order" << be_nl;
  // size of the encapsulation byte order flag. Although it is 1 byte, the
  // aligned size is 4 bytes
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // generate repoID
  this->gen_repoID (node);

  // generate name
  os->indent ();
  this->gen_name (node);

  // generate typecode for the base type
  bt = be_type::narrow_from_decl (node->base_type ());
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED);
  if (!bt || bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::gen_encapsulation (typedef) - ")
                         ACE_TEXT ("failed to generate typecode\n")),
                        -1);
    }
  return 0;
}

int
be_visitor_typecode_defn::gen_typecode (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  // check if we are repeated
  const be_visitor_typecode_defn::QNode *qnode =
    this->queue_lookup (this->tc_queue_, node);
  if (qnode)
    {
      // we are repeated, so we must generate an indirection here
      *os << "0xffffffff, // indirection" << be_nl;
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
      // the offset must point to the tc_kind value of the first occurrence of
      // this type
      os->print ("0x%x, // negative offset (%ld)\n",
                 (qnode->offset - this->tc_offset_),
                 (qnode->offset - this->tc_offset_));
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
    }
  else
    {
      // Insert node into tc_queue_ in case the node is involved in
      // some form of recursion.
      if (this->queue_insert (this->tc_queue_, node, this->tc_offset_) == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typecode_defn::"
                           "visit_type - "
                           "queue insert failed\n"),
                          -1);
      }

      *os << "CORBA::tk_union, // typecode kind" << be_nl;
      // size of the enum
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      {
        Scoped_Compute_Queue_Guard guard (this);

        // emit the encapsulation length
        this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
        if (node->accept (this) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%N:%l) - be_visitor_typecode_defn")
                               ACE_TEXT ("gen_typecode (union) - ")
                               ACE_TEXT ("Failed to get encap length\n")),
                              -1);
          }
      }

      *os << this->computed_encap_len_ << ", // encapsulation length"
          << be_idt << "\n";
      // size of the encap length
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      // now emit the encapsulation
      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION);
      if (node->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::gen_typecode (union) - ")
                             ACE_TEXT ("failed to generate encapsulation\n")),
                            -1);
        }
      *os << be_uidt << "\n";
    }
  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_type *discrim;

  os->indent (); // start from whatever indentation level we were at

  *os << "TAO_ENCAP_BYTE_ORDER, // byte order" << be_nl;
  // size of the encapsulation byte order flag. Although it is 1 byte, the
  // aligned size is 4 bytes
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // generate repoID
  this->gen_repoID (node);

  // generate name
  os->indent ();
  this->gen_name (node);

  // generate typecode for discriminant
  discrim = be_type::narrow_from_decl (node->disc_type ());
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED);
  if (discrim->accept (this) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_union: cannot generate typecode for discriminant\n"));
      return -1;
    }

  // generate the default used flag
  os->indent ();
  *os << node->default_index () << ", // default used index" << be_nl;
  // size of the default index used
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // generate the member count
  *os << node->nfields () << ", // member count\n";
  // size of the member count
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // hand over to the scope to generate the typecode for elements
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE);

  // Set the scope node as "node" in which the code is being
  // generated so that elements in the node's scope can use it
  // for code generation.
  this->ctx_->scope (node);

  if (node->accept (this) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_union: cannot generate code for members\n"));
      return -1;
    }

  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_union_branch *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_type *bt;  // our type node

  be_union *ub = this->ctx_->be_scope_as_union ();

  ACE_UINT32 buf[1];
  ACE_OS::memset (buf, 0, sizeof (buf));

  os->indent ();

  // Emit the case label value. We use only the first value in the
  // label list even if there are multiple case labels for this node.
  // The TypeCode class does not have any way of dealing with
  // any count but member count. We make sure in the generation of
  // the union's _discriminant(0 function (called by (de)marshaling
  // code) that the same value is returned that is generated here.
  if (node->label (0)->label_kind () == AST_UnionLabel::UL_label)
    {
      AST_Expression *expression = node->label (0)->label_val ();
      AST_Expression::AST_ExprValue *ev = expression->ev ();
      switch (ub->udisc_type ())
        {
          case AST_Expression::EV_char:
            os->print ("ACE_IDL_NCTOHL (0x%02.2x)",
                       (unsigned char)ev->u.cval);
            // size of char aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_bool:
            os->print ("ACE_IDL_NCTOHL (0x%02.2x)",
                       (unsigned char)ev->u.bval);
            // size of bool aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_wchar:
          case AST_Expression::EV_short:
            os->print ("ACE_IDL_NSTOHL (0x%04.4x)",
                       (unsigned short)ev->u.sval);
            // size of short/wchar aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_ushort:
            os->print ("ACE_IDL_NSTOHL (0x%04.4x)",
                       (unsigned short)ev->u.usval);
            // size of unsigned short aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_long:
            os->print ("0x%08.8x", (unsigned long)ev->u.lval);
            // size of long aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_ulong:
            os->print ("0x%08.8x", ev->u.ulval);
            // size of unsigned long aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_enum:
            // enum
            os->print ("0x%08.8x", (unsigned long)ev->u.eval);
            // size of any aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_ulonglong:
          case AST_Expression::EV_longlong:
            // unimplemented yet

          default:
            ACE_ERROR_RETURN ((LM_DEBUG,
                               "be_union_branch: (%N:%l) Label value "
                               "type (%d) is invalid\n", ev->et), -1);
            ACE_NOTREACHED (break;)
        }

              *os << ", // union case label (evaluated value)" << be_nl;
    }
  else
    {
      // default case
      be_union::DefaultValue dv;

      if (ub->default_value (dv) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typecode::"
                             "gen_encapsulation (union_branch) - "
                             "computing default value failed\n"),
                            -1);
        }

      switch (ub->udisc_type ())
        {
          case AST_Expression::EV_char:
            os->print ("ACE_IDL_NCTOHL (0x%02.2x)",
                       (unsigned char)dv.u.char_val);
            // size of bool/char aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_bool:
            os->print ("ACE_IDL_NCTOHL (0x%02.2x)",
                       (unsigned char)dv.u.bool_val);
            // size of bool/char aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_wchar:
            os->print ("ACE_IDL_NSTOHL (0x%04.4x)",
                       (unsigned short)dv.u.wchar_val);
            // size of short/wchar aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_short:
            os->print ("ACE_IDL_NSTOHL (0x%04.4x)",
                       (unsigned short)dv.u.short_val);
            // size of short/wchar aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_ushort:
            os->print ("ACE_IDL_NSTOHL (0x%04.4x)",
                       (unsigned short)dv.u.ushort_val);
            // size of short/wchar aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_long:
            os->print ("0x%08.8x", (unsigned long)dv.u.long_val);
            // size of short/wchar aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_ulong:
            os->print ("0x%08.8x", (unsigned long)dv.u.ulong_val);
            // size of short/wchar aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_enum:
            // enum
            os->print ("0x%08.8x", (unsigned long)dv.u.enum_val);
            // size of short/wchar aligned to 4 bytes
            this->tc_offset_ += sizeof (ACE_CDR::ULong);
            break;

          case AST_Expression::EV_ulonglong:
          case AST_Expression::EV_longlong:
            // unimplemented yet

          default:
            ACE_ERROR_RETURN ((LM_DEBUG,
                               "be_union_branch: (%N:%l) Label value "
                               "type (%d) is invalid\n",
                               ub->udisc_type ()),
                              -1);
            ACE_NOTREACHED (break;)
        }

      *os << ", // union default label (evaluated value)" << be_nl;
    }

  // emit name
  this->gen_name (node);

  // hand over code generation to our type node
  bt = be_type::narrow_from_decl (node->field_type ());
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED);

  if (!bt || bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
          ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
          ACE_TEXT ("::gen_encapsulation (union_branch) - ")
          ACE_TEXT ("failed to generate typecode\n")
        ),
       -1);
    }

  // revert the state
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE);
  return 0;
}

int
be_visitor_typecode_defn::gen_typecode (be_valuetype *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  // check if we are repeated
  const be_visitor_typecode_defn::QNode *qnode =
    this->queue_lookup (this->tc_queue_, node);
  if (qnode)
    {
      // we are repeated, so we must generate an indirection here
      *os << "0xffffffff, // indirection" << be_nl;
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
      // the offset must point to the tc_kind value of the first occurrence of
      // this type
      os->print ("0x%x, // negative offset (%ld)\n",
                 (qnode->offset - this->tc_offset_),
                 (qnode->offset - this->tc_offset_));
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
    }
  else
    {
      // Insert node into tc_queue_ in case the node is involved in
      // some form of recursion.
      if (this->queue_insert (this->tc_queue_,
                              node,
                              this->tc_offset_) == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typecode_defn::"
                           "visit_type - "
                           "queue insert failed\n"),
                          -1);
      }

      *os << "CORBA::tk_value, // typecode kind" << be_nl;
      // size of the enum
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      {
        Scoped_Compute_Queue_Guard guard (this);

        // emit the encapsulation length
        this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
        if (node->accept (this) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%N:%l) - be_visitor_typecode_defn")
                               ACE_TEXT ("gen_typecode (valuetype) - ")
                               ACE_TEXT ("Failed to get encap length\n")),
                              -1);
          }
      }

      *os << this->computed_encap_len_ << ", // encapsulation length"
          << be_idt << "\n";
      // size of the encap length
      this->tc_offset_ += sizeof (ACE_CDR::ULong);

      // now emit the encapsulation
      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAPSULATION);
      if (node->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::gen_typecode (valuetype) - ")
                             ACE_TEXT ("failed to generate encapsulation\n")),
                            -1);
        }
      *os << be_uidt << "\n";
    }
  return 0;
}

int
be_visitor_typecode_defn::gen_encapsulation (be_valuetype *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream

  os->indent (); // start from whatever indentation level we were at

  *os << "TAO_ENCAP_BYTE_ORDER, // byte order" << be_nl;
  // size of the encapsulation byte order flag. Although it is 1 byte, the
  // aligned size is 4 bytes
  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  // STEP 1: generate repoID
  this->gen_repoID (node);

  // STEP 2: generate name
  os->indent ();
  this->gen_name (node);

  // STEP 3: generate ValueModifier

  // By spec ValueModifier is UShort but aligned size
  // seems always to be 4.
  os->indent ();

  // TAO doesn't support neither CUSTOM nor TRUNCATABLE
  // valuetypes. So basically need to choose between
  // VM_NONE = 0 and VM_ABSTRACT = 2
  ACE_CDR::ULong value_modifier = node->is_abstract_valuetype () ? 2 : 0;

  *os << value_modifier << ", // value modifier" << "\n";

  this->tc_offset_ += sizeof (ACE_CDR::ULong);

  //STEP 4: generate TypeCode of concrete base

  AST_Interface *inherited = 0;
  if (node->n_inherits () > 0 &&
      (   // Statefull base valuetype is always first
          inherited =
          AST_Interface::narrow_from_decl(node->inherits ()[0])
      ) != 0 &&
      inherited->is_valuetype () &&
      !inherited->is_abstract ()
     )
    {
      // Got non-abstract base valuetype. Now emit its typecode
      be_valuetype *vt = be_valuetype::narrow_from_decl(node->inherits ()[0]);
      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED);
      if (!vt || vt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::gen_encapsulation (valuetype) - ")
                             ACE_TEXT ("failed to generate typecode\n")),
                            -1);
        }
      // revert the state to what it was before
      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE);
    }
  else
    {
      // emit tk_null
      os->indent ();
      *os << "CORBA::tk_null, // no stateful base valuetype"
          << "\n\n";

      // size of the enum
      this->tc_offset_ += sizeof (ACE_CDR::ULong);
    }

  //STEP 5: generate the member count
  os->indent ();

  *os << node->data_members_count () << ", // member count\n";
  // size of the member count
   this->tc_offset_ += sizeof (ACE_CDR::ULong);

  //STEP 6: hand over to the scope to generate the typecode for elements
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE);

  if (node->accept (this) == -1)
    {
      ACE_ERROR_RETURN((
          LM_ERROR,
          "be_valuetype: cannot generate typecode for members\n"), -1);
    }

  return 0;
}


// = methods for computing typecode and encapsulation sizes

ACE_CDR::Long
be_visitor_typecode_defn::compute_tc_size (be_array *node)
{
  // 4 bytes for enumeration, 4 bytes for storing encap length val, followed by the
  // actual encapsulation

  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
  if (node->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_tc_size (array) - ")
                         ACE_TEXT ("cannot compute encap len\n")),
                        -1);
    }

  this->computed_tc_size_ =  4 + 4 + this->computed_encap_len_;
  return this->computed_tc_size_;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_array *node)
{
  // Suppose "N" is the number of dimensions, then for a N dimensional array,
  // we will have N encapsulations. The innermost encapsulation will hold the
  // typecode of the real base type.
  // Thus, we will have N byte order flags and dimensions, and N-1 tk_array
  // enumerations, encapsulation lengths, and dimensions.

  be_type *bt; // base type

  bt = be_type::narrow_from_decl (node->base_type ());

  ACE_CDR::Long encap_len =
    // N byte order flags and dimensions
    node->n_dims () * (4 + 4)
    +
    // N-1 of tk_array and that many encapsulation lengths
    (node->n_dims () - 1) * (4 + 4);

  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TC_SIZE);
  if (!bt || bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (array) - ")
                         ACE_TEXT ("cannot compute tc size for base\n")),
                        -1);
    }

  this->computed_encap_len_ = encap_len + this->computed_tc_size_;
  return this->computed_encap_len_;
}

ACE_CDR::Long
be_visitor_typecode_defn::compute_tc_size (be_enum *node)
{
  // while computing the encapsulation length we must keep in mind the typecode
  // that has gotten generated until this point. Hence, we must first check the
  // "tc_queue" to ensure if are already there somewhere in a previous
  // encapsulation in which case we must count only the bytes for the
  // indirection. If we are not already generated, we must then check if we
  // have already been counted in the current computation or not by checking
  // for our presence in the compute queue. In both cases, we only include the
  // 8 bytes in the computation
  if (this->queue_lookup (this->tc_queue_, node) ||
      this->queue_lookup (this->compute_queue_, node))
    {
      this->computed_tc_size_ = 4 + 4;
    }
  else
    {
      // Insert node into tc_queue_ in case the node is involved in
      // some form of recursion.
      if (this->queue_insert (this->compute_queue_, node, this->tc_offset_) == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typecode_defn::"
                           "compute_tc_size (enum) - "
                           "queue insert failed\n"),
                          -1);
      }

      // 4 bytes for enumeration, 4 bytes for storing encap length val, followed by the
      // actual encapsulation

      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
      if (node->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::compute_tc_size (enum) - ")
                             ACE_TEXT ("cannot compute encap len\n")),
                            -1);
        }

      this->computed_tc_size_ = 4 + 4 + this->computed_encap_len_;
    }
  return this->computed_tc_size_;
}

ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_enum *node)
{
  ACE_CDR::Long encap_len;
  encap_len = 4;  // holds the byte order flag

  encap_len +=
    this->repoID_encap_len (node); // repoID storage

  // do the same thing for the local name
  encap_len += this->name_encap_len (node);

  encap_len += 4; // to hold the member count

  // save the current value of scope len and start with a fresh one for our
  // scope length computation
  if (this->push (this->computed_scope_encap_len_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (enum) - ")
                         ACE_TEXT ("push failed\n")),
                        -1);
    }
  this->computed_scope_encap_len_ = 0;

  // compute encap length for members
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN);
  if (node->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (enum) - ")
                         ACE_TEXT ("cannot compute scope tc size\n")),
                        -1);
    }

  this->computed_encap_len_ = encap_len + this->computed_scope_encap_len_;

  // pop off the previous value of computed_scope_len_
  if (this->pop (this->computed_scope_encap_len_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (enum) - ")
                         ACE_TEXT ("pop failed\n")),
                        -1);
    }

  return this->computed_encap_len_;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_enum_val *node)
{
  this->computed_encap_len_ = this->name_encap_len (node);
  return this->computed_encap_len_;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_tc_size (be_exception *node)
{
  // while computing the encapsulation length we must keep in mind the typecode
  // that has gotten generated until this point. Hence, we must first check the
  // "tc_queue" to ensure if are already there somewhere in a previous
  // encapsulation in which case we must count only the bytes for the
  // indirection. If we are not already generated, we must then check if we
  // have already been counted in the current computation or not by checking
  // for our presence in the compute queue. In both cases, we only include the
  // 8 bytes in the computation
  if (this->queue_lookup (this->tc_queue_, node) ||
      this->queue_lookup (this->compute_queue_, node))
    {
      this->computed_tc_size_ = 4 + 4;
    }
  else
    {
      // Insert node into tc_queue_ in case the node is involved in
      // some form of recursion.
      if (this->queue_insert (this->compute_queue_, node, this->tc_offset_) == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typecode_defn::"
                           "compute_tc_size (exception) - "
                           "queue insert failed\n"),
                          -1);
      }

      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
      if (node->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::compute_tc_size (array) - ")
                             ACE_TEXT ("cannot compute encap len\n")),
                            -1);
        }
      this->computed_tc_size_ = 4 + 4 + this->computed_encap_len_;
    }
  return this->computed_tc_size_;
}

ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_exception *node)
{
  ACE_CDR::Long encap_len;
  encap_len = 4;  // holds the byte order flag

  encap_len += this->repoID_encap_len (node); // repoID

  // do the same thing for the local name
  encap_len += this->name_encap_len (node);

  encap_len += 4; // to hold the member count

  // save the current value of scope len and start with a fresh one for our
  // scope length computation
  if (this->push (this->computed_scope_encap_len_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (exception) - ")
                         ACE_TEXT ("push failed\n")),
                        -1);
    }
  this->computed_scope_encap_len_ = 0;

  // compute encap length for members
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN);
  if (node->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (exception) - ")
                         ACE_TEXT ("cannot compute scope tc size\n")),
                        -1);
    }

  this->computed_encap_len_ = encap_len + this->computed_scope_encap_len_;

  // pop off the previous value of computed_scope_len_
  if (this->pop (this->computed_scope_encap_len_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (exception) - ")
                         ACE_TEXT ("pop failed\n")),
                        -1);
    }

  return this->computed_encap_len_;
}

ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_field *node)
{
  be_type *bt;

  // struct member is represented as the "name" followed by the typecode

  ACE_CDR::Long encap_len =
    this->name_encap_len (node); // for name

  // add to this, the size of our typecode
  bt = be_type::narrow_from_decl (node->field_type ());
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TC_SIZE);
  if (!bt || bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (array) - ")
                         ACE_TEXT ("cannot compute tc size\n")),
                        -1);
    }

  // note that we must add typecode size of base type
  this->computed_encap_len_ = encap_len + this->computed_tc_size_;

  // revert the sub state
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN);

  if (node->visibility() != AST_Field::vis_NA)
    {
      // count visibility marker

      // Even though visibility marker is UShort it seems that
      // it would always be aligned on ULong boundary.
      this->computed_encap_len_ += 4;
    }


  return this->computed_encap_len_;
}

ACE_CDR::Long
be_visitor_typecode_defn::compute_tc_size (be_interface *node)
{
  // while computing the encapsulation length we must keep in mind the typecode
  // that has gotten generated until this point. Hence, we must first check the
  // "tc_queue" to ensure if are already there somewhere in a previous
  // encapsulation in which case we must count only the bytes for the
  // indirection. If we are not already generated, we must then check if we
  // have already been counted in the current computation or not by checking
  // for our presence in the compute queue. In both cases, we only include the
  // 8 bytes in the computation
  if (this->queue_lookup (this->tc_queue_, node) ||
      this->queue_lookup (this->compute_queue_, node))
    {
      this->computed_tc_size_ = 4 + 4;
    }
  else
    {
      // Insert node into tc_queue_ in case the node is involved in
      // some form of recursion.
      if (this->queue_insert (this->compute_queue_, node, this->tc_offset_) == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typecode_defn::"
                           "compute_tc_size (interface) - "
                           "queue insert failed\n"),
                          -1);
      }

      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
      if (node->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::compute_tc_size (interface) - ")
                             ACE_TEXT ("cannot compute encap len\n")),
                            -1);
        }

      this->computed_tc_size_ = 4 + 4 + this->computed_encap_len_;
    }
  return this->computed_tc_size_;
}

ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_interface *node)
{
  this->computed_encap_len_ = 4;  // holds the byte order flag

  this->computed_encap_len_ +=
    this->repoID_encap_len (node); // for repoID

  // do the same thing for the local name
  this->computed_encap_len_ +=
    this->name_encap_len (node);

  return this->computed_encap_len_;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_tc_size (be_interface_fwd *)
{
  return 0;
}

ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_interface_fwd *)
{
  return 0;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_tc_size (be_predefined_type *node)
{
  if (!ACE_OS::strcmp (node->local_name ()->get_string (),
                       "Object")) // not same
    {
      // while computing the encapsulation length we must keep in mind the typecode
      // that has gotten generated until this point. Hence, we must first check the
      // "tc_queue" to ensure if are already there somewhere in a previous
      // encapsulation in which case we must count only the bytes for the
      // indirection. If we are not already generated, we must then check if we
      // have already been counted in the current computation or not by checking
      // for our presence in the compute queue. In both cases, we only include the
      // 8 bytes in the computation
      if (this->queue_lookup (this->tc_queue_, node) ||
          this->queue_lookup (this->compute_queue_, node))
        {
          this->computed_tc_size_ = 4 + 4;
        }
      else
        {
          // Insert node into tc_queue_ in case the node is involved in
          // some form of recursion.
          if (this->queue_insert (this->compute_queue_, node, this->tc_offset_) == 0)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_visitor_typecode_defn::"
                               "compute_tc_size (predefined type) - "
                               "queue insert failed\n"),
                              -1);
          }

          this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
          if (node->accept (this) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                                 ACE_TEXT ("::compute_tc_size (predefined objref) - ")
                                 ACE_TEXT ("cannot compute encap len\n")),
                                -1);
            }

          this->computed_tc_size_ = 4 + 4 + this->computed_encap_len_;
        }
    }
  else
    this->computed_tc_size_ = 4;

  return this->computed_tc_size_;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_predefined_type *node)
{
  if (!ACE_OS::strcmp (node->local_name ()->get_string (),
                       "Object")) // not same
    {
      this->computed_encap_len_ = 4;  // holds the byte order flag

      this->computed_encap_len_ +=
        this->repoID_encap_len (node); // for repoID

      // do the same thing for the local name
      this->computed_encap_len_ +=
        this->name_encap_len (node);
    }
  else
    this->computed_encap_len_ = 0;

  return this->computed_encap_len_;
}

ACE_CDR::Long
be_visitor_typecode_defn::compute_tc_size (be_sequence *node)
{
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
  if (node->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_tc_size (sequence) - ")
                         ACE_TEXT ("cannot compute encap len\n")),
                        -1);
    }

  this->computed_tc_size_ = 4 + 4 + this->computed_encap_len_;
  return this->computed_tc_size_;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_sequence *node)
{
  be_type *bt; // base type

  ACE_CDR::Long encap_len = 4;  // holds the byte order flag

  // add the encapsulation length of our base type
  bt = be_type::narrow_from_decl (node->base_type ());
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TC_SIZE);
  if (!bt || bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (sequence) - ")
                         ACE_TEXT ("cannot compute tc size\n")),
                        -1);
    }

  this->computed_encap_len_ = encap_len + this->computed_tc_size_;
  this->computed_encap_len_ += 4; // to hold the max size

  return this->computed_encap_len_;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_tc_size (be_string *)
{
  this->computed_tc_size_ = 4 + 4;
  return this->computed_tc_size_;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_string *)
{
  this->computed_encap_len_ = 0;
  return this->computed_encap_len_;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_tc_size (be_structure *node)
{
  // while computing the encapsulation length we must keep in mind the typecode
  // that has gotten generated until this point. Hence, we must first check the
  // "tc_queue" to ensure if are already there somewhere in a previous
  // encapsulation in which case we must count only the bytes for the
  // indirection. If we are not already generated, we must then check if we
  // have already been counted in the current computation or not by checking
  // for our presence in the compute queue. In both cases, we only include the
  // 8 bytes in the computation
  if (this->queue_lookup (this->tc_queue_, node) ||
      this->queue_lookup (this->compute_queue_, node))
    {
      this->computed_tc_size_ = 4 + 4;
    }
  else
    {
      if (this->queue_insert (this->compute_queue_, node, this->tc_offset_) == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typecode_defn::"
                           "compute_tc_size (structure) - "
                           "queue insert failed\n"),
                          -1);
      }

      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
      if (node->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::compute_tc_size (structure) - ")
                             ACE_TEXT ("cannot compute encap len\n")),
                            -1);
        }

      this->computed_tc_size_ = 4 + 4 + this->computed_encap_len_;
    }
  return this->computed_tc_size_;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_structure *node)
{
  ACE_CDR::Long encap_len = 4;  // holds the byte order flag

  encap_len += this->repoID_encap_len (node); // repoID

  // do the same thing for the local name
  encap_len += this->name_encap_len (node);

  encap_len += 4; // to hold the member count

  // save the current value of scope len and start with a fresh one for our
  // scope length computation
  if (this->push (this->computed_scope_encap_len_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (struct) - ")
                         ACE_TEXT ("push failed\n")),
                        -1);
    }
  this->computed_scope_encap_len_ = 0;

  // compute encap length for members
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN);
  if (node->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (structure) - ")
                         ACE_TEXT ("cannot compute scope tc size\n")),
                        -1);
    }

  this->computed_encap_len_ = encap_len + this->computed_scope_encap_len_;

  // pop off the previous value of computed_scope_len_
  if (this->pop (this->computed_scope_encap_len_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (struct) - ")
                         ACE_TEXT ("pop failed\n")),
                        -1);
    }

  return this->computed_encap_len_;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_tc_size (be_typedef *node)
{
  // while computing the encapsulation length we must keep in mind the typecode
  // that has gotten generated until this point. Hence, we must first check the
  // "tc_queue" to ensure if are already there somewhere in a previous
  // encapsulation in which case we must count only the bytes for the
  // indirection. If we are not already generated, we must then check if we
  // have already been counted in the current computation or not by checking
  // for our presence in the compute queue. In both cases, we only include the
  // 8 bytes in the computation
  if (this->queue_lookup (this->tc_queue_, node) ||
      this->queue_lookup (this->compute_queue_, node))
    {
      this->computed_tc_size_ = 4 + 4;
    }
  else
    {
      if (this->queue_insert (this->compute_queue_, node, this->tc_offset_) == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typecode_defn::"
                           "compute_tc_size (typedef) - "
                           "queue insert failed\n"),
                          -1);
      }

      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
      if (node->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::compute_tc_size (array) - ")
                             ACE_TEXT ("cannot compute encap len\n")),
                            -1);
        }

      this->computed_tc_size_ = 4 + 4 + this->computed_encap_len_;
    }
  return this->computed_tc_size_;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_typedef *node)
{
  be_type *bt; // base type
  ACE_CDR::Long encap_len = 4;  // holds the byte order flag

  encap_len +=
    this->repoID_encap_len (node); // repoID

  // do the same thing for the local name
  encap_len +=
    this->name_encap_len (node);

  // add the encapsulation length of our base type
  bt = be_type::narrow_from_decl (node->base_type ());
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TC_SIZE);
  if (!bt || bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (typedef) - ")
                         ACE_TEXT ("cannot compute tc size\n")),
                        -1);
    }

  this->computed_encap_len_ = encap_len + this->computed_tc_size_;
  return this->computed_encap_len_;

}


ACE_CDR::Long
be_visitor_typecode_defn::compute_tc_size (be_union *node)
{
  // while computing the encapsulation length we must keep in mind the typecode
  // that has gotten generated until this point. Hence, we must first check the
  // "tc_queue" to ensure if are already there somewhere in a previous
  // encapsulation in which case we must count only the bytes for the
  // indirection. If we are not already generated, we must then check if we
  // have already been counted in the current computation or not by checking
  // for our presence in the compute queue. In both cases, we only include the
  // 8 bytes in the computation
  if (this->queue_lookup (this->tc_queue_, node) ||
      this->queue_lookup (this->compute_queue_, node))
    {
      this->computed_tc_size_ = 4 + 4;
    }
  else
    {
      // Insert node into tc_queue_ in case the node is involved in
      // some form of recursion.
      if (this->queue_insert (this->compute_queue_, node, this->tc_offset_) == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typecode_defn::"
                           "compute_tc_size (union) - "
                           "queue insert failed\n"),
                          -1);
      }

      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
      if (node->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::compute_tc_size (union) - ")
                             ACE_TEXT ("cannot compute encap len\n")),
                            -1);
        }

      this->computed_tc_size_ = 4 + 4 + this->computed_encap_len_;
    }
  return this->computed_tc_size_;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_union *node)
{
  be_type *discrim;

  ACE_CDR::Long encap_len = 4;  // holds the byte order flag

  encap_len += this->repoID_encap_len (node); // for repoID

  // do the same thing for the local name
  encap_len += this->name_encap_len (node); // for name

  // add encapsulation size of discriminant typecode
  discrim = be_type::narrow_from_decl (node->disc_type ());
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TC_SIZE);
  if (!discrim || discrim->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (union) - ")
                         ACE_TEXT ("cannot compute tc size\n")),
                        -1);
    }

  encap_len += this->computed_tc_size_;

  encap_len += 4; // to hold the "default used" flag
  encap_len += 4; // to hold the member count

  // save the current value of scope len and start with a fresh one for our
  // scope length computation
  if (this->push (this->computed_scope_encap_len_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (union) - ")
                         ACE_TEXT ("push failed\n")),
                        -1);
    }
  this->computed_scope_encap_len_ = 0;

  // compute encap length for members
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN);
  if (node->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (union) - ")
                         ACE_TEXT ("cannot compute scope tc size\n")),
                        -1);
    }

  this->computed_encap_len_ = encap_len + this->computed_scope_encap_len_;

  // pop off the previous value of computed_scope_len_
  if (this->pop (this->computed_scope_encap_len_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (union) - ")
                         ACE_TEXT ("pop failed\n")),
                        -1);
    }

  return this->computed_encap_len_;
}


ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_union_branch *node)
{
  be_type *bt;

  ACE_CDR::Long encap_len = 0;

  encap_len += 4; // case label;
  encap_len += this->name_encap_len (node); // for name

  bt = be_type::narrow_from_decl (node->field_type ());
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TC_SIZE);
  if (!bt || bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (union branch) - ")
                         ACE_TEXT ("cannot compute tc size\n")),
                        -1);
    }

  encap_len += this->computed_tc_size_;

  this->computed_encap_len_ = encap_len;

  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN);
  return this->computed_encap_len_;
}

ACE_CDR::Long
be_visitor_typecode_defn::compute_tc_size (be_valuetype *node)
{
  // while computing the encapsulation length we must keep in mind the typecode
  // that has gotten generated until this point. Hence, we must first check the
  // "tc_queue" to ensure if are already there somewhere in a previous
  // encapsulation in which case we must count only the bytes for the
  // indirection. If we are not already generated, we must then check if we
  // have already been counted in the current computation or not by checking
  // for our presence in the compute queue. In both cases, we only include the
  // 8 bytes in the computation
  if (this->queue_lookup (this->tc_queue_, node) ||
      this->queue_lookup (this->compute_queue_, node))
    {
      this->computed_tc_size_ = 4 + 4;
    }
  else
    {
      // Insert node into tc_queue_ in case the node is involved in
      // some form of recursion.
      if (this->queue_insert (this->compute_queue_,
                              node,
                              this->tc_offset_) == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typecode_defn::"
                           "compute_tc_size (valuetype) - "
                           "queue insert failed\n"),
                          -1);
      }

      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);
      if (node->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::compute_tc_size (valuetype) - ")
                             ACE_TEXT ("cannot compute encap len\n")),
                            -1);
        }

      this->computed_tc_size_ = 4 + 4 + this->computed_encap_len_;
    }
  return this->computed_tc_size_;
}

ACE_CDR::Long
be_visitor_typecode_defn::compute_encap_length (be_valuetype *node)
{
  // STEP 1:
  ACE_CDR::Long encap_len = 4;  // holds the byte order flag

  // STEP 2:
  encap_len += this->repoID_encap_len (node); // repoID

  //STEP 3: do the same thing for the local name
  encap_len += this->name_encap_len (node);

  // STEP 4:
  encap_len += 4; // to hold the ValueModifier


  // STEP 5: get encapsulation length for concrete base valuetype
  AST_Interface *inherited = 0;
  if (node->n_inherits () > 0 &&
      (   // Statefull abse valuetype is always first
          inherited =
          AST_Interface::narrow_from_decl(node->inherits ()[0])
      ) != 0 &&
      inherited->is_valuetype () &&
      !inherited->is_abstract ()
     )
    {
      // Got non-abstract base valuetype.

      this->computed_encap_len_ = 0;

      be_valuetype *vt = be_valuetype::narrow_from_decl(node->inherits ()[0]);
      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TC_SIZE);
      if (!vt || vt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                             ACE_TEXT ("::compute_encap_len (valuetype) - ")
                             ACE_TEXT ("failed to compute len\n")),
                            -1);
        }
      // revert the state to what it was before
      this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_ENCAP_LEN);

      encap_len += this->computed_encap_len_;
    }
  else
    {
      encap_len += 4; // to hold the CORBA::tk_null
    }

  //STEP 6:
  encap_len += 4; // to hold the member count

  // save the current value of scope len and start with a fresh one for our
  // scope length computation
  if (this->push (this->computed_scope_encap_len_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (struct) - ")
                         ACE_TEXT ("push failed\n")),
                        -1);
    }
  this->computed_scope_encap_len_ = 0;

  // compute encap length for members
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN);
  if (node->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (valuetype) - ")
                         ACE_TEXT ("cannot compute scope tc size\n")),
                        -1);
    }

  this->computed_encap_len_ = encap_len + this->computed_scope_encap_len_;

  // pop off the previous value of computed_scope_len_
  if (this->pop (this->computed_scope_encap_len_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::compute_encap_len (valuetype) - ")
                         ACE_TEXT ("pop failed\n")),
                        -1);
    }

  return this->computed_encap_len_;
}


// helpers that accomplish a common task - that of generating the repository
// IDs and names in a TypeCode description

void
be_visitor_typecode_defn::gen_repoID (be_decl *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  int flag = 0;

  // check if we want to generate optimized typecodes. In such a case, there is
  // no need to generate the repoID (unless we are an object reference or an
  // exception in which case it is mandatory to have the repository ID)
  // generate repoID

  if (be_global->opt_tc ())
    {
      switch (node->node_type ())
        {
        case AST_Decl::NT_interface:
        case AST_Decl::NT_interface_fwd:
        case AST_Decl::NT_except:
          flag = 0;
          break;
        case AST_Decl::NT_pre_defined:
          if (!ACE_OS::strcmp (node->local_name ()->get_string (),
                               "Object"))
            flag = 0;
          else
            flag = 1;
          break;
        default:
          flag = 1;
        }
    }

  // @@@JP CORBA 2.3.1 states that the Repository ID must be present
  // for interfaces, valuetypes and exceptions, as Andy has implemented
  // above. It also states that it must be present in structs, unions,
  // enumerations and aliases for typecodes from an interface repository
  // or created by the ORB. We've also had requests to leave the repo
  // ID in even in optimized typecodes, so I'll disable the
  // optimization of this field for now.
  flag = 0;

  if (flag)
    {
      // optimized case
      *os << "1, 0x0,";
      *os << " // repository ID = ";

      // repoID generation.
      *os << node->repoID ();

      // size of the repoID filed
      this->tc_offset_ += (2 * sizeof (ACE_CDR::ULong));
    }
  else
    {
      // Unoptimized case.

      *os << (ACE_OS::strlen (node->repoID ()) + 1)
          << "," << be_nl;

      ACE_CDR::ULong *arr, i, arrlen;

      (void) this->tc_name2long (node->repoID (), arr, arrlen);

      for (i = 0; i < arrlen; i++)
        {
          os->print ("ACE_NTOHL (0x%x), ", arr[i]);

          if (i < arrlen - 1)
            {
              *os << be_nl;
            }
        }

      // Comment.
      *os << " // repository ID = " << node->repoID ();

      // size of the repoID field
      this->tc_offset_ += (arrlen + 1) * sizeof (ACE_CDR::ULong);
    }
  *os << "\n";

  return;
}

void
be_visitor_typecode_defn::gen_name (be_decl *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // @@ AMI code generation should be done for this optimized
  //    case. (Alex).

  // Generate name.
  if (be_global->opt_tc ())
    {
      *os << "1, 0x0,";
      *os << " // name = " << node->local_name ();
      // size of the name field
      this->tc_offset_ += (2 * sizeof (ACE_CDR::ULong));
    }
  else
    {
      ACE_CDR::ULong *arr, i, arrlen;

      *os << (ACE_OS::strlen (node->local_name ()->get_string ()) + 1)
          << "," << be_nl;

      (void) this->tc_name2long (node->local_name ()->get_string (), arr, arrlen);

      for (i = 0; i < arrlen; i++)
        {
          os->print ("ACE_NTOHL (0x%x), ", arr[i]);

          if (i < arrlen - 1)
            {
              *os << be_nl;
            }
        }

      *os << " // name = " << node->local_name ();

      // size of the name field.
      this->tc_offset_ += (arrlen + 1) * sizeof (ACE_CDR::ULong);
    }
  *os << "\n";
  return;
}

// return the length in bytes to hold the repoID inside a typecode. This
// comprises 4 bytes indicating the length of the string followed by the actual
// string represented as longs.
ACE_CDR::ULong
be_visitor_typecode_defn::repoID_encap_len (be_decl *node)
{
  int flag = 0;

  // check if we want to generate optimized typecodes. In such a case, there is
  // no need to generate the repoID (unless we are an object reference or an
  // exception in which case it is mandatory to have the repository ID)
  // generate repoID

  if (be_global->opt_tc ())
    {
      switch (node->node_type ())
        {
        case AST_Decl::NT_interface:
        case AST_Decl::NT_interface_fwd:
        case AST_Decl::NT_except:
          flag = 0;
          break;
        case AST_Decl::NT_pre_defined:
          if (!ACE_OS::strcmp (node->local_name ()->get_string (),
                               "Object"))
            flag = 0;
          else
            flag = 1;
          break;
        default:
          flag = 1;
        }
    }

  // @@@JP CORBA 2.3.1 states that the Repository ID must be present
  // for interfaces, valuetypes and exceptions, as Andy has implemented
  // above. It also states that it must be present in structs, unions,
  // enumerations and aliases for typecodes from an interface repository
  // or created by the ORB. We've also had requests to leave the repo
  // ID in even in optimized typecodes, so I'll disable the
  // optimization of this field for now.
  flag = 0;

  if (flag)
    {
      return 4 + 4;
    }
  else
    {
      ACE_CDR::ULong slen = ACE_OS::strlen (node->repoID ()) + 1;
      // + 1 for NULL terminating char

      // the number of bytes to hold the string must be a multiple of 4 since this
      // will be represented as an array of longs
      return 4 + 4 * (slen/4 + (slen%4 ? 1:0));
    }
}

// return the length in bytes to hold the name inside a typecode. This
// comprises 4 bytes indicating the length of the string followed by the actual
// string represented as longs.
ACE_CDR::ULong
be_visitor_typecode_defn::name_encap_len (be_decl *node)
{
  if (be_global->opt_tc ())
    {
      return 4 + 4;
    }
  else
    {
      ACE_CDR::ULong slen =
        ACE_OS::strlen (node->local_name ()->get_string ()) + 1;

      // the number of bytes to hold the string must be a multiple of 4 since this
      // will be represented as an array of longs
      return 4 + 4 * (slen/4 + (slen%4 ? 1:0));
    }
}

// converts a string name into an array of 4 byte longs
int
be_visitor_typecode_defn::tc_name2long (const char *name,
                                        ACE_CDR::ULong *&larr,
                                        ACE_CDR::ULong &arrlen)
{
  const int bytes_per_word = sizeof (ACE_CDR::ULong);
  static ACE_CDR::ULong buf [NAMEBUFSIZE];
  ACE_CDR::ULong i, slen;

  slen = ACE_OS::strlen (name) + 1; // 1 for NULL terminating

  // compute the number of bytes necessary to hold the name rounded to
  // the next multiple of 4 (i.e., size of long)
  arrlen = slen / bytes_per_word + (slen % bytes_per_word ? 1 : 0);

  ACE_OS::memset (buf, 0, sizeof (buf));
  larr = buf;
  ACE_OS::memcpy (buf, name, slen);
  for (i = 0; i < arrlen; i++)
    larr [i] = ACE_HTONL (larr [i]);
  return 0;
}

// post processing
int
be_visitor_typecode_defn::post_process (be_decl *)
{
  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN)
    {
      this->computed_scope_encap_len_ += this->computed_encap_len_;
    }
  return 0;
}

// scope stack routines
int
be_visitor_typecode_defn::push (ACE_CDR::Long val)
{
  if (this->index_ >= TAO_BE_VISITOR_TYPECODE_DEFN_MAX_STACK_SIZE)
    return -1;

  this->scope_stack_ [++this->index_] = val;
  return 0;
}

int
be_visitor_typecode_defn::pop (ACE_CDR::Long &val)
{
  if (this->index_ < 0)
    return -1;

  val = this->scope_stack_[this->index_--];
  return 0;
}

const be_visitor_typecode_defn::QNode *
be_visitor_typecode_defn::
queue_insert (ACE_Unbounded_Queue <be_visitor_typecode_defn::QNode *> &queue,
              be_type *node, ACE_CDR::Long offset)
{
  be_visitor_typecode_defn::QNode *qnode;

  ACE_NEW_RETURN (qnode, be_visitor_typecode_defn::QNode, 0);

  qnode->node = node;
  qnode->offset = offset;

  if (queue.enqueue_tail (qnode) == -1)
    {
      delete qnode;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::queue_insert - failed\n")),
                        0);
    }

  return qnode;
}

const be_visitor_typecode_defn::QNode *
be_visitor_typecode_defn::
queue_lookup (ACE_Unbounded_Queue <be_visitor_typecode_defn::QNode *> &queue,
              be_type *node)
{
  for (ACE_Unbounded_Queue_Iterator<be_visitor_typecode_defn::QNode *> 
         iter (queue);
       !iter.done ();
       iter.advance ())
    {
      be_visitor_typecode_defn::QNode **addr, *item;
      iter.next (addr);
      item = *addr;

      if (!ACE_OS::strcmp (item->node->full_name (),
                           node->full_name ()))
        {
          // Found.
          return item;
        }
    }

  return 0;
}

void
be_visitor_typecode_defn::
queue_reset (ACE_Unbounded_Queue <be_visitor_typecode_defn::QNode *> &queue)
{
  while (!queue.is_empty ())
    {
      be_visitor_typecode_defn::QNode *qnode;
      (void) queue.dequeue_head (qnode);
      delete qnode;
    }
  return;
}
