//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes. This is a generic visitor.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    based on interface.cpp from Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype, 
           valuetype, 
           "$Id$")

be_visitor_valuetype::be_visitor_valuetype (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_valuetype::~be_visitor_valuetype (void)
{
}

// This method must be overridden by the derived valuetype visitors.
int
be_visitor_valuetype::visit_valuetype (be_valuetype *)
{
  return -1;
}

int
be_visitor_valuetype::visit_valuetype_scope (be_valuetype *node)
{
  int n_processed = 0;
  this->elem_number_ = 0;

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl *d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "bad node in this scope\n"), 
                            -1);
        }

      AST_Field *field = AST_Field::narrow_from_decl (d);

      if (field && field->visibility () == AST_Field::vis_PRIVATE)
        {
          continue;      
          // Ignore private fields in this run
          // AST_Attribute derives from AST_Field, so test for
          // vis_PRIVATE is ok (the attribute has it set to vis_NA)
        }

      be_decl *bd = be_decl::narrow_from_decl (d);
      // Set the scope node as "node" in which the code is being
      // generated so that elements in the node's scope can use it
      // for code generation.

      this->ctx_->scope (node->decl ());
      this->ctx_->node (bd);
      this->elem_number_++;

      if (bd == 0 || bd->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "codegen for scope failed\n"), 
                            -1);
          
        }
    }

  this->elem_number_ = 0;

  for (UTL_ScopeActiveIterator sj (node, UTL_Scope::IK_decls);
       !sj.is_done ();
       sj.next())
    {
      AST_Decl *d = sj.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "bad node in this scope\n"), 
                            -1);
        }

      AST_Field *field = AST_Field::narrow_from_decl (d);

      if (!field 
          || (field && field->visibility () != AST_Field::vis_PRIVATE))
        {
          // Only private fields.
          continue;
        }

      ++ n_processed;

      if (n_processed == 1)
        {
          this->begin_private ();
        }

      be_decl *bd = be_decl::narrow_from_decl (d);
      // Set the scope node as "node" in which the code is being
      // generated so that elements in the node's scope can use it
      // for code generation.

      this->ctx_->scope (node->decl ());
      this->ctx_->node (bd);
      this->elem_number_++;

      if (bd == 0 || bd->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "codegen for scope failed\n"),
                            -1);
        }
    }

  return 0;
}

// These two are called from visit_valuetype_scope()
void
be_visitor_valuetype::begin_public ()
{
  // In derived visitors print "public:" in class definition
}

void
be_visitor_valuetype::begin_private ()
{
  // In derived visitors print "protected:" in class definition
}

// All common visit methods for valuetype visitor.

int
be_visitor_valuetype::visit_attribute (be_attribute *node)
{
  this->ctx_->node (node);
  this->ctx_->attribute (node);

  be_operation get_op (node->field_type (),
                       AST_Operation::OP_noflags,
                       node->name (),
                       0,
                       0);

  get_op.set_name ((UTL_IdList *) node->name ()->copy ());

  if (this->visit_operation (&get_op) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "codegen for get_attribute failed\n"),
                        -1);
    }

  if (node->readonly ())
    {
      // Nothing else to do.
      return 0;
    }

  Identifier id ("void");

  UTL_ScopedName sn (&id,
                     0);

  be_predefined_type rt (AST_PredefinedType::PT_void,
                         &sn);

  // Argument type is the same as the attribute type.
  be_argument arg (AST_Argument::dir_IN,
                   node->field_type (),
                   node->name ());

  arg.set_name ((UTL_IdList *) node->name ()->copy ());

  // Create the operation.
  be_operation set_op (&rt,
                       AST_Operation::OP_noflags,
                       node->name (),
                       0,
                       0);

  set_op.set_name ((UTL_IdList *) node->name ()->copy ());
  set_op.be_add_argument (&arg);

 if (this->visit_operation (&set_op) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "codegen for set_attribute failed\n"),
                        -1);
    }

  return 0;
}


int
be_visitor_valuetype::visit_constant (be_constant *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_constant_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_constant_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CI:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CS:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CI:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
      return 0; // Nothing to be done.
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_valuetype::"
                           "visit_constant - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_constant - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype::visit_enum (be_enum *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_enum_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_enum_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_enum_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_enum_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_enum_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_enum_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CI:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CS:
    case TAO_CodeGen::TAO_ROOT_CI:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
      return 0; // Nothing to be done.
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_valuetype::"
                           "visit_enum - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_enum - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype::visit_exception (be_exception *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_exception_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {        
        be_visitor_exception_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_exception_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_exception_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_exception_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_exception_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_exception_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_exception_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // Nothing to be done.
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_exception - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype::visit_structure (be_structure *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_structure_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_structure_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_structure_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_structure_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_structure_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_structure_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_structure_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_structure_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // Nothing to be done.
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_structure - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype::visit_structure_fwd (be_structure_fwd *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_structure_fwd_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_structure_fwd - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype::visit_union (be_union *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_union_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_union_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_union_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_union_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_union_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_union_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_union_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_union_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CI:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CS:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
      return 0; // Nothing to be done.
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_valuetype::"
                           "visit_union - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_union - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype::visit_union_fwd (be_union_fwd *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_union_fwd_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_union_fwd - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype::visit_typedef (be_typedef *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_typedef_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_typedef_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_typedef_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_typedef_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_typedef_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_typedef_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_typedef_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_typedef_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CI:
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CS:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
      return 0; // Nothing to be done.
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_valuetype::"
                           "visit_typedef - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype::"
                         "visit_typedef - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype::visit_field (be_field *)
{
  // Is overridden in derived visitors.
  return 0;
}


// Private data fields for scope.
int
be_visitor_valuetype::gen_pd (be_valuetype *node)
{
  int n_processed = 0;
  this->elem_number_ = 0;

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl *d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "bad node in this scope\n"), 
                            -1);
        }

      be_field *field = be_field::narrow_from_decl (d);

      if (!field)
        {
          continue;
        }

      ++n_processed;

      // Set the scope node as "node" in which the code is being
      // generated so that elements in the node's scope can use it
      // for code generation.
      this->ctx_->scope (node->decl ());

      // Set the node to be visited.
      this->ctx_->node (field);
      this->elem_number_++;

      if (this->gen_field_pd (field) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "codegen for scope failed\n"), 
                            -1);
        }
    }

  return 0;
}

// Private data for field.
int
be_visitor_valuetype::gen_field_pd (be_field *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // First generate the type information.
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  be_valuetype *vt = be_valuetype::narrow_from_scope (node->defined_in ());

  if (!bt || !vt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_ch::"
                         "visit_field - "
                         "Bad field type\n"), 
                        -1);
    }

  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  be_visitor_field_ch visitor (&ctx);

  *os << be_nl;

  if (bt->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_ch::"
                         "visit_field - "
                         "codegen for field type failed\n"), 
                        -1);
    }

  // Now output the field name.  
  *os << " " << vt->field_pd_prefix ()
      << node->local_name ()
      << vt->field_pd_postfix () << ";";

  return 0;
}


// Generate the _init definition.
int
be_visitor_valuetype::gen_init_defn (be_valuetype *node)
{
  if (node->is_abstract ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "class " << be_global->stub_export_macro ()
      << " " << node->local_name ()
      << "_init : public CORBA::ValueFactoryBase" << be_nl;

  // Generate the body.
  *os << "{" << be_nl
      << "public:" << be_idt_nl
      << "virtual ~" << node->local_name () << "_init (void);" << be_nl;

  *os << "virtual const char* tao_repository_id (void);\n" << be_nl;
  *os << "// create () goes here" << be_nl;
  *os << be_uidt_nl << "};" << be_nl;

  return 0;
}

int
be_visitor_valuetype::gen_init_impl (be_valuetype *node)
{
  if (node->is_abstract ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  os->indent ();

  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _out names

  ACE_OS::memset (fname, 
                  '\0', 
                  NAMEBUFSIZE);
  ACE_OS::sprintf (fname, 
                   "%s_init", 
                   node->full_name ());

  ACE_OS::memset (lname, 
                  '\0', 
                  NAMEBUFSIZE);
  ACE_OS::sprintf (lname, 
                   "%s_init", 
                   node->local_name ());

  // Destructor.
  *os << fname << "::~" << lname << " (void)" << be_nl
      << "{" << be_nl << "}\n\n";

  *os << "const char* " << be_nl
      << fname << "::tao_repository_id (void)" << be_nl
      << "{" << be_idt_nl
      <<   "return " << node->local_name ()
      <<                "::_tao_obv_static_repository_id ();"
      << be_uidt_nl << "}\n\n";

  return 0;
}

be_visitor_valuetype::FactoryStyle
be_visitor_valuetype::determine_factory_style (be_valuetype* node)
{
  FactoryStyle factory_style = FS_UNKNOWN;

  if (node == 0)
    {
      return factory_style;
    }

  if (node->is_abstract ())
    {
      return FS_NO_FACTORY;
    }

  // Check whether we have at least one operation or not.
  idl_bool have_operation = be_visitor_valuetype::have_operation (node);


  idl_bool have_factory = 0;

  // Try only our own scope.
  if (node->nmembers () > 0)
    {
      // Initialize an iterator to iterate thru our scope
      // Continue until each element is visited.
      for (UTL_ScopeActiveIterator si (node,
                                       UTL_Scope::IK_decls); 
           !si.is_done (); 
           si.next())
        {
          AST_Decl *d = si.item ();

          if (!d)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_valuetype_init::"
                                 "determine_factory_style"
                                 "bad node in this scope\n"),
                                factory_style);

            }

          AST_Decl::NodeType node_type = d->node_type();

          if (node_type == AST_Decl::NT_factory)
            {
              have_factory = 1;
              break;
            }

        } // end of for loop
    } // end of if

  if (!have_operation && !have_factory)
    {
      factory_style = FS_CONCRETE_FACTORY;
    }
  else if (have_operation && !have_factory)
    {
      factory_style = FS_NO_FACTORY;
    }
  else
    {
      factory_style = FS_ABSTRACT_FACTORY;
    }

  return factory_style;
}

idl_bool
be_visitor_valuetype::have_operation (be_valuetype* node)
{
  // Check whatever scope we get for operations/attributes.

  if (node == 0)
    {
      return 0;
    }

  idl_bool have_operation = 0;

  // Operations are either operations or attributes of:
  // -its own
  // -derived (abstract VT | VT | abstract iface | iface)
  //

  // First try our own scope.
  if (node->nmembers () > 0)
    {
      // Initialize an iterator to iterate thru our scope
      // Continue until each element is checked.
      for (UTL_ScopeActiveIterator si (node,
                                       UTL_Scope::IK_decls); 
           !si.is_done (); 
           si.next())
        {
          AST_Decl *d = si.item ();

          if (!d)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_valuetype_init::"
                                 "has_operation"
                                 "bad node in this scope\n"),
                                0);

            }

          AST_Decl::NodeType node_type = d->node_type();

          if (node_type == AST_Decl::NT_op)
            {
              have_operation = 1;
              continue;
            }

          if (node_type == AST_Decl::NT_attr)
            {
              have_operation = 1;
              continue;
            }

          if (node_type == AST_Decl::NT_factory)
            {
              continue;
            }

          if (node_type == AST_Decl::NT_field)
            {
              continue;
            }

        } // end of for loop
    } // end of if

  // Now traverse inheritance tree.
  long i;  // loop index
  long n_inherits = node->n_inherits ();
  AST_Interface **inherits = node->inherits ();

  for (i = 0; i < n_inherits; ++i)
    {
      be_valuetype *vt = be_valuetype::narrow_from_decl (inherits[i]);

      if (vt != 0)
        {
          have_operation = have_operation ||
            be_visitor_valuetype::have_operation (vt);

          if (have_operation)
            {
              break;
            }
        }
    }

	have_operation = have_operation || be_visitor_valuetype::have_supported_op (node);

  return have_operation;
}

idl_bool
be_visitor_valuetype::have_supported_op (be_valuetype * node)
{
	// Check for operations on supported interfaces

	if (node == 0)
		return 0;

	idl_bool have_supported_op = 0;

	// Check each supported interface to see if it has operations defined
	long i; // loop index
	long n_supports = node->n_supports (); // get the number of supported interfaces
	AST_Interface ** supports = node->supports ();

	for (i = 0; i < n_supports; i++)
		{
			be_interface * vt = be_interface::narrow_from_decl (supports[i]);

			if (vt != 0)
				{

					if (vt->nmembers () > 0)

					// Initialize an iterator for supported interface elements
					for (UTL_ScopeActiveIterator si (vt, UTL_Scope::IK_decls); !si.is_done (); si.next())
						{
							AST_Decl *d = si.item ();

							if (!d)
								{
									ACE_ERROR_RETURN ((LM_ERROR,
										                 "(%N:%l) be_visitor_valuetype_init::"
											               "has_operation"
												             "bad node in this scope\n"),
													          0);

								}

							AST_Decl::NodeType node_type = d->node_type();

							// Check the type of each element in the supported interface
							if (node_type == AST_Decl::NT_op)
								{
									have_supported_op = 1;
									continue;
								}

							if (node_type == AST_Decl::NT_attr)
								{
									have_supported_op = 1;
									continue;
								}
						} // end for loop

				  if (have_supported_op)
					  {
						  break;
						}
				} // end if
		} // end for loop

	return have_supported_op;
}

idl_bool
be_visitor_valuetype::obv_need_ref_counter (be_valuetype* node)
{
  // VT needs RefCounter if it has concrete factory or supports an
  // abstract interface and none of its base VT has ref_counter

  if (be_visitor_valuetype::determine_factory_style (node) != FS_CONCRETE_FACTORY && !node->supports_abstract ())
    {
      return 0;
    }

  // Now go thru our base VTs and see if one has already.
  for (int i = 0; i < node->n_inherits (); ++i)
    {
      be_valuetype *vt = be_valuetype::narrow_from_decl (node->inherits ()[i]);

      if (vt != 0)
        {
          if (be_visitor_valuetype::obv_have_ref_counter (vt))
            {
              return 0;
            }
        }
    }

  return 1;
}

idl_bool
be_visitor_valuetype::obv_have_ref_counter (be_valuetype* node)
{

  // Just try to find a VT with concrete factory in inheritance tree.
  if (node == 0)
    {
      return 0;
    }

  if (be_visitor_valuetype::determine_factory_style (node) == FS_CONCRETE_FACTORY)
    {
      return 1;
    }

  // Now go thru our base VTs.
  for (int i = 0; i < node->n_inherits (); ++i)
    {
      be_valuetype *vt = be_valuetype::narrow_from_decl (node->inherits ()[i]);

      if (vt != 0)
        {
          if (be_visitor_valuetype::obv_have_ref_counter (vt))
            {
              return 1;
            }
        }
    }

  return 0;
}
