// $Id$
//
// IDL3 Grammar checker implemented with Spirit (http://spirit.sourceforge.net/)
//
#ifndef CIDL_GRAMMAR_H
#define CIDL_GRAMMAR_H

#include "idl3/idl3_grammar.h"

struct cidl_grammar : public grammar<cidl_grammar>
{
  idl3_grammar idl3_g;
  
  template <typename ScannerT>
  struct definition
  {
    definition (cidl_grammar const & self);
    rule<ScannerT> const & start (void) const;
    
    idl3_grammar::definition<ScannerT> idl3;
    
    symbols<> keywords;
    
    chlit<>
      DOT;
    
    rule<ScannerT>
      CIDL_COMPOSITION, CIDL_ENTITY, CIDL_PROCESS, CIDL_SERVICE,
      CIDL_SESSION, CIDL_CATALOG, CIDL_EXECUTOR, CIDL_IMPLEMENTS,
      CIDL_BINDS_TO, CIDL_STORED_ON, CIDL_SEGMENT, CIDL_FACET,
      CIDL_DELEGATES_TO, CIDL_STORAGE, CIDL_STORAGEHOME, CIDL_PROXY;
    
    rule<ScannerT>
      specification, composition, category, composition_body,
      catalog_use_dcl, home_executor_def, proxy_home_def,
      catalog_dcl, catalog_type_spec, catalog_label,
      home_executor_body, home_impl_dcl, abstract_storage_home_binding,
      stored_on_dcl, executor_def, home_persistence_dcl,
      abstract_storage_home_delegation_spec, executor_delegation_spec,
      abstract_spec, home_type_name, abstract_storage_home_name,
      abstract_storage_home_label, executor_body, executor_member,
      segment_def, feature_delegation_spec, segment_member,
      segment_persistence_dcl, facet_dcl, feature_delegation_list,
      feature_delegation, feature_name, storage_member_name,
      delegation_list, delegation, operation_name, operation_list,
      proxy_home_member, home_delegation_spec;
  };
};     

#include "cidl_grammar.cpp"
  
#endif /* CIDL_GRAMMAR_H */

        
