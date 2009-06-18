//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    component_init.cpp
//
// = DESCRIPTION
//    Visitor for generation of code for CIAO component attribute
//    initialization
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

be_visitor_attribute_component_init::be_visitor_attribute_component_init (
    be_visitor_context *ctx)
  : be_visitor_any_extracted_type_decl (ctx),
    attr_ (0)
{
}

be_visitor_attribute_component_init::~be_visitor_attribute_component_init (
  void)
{
}

int
be_visitor_attribute_component_init::visit_attribute (
  be_attribute *node)
{
  attr_ = node;
  be_type *ft = be_type::narrow_from_decl (node->field_type ());
  return ft->accept (this);
}

int
be_visitor_attribute_component_init::visit_array (be_array *)
{
  this->emit_error ("array");
  return 0;
}

int
be_visitor_attribute_component_init::visit_component (be_component *)
{
  this->emit_error ("component");
  return 0;
}

int
be_visitor_attribute_component_init::visit_enum (be_enum *)
{
  this->emit_init_block ();
  return 0;
}

int
be_visitor_attribute_component_init::visit_eventtype (be_eventtype *)
{
  this->emit_error ("eventtype");
  return 0;
}

int
be_visitor_attribute_component_init::visit_home (be_home *)
{
  this->emit_error ("home");
  return 0;
}

int
be_visitor_attribute_component_init::visit_interface (be_interface *)
{
  this->emit_error ("interface");
  return 0;
}

int
be_visitor_attribute_component_init::visit_predefined_type (be_predefined_type *)
{
  this->emit_init_block ();
  return 0;
}

int
be_visitor_attribute_component_init::visit_sequence (be_sequence *)
{
  this->emit_error ("sequence");
  return 0;
}

int
be_visitor_attribute_component_init::visit_string (
  be_string *node)
{
  if (node->max_size ()->ev ()->u.ulval == 0)
    {
      this->emit_init_block ();
    }
  else
    {
      this->emit_error ("bounded string");
    }
    
  return 0;
}

int
be_visitor_attribute_component_init::visit_structure (be_structure *)
{
  this->emit_error ("structure");
  return 0;
}

int
be_visitor_attribute_component_init::visit_typedef (
  be_typedef *node)
{
  return node->primitive_base_type ()->accept (this);
}

int
be_visitor_attribute_component_init::visit_union (be_union *)
{
  this->emit_error ("union");
  return 0;
}

int
be_visitor_attribute_component_init::visit_valuebox (be_valuebox *)
{
  this->emit_error ("valuebox");
  return 0;
}

int
be_visitor_attribute_component_init::visit_valuetype (be_valuetype *)
{
  this->emit_error ("valuetype");
  return 0;
}

void
be_visitor_attribute_component_init::emit_init_block (void)
{
  this->open_if_block ();
  
  be_visitor_any_extracted_type_decl decl_emitter (this->ctx_);
  be_type *ft = be_type::narrow_from_decl (attr_->field_type ());
  
  if (ft->accept (&decl_emitter) == -1)
    {
    }
  
  os_ << be_nl
      << "descr_value >>= _extract_val;" << be_nl
      << "this->" << attr_->local_name ()->get_string ()
      << " (";
      
  be_visitor_attribute_set_from_extracted arg_emitter (this->ctx_);
  
  if (ft->accept (&arg_emitter) == -1)
    {
    }
    
  os_ << ");";
  
  this->close_if_block ();
}

void
be_visitor_attribute_component_init::emit_error (
  const char *corba_kind)
{
  this->open_if_block ();
  
  os_ << "ACE_ERROR ((LM_ERROR," << be_nl
      << "            \"Component attributes of "
      << corba_kind << "\"" << be_nl
      << "            \"IDL type are not yet "
      << "supported by CIAO\\n\"));"
      << be_nl;
      
  this->close_if_block ();
}

void
be_visitor_attribute_component_init::open_if_block (void)
{
  os_ << be_nl << be_nl
      << "if (ACE_OS::strcmp (descr_name, \""
      << attr_->local_name ()->get_string ()
      << "\") == 0)" << be_idt_nl
      << "{" << be_idt_nl;
}

void
be_visitor_attribute_component_init::close_if_block (void)
{
  os_ << be_nl
      << "continue;" << be_uidt_nl
      << "}" << be_uidt;
}
