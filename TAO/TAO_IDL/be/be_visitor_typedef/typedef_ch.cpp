//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    typedef_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Typedef in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID(be_visitor_typedef, 
          typedef_ch, 
          "$Id$")

// ******************************************************
// Typedef visitor for client header
// ******************************************************

be_visitor_typedef_ch::be_visitor_typedef_ch (be_visitor_context *ctx)
  : be_visitor_typedef (ctx)
{
}

be_visitor_typedef_ch::~be_visitor_typedef_ch (void)
{
}

int
be_visitor_typedef_ch::visit_typedef (be_typedef *node)
{
  // In general, we may have a chain of typedefs. i.e.,
  // typedef sequence<long> X;
  // typedef X Y;
  // typedef Y Z; and so on
  // The first time we will be in will be for node Z for which the code
  // generation has to take place. However, it is not enough to just generate
  // code that looks like -
  // typedef Y Z;
  // For different types (in this case we have a sequence), we will need
  // typedefs for the _var and _out types for Z. If it had been an array, we
  // will additionally have the _forany type as well as inlined *_alloc, _dup,
  // and _free methods.
  //
  // Finally, we need to differentiate between the case where we were
  // generating code for
  // typedef sequence<long> X; and
  // typedef Y Z; where Y was somehow aliased to the sequence. In the former
  // case, we will need to generate all the code for sequence<long> or whatever
  // the type maybe. In the latter, we just need typedefs for the type and all
  // associated _var, _out, and other types.

  be_type *bt;

  if (this->ctx_->tdef ())
    {
      // The fact that we are here indicates that we were generating code for
      // a typedef node whose base type also happens to be another typedefed
      // (i.e. an alias) node for another (possibly alias) node.

      this->ctx_->alias (node);

      // Grab the most primitive base type in the chain to avoid recusrsively
      // going thru this visit method.
      bt = node->primitive_base_type ();

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "bad primitive base type\n"),
                            -1);
        }

      // Accept on this base type, but generate code for the typedef node.
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"),
                            -1);
        }

      this->ctx_->alias (0);
    }
  else
    {
      // The context has not stored any "tdef" node. So we must be in here for
      // the first time.
      this->ctx_->tdef (node);

      // Grab the immediate base type node.
      bt = be_type::narrow_from_decl (node->base_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "bad base type\n"),
                            -1);
        }

      // accept on this base type, but generate code for the typedef node.
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"),
                            -1);
        }

      // Generate the typecode decl for this typedef node.
      if (!node->imported () && be_global->tc_support ())
        {
          be_visitor_context ctx (*this->ctx_);
          be_visitor_typecode_decl visitor (&ctx);

          if (node->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_typedef_ch::"
                                 "visit_typedef - "
                                 "TypeCode declaration failed\n"),
                                -1);
            }
        }

      this->ctx_->tdef (0);
    }

  return 0;
}

int
be_visitor_typedef_ch::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_typedef *tdef = this->ctx_->tdef ();
  be_decl *scope = this->ctx_->scope ();
  be_type *bt;

  // Is the base type an alias to an array node or an actual array node.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  // Is our base type an array node? If so, generate code for that array node.
  // In the first layer of typedef for an array, cli_hdr_gen() causes us to
  // skip all the code reached from the first branch. Then the ELSE branch is
  // skipped and we fail to generate any typedefs for that node. Adding the
  // check for cli_hdr_gen() to the IF statement keeps it in. Subsequent
  // layers of typedef, if any, assign the context alias to bt, so we go
  // straight to the ELSE branch.
  if (bt->node_type () == AST_Decl::NT_array && bt->cli_hdr_gen () == 0)
    {
      // Let the base class visitor handle this case.
      if (this->be_visitor_typedef::visit_array (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_array - "
                             "base class visitor failed \n"),
                            -1);
        }
    }
  else
    {
      *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

      // Base type is simply an alias to an array node. Simply output the
      // required typedefs.

      // Typedef the type and the _slice type.
      *os << "typedef " << bt->nested_type_name (scope)
          << " " << tdef->nested_type_name (scope) << ";" << be_nl;
      *os << "typedef " << bt->nested_type_name (scope, "_slice")
          << " " << tdef->nested_type_name (scope, "_slice") << ";" << be_nl;
      // Typedef the _var, _out, _forany, and _life types.
      *os << "typedef " << bt->nested_type_name (scope, "_var")
          << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;
      *os << "typedef " << bt->nested_type_name (scope, "_out")
          << " " << tdef->nested_type_name (scope, "_out") << ";" << be_nl;
      *os << "typedef " << bt->nested_type_name (scope, "_forany")
          << " " << tdef->nested_type_name (scope, "_forany") << ";" << be_nl;
      *os << "typedef " << bt->fwd_helper_name () << "_life "
          << tdef->fwd_helper_name () << "_life;" << be_nl;

      // The _alloc, _dup, copy, and free methods

      // Since the function nested_type_name() contains a static buffer,
      // we can have only one call to it from any instantiation per stream
      // output statement.

      // _alloc
      *os << "ACE_INLINE " << tdef->nested_type_name (scope, "_slice") << " *";
      *os << tdef->nested_type_name (scope, "_alloc") << " (void);" << be_nl;
      // _dup
      *os << "ACE_INLINE " << tdef->nested_type_name (scope, "_slice") << " *";
      *os << tdef->nested_type_name (scope, "_dup") << " (const ";
      *os << tdef->nested_type_name (scope, "_slice") << " *_tao_slice);" << be_nl;
      // _copy
      *os << "ACE_INLINE void " << tdef->nested_type_name (scope, "_copy") << " (";
      *os << tdef->nested_type_name (scope, "_slice") << " *_tao_to, const ";
      *os << tdef->nested_type_name (scope, "_slice") << " *_tao_from);" << be_nl;
      // _free
      *os << "ACE_INLINE void " << tdef->nested_type_name (scope, "_free") << " (";
      *os << tdef->nested_type_name (scope, "_slice") << " *_tao_slice);";
    }

  return 0;
}

int
be_visitor_typedef_ch::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_typedef *tdef = this->ctx_->tdef ();
  be_decl *scope = this->ctx_->scope ();
  be_type *bt;

  // Typedef of a typedef?
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (bt->node_type () == AST_Decl::NT_enum)
    {
      // Let the base class visitor handle this case.
      if (this->be_visitor_typedef::visit_enum (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_enum - "
                             "base class visitor failed \n"),
                            -1);
        }
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // typedef the type and the _slice type.
  *os << "typedef " << bt->nested_type_name (scope)
      << " " << tdef->nested_type_name (scope) << ";" << be_nl;
  // Typedef the _out
  *os << "typedef " << bt->nested_type_name (scope, "_out")
      << " " << tdef->nested_type_name (scope, "_out") << ";";

  return 0;
}

int
be_visitor_typedef_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_typedef *tdef = this->ctx_->tdef ();
  be_decl *scope = this->ctx_->scope ();
  be_type *bt;

  // Typedef of a typedef?
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Typedef the object.
  *os << "typedef " << bt->nested_type_name (scope) << " "
      << tdef->nested_type_name (scope) << ";" << be_nl;

  // Typedef the _ptr.
  *os << "typedef " << bt->nested_type_name (scope, "_ptr")
      << " " << tdef->nested_type_name (scope, "_ptr") << ";" << be_nl;

  // Typedef the _var.
  *os << "typedef " << bt->nested_type_name (scope, "_var")
      << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;

  // typedef the _out
  *os << "typedef " << bt->nested_type_name (scope, "_out")
      << " " << tdef->nested_type_name (scope, "_out") << ";" << be_nl;

  // typedef the _life
  *os << "typedef " << bt->fwd_helper_name () << "_life "
      << tdef->fwd_helper_name () << "_life;" << be_nl;

  // typedef the _cast
  *os << "typedef " << bt->fwd_helper_name () << "_cast "
      << tdef->fwd_helper_name () << "_cast;";

  return 0;
}

int
be_visitor_typedef_ch::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_typedef *tdef = this->ctx_->tdef ();
  be_decl *scope = this->ctx_->scope ();
  be_type *bt;

  // Typedef of a typedef?
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Typedef the type.
  *os << "typedef " << bt->nested_type_name (scope)
      << " " << tdef->nested_type_name (scope) << ";" << be_nl;

  AST_PredefinedType::PredefinedType pt = node->pt ();

  if (pt == AST_PredefinedType::PT_pseudo
      || pt == AST_PredefinedType::PT_any
      || pt == AST_PredefinedType::PT_object)
    {
      // Typedef the _ptr and _var.
      *os << "typedef " << bt->nested_type_name (scope, "_ptr")
          << " " << tdef->nested_type_name (scope, "_ptr") << ";" << be_nl;
      *os << "typedef " << bt->nested_type_name (scope, "_var")
          << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;
    }

  // Typedef the _out.
  *os << "typedef " << bt->nested_type_name (scope, "_out")
      << " " << tdef->nested_type_name (scope, "_out") << ";";

  return 0;
}

int
be_visitor_typedef_ch::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_typedef *tdef = this->ctx_->tdef ();
  be_decl *scope = this->ctx_->scope ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  if (node->width () == (long) sizeof (char))
    {
      *os << "typedef char *"
          << " " << tdef->nested_type_name (scope) << ";" << be_nl;
      // Typedef the _var and _out types.
      *os << "typedef CORBA::String_var"
          << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;
      *os << "typedef CORBA::String_out"
          << " " << tdef->nested_type_name (scope, "_out") << ";";
    }
  else
    {
      *os << "typedef CORBA::WChar *"
          << " " << tdef->nested_type_name (scope) << ";" << be_nl;
      // Typedef the _var and _out types.
      *os << "typedef CORBA::WString_var"
          << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;
      *os << "typedef CORBA::WString_out"
          << " " << tdef->nested_type_name (scope, "_out") << ";";
    }

  return 0;
}

int
be_visitor_typedef_ch::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_typedef *tdef = this->ctx_->tdef ();
  be_decl *scope = this->ctx_->scope ();
  be_type *bt;

  // Typedef of a typedef?
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      // Let the base class visitor handle this case.
      if (this->be_visitor_typedef::visit_sequence (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_sequence - "
                             "base class visitor failed \n"),  
                            -1);
        }
    }
  else
    {
      *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

      // Typedef the type.
      *os << "typedef " << bt->nested_type_name (scope)
          << " " << tdef->nested_type_name (scope) << ";" << be_nl;
      // Typedef the _var and _out types.
      *os << "typedef " << bt->nested_type_name (scope, "_var")
          << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;
      *os << "typedef " << bt->nested_type_name (scope, "_out")
          << " " << tdef->nested_type_name (scope, "_out") << ";";
    }

  return 0;
}

int
be_visitor_typedef_ch::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_typedef *tdef = this->ctx_->tdef ();
  be_decl *scope = this->ctx_->scope ();
  be_type *bt;

  // Typedef of a typedef?
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (bt->node_type () == AST_Decl::NT_struct)
    {
      // Let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_structure (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_structure - "
                             "base class visitor failed \n"),  
                            -1);
        }
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Typedef the type.
  *os << "typedef " << bt->nested_type_name (scope)
      << " " << tdef->nested_type_name (scope) << ";" << be_nl;
  // typedef the _var and _out types.
  *os << "typedef " << bt->nested_type_name (scope, "_var")
      << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;
  *os << "typedef " << bt->nested_type_name (scope, "_out")
      << " " << tdef->nested_type_name (scope, "_out") << ";";

  return 0;
}

int
be_visitor_typedef_ch::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_typedef *tdef = this->ctx_->tdef ();
  be_decl *scope = this->ctx_->scope ();
  be_type *bt;

  // Typedef of a typedef?
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (bt->node_type () == AST_Decl::NT_union)
    {
      // Let the base class visitor handle this case.
      if (this->be_visitor_typedef::visit_union (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_union - "
                             "base class visitor failed \n"),  
                            -1);
        }
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Typedef the type.
  *os << "typedef " << bt->nested_type_name (scope)
      << " " << tdef->nested_type_name (scope) << ";" << be_nl;
  // Typedef the _var and _out types.
  *os << "typedef " << bt->nested_type_name (scope, "_var")
      << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;
  *os << "typedef " << bt->nested_type_name (scope, "_out")
      << " " << tdef->nested_type_name (scope, "_out") << ";";

  return 0;
}

int
be_visitor_typedef_ch::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_typedef *tdef = this->ctx_->tdef ();
  be_decl *scope = this->ctx_->scope ();
  be_type *bt;

  // Typedef of a typedef?
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Typedef the object.
  *os << "typedef " << bt->nested_type_name (scope) << " "
      << tdef->nested_type_name (scope) << ";" << be_nl;

  // Typedef the _var.
  *os << "typedef " << bt->nested_type_name (scope, "_var")
      << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;

  // typedef the _out
  *os << "typedef " << bt->nested_type_name (scope, "_out")
      << " " << tdef->nested_type_name (scope, "_out") << ";" << be_nl;

  // typedef the _life
  *os << "typedef " << bt->fwd_helper_name () << "_life "
      << tdef->fwd_helper_name () << "_life;";

  return 0;
}

