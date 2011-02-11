
//=============================================================================
/**
 *  @file    rettype.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for return type of the Operation node
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ****************************************************************************
// Operation visitor for return types. This generates the mapping for a return
// type in an operation signature
// ****************************************************************************

be_visitor_operation_rettype::be_visitor_operation_rettype (be_visitor_context
                                                            *ctx)
  : be_visitor_decl (ctx),
    os (ctx->stream ())
{
}

be_visitor_operation_rettype::~be_visitor_operation_rettype (void)
{
}

int
be_visitor_operation_rettype::visit_array (
  be_array *node)
{
  *os << "::" << this->type_name (node) << "_slice *";

  return 0;
}

int
be_visitor_operation_rettype::visit_enum (
  be_enum *node)
{
  *os << "::" << this->type_name (node);

  return 0;
}

int
be_visitor_operation_rettype::visit_interface (
  be_interface *node)
{
  *os << "::" << this->type_name (node) << "_ptr";

  return 0;
}

int
be_visitor_operation_rettype::visit_interface_fwd (
  be_interface_fwd *node)
{
  *os << "::" << this->type_name (node) << "_ptr";

  return 0;
}

int
be_visitor_operation_rettype::visit_native (
  be_native *node)
{
  *os << "::" << this->type_name (node);

  return 0;
}

int
be_visitor_operation_rettype::visit_predefined_type (
  be_predefined_type *node)
{
  be_type *bt = 0;

  if (this->ctx_->alias ())
    {
      // A typedefed return type.
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  AST_PredefinedType::PredefinedType pt = node->pt ();

  if (pt != AST_PredefinedType::PT_void)
    {
      *os << "::";
    }

  switch (pt)
    {
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_object:
    case AST_PredefinedType::PT_abstract:
      *os << bt->name ();

      // If it's not TCKind, it's either Object, TypeCode or ValueBase.
      if (ACE_OS::strcmp (bt->local_name ()->get_string (), "TCKind") != 0)
        {
          *os << "_ptr";
        }

      break;
    case AST_PredefinedType::PT_any:
    case AST_PredefinedType::PT_value:
      {
        *os << bt->name () << " *";

        break;
      }
    default:
      {
        *os << bt->name ();

        break;
      }
    }

  return 0;
}

int
be_visitor_operation_rettype::visit_sequence (
  be_sequence *node)
{
  *os << "::" << this->type_name (node);

  if (!be_global->alt_mapping () || !node->unbounded ())
    {
      *os << " *";
    }

  return 0;
}

int
be_visitor_operation_rettype::visit_string (be_string *node)
{
  ACE_CDR::ULong bound = node->max_size ()->ev ()->u.ulval;
  bool wide = (node->width () != (long) sizeof (char));

  if (wide)
    {
      *os << "::CORBA::WChar *";
    }
  else if (bound == 0 && be_global->alt_mapping ())
    {
      *os << "std::string";
    }
  else
    {
      *os << "char *";
    }

  return 0;
}

int
be_visitor_operation_rettype::visit_structure (
  be_structure *node)
{
  *os << "::" << this->type_name (node);

  // Based on whether we are variable or not, we return a pointer or the
  // aggregate type.
  if (node->size_type () == AST_Type::VARIABLE)
    {
      *os << " *";
    }

  return 0;
}

int
be_visitor_operation_rettype::visit_typedef (
  be_typedef *node)
{
  // Set the alias node.
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype::visit_union (
  be_union *node)
{
  *os << "::" << this->type_name (node);

  // Based on whether we are variable or not, we return
  // a pointer or the aggregate type.
  if (node->size_type () == AST_Type::VARIABLE)
    {
      *os << " *";
    }

  return 0;
}

int
be_visitor_operation_rettype::visit_valuetype (
  be_valuetype *node)
{
  *os << "::" << this->type_name (node) << " *";

  return 0;
}

int
be_visitor_operation_rettype::visit_valuetype_fwd (
  be_valuetype_fwd *node)
{
  *os << "::" << this->type_name (node) << " *";

  return 0;
}

int
be_visitor_operation_rettype::visit_component (
  be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_operation_rettype::visit_component_fwd (
  be_component_fwd *node)
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_operation_rettype::visit_eventtype (
  be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_operation_rettype::visit_eventtype_fwd (
  be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_operation_rettype::visit_home (be_home *node)
{
  return this->visit_interface (node);
}

int
be_visitor_operation_rettype::visit_valuebox (
  be_valuebox *node)
{
  *os << "::" << this->type_name (node) << " *";

  return 0;
}

const char *
be_visitor_operation_rettype::type_name (be_type *node)
{
  be_type *bt = 0;

  if (this->ctx_->alias () != 0)
    {
      // A typedefed return type.
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  return bt->full_name ();
}
