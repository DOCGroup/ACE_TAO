//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    field_ch.cpp
//
// = DESCRIPTION
//    Visitor generating the accessor and modifier declarations
//    for valuetype fields in the valuetype class (header).
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    derived from be_visitor_union_branch/public_ch.cpp
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

ACE_RCSID(be_visitor_valuetype, field_ch, "$Id$")


// constructor
be_visitor_valuetype_field_ch::be_visitor_valuetype_field_ch
    (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
  setenclosings ("",";");
}

// destructor
be_visitor_valuetype_field_ch::~be_visitor_valuetype_field_ch (void)
{
}

// visit the field node
int
be_visitor_valuetype_field_ch::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_union_branch - "
                         "Bad type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_union_branch - "
                         "codegen failed\n"
                         ), -1);
    }
  return 0;
}

// =visit operations on all possible data types (valuetype state member)

// visit array type
int
be_visitor_valuetype_field_ch::visit_array (be_array *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get state member
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union node
  be_type *bt;

  // check if we are visiting this via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // this is the case of an anonymous array inside a union

      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the array declaration
      ctx.state (TAO_CodeGen::TAO_ARRAY_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_array - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_array - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
      // now use this array as a "type" for the subsequent declarator
      os->indent (); // start from current indentation
      // the set method
      *os << pre_op() << "void " << ub->local_name () << " ("
          << "_" << bt->local_name () << ")"
          << post_op() << "    // set" << be_nl;
      // the get method
      *os << pre_op() << "_" << bt->local_name ()
          << "_slice * " << ub->local_name ()
          << " (void) const" << post_op() << "     // get method\n\n";
    }
  else
    {
      // now use this array as a "type" for the subsequent declarator
      os->indent (); // start from current indentation
      // the set method
      *os << pre_op() << "void " << ub->local_name () << " ("
          << bt->nested_type_name (bu) << ")" << post_op() << "    // set"
          << be_nl;
      // the get method
 /* %! is const return really in the specs ?
      *os << pre_op() << "const "
          << bt->nested_type_name (bu, "_slice *") << " " << ub->local_name ()
          << " (void) const" << post_op() << "     // get (read only) method\n\n";
 */
      // the get (read/write) method
      *os << pre_op()
          << bt->nested_type_name (bu, "_slice *") << " " << ub->local_name ()
          << " (void) const" << post_op() << "     // get method\n\n";
    }

  return 0;
}

// visit enum type
int
be_visitor_valuetype_field_ch::visit_enum (be_enum *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get state member
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union node
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_enum - "
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
      ctx.state (TAO_CodeGen::TAO_ENUM_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_enum - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_enum - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  // now use this enum as a "type" for the subsequent declarator
  os->indent (); // start from current indentation
  // the set method
  *os << pre_op() << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu) << ")" << post_op() << "    // set"
      << be_nl;
  // the get method
  *os << pre_op() << bt->nested_type_name (bu) << " " << ub->local_name ()
      << " (void) const" << post_op() << "     // get method\n\n";

  return 0;
}

// visit interface type
int
be_visitor_valuetype_field_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get state member
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_interface - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // set method
  *os << pre_op() << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "_ptr")
      << ")" << post_op() << "    // set" << be_nl;
  // get method
  *os << pre_op()
      << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name ()
      << " (void) const" << post_op() << "     // get method\n\n";
  return 0;
}

// visit interface forward type
int
be_visitor_valuetype_field_ch::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get state member
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_interface_fwd - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // set method
  *os << pre_op() << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "_ptr")
      << ")" << post_op() << "    // set" << be_nl;
  // get method
  *os << pre_op()
      << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name ()
      << " (void) const" << post_op() << "     // get method\n\n";
  return 0;
}

// visit valuetype type
int
be_visitor_valuetype_field_ch::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get state member
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_valuetype - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // set method
  *os << pre_op() << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "*")
      << ")" << post_op() << "    // set" << be_nl;
  // get method
  *os << pre_op()
      << bt->nested_type_name (bu, "*") << " " << ub->local_name ()
      << " (void) const" << post_op() << "     // get method\n\n";
  return 0;
}

// visit valuetype forward type
int
be_visitor_valuetype_field_ch::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get state member
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_valuetype_fwd - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent (); // start from current indentation
  // set method
  *os << pre_op() << "void " << ub->local_name () << " ("
      << bt->nested_type_name (bu, "*")
      << ")" << post_op() << "    // set" << be_nl;
  // get method
  *os << pre_op()
      << bt->nested_type_name (bu, "*") << " " << ub->local_name ()
      << " (void) const" << post_op() << "     // get method\n\n";
  return 0;
}

// visit predefined type
int
be_visitor_valuetype_field_ch::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get state member
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_predefined_type - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();
  os->indent (); // start from current indentation
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      // set method
      *os << pre_op() << "void " << ub->local_name () << " ("
          << bt->nested_type_name (bu, "_ptr") << ")"
          << post_op() << "    // set" << be_nl;
        // get method
      *os << pre_op()
          << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name ()
          << " (void) const" << post_op() << "     // get method\n\n";
      break;
    case AST_PredefinedType::PT_any:
      // set method
      *os << pre_op() << "void " << ub->local_name () << " ("
          << bt->nested_type_name (bu) << ")"
          << post_op() << "    // set" << be_nl;
      // get method (read-only)
      *os << pre_op() << "const " << bt->nested_type_name (bu) << " "
          << ub->local_name () << " (void) const"
          << post_op() << "     // get method\n\n";
      // get method (read/write)
      *os << pre_op() << bt->nested_type_name (bu) << " "
          << ub->local_name () << " (void)"
          << post_op() << "     // get method\n\n";
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      // set method
      *os << pre_op() << "void " << ub->local_name () << " ("
          << bt->nested_type_name (bu) << ")"
          << post_op() << "    // set" << be_nl;
      // get method
      *os << pre_op() << bt->nested_type_name (bu) << " " << ub->local_name ()
          << " (void) const" << post_op() << "     // get method\n\n";
    }
  return 0;
}

// visit sequence type
int
be_visitor_valuetype_field_ch::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get state member
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
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

      // first generate the sequence declaration
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_sequence - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_sequence - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }
  os->indent ();
  // set method
  *os << pre_op() << "void " << ub->local_name () << " (const "
      << bt->nested_type_name (bu) << " &)"
      << post_op() << "    // set" << be_nl;
  // read-only
  *os << pre_op() << "const " << bt->nested_type_name (bu) << " &"
      << ub->local_name  () << " (void) const"
      << post_op() << "     // get method (read only)" << be_nl;
  // read/write
  *os << pre_op() << bt->nested_type_name (bu) << " &" << ub->local_name ()
      << " (void)"
      << post_op() << "     // get method (read/write only)\n\n";

  return 0;
}

// visit string type
int
be_visitor_valuetype_field_ch::visit_string (be_string *)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get state member
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
                         "visit_string - "
                         "bad context information\n"
                         ), -1);
    }

  os = this->ctx_->stream ();

  os->indent ();
  // three methods to set the string value
  *os << pre_op()
      << "void " << ub->local_name () << " (char *)"
      << post_op() << "     // set" << be_nl;
  *os << pre_op()
      << "void " << ub->local_name () << " (const char *)"
      << post_op() << "     // set" << be_nl;
  *os << pre_op()
      << "void " << ub->local_name () << " (const CORBA::String_var&)"
      << post_op() << "     // set" << be_nl;
  //get method
  *os << pre_op() << "const char *" << ub->local_name ()
      << " (void) const" << post_op() << "     // get method\n\n";
  return 0;
}

// visit structure type
int
be_visitor_valuetype_field_ch::visit_structure (be_structure *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get state member
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
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

      // first generate the sequence declaration
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_structure - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_structure - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }
  os->indent ();
  // set method
  *os << pre_op() << "void " << ub->local_name () << " (const "
      << bt->nested_type_name (bu) << " &)"
      << post_op() << "    // set" << be_nl
    // read-only
      << pre_op() << "const " << bt->nested_type_name (bu) << " &"
      << ub->local_name  () << " (void) const"
      << post_op() << "     // get method (read only)" << be_nl
    // read/write
      << pre_op() << bt->nested_type_name (bu) << " &" << ub->local_name ()
      << " (void)" << post_op() << "     // get method (read/write only)\n\n";

  return 0;
}

// visit typedefed type
int
be_visitor_valuetype_field_ch::visit_typedef (be_typedef *node)
{
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();
  os->indent (); // start from current indentation level
  this->ctx_->alias (node);     // save the node for use in code generation and
                               // indicate that the union_branch of the union_branch node
                               // is a typedefed quantity

  // make a decision based on the primitive base type
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_spec_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// visit union type
int
be_visitor_valuetype_field_ch::visit_union (be_union *node)
{
  TAO_OutStream *os; // output stream
  be_decl *ub = this->ctx_->node (); // get state member
  be_decl *bu = this->ctx_->scope ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ch::"
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

      // first generate the sequence declaration
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_union - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ch::"
                             "visit_union - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }
  os->indent ();
  // set method
  *os << pre_op() << "void " << ub->local_name () << " (const "
      << bt->nested_type_name (bu) << " &)"
      << post_op() << "    // set" << be_nl
    // read-only
      << pre_op() << "const " << bt->nested_type_name (bu) << " &"
      << ub->local_name  () << " (void) const"
      << post_op() << "     // get method (read only)"
      << be_nl
    // read/write
      << pre_op() << bt->nested_type_name (bu) << " &" << ub->local_name ()
      << " (void)" << post_op() << "     // get method (read/write only)\n\n";

  return 0;
}

void
be_visitor_valuetype_field_ch::setenclosings (char *pre, char *post)
{
  pre_op_ = pre;
  post_op_ = post;
}

const char*
be_visitor_valuetype_field_ch::pre_op ()
{
  return pre_op_;
}

const char*
be_visitor_valuetype_field_ch::post_op ()
{
  return post_op_;
}
