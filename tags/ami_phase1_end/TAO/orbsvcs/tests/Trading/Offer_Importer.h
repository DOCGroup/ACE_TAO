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

class TAO_Offer_Importer
{
public:

  TAO_Offer_Importer (CosTrading::Lookup_ptr lookup_if,
                      CORBA::Boolean verbose = 1);

  void perform_queries (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     CosTrading::IllegalServiceType,
                     CosTrading::UnknownServiceType,
                     CosTrading::IllegalConstraint,
                     CosTrading::Lookup::IllegalPreference,
                     CosTrading::Lookup::IllegalPolicyName,
                     CosTrading::Lookup::PolicyTypeMismatch,
                     CosTrading::Lookup::InvalidPolicyValue,
                     CosTrading::IllegalPropertyName,
                     CosTrading::DuplicatePropertyName,
                     CosTrading::DuplicatePolicyName));
  // Barrage the bootstrapped-to trader with queries.

  void perform_directed_queries (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     CosTrading::IllegalServiceType,
                     CosTrading::UnknownServiceType,
                     CosTrading::IllegalConstraint,
                     CosTrading::Lookup::IllegalPreference,
                     CosTrading::Lookup::IllegalPolicyName,
                     CosTrading::Lookup::PolicyTypeMismatch,
                     CosTrading::Lookup::InvalidPolicyValue,
                     CosTrading::IllegalPropertyName,
                     CosTrading::DuplicatePropertyName,
                     CosTrading::DuplicatePolicyName));
  // Direct a query to a trader two graph edges distant from the
  // bootstrapped-to trader.

private:

  void perform_queries_with_policies (const TAO_Policy_Creator& policy_manager,
                                      CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     CosTrading::IllegalServiceType,
                     CosTrading::UnknownServiceType,
                     CosTrading::IllegalConstraint,
                     CosTrading::Lookup::IllegalPreference,
                     CosTrading::Lookup::IllegalPolicyName,
                     CosTrading::Lookup::PolicyTypeMismatch,
                     CosTrading::Lookup::InvalidPolicyValue,
                     CosTrading::IllegalPropertyName,
                     CosTrading::DuplicatePropertyName,
                     CosTrading::DuplicatePolicyName));
  // Perform a query on the bootstrapped-to trader given the policies
  // pass in <policy_manager>.

  void display_results (const CosTrading::OfferSeq& offer_seq,
                        CosTrading::OfferIterator_ptr offer_iterator,
                        CORBA::Environment& _env) const
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Display the results of the query to the screen.

  CORBA::Boolean verbose_;
  // Output the contents of each returned offer, if true.

  CosTrading::Lookup_ptr lookup_;
  // The interface on which to perform all those wonderful queries.
};

#endif /* TAO_OFFER_IMPORTER */
