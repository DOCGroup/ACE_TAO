// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_interface.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Interface
//
// = AUTHOR
//    Aniruddha Gokhale and Carlos O'Ryan
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

#include "be_visitor_interface.h"

// ******************************************************
// Generic Interface visitor
// ******************************************************

be_visitor_interface::be_visitor_interface (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_interface::~be_visitor_interface (void)
{
}

// this method must be overridden by the derived interface visitors
int
be_visitor_interface::visit_interface (be_interface *node)
{
  return -1;
}

// =all common visit methods for interface visitor

// visit an attribute
int
be_visitor_interface::visit_attribute (be_attribute *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_SS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_ATTRIBUTE_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CI:
    case TAO_CodeGen::TAO_INTERFACE_SI:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
                           "visit_attribute - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_attribute - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_attribute - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit a constant
int
be_visitor_interface::visit_constant (be_constant *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_CONSTANT_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_CONSTANT_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SS:
    case TAO_CodeGen::TAO_INTERFACE_CI:
    case TAO_CodeGen::TAO_INTERFACE_SH:
    case TAO_CodeGen::TAO_INTERFACE_SI:
    case TAO_CodeGen::TAO_INTERFACE_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
                           "visit_constant - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_constant - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_constant - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

//visit an enum
int
be_visitor_interface::visit_enum (be_enum *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CI:
    case TAO_CodeGen::TAO_INTERFACE_SH:
    case TAO_CodeGen::TAO_INTERFACE_SI:
    case TAO_CodeGen::TAO_INTERFACE_SS:
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
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
                         "(%N:%l) be_visitor_interface::"
                         "visit_enum - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_enum - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an exception
int
be_visitor_interface::visit_exception (be_exception *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CI:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SH:
    case TAO_CodeGen::TAO_INTERFACE_SI:
    case TAO_CodeGen::TAO_INTERFACE_SS:
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
                           "visit_exception - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_exception - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_exception - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an operation
int
be_visitor_interface::visit_operation (be_operation *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
      //      return node->gen_client_header ();
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
      //      return node->gen_client_stubs ();
      break;
    case TAO_CodeGen::TAO_INTERFACE_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SH);
      //      return node->gen_server_header ();
      break;
    case TAO_CodeGen::TAO_INTERFACE_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SS);
      //      return node->gen_server_skeletons ();
      break;
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CI:
    case TAO_CodeGen::TAO_INTERFACE_SI:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
                           "visit_operation - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
    }

  // grab the appropriate visitor
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_operation - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // visit the node using this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_operation - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an structure
int
be_visitor_interface::visit_structure (be_structure *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CI:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SH:
    case TAO_CodeGen::TAO_INTERFACE_SI:
    case TAO_CodeGen::TAO_INTERFACE_SS:
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
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
                         "(%N:%l) be_visitor_interface::"
                         "visit_structure - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_structure - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit a union
int
be_visitor_interface::visit_union (be_union *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CI:
      ctx.state (TAO_CodeGen::TAO_UNION_CI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SH:
    case TAO_CodeGen::TAO_INTERFACE_SI:
    case TAO_CodeGen::TAO_INTERFACE_SS:
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
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
                         "(%N:%l) be_visitor_interface::"
                         "visit_union - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_union - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit a typedef
int
be_visitor_interface::visit_typedef (be_typedef *node)
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
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CH);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CI:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CI);
      break;
    case TAO_CodeGen::TAO_INTERFACE_CS:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CS);
      break;
    case TAO_CodeGen::TAO_INTERFACE_SH:
    case TAO_CodeGen::TAO_INTERFACE_SI:
    case TAO_CodeGen::TAO_INTERFACE_SS:
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SH:
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
                           "visit_typedef - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_typedef - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_typedef - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// ******************************************************
// Interface visitor for client header
// ******************************************************

be_visitor_interface_ch::be_visitor_interface_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ch::~be_visitor_interface_ch (void)
{
}

int
be_visitor_interface_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream
  long i;            // loop index

  if (!node->cli_hdr_gen () && !node->imported ()) // not already generated and
                                                   // not imported
    {

      os = this->ctx_->stream ();

      // == STEP 1:  generate the class name and class names we inherit ==

      // generate the ifdefined macro for  the _ptr type
      os->gen_ifdef_macro (node->flatname (), "_ptr");

      // the following two are required to be under the ifdef macro to avoid
      // multiple declarations

      os->indent (); // start with whatever indentation level we are at
      // forward declaration
      *os << "class " << node->local_name () << ";" << be_nl;
      // generate the _ptr declaration
      *os << "typedef " << node->local_name () << " *" << node->local_name ()
          << "_ptr;" << be_nl;

      os->gen_endif ();

      // generate the ifdefined macro for the var type
      os->gen_ifdef_macro (node->flatname (), "_var");

      // generate the _var declaration
      if (node->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ch::"
                             "visit_interface - "
                             "codegen for _var failed\n"), -1);
        }
      os->gen_endif ();

      // generate the ifdef macro for the _out class
      os->gen_ifdef_macro (node->flatname (), "_out");

      // generate the _out declaration - ORBOS/97-05-15 pg 16-20 spec
      if (node->gen_out_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ch::"
                             "visit_interface - "
                             "codegen for _out failed\n"), -1);
        }
      // generate the endif macro
      os->gen_endif ();

      // now the interface definition itself
      os->gen_ifdef_macro (node->flatname ());

      // now generate the class definition
      os->indent ();
      *os << "class " << idl_global->export_macro ()
	  << " " << node->local_name ();

      if (node->n_inherits () > 0)  // node interface inherits from other
                                    // interfaces
        {
          *os << ": ";
          for (i = 0; i < node->n_inherits (); i++)
            {
              be_interface *inherited =
                be_interface::narrow_from_decl (node->inherits ()[i]);
              be_decl *scope = 0;
              if (inherited->is_nested ())
                {
                  // inherited node is used in the scope of "node" node
                  scope = be_scope::narrow_from_scope (node->defined_in ())
                    ->decl ();
                }

              *os << "public virtual ";
              *os << inherited->nested_type_name (scope);  // dump the scoped name
              if (i < node->n_inherits () - 1) // node is the case of multiple
                                               // inheritance, so put a comma
                {
                  *os << ", ";
                }
            }  // end of for loop
          *os << be_nl;
        }
      else
        {
          // we do not inherit from anybody, hence we do so from the base
          // CORBA::Object class
          *os << " : public virtual CORBA::Object" << be_nl;
        }

      // generate the body

      *os << "{" << be_nl
          << "public:" << be_idt_nl
        // generate the static _duplicate, _narrow, and _nil operations
          << "// the static operations" << be_nl
          << "static " << node->local_name () << "_ptr " << "_duplicate ("
          << node->local_name () << "_ptr obj);" << be_nl
          << "static " << node->local_name () << "_ptr " << "_narrow ("
          << "CORBA::Object_ptr obj, CORBA::Environment &env);" << be_nl
          << "static " << node->local_name () << "_ptr " << "_nil (void);\n\n";

      // generate code for the interface definition by traversing thru the
      // elements of its scope. We depend on the front-end to have made sure
      // that only legal syntactic elements appear in our scope.
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ch::"
                             "visit_interface - "
                             "codegen for scope failed\n"), -1);
        }
      // the _is_a method
      os->indent ();
      *os << "virtual CORBA::Boolean _is_a ("
	  << "const CORBA::Char *type_id, " << be_idt << be_idt_nl
          << "CORBA::Environment &env" << be_uidt_nl
	  << ");" << be_uidt_nl
	  << "virtual const char* "
	  << "_interface_repository_id (void) const;" << be_uidt_nl;

      // generate the "protected" constructor so that users cannot instantiate
      // us
      *os << "protected:" << be_idt_nl
	  << node->local_name () << " (void); // default constructor" << be_nl
          << node->local_name ()
	  << " (STUB_Object *objref, " << be_idt << be_idt_nl
          << "TAO_ServantBase *_tao_servant = 0, " << be_nl
          << "CORBA::Boolean _tao_collocated = 0" << be_uidt_nl
	  << ");" << be_uidt_nl
          << "virtual ~" << node->local_name () << " (void);" << be_uidt_nl;

      // private copy constructor and assignment operator. These are not
      // allowed, hence they are private.
      *os << "private:\n";
      os->incr_indent ();
      *os << node->local_name () << " (const " << node->local_name () << " &);"
          << be_nl
          << "void operator= (const " << node->local_name () << " &);\n";
      os->decr_indent ();
      *os << "};\n\n";
      os->gen_endif ();


      // generate the Any <<= and >>= operators
      os->indent ();
      if (node->is_nested ())
        *os << "friend ";
      *os << "void operator<<= (CORBA::Any &, " << node->local_name ()
          << "_ptr); // copying version" << be_nl;
      if (node->is_nested ())
        *os << "friend ";
      *os << "void operator<<= (CORBA::Any &, " << node->local_name ()
          << "_ptr *); // noncopying version" << be_nl;
      if (node->is_nested ())
        *os << "friend ";
      *os << "CORBA::Boolean operator>>= (const CORBA::Any &, "
          << node->local_name () << "_ptr &);\n";

      // generate the typecode decl. If we are in the outermost scope, our typecode
      // decl is extern
      if (node->is_nested ())
        {
          // we have a scoped name
          os->indent ();
          *os << "static CORBA::TypeCode_ptr "
	      << node->tc_name ()->last_component () << ";\n\n";
        }
      else
        {
          // we are in the ROOT scope
          os->indent ();
          *os << "extern CORBA::TypeCode_ptr "
	      << node->tc_name ()->last_component () << ";\n\n";
        }

      node->cli_hdr_gen (I_TRUE);
    } // if !cli_hdr_gen
  return 0;
}

// **************************************************
// Interface visitor for client inline
// **************************************************
be_visitor_interface_ci::be_visitor_interface_ci (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ci::~be_visitor_interface_ci (void)
{
}

int
be_visitor_interface_ci::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream

  if (node->cli_inline_gen () || node->imported ())
    return 0;

  os = this->ctx_->stream ();

  os->indent (); // start from the current indentation level

  // generate the constructors and destructor
  *os << "ACE_INLINE" << be_nl;
  *os << node->name () << "::" << node->local_name () <<
    " (void) // default constructor" << be_nl;
  *os << "{}" << be_nl << be_nl;

  *os << "ACE_INLINE" << be_nl;
  *os << node->name () << "::" << node->local_name () <<
    " (STUB_Object *objref, TAO_ServantBase *_tao_servant, "
      << "CORBA::Boolean _tao_collocated) // constructor" << be_nl;
  *os << "  : CORBA_Object (objref, _tao_servant, _tao_collocated)" << be_nl;
  *os << "{}" << be_nl << be_nl;

  *os << "ACE_INLINE" << be_nl;
  *os << node->name () << "::~" << node->local_name () <<
    " (void) // destructor" << be_nl;
  *os << "{}\n\n";

  // generate the ifdefined macro for  the _var type
  os->gen_ifdef_macro (node->flatname (), "_var");
  if (node->gen_var_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for _var failed\n"), -1);
    }
  os->gen_endif ();

  // generate the ifdefined macro for  the _out type
  os->gen_ifdef_macro (node->flatname (), "_out");
  if (node->gen_out_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for _out failed\n"), -1);
    }
  os->gen_endif ();

  // generate inline methods for elements of our scope
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  return 0;
}

// ************************************************************
// Interface visitor for client stubs
// ************************************************************

be_visitor_interface_cs::be_visitor_interface_cs (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_cs::~be_visitor_interface_cs (void)
{
}

int
be_visitor_interface_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream

  if (node->cli_stub_gen () || node->imported ())
    return 0;

  os = this->ctx_->stream ();

  os->indent (); // start with whatever indentation level we are at

  // first generate the code for the static methods

  // The _duplicate method
  *os << node->name () << "_ptr " << node->name () << "::_duplicate ("
      << node->name () << "_ptr obj)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "if (!CORBA::is_nil (obj))\n";
  os->incr_indent ();
  *os << "obj->AddRef ();\n";
  os->decr_indent ();
  *os << be_nl;
  *os << "return obj;\n";
  os->decr_indent ();
  *os << "} // end of _duplicate" << be_nl << be_nl;

  // The _narrow method
  *os << node->name () << "_ptr " << node->name ()
      << "::_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj," << be_nl
      << "CORBA::Environment &env" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (CORBA::is_nil (obj))" << be_idt_nl
      << "return " << node->name () << "::_nil ();" << be_uidt_nl
      << "if (!obj->_is_a (\"" << node->repoID () << "\", env))"
      << be_idt_nl
      << "return " << node->name () << "::_nil ();" << be_uidt_nl;

  *os << "if (!obj->_is_collocated ()" << be_idt << be_idt << be_idt_nl
      << " || !obj->_servant()" << be_nl
      << " || obj->_servant()->_downcast (\""
      << node->repoID () << "\") == 0" << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl
      << "{" << be_idt_nl;
  *os << node->name () << "_ptr new_obj = new "
      << node->name () << "(obj->_get_parent ());" << be_nl
      << "return new_obj;" << be_uidt_nl
      << "} // end of if" << be_nl;

  *os << "STUB_Object *stub = obj->_servant ()->_create_stub (env);" << be_nl
      << "if (env.exception () != 0)" << be_idt_nl
      << "return " << node->name () << "::_nil ();" << be_uidt_nl
      << "void* servant = obj->_servant ()->_downcast (\""
      << node->repoID () << "\");" << be_nl
      << "return new ";

  // This may be necessary to work around a GCC compiler bug!
  const char *skel_name = node->full_skel_name ();
  const char *coll_name = node->full_coll_name ();
  assert (coll_name != 0);

  *os << coll_name << "(" << be_idt << be_idt_nl
      << "ACE_reinterpret_cast(" << skel_name
      << "_ptr, servant)," << be_nl
      << "stub" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

  // _nil method
  *os << node->name () << "_ptr " << node->name () << "::_nil (void)" <<
    be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return (" << node->name () << "_ptr)NULL;\n";
  os->decr_indent ();
  *os << "} // end of _nil" << be_nl << be_nl;

  // generate code for the elements of the interface
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  // generate the is_a method
  os->indent ();
  *os << "CORBA::Boolean " << node->name () << "::_is_a (" <<
    "const CORBA::Char *value, CORBA::Environment &env)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "if (\n";
  os->incr_indent (0);
  if (node->traverse_inheritance_graph (be_interface::is_a_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "_is_a method codegen failed\n"), -1);
    }

  os->indent ();
  *os << "(!ACE_OS::strcmp ((char *)value, CORBA::_tc_Object->id (env))))\n";
  *os << "  return 1; // success using local knowledge\n";
  os->decr_indent ();
  *os << "else" << be_nl;
  *os << "  return this->CORBA_Object::_is_a (value, env); // remote call\n";
  os->decr_indent ();
  *os << "}\n\n";

  os->indent ();
  *os << "const char* " << node->name ()
      << "::_interface_repository_id (void) const"
      << be_nl
      << "{" << be_idt_nl
      << "return \"" << node->repoID () << "\";" << be_uidt_nl
      << "}\n\n";

  // Any <<= and >>= operators
  os->indent ();
  *os << "void operator<<= (CORBA::Any &_tao_any, "
      << node->name () << "_ptr _tao_elem) // copying" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Environment _tao_env;" << be_nl
      << "_tao_any.replace (" << node->tc_name () << ", &"
      << "_tao_elem, 1, _tao_env);" << be_uidt_nl
      << "}" << be_nl;

  *os << "void operator<<= (CORBA::Any &_tao_any, "
      << node->name () << "_ptr *_tao_elem) // non copying" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Environment _tao_env;" << be_nl
      << "_tao_any.replace (" << node->tc_name () << ", "
      << "_tao_elem, 0, _tao_env);" << be_uidt_nl
      << "}" << be_nl;

  *os << "CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, "
      << node->name () << "_ptr *&_tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Environment _tao_env;" << be_nl
      << "*_tao_elem = " << node->name () << "::_nil ();" << be_nl
      << "if (!_tao_any.type ()->equal (" << node->tc_name ()
      << ", _tao_env)) return 0; // not equal" << be_nl
      << "TAO_InputCDR stream ((ACE_Message_Block *)_tao_any.value ());"
      << be_nl
      << "CORBA::Object_ptr _tao_obj;" << be_nl
      << "if (stream.decode (" << node->tc_name ()
      << ", &_tao_obj, 0, _tao_env)" << be_nl
      << "  == CORBA::TypeCode::TRAVERSE_CONTINUE)" << be_nl
      << "{" << be_idt_nl
      << "*_tao_elem = " << node->name () << "::_narrow (_tao_obj, _tao_env);"
      << be_nl
      << "CORBA::release (_tao_obj);" << be_nl
      << "if (_tao_env.exception ()) return 0; // narrow failed" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}\n\n";

  os->indent ();
  // generate the typecode information here
  os->indent (); // start from current indentation level
  *os << "static const CORBA::Long _oc_" << node->flatname () << "[] =" <<
    be_nl;
  *os << "{\n";
  os->incr_indent (0);
  if (node->gen_encapsulation () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "codegen for typecode failed\n"), -1);
    }

  os->decr_indent ();
  *os << "};" << be_nl;

  *os << "static CORBA::TypeCode _tc__tc_" << node->flatname () <<
    " (CORBA::tk_objref, sizeof (_oc_" <<  node->flatname () <<
    "), (char *) &_oc_" << node->flatname () <<
    ", CORBA::B_FALSE);" << be_nl;
  *os << "CORBA::TypeCode_ptr " << node->tc_name () << " = &_tc__tc_" <<
    node->flatname () << ";\n\n";

  return 0;
}

// ************************************************************
// Interface visitor for server header
// ************************************************************

be_visitor_interface_sh::be_visitor_interface_sh (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_sh::~be_visitor_interface_sh (void)
{
}

int
be_visitor_interface_sh::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream
  long i; // loop index
  static char namebuf [NAMEBUFSIZE]; // holds the class name

  if (node->srv_hdr_gen () || node->imported ())
    return 0;

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);

  os = this->ctx_->stream ();

  // generate the skeleton class name

  os->indent (); // start with whatever indentation level we are at

  // we shall have a POA_ prefix only if we are at the topmost level
  if (!node->is_nested ())
    {
      // we are outermost
      ACE_OS::sprintf (namebuf, "POA_%s", node->local_name ()->get_string ());
    }
  else
    {
      ACE_OS::sprintf (namebuf, "%s", node->local_name ()->get_string ());
    }

  *os << "class " << namebuf << ";" << be_nl;

  // generate the _ptr declaration
  *os << "typedef " << namebuf << " *" << namebuf
      << "_ptr;" << be_nl;

  // now generate the class definition
  *os << "class " << idl_global->export_macro ()
      << " " << namebuf << " : ";
  if (node->n_inherits () > 0)  // this interface inherits from other interfaces
    {
      be_interface *intf; // inherited interface

      *os << "public virtual ";
      intf = be_interface::narrow_from_decl (node->inherits ()[0]);
      *os << intf->relative_skel_name (node->full_skel_name ());
      for (i = 1; i < node->n_inherits (); i++)
        {
          *os << ", public virtual ";
          intf = be_interface::narrow_from_decl (node->inherits ()[i]);
          *os << intf->relative_skel_name (node->full_skel_name ());
        }  // end of for loop
    }
  else
    {
      // We don't inherit from another user defined object, hence our
      // base class is the ServantBase class.
      *os << " public virtual PortableServer::ServantBase";
    }
  *os << be_nl
      << "{" << be_nl
      << "protected:" << be_idt_nl
      << namebuf << " (void);" << be_uidt_nl
      << "public:" << be_idt_nl
      << "virtual ~" << namebuf << " (void);" << be_nl;

  *os << "virtual CORBA::Boolean _is_a (" << be_idt << be_idt_nl
      << "const char* logical_type_id," << be_nl
      << "CORBA::Environment &_tao_environment" << be_uidt
      << ");\n" << be_uidt;

  os->indent ();
  *os << "virtual void* _downcast (" << be_idt << be_idt_nl
      << "const char* logical_type_id" << be_uidt_nl
      << ");\n" << be_uidt;

  // generate code for elements in the scope (e.g., operations)
  if (this->visit_scope (node) ==  -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }
  // add a skeleton for our _is_a method
  os->indent ();
  *os << "static void _is_a_skel (CORBA::ServerRequest &req, " <<
    "void *obj, void *context, CORBA::Environment &_tao_enviroment);\n\n";

  // generate skeletons for operations of our base classes. These skeletons
  // just cast the pointer to the appropriate type before invoking the call
  if (node->traverse_inheritance_graph (be_interface::gen_skel_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "inheritance graph traversal failed\n"),
                        -1);
    }

  // add the dispatch method
  os->indent ();
  *os << "virtual void _dispatch (CORBA::ServerRequest &_tao_req, "
      << "void *_tao_context, CORBA::Environment &_tao_env);\n\n";

  // Print out the _this() method.
  os->indent ();
  *os << node->name () << " *_this (CORBA::Environment &_tao_environment);\n";
  // the _interface_repository_id method
  os->indent ();
  *os << "virtual const char* _interface_repository_id"
      << " (void) const;\n";

  os->decr_indent ();

  *os << "};\n\n";

  // generate the collocated class
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "codegen for collocated class failed\n"),
                        -1);
    }

  *os << "\n";

  return 0;
}

// ************************************************************************
// Interface visitor for server inline
// ************************************************************************

be_visitor_interface_si::be_visitor_interface_si (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_si::~be_visitor_interface_si (void)
{
}

int
be_visitor_interface_si::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream

  if (node->srv_inline_gen () || node->imported ())
    return 0;

  os = this->ctx_->stream ();

  os->indent (); // start with whatever indentation level we are at

  // Generate skeletons for operations of our base classes. These skeletons
  // just cast the pointer to the appropriate type before invoking the
  // call. Hence we generate these in the inline file
  if (node->traverse_inheritance_graph (be_interface::gen_skel_helper, os)
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_si::"
                         "visit_interface - "
                         "codegen for base class skeletons failed\n"), -1);
    }

  return 0;
}

// ************************************************************
// Interface visitor for server skeletons
// ************************************************************

be_visitor_interface_ss::be_visitor_interface_ss (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ss::~be_visitor_interface_ss (void)
{
}

int
be_visitor_interface_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream

  if (node->srv_skel_gen () || node->imported ())
    return 0;

  os = this->ctx_->stream ();

  // generate the skeleton class name

  os->indent (); // start with whatever indentation level we are at

  if (node->gen_operation_table () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "codegen for operation table failed\n"),
                        -1);
    }

  // constructor
  *os << "// skeleton constructor" << be_nl;
  // find if we are at the top scope or inside some module
  if (!node->is_nested ())
    {
      // we are outermost. So the POA_ prefix is prepended to our name
      *os << node->full_skel_name () << "::POA_" << node->local_name () <<
        " (void)" << be_nl;
    }
  else
    {
      // the POA_ prefix is prepended to our outermost module name
      *os << node->full_skel_name () << "::" << node->local_name () <<
        " (void)" << be_nl;
    }

  *os << "{" << be_idt_nl
      << "this->optable_ = &tao_" << node->flatname ()
      << "_optable;" << be_uidt_nl
      << "}\n\n";

  // destructor
  os->indent ();
  *os << "// skeleton destructor" << be_nl;

  if (!node->is_nested ())
    {
      // we are outermost. So the POA_ prefix is prepended to our name
      *os << node->full_skel_name () << "::~POA_" << node->local_name () <<
        " (void)" << be_nl;
    }
  else
    {
      // the POA_ prefix is prepended to our outermost module name
      *os << node->full_skel_name () << "::~" << node->local_name () <<
        " (void)" << be_nl;
    }
  *os << "{\n";
  *os << "}\n";


  // generate code for elements in the scope (e.g., operations)
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // generate code for the _is_a skeleton
  os->indent ();
  *os << "static const TAO_Param_Data_Skel " << node->flatname ()
      << "_is_a_paramdata [] = " << be_nl;
  *os << "{" << be_idt_nl;
  *os << "{CORBA::_tc_boolean, 0, 0}," << be_nl;
  *os << "{CORBA::_tc_string, CORBA::ARG_IN, 0}" << be_uidt_nl;
  *os << "};" << be_nl;
  *os << "static const TAO_Call_Data_Skel " << node->flatname ()
      << "_is_a_calldata = " << be_nl;
  *os << "{\"_is_a\", 1, 2, " << node->flatname () << "_is_a_paramdata};" << be_nl;
  *os << "void " << node->full_skel_name ()
      << "::_is_a_skel (" << be_idt << be_idt_nl
      << "CORBA::ServerRequest &_tao_server_request, " << be_nl
      << "void * _tao_object_reference," << be_nl
      << "void * /*context*/," << be_nl
      << "CORBA::Environment &_tao_environment" << be_uidt_nl
      << ")" << be_uidt_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << node->full_skel_name () << "_ptr  _tao_impl = ("
      << node->full_skel_name () << "_ptr) _tao_object_reference;"
      << be_nl;
  *os << "CORBA::Boolean _tao_retval;" << be_nl;
  *os << "char *_tao_value = 0;" << be_nl;
  *os << "_tao_server_request.demarshal (" << be_idt_nl
      << "_tao_environment, " << be_nl
      << "&" << node->flatname () << "_is_a_calldata, " << be_nl
      << "&_tao_retval, " << be_nl
      << "&_tao_value" << be_uidt_nl
      << ");" << be_nl;
  *os << "if (_tao_environment.exception () != 0) return;" << be_nl;
  *os << "_tao_retval = _tao_impl->_is_a (_tao_value, _tao_environment);"
      << be_nl;
  *os << "_tao_server_request.marshal (" << be_idt_nl
      << "_tao_environment, " << be_nl
      << "&" << node->flatname () << "_is_a_calldata, " << be_nl
      << "&_tao_retval, " << be_nl
      << "&_tao_value" << be_uidt_nl
      << ");" << be_nl;
  *os << "CORBA::string_free (_tao_value);" << be_uidt_nl;
  *os << "}\n\n";


  os->indent ();
  *os << "CORBA::Boolean " << node->full_skel_name ()
      << "::_is_a (" << be_idt << be_idt_nl
      << "const char* value," << be_nl
      << "CORBA::Environment &_tao_environment" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (\n" << be_idt;
  if (node->traverse_inheritance_graph (be_interface::is_a_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "traversal of inhertance graph failed\n"),
                        -1);
    }

  os->indent ();
  *os << "(!ACE_OS::strcmp ((char *)value, "
      << "CORBA::_tc_Object->id (_tao_environment))))"
      << be_idt_nl << "return CORBA::B_TRUE;" << be_uidt_nl
      << "else" << be_idt_nl
      << "return CORBA::B_FALSE;" << be_uidt << be_uidt << be_uidt_nl
      << "}\n\n";

  os->indent ();
  *os << "void* " << node->full_skel_name ()
      << "::_downcast (" << be_idt << be_idt_nl
      << "const char* logical_type_id" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  if (node->traverse_inheritance_graph (be_interface::downcast_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "traversal of inhertance graph failed\n"),
                        -1);
    }

  *os << "if (ACE_OS::strcmp (logical_type_id, "
      << "\"IDL:omg.org/CORBA/Object:1.0\") == 0)" << be_idt_nl
      << "return ACE_static_cast(PortableServer::Servant, this);"
      << be_uidt_nl;

  *os << "return 0;" << be_uidt_nl
      << "}\n\n";


  // now the dispatch method
  os->indent ();
  *os << "void " << node->full_skel_name () <<
    "::_dispatch (CORBA::ServerRequest &req, " <<
    "void *context, CORBA::Environment &env)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "TAO_Skeleton skel; // pointer to skeleton for operation" << be_nl;
  *os << "const char *opname = req.operation (); // retrieve operation name"
      << be_nl;
  *os << "// find the skeleton corresponding to this opname" << be_nl;
  *os << "if (this->_find (opname, skel) == -1)" << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "env.exception (new CORBA_BAD_OPERATION (CORBA::COMPLETED_NO));"
      << be_nl;
  *os << "ACE_ERROR ((LM_ERROR, \"Bad operation <%s>\\n\", opname));\n";
  os->decr_indent ();
  *os << "}\n";
  *os << "else" << be_nl;
  *os << "  skel (req, this, context, env);\n";
  os->decr_indent ();
  *os << "}\n\n";

  os->indent ();
  *os << "const char* " << node->full_skel_name ()
      << "::_interface_repository_id (void) const"
      << be_nl;
  *os << "{\n";
  os->incr_indent ();
  *os << "return \"" << node->repoID () << "\";\n";
  os->decr_indent ();
  *os << "}\n\n";

  // generate the collocated class impl
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "Bad visitor for collocated class\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "codegen for collocated class failed\n"),
                        -1);
    }
  delete visitor;

  *os << "\n";

  // the _this () operation
  *os << node->name () << "*" << be_nl
      << node->full_skel_name ()
      << "::_this (CORBA_Environment &_env)" << be_nl
      << "{" << be_idt_nl
      << "STUB_Object *stub = this->_create_stub (_env);" << be_nl
      << "if (_env.exception () != 0)" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "return new " << node->full_coll_name ()
      << " (this, stub);" << be_uidt << be_nl;

  *os << "}\n\n";

   return 0;
}

// ************************************************************
//  collocated class in header
// ************************************************************

be_visitor_interface_collocated_sh::be_visitor_interface_collocated_sh
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_collocated_sh::~be_visitor_interface_collocated_sh (void)
{
}

int be_visitor_interface_collocated_sh::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->gen_ifdef_macro (node->flatname (), "_collocated");

  // output the class defn
  os->indent ();
  *os << "class " << idl_global->export_macro ()
      << " " << node->local_coll_name ();
  os->incr_indent ();
  *os << " : public virtual " << node->name ();

  // generate base classes if any
  if (node->n_inherits () > 0)
    {
      for (int i = 0; i < node->n_inherits (); ++i)
        {
          *os << "," << be_nl;
          be_interface* parent =
            be_interface::narrow_from_decl (node->inherits()[i]);
          *os << "  public virtual "
              << parent->relative_coll_name (node->full_coll_name ());
        }
    }
  *os << "\n";
  os->decr_indent ();
  *os << "{" << be_nl;
  *os << "public:\n";
  os->incr_indent ();

  *os << node->local_coll_name () << " (\n";

  os->incr_indent (0);
  os->incr_indent ();

  // XXXASG - can we make this a method to return the right name ??
  if (!node->is_nested ())
    {
      // The skeleton name is the outermost, we need to printout the
      // POA_ prefix that goes with it.
      *os << "POA_";
    }

  *os << node->local_name () << "_ptr "
      << " servant," << be_nl;

  *os << "STUB_Object *stub\n";
  os->decr_indent ();
  *os << ");\n";
  os->decr_indent (0);

  os->indent ();
  if (!node->is_nested ())
    {
      // The skeleton name is the outermost, we need to printout the
      // POA_ prefix that goes with it.
      *os << "POA_";
    }
  *os << node->local_name ()
      << "_ptr _get_servant (void) const;" << be_nl;

  *os << "virtual CORBA::Boolean _is_a (" << be_idt << be_idt_nl
      << "const char *logical_type_id," << be_nl
      << "CORBA::Environment &_tao_environment" << be_uidt_nl
      << ");\n" << be_uidt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_collocated_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

#if 0
  // XXXASG - don't remove until we are sure that the scope visitor is doing
  // the task we expect it to do
  if (node->nmembers () > 0)
    {
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (node,
                                               UTL_Scope::IK_decls),
                      -1);
      while (!si->is_done ())
        {
          AST_Decl *d = si->item ();
          si->next ();
          be_decl *bd = be_decl::narrow_from_decl (d);
          if (d->imported () || bd == 0)
            {
              continue;
            }
          if (bd->accept (this) == -1)
            {
              delete si;
              return -1;
            }
        }
      delete si;
    }
#endif

  os->decr_indent ();

  *os << be_nl << "private:\n";
  os->incr_indent ();
  if (!node->is_nested ())
    {
      // The skeleton name is the outermost, we need to printout the
      // POA_ prefix that goes with it.
      *os << "POA_";
    }
  *os << node->local_name () << "_ptr servant_;\n";
  os->decr_indent ();
  *os << "};\n\n";

  os->gen_endif ();

  return 0;
}

#if 0
int be_visitor_interface_collocated_sh::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->indent (); // start with the current indentation level

  // every operation is declared virtual
  *os << "virtual ";

  // first generate the return type
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  // XXASG - this changes
  if (bt->write_as_return (os, bt) == -1)
    {
      return -1;
    }

  // generate the operation name
  *os << " " << node->local_name () << " ";

  // XXXASG - TODO
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_colocated_sh::"
                         "visit_operation - "
                         "Bad visitor for arglist\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_colocated_sh::"
                         "visit_operation - "
                         "codegen for arglist failed\n"),
                        -1);
    }
  *os << ";\n";

  return 0;
}

int be_visitor_interface_collocated_sh::visit_attribute (be_attribute *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  os->indent (); // start with the current indentation level

  be_type* bt = be_type::narrow_from_decl (node->field_type ());

  // the retrieve method is defined virtual
  *os << "virtual ";

  // XXXASG - change this
  if (bt->write_as_return (os, bt) == -1)
    {
      return -1;
    }

  *os << " " << node->local_name () << " (" << be_idt << be_idt_nl
      << "CORBA::Environment &env" << be_uidt_nl
      << ");\n" << be_uidt;

  if (!node->readonly ())
    {
      os->indent ();
      *os << "virtual void " << node->local_name ()
          << " (" << be_idt << be_idt;

      // XXXASG - TODO
      be_visitor_args_decl vdecl (os);
      vdecl.current_type_name (bt->name ());
      vdecl.argument_direction (AST_Argument::dir_IN);
      if (bt->accept (&vdecl) == -1)
        return -1;

      *os << " _tao_value," << be_nl
          << "CORBA::Environment &_tao_environment" << be_uidt_nl
          << ");\n" << be_uidt;
    }
  return 0;
}
#endif

// ************************************************************
//  be_visitor_interface_collacted_ss
// ************************************************************

be_visitor_interface_collocated_ss::be_visitor_interface_collocated_ss
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_collocated_ss::~be_visitor_interface_collocated_ss (void)
{
}

int be_visitor_interface_collocated_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os = tao_cg->server_skeletons ();
  long i;

  this->ctx_->node (node);

  *os << node->full_coll_name () << "::"
      << node->local_coll_name () << " (\n";

  os->incr_indent (0);
  os->incr_indent ();
  *os << node->full_skel_name () << "_ptr "
      << " servant," << be_nl;

  *os << "STUB_Object *stub\n";
  os->decr_indent ();
  *os << ")\n";
  os->decr_indent (0);

  os->incr_indent ();
#if defined (ACE_WIN32)
  // @@ TODO MSVC++ compiler has some kind of issue (read
  // *bug*) wrt nested classes in constructors, if the fully
  // qualified name is used it gets all confused. Quite to my
  // dismay the work around is to use a non-qualified name for
  // the base class!
  // I wish I never have to know why the symbol table for
  // MSVC++ can get so confused ;-) (coryan)
  *os << ": " << node->local_name ()
      << " (stub, servant, CORBA::B_TRUE)," << be_nl;
#else
  *os << ": " << node->name ()
      << " (stub, servant, CORBA::B_TRUE)," << be_nl;
#endif /* ACE_WIN32 */

  // @@ We should call the constructor for all base classes, since we
  // are using multiple inheritance.

  if (node->n_inherits () > 0)
    {
      for (i = 0; i < node->n_inherits (); ++i)
        {
          be_interface* parent =
            be_interface::narrow_from_decl (node->inherits()[i]);
#if defined (ACE_WIN32)
          // @@ TODO MSVC++ compiler has some kind of issue (read
          // *bug*) wrt nested classes in constructors, if the fully
          // qualified name is used it gets all confused. Quite to my
          // dismay the work around is to use a non-qualified name for
          // the base class!
          // I wish I never have to know why the symbol table for
          // MSVC++ can get so confused ;-) (coryan)
          *os << "  " << parent->local_coll_name () << " (servant, stub),"
              << be_nl;
#else
          *os << "  " << parent->full_coll_name () << " (servant, stub),"
              << be_nl;
#endif /* ACE_WIN32 */
        }
    }

  *os << "  CORBA_Object (stub, servant, CORBA::B_TRUE)," << be_nl
      << "  servant_ (servant)";

  *os << "\n";
  os->decr_indent ();
  *os << "{\n";
  *os << "}\n\n";

  os->indent ();
  *os << node->full_skel_name () << "_ptr "
      << node->full_coll_name () << "::"
      << "_get_servant (void) const\n"
      << "{\n";
  os->incr_indent ();
  *os << "return this->servant_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  os->indent ();
  *os << "CORBA::Boolean " << node->full_coll_name ()
      << "::_is_a (" << be_idt << be_idt_nl
      << "const char* logical_type_id," << be_nl
      << "CORBA::Environment &_tao_environment" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return this->servant_->_is_a (" << be_idt << be_idt_nl
      << "logical_type_id," << be_nl
      << "_tao_environment" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}\n\n";

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_collocated_ss::"
                         "visit_scope - "
                         "codegen for scope failed\n"),
                        -1);
    }

#if 0
  // XXXASG - DO NOT DELETE until the visit_scope has been tested
  if (node->nmembers () > 0)
    {
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (node,
                                               UTL_Scope::IK_decls),
                      -1);
      while (!si->is_done ())
        {
          AST_Decl *d = si->item ();
          si->next ();
          be_decl *bd = be_decl::narrow_from_decl (d);
          // Only printout the operations, nested interfaces and
          // structures only go in the main declaration.
          if (d->imported () || bd == 0)
            {
              continue;
            }
          if (bd->accept (this) == -1)
            {
              delete si;
              return -1;
            }
        }
      delete si;
    }
#endif

  return 0;
}

#if 0
int be_visitor_interface_collocated_ss::visit_operation (be_operation *node)
{
  TAO_OutStream *os = tao_cg->server_skeletons ();
  be_interface *intf = this->ctx_->be_scope_as_interface ();

  // retrieve the return type again because we have used bt to also retrieve
  // the argument types
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (bt->write_as_return (os, bt) == -1)
    {
      return -1;
    }

  *os << " " << intf->full_coll_name () << "::"
      << node->local_name () << " ";

  be_visitor_args_decl visitor (os);
  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) args decl failed\n"), -1);
    }
  *os << "\n";
  os->indent ();
  *os << "{\n";
  os->incr_indent ();

  if (bt->node_type () != AST_Decl::NT_pre_defined
      || be_predefined_type::narrow_from_decl (bt)->pt () != AST_PredefinedType::PT_void)
    {
      *os << "return ";
    }

  *os << "this->servant_->" << node->local_name () << " (\n";
  os->incr_indent (0);
  os->incr_indent (0);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_collocated_ss::"
                         "visit_operation - "
                         "codegen for scope failed\n"),
                        -1);
    }

  os->indent ();
  *os << " _tao_environment\n";
  os->decr_indent ();
  *os << ");\n";
  os->decr_indent (0);
  os->decr_indent (0);
  *os << "}\n\n";

  return 0;
}

int be_visitor_interface_collocated_ss::visit_attribute (be_attribute *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_interface *intf = this->ctx_->be_scope_as_interface ();

  os->indent (); // start with the current indentation level

  be_type* bt = be_type::narrow_from_decl (node->field_type ());

  if (bt->write_as_return (os, bt) == -1)
    {
      return -1;
    }

  *os << be_nl << intf->full_coll_name ()
      << "::" << node->local_name () << " (" << be_idt << be_idt_nl
      << "CORBA::Environment &_tao_environment" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return this->servant_->"
      << node->local_name () << "(_tao_environment);" << be_uidt_nl
      << "}\n";

  if (!node->readonly ())
    {
      *os << be_nl
          << "void "
          << intf->full_coll_name ()
          << "::" << node->local_name ()
          << " (" << be_idt << be_idt_nl;
      // XXXASG - TODO
      be_visitor_args_decl vdecl (os);
      vdecl.current_type_name (bt->name ());
      vdecl.argument_direction (AST_Argument::dir_IN);
      if (bt->accept (&vdecl) == -1)
        return -1;

      *os << " _tao_value," << be_nl
          << "CORBA::Environment &_tao_environment" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "this->servant_->" << node->local_name ()
          << " (" << be_idt << be_idt_nl
          << "_tao_value," << be_nl
          << "_tao_environment" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}\n\n";
    }
  return 0;
}

int be_visitor_interface_collocated_ss::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->indent ();
  *os << node->local_name () << ",\n";
  return 0;
}
#endif
