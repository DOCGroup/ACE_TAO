//=============================================================================
/**
*  @file   be_visitor_arg_traits.cpp
*
*  $Id$
*
*  This visitor generates template specializations for argument traits classes.
*
*  @author Jeff Parsons <j.parsons@vanderbilt.edu>
*/
//=============================================================================

#include "be_visitor_arg_traits.h"
#include "be_visitor_context.h"
#include "be_root.h"
#include "be_module.h"
#include "be_interface.h"
#include "be_valuetype.h"
#include "be_interface_fwd.h"
#include "be_valuetype_fwd.h"
#include "be_component_fwd.h"
#include "be_component.h"
#include "be_home.h"
#include "be_eventtype.h"
#include "be_eventtype_fwd.h"
#include "be_operation.h"
#include "be_attribute.h"
#include "be_argument.h"
#include "be_array.h"
#include "be_enum.h"
#include "be_predefined_type.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_field.h"
#include "be_union.h"
#include "be_union_branch.h"
#include "be_typedef.h"
#include "be_helper.h"
#include "be_extern.h"
#include "utl_identifier.h"
#include "idl_defines.h"

#include <string>


ACE_RCSID (be,
           be_visitor_arg_traits,
           "$Id$")

be_visitor_arg_traits::be_visitor_arg_traits (const char *S,
                                              be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    S_ (ACE::strnew (S))
{
}

be_visitor_arg_traits::~be_visitor_arg_traits (void)
{
  delete [] this->S_;
}

int
be_visitor_arg_traits::visit_root (be_root *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl << be_nl
      << "// Arg traits specializations." << be_nl
      << "namespace TAO" << be_nl
      << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_root - visit scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "}" << be_nl;

  return 0;
}

int
be_visitor_arg_traits::visit_module (be_module *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_module - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_arg_traits::visit_interface (be_interface *node)
{
  if (this->generated (node))
    {
      return 0;
    }

  if (node->seen_in_operation ())
    {
      TAO_OutStream *os = this->ctx_->stream ();
      idl_bool stub = (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_CS);
      BE_GlobalData *b = be_global;

      std::string guard_suffix =
        std::string (this->S_) + std::string ("arg_traits");

      // This should be generated even for imported nodes. The ifdef
      // guard prevents multiple declarations.
      os->gen_ifdef_macro (node->flat_name (), guard_suffix.c_str ());

      *os << be_nl << be_nl
          << "template<>" << be_nl
          << "class "
          << (stub ? b->stub_export_macro () : b->skel_export_macro ())
          << " " << this->S_ << "Arg_Traits<"
          << node->name () << ">" << be_idt_nl
          << ": public" << be_idt << be_idt_nl
          << "Object_" << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl
          << node->name () << "_ptr," << be_nl
          << node->name () << "_var," << be_nl
          << node->name () << "_out";

      if (ACE_OS::strlen (this->S_) == 0)
        {
          *os << "," << be_nl
            << "TAO::Objref_Traits<" << node->name () << ">";
        }

      *os << be_uidt_nl
          << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
          << "{" << be_nl
          << "};";

      os->gen_endif ();
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_interface - visit scope failed\n"),
                        -1);
    }

  this->generated (node, I_TRUE);
  return 0;
}

int
be_visitor_arg_traits::visit_interface_fwd (be_interface_fwd *node)
{
  if (this->generated (node))
    {
      return 0;
    }

  be_interface *fd =
    be_interface::narrow_from_decl (node->full_definition ());

  // The logic in visit_interface() should handle what gets generated
  // and what doesn't.
  if (this->visit_interface (fd) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_interface_fwd - code generation failed\n"),
                        -1);
    }

  this->generated (node, I_TRUE);
  return 0;
}

int
be_visitor_arg_traits::visit_valuetype (be_valuetype *node)
{
  if (this->generated (node))
    {
      return 0;
    }

  if (node->seen_in_operation ())
    {
      TAO_OutStream & os = *this->ctx_->stream ();

      std::string guard_suffix =
        std::string (this->S_) + std::string ("arg_traits");

      // This should be generated even for imported nodes. The ifdef
      // guard prevents multiple declarations.
      os.gen_ifdef_macro (node->flat_name (), guard_suffix.c_str ());

      os << be_nl << be_nl
         << "template<>" << be_nl
         << "class " << be_global->stub_export_macro () << " "
         << this->S_ << "Arg_Traits<"
         << node->name () << ">" << be_idt_nl
         << ": public" << be_idt << be_idt_nl
         << "Object_" << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl
         << node->name () << " *," << be_nl
         << node->name () << "_var," << be_nl
         << node->name () << "_out";

      // The SArgument classes don't need the traits parameter (yet?)
      if (ACE_OS::strlen (this->S_) == 0)
        {
          os << "," << be_nl
             << "TAO::Value_Traits<" << node->name () << ">";
        }

      os << be_uidt_nl
         << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
         << "{" << be_nl
         << "};";

      os.gen_endif ();
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_valuetype - visit scope failed\n"),
                        -1);
    }

  this->generated (node, I_TRUE);
  return 0;
}

int
be_visitor_arg_traits::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  if (this->generated (node))
    {
      return 0;
    }

  be_valuetype *fd =
    be_valuetype::narrow_from_decl (node->full_definition ());

  // The logic in visit_valuetype() should handle what gets generated
  // and what doesn't.
  int status = this->visit_valuetype (fd);

  if (status != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_valuetype_fwd - code generation failed\n"),
                        -1);
    }

  this->generated (node, I_TRUE);
  return 0;
}

int
be_visitor_arg_traits::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_arg_traits::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_arg_traits::visit_operation (be_operation *node)
{
  if (this->generated (node) || node->is_local ())
    {
      return 0;
    }

  AST_Type *rt = node->return_type ();
  AST_Decl::NodeType nt = rt->node_type ();

  // If our return type is an unaliased bounded (w)string, we create
  // an empty struct using the operation's flat name for the type,
  // and use this type as the Arg_Traits<> template parameter. All
  // this is necessary because there could be any number of such
  // return types, all identical, in the same interface, valuetype,
  // translation unit, or build, and we need a unique type for the
  // Arg_Traits<> template parameter.
  if (nt == AST_Decl::NT_string || nt == AST_Decl::NT_wstring)
    {
      AST_String *str = AST_String::narrow_from_decl (rt);
      unsigned long bound = str->max_size ()->ev ()->u.ulval;

      if (bound > 0)
        {
          TAO_OutStream *os = this->ctx_->stream ();
          idl_bool wide = (str->width () != 1);

          *os << be_nl << be_nl;

          idl_bool const skel =
            (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_SS);

          // Avoid generating a duplicate structure in the skeleton
          // when generating Arg_Traits<> for ThruPOA and direct
          // collocation code.
          if (!skel
              || (skel && ACE_OS::strlen (this->S_) != 0))
            {
              *os << "struct " << node->flat_name () << " {};"
                  << be_nl << be_nl;
            }

          *os << "template<>" << be_nl
              << "class " << be_global->stub_export_macro () << " "
              << this->S_ << "Arg_Traits<" << node->flat_name ()
              << ">" << be_idt_nl
              << ": public" << be_idt << be_idt_nl
              << "BD_" << (wide ? "W" : "")
              << "String_" << this->S_ << "Arg_Traits<" << bound << ">"
              << be_uidt << be_uidt << be_uidt_nl
              << "{" << be_nl
              << "};";
        }
    }

  // This will catch (in visit_argument() below) any parameters that
  // are unaliased, bounded (w)strings.
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_operation - visit scope failed\n"),
                        -1);
    }

  this->generated (node, I_TRUE);
  return 0;
}

int
be_visitor_arg_traits::visit_attribute (be_attribute *node)
{
  if (this->ctx_->alias () != 0 || this->generated (node))
    {
      return 0;
    }

  AST_String *st = AST_String::narrow_from_decl (node->field_type ());

  if (st == 0)
    {
      return 0;
    }

  unsigned long bound = st->max_size ()->ev ()->u.ulval;

  if (bound == 0)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  idl_bool wide = (st->width () != 1);

  // It is legal IDL to declare a bounded (w)string as an operation
  // parameter type. There could be any number of identical
  // declarations in the same build, translation unit, or even in
  // the same operation, so we use the argument's flat name to
  // declare an empty struct, and use that struct as the template
  // parameter for Arg_Traits<>.

  *os << be_nl;

  idl_bool const skel =
    (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_SS);

  // Avoid generating a duplicate structure in the skeleton when
  // generating Arg_Traits<> for ThruPOA and direct collocation code.
  if (!skel
      || (skel && ACE_OS::strlen (this->S_) != 0))
    {
      *os << "struct " << node->flat_name () << " {};"
          << be_nl << be_nl;
    }

  *os << "template<>" << be_nl
      << "class " << be_global->stub_export_macro () << " "
      << this->S_ << "Arg_Traits<" << node->flat_name ()
      << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl
      << "BD_" << (wide ? "W" : "")
      << "String_" << this->S_ << "Arg_Traits<" << bound << ">"
      << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  this->generated (node, I_TRUE);
  return 0;
}

int
be_visitor_arg_traits::visit_argument (be_argument *node)
{
  if (this->ctx_->alias () != 0 || this->generated (node))
    {
      return 0;
    }

  AST_Type *bt = node->field_type ();
  AST_Decl::NodeType nt = bt->node_type ();

  // We are interested here only in unaliased, bounded
  // (w)strings.

  if (nt != AST_Decl::NT_string && nt != AST_Decl::NT_wstring)
    {
      return 0;
    }

  be_string *st = be_string::narrow_from_decl (bt);
  unsigned long bound = st->max_size ()->ev ()->u.ulval;

  if (bound == 0)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  idl_bool wide = (st->width () != 1);

  // It is legal IDL to declare a bounded (w)string as an operation
  // parameter type. There could be any number of identical
  // declarations in the same build, translation unit, or even in
  // the same operation, so we use the argument's flat name to
  // declare an empty struct, and use that struct as the template
  // parameter for Arg_Traits<>.
  *os << be_nl << be_nl;

  idl_bool const skel =
    (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_SS);

  // Avoid generating a duplicate structure in the skeleton when
  // generating Arg_Traits<> for ThruPOA and direct collocation code.
  if (!skel
      || (skel && ACE_OS::strlen (this->S_) != 0))
    {
      *os << "struct " << node->flat_name () << " {};"
          << be_nl << be_nl;
    }

  *os << "template<>" << be_nl
      << "class " << be_global->stub_export_macro () << " "
      << this->S_ << "Arg_Traits<" << node->flat_name ()
      << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl
      << "BD_" << (wide ? "W" : "")
      << "String_" << this->S_ << "Arg_Traits<" << bound << ">"
      << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  this->generated (node, I_TRUE);
  return 0;
}

int
be_visitor_arg_traits::visit_sequence (be_sequence *node)
{
  if (this->generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  be_typedef *alias = this->ctx_->alias ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  std::string guard_suffix =
    std::string (this->S_) + std::string ("arg_traits");

  // This should be generated even for imported nodes. The ifdef
  // guard prevents multiple declarations.
  os->gen_ifdef_macro (alias->flat_name (), guard_suffix.c_str ());

  *os << be_nl << be_nl
      << "template<>" << be_nl
      << "class " << be_global->stub_export_macro () << " "
      << this->S_ << "Arg_Traits<"
      << alias->name () << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl
      << "Var_Size_" << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl
      << alias->name () << "," << be_nl
      << alias->name () << "_var," << be_nl
      << alias->name () << "_out" << be_uidt_nl
      << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  os->gen_endif ();

  this->generated (node, I_TRUE);
  return 0;
}

int
be_visitor_arg_traits::visit_string (be_string *node)
{
  if (this->generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  unsigned long bound = node->max_size ()->ev ()->u.ulval;
  be_type *alias = this->ctx_->alias ();

  // Unbounded (w)string args are handled as a predefined type.
  // Bounded (w)strings must come in as a typedef - they can't
  // be used directly as arguments or return types.
  if (bound == 0)
    {
      return 0;
    }

  idl_bool wide = (node->width () != 1);

  TAO_OutStream *os = this->ctx_->stream ();

  std::string guard_suffix =
    std::string (this->S_) + std::string ("arg_traits");

  // This should be generated even for imported nodes. The ifdef
  // guard prevents multiple declarations.
  os->gen_ifdef_macro (node->flat_name (), guard_suffix.c_str ());


  idl_bool const skel =
    (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_SS);

  // Avoid generating a duplicate structure in the skeleton when
  // generating Arg_Traits<> for ThruPOA and direct collocation code.
  if (!skel
      || (skel && ACE_OS::strlen (this->S_) != 0))
    {
      // A workaround 'dummy' type, since bounded (w)strings are all
      // generated as typedefs of (w)char *.
      *os << be_nl << be_nl
          << "struct ";

      if (alias == 0)
        {
          *os << node->flat_name ();
        }
      else
        {
          *os << alias->local_name () << "_" << bound;
        }

      *os << " {};";
    }

  *os << be_nl << be_nl
      << "template<>" << be_nl
      << "class " << be_global->stub_export_macro () << " "
      << this->S_ << "Arg_Traits<";

  if (alias == 0)
    {
      *os << node->flat_name ();
    }
  else
    {
      *os << alias->local_name () << "_" << bound;
    }

  *os << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl
      << "BD_" << (wide ? "W" : "")
      << "String_" << this->S_ << "Arg_Traits<" << bound << ">"
      << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  os->gen_endif ();

  this->generated (node, I_TRUE);
  return 0;
}

int
be_visitor_arg_traits::visit_array (be_array *node)
{
  if (this->generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // This should be generated even for imported nodes. The ifdef guard prevents
  // multiple declarations.
//  os->gen_ifdef_macro (node->flat_name (), "arg_traits");

  // Generate the array traits specialization definitions,
  // guarded by #ifdef on unaliased array element type and length.

  ACE_CString unique (this->S_);
  unique += ACE_CString ("_");
  be_type *bt = be_type::narrow_from_decl (node->base_type ());
  AST_Decl::NodeType nt = bt->node_type ();

  if (nt == AST_Decl::NT_typedef)
    {
      be_typedef *td = be_typedef::narrow_from_decl (bt);
      unique += td->primitive_base_type ()->flat_name ();
    }
  else
    {
      unique += bt->flat_name ();
    }

  char buf[NAMEBUFSIZE];

  for (unsigned long i = 0; i < node->n_dims (); ++i)
    {
      ACE_OS::memset (buf,
                      '\0',
                      NAMEBUFSIZE);
      ACE_OS::sprintf (buf,
                       "_%ld",
                       node->dims ()[i]->ev ()->u.ulval);
      unique += buf;
    }

  unique += "_traits";
  os->gen_ifdef_macro (unique.fast_rep ());

  *os << be_nl << be_nl
      << "template<>" << be_nl
      << "class " << be_global->stub_export_macro () << " "
      << this->S_ << "Arg_Traits<"
      << node->name () << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl;

  *os << (node->size_type () == AST_Type::FIXED ? "Fixed" : "Var")
      << "_Array_" << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl
      << node->name () << "," << be_nl
      << node->name () << "_slice," << be_nl
      << node->name () << "_var," << be_nl;

  if (node->size_type () == AST_Type::VARIABLE)
    {
      *os << node->name () << "_out," << be_nl;
    }

  *os << node->name () << "_forany";

  // The SArgument classes don't need the TAG parameter,
  if (ACE_OS::strlen (this->S_) == 0)
    {
      *os << "," << be_nl
          << node->name () << "_tag";
    }

  *os << be_uidt_nl
      << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  os->gen_endif ();

  this->generated (node, I_TRUE);
  return 0;
}

int
be_visitor_arg_traits::visit_enum (be_enum *node)
{
  if (this->generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  std::string guard_suffix =
    std::string (this->S_) + std::string ("arg_traits");

  // This should be generated even for imported nodes. The ifdef
  // guard prevents multiple declarations.
  os->gen_ifdef_macro (node->flat_name (), guard_suffix.c_str ());

  *os << be_nl << be_nl
      << "template<>" << be_nl
      << "class " << be_global->stub_export_macro () << " "
      << this->S_ << "Arg_Traits<"
      << node->name () << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl;

  *os << "Basic_" << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl
      << node->name () << be_uidt_nl
      << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  os->gen_endif ();

  this->generated (node, I_TRUE);
  return 0;
}

int
be_visitor_arg_traits::visit_structure (be_structure *node)
{
  if (this->generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  // This should be generated even for imported nodes. The ifdef guard prevents
  // multiple declarations.
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  std::string guard_suffix =
    std::string (this->S_) + std::string ("arg_traits");

  // This should be generated even for imported nodes. The ifdef
  // guard prevents multiple declarations.
  os->gen_ifdef_macro (node->flat_name (), guard_suffix.c_str ());

  *os << be_nl << be_nl
      << "template<>" << be_nl
      << "class " << be_global->stub_export_macro () << " "
      << this->S_ << "Arg_Traits<"
      << node->name () << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl;

  *os << (node->size_type () == AST_Type::FIXED ? "Fixed" : "Var")
      << "_Size_" << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl
      << node->name ();

  if (node->size_type () == AST_Type::VARIABLE)
    {
      *os << "," << be_nl
          << node->name () << "_var," << be_nl
          << node->name () << "_out";
    }

  *os << be_uidt_nl
      << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  os->gen_endif ();

  /* Set this before visiting the scope so things like

      interface foo
      {
        struct bar
        {
          ....
          foo foo_member;
        };

        void op (in bar inarg);
      };

     will not cause infinite recursion in this visitor.
  */

  this->generated (node, I_TRUE);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_structure - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_arg_traits::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  // Valuetypes may not be *declared* in a field, so this will
  // get handled elsewhere, and will also avoid nested valuetype
  // recursion. So we set the field node as processed (the
  // field *type* may not have been reached yet) and return.
  if (bt->base_node_type () == AST_Decl::NT_valuetype)
    {
      node->cli_traits_gen (I_TRUE);
      return 0;
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  this->generated (node, I_TRUE);
  this->generated (bt, I_TRUE);
  return 0;
}

int
be_visitor_arg_traits::visit_union (be_union *node)
{
  if (this->generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  // This should be generated even for imported nodes. The ifdef guard prevents
  // multiple declarations.
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  std::string guard_suffix =
    std::string (this->S_) + std::string ("arg_traits");

  // This should be generated even for imported nodes. The ifdef
  // guard prevents multiple declarations.
  os->gen_ifdef_macro (node->flat_name (), guard_suffix.c_str ());

  *os << be_nl << be_nl
      << "template<>" << be_nl
      << "class " << be_global->stub_export_macro () << " "
      << this->S_ << "Arg_Traits<"
      << node->name () << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl;

  *os << (node->size_type () == AST_Type::FIXED ? "Fixed" : "Var")
      << "_Size_" << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl
      << node->name ();

  if (node->size_type () == AST_Type::VARIABLE)
    {
      *os << "," << be_nl
          << node->name () << "_var," << be_nl
          << node->name () << "_out";
    }

  *os << be_uidt_nl
      << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  os->gen_endif ();

  /* Set this before visiting the scope so things like

      interface foo
      {
        struct bar
        {
          ....
          foo foo_member;
        };

        void op (in bar inarg);
      };

     will not cause infinite recursion in this visitor.
  */

  this->generated (node, I_TRUE);

  int status = this->visit_scope (node);

  if (status != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_union - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_arg_traits::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"),
                        -1);
    }

  this->generated (node, I_TRUE);
  return 0;
}

int
be_visitor_arg_traits::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // Make a decision based on the primitive base type.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  this->generated (node, I_TRUE);
  return 0;
}

idl_bool
be_visitor_arg_traits::generated (be_decl *node) const
{
  if (ACE_OS::strcmp (this->S_, "") == 0)
    {
      switch (this->ctx_->state ())
        {
          case TAO_CodeGen::TAO_ROOT_CS:
            return node->cli_arg_traits_gen ();
          case TAO_CodeGen::TAO_ROOT_SS:
            return node->srv_arg_traits_gen ();
          default:
            return 0;
        }
    }

  return node->srv_sarg_traits_gen ();
}

void
be_visitor_arg_traits::generated (be_decl *node,
                                  idl_bool )
{
  if (ACE_OS::strcmp (this->S_, "") == 0)
    {
      switch (this->ctx_->state ())
        {
          case TAO_CodeGen::TAO_ROOT_CS:
            node->cli_arg_traits_gen (I_TRUE);
            return;
          case TAO_CodeGen::TAO_ROOT_SS:
            node->srv_arg_traits_gen (I_TRUE);
            return;
          default:
            return;
        }
    }

  node->srv_sarg_traits_gen (I_TRUE);
}

int
be_visitor_arg_traits::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_arg_traits::visit_component_fwd (be_component_fwd *node)
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_arg_traits::visit_home (be_home *node)
{
  return this->visit_interface (node);
}
