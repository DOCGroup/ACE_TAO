//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_vardecl_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for return type variable declaration in the stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_operation, 
           rettype_vardecl_cs, 
           "$Id$")

// ************************************************************
//    be_visitor_operation_rettype_vardecl_cs
//    This visitor generates code for variable declaration and initialization
//    of the return type.
// ************************************************************

be_visitor_operation_rettype_vardecl_cs::
be_visitor_operation_rettype_vardecl_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_vardecl_cs::
~be_visitor_operation_rettype_vardecl_cs (void)
{
}

int
be_visitor_operation_rettype_vardecl_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_idt_nl
      << node->name () << "," << be_nl
      << node->name () << "_slice," << be_nl
      << node->name () << "_var," << be_nl;

  if (node->size_type () == AST_Type::VARIABLE)
    {
      *os << node->name () << "_out," << be_nl;
    }

  *os << node->name () << "_forany" << be_uidt_nl;

  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  *os << bt->name ();

  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (node->is_defined ())
    {
      *os << bt->name () << "_var _tao_retval ("
          << bt->name () << "::_nil ());";
    }
  else
    {
      *os << bt->name () << "_var _tao_retval (" << be_idt << be_idt_nl;

      AST_Decl *parent = ScopeAsDecl (node->defined_in ());

      if (parent != 0 && parent->node_type () != AST_Decl::NT_root)
        {
          *os << parent->name () << "::";
        }

      *os << "tao_" << node->local_name () << "_life::tao_nil ()" 
          << be_uidt_nl
          << ");" << be_uidt_nl;
    }

  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_interface_fwd (
    be_interface_fwd *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (node->is_defined ())
    {
      *os << bt->name () << "_var _tao_retval ("
          << bt->name () << "::_nil ());";
    }
  else
    {
      *os << bt->name () << "_var _tao_retval ("
          << " tao_" << node->flat_name () << "_nil ());";
    }

  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  *os << "::" << bt->name () << "_var _tao_retval;";

  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  *os << "::" << bt->name () << "_var _tao_retval;";

  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_predefined_type (
    be_predefined_type *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_object:
      *os << bt->name () << "_var _tao_retval ("
          << bt->name () << "::_nil ());";

      break;
    case AST_PredefinedType::PT_any:
      *os << bt->name () << "_var _tao_retval;";

      break;
    case AST_PredefinedType::PT_void:
      break;
    case AST_PredefinedType::PT_longdouble:
      *os << bt->name ()
          << " _tao_retval = ACE_CDR_LONG_DOUBLE_INITIALIZER;";

      break;
    case AST_PredefinedType::PT_longlong:
      *os << bt->name ()
          << " _tao_retval = ACE_CDR_LONGLONG_INITIALIZER;";

      break;
    default:
      *os << bt->name () << " _tao_retval = 0;";

      break;
    }

  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_sequence (be_sequence *node)
{
  // We should never directly be here because anonymous sequence return types
  // are not allowed.
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  *os << bt->name () << "_var _tao_retval;";

  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  *os << "CORBA::";

  if (node->width () != (long) sizeof (char))
    {
      *os << "W";
    }

  *os << "Char *";

  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  // Based on whether we are variable or not, we return a pointer or the
  // aggregate type.
  if (node->size_type () == AST_Type::VARIABLE)
    {
      *os << bt->name () << "_var _tao_retval;";
    }
  else
    {
      *os << bt->name () << " _tao_retval;" << be_nl;
      // @@ This seems like a waste of time!
      *os << "ACE_OS::memset (&_tao_retval, 0, sizeof (_tao_retval));";
    }

  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_vardecl_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  // Based on whether we are variable or not, we return a pointer or the
  // aggregate type.
  if (node->size_type () == AST_Type::VARIABLE)
    {
      *os << bt->name () << "_var _tao_retval;";
    }
  else
    {
      *os << bt->name () << " _tao_retval;";
    }

  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_operation_rettype_vardecl_cs::visit_eventtype_fwd (
    be_eventtype_fwd *node
  )
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_operation_rettype_vardecl_cs::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_operation_rettype_vardecl_cs::visit_component_fwd (
    be_component_fwd *node
  )
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_operation_rettype_vardecl_cs::visit_home (be_home *node)
{
  return this->visit_interface (node);
}

