//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    gen_unbounded_obj_sequence_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequence in the client header
//
// = AUTHOR
//    Michael Kircher
//
//    Modifications by Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_sequence, 
           gen_unbounded_obj_sequence_ch, 
           "$Id$")

int
be_visitor_sequence_ch::gen_unbounded_obj_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_ch::"
                         "visit_sequence - "
                         "Bad element type\n"), -1);
    }

  be_type *pt;

  if (bt->node_type () == AST_Decl::NT_typedef)
    {
      // Get the primitive base type of this typedef node.
      be_typedef *t = be_typedef::narrow_from_decl (bt);
      pt = t->primitive_base_type ();
    }
  else
    {
      pt = bt;
    }

  // Generate the class name.
  const char *class_name = node->instance_name ();

  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_SEQUENCE_BASE_CH);
  be_visitor_sequence_base visitor (&ctx);

  *os << be_nl << "// TAO_IDL - Generated from "
      << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  os->gen_ifdef_AHETI();
  os->gen_ifdef_macro (class_name);

  *os << "class TAO_EXPORT_MACRO " << class_name << be_idt_nl
      << ": public TAO_Unbounded_Base_Sequence" << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "// = Initialization and termination methods." << be_nl;

  // default constructor
  *os << class_name << " (void);" << be_nl;

  // constructor
  *os << class_name << " (CORBA::ULong maximum);" << be_nl;

  // constructor
  *os << class_name << " (" << be_idt << be_idt_nl
      << "CORBA::ULong maximum," << be_nl
      << "CORBA::ULong length," << be_nl;

  bt->accept (&visitor);

  *os <<"* *value," << be_nl
      << "CORBA::Boolean release = 0" << be_uidt_nl
      << ");" << be_uidt_nl;

  // constructor
  *os << class_name << " (" << be_idt << be_idt_nl
      << "const " << class_name << " &rhs" << be_uidt_nl
      << ");" << be_uidt_nl;

  // operator=
  *os << class_name << " &operator= (" << be_idt << be_idt_nl
      << "const " << class_name << " &rhs" << be_uidt_nl
      << ");" << be_uidt_nl;

  // destructor
  *os << "virtual ~" << class_name << " (void);" << be_nl << be_nl;

  // Accessors
  *os << "// = Accessors." << be_nl;

  // operator[]
  be_predefined_type *prim = be_predefined_type::narrow_from_decl (pt);
  int is_pseudo_object =
    pt->node_type () == AST_Decl::NT_pre_defined
    && prim && prim->pt () == AST_PredefinedType::PT_pseudo
    && ACE_OS::strcmp (prim->local_name ()->get_string (),
                       "Object") != 0;
  int is_valuetype = 0;

  if (is_pseudo_object)
    {
      *os << "TAO_Pseudo_Object_Manager<";
    }
  else
    {
      be_interface *bf = be_interface::narrow_from_decl (pt);

      if (bf != 0)
        {
          is_valuetype = bf->is_valuetype ();
        }
      else
        {
          be_interface_fwd *bff = be_interface_fwd::narrow_from_decl (pt);

          if (bff != 0)
            {
              is_valuetype = bff->is_valuetype ();
            }
        }

      if (is_valuetype)
        {
          *os << "TAO_Valuetype_Manager<";
        }
      else
        {
          *os << "TAO_Object_Manager<";
        }
    }

  *os << bt->name () << ","
      << bt->name () << "_var>"
      << " operator[] (CORBA::ULong index) const;" << be_nl << be_nl;

  // Static operations
  *os << "// = Static operations." << be_nl;

  // allocbuf
  *os << "static ";

  bt->accept (&visitor);

  *os << " **allocbuf (CORBA::ULong nelems);" << be_nl;

  // freebuf
  *os << "static void freebuf (";

  bt->accept (&visitor);

  *os << " **buffer);" << be_nl << be_nl;

  // Implement the TAO_Base_Sequence methods (see Sequence.h)
  *os << "// Implement the TAO_Base_Sequence methods (see Sequence.h)" << be_nl;

  // allocate_buffer
  *os << "virtual void _allocate_buffer (CORBA::ULong length);" << be_nl;

  // deallocate_buffer
  *os << "virtual void _deallocate_buffer (void);" << be_nl;

  // get_buffer
  bt->accept (&visitor);

  *os << "* *get_buffer (CORBA::Boolean orphan = 0);" << be_nl;

  // get_buffer
  *os << "const ";

  bt->accept (&visitor);

  *os << "* *get_buffer (void) const;" << be_nl;

  // shrink_buffer
  *os << "virtual void _shrink_buffer (" << be_idt << be_idt_nl
      << "CORBA::ULong nl," << be_nl
      << "CORBA::ULong ol" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  if (! (is_pseudo_object || is_valuetype))
    {
      // Pseudo objects do not require these methods.
      *os << "virtual void _downcast (" << be_idt << be_idt_nl
                << "void* target," << be_nl
                << "CORBA_Object *src" << be_nl
                << "ACE_ENV_ARG_DECL_WITH_DEFAULTS"  << be_uidt_nl
                << ");" << be_uidt_nl;

      *os << "virtual CORBA_Object* _upcast (void *src) const;";
    }

  *os << be_uidt_nl << "};" << be_nl;

  os->gen_endif (); // endif macro

  // generate #endif for AHETI
  os->gen_endif_AHETI();

  return 0;
}
