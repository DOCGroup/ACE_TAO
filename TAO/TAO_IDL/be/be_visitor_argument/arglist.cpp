//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    arglist.cpp
//
// = DESCRIPTION
//    Visitor that generates the parameters in an Operation signature
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_argument, 
           arglist, 
           "$Id$")


// ************************************************************
// be_visitor_args_arglist for parameter list in method declarations and
// definitions
// ************************************************************

be_visitor_args_arglist::be_visitor_args_arglist (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_arglist::~be_visitor_args_arglist (void)
{
}

int be_visitor_args_arglist::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  // Retrieve the type.
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_arglist::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type.
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_arglist::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  *os << " " << node->local_name ();
  return 0;
}

int be_visitor_args_arglist::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const " << this->type_name (node);
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node);
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node);
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node, "_ptr");
      break;
    case AST_Argument::dir_INOUT: // inout
      *os << this->type_name (node, "_ptr") << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node, "_ptr");
      break;
    case AST_Argument::dir_INOUT: // inout
      *os << this->type_name (node, "_ptr") << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_native (be_native *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node);
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node) << " &";
      break;
    }
  return 0;
}

int be_visitor_args_arglist::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  AST_PredefinedType::PredefinedType pt = node->pt ();

  if (pt == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          *os << "const " << this->type_name (node) << " &";
          break;
        case AST_Argument::dir_INOUT:
          *os << this->type_name (node) << " &";
          break;
        case AST_Argument::dir_OUT:
          *os << this->type_name (node, "_out");
          break;
        }
    }
  else if (pt == AST_PredefinedType::PT_pseudo
           || pt == AST_PredefinedType::PT_object)
    {
      // The only PT_pseudo that doesn't take a _ptr suffix.
      idl_bool is_tckind =
        (ACE_OS::strcmp (node->local_name ()->get_string (), "TCKind") == 0);

      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          if (is_tckind)
            {
              *os << this->type_name (node);
            }
          else
            {
              *os << this->type_name (node, "_ptr");
            }

          break;
        case AST_Argument::dir_INOUT:
          if (is_tckind)
            {
              *os << this->type_name (node) << " &";
            }
          else
            {
              *os << this->type_name (node, "_ptr") << " &";
            }

          break;
        case AST_Argument::dir_OUT:
          *os << this->type_name (node, "_out");
          break;
        }
    }
  else
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          *os << this->type_name (node);
          break;
        case AST_Argument::dir_INOUT:
          *os << this->type_name (node) << " &";
          break;
        case AST_Argument::dir_OUT:
          *os << this->type_name (node, "_out");
          break;
        }
    }

  return 0;
}

int be_visitor_args_arglist::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const " << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  if (node->width () == (long) sizeof (char))
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          *os << "const char *";
          break;
        case AST_Argument::dir_INOUT:
          *os << "char *&";
          break;
        case AST_Argument::dir_OUT:
          *os << "CORBA::String_out";
          break;
        }
    }
  else
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          *os << "const CORBA::WChar *";
          break;
        case AST_Argument::dir_INOUT:
          *os << "CORBA::WChar *&";
          break;
        case AST_Argument::dir_OUT:
          *os << "CORBA::WString_out";
          break;
        }
    }

  return 0;
}

int be_visitor_args_arglist::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const " << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const " << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_arglist::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}


int be_visitor_args_arglist::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node) << " *";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " *&";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

int be_visitor_args_arglist::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const " << this->type_name (node) << " *";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " *&";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }

  return 0;
}

