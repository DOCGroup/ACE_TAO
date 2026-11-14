#ifndef TAO_SERVICE_TYPE_EXPORTER_H
#define TAO_SERVICE_TYPE_EXPORTER_H

#include "TT_Info.h"
#include "ttest_export.h"

/// Tests the Trading Service's Service Type Repository.
class TAO_TTest_Export TAO_Service_Type_Exporter
{
public:
  TAO_Service_Type_Exporter (CosTrading::Lookup_ptr lookup_if,
                             CORBA::Boolean verbose);

  ~TAO_Service_Type_Exporter ();

  /// Remove all service types from the trading service instance.
  void remove_all_types ();

  /// Add all generated service types to the trading service instance.
  void add_all_types ();

  /// Add generated service types to all linked trading service instances.
  void add_all_types_to_all ();

  /// List the service types registered with the trading service instance.
  void list_all_types ();

  /// Describe all service types registered with the trading service instance.
  void describe_all_types ();

  /// Describe each service type, including properties inherited from its bases.
  void fully_describe_all_types ();

private:
  /// Generate the service type description structures.
  void create_types ();

  /// Add all types to the specified service type repository.
  void add_all_types_to (CosTradingRepos::ServiceTypeRepository_ptr repos);

  /// Dump a service type description to standard output.
  void
  dump_typestruct (const char* type_name,
                   const CosTradingRepos::ServiceTypeRepository::TypeStruct& type_struct) const;

  /// Whether the user requested verbose output.
  CORBA::Boolean verbose_;

  /// Lookup interface for the trading service instance.
  CosTrading::Lookup_ptr lookup_;

  /// Service type repository instance.
  CosTradingRepos::ServiceTypeRepository_var repos_;

  /// Descriptions for service types whose property names are defined in TT_Info.
  CosTradingRepos::ServiceTypeRepository::TypeStruct type_structs_[NUM_TYPES];
};

#endif /* TAO_SERVICE_TYPE_EXPORTER_H */
