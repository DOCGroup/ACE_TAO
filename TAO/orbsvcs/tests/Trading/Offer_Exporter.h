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
#include "orbsvcs/Trader/Dynamic_Property.h"

class TAO_Offer_Exporter
{
public:

  TAO_Offer_Exporter (CosTrading::Lookup_ptr lookup_if,
		      CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  ~TAO_Offer_Exporter (void);
  
  void export_offers (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException, 
		     CosTrading::Register::InvalidObjectRef, 
		     CosTrading::IllegalServiceType, 
		     CosTrading::UnknownServiceType, 
		     CosTrading::Register::InterfaceTypeMismatch, 
		     CosTrading::IllegalPropertyName, 
		     CosTrading::PropertyTypeMismatch, 
		     CosTrading::ReadonlyDynamicProperty, 
		     CosTrading::MissingMandatoryProperty, 
		     CosTrading::DuplicatePropertyName));

  void export_offers_to_all (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException, 
		     CosTrading::Register::InvalidObjectRef, 
		     CosTrading::IllegalServiceType, 
		     CosTrading::UnknownServiceType, 
		     CosTrading::Register::InterfaceTypeMismatch, 
		     CosTrading::IllegalPropertyName, 
		     CosTrading::PropertyTypeMismatch, 
		     CosTrading::ReadonlyDynamicProperty, 
		     CosTrading::MissingMandatoryProperty, 
		     CosTrading::DuplicatePropertyName));

  
  void withdraw_offers (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException, 
		     CosTrading::IllegalOfferId, 
		     CosTrading::UnknownOfferId, 
		     CosTrading::Register::ProxyOfferId));

  void describe_offers (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException, 
		     CosTrading::IllegalOfferId, 
		     CosTrading::UnknownOfferId, 
		     CosTrading::Register::ProxyOfferId));

  void modify_offers (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException, 
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

  void withdraw_offers_using_constraints (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException, 
		     CosTrading::IllegalServiceType, 
		     CosTrading::UnknownServiceType, 
		     CosTrading::IllegalConstraint, 
		     CosTrading::Register::NoMatchingOffers));  
private:

  CosTrading::OfferIdSeq* grab_offerids (CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException,
		     CosTrading::NotImplemented));

  void create_offers (void);

  void export_to (CosTrading::Register_ptr reg,
                  CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException, 
		     CosTrading::Register::InvalidObjectRef, 
		     CosTrading::IllegalServiceType, 
		     CosTrading::UnknownServiceType, 
		     CosTrading::Register::InterfaceTypeMismatch, 
		     CosTrading::IllegalPropertyName, 
		     CosTrading::PropertyTypeMismatch, 
		     CosTrading::ReadonlyDynamicProperty, 
		     CosTrading::MissingMandatoryProperty, 
		     CosTrading::DuplicatePropertyName));
  
  CosTrading::Register_var register_;
  CosTrading::Admin_var admin_;
  
  TT_Info::Printer printer_[NUM_OFFERS];
  TT_Info::Plotter plotter_[NUM_OFFERS];
  TT_Info::File_System fs_[NUM_OFFERS];
  
  TAO_DP_Dispatcher dp_plotters_[NUM_OFFERS];
  TAO_DP_Dispatcher dp_printers_[NUM_OFFERS];
  TAO_DP_Dispatcher dp_fs_[NUM_OFFERS];
  
  CosTrading::PropertySeq props_plotters_[NUM_OFFERS];
  CosTrading::PropertySeq props_printers_[NUM_OFFERS];
  CosTrading::PropertySeq props_fs_[NUM_OFFERS];
};

#endif /* TAO_OFFER_EXPORTER_H */
