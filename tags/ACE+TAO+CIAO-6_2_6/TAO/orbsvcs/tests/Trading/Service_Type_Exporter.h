// $Id$

// ========================================================================
//
// = FILE
//     Offer_Exporter.h
//
// = DESCRIPTION
//    Class that tests the Trading Service's Service Type Repository
//
// = AUTHOR
//     Seth Widoff <sbw1@cs.wustl.edu>
//
// =======================================================================

#ifndef TAO_SERVICE_TYPE_EXPORTER_H
#define TAO_SERVICE_TYPE_EXPORTER_H

#include "TT_Info.h"
#include "ttest_export.h"

class TAO_TTest_Export TAO_Service_Type_Exporter
{
public:

  TAO_Service_Type_Exporter (CosTrading::Lookup_ptr lookup_if,
                             CORBA::Boolean verbose);

  ~TAO_Service_Type_Exporter (void);

  void remove_all_types (void);
  // Remove all service types from the trading service instance.

  void add_all_types (void);
  // Add all the generated service types to the trading service instance.

  void add_all_types_to_all (void);
  // Add all the generated service types to all the trading service
  // instances linked to the trading service we bootstrapped to.

  void list_all_types (void);
  // List all the service types registered with the trading service
  // instance.

  void describe_all_types (void);
  // Describe all the service types registered with the trading
  // service instance.

  void fully_describe_all_types (void);
  // Same as describe_all_types, but fully describe lists the
  // properties of all service types the described service type
  // inherits from.

private:

  void create_types (void);
  // Generate the service type description structures.

  void add_all_types_to (CosTradingRepos::ServiceTypeRepository_ptr repos);
  // Add all types to a sepcified service type repository reference.

  void
  dump_typestruct (const char* type_name,
                   const CosTradingRepos::ServiceTypeRepository::TypeStruct& type_struct) const;
  // Dump the contents of a service type description to standard
  // output.

  CORBA::Boolean verbose_;
  // True if the user want profuse output.

  CosTrading::Lookup_ptr lookup_;
  // A reference to the lookup interface of the trading service
  // instance.

  CosTradingRepos::ServiceTypeRepository_var repos_;
  // A reference to a service type repository instance.

  CosTradingRepos::ServiceTypeRepository::TypeStruct type_structs_[NUM_TYPES];
  // Descriptions of all the service types whose property names are
  // defined in TT_Info.
};

#endif /* TAO_SERVICE_TYPE_EXPORTER_H */
