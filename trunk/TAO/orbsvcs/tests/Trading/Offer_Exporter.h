// $Id$

// ========================================================================
//
// = FILE
//     Offer_Exporter.h
//
// = DESCRIPTION
//    Class that tests the Trading Service's Register Interface
//
// = AUTHOR
//     Seth Widoff <sbw1@cs.wustl.edu>
//
// =======================================================================

#ifndef TAO_OFFER_EXPORTER_H
#define TAO_OFFER_EXPORTER_H

#include "TT_Info.h"
#include "Simple_Dynamic.h"

class TAO_Offer_Exporter
{
public:

  TAO_Offer_Exporter (CosTrading::Lookup_ptr lookup_if,
                      CORBA::Boolean verbose
                      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  ~TAO_Offer_Exporter (void);

  void export_offers (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTrading::Register::InvalidObjectRef,
                     CosTrading::IllegalServiceType,
                     CosTrading::UnknownServiceType,
                     CosTrading::Register::InterfaceTypeMismatch,
                     CosTrading::IllegalPropertyName,
                     CosTrading::PropertyTypeMismatch,
                     CosTrading::ReadonlyDynamicProperty,
                     CosTrading::MissingMandatoryProperty,
                     CosTrading::DuplicatePropertyName));
  // Export a number of offers to the Trading Service.

  void export_offers_to_all (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTrading::Register::InvalidObjectRef,
                     CosTrading::IllegalServiceType,
                     CosTrading::UnknownServiceType,
                     CosTrading::Register::InterfaceTypeMismatch,
                     CosTrading::IllegalPropertyName,
                     CosTrading::PropertyTypeMismatch,
                     CosTrading::ReadonlyDynamicProperty,
                     CosTrading::MissingMandatoryProperty,
                     CosTrading::DuplicatePropertyName));
  // Export a number of offers to all traders accessible by the
  // bootstrapped trader.

  void withdraw_offers (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTrading::IllegalOfferId,
                     CosTrading::UnknownOfferId,
                     CosTrading::Register::ProxyOfferId));
  // Withdraw all exported offers.

  void describe_offers (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTrading::IllegalOfferId,
                     CosTrading::UnknownOfferId,
                     CosTrading::Register::ProxyOfferId));
  // Describe all the offers registered with the bootstrapped trader.

  void modify_offers (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
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
  // Remove some properties and change some properties in each offer.

  void withdraw_offers_using_constraints (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTrading::IllegalServiceType,
                     CosTrading::UnknownServiceType,
                     CosTrading::IllegalConstraint,
                     CosTrading::Register::NoMatchingOffers));
  // Withdraw a number of offers based on a constraint string.

private:

  CosTrading::OfferIdSeq* grab_offerids (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTrading::NotImplemented));

  void create_offers (TAO_ENV_SINGLE_ARG_DECL);
  // Fill in each of the offer structures.

  void export_to (CosTrading::Register_ptr reg
                  TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTrading::Register::InvalidObjectRef,
                     CosTrading::IllegalServiceType,
                     CosTrading::UnknownServiceType,
                     CosTrading::Register::InterfaceTypeMismatch,
                     CosTrading::IllegalPropertyName,
                     CosTrading::PropertyTypeMismatch,
                     CosTrading::ReadonlyDynamicProperty,
                     CosTrading::MissingMandatoryProperty,
                     CosTrading::DuplicatePropertyName));
  // Export the offers to the give Register interface.

  typedef ACE_Unbounded_Queue<TAO_Dynamic_Property*> DP_Queue;

  CORBA::Boolean verbose_;
  // True if the user wants verbose output.

  CosTrading::Register_var register_;
  CosTrading::Admin_var admin_;

  TT_Info::Printer printer_[NUM_OFFERS];
  TT_Info::Plotter plotter_[NUM_OFFERS];
  TT_Info::File_System fs_[NUM_OFFERS];
  // The objects being exported.

  CosTrading::PropertySeq props_plotters_[NUM_OFFERS];
  CosTrading::PropertySeq props_printers_[NUM_OFFERS];
  CosTrading::PropertySeq props_fs_[NUM_OFFERS];
  // Property sequences describing each service.

  DP_Queue clean_up_;
  // A list of dynamic properties to clean up upon destruction.
};

#endif /* TAO_OFFER_EXPORTER_H */
