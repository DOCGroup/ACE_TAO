// $Id$

#include "idl3/idl3_grammar.cpp"

#ifndef CIDL_GRAMMAR_C
#define CIDL_GRAMMAR_C

template<typename ScannerT>
rule<ScannerT> const & 
cidl_grammar::definition<ScannerT>::start (void) const
{
  return specification;
}

template<typename ScannerT>
cidl_grammar::definition<ScannerT>::definition (
    cidl_grammar const & self
  )
  : idl3 (self.idl3_g),
    DOT ('.')
{
  keywords =
    "composition", "entity", "process", "service", "session",
    "catalog", "executor", "implements", "bindsTo", "storedOn",
    "segment", "facet", "delegatesTo", "storage", "storagehome",
    "proxy";
    
  CIDL_COMPOSITION =  strlit<>("composition");
  CIDL_ENTITY =       strlit<>("entity");
  CIDL_PROCESS =      strlit<>("process");
  CIDL_SERVICE =      strlit<>("service");
  CIDL_SESSION =      strlit<>("session");
  CIDL_CATALOG =      strlit<>("catalog");
  CIDL_EXECUTOR =     strlit<>("executor");
  CIDL_IMPLEMENTS =   strlit<>("implements");
  CIDL_BINDS_TO =     strlit<>("bindsTo");
  CIDL_STORED_ON =    strlit<>("storedOn");
  CIDL_SEGMENT =      strlit<>("segment");
  CIDL_FACET =        strlit<>("facet");
  CIDL_DELEGATES_TO = strlit<>("delegatesTo");
  CIDL_STORAGE =      strlit<>("storage");
  CIDL_STORAGEHOME =  strlit<>("storagehome");
  CIDL_PROXY =        strlit<>("proxy");
  
  idl3.idl2.IDENTIFIER =
    lexeme_d
      [
        ((alpha_p | '_') >> *(alnum_p | '_'))
        - (idl3.idl2.keywords >> anychar_p - (alnum_p | '_'))
        - (idl3.keywords >> anychar_p - (alnum_p | '_'))
        - (this->keywords >> anychar_p - (alnum_p | '_'))
      ]
    ;
  
  BOOST_SPIRIT_DEBUG_RULE (CIDL_COMPOSITION);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_ENTITY);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_PROCESS);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_SERVICE);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_SESSION);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_CATALOG);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_EXECUTOR);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_IMPLEMENTS);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_BINDS_TO);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_STORED_ON);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_SEGMENT);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_FACET);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_DELEGATES_TO);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_STORAGE);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_STORAGEHOME);
  BOOST_SPIRIT_DEBUG_RULE (CIDL_PROXY);

  BOOST_SPIRIT_DEBUG_RULE (specification);
  BOOST_SPIRIT_DEBUG_RULE (composition);
  BOOST_SPIRIT_DEBUG_RULE (category);
  BOOST_SPIRIT_DEBUG_RULE (composition_body);
  BOOST_SPIRIT_DEBUG_RULE (catalog_use_dcl);
  BOOST_SPIRIT_DEBUG_RULE (home_executor_def);
  BOOST_SPIRIT_DEBUG_RULE (proxy_home_def);
  BOOST_SPIRIT_DEBUG_RULE (catalog_dcl);
  BOOST_SPIRIT_DEBUG_RULE (catalog_type_spec);
  BOOST_SPIRIT_DEBUG_RULE (catalog_label);
  BOOST_SPIRIT_DEBUG_RULE (home_executor_body);
  BOOST_SPIRIT_DEBUG_RULE (home_impl_dcl);
  BOOST_SPIRIT_DEBUG_RULE (abstract_storage_home_binding);
  BOOST_SPIRIT_DEBUG_RULE (stored_on_dcl);
  BOOST_SPIRIT_DEBUG_RULE (executor_def);
  BOOST_SPIRIT_DEBUG_RULE (abstract_storage_home_delegation_spec);
  BOOST_SPIRIT_DEBUG_RULE (executor_delegation_spec);
  BOOST_SPIRIT_DEBUG_RULE (absract_spec);
  BOOST_SPIRIT_DEBUG_RULE (home_type_name);
  BOOST_SPIRIT_DEBUG_RULE (abstract_storage_home_name);
  BOOST_SPIRIT_DEBUG_RULE (abstract_storage_hom_label);
  BOOST_SPIRIT_DEBUG_RULE (executor_body);
  BOOST_SPIRIT_DEBUG_RULE (executor_member);
  BOOST_SPIRIT_DEBUG_RULE (segment_def);
  BOOST_SPIRIT_DEBUG_RULE (feature_delegation_spec);
  BOOST_SPIRIT_DEBUG_RULE (segment_member);
  BOOST_SPIRIT_DEBUG_RULE (segment_persistence_dcl);
  BOOST_SPIRIT_DEBUG_RULE (facet_dcl);
  BOOST_SPIRIT_DEBUG_RULE (feature_delegation_list);
  BOOST_SPIRIT_DEBUG_RULE (feature_delegation);
  BOOST_SPIRIT_DEBUG_RULE (feature_name);
  BOOST_SPIRIT_DEBUG_RULE (storage_member_name);
  BOOST_SPIRIT_DEBUG_RULE (delegation_list);
  BOOST_SPIRIT_DEBUG_RULE (delegation);
  BOOST_SPIRIT_DEBUG_RULE (operation_name);
  BOOST_SPIRIT_DEBUG_RULE (operation_list);
  BOOST_SPIRIT_DEBUG_RULE (proxy_home_member);
  BOOST_SPIRIT_DEBUG_RULE (home_delegation_spec);
  
  specification
    = *idl3.import >> +idl3.idl2.declaration
    ;
    
  idl3.idl2.declaration
    = idl3.idl2.type_dcl >> idl3.idl2.SEMI
    | idl3.idl2.const_dcl >> idl3.idl2.SEMI
    | idl3.idl2.except_dcl >> idl3.idl2.SEMI
    | idl3.idl2.interface >> idl3.idl2.SEMI
    | idl3.idl2.module >> idl3.idl2.SEMI
    | idl3.idl2.value >> idl3.idl2.SEMI
    | idl3.type_id_dcl >> idl3.idl2.SEMI
    | idl3.type_prefix_dcl >> idl3.idl2.SEMI
    | idl3.event >> idl3.idl2.SEMI
    | idl3.component >> idl3.idl2.SEMI
    | idl3.home_dcl >> idl3.idl2.SEMI
    | composition >> idl3.idl2.SEMI
    ;
    
  composition
    = CIDL_COMPOSITION >> category >> idl3.idl2.IDENTIFIER
      >> idl3.idl2.LBRACE >> composition_body >> idl3.idl2.RBRACE
    ;
    
  category
    = CIDL_ENTITY
    | CIDL_PROCESS
    | CIDL_SERVICE
    | CIDL_SESSION
    ;
    
  composition_body
    = !catalog_use_dcl >> home_executor_def >> !proxy_home_def
    ;
    
  catalog_use_dcl
    = idl3.IDL_USES >> CIDL_CATALOG >> idl3.idl2.LBRACE
      >> +catalog_dcl >> idl3.idl2.RBRACE >> idl3.idl2.SEMI
    ;
 
  catalog_dcl
    = catalog_type_spec >> catalog_label
    ;
    
  catalog_type_spec
    = idl3.idl2.scoped_name
    ;

  catalog_label
    = idl3.idl2.IDENTIFIER
    ;
    
  home_executor_def
    = idl3.IDL_HOME >> CIDL_EXECUTOR >> idl3.idl2.IDENTIFIER
      >> idl3.idl2.LBRACE >> home_executor_body
      >> idl3.idl2.RBRACE >> idl3.idl2.SEMI
    ;
      
  home_executor_body
    = home_impl_dcl >> !abstract_storage_home_binding
      >> !stored_on_dcl >> executor_def
      >> !abstract_storage_home_delegation_spec
      >> !executor_delegation_spec >> !abstract_spec
    ;
  
  home_impl_dcl
    = CIDL_IMPLEMENTS >> home_type_name >> idl3.idl2.SEMI
    ;
    
  home_type_name
    = idl3.idl2.scoped_name
    ;

  abstract_storage_home_binding
    = CIDL_BINDS_TO >> abstract_storage_home_name
      >> idl3.idl2.SEMI
    ;
    
  abstract_storage_home_name
    = catalog_label >> DOT >> abstract_storage_home_label
    ;
    
  abstract_storage_home_label
    = idl3.idl2.IDENTIFIER
    ;
    
  home_persistence_dcl
    = CIDL_STORED_ON >> abstract_storage_home_name
      >> idl3.idl2.SEMI
    ;
    
  executor_def
    = idl3.IDL_MANAGES >> idl3.idl2.IDENTIFIER
      >> !executor_body >> idl3.idl2.SEMI
    ;
    
  executor_body
    = idl3.idl2.LBRACE >> +executor_member >> idl3.idl2.RBRACE
    ;
    
  executor_member
    = segment_def
    | feature_delegation_spec
    ;
    
  segment_def
    = CIDL_SEGMENT >> idl3.idl2.IDENTIFIER >> idl3.idl2.LBRACE
      >> +segment_member >> idl3.idl2.RBRACE
    ;
    
  segment_member
    = segment_persistence_dcl >> idl3.idl2.SEMI
    | facet_dcl >> idl3.idl2.SEMI
    ;
    
  segment_persistence_dcl
    = CIDL_STORED_ON >> abstract_storage_home_name
      >> idl3.idl2.SEMI
    ;
    
  facet_dcl
    = idl3.IDL_PROVIDES >> CIDL_FACET >> idl3.idl2.IDENTIFIER
      >> *(idl3.idl2.COMMA >> idl3.idl2.IDENTIFIER)
    ;
    
  feature_delegation_spec
    = CIDL_DELEGATES_TO >> CIDL_STORAGE 
      >> feature_delegation_list
    ;
    
  feature_delegation_list
    = idl3.idl2.LPAREN >> feature_delegation
      >> *(idl3.idl2.COMMA >> feature_delegation) 
      >> idl3.idl2.RPAREN
  ;
  
  feature_delegation
    = feature_name >> idl3.idl2.COLON >> storage_member_name
    ;
    
  feature_name
    = idl3.idl2.IDENTIFIER
    ;
    
  storage_member_name
    = idl3.idl2.IDENTIFIER
    ;
    
  abstract_storage_home_delegation_spec
    = CIDL_DELEGATES_TO >> idl3.idl2.IDL_ABSTRACT
      >> CIDL_STORAGEHOME >> delegation_list >> idl3.idl2.SEMI
    ;
    
  executor_delegation_spec
    = CIDL_DELEGATES_TO >> CIDL_EXECUTOR >> delegation_list
      >> idl3.idl2.SEMI
    ;
    
  delegation_list
    = idl3.idl2.LPAREN >> delegation
      >> *(idl3.idl2.COMMA >> delegation) >> idl3.idl2.RPAREN
    ;
    
  delegation
    = operation_name >> !(idl3.idl2.COLON >> operation_name)
    ;
    
  operation_name
    = idl3.idl2.IDENTIFIER
    ;
    
  abstract_spec
    = idl3.idl2.IDL_ABSTRACT >> operation_list
      >> idl3.idl2.SEMI
    ;
    
  operation_list
    = idl3.idl2.LPAREN >> operation_name
      >> *(idl3.idl2.COMMA >> operation_name) 
      >> idl3.idl2.RPAREN
    ;
    
  proxy_home_def
    = CIDL_PROXY >> idl3.IDL_HOME >> idl3.idl2.IDENTIFIER
      >> idl3.idl2.LBRACE >> +proxy_home_member
      >> idl3.idl2.RBRACE >> idl3.idl2.SEMI
    ;
    
  proxy_home_member
    = home_delegation_spec >> idl3.idl2.SEMI
    | abstract_spec
    ;
    
  home_delegation_spec
    = CIDL_DELEGATES_TO >> idl3.IDL_HOME >> delegation_list
    ;
}

#endif /* CIDL_GRAMMAR_C */
