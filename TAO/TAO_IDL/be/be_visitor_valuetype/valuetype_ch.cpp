//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes in the client header
//    (see C++ mapping OMG 20.17)
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>,
//    based on interface_ch.cpp from Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype, 
           valuetype_ch, 
           "$Id$")

// ******************************************************
// Valuetype visitor for client header
// ******************************************************

be_visitor_valuetype_ch::be_visitor_valuetype_ch (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_ch::~be_visitor_valuetype_ch (void)
{
}

int
be_visitor_valuetype_ch::visit_valuetype (be_valuetype *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  // This will be a no-op if it has alread by done by a forward
  // declaration.
  node->gen_var_out_seq_decls ();

  TAO_OutStream *os = this->ctx_->stream ();
  int status = 0;

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Now the valuetype definition itself.
  os->gen_ifdef_macro (node->flat_name ());

  // Now generate the class definition.
  *os << be_nl << be_nl << "class " << be_global->stub_export_macro ()
      << " " << node->local_name ();

  // Node valuetype inherits from other valuetypes (OMG 20.17.9)
  // (ordinary (not abstract) interfaces ignored).

  *os << be_idt_nl <<": " << be_idt;

  long i;  // loop index
  be_valuetype *inherited = 0;
  long n_inherits = node->n_inherits ();
  int inherits_eventtype = 0;

  if (n_inherits > 0)
    {
      for (i = 0; i < n_inherits; ++i)
        {
          inherited =
            be_valuetype::narrow_from_decl (node->inherits ()[i]);

          if (inherited->node_type () == AST_Decl::NT_eventtype)
            {
              inherits_eventtype = 1;
            }

          if (i > 0)
            {
              *os << "," << be_nl;
            }

          be_decl *scope = 0;

          if (inherited->is_nested ())
            {
              // Inherited node is used in the scope of "node" node.
              scope =
                be_scope::narrow_from_scope (node->defined_in ())->decl ();
            }

          // Dump the scoped name.
          *os << "public virtual ";
          *os << inherited->nested_type_name (scope);
        }  // end of for loop
    }

  /***********************************************************************
  ** This is where we diverge for an ExceptionHolder ValueType.
  ** This is how we proceed:
  ** 1) Identify it is an AMH_ExceptionHolder class.
  ** 2) Inherit from CORBA::DefaultValueBaseRef i.e. provide a CONCRETE
  **    implementation for this ValueType!  This is because the alternative
  **    design of deriving a concrete-exception-holder class that the IDL
  **    compiler again has to generate is superflous, unnecessary, more
  **    coe bloat and unnecessary information for the app-programmer.  The
  **    changes required for this (n the *C.h file) are:
  **      2.1) Generate the raise_method as non-abstract and provide a
  **           definition in place
  **      2.2) Generate a new constructor that takes in a 
               CORBA::Exception*
  **      2.3) Make the destructor public (instead of protected) 
  **      2.4) Generate a private CORBA::Exception* field.
  **      2.5) Generate the tao_marshal and tao_unmarshal methods as 
  **           non-abstarct. 
  **      2.6) Generate the right throw spec for the AMH ExceptionHolders
  ************************************************************************/

  /****************************************************************/
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

  if (is_an_amh_exception_holder)
    {
      if (n_inherits > 0)
        {
          *os << "," << be_nl;
        }

      *os << "public virtual CORBA::DefaultValueRefCountBase";
    }

  if (node->node_type () == AST_Decl::NT_eventtype)
    {
      if (inherits_eventtype == 0)
        {
          if (n_inherits > 0)
            {
              *os << "," << be_nl;
            }

          *os << "public virtual CORBA::ValueBase";
        }
    }
  else if (n_inherits == 0)
    {
      if (is_an_amh_exception_holder)
        {
          *os << "," << be_nl;
        }

      *os << "public virtual CORBA::ValueBase";
    }

  if (node->supports_abstract ())
    {
      status =
        node->traverse_supports_list_graphs (
            be_valuetype::abstract_supports_helper,
            os,
            I_TRUE,
            I_FALSE
          );    

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_ch::"
                             "visit_valuetype - "
                             "traversal of supported interfaces failed\n"),
                            -1);
        }
    }

  // Generate the body.
  *os << be_uidt << be_uidt_nl 
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "typedef " << node->local_name () << "_var _var_type;" << be_nl;

  /***********************************************************************/
  // 2.2, 2.3
  if (is_an_amh_exception_holder)
    {
      // Generate the constructor
      *os << node->local_name () << " (CORBA::Exception *ex)" << be_nl
          << "{ this->exception = ex; }" << be_nl << be_nl;
      // and the destructor
      *os << "virtual ~" << node->local_name () << " (void);" 
          << be_nl << be_nl;
    }
  /***********************************************************************/

  // Generate the static _downcast operation.
  // (see OMG 20.17.{4,5}).
  *os << "static " << node->local_name () << "* "
      << "_downcast (CORBA::ValueBase* );" << be_nl
      << "// The address of static _downcast is implicit used as type id\n"
      << be_nl
      << "// (TAO extensions or internals)" << be_nl
      << "static CORBA::Boolean _tao_unmarshal (" << be_idt << be_idt_nl
      << "TAO_InputCDR &," << be_nl
      << node->local_name () << " *&" << be_uidt_nl
      << ");" << be_uidt_nl
      << "virtual const char* "
      << "_tao_obv_repository_id (void) const;"
      << be_nl
      << "static const char* "
      << "_tao_obv_static_repository_id (void);" << be_nl << be_nl;

  if (be_global->any_support ())
    {
      *os << "static void _tao_any_destructor (void *);";
    }

  // Generate code for the valuetype definition.
  if (this->visit_valuetype_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ch::"
                         "visit_valuetype - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // Generate pure virtual declarations of the operations in our
  // supported interfaces.
  status =
    node->traverse_supports_list_graphs (
        be_visitor_valuetype_ch::gen_supported_ops,
        os,
        I_FALSE,
        I_TRUE
      );

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ch::"
                         "visit_valuetype - "
                         "traversal of supported interfaces failed\n"),
                        -1);
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // If we inherit from both CORBA::ValueBase and CORBA::AbstractBase,
  // we have to add this to avoid ambiguity.
  if (node->supports_abstract ())
    {
      *os << be_nl << be_nl << "virtual void _add_ref (void) = 0;" << be_nl;
      *os << "virtual void _remove_ref (void) = 0;" << be_nl;
      *os << "virtual CORBA::ValueBase *_tao_to_value (void);";
    }

  // Generate the "protected" constructor so that users cannot
  // instantiate us.
  *os << be_uidt_nl << be_nl << "protected:" << be_idt_nl
      << node->local_name ()
      << " (void);" << be_nl;

  if (!is_an_amh_exception_holder)
    {
      *os << "virtual ~" << node->local_name () << " (void);" 
          << be_nl << be_nl;
    }

  // TAO internals.
  *os << "virtual void *_tao_obv_narrow (ptrdiff_t);"
      << "\n#if defined (_MSC_VER)" << be_nl
      << "virtual void *" << node->flat_name () 
      << "_tao_obv_narrow (ptrdiff_t);"
      << "\n#endif /* _MSC_VER */" << be_nl;

  // Support for marshalling.
  if (!node->is_abstract () && !is_an_amh_exception_holder)
    {
      *os << "virtual CORBA::Boolean "
          << "_tao_marshal_v (TAO_OutputCDR &);" << be_nl;
      *os << "virtual CORBA::Boolean "
          << "_tao_unmarshal_v (TAO_InputCDR &);" << be_nl;
    }

  if (is_an_amh_exception_holder)
    {
      *os << "// *** Terrible Hack ? ***" << be_nl;
      *os << "virtual CORBA::Boolean "
          << "_tao_marshal_v (TAO_OutputCDR &) { return 1; }" << be_nl;
      *os << "virtual CORBA::Boolean "
          << "_tao_unmarshal_v (TAO_InputCDR &) { return 1; }" << be_nl;
    }


  // Private member:

  // Private copy constructor and assignment operator. These are not
  // allowed, hence they are private.
  *os << be_uidt_nl << "private:" << be_idt_nl;
  *os << node->local_name () << " (const " << node->local_name () << " &);"
      << be_nl
      << "void operator= (const " << node->local_name () << " &);"
      << be_nl;
  
  /*********************************************************/
  // 2.4
  if (is_an_amh_exception_holder)
    {
      *os << "CORBA::Exception *exception;"
          << be_nl;
    }
  /*********************************************************/


  // Map fields to private data (if optimizing).
  if (node->opt_accessor ())
    {
      *os << be_uidt_nl << "protected:" << be_idt_nl;
      *os << "CORBA::Boolean "
          << "_tao_marshal_state (TAO_OutputCDR &);" << be_nl
          << "CORBA::Boolean "
          << "_tao_unmarshal_state (TAO_InputCDR &);"
          << be_uidt_nl << be_nl;
      *os << "private:" << be_idt_nl;

      this->gen_pd (node);
    }
  else // Need a way to access the state of derived OBV_ classes.
    {
      if (!node->is_abstract ())
        {
          *os << be_uidt_nl << "protected:" << be_idt_nl;
          /*********************************************************/
          // 2.5
          if (is_an_amh_exception_holder)
            {
              *os << "virtual CORBA::Boolean _tao_marshal__"
                  <<    node->flat_name () << " (TAO_OutputCDR &) {return 1;}"
                  << be_nl;
              *os << "virtual CORBA::Boolean _tao_unmarshal__"
                  <<    node->flat_name () << " (TAO_InputCDR &) {return 1;}";
            }          
          /*********************************************************/
          else
            {            
              *os << "virtual CORBA::Boolean _tao_marshal__"
                  <<    node->flat_name () << " (TAO_OutputCDR &) = 0;"
                  << be_nl;
              *os << "virtual CORBA::Boolean _tao_unmarshal__"
                  <<    node->flat_name () << " (TAO_InputCDR &) = 0;";
            }
        }
    }

  *os << be_uidt_nl << "};";

  os->gen_endif ();

  // Generate the _init -related declarations.
  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_init_ch visitor (&ctx);

  if (visitor.visit_valuetype (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ch::"
                         "visit_valuetype - "
                         "failed to generate _init construct.\n"),  
                        -1);
    }

  // Step last: generate typecode declaration.
  if (be_global->tc_support ())
    {
      be_visitor_context ctx (*this->ctx_);
      be_visitor_typecode_decl visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_ch::"
                             "visit_structure - "
                             "TypeCode declaration failed\n"),
                            -1);
        }
    }

  node->cli_hdr_gen (I_TRUE);

  return 0;
}


int
be_visitor_valuetype_ch::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Every operation is declared virtual in the client code.
  *os << "virtual ";

  // STEP I: Generate the return type.
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ch::"
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
                         "(%N:%l) be_visitor_valuetype_ch::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  // STEP 2: Generate the operation name.
  *os << " " << node->local_name ();

  // STEP 3: Generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing.
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_CH);
  be_visitor_obv_operation_arglist ooa_visitor (&ctx);

  if (node->accept (&ooa_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ch::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype_ch::visit_field (be_field *node)
{
  be_valuetype *vt = be_valuetype::narrow_from_scope (node->defined_in ());

  if (!vt)
    {
      return -1;
    }

  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_field_ch visitor (&ctx);

  if (vt->opt_accessor ())
    {
      visitor.setenclosings ("",";");
    }
  else
    {
      visitor.setenclosings ("virtual "," = 0;");
    }

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_obv_ch::"
                         "visit_field - codegen failed\n"),
                        -1);
    }

  return 0;
}


void
be_visitor_valuetype_ch::begin_public (void)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "public:" << be_idt_nl;
}

void
be_visitor_valuetype_ch::begin_private (void)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_uidt_nl << be_nl << "protected:" << be_idt;
}

int 
be_visitor_valuetype_ch::gen_supported_ops (be_interface *,
                                            be_interface *base,
                                            TAO_OutStream *os)
{
  // We inherit from abstract supported interfaces, so no need
  // to declare their pure virtual operations again in our scope.
  /*if (base->is_abstract ())
    {
      return 0;
    }
   */

  AST_Decl *d = 0;
  be_visitor_context ctx;
  ctx.stream (os);

  for (UTL_ScopeActiveIterator si (base, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_ch::"
                             "gen_supported_ops - "
                             "bad node in this scope\n"),
                            -1);
        }

      AST_Decl::NodeType nt = d->node_type ();
      be_visitor_valuetype_ch visitor (&ctx);

      if (nt == AST_Decl::NT_op)
        {
          be_operation *op = be_operation::narrow_from_decl (d);

          if (visitor.visit_operation (op) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_valuetype_ch::"
                                 "gen_supported_ops - "
                                 "failed to accept visitor\n"),
                                -1);
            }
        }
    }

  return 0;
}

