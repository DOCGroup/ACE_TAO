// $Id$

#include "idl3_grammar.h"

#ifndef IDL3_GRAMMAR_C
#define IDL3_GRAMMAR_C

template<typename ScannerT>
rule<ScannerT> const & 
idl3_grammar::definition<ScannerT>::start (void) const
{
  return specification;
}

template<typename ScannerT>
idl3_grammar::definition<ScannerT>::definition (
    idl3_grammar const & self
  )
  : idl2 (self.idl2_g)
{      
  keywords =
    "import", "typeid", "typeprefix", "getraises", "setraises",
    "component", "provides", "uses", "multiple", "emits",
    "publishes", "consumes", "home", "manages", "primarykey", 
    "finder", "eventtype";
    
  IDL_IMPORT =      strlit<>("import");
  IDL_TYPEID =      strlit<>("typeid");
  IDL_TYPEPREFIX =  strlit<>("typeprefix");
  IDL_GETRAISES =   strlit<>("getraises");
  IDL_SETRAISES =   strlit<>("setraises");
  IDL_COMPONENT =   strlit<>("component");
  IDL_PROVIDES =    strlit<>("provides");
  IDL_USES =        strlit<>("uses");
  IDL_MULTIPLE =    strlit<>("multiple");
  IDL_EMITS =       strlit<>("emits");
  IDL_PUBLISHES =   strlit<>("publishes");
  IDL_CONSUMES =    strlit<>("consumes");
  IDL_HOME =        strlit<>("home");
  IDL_MANAGES =     strlit<>("manages");
  IDL_PRIMARYKEY =  strlit<>("primarykey");
  IDL_FINDER =      strlit<>("finder");
  IDL_EVENTTYPE =   strlit<>("eventtype");
  
  idl2.IDENTIFIER =
    lexeme_d
      [
        ((alpha_p | '_') >> *(alnum_p | '_'))
        - (idl2.keywords >> anychar_p - (alnum_p | '_'))
        - (this->keywords >> anychar_p - (alnum_p | '_'))
      ]
    ;
    
  BOOST_SPIRIT_DEBUG_RULE (IDL_IMPORT);
  BOOST_SPIRIT_DEBUG_RULE (IDL_TYPEID);
  BOOST_SPIRIT_DEBUG_RULE (IDL_TYPEPREFIX);
  BOOST_SPIRIT_DEBUG_RULE (IDL_GETRAISES);
  BOOST_SPIRIT_DEBUG_RULE (IDL_SETRAISES);
  BOOST_SPIRIT_DEBUG_RULE (IDL_COMPONENT);
  BOOST_SPIRIT_DEBUG_RULE (IDL_PROVIDES);
  BOOST_SPIRIT_DEBUG_RULE (IDL_USES);
  BOOST_SPIRIT_DEBUG_RULE (IDL_MULTIPLE);
  BOOST_SPIRIT_DEBUG_RULE (IDL_EMITS);
  BOOST_SPIRIT_DEBUG_RULE (IDL_PUBLISHES);
  BOOST_SPIRIT_DEBUG_RULE (IDL_CONSUMES);
  BOOST_SPIRIT_DEBUG_RULE (IDL_HOME);
  BOOST_SPIRIT_DEBUG_RULE (IDL_MANAGES);
  BOOST_SPIRIT_DEBUG_RULE (IDL_PRIMARYKEY);
  BOOST_SPIRIT_DEBUG_RULE (IDL_FINDER);
  BOOST_SPIRIT_DEBUG_RULE (IDL_EVENTTYPE);
  
  BOOST_SPIRIT_DEBUG_RULE (specification);     
  BOOST_SPIRIT_DEBUG_RULE (import);     
  BOOST_SPIRIT_DEBUG_RULE (imported_scope);     
  BOOST_SPIRIT_DEBUG_RULE (type_id_dcl);     
  BOOST_SPIRIT_DEBUG_RULE (type_prefix_dcl);     
  BOOST_SPIRIT_DEBUG_RULE (readonly_attr_spec);
  BOOST_SPIRIT_DEBUG_RULE (attr_spec);
  BOOST_SPIRIT_DEBUG_RULE (readonly_attr_declarator);
  BOOST_SPIRIT_DEBUG_RULE (attr_declarator);
  BOOST_SPIRIT_DEBUG_RULE (attr_raises_expr);
  BOOST_SPIRIT_DEBUG_RULE (get_excep_expr);
  BOOST_SPIRIT_DEBUG_RULE (set_excep_expr);
  BOOST_SPIRIT_DEBUG_RULE (exception_list);
  BOOST_SPIRIT_DEBUG_RULE (component);
  BOOST_SPIRIT_DEBUG_RULE (component_dcl);
  BOOST_SPIRIT_DEBUG_RULE (component_forward_dcl);
  BOOST_SPIRIT_DEBUG_RULE (component_header);
  BOOST_SPIRIT_DEBUG_RULE (component_body);
  BOOST_SPIRIT_DEBUG_RULE (component_inheritance_spec);
  BOOST_SPIRIT_DEBUG_RULE (component_export);
  BOOST_SPIRIT_DEBUG_RULE (supported_interface_spec);
  BOOST_SPIRIT_DEBUG_RULE (component_export);
  BOOST_SPIRIT_DEBUG_RULE (provides_dcl);
  BOOST_SPIRIT_DEBUG_RULE (uses_dcl);
  BOOST_SPIRIT_DEBUG_RULE (emits_dcl);
  BOOST_SPIRIT_DEBUG_RULE (publishes_dcl);
  BOOST_SPIRIT_DEBUG_RULE (consumes_dcl);
  BOOST_SPIRIT_DEBUG_RULE (interface_type);
  BOOST_SPIRIT_DEBUG_RULE (home_dcl);
  BOOST_SPIRIT_DEBUG_RULE (home_header);
  BOOST_SPIRIT_DEBUG_RULE (home_body);
  BOOST_SPIRIT_DEBUG_RULE (home_inheritance_spec);
  BOOST_SPIRIT_DEBUG_RULE (home_export);
  BOOST_SPIRIT_DEBUG_RULE (primary_key_spec);
  BOOST_SPIRIT_DEBUG_RULE (factory_dcl);
  BOOST_SPIRIT_DEBUG_RULE (finder_dcl);
  BOOST_SPIRIT_DEBUG_RULE (event);
  BOOST_SPIRIT_DEBUG_RULE (event_dcl);
  BOOST_SPIRIT_DEBUG_RULE (event_abs_dcl);
  BOOST_SPIRIT_DEBUG_RULE (event_forward_dcl);
  BOOST_SPIRIT_DEBUG_RULE (event_header);

  specification
    = *import >> +idl2.declaration
    ;
    
  idl2.declaration
    = idl2.type_dcl >> idl2.SEMI
    | idl2.const_dcl >> idl2.SEMI
    | idl2.except_dcl >> idl2.SEMI
    | idl2.interface >> idl2.SEMI
    | idl2.module >> idl2.SEMI
    | idl2.value >> idl2.SEMI
    | type_id_dcl >> idl2.SEMI
    | type_prefix_dcl >> idl2.SEMI
    | event >> idl2.SEMI
    | component >> idl2.SEMI
    | home_dcl >> idl2.SEMI
    ;
    
  idl2.export
    = idl2.type_dcl >> idl2.SEMI
    | idl2.const_dcl >> idl2.SEMI
    | idl2.except_dcl >> idl2.SEMI
    | idl2.attr_dcl >> idl2.SEMI
    | idl2.op_dcl >> idl2.SEMI
    | type_id_dcl >> idl2.SEMI
    | type_prefix_dcl >> idl2.SEMI
    ;
  
  idl2.init_dcl
    = idl2.IDL_FACTORY >> idl2.IDENTIFIER >> idl2.LPAREN
      >> !idl2.init_param_decls >> idl2.RPAREN 
      >> !idl2.raises_expr >> idl2.SEMI
    ;
    
  idl2.attr_dcl
    = readonly_attr_spec
    | attr_spec
    ;
    
  import
    = IDL_IMPORT >> imported_scope >> idl2.SEMI
    ;
    
  imported_scope
    = idl2.scoped_name
    | idl2.STRING_LITERAL
    ;
    
  type_id_dcl
    = IDL_TYPEID >> idl2.scoped_name >> idl2.STRING_LITERAL
    ;
    
  type_prefix_dcl
    = IDL_TYPEPREFIX >> idl2.scoped_name >> idl2.STRING_LITERAL
    ;
    
  readonly_attr_spec
    = idl2.IDL_READONLY >> idl2.IDL_ATTRIBUTE 
      >> idl2.param_type_spec
      >> readonly_attr_declarator
    ;
    
  readonly_attr_declarator
    = idl2.simple_declarator >> idl2.raises_expr
    | idl2.simple_declarator 
      >> *(idl2.COMMA >> idl2.simple_declarator)
    ;
    
  attr_spec
    = idl2.IDL_ATTRIBUTE >> idl2.param_type_spec 
      >> attr_declarator
    ;
    
  attr_declarator
    = idl2.simple_declarator >> attr_raises_expr
    | idl2.simple_declarator 
      >> *(idl2.COMMA >> idl2.simple_declarator)
    ;
    
  attr_raises_expr
    = get_excep_expr >> !set_excep_expr
    | set_excep_expr
    ;
    
  get_excep_expr
    = IDL_GETRAISES >> exception_list
    ;
    
  set_excep_expr
    = IDL_SETRAISES >> exception_list
    ;
    
  exception_list
    = idl2.LPAREN >> idl2.scoped_name 
      >> *(idl2.COMMA >> idl2.scoped_name) >> idl2.RPAREN
    ;
    
  component
    = component_dcl
    | component_forward_dcl
    ;
    
  component_forward_dcl
    = IDL_COMPONENT >> idl2.IDENTIFIER
    ;
    
  component_dcl
    = component_header >> idl2.LBRACE 
      >> component_body >> idl2.RBRACE
    ;
    
  component_header
    = IDL_COMPONENT >> idl2.IDENTIFIER 
      >> !component_inheritance_spec
      >> !supported_interface_spec
    ;
    
  supported_interface_spec
    = idl2.IDL_SUPPORTS >> idl2.scoped_name 
      >> *(idl2.COMMA >> idl2.scoped_name)
    ;
    
  component_inheritance_spec
    = idl2.COLON >> idl2.scoped_name
    ;
    
  component_body
    = *component_export
    ;
    
  component_export
    = provides_dcl >> idl2.SEMI
    | uses_dcl >> idl2.SEMI
    | emits_dcl >> idl2.SEMI
    | publishes_dcl >> idl2.SEMI
    | consumes_dcl >> idl2.SEMI
    | idl2.attr_dcl >> idl2.SEMI
    ;
    
  provides_dcl
    = IDL_PROVIDES >> interface_type >> idl2.IDENTIFIER
    ;
    
  interface_type
    = idl2.scoped_name
    | idl2.IDL_OBJECT
    ;

  uses_dcl
    = IDL_USES >> !IDL_MULTIPLE 
      >> interface_type >> idl2.IDENTIFIER
    ;
    
  emits_dcl
    = IDL_EMITS >> idl2.scoped_name >> idl2.IDENTIFIER
    ;
    
  publishes_dcl
    = IDL_PUBLISHES >> idl2.scoped_name >> idl2.IDENTIFIER
    ;
    
  consumes_dcl
    = IDL_CONSUMES >> idl2.scoped_name >> idl2.IDENTIFIER
    ;
    
  home_dcl
    = home_header >> home_body
    ;
    
  home_header
    = IDL_HOME >> idl2.IDENTIFIER
      >> !home_inheritance_spec
      >> !supported_interface_spec
      >> IDL_MANAGES >> idl2.scoped_name
      >> !primary_key_spec
    ;
    
  home_inheritance_spec
    = idl2.COLON >> idl2.scoped_name
    ;
    
  primary_key_spec
    = IDL_PRIMARYKEY >> idl2.scoped_name
    ;
    
  home_body
    = idl2.LBRACE >> *home_export >> idl2.RBRACE
    ;
    
  home_export
    = idl2.export
    | factory_dcl >> idl2.SEMI
    | finder_dcl >> idl2.SEMI
    ;
    
  factory_dcl
    = idl2.IDL_FACTORY >> idl2.IDENTIFIER >> idl2.LPAREN
      >> !idl2.init_param_decls >>idl2.RPAREN
      >> !idl2.raises_expr
    ;
    
  finder_dcl
    = IDL_FINDER >> idl2.IDENTIFIER >> idl2.LPAREN
      >> !idl2.init_param_decls >>idl2.RPAREN
      >> !idl2.raises_expr
    ;
    
  event
    = event_dcl
    | event_abs_dcl
    | event_forward_dcl
    ;
  
  event_forward_dcl
    = !idl2.IDL_ABSTRACT >> IDL_EVENTTYPE >> idl2.IDENTIFIER
    ;
    
  event_abs_dcl
    = !idl2.IDL_ABSTRACT >> IDL_EVENTTYPE >> idl2.IDENTIFIER
      >> !idl2.value_inheritance_spec >> idl2.LBRACE
      >> *idl2.export >> idl2.RBRACE
    ;
    
  event_dcl
    = event_header >> idl2.LBRACE >> *idl2.value_element
      >> idl2.RBRACE
    ;
    
  event_header
    = !idl2.IDL_CUSTOM >> IDL_EVENTTYPE >> idl2.IDENTIFIER
      >> !idl2.value_inheritance_spec
    ;
}

#endif /* IDL3_GRAMMAR_C */

