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

class TAO_Service_Type_Exporter
{
public:

  typedef CosTradingRepos::ServiceTypeRepository SERVICE_TYPE_REPOS;
  
  TAO_Service_Type_Exporter (CosTradingRepos::ServiceTypeRepository_ptr str);

  void remove_all_types (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException,
		     CosTrading::IllegalServiceType, 
		     CosTrading::UnknownServiceType, 
		     SERVICE_TYPE_REPOS::HasSubTypes));
  
  void add_all_types (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException,
		     CosTrading::IllegalServiceType, 
		     SERVICE_TYPE_REPOS::ServiceTypeExists, 
		     SERVICE_TYPE_REPOS::InterfaceTypeMismatch, 
		     CosTrading::IllegalPropertyName, 
		     CosTrading::DuplicatePropertyName, 
		     SERVICE_TYPE_REPOS::ValueTypeRedefinition, 
		     CosTrading::UnknownServiceType, 
		     SERVICE_TYPE_REPOS::DuplicateServiceTypeName));

  void list_all_types (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  void describe_all_types (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::IllegalServiceType, 
		   CosTrading::UnknownServiceType));

  void fully_describe_all_types (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException, 
		     CosTrading::IllegalServiceType, 
		     CosTrading::UnknownServiceType));
  
private:

  void create_types (void);

  void dump_typestruct (const char* typename,
			const SERVICE_TYPE_REPOS::TypeStruct& type_struct) const;
  
  CosTradingRepos::ServiceTypeRepository_var repos_;
  SERVICE_TYPE_REPOS::TypeStruct type_structs_[NUM_TYPES];
};

#endif /* TAO_SERVICE_TYPE_EXPORTER_H */
