//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes in the client stubs file.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    based on code from Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype, 
           valuetype_cs, 
           "$Id$")

// ************************************************************
// Valuetype visitor for client stubs.
// ************************************************************

be_visitor_valuetype_cs::be_visitor_valuetype_cs (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_cs::~be_visitor_valuetype_cs (void)
{
}

int
be_visitor_valuetype_cs::is_amh_exception_holder (be_valuetype *node)
{
  // 1) Find out if the ValueType is an AMH_*ExceptionHolder, the
  // conditions are:
  //  a) The local_name starts with AMH_
  //  b) The local_name ends with ExceptionHolder
   int is_an_amh_exception_holder = 0;
   const char *amh_underbar = "AMH_";
   const char *node_name = node->local_name ();

   if( amh_underbar[0] == node_name[0] &&
       amh_underbar[1] == node_name[1] &&
       amh_underbar[2] == node_name[2] &&
       amh_underbar[3] == node_name[3]
       ) // node name starts with "AMH_"
     {
       const char *last_E = ACE_OS::strrchr (node->full_name (), 'E');

       if (last_E != 0
           && ACE_OS::strcmp (last_E, "ExceptionHolder") == 0)
         {
           is_an_amh_exception_holder = 1;
         }
     }
   return is_an_amh_exception_holder;
}

int
be_visitor_valuetype_cs::visit_valuetype (be_valuetype *node)
{
  if (node->cli_stub_gen () || node->imported ())
    {
      return 0;
    }

  if (be_global->tc_support ())
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE);
      be_visitor_typecode_defn tc_visitor (&ctx);

      if (node->accept (&tc_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_cs::"
                             "visit_valuetype - "
                             "TypeCode definition failed\n"), 
                            -1);
        }
    }
  
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl << be_nl
      << "void" << be_nl
      << "TAO::Value_Traits<" << node->name  () << ">::tao_add_ref (" 
      << be_idt << be_idt_nl
      << node->name () << " * p" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "CORBA::add_ref (p);" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << "void" << be_nl
      << "TAO::Value_Traits<" << node->name () << ">::tao_remove_ref ("
      << be_idt << be_idt_nl
      << node->name () << " * p" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "CORBA::remove_ref (p);" << be_uidt_nl
      << "}";

  const char *fhname = node->fwd_helper_name ();

  if (ACE_OS::strcmp (fhname, "") == 0)
    {
      node->gen_fwd_helper_name ();
      fhname = node->fwd_helper_name ();
    }

  // Helper functions to allow non-defined forward declared valuetypes
  // access to some methods in the full definition.
  *os << be_nl << be_nl
      << "void" << be_nl
      << fhname << "_life::tao_add_ref (" 
      << be_idt << be_idt_nl
      << node->full_name () << " * p" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "CORBA::add_ref (p);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "void" << be_nl
      << fhname << "_life::tao_remove_ref (" 
      << be_idt << be_idt_nl
       << node->full_name () << " * p" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "CORBA::remove_ref (p);" << be_uidt_nl
      << "}";

  // The _downcast method    // %! use ACE_xxx_cast here ?
  *os << be_nl << be_nl
      << node->name () << " *" << be_nl << node->name ()
      << "::_downcast (CORBA::ValueBase *v)" << be_nl
      << "{" << be_idt_nl
      << "if (v == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return (" << node->local_name () << " *) "
      << "v->_tao_obv_narrow ((ptrdiff_t) &_downcast);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // The _tao_obv_repository_id method
  *os << "const char *" << be_nl 
      << node->name () << "::_tao_obv_repository_id (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->_tao_obv_static_repository_id ();" << be_uidt_nl
      << "}" << be_nl;

  // The _tao_obv_narrow method

  *os << be_nl
      << "void *" << be_nl
      << "#if defined (_MSC_VER)" << be_nl
      << node->name () << "::" << node->flat_name ()
      << "_tao_obv_narrow (ptrdiff_t type_id)" << be_nl
      << "#else" << be_nl
      << node->name () << "::"
      << "_tao_obv_narrow (ptrdiff_t type_id)" << be_nl
      << "#endif /* _MSC_VER */" << be_nl
      << "{" << be_idt_nl
      << "if (type_id == (ptrdiff_t) &_downcast)" << be_idt_nl
      << "{" << be_idt_nl
      << "return this;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "void *rval = 0;" << be_nl;

  // Find the possible base classes.

  int n_inherits_downcastable = 0;
  AST_Interface *inherited = 0;

  for (int i = 0; i < node->n_inherits (); ++i)
    {
      inherited = node->inherits ()[i];

      ++n_inherits_downcastable;

      *os << be_nl
          << "if (rval == 0)" << be_idt_nl
          << "{"
          << "\n#if defined (_MSC_VER)" << be_idt_nl
          << "rval = this->" << inherited->flat_name ()
          << "_tao_obv_narrow (type_id);"
          << "\n#else" << be_nl
          << "rval = this->" << inherited->name () << "::"
          << "_tao_obv_narrow (type_id);"
          << "\n#endif /* _MSC_VER */" << be_uidt_nl
          << "}" << be_uidt_nl;
    }

  if (node->supports_abstract ())
    {
      long size = node->n_supports ();
      AST_Interface *supported = 0;

      for (long i = 0; i < size; ++i)
        {
          supported = node->supports ()[i];

          if (supported->is_abstract ())
            {
              *os << be_nl
                  << "if (rval == 0)" << be_idt_nl
                  << "{"
                  << "\n#if defined (_MSC_VER)" << be_idt_nl
                  << "rval = this->"
                  << supported->flat_name () 
                  << "_tao_obv_narrow (type_id);"
                  << "\n#else" << be_nl
                  << "rval = this->" << supported->name ()
                  << "::_tao_obv_narrow (type_id);"
                  << "\n#endif /* _MSC_VER */" << be_uidt_nl
                  << "}" << be_uidt_nl;
            }
        }
    }

  *os << be_nl << "return rval;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "#if defined (_MSC_VER)" << be_nl

      << "void *" << be_nl << node->name ()
      << "::_tao_obv_narrow (ptrdiff_t type_id)" << be_nl
      << "{" << be_idt_nl
      << "return this->" << node->flat_name ()
      << "_tao_obv_narrow (type_id);" << be_uidt_nl
      << "}" << be_nl
      << "#endif /* _MSC_VER */" << be_uidt_nl << be_nl;

  if (be_global->any_support ())
    {
      *os << "void" << be_nl
          << node->name () 
          << "::_tao_any_destructor (void *_tao_void_pointer)" << be_nl
          << "{" << be_idt_nl
          << node->local_name () << " *tmp =" << be_idt_nl
          << "ACE_static_cast (" << be_idt << be_idt_nl
          << node->local_name () << " *," << be_nl
          << "_tao_void_pointer" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "CORBA::remove_ref (tmp);" << be_uidt_nl
          << "}" << be_nl << be_nl;
    }


  // Nothing to marshal if abstract valuetype.
  if (!node->is_abstract () && !is_amh_exception_holder (node))
    {
      // The virtual _tao_marshal_v method.
      *os << "CORBA::Boolean " << node->name ()
          << "::_tao_marshal_v (TAO_OutputCDR & strm)"
          << be_nl
          << "{" << be_idt_nl
          << "return ";

      if (node->opt_accessor ())
        {
          be_decl *scope = 
            be_scope::narrow_from_scope (node->defined_in ())->decl ();

          *os << "ACE_NESTED_CLASS ("
              << scope->name () << ","
              << node->local_name () << ")"
              <<"::_tao_marshal_state (strm);" << be_uidt_nl;
        }
      else
        {
          *os << "this->_tao_marshal__" << node->flat_name ()
              << " (strm);" << be_uidt_nl;
        }

      *os << "}\n" << be_nl;

      // The virtual _tao_unmarshal_v method.
      *os << "CORBA::Boolean " << node->name ()
          << "::_tao_unmarshal_v (TAO_InputCDR & strm)"
          << be_nl
          << "{" << be_idt_nl
          << "return ";

      if (node->opt_accessor ())
        {
          be_decl *scope = 
            be_scope::narrow_from_scope (node->defined_in ())->decl ();

          *os << "ACE_NESTED_CLASS ("
              << scope->name () << ","
              << node->local_name () << ")"
              <<"::_tao_unmarshal_state (strm);" << be_uidt_nl;
        }
      else
        {
          *os << "this->_tao_unmarshal__" << node->flat_name ()
              << " (strm);" << be_uidt_nl;
        }

      *os << "}\n" << be_nl;
    }

  // The static T::_tao_unmarshal method

  *os << "CORBA::Boolean " << node->name()
      << "::_tao_unmarshal (" << be_idt << be_idt_nl
      << "TAO_InputCDR &strm," << be_nl
      << node->local_name () << " *&new_object" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "CORBA::ValueBase *base = 0;" << be_nl
      << "CORBA::ValueFactory_var factory;" << be_nl
      << "CORBA::Boolean retval =" << be_idt_nl
      << "CORBA::ValueBase::_tao_unmarshal_pre (" << be_idt << be_idt_nl
      << "strm," << be_nl
      << "factory.out ()," << be_nl
      << "base," << be_nl
      << node->local_name () << "::_tao_obv_static_repository_id ()" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl << be_nl
      << "if (retval == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "if (factory.in () != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "base = factory->create_for_unmarshal ();" << be_nl << be_nl
      << "if (base == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return 0;  // %! except.?" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "retval = base->_tao_unmarshal_v (strm);" << be_nl << be_nl
      << "if (retval == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl 
      << "}" << be_uidt_nl << be_nl
      << "// Now base must be null or point to the unmarshaled object." << be_nl
      << "// Align the pointer to the right subobject." << be_nl
      << "new_object = " << node->local_name () << "::_downcast (base);" << be_nl
      << "return retval;" << be_uidt_nl
      << "}";

  if (node->supports_abstract ())
    {
      *os << be_nl << be_nl
          << "CORBA::ValueBase *" << be_nl
          << node->name () << "::_tao_to_value (void)" << be_nl
          << "{" << be_idt_nl
          << "return this;" << be_uidt_nl
          << "}";
    }

  // Generate code for the elements of the valuetype.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cs::"
                         "visit_valuetype - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  // Generate the _init-related code.
  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_init_cs vi_visitor (&ctx);

  if (vi_visitor.visit_valuetype (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ch::"
                         "visit_valuetype - "
                         "failed to generate _init construct.\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype_cs::visit_operation (be_operation *node)
{
  if (node->cli_stub_gen () || node->imported ())
    {
      return 0;
    }

  be_valuetype *parent =
    be_valuetype::narrow_from_scope (node->defined_in ());

  if (parent == 0 || ! this->is_amh_exception_holder (parent))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // STEP I: Generate the return type.
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cs::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Grab the right visitor to generate the return type.
  be_visitor_context ctx (*this->ctx_);
  be_visitor_operation_rettype or_visitor (&ctx);

  if (bt->accept (&or_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cs::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  // STEP 2: Generate the operation name.
  *os << be_nl << parent->name () << "::" << node->local_name ();

  // STEP 3: Generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing.
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_CS);
  be_visitor_obv_operation_arglist ooa_visitor (&ctx);

  if (node->accept (&ooa_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  // We need to throw an exceptions that was assigned in a
  // different place (by the app-developer). ACE_THROW does
  // not fit the bill since the ACE_THROW macro contructs the
  // exception passed to it.  Also exception->_raise() is
  // ruled out since in platforms without native exception
  // support, the _raise() function does not do anything. Below we
  // explicitly take care of both cases (platforms with
  // and without native exception support).
  *os << be_nl
      << "{" << be_nl
      << "#if defined (TAO_HAS_EXCEPTIONS)" << be_idt_nl
      << "auto_ptr<CORBA::Exception> safety (this->exception);" << be_nl
      << "// Direct throw because we don't have the ACE_TRY_ENV." << be_nl
      << "this->exception->_raise ();" << be_uidt_nl
      << "#else" << be_idt_nl
      << "// We can not use ACE_THROW here." << be_nl
      << "ACE_TRY_ENV.exception (this->exception);" << be_uidt_nl
      << "#endif" << be_nl
      << "}" 
      << be_uidt_nl;

  return 0;
}

