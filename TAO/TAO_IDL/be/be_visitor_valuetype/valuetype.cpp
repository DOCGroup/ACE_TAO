
//=============================================================================
/**
 *  @file    valuetype.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Valuetypes. This is a generic visitor.
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de> based on interface.cpp from Aniruddha Gokhale
 */
//=============================================================================

be_visitor_valuetype::be_visitor_valuetype (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_valuetype::~be_visitor_valuetype (void)
{
}

int
be_visitor_valuetype::visit_valuetype_scope (be_valuetype *node)
{
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

      be_decl *bd = be_decl::narrow_from_decl (d);
      // Set the scope node as "node" in which the code is being
      // generated so that elements in the node's scope can use it
      // for code generation.

      this->ctx_->scope (node);
      this->ctx_->node (bd);
      this->elem_number_++;

      AST_Field *field = AST_Field::narrow_from_decl (d);

      if (field != 0 && field->visibility () == AST_Field::vis_PRIVATE)
        {
          this->begin_private ();
        }
      else
        {
          this->begin_public ();
        }

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

  get_op.destroy ();

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
  AST_Argument *arg =
    idl_global->gen ()->create_argument (AST_Argument::dir_IN,
                                         node->field_type (),
                                         node->name ());

  arg->set_name ((UTL_IdList *) node->name ()->copy ());

  // Create the operation.
  be_operation set_op (&rt,
                       AST_Operation::OP_noflags,
                       node->name (),
                       0,
                       0);

  set_op.set_name ((UTL_IdList *) node->name ()->copy ());
  set_op.be_add_argument (arg);

 if (this->visit_operation (&set_op) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "codegen for set_attribute failed\n"),
                        -1);
    }

  set_op.destroy ();
  rt.destroy ();

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
    case TAO_CodeGen::TAO_MODULE_OBV_CI:
    case TAO_CodeGen::TAO_MODULE_OBV_CS:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CI:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_IS:
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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_enum_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
    case TAO_CodeGen::TAO_ROOT_CI:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_IS:
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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_union_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_IS:
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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_typedef_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
    case TAO_CodeGen::TAO_MODULE_OBV_CI:
    case TAO_CodeGen::TAO_MODULE_OBV_CS:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_IS:
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
      be_attribute *attr = be_attribute::narrow_from_decl (d);

      if (field == 0 || attr != 0)
        {
          continue;
        }

      ++n_processed;

      // Set the scope node as "node" in which the code is being
      // generated so that elements in the node's scope can use it
      // for code generation.
      this->ctx_->scope (node);

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

void
be_visitor_valuetype::gen_obv_init_constructor_args (be_valuetype *node,
                                                     unsigned long &index)
{
  TAO_OutStream *os = this->ctx_->stream ();
  AST_Type *parent = node->inherits_concrete ();

  // Generate for inherited members first.
  if (parent != 0)
    {
      be_valuetype *be_parent =
        be_valuetype::narrow_from_decl (parent);
      this->gen_obv_init_constructor_args (be_parent, index);
    }

  be_visitor_context ctx (*this->ctx_);
  be_visitor_args_arglist visitor (&ctx);

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      // be_attribute inherits from be_field
      // so we have to also screen out attributes
      be_field *f = be_field::narrow_from_decl (si.item ());
      be_attribute *attr =
        be_attribute::narrow_from_decl (si.item ());

      if (f == 0 || attr != 0)
        {
          continue;
        }

      *os << (index++ != 0 ? "," : "") << be_nl;

      ACE_CString arg_name ("_tao_init_");
      arg_name += f->local_name ()->get_string ();
      Identifier id (arg_name.c_str ());
      UTL_ScopedName sn (&id, 0);
      be_type *ft = be_type::narrow_from_decl (f->field_type ());
      bool seen = ft->seen_in_operation ();

      // This sets ft->seen_in_operation (true), so we have to
      // restore the original value below.
      be_argument arg (AST_Argument::dir_IN,
                       ft,
                       &sn);
      ft->seen_in_operation (seen);
      visitor.visit_argument (&arg);

      // AST_Argument inherits from AST_Field, which will destroy
      // its field type if it is anonymous - we don't want that.
      arg.be_decl::destroy ();
      arg.AST_Decl::destroy ();
      id.destroy ();
    }
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

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "class " << be_global->stub_export_macro ()
      << " " << node->local_name ()
      << "_init : public ::CORBA::ValueFactoryBase" << be_nl;

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

  *os << "const char*" << be_nl
      << fname << "::tao_repository_id (void)" << be_nl
      << "{" << be_idt_nl
      <<   "return " << node->local_name ()
      <<                "::_tao_obv_static_repository_id ();"
      << be_uidt_nl << "}\n\n";

  return 0;
}

bool
be_visitor_valuetype::obv_need_ref_counter (be_valuetype* node)
{
  // Go thru our base VTs and see if one has already.
  for (int i = 0; i < node->n_inherits (); ++i)
    {
      be_valuetype *vt =
        be_valuetype::narrow_from_decl (node->inherits ()[i]);

      if (vt != 0)
        {
          if (be_visitor_valuetype::obv_have_ref_counter (vt))
            {
              return false;
            }
        }
    }

  // If we inherit from CORBA::Object and/or CORBA::AbstractBase
  // (in addition to CORBA::ValueBase) we have to override _add_ref()
  // and _remove_ref() by calling the one in DefaultValueRefCountBase
  // to avoid ambiguity.
  if (node->n_supports () > 0)
    {
      return true;
    }

  // VT needs RefCounter if it has concrete factory.
  if (be_valuetype::FS_CONCRETE_FACTORY == node->determine_factory_style ())
    {
      return true;
    }

  return false;
}

bool
be_visitor_valuetype::obv_have_ref_counter (be_valuetype* node)
{

  // Just try to find a VT with concrete factory in inheritance tree.
  if (node == 0)
    {
      return false;
    }

  if (node->determine_factory_style () == be_valuetype::FS_CONCRETE_FACTORY)
    {
      return true;
    }

  // Now go thru our base VTs.
  for (int i = 0; i < node->n_inherits (); ++i)
    {
      be_valuetype *vt = be_valuetype::narrow_from_decl (node->inherits ()[i]);

      if (vt != 0)
        {
          if (be_visitor_valuetype::obv_have_ref_counter (vt))
            {
              return true;
            }
        }
    }

  return false;
}

bool
be_visitor_valuetype::is_amh_exception_holder (be_valuetype *node)
{
 if (ACE_OS::strncmp (node->local_name (), "AMH_", 4) == 0)
   {
     const char *last_E =
      ACE_OS::strrchr (node->full_name (), 'E');

     if (last_E != 0
         && ACE_OS::strcmp (last_E, "ExceptionHolder") == 0)
       {
         return true;
       }
   }

  return false;
}
