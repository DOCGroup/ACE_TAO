//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    smart_proxy_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for smart_proxy classes for an Interface in the
//    client proxy file.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, smart_proxy_cs, "$Id$")


// ************************************************************
//  be_visitor_interface_smart_proxy_cs
// ************************************************************

be_visitor_interface_smart_proxy_cs::be_visitor_interface_smart_proxy_cs
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_smart_proxy_cs::~be_visitor_interface_smart_proxy_cs (void)
{
}

int be_visitor_interface_smart_proxy_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  this->ctx_->node (node);

  os->indent ();

  // Its necessary to take care of the nested case. The smart proxy classes
  // are in the same scope as the proxy.
  be_decl* scope = be_scope::narrow_from_scope (node->defined_in ())->decl ();
  
  *os << scope->full_name ();

  // Only if there exists any nesting "::" is needed!
  if (node->is_nested ())
    *os << "::";
  
  *os <<"TAO_"<< node->flat_name () << "_Default_Proxy_Factory::";
  *os << "TAO_"
      << node->flat_name () << "_Default_Proxy_Factory (int register_proxy_factory)"
      << be_nl 
      << "{" << be_idt_nl
      << "if (register_proxy_factory)"<< be_idt_nl
      << "{" << be_idt_nl
      << "TAO_" << node->flat_name () 
      << "_PROXY_FACTORY_ADAPTER::instance ()->register_proxy_factory (this);"<< be_uidt_nl
      << "}" << be_uidt << be_uidt_nl 
      << "}\n\n";

  os->indent ();

  *os << scope->full_name ();
  
  // Only if there exists any nesting "::" is needed!
  if (node->is_nested ())
    *os << "::";
  
  *os << "TAO_"<< node->flat_name () 
      << "_Default_Proxy_Factory::~";
  *os <<"TAO_"
      << node->flat_name () << "_Default_Proxy_Factory (void)"
      << be_nl << be_uidt << be_uidt
      << "{" << be_nl
      << "}\n\n";

  os->indent ();
  *os << node->full_name () << "_ptr" << be_nl << be_uidt << be_uidt;
  *os << scope->full_name ();

  // Only if there exists any nesting "::" is needed!
  if (node->is_nested ())
    *os << "::";
  
  *os << "TAO_" << node->flat_name () 
      << "_Default_Proxy_Factory::create_proxy (" << be_idt << be_idt_nl 
      << "::" << node->full_name () 
      << "_ptr proxy," << be_nl 
      << "CORBA::Environment &" << be_uidt_nl 
      << ")" << be_uidt << be_uidt_nl
      << "{" << be_idt_nl
      << "return proxy;" << be_uidt << be_uidt_nl 
      << "}\n\n";

  os->indent ();
  *os << scope->full_name ();

  // Only if there exists any nesting "::" is needed!
  if (node->is_nested ())
    *os << "::";
  
  *os << "TAO_" << node->flat_name () << "_Proxy_Factory_Adapter::"; 
  *os << "TAO_"
      << node->flat_name () << "_Proxy_Factory_Adapter (void)" <<be_idt_nl
      << " : proxy_factory_ (0)," << be_idt_nl
      << " delete_proxy_factory_ (0)" << be_uidt << be_uidt_nl
      << "{"<< be_nl
      << "}\n\n";

  os->indent ();
 *os << scope->full_name ();

  // Only if there exists any nesting "::" is needed!
  if (node->is_nested ())
    *os << "::";
  
  *os << "TAO_" << node->flat_name () << "_Proxy_Factory_Adapter::"; 
  *os << "~TAO_"
      << node->flat_name () << "_Proxy_Factory_Adapter (void)" <<be_nl
      << "{" << be_idt_nl
      << "// Making sure the factory which the adapter has is destroyed with it."<<be_nl
      << "if (this->proxy_factory_ != 0)" <<be_idt_nl
      << "delete this->proxy_factory_;" << be_uidt<<be_uidt_nl
      << "}\n\n";

  os->indent ();
  *os << "int" << be_nl;
  *os << scope->full_name ();

  // Only if there exists any nesting "::" is needed!
  if (node->is_nested ())
    *os << "::";  
  *os << "TAO_" << node->flat_name () 
      << "_Proxy_Factory_Adapter::register_proxy_factory (" << be_idt_nl
      << "TAO_" << node->flat_name ()
      << "_Default_Proxy_Factory *df," << be_idt_nl
      << " CORBA::Environment &ACE_TRY_ENV" << be_idt_nl
      << ")" << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_idt_nl
      << "ACE_MT (ACE_GUARD_RETURN (" 
      << "ACE_Recursive_Thread_Mutex, ace_mon," << be_idt_nl
      << "this->lock_, 0));" <<be_uidt_nl
      << "// Remove any existing <proxy_factory_> and replace with the new one."<<be_nl
      << "this->unregister_proxy_factory (ACE_TRY_ENV);" << be_nl
      << "this->proxy_factory_ = df;"<< be_nl
      << "this->delete_proxy_factory_ = 0;" << be_uidt << be_uidt_nl
      << "return 0;" << be_uidt << be_uidt_nl
      << "}\n\n";

  os->indent ();
  *os << "int" << be_nl;
  *os << scope->full_name ();

  // Only if there exists any nesting "::" is needed!
  if (node->is_nested ())
    *os << "::";  
 
  *os << "TAO_"<< node->flat_name ()
      << "_Proxy_Factory_Adapter::unregister_proxy_factory (" << be_idt_nl
      << "CORBA::Environment &" << be_idt_nl
      << ")" << be_uidt << be_uidt_nl
      << "{" << be_idt_nl
      << "ACE_MT (ACE_GUARD_RETURN (" 
      << "ACE_Recursive_Thread_Mutex, ace_mon," << be_idt_nl
      << "this->lock_, 0));" <<be_uidt_nl
      << "if ("
      << "this->delete_proxy_factory_ == 0 && this->proxy_factory_ != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "// Its necessary to set <delete_proxy_factory_> to 1 to make sure that it" <<be_nl
      << "// doesnt get into an infinite loop in <unregister_proxy_factory> as it is "<<be_nl
      << "// invoked in the destructor of the class too."<<be_nl
      << "this->delete_proxy_factory_ = 1;" << be_nl     
      << "delete "
      << "this->proxy_factory_;" << be_nl
      << "this->proxy_factory_ = 0;" << be_uidt_nl
      << "}" << be_uidt<<be_uidt_nl
      << "return 0;" << be_uidt << be_uidt_nl
      << "}\n\n";

  os->indent ();
  *os << node->full_name () << "_ptr" << be_nl;
  *os << scope->full_name ();

  // Only if there exists any nesting "::" is needed!
  if (node->is_nested ())
    *os << "::";  
 
  *os << "TAO_"<< node->flat_name ()
      << "_Proxy_Factory_Adapter::create_proxy (" << be_idt << be_idt_nl
      << "::" << node->full_name () 
      << "_ptr proxy," << be_nl
      << "CORBA::Environment &" << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl
      << "{" << be_idt_nl
      << "ACE_MT (ACE_GUARD_RETURN (" 
      << "ACE_Recursive_Thread_Mutex, ace_mon," << be_idt_nl
      << "this->lock_, 0));" <<be_uidt_nl
      << "// Verify that an <proxy_factory_> is available else make one."<<be_nl
      << "if (" 
      <<"this->proxy_factory_ == 0)" << be_idt_nl
      << "ACE_NEW_RETURN ("
      << "this->proxy_factory_," << be_idt <<be_idt_nl
      << "TAO_" << node->flat_name () 
      << "_Default_Proxy_Factory (1), "<< be_nl
      << " 0);" << be_uidt_nl << be_uidt_nl << be_uidt_nl
      << "return "
      << "this->proxy_factory_->create_proxy (proxy);"
      << be_uidt << be_uidt_nl
      << "}\n\n";
  
  os->indent ();
  *os << scope->full_name ();

  // Only if there exists any nesting "::" is needed!
  if (node->is_nested ())
    *os << "::";  
 
  *os <<  "TAO_" << node->flat_name () << "_Smart_Proxy_Base::";
  *os << "TAO_" 
      <<  node->flat_name () << "_Smart_Proxy_Base (" 
      <<  "::" << node->full_name () 
      << "_ptr proxy)" << be_nl
      <<  ": base_proxy_ (proxy)" << be_nl
      << "{" << be_nl
      << "}\n\n";

  os->indent ();
  *os << scope->full_name ();

  // Only if there exists any nesting "::" is needed!
  if (node->is_nested ())
    *os << "::";  
 
  *os <<  "TAO_" << node->flat_name () << "_Smart_Proxy_Base::";
  *os << "~TAO_" 
      <<  node->flat_name () << "_Smart_Proxy_Base (void)" <<be_nl
      << "{" << be_nl
      << "}\n\n";

  // Implement the <stubobj> method of the base class
  os->indent ();
  *os << "TAO_Stub *"<<be_nl;
  *os << scope->full_name ();

  // Only if there exists any nesting "::" is needed!
  if (node->is_nested ())
    *os << "::";  
  
  *os <<  "TAO_" << node->flat_name () << "_Smart_Proxy_Base::";
  *os << "_stubobj (void) const"<<be_nl
      << "{" << be_idt_nl
      << "return this->base_proxy_->_stubobj ();"<< be_uidt_nl
      << "}\n\n";


    if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_smart_proxy_cs::"
                         "visit_scope - "
                         "codegen for scope failed\n"),
                        -1);
    }
 
    os->indent ();
    *os << "#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) || \\"
        << be_idt_nl<<"defined (ACE_HAS_GNU_REPO)"<<be_uidt_nl
        << "template class ACE_Singleton<";
    *os << scope->full_name ();
    
    // Only if there exists any nesting "::" is needed!
    if (node->is_nested ())
      *os << "::";
    *os <<"TAO_" <<node->flat_name ()
        << "_Proxy_Factory_Adapter, ACE_SYNCH_RECURSIVE_MUTEX >;"<<be_nl
        << "#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)"
        << be_nl
        << "#pragma instantiate ACE_Singleton<";
     *os << scope->full_name ();

     // Only if there exists any nesting "::" is needed!
     if (node->is_nested ())
       *os << "::";
     *os << "TAO_"<<node->flat_name ()
        << "_Proxy_Factory_Adapter, ACE_SYNCH_RECURSIVE_MUTEX>"<<be_nl
        << "#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */"<<be_nl<<be_nl;

  return 0;
}
