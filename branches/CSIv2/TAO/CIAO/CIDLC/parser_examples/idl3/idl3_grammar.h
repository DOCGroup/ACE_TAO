// $Id$
//
// IDL3 Grammar checker implemented with Spirit (http://spirit.sourceforge.net/)
//
#ifndef IDL3_GRAMMAR_H
#define IDL3_GRAMMAR_H

#include "idl2/idl2_grammar.h"

struct idl3_grammar : public grammar<idl3_grammar>
{
  idl2_grammar idl2_g;
  
  template <typename ScannerT>
  struct definition
  {
    definition (idl3_grammar const & self);
    rule<ScannerT> const & start (void) const;
    
    idl2_grammar::definition<ScannerT> idl2;
    
    symbols<> keywords;
    
    rule<ScannerT>
      IDL_IMPORT, IDL_TYPEID, IDL_TYPEPREFIX, IDL_GETRAISES,
      IDL_SETRAISES, IDL_COMPONENT, IDL_PROVIDES, IDL_USES,
      IDL_MULTIPLE, IDL_EMITS, IDL_PUBLISHES, IDL_CONSUMES,
      IDL_HOME, IDL_MANAGES, IDL_PRIMARYKEY, IDL_FINDER, 
      IDL_EVENTTYPE;
      
    rule<ScannerT>
      specification,
      import, imported_scope, type_id_dcl, type_prefix_dcl,
      readonly_attr_spec, attr_spec, readonly_attr_declarator,
      attr_declarator, attr_raises_expr, get_excep_expr,
      set_excep_expr, exception_list, component, component_dcl,
      component_forward_dcl, component_header, component_body,
      component_inheritance_spec, component_export,
      supported_interface_spec, provides_dcl, uses_dcl, emits_dcl, 
      publishes_dcl, consumes_dcl, interface_type, home_dcl,
      home_header, home_body, home_inheritance_spec, home_export,
      primary_key_spec, factory_dcl, finder_dcl, event, event_dcl,
      event_abs_dcl, event_forward_dcl, event_header;
        
  };
};

#include "idl3_grammar.cpp"
  
#endif /* IDL3_GRAMMAR_H */

