//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_typedef.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Typedef
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_typedef.h"

// ******************************************************
// Generic Typedef visitor
// ******************************************************

be_visitor_typedef::be_visitor_typedef (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_typedef::~be_visitor_typedef (void)
{
}

// visit the Typedef node and its scope
int be_visitor_typedef::visit_typedef (be_typedef *node)
{
  return -1; // must be overridden
}

// =all common visit methods for typedef visitor

// visit a array
int
be_visitor_typedef::visit_array (be_array *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same


  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      ctx.state (TAO_CodeGen::TAO_ARRAY_CH);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      ctx.state (TAO_CodeGen::TAO_ARRAY_CS);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      ctx.state (TAO_CodeGen::TAO_ARRAY_CI);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_ARRAY_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_ARRAY_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_SH:
    case TAO_CodeGen::TAO_TYPEDEF_SI:
    case TAO_CodeGen::TAO_TYPEDEF_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typedef::"
                           "visit_array - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_array - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_array - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

//visit an enum
int
be_visitor_typedef::visit_enum (be_enum *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_CH);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_CS);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CI:
    case TAO_CodeGen::TAO_TYPEDEF_SH:
    case TAO_CodeGen::TAO_TYPEDEF_SI:
    case TAO_CodeGen::TAO_TYPEDEF_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typedef::"
                           "visit_enum - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_enum - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_enum - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an predefined type
int
be_visitor_typedef::visit_predefined_type (be_predefined_type *node)
{
  return 0;
}

// visit an sequence
int
be_visitor_typedef::visit_sequence (be_sequence *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CH);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CI);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CS);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_SH:
    case TAO_CodeGen::TAO_TYPEDEF_SI:
    case TAO_CodeGen::TAO_TYPEDEF_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typedef::"
                           "visit_sequence - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_sequence - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_sequence - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an structure
int
be_visitor_typedef::visit_string (be_string *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH:
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS:
    case TAO_CodeGen::TAO_TYPEDEF_SH:
    case TAO_CodeGen::TAO_TYPEDEF_SI:
    case TAO_CodeGen::TAO_TYPEDEF_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typedef::"
                           "visit_string - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_string - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_string - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an structure
int
be_visitor_typedef::visit_structure (be_structure *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_SH:
    case TAO_CodeGen::TAO_TYPEDEF_SI:
    case TAO_CodeGen::TAO_TYPEDEF_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typedef::"
                           "visit_structure - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_structure - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_structure - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an union
int
be_visitor_typedef::visit_union (be_union *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_CH);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      ctx.state (TAO_CodeGen::TAO_UNION_CI);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_CS);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_TYPEDEF_SH:
    case TAO_CodeGen::TAO_TYPEDEF_SI:
    case TAO_CodeGen::TAO_TYPEDEF_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typedef::"
                           "visit_union - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_union - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_union - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

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
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();

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

  be_type *bt; // base type

  if (this->ctx_->tdef ())
    {
      // the fact that we are here indicates that we were generating code for a
      // typedef node whose base type also happens to be another typedef-ed
      // (i.e. an alias) node for another (possibly alias) node

      this->ctx_->alias (node); // save this alias

      // grab the most primitive base type in the chain to avoid recusrsively
      // going thru this visit method
      bt = node->primitive_base_type ();
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "bad primitive base type\n"
                             ),  -1);
        }

      // accept on this base type, but generate code for the typedef node
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"
                             ),  -1);
        }
      this->ctx_->alias (0); // reset
    }
  else
    {
      // the context has not stored any "tdef" node. So we must be in here for
      // the first time
      this->ctx_->tdef (node); // save the typedef node

      // grab the immediate base type node
      bt = be_type::narrow_from_decl (node->base_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "bad base type\n"
                             ),  -1);
        }

      // accept on this base type, but generate code for the typedef node
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"
                             ),  -1);
        }

      // generate the typecode decl for this typedef node
      if (!node->imported ())
        {
          if (node->is_nested ())
            {
              *os << "static CORBA::TypeCode_ptr "
                  << node->tc_name ()->last_component () << ";\n\n";
            }
          else
            {
              // we are in the ROOT scope
              *os << "extern CORBA::TypeCode_ptr "
                  << node->tc_name ()->last_component () << ";\n\n";
            }
        }
      this->ctx_->tdef (0); // reset
    }

  return 0;
}

int
be_visitor_typedef_ch::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!bt->imported () &&
      bt->node_type () == AST_Decl::NT_array) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_array (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_array - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }

  // now generate the typedefs
  os->indent ();
  // typedef the type and the _slice type
  *os << "typedef " << bt->nested_type_name (scope)
      << " " << tdef->nested_type_name (scope) << ";" << be_nl;
  *os << "typedef " << bt->nested_type_name (scope, "_slice")
      << " " << tdef->nested_type_name (scope, "_slice") << ";" << be_nl;
  // typedef the _var, _out, and _forany types
  *os << "typedef " << bt->nested_type_name (scope, "_var")
      << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;
  *os << "typedef " << bt->nested_type_name (scope, "_out")
      << " " << tdef->nested_type_name (scope, "_out") << ";" << be_nl;
  *os << "typedef " << bt->nested_type_name (scope, "_forany")
      << " " << tdef->nested_type_name (scope, "_forany") << ";" << be_nl;
  // the _alloc, _dup, and free methods
  *os << tdef->nested_type_name (scope, "_slice") << " *"
      << tdef->nested_type_name (scope, "_alloc") << " (void);" << be_nl;
  *os << tdef->nested_type_name (scope, "_slice") << " *"
      << tdef->nested_type_name (scope, "_dup") << " ("
      << tdef->nested_type_name (scope, "_slice") << " *slice);" << be_nl;
  *os << tdef->nested_type_name (scope, "_slice") << " *"
      << tdef->nested_type_name (scope, "_free") << " ("
      << tdef->nested_type_name (scope, "_slice") << " *slice);" << be_nl;
  return 0;
}

int
be_visitor_typedef_ch::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_enum) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_enum (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_enum - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }

  // now generate the typedefs
  os->indent ();
  // typedef the type and the _slice type
  *os << "typedef " << bt->nested_type_name (scope)
      << " " << tdef->nested_type_name (scope) << ";" << be_nl;
  // typedef the _out
  *os << "typedef " << bt->nested_type_name (scope, "_out")
      << " " << tdef->nested_type_name (scope, "_out") << ";" << be_nl;
  return 0;
}

int
be_visitor_typedef_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  // now generate the typedefs
  os->indent ();

  // typedef the _ptr
  *os << "typedef " << bt->nested_type_name (scope, "_ptr")
      << " " << tdef->nested_type_name (scope, "_ptr") << ";" << be_nl;

  // typedef the _var
  *os << "typedef " << bt->nested_type_name (scope, "_var")
      << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;

  // typedef the _out
  *os << "typedef " << bt->nested_type_name (scope, "_out")
      << " " << tdef->nested_type_name (scope, "_out") << ";" << be_nl;

  return 0;
}

int
be_visitor_typedef_ch::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  // now generate the typedefs
  os->indent ();
  // typedef the type
  *os << "typedef " << bt->nested_type_name (scope)
      << " " << tdef->nested_type_name (scope) << ";" << be_nl;
  if (node->pt () == AST_PredefinedType::PT_pseudo)
    {
      // typedef the _ptr
      *os << "typedef " << bt->nested_type_name (scope, "_ptr")
          << " " << tdef->nested_type_name (scope, "_ptr") << ";" << be_nl;
    }
  // typedef the _out
  *os << "typedef " << bt->nested_type_name (scope, "_out")
      << " " << tdef->nested_type_name (scope, "_out") << ";" << be_nl;
  return 0;
}

int
be_visitor_typedef_ch::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used

  // now generate the typedefs
  os->indent ();
  // typedef the type and the _slice type
  *os << "typedef char *"
      << " " << tdef->nested_type_name (scope) << ";" << be_nl;
  // typedef the _var, _out, and _forany types
  *os << "typedef CORBA::String_var"
      << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;
  *os << "typedef CORBA::String_out"
      << " " << tdef->nested_type_name (scope, "_out") << ";" << be_nl;
  return 0;
}

int
be_visitor_typedef_ch::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_sequence) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_sequence (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_sequence - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }
  else
    {
      // now generate the typedefs
      os->indent ();
      // typedef the type
      *os << "typedef " << bt->nested_type_name (scope)
          << " " << tdef->nested_type_name (scope) << ";" << be_nl;
      // typedef the _var, _out types
      *os << "typedef " << bt->nested_type_name (scope, "_var")
          << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;
      *os << "typedef " << bt->nested_type_name (scope, "_out")
          << " " << tdef->nested_type_name (scope, "_out") << ";" << be_nl;
    }
  return 0;
}

int
be_visitor_typedef_ch::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_struct) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_structure (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_structure - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }

  // now generate the typedefs
  os->indent ();
  // typedef the type
  *os << "typedef " << bt->nested_type_name (scope)
      << " " << tdef->nested_type_name (scope) << ";" << be_nl;
  // typedef the _var, _out types
  *os << "typedef " << bt->nested_type_name (scope, "_var")
      << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;
  *os << "typedef " << bt->nested_type_name (scope, "_out")
      << " " << tdef->nested_type_name (scope, "_out") << ";" << be_nl;
  return 0;
}

int
be_visitor_typedef_ch::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_union) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_union (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_union - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }

  // now generate the typedefs
  os->indent ();
  // typedef the type and the _slice type
  *os << "typedef " << bt->nested_type_name (scope)
      << " " << tdef->nested_type_name (scope) << ";" << be_nl;
  // typedef the _var, _out types
  *os << "typedef " << bt->nested_type_name (scope, "_var")
      << " " << tdef->nested_type_name (scope, "_var") << ";" << be_nl;
  *os << "typedef " << bt->nested_type_name (scope, "_out")
      << " " << tdef->nested_type_name (scope, "_out") << ";" << be_nl;
  return 0;
}

// ******************************************************
// Typedef visitor for client inline
// ******************************************************

be_visitor_typedef_ci::be_visitor_typedef_ci (be_visitor_context *ctx)
  : be_visitor_typedef (ctx)
{
}

be_visitor_typedef_ci::~be_visitor_typedef_ci (void)
{
}

int
be_visitor_typedef_ci::visit_typedef (be_typedef *node)
{
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();

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
  // will additionally have the _forany type as well as inlined *_alloc
  // methods.
  //
  // Finally, we need to differentiate between the case where we were
  // generating code for
  // typedef sequence<long> X; and
  // typedef Y Z; where Y was somehow aliased to the sequence. In the former
  // case, we will need to generate all the code for sequence<long> or whatever
  // the type maybe. In the latter, we just need typedefs for the type and all
  // associated _var, _out, and other types.

  be_type *bt; // base type

  if (this->ctx_->tdef ())
    {
      // the fact that we are here indicates that we were generating code for a
      // typedef node whose base type also happens to be another typedef-ed
      // (i.e. an alias) node for another (possibly alias) node

      this->ctx_->alias (node); // save this alias

      // grab the most primitive base type in the chain to avoid recusrsively
      // going thru this visit method
      bt = node->primitive_base_type ();
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "bad primitive base type\n"
                             ),  -1);
        }
      // accept on this base type
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"
                             ),  -1);
        }

      this->ctx_->alias (0);
    }
  else
    {
      // the context has not stored any "tdef" node. So we must be in here for
      // the first time
      this->ctx_->tdef (node); // save the typedef node

      // grab the immediate base type node
      bt = be_type::narrow_from_decl (node->base_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "bad base type\n"
                             ),  -1);
        }
      // accept on this base type
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"
                             ),  -1);
        }

      this->ctx_->tdef (0);
    }
  return 0;
}

int
be_visitor_typedef_ci::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_array) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_array (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ci::"
                             "visit_array - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }
  return 0;
}

int
be_visitor_typedef_ci::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_sequence) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_sequence (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ci::"
                             "visit_sequence - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }
  return 0;
}

int
be_visitor_typedef_ci::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_struct) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_structure (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ci::"
                             "visit_structure - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }
  return 0;
}

int
be_visitor_typedef_ci::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_union) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_union (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ci::"
                             "visit_union - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }
  return 0;
}

// ******************************************************
// Typedef visitor for client stubs
// ******************************************************

be_visitor_typedef_cs::be_visitor_typedef_cs (be_visitor_context *ctx)
  : be_visitor_typedef (ctx)
{
}

be_visitor_typedef_cs::~be_visitor_typedef_cs (void)
{
}

int
be_visitor_typedef_cs::visit_typedef (be_typedef *node)
{
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();

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
  // will additionally have the _forany type as well as inlined *_alloc
  // methods.
  //
  // Finally, we need to differentiate between the case where we were
  // generating code for
  // typedef sequence<long> X; and
  // typedef Y Z; where Y was somehow aliased to the sequence. In the former
  // case, we will need to generate all the code for sequence<long> or whatever
  // the type maybe. In the latter, we just need typedefs for the type and all
  // associated _var, _out, and other types.

  be_type *bt; // base type

  if (this->ctx_->tdef ())
    {
      // the fact that we are here indicates that we were generating code for a
      // typedef node whose base type also happens to be another typedef-ed
      // (i.e. an alias) node for another (possibly alias) node

      this->ctx_->alias (node); // save this alias

      // grab the most primitive base type in the chain to avoid recusrsively
      // going thru this visit method
      bt = node->primitive_base_type ();
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "bad primitive base type\n"
                             ),  -1);
        }
      // accept on this base type
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"
                             ),  -1);
        }
      this->ctx_->alias (0);
    }
  else
    {
      // the context has not stored any "tdef" node. So we must be in here for
      // the first time
      this->ctx_->tdef (node); // save the typedef node

      // grab the immediate base type node
      bt = be_type::narrow_from_decl (node->base_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "bad base type\n"
                             ),  -1);
        }
      // accept on this base type
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"
                             ),  -1);
        }

      if (!node->imported ())
        {
          // generate the typecode information here
          os->indent (); // start from current indentation level
          *os << "static const CORBA::Long _oc_" << node->flatname () << "[] ="
              << be_nl;
          *os << "{\n";
          os->incr_indent (0);
          if (node->gen_encapsulation () == -1)
            {
              ACE_ERROR ((LM_ERROR, "Error generating typecode\n\n"));
              return -1;
            }
          os->decr_indent ();
          *os << "};" << be_nl;

          *os << "static CORBA::TypeCode _tc__tc_" << node->flatname ()
              << " (CORBA::tk_alias, sizeof (_oc_" << node->flatname ()
              << "), (char *) &_oc_" << node->flatname ()
              << ", CORBA::B_FALSE);" << be_nl;
          *os << "CORBA::TypeCode_ptr " << node->tc_name () << " = &_tc__tc_"
              << node->flatname () << ";\n\n";

        }
      this->ctx_->tdef (0);
    }

  return 0;
}

// ***************************************************************************
// Typedef visitor for generating Any operator declarations in the client header
// ***************************************************************************

be_visitor_typedef_any_op_ch::be_visitor_typedef_any_op_ch
(be_visitor_context *ctx)
  : be_visitor_typedef (ctx)
{
}

be_visitor_typedef_any_op_ch::~be_visitor_typedef_any_op_ch (void)
{
}

int
be_visitor_typedef_any_op_ch::visit_typedef (be_typedef *node)
{
  if (node->cli_hdr_any_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = tao_cg->client_header ();

  // generate the Any <<= and >>= operator declarations
  // Any <<= and >>= operators
  os->indent ();

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

  be_type *bt; // base type

  if (this->ctx_->tdef ())
    {
      // the fact that we are here indicates that we were generating code for a
      // typedef node whose base type also happens to be another typedef-ed
      // (i.e. an alias) node for another (possibly alias) node

      this->ctx_->alias (node); // save this alias

      // grab the most primitive base type in the chain to avoid recusrsively
      // going thru this visit method
      bt = node->primitive_base_type ();
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_ch::"
                             "visit_typedef - "
                             "bad primitive base type\n"
                             ),  -1);
        }

      // accept on this base type, but generate code for the typedef node
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"
                             ),  -1);
        }
      this->ctx_->alias (0); // reset
    }
  else
    {
      // the context has not stored any "tdef" node. So we must be in here for
      // the first time
      this->ctx_->tdef (node); // save the typedef node

      // grab the immediate base type node
      bt = be_type::narrow_from_decl (node->base_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_ch::"
                             "visit_typedef - "
                             "bad base type\n"
                             ),  -1);
        }

      // accept on this base type, but generate code for the typedef node
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"
                             ),  -1);
        }

      this->ctx_->tdef (0); // reset
    }

  node->cli_hdr_any_op_gen (1);
  return 0;
}

int
be_visitor_typedef_any_op_ch::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!bt->imported () &&
      bt->node_type () == AST_Decl::NT_array) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_array (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_ch::"
                             "visit_array - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }

  return 0;
}

int
be_visitor_typedef_any_op_ch::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_enum) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_enum (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_ch::"
                             "visit_enum - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }

  return 0;
}

int
be_visitor_typedef_any_op_ch::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_sequence) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_sequence (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_ch::"
                             "visit_sequence - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }

  return 0;
}

int
be_visitor_typedef_any_op_ch::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_struct) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_structure (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_ch::"
                             "visit_structure - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }

  return 0;
}

int
be_visitor_typedef_any_op_ch::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_union) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_union (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_ch::"
                             "visit_union - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }

  return 0;
}

// ***************************************************************************
// Typedef visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_typedef_any_op_cs::be_visitor_typedef_any_op_cs
(be_visitor_context *ctx)
  : be_visitor_typedef (ctx)
{
}

be_visitor_typedef_any_op_cs::~be_visitor_typedef_any_op_cs (void)
{
}

int
be_visitor_typedef_any_op_cs::visit_typedef (be_typedef *node)
{
  if (node->cli_stub_any_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = tao_cg->client_stubs ();

  // generate the Any <<= and >>= operator declarations
  // Any <<= and >>= operators
  os->indent ();

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

  be_type *bt; // base type

  if (this->ctx_->tdef ())
    {
      // the fact that we are here indicates that we were generating code for a
      // typedef node whose base type also happens to be another typedef-ed
      // (i.e. an alias) node for another (possibly alias) node

      this->ctx_->alias (node); // save this alias

      // grab the most primitive base type in the chain to avoid recusrsively
      // going thru this visit method
      bt = node->primitive_base_type ();
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_cs::"
                             "visit_typedef - "
                             "bad primitive base type\n"
                             ),  -1);
        }

      // accept on this base type, but generate code for the typedef node
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_cs::"
                             "visit_typedef - "
                             "failed to accept visitor\n"
                             ),  -1);
        }
      this->ctx_->alias (0); // reset
    }
  else
    {
      // the context has not stored any "tdef" node. So we must be in here for
      // the first time
      this->ctx_->tdef (node); // save the typedef node

      // grab the immediate base type node
      bt = be_type::narrow_from_decl (node->base_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_cs::"
                             "visit_typedef - "
                             "bad base type\n"
                             ),  -1);
        }

      // accept on this base type, but generate code for the typedef node
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_cs::"
                             "visit_typedef - "
                             "failed to accept visitor\n"
                             ),  -1);
        }

      this->ctx_->tdef (0); // reset
    }

  node->cli_stub_any_op_gen (1);
  return 0;
}

int
be_visitor_typedef_any_op_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!bt->imported () &&
      bt->node_type () == AST_Decl::NT_array) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_array (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_cs::"
                             "visit_array - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }

  return 0;
}

int
be_visitor_typedef_any_op_cs::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_enum) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_enum (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_cs::"
                             "visit_enum - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }

  return 0;
}

int
be_visitor_typedef_any_op_cs::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_sequence) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_sequence (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_cs::"
                             "visit_sequence - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }

  return 0;
}

int
be_visitor_typedef_any_op_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_struct) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_structure (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_cs::"
                             "visit_structure - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }

  return 0;
}

int
be_visitor_typedef_any_op_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_union) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_union (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_any_op_cs::"
                             "visit_union - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }

  return 0;
}
