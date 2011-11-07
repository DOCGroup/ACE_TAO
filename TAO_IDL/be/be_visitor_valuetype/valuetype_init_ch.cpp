
//=============================================================================
/**
 *  @file    valuetype_init_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Valuetypes factory in the client header
 *  (see IDL to C++ mapping). Based on ptc/00-01-02.
 *
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================


be_visitor_valuetype_init_ch::be_visitor_valuetype_init_ch (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype_init (ctx)
{
}

be_visitor_valuetype_init_ch::~be_visitor_valuetype_init_ch (void)
{
}

int
be_visitor_valuetype_init_ch::visit_valuetype (be_valuetype *node)
{
  if (node->is_abstract ())
    {
      return 0;
    }

  // There are three possible situations.
  // (1) If there is no initializers but at least one operation.
  //     In this case we don't need to bother about factory.
  //
  // (2) There are no (operations or initializers) (i.e. only state
  //     members) then we need a concrete type-specific factory
  //     class whose create_for_unmarshal creates OBV_ class.
  //
  // (3) There is at least one operation and at least one initializer.
  //     In this case we need to generate abstract factory class.

  be_valuetype::FactoryStyle factory_style =
    node->determine_factory_style ();

  if (factory_style == be_valuetype::FS_NO_FACTORY)
    {
      // Nothing to do.
      return 0;
    }

  TAO_OutStream& os = *(this->ctx_->stream ());

  os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  //@@ If I'm generating concrete class I need a RefCounter.
  os << "class " << be_global->stub_export_macro ()
     << " " << node->local_name ()
     << "_init" << be_idt_nl
     << ": public virtual ::CORBA::ValueFactoryBase" << be_uidt_nl;

  // Generate the body.
  os << "{" << be_nl
     << "public:" << be_idt;

  if (factory_style == be_valuetype::FS_CONCRETE_FACTORY)
    {
      // Public constructor.
      os << be_nl
         << node->local_name () << "_init (void);";
    }

  if (this->visit_valuetype_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_init_ch::"
                         "visit_valuetype - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // Generate _downcast method.
  os << be_nl_2
     << "static " << node->local_name () << "_init* "
     << "_downcast ( ::CORBA::ValueFactoryBase *);";

  if (factory_style == be_valuetype::FS_CONCRETE_FACTORY)
    {
      //@@ Boris: create_for_unmarshal is still public...
      // generate create_for_unmarshal
      os << be_nl_2
         << "virtual ::CORBA::ValueBase *" << be_nl
         << "create_for_unmarshal (void);";

      if (node->supports_abstract ())
        {
          os << be_nl_2
             << "virtual ::CORBA::AbstractBase_ptr" << be_nl
             << "create_for_unmarshal_abstract (void);" << be_uidt;
        }
    }

  os << be_nl_2;

    // Proprietary extensions.
  os << "// TAO-specific extensions"
     << be_uidt_nl
     << "public:" << be_idt_nl;
  os << "virtual const char* tao_repository_id (void);";

  if (factory_style == be_valuetype::FS_ABSTRACT_FACTORY)
    {
      // Protected constructor.
      os << be_uidt_nl << be_nl
         << "protected:" << be_idt_nl
         << node->local_name () << "_init (void);";
    }

  // Protected virtual destructor.
  os << be_uidt_nl << be_nl
     << "protected:" << be_idt_nl
     << "virtual ~" << node->local_name () << "_init (void);"
     << be_uidt_nl
     << "};";

  return 0;
}

int
be_visitor_valuetype_init_ch::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_valuetype_init_ch::visit_factory (be_factory *node)
{

  TAO_OutStream& os = *(this->ctx_->stream ());

  be_valuetype *vt =
    be_valuetype::narrow_from_decl (this->ctx_->scope ()->decl ());


  // STEP I: Generate preambule.
  os << be_nl_2
     << "virtual " << vt->local_name () << "* ";

  // STEP 2: Generate the operation name.
  os << node->local_name ();

  // STEP 3: Generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing.
  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_init_arglist_ch visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_init_arglist__ch::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  // Make pure virtual.
  os << " = 0;";

  return 0;
}
