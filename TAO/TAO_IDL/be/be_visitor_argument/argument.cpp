
//=============================================================================
/**
 *  @file    argument.cpp
 *
 *  generic visitor for Argument node
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "argument.h"

#include <string_view>
#include "argument.h"

be_visitor_args::be_visitor_args (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

int be_visitor_args::visit_argument (be_argument *)
{
  return -1;
}

// Helper that returns the type name either as a nested type name (for header
// files) or as a fully scoped name. In addition, we make sure that if the type
// is an alias, we use that name.
std::string
be_visitor_args::type_name (be_type *node,
                            std::string_view suffix)
{
  be_type *bt = nullptr;

  // Use the typedefed name if that is the one
  // used in the IDL defn.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  std::string name ("::");
  name += bt->full_name ();

  if (!suffix.empty ())
    {
      name.append (suffix.data (), suffix.size ());
    }

  return name;
}

// Helper that returns the direction type of the argument
AST_Argument::Direction
be_visitor_args::direction ()
{
  if (this->fixed_direction_ != -1)
    {
      return AST_Argument::Direction (this->fixed_direction_);
    }

  // Grab the argument node. We know that our context has stored the right
  // argument node.
  be_argument *arg =
    dynamic_cast<be_argument*> (this->ctx_->node ());

  return arg->direction ();
}

void
be_visitor_args::set_fixed_direction (AST_Argument::Direction direction)
{
  this->fixed_direction_ = direction;
}

int
be_visitor_args::gen_pd_arg (be_predefined_type *node,
                             bool for_stub)
{
  TAO_CodeGen::CG_SUB_STATE ss = this->ctx_->sub_state ();
  AST_Argument::Direction d = this->direction ();

  bool in_arg = (d == AST_Argument::dir_IN);
  bool out_arg = (d == AST_Argument::dir_OUT);
  bool out_stream = (ss == TAO_CodeGen::TAO_CDR_OUTPUT);
  bool in_stream = (ss == TAO_CodeGen::TAO_CDR_INPUT);

  bool skip = (in_arg && for_stub && in_stream)
              || (in_arg && !for_stub && out_stream)
              || (out_arg && for_stub && out_stream)
              || (out_arg && !for_stub && in_stream);

  if (skip)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  std::string_view var_call;
  std::string_view any_deref;

  AST_PredefinedType::PredefinedType pt = node->pt ();
  bool is_any = (pt == AST_PredefinedType::PT_any);

  if (for_stub)
    {
      if (in_stream && out_arg)
        {
          var_call = ".ptr ()";
          any_deref = "*";
        }
    }
  else if (out_stream)
    {
      var_call = ".in ()";

      if (is_any && !out_arg)
        {
          var_call = "";
        }
    }
  else if (!is_any)
    {
      var_call = ".out ()";
    }

  std::string_view const to_from = in_stream
    ? "::ACE_InputCDR::to_"
    : "::ACE_OutputCDR::from_";

  be_argument *arg =
    dynamic_cast<be_argument*> (this->ctx_->node ());
  std::string const& lname =
    arg->local_name ()->get_string ();

  switch (pt)
  {
    case AST_PredefinedType::PT_any:
      *os << any_deref;
      ACE_FALLTHROUGH;
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_object:
      *os << lname << var_call;
      break;
    case AST_PredefinedType::PT_char:
      *os << to_from << "char (" << lname << ")";
      break;
    case AST_PredefinedType::PT_wchar:
      *os << to_from << "wchar (" << lname << ")";
      break;
    case AST_PredefinedType::PT_boolean:
      *os << to_from << "boolean (" << lname << ")";
      break;
    case AST_PredefinedType::PT_octet:
      *os << to_from << "octet (" << lname << ")";
      break;
    default:
      *os << lname;
      break;
  }

  return 0;
}

