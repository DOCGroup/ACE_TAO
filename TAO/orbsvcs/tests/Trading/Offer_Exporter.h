#ifndef TAO_OFFER_EXPORTER_H
#define TAO_OFFER_EXPORTER_H

#include "TT_Info.h"
#include "Simple_Dynamic.h"
#include "ttest_export.h"

#include <memory>
#include <vector>

/// Tests the Trading Service's Register interface.
class TAO_TTest_Export TAO_Offer_Exporter
{
public:
  TAO_Offer_Exporter (CosTrading::Lookup_ptr lookup_if,
                      CORBA::Boolean verbose);

  ~TAO_Offer_Exporter ();
  TAO_Offer_Exporter (TAO_Offer_Exporter const &) = delete;
  TAO_Offer_Exporter &operator= (TAO_Offer_Exporter const &) = delete;
  TAO_Offer_Exporter (TAO_Offer_Exporter &&) = delete;
  TAO_Offer_Exporter &operator= (TAO_Offer_Exporter &&) = delete;

  /// Export offers to the Trading Service.
  void export_offers ();

  /// Export offers to all traders accessible from the bootstrapped trader.
  void export_offers_to_all ();

  /// Withdraw all exported offers.
  void withdraw_offers ();

  /// Describe all offers registered with the bootstrapped trader.
  void describe_offers ();

  /// Remove and change properties in each offer.
  void modify_offers ();

  /// Withdraw offers selected by a constraint string.
  void withdraw_offers_using_constraints ();

private:
  CosTrading::OfferIdSeq* grab_offerids ();

  /// Fill in each offer structure.
  void create_offers ();

  /// Export offers through the supplied Register interface.
  void export_to (CosTrading::Register_ptr reg);

  using DP_Queue = std::vector<std::unique_ptr<TAO_Dynamic_Property>>;

  /// Whether the user requested verbose output.
  CORBA::Boolean verbose_;

  CosTrading::Register_var register_;
  CosTrading::Admin_var admin_;

  /// Objects being exported.
  TT_Info::Printer printer_[NUM_OFFERS];
  TT_Info::Plotter plotter_[NUM_OFFERS];
  TT_Info::File_System fs_[NUM_OFFERS];

  /// Property sequences describing each service.
  CosTrading::PropertySeq props_plotters_[NUM_OFFERS];
  CosTrading::PropertySeq props_printers_[NUM_OFFERS];
  CosTrading::PropertySeq props_fs_[NUM_OFFERS];

  /// Dynamic properties reclaimed when this object is destroyed.
  DP_Queue clean_up_;
};

#endif /* TAO_OFFER_EXPORTER_H */
