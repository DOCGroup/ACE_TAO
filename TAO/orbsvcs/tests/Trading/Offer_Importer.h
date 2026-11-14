#ifndef TAO_OFFER_IMPORTER
#define TAO_OFFER_IMPORTER

#include "TT_Info.h"
#include "ace/Task_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Trader/Trader_Utils.h"
#include "ttest_export.h"

/// Tests the Trading Service's Lookup interface.
class TAO_TTest_Export TAO_Offer_Importer
{
public:
  TAO_Offer_Importer (CosTrading::Lookup_ptr lookup_if,
                      CORBA::Boolean verbose = 1);

  /// Send a set of queries to the bootstrapped trader.
  void perform_queries ();

  /// Send a query to a trader two graph edges from the bootstrapped trader.
  void perform_directed_queries ();

private:
  /// Query the bootstrapped trader using the supplied policies.
  void perform_queries_with_policies (const TAO_Policy_Creator& policy_manager);

  /// Display query results.
  void display_results (const CosTrading::OfferSeq& offer_seq,
                        CosTrading::OfferIterator_ptr offer_iterator) const;

  /// Whether to display the contents of each returned offer.
  CORBA::Boolean verbose_;

  /// Lookup interface used for the queries.
  CosTrading::Lookup_ptr lookup_;
};

#endif /* TAO_OFFER_IMPORTER */
