//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    paramlist.cpp
//
// = DESCRIPTION
//    Visitor that generates the Dyanmic::ParameterList
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//    Ossama Othman <ossama@uci.edu>
//
// ============================================================================

ACE_RCSID (be_visitor_argument,
           paramlist,
           "$Id$")


// ************************************************************
// be_visitor_args_paramlist for parameter list in method declarations and
// definitions
// ************************************************************

be_visitor_args_paramlist::be_visitor_args_paramlist (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_paramlist::~be_visitor_args_paramlist (void)
{
}

int be_visitor_args_paramlist::visit_argument (be_argument *node)
{
  // Get output stream.
  TAO_OutStream *os = this->ctx_->stream ();

  // Save the argument node.
  this->ctx_->node (node);

  // We do not put "out" arguments into the arglist.
  if (this->direction () ==  AST_Argument::dir_OUT)
    {
      return 0;
    }

  // Retrieve the type.
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  // If node is typedef'd, we must strip it off.
  if (bt->node_type () == AST_Decl::NT_typedef)
    {
      be_typedef *td = be_typedef::narrow_from_decl (bt);
      bt = td->primitive_base_type ();
    }

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_arglist::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Generate valuetype check: this hack is needed as there is no
  // available way to check for valuetype type.
  // Any operators for valuetypes arent there so we dont add it to the
  // paramlist.
  if (bt->node_type () == AST_Decl::NT_interface)
    {
      be_valuetype *vt = be_valuetype::narrow_from_decl (node->field_type ());

      // @@ NOTE: If this check is removed, and insertion operators
      //          for valuetypes are implemented, don't forget to also
      //          remove the same valuetype check in
      //          be_visitor_operation::count_non_out_parameters().

      // If it is a valuetype just return.
      if (vt)
        {
          return 0;
        }
    }

  // Amazed by the zillion os operators below? Its just to combat
  // side effects functions like type_name() have on the os stream.
  // This was causing a problem with the stubs not been generated accurately
  // on Linux using egcs.
  if (bt->node_type () == AST_Decl::NT_array)
    {
      *os << this->type_name (bt, "_forany");
      *os << " _tao_forany_" << node->local_name () << " (";
      *os << this->type_name (bt, "_dup");
      *os << " (";
      // This is to placate some compilers which have
      // trouble with IN args that are multidimensional arrays.
      //if (node->n_dims () > 1) cant do this here since dont have a be_array node
      if (this->direction () != AST_Argument::dir_IN)
        *os << "(const ::" << bt->name () << "_slice *) ";
      *os << "this->";
      *os << node->local_name () << "_));" << be_nl;
      *os << "(*parameter_list)[len].argument <<= _tao_forany_" ;
      *os << node->local_name () << ";" << be_nl;
    }
  else
    {
      *os << "(*parameter_list)[len].argument <<= ";
      // Insertion into an Any has some special cases which need to be
      // dealt with.

      switch (bt->node_type ())
        {
        case AST_Decl::NT_pre_defined:
        case AST_Decl::NT_string:
        case AST_Decl::NT_wstring:
          if (bt->accept (this) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_args_paramlist::"
                                 "visit_argument - "
                                 "cannot accept visitor\n"),
                                -1);
            }
          break;

        default:
          *os << " this->" << node->local_name () << "_;";

        }
    }

   *os << be_nl;

  // Set the appropriate mode for each parameter.
  switch (node->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "(*parameter_list)[len].mode = CORBA::PARAM_IN;" << be_nl;
      break;
    case AST_Argument::dir_INOUT:
      *os << "(*parameter_list)[len].mode = CORBA::PARAM_INOUT;" << be_nl;
      break;
    case AST_Argument::dir_OUT:
      *os << "(*parameter_list)[len].mode = CORBA::PARAM_OUT;" << be_nl;
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_args_paramlist::"
                         "visit_argument - "
                         "Bad context\n"),
                        -1);
    }

  *os << "len++;" << be_nl;


  return 0;
}

int
be_visitor_args_paramlist::visit_string (be_string *node)
{
  // Get output stream.
  TAO_OutStream *os = this->ctx_->stream ();

  // Get the argument.
  be_argument *arg = this->ctx_->be_node_as_argument ();

  // We need to make a distinction between bounded and unbounded strings.
  if (node->max_size ()->ev ()->u.ulval != 0)
    {
      // Bounded strings.
      if (node->width () == (long) sizeof (char))
        {
          *os << "CORBA::Any::from_string ((char *)";
        }
      else
        {
          *os << "CORBA::Any::from_wstring ((CORBA::WChar *)";
        }

      *os << arg->local_name () << "_, "
          << node->max_size ()->ev ()->u.ulval
          << ");";
    }
  else
    {
      *os << arg->local_name () <<"_;";
    }

  return 0;
}

int
be_visitor_args_paramlist::visit_predefined_type (be_predefined_type *node)
{
  // Get output stream.
  TAO_OutStream *os = this->ctx_->stream ();

  // Get the argument.
  be_argument *arg = this->ctx_->be_node_as_argument ();

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_boolean:
      *os << "CORBA::Any::from_boolean (this->";
      *os << arg->local_name () << "_);";
      break;
    case AST_PredefinedType::PT_char:
      *os << "CORBA::Any::from_char (this->";
      *os << arg->local_name () << "_);";
      break;
    case AST_PredefinedType::PT_wchar:
      *os << "CORBA::Any::from_wchar (this->";
      *os << arg->local_name () << "_);";
      break;
    case AST_PredefinedType::PT_octet:
      *os << "CORBA::Any::from_octet (this->";
      *os << arg->local_name () << "_);";
      break;
    default:
      *os << arg->local_name () << "_;";
      break;
    }

  return 0;
}
