//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    elemtype.cpp
//
// = DESCRIPTION
//    Visitor generating code for the element type for the Sequence
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_sequence, 
           elemtype, 
           "$Id$")

// ***********************************************************
// sequence element type visitor for return types of [] methods
// ***********************************************************


be_visitor_sequence_elemtype::be_visitor_sequence_elemtype (
    be_visitor_context *ctx
  )
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
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
    {
      *os << bt->nested_type_name (this->ctx_->scope ()) << " &";
    }
  else
    {
      *os << bt->name () << " &";
    }

  return 0;
}

int
be_visitor_sequence_elemtype::visit_predefined_type (be_predefined_type *node)
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
      {
        int is_pseudo_object =
          ACE_OS::strcmp (node->local_name ()->get_string (),
                          "Object") != 0;

        if (is_pseudo_object)
          {
            *os << "TAO_Pseudo_Object_Manager<";
          }
        else
          {
            *os << "TAO_Object_Manager<";
          }

        if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
          {
            *os << bt->nested_type_name (this->ctx_->scope ()) << ",";
            *os << bt->nested_type_name (this->ctx_->scope (), "_var") << ">";
          }
        else
          {
            *os << bt->name () << ","
                << bt->name () <<"_var>";
          }
      }
      break;
    default:
      if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
        {
	        *os << bt->nested_type_name (this->ctx_->scope ()) << " &";
        }
      else
        {
	        *os << bt->name () << " &";
        }
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
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
    {
      *os << "TAO_Object_Manager<";
      *os << bt->nested_type_name (this->ctx_->scope ()) << ", ";
      *os << bt->nested_type_name (this->ctx_->scope (), "_var") << ">";
    }
  else
    {
      *os << "TAO_Object_Manager<";
      *os << bt->name () << ", ";
      *os << bt->name () << "_var>";
    }

  return 0;
}

int
be_visitor_sequence_elemtype::visit_valuetype (be_valuetype *node)
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

  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
    {
      *os << "TAO_Valuetype_Manager<";
      *os << bt->nested_type_name (this->ctx_->scope ()) << ", ";
      *os << bt->nested_type_name (this->ctx_->scope (), "_var") << ">";
    }
  else
    {
      *os << "TAO_Valuetype_Manager<";
      *os << bt->name () << ", ";
      *os << bt->name () << "_var>";
    }

  return 0;
}

int
be_visitor_sequence_elemtype::visit_interface_fwd (be_interface_fwd *node)
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

  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
    {
      *os << "TAO_Object_Manager<";
      *os << bt->nested_type_name (this->ctx_->scope ()) << ",";
      *os << bt->nested_type_name (this->ctx_->scope (), "_var") << ">";
    }
  else
    {
      *os << "TAO_Object_Manager<";
      *os << bt->name () << ",";
      *os << bt->name () << "_var>";
    }

  return 0;
}

int
be_visitor_sequence_elemtype::visit_valuetype_fwd (be_valuetype_fwd *node)
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

  if (this->ctx_->state () == TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH)
    {
      *os << "TAO_Valuetype_Manager<";
      *os << bt->nested_type_name (this->ctx_->scope ()) << ",";
      *os << bt->nested_type_name (this->ctx_->scope (), "_var") << ">";
    }
  else
    {
      *os << "TAO_Valuetype_Manager<";
      *os << bt->name () << ",";
      *os << bt->name () << "_var>";
    }

  return 0;
}

int
be_visitor_sequence_elemtype::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (node->width () == (long) sizeof (char))
    {
      *os << "TAO_SeqElem_String_Manager";
    }
  else
    {
      *os << "TAO_SeqElem_WString_Manager";
    }

  return 0;
}

int
be_visitor_sequence_elemtype::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_structure_fwd (be_structure_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_union_fwd (be_union_fwd *node)
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
