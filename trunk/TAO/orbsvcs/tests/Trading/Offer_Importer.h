// $Id$

// ========================================================================
//
// = FILE
//     Offer_Importer.h
//
// = DESCRIPTION
//    Class that tests the Trading Service's Lookup Interface
//
// = AUTHOR
//     Seth Widoff <sbw1@cs.wustl.edu>
//
// =======================================================================

#ifndef TAO_OFFER_IMPORTER
#define TAO_OFFER_IMPORTER

#include "TT_Info.h"
#include "ace/Task_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Trader/Trader_Utils.h"
#include "ttest_export.h"

class TAO_TTest_Export TAO_Offer_Importer
{
public:

  TAO_Offer_Importer (CosTrading::Lookup_ptr lookup_if,
                      CORBA::Boolean verbose = 1);

  void perform_queries (void);
  // Barrage the bootstrapped-to trader with queries.

  void perform_directed_queries (void);
  // Direct a query to a trader two graph edges distant from the
  // bootstrapped-to trader.

private:

  void perform_queries_with_policies (const TAO_Policy_Creator& policy_manager);
  // Perform a query on the bootstrapped-to trader given the policies
  // pass in <policy_manager>.

  void display_results (const CosTrading::OfferSeq& offer_seq,
                        CosTrading::OfferIterator_ptr offer_iterator) const;
  // Display the results of the query to the screen.

  CORBA::Boolean verbose_;
  // Output the contents of each returned offer, if true.

  CosTrading::Lookup_ptr lookup_;
  // The interface on which to perform all those wonderful queries.
};

#endif /* TAO_OFFER_IMPORTER */
