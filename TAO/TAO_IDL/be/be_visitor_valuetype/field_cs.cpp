//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    field_cs.cpp
//
// = DESCRIPTION
//     Visitor for the Valuetype class.
//     This one generates code for accessor and modifier functions of
//     valuetype state members (in the stub file).
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    derived from be_visitor_union_branch/public_ci.cpp
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype, field_cs, "$Id$")


// constructor
be_visitor_valuetype_field_cs::
be_visitor_valuetype_field_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    in_obv_space_ (0)
{
  setenclosings ("");
}

// destructor
be_visitor_valuetype_field_cs::
~be_visitor_valuetype_field_cs (void)
{
}

// visit the field node
int
be_visitor_valuetype_field_cs::
visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cs::"
                         "visit_field - "
                         "Bad field type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cs::"
                         "visit_field - "
                         "codegen for field type failed\n"
                         ), -1);
    }
  return 0;
}

// =visit operations on all possible data types that a field can be

int
be_visitor_valuetype_field_cs::visit_array (be_array *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();
  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the valuetype
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the inline operations for this anonymous array type
      ctx.state (TAO_CodeGen::TAO_ARRAY_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cs::"
                             "visit_array - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cs::"
                             "visit_array - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  // for anonymous arrays, the type name has a _ prepended. We compute the
  // full_name with or without the underscore and use it later on.
  char fname [NAMEBUFSIZE];  // to hold the full and

  // save the node's local name and full name in a buffer for quick use later
  // on
  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // for anonymous arrays ...
      // we have to generate a name for us that has an underscope prepended to
      // our local name. This needs to be inserted after the parents's name

      if (bt->is_nested ())
        {
          be_decl *parent =
                 be_scope::narrow_from_scope (bt->defined_in ())->decl ();
          ACE_OS::sprintf (fname, "%s::_%s", parent->full_name (),
                           bt->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (fname, "_%s", bt->full_name ());
        }
    }
  else
    {
      // typedefed node
      ACE_OS::sprintf (fname, "%s", bt->full_name ());
    }

  // set method
  os->indent (); // start from current indentation
  *os << "// accessor to set the member" << be_nl
      << this->pre_op() << "void" << be_nl;
  this->op_name(bu,os); *os << "::" << ub->local_name () << " (" << fname
      << " val)// set" << be_nl
      << "{" << be_idt_nl;

  *os << fname << "_copy ("
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
      << ", val);" << be_uidt_nl;

  *os << "}" << be_nl;

  // get method
  *os << "// retrieve the member" << be_nl
      << this->pre_op() << "const " << fname << "_slice *" << be_nl;
  this->op_name(bu,os);
  *os << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl;
  *os << "return this->"
      << bu->field_pd_prefix() << ub->local_name ()
      << bu->field_pd_postfix() << ";" << be_uidt_nl
      << "}\n" << be_nl;

  // get method
  *os << "// retrieve the member" << be_nl
      << this->pre_op() << fname << "_slice *" << be_nl;
      this->op_name(bu,os);
      *os << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl;
      *os << "return this->"
          << bu->field_pd_prefix() << ub->local_name ()
          << bu->field_pd_postfix() << ";" << be_uidt_nl;
  *os << "}\n\n";
  return 0;
}

int
be_visitor_valuetype_field_cs::visit_enum (be_enum *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_enum - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  // set method
  os->indent (); // start from current indentation
  *os << "// accessor to set the member" << be_nl
      << this->pre_op() << "void" << be_nl;
      this->op_name(bu,os);
      *os << "::" << ub->local_name () << " (" << bt->name ()
      << " val)// set" << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
      << " = val;\n";
  os->decr_indent ();
  *os << "}" << be_nl;

  // get method
  *os << "// retrieve the member" << be_nl
      << this->pre_op() << bt->name () << be_nl;
      this->op_name(bu,os);
      *os << "::" << ub->local_name () << " () const"
      << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "return this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
      << ";\n";
  os->decr_indent ();
  *os << "}\n\n";
  return 0;
}

int
be_visitor_valuetype_field_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  // set method
  os->indent (); // start from current indentation
  *os << "// accessor to set the member" << be_nl
      << this->pre_op() << "void" << be_nl;
      this->op_name(bu,os);
      *os << "::" << ub->local_name () << " (" << bt->name ()
      << "_ptr val)// set" << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
      << " = " << bt->name () << "::_duplicate (val);" << be_uidt_nl;
  *os << "}" << be_nl;

  // get method
  *os << "// retrieve the member" << be_nl
      << this->pre_op() << bt->name () << "_ptr " << be_nl;
      this->op_name(bu,os);
      *os << "::" << ub->local_name () << " () const"
      << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "return this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
      << ".ptr ();\n";
  os->decr_indent ();
  *os << "}\n\n";
  return 0;
}

int
be_visitor_valuetype_field_cs::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  // set method
  os->indent (); // start from current indentation
  *os << "// accessor to set the member" << be_nl
      << this->pre_op() << "void" << be_nl;
      this->op_name(bu,os);
      *os << "::" << ub->local_name () << " (" << bt->name ()
      << "_ptr val)// set" << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
      << " = " << bt->name () << "::_duplicate (val);" << be_uidt_nl;
  *os << "}" << be_nl;

  // get method
  *os << "// retrieve the member" << be_nl
      << this->pre_op() << bt->name () << "_ptr " << be_nl;
      this->op_name(bu,os);
      *os << "::" << ub->local_name () << " () const"
      << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "return this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
      << ".ptr ();\n";
  os->decr_indent ();
  *os << "}\n\n";
  return 0;
}

int
be_visitor_valuetype_field_cs::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  // set method
  os->indent (); // start from current indentation
  *os << "// accessor to set the member" << be_nl
      << this->pre_op() << "void" << be_nl;
      this->op_name(bu,os);
      *os << "::" << ub->local_name () << " (" << bt->name ()
      << "* val)// set" << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "CORBA::add_ref (val);" << be_nl
      << "this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
      << " = val;" << be_uidt_nl;
  *os << "}" << be_nl;

  // get method
  *os << "// retrieve the member" << be_nl
      << this->pre_op() << bt->name () << "* " << be_nl;
      this->op_name(bu,os);
      *os << "::" << ub->local_name () << " () const"
      << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "return this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
      << ".ptr ();\n";
  os->decr_indent ();
  *os << "}\n\n";
  return 0;
}

int
be_visitor_valuetype_field_cs::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  // set method
  os->indent (); // start from current indentation
  *os << "// accessor to set the member" << be_nl
      << this->pre_op () << "void" << be_nl;
      this->op_name (bu, os);
      *os << "::" << ub->local_name () << " (" << bt->name ()
      << "* val)// set" << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "CORBA::add_ref (val);" << be_nl
      << "this->"
      << bu->field_pd_prefix () << ub->local_name () << bu->field_pd_postfix()
      << " = val;" << be_uidt_nl;
  *os << "}" << be_nl;

  // get method
  *os << "// retrieve the member" << be_nl
      << this->pre_op () << bt->name () << "* " << be_nl;
      this->op_name (bu, os);
      *os << "::" << ub->local_name () << " () const"
      << be_nl
      << "{\n";
  os->incr_indent ();
  *os << "return this->"
      << bu->field_pd_prefix () << ub->local_name () << bu->field_pd_postfix()
      << ".ptr ();\n";
  os->decr_indent ();
  *os << "}\n\n";
  return 0;
}

int
be_visitor_valuetype_field_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os; // output stream

  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());
  be_type *bt;
  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_predef... - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  // set method
  os->indent (); // start from current indentation
  *os << "// accessor to set the member" << be_nl
      << this->pre_op() << "void" << be_nl;
  this->op_name(bu,os);
  *os << "::" << ub->local_name ()
      << " (" << bt->name ();
  if (node->pt () == AST_PredefinedType::PT_pseudo)
    *os << "_ptr";
  *os << " val) // set" << be_nl
      << "{" << be_idt_nl;

      switch (node->pt ())
        {
        case AST_PredefinedType::PT_pseudo:
          *os << "this->" << bu->field_pd_prefix() << ub->local_name () 
              << bu->field_pd_postfix() << " = "
              << bt->name () << "::_duplicate (val);" << be_uidt_nl;
          break;

        case AST_PredefinedType::PT_any:
          *os << "ACE_NEW (" << be_idt << be_idt_nl
              << "this->" << bu->field_pd_prefix() << ub->local_name ()
              << bu->field_pd_postfix() << "," << be_nl
              << bt->name () << " (val)" << be_uidt_nl
              << ");" << be_uidt << be_uidt_nl;
          break;

        case AST_PredefinedType::PT_void:
          break;

        default:
          *os << "// set the value" << be_nl
              << "this->" << bu->field_pd_prefix () << ub->local_name () 
              << bu->field_pd_postfix ()
              << " = val;" << be_uidt_nl;
        }
  *os << "}" << be_nl;

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      // get method
      *os << "// retrieve the member" << be_nl
          << this->pre_op() << bt->name () << "_ptr" << be_nl;
      this->op_name(bu,os);
      *os << "::" << ub->local_name ()
          << " (void) const" << be_nl
          << "{" << be_idt_nl
          << "return this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
          << ";" << be_uidt_nl
          << "}\n\n";
      break;
    case AST_PredefinedType::PT_any:
      // get method with read-only access
      *os << "// retrieve the member" << be_nl
          << this->pre_op() << "const " << bt->name () << be_nl;
      this->op_name(bu,os);
      *os << "::" << ub->local_name ()
          << " (void) const" << be_nl
          << "{" << be_idt_nl
          << "return *this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
          << ";" << be_uidt_nl
          << "}" << be_nl;

      // get method with read/write access
      *os << "// retrieve the member" << be_nl
          << this->pre_op() << bt->name () << be_nl;
      this->op_name(bu,os);
      *os << "::" << ub->local_name ()
          << " (void)" << be_nl
          << "{" << be_idt_nl
          << "return *this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
          << ";" << be_uidt_nl
          << "}\n\n";
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      // get method
      *os << "// retrieve the member" << be_nl
          << this->pre_op() << bt->name () << be_nl;
      this->op_name(bu,os);
      *os << "::" << ub->local_name ()
          << " (void) const" << be_nl
          << "{" << be_idt_nl
          << "return this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
          << ";" << be_uidt_nl
          << "}\n\n";
    }
  return 0;
}

int
be_visitor_valuetype_field_cs::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_sequence - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the inline operations for this anonymous sequence type
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cs::"
                             "visit_sequence - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cs::"
                             "visit_sequence - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  // (1) set from a const
  *os << "// accessor to set the member" << be_nl
      << this->pre_op() << "void" << be_nl;
  this->op_name(bu,os);
  *os << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl;

  *os << "this->" 
      << bu->field_pd_prefix() << ub->local_name () 
                               << bu->field_pd_postfix() 
      << " = val;" << be_uidt_nl;

  *os << "}" << be_nl << be_nl;

  // readonly get method
  *os << "// readonly get method " << be_nl
      << this->pre_op() << "const " << bt->name () << " &" << be_nl;
  this->op_name(bu,os);
  *os << "::" << ub->local_name ()
      << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->"   // %! *this (seq_var, not seq member)
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
      << ";" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // read/write get method
  *os << "// read/write get method " << be_nl
      << this->pre_op() << bt->name () << " &" << be_nl;
  this->op_name(bu,os);
  *os << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
      << ";" << be_uidt_nl
      << "}\n\n";

  return 0;
}

int
be_visitor_valuetype_field_cs::visit_string (be_string *node)
{
  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_string - "
                         "bad context information\n"
                         ), -1);
    }
  TAO_OutStream *os = this->ctx_->stream ();

  // three methods to set the string value

  // (1) set method from char* or wchar*
  os->indent (); // start from current indentation
  *os << "// accessor to set the member" << be_nl
      << this->pre_op () << "void" << be_nl;
  this->op_name (bu, 
                 os);

  if (node->width () == (long) sizeof (char))
    {
      *os << "::" << ub->local_name () << " (char *val)";
    }
  else
    {
      *os << "::" << ub->local_name () << " (CORBA::WChar *val)";
    }

  *os << be_nl
      << "{" << be_idt_nl;

      *os << "// set the value" << be_nl
          << "this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
          << " = val;" << be_uidt_nl
      << "}" << be_nl;

  // (2) set method from const char * or const wchar*
  *os << "// accessor to set the member" << be_nl
      << this->pre_op () << "void" << be_nl;
  this->op_name (bu,
                 os);
  *os << "::" << ub->local_name ();

  if (node->width () == (long) sizeof (char))
    {
      *os << " (const char *val)" << be_nl;
    }
  else
    {
      *os << " (const CORBA::WChar *val)" << be_nl;
    }

  *os << "{\n";
  os->incr_indent ();

  *os << "// set the value" << be_nl
      << "this->"
      << bu->field_pd_prefix () << ub->local_name () << bu->field_pd_postfix ()
      << " = ";

  if (node->width () == (long) sizeof (char))
    {
      *os << "CORBA::string_dup (val);" << be_uidt_nl;
    }
  else
    {
      *os << "CORBA::wstring_dup (val);" << be_uidt_nl;
    }

  *os << "}" << be_nl;

  // (3) set from const String_var&
  *os << "// accessor to set the member" << be_nl
      << this->pre_op () << "void" << be_nl;
  this->op_name (bu,
                 os);
  *os << "::" << ub->local_name ();

  if (node->width () == (long) sizeof (char))
    {
      *os << " (const CORBA::String_var &val)" << be_nl;
    }
  else
    {
      *os << " (const CORBA::WString_var &val)" << be_nl;
    }

  *os << "{" << be_idt_nl;
  *os << ";" << be_nl;

  *os << "// set the value" << be_nl;

  if (node->width () == (long) sizeof (char))
    {
      *os << "CORBA::String_var " << ub->local_name ();
    }
  else
    {
      *os << "CORBA::WString_var " << ub->local_name ();
    }

  *os << "_var = val;" << be_nl
      << "this->"
      << bu->field_pd_prefix () << ub->local_name () << bu->field_pd_postfix ()
      << " = "
      << ub->local_name () << "_var._retn ();" << be_uidt_nl;

  *os << "}" << be_nl;

  // get method
  if (node->width () == (long) sizeof (char))
    {
      *os << this->pre_op () << "const char *" << be_nl;
    }
  else
    {
      *os << this->pre_op () << "const CORBA::WChar *" << be_nl;
    }

  this->op_name (bu,
                 os);
  *os << "::" << ub->local_name ()
      << " (void) const // get method" << be_nl
      << "{" << be_idt_nl
      << "return this->"
      << bu->field_pd_prefix () << ub->local_name () << bu->field_pd_postfix ()
      << ";" << be_uidt_nl
      << "}\n\n";
  return 0;
}

int
be_visitor_valuetype_field_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_structure - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the enum declaration
      ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cs::"
                             "visit_structure - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cs::"
                             "visit_structure - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  // (1) set from a const
  *os << "// accessor to set the member" << be_nl
      << this->pre_op() << "void" << be_nl;
  this->op_name(bu,os);
  *os << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl;

  if (0) // %! (bt->size_type () == be_type::VARIABLE)
    { cerr <<"!t VARIABLE struct in field_cs\n";
      *os << "delete this->"
          << bu->field_pd_prefix() << ub->local_name () 
          << bu->field_pd_postfix()
          << ";" << be_nl;

      *os << "ACE_NEW (" << be_idt << be_idt_nl
          << "this->" << bu->field_pd_prefix() << ub->local_name ()
          << bu->field_pd_postfix() << "," << be_nl
          << bt->name () << " (val)" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl;

// This was replaced by the above output statement, but this doesn't work
//      *os << "this->"
//     << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
//          << " = new "
//          << bt->name () << " (val);" << be_uidt_nl;
    }
  else
    {
      *os << "this->"
     << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
          << " = val;" << be_uidt_nl;
    }

  *os << "}" << be_nl;

  // readonly get method
  *os << "// readonly get method " << be_nl
      << this->pre_op() << "const " << bt->name () << " &" << be_nl;
  this->op_name(bu,os);
  *os << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl;
  if (0) // %! (bt->size_type () == be_type::VARIABLE)
    *os << "return *this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
        << ";" << be_uidt_nl;
  else
    *os << "return this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
        << ";" << be_uidt_nl;
  *os << "}" << be_nl;

  // read/write get method
  *os << "// read/write get method " << be_nl
      << this->pre_op() << bt->name () << " &" << be_nl;
  this->op_name(bu,os);
  *os << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl;
  if (0) // %! (bt->size_type () == be_type::VARIABLE)
    *os << "return *this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
        << ";" << be_uidt_nl;
  else
    *os << "return this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
        << ";" << be_uidt_nl;
  *os << "}\n\n";

  return 0;
}

int
be_visitor_valuetype_field_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // save the typedef node for use in code generation
                           // as we visit the base type

  // the node to be visited in the base primitve type that gets typedefed
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_valuetype_field_cs::visit_union (be_union *node)
{
  cerr << "!u be_visitor_valuetype_field_cs::visit_union unimp.\n";
  TAO_OutStream *os; // output stream
  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cs::"
                         "visit_union - "
                         "bad context information\n"
                         ), -1);
    }
  os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the enum declaration
      ctx.state (TAO_CodeGen::TAO_UNION_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cs::"
                             "visit_union - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cs::"
                             "visit_union - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  // (1) set from a const
  *os << "// accessor to set the member" << be_nl
      << this->pre_op() << "void" << be_nl;
  this->op_name(bu,os);
  *os << "::" << ub->local_name ()
      << " (const " << bt->name () << " &val)" << be_nl
      << "{" << be_idt_nl;

  *os << "ACE_NEW (" << be_idt << be_idt_nl
      << "this->" << bu->field_pd_prefix() << ub->local_name ()
      << bu->field_pd_postfix() << "_var," << be_nl
      << bt->name () << " (val)" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl;

// This was replaced by the above output statement, but this doesn't work
//      *os << "this->"
//     << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
//          << "_var = new " << bt->name ()
//          << " (val);" << be_uidt_nl;

  *os << "}" << be_nl;

  // readonly get method
  *os << "// readonly get method " << be_nl
      << this->pre_op() << "const " << bt->name () << " &" << be_nl;
  this->op_name(bu,os);
  *os << "::" << ub->local_name () << " (void) const" << be_nl
      << "{" << be_idt_nl
      << "return *this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
      << ";" << be_uidt_nl
      << "}" << be_nl;

  // read/write get method
  *os << "// read/write get method " << be_nl
      << this->pre_op() << bt->name () << " &" << be_nl;
  this->op_name(bu,os);
  *os << "::" << ub->local_name () << " (void)" << be_nl
      << "{" << be_idt_nl
      << "return *this->"
      << bu->field_pd_prefix() << ub->local_name () << bu->field_pd_postfix()
      << ";" << be_uidt_nl
      << "}\n\n";

  return 0;
}

void
be_visitor_valuetype_field_cs::setenclosings (const char *pre)
{
  pre_op_ = pre;
}

const char*
be_visitor_valuetype_field_cs::pre_op ()
{
  return pre_op_;
}

// retrieve the fully scoped skeleton name
void
be_visitor_valuetype_field_cs::op_name (be_valuetype *node,
                                        TAO_OutStream *os)
{
  if (this->in_obv_space_)
    *os << node->full_obv_skel_name ();
  else
    *os << node->name ();
}
