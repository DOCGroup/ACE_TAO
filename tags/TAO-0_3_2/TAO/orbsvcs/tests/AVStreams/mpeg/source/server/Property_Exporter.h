/* -*- C++ -*- */

// ========================================================================
// $Id$
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs
//
// = FILENAME
//    Property_Exporter.h
//
// = DESCRIPTION
//    Allows a service to export properties to both a Property Set and
//    a Trading Service simulatenously.
//
// = AUTHORS
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#ifndef TAO_PROPERTY_EXPORTER_H
#define TAO_PROPERTY_EXPORTER_H

#include "orbsvcs/CosTradingC.h"
#include "orbsvcs/CosPropertyServiceC.h"
#include "orbsvcs/Trader/Trader_Utils.h"

class TAO_Property_Exporter;

class TAO_Exportable
// = DESCRIPTION
//   Interface that allows property containers to export their
//   properties to a TAO_Property_Exporter instance.
{
public:

  virtual void
    export_properties (TAO_Property_Exporter& prop_exporter) {}
  // Insert exportable properties into the proper_exporter.

  virtual int define_properties
    (CosTradingRepos::ServiceTypeRepository::PropStructSeq& prop_seq,
     CORBA::ULong offset = 0) const = 0;
  // Append property definitions to the service type defintition
  // sequence. The <offset> is the point at which the method can begin
  // placing the PropStructures. define_properties returns the number
  // of properties inserted from the offset.
};


class TAO_Property_Exporter
// = TITLE
//   Publish public properties of a servant to a
//   CosPropertyService::PropSet instance and a Trading Service
//   offer.
//
//   Use this class in the following way:
//   1. Instantiate.
//   2. for (int i = 0; i < num_properties; i++)  add_*_property ();
//   3. export ()
{
 public:

  TAO_Property_Exporter (CosTrading::Lookup_var lookup_if,
                         CosPropertyService::PropertySet_ptr prop_set,
                         CORBA::ULong initial_size = 10);


  ~TAO_Property_Exporter (void);
  // Delete all Dynamic Property adapters

  void add_static_property (const char* name,
                            const CORBA::Any& value);
  // Add a property to the Offer and the PropSet.

  void add_dynamic_property (const char* name,
                             const CORBA::Any& intial_value,
                             TAO_Dynamic_Property& dp);
  // Add a property to the PropSet and a dynamic property to the
  // Offer. Have the dynamic property connect to the PropSet accessor
  // for that name.

  void add_dynamic_property (const char* name,
                             CosTradingDynamic::DynamicProp& dp_struct);

  void add_dynamic_property (const char* name,
                             CosTradingDynamic::DynamicProp* dp_struct);

  CosTrading::OfferId export (const CORBA::Object_ptr object_ref,
                              const CosTrading::ServiceTypeName type,
                              CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     CosPropertyService::MultipleExceptions,
                     CosTrading::Register::InvalidObjectRef,
                     CosTrading::IllegalServiceType,
                     CosTrading::UnknownServiceType,
                     CosTrading::Register::InterfaceTypeMismatch,
                     CosTrading::IllegalPropertyName,
                     CosTrading::PropertyTypeMismatch,
                     CosTrading::ReadonlyDynamicProperty,
                     CosTrading::MissingMandatoryProperty,
                     CosTrading::DuplicatePropertyName));
  // Export the offer to the trader under the given type.

  typedef CosTradingRepos::ServiceTypeRepository TRADING_REPOS;
  CosTrading::OfferId export (const CORBA::Object_ptr object_ref,
                              const CosTrading::ServiceTypeName type,
                              const TRADING_REPOS::PropStructSeq& props,
                              const TRADING_REPOS::ServiceTypeNameSeq& stypes,
                              CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     CosPropertyService::MultipleExceptions,
                     CosTrading::IllegalServiceType,
                     TRADING_REPOS::ServiceTypeExists,
                     TRADING_REPOS::InterfaceTypeMismatch,
                     CosTrading::IllegalPropertyName,
                     CosTrading::DuplicatePropertyName,
                     TRADING_REPOS::ValueTypeRedefinition,
                     TRADING_REPOS::DuplicateServiceTypeName,
                     CosTrading::Register::InvalidObjectRef,
                     CosTrading::Register::InterfaceTypeMismatch,
                     CosTrading::ReadonlyDynamicProperty,
                     CosTrading::MissingMandatoryProperty,
                     CosTrading::DuplicatePropertyName));
  // Export the offer to the trader under the given type. Register the
  // type with the current definition if the type can't be found in
  // the service type repository.

private:

  TAO_Property_Exporter (const TAO_Property_Exporter&);
  void operator= (const TAO_Property_Exporter&);
  // These don't make sense on this class.

  const int increment_;

  CosTrading::Lookup_var lookup_;
  CosPropertyService::PropertySet_var prop_set_;

  CORBA::ULong pcount_, tcount_;
  CosTrading::PropertySeq tprops_;
  CosPropertyService::Properties pprops_;
};

class TAO_Property_Modifier
// = TITLE
//   Modify or delete properties published in a PropSet instance or as
//   an offer in a Trading Service instance.
//
// = DESCRIPTION
//   This interface is primarily for properties registered as static
//   with the trading service, yet periodically change. The change
//   isn't frequent enough to warrant the constant overhead of dynamic
//   properties, which can affect the server performance.
{
public:

  TAO_Property_Modifier (CosTrading::Lookup_var lookup_if,
                         CosPropertyService::PropertySet_ptr prop_set,
                         CORBA::ULong initial_size = 10);

  // If no Lookup Interface is provided to the constructor, then
  // resolve_initial_references ("TradingService") will be used.

  void delete_property (const char* name);
  // Remove a property from the current prop_set/offer.

  void modify_property (const char* name, const CORBA::Any& value);
  // Modify a property from the current prop_set/offer.

  void commit (CosTrading::OfferId id, CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     CosPropertyService::MultipleExceptions,
                     CosTrading::NotImplemented,
                     CosTrading::IllegalOfferId,
                     CosTrading::UnknownOfferId,
                     CosTrading::Register::ProxyOfferId,
                     CosTrading::IllegalPropertyName,
                     CosTrading::Register::UnknownPropertyName,
                     CosTrading::PropertyTypeMismatch,
                     CosTrading::ReadonlyDynamicProperty,
                     CosTrading::Register::MandatoryProperty,
                     CosTrading::Register::ReadonlyProperty,
                     CosTrading::DuplicatePropertyName));
  // Commit your changes to the trading service.

 private:

  const int increment_;

  int pdcount_;
  int pmcount_;

  int tdcount_;
  int tmcount_;

  CosTrading::Lookup_var lookup_;
  CosPropertyService::PropertySet_var prop_set_;

  CosPropertyService::PropertyNames pdelete_;
  CosPropertyService::Properties pmodify_;

  CosTrading::PropertyNameSeq tdelete_;
  CosTrading::PropertySeq tmodify_;

  TAO_Property_Modifier (const TAO_Property_Modifier&);
  void operator= (const TAO_Property_Modifier&);
  // These don't make sense on this class.
};

#endif /* TAO_PROPERTY_EXPORTER_H */
