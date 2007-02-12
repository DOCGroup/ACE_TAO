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
#include "ttest_export.h"

class TAO_TTest_Export TAO_Offer_Exporter
{
public:

  TAO_Offer_Exporter (CosTrading::Lookup_ptr lookup_if,
                      CORBA::Boolean verbose);

  ~TAO_Offer_Exporter (void);

  void export_offers (void);
  // Export a number of offers to the Trading Service.

  void export_offers_to_all (void);
  // Export a number of offers to all traders accessible by the
  // bootstrapped trader.

  void withdraw_offers (void);
  // Withdraw all exported offers.

  void describe_offers (void);
  // Describe all the offers registered with the bootstrapped trader.

  void modify_offers (void);
  // Remove some properties and change some properties in each offer.

  void withdraw_offers_using_constraints (void);
  // Withdraw a number of offers based on a constraint string.

private:

  CosTrading::OfferIdSeq* grab_offerids (void);

  void create_offers (void);
  // Fill in each of the offer structures.

  void export_to (CosTrading::Register_ptr reg);
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
