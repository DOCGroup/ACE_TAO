// -*- C++ -*-
/**
 * @defgroup Importance_Bin_Packer Importance_Bin_Packer
 *
 * The Importance Bin Packer uses a simplified Kimchee bin-packing
 * algorithim to distribute applications across available computing
 * nodes.  Lower-importance application strings will be automatically
 * torn down to make room for higher-importance application string.
 *
 * @author Edward Mulholland <emulholl@atl.lmco.com>
 */
/**
 * @file
 * @ingroup Importance_Bin_Packer
 * @brief C++ implementation of the ImportanceBinPacker component.
 *
 * @copydoc Importance_Bin_Packer
 * $Id$
 */

#ifndef IMPORTANCEBINPACKER_H
#define IMPORTANCEBINPACKER_H

#include /**/ "ace/pre.h"
#include <iostream>
#include "Packaging_DataC.h"
#include "Planners/Interface/Planner_IC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

typedef ::CIAO::RACE::Plan_Actions Plan_Seq;
typedef ::CIAO::RACE::Plan_Action Plan_Struct;
typedef ::CIAO::RACE::Action Action_Enum;
#define Action_Types ::CIAO::RACE
typedef ::CIAO::RACE::Planner_I::Domain_Change_Type Domain_Change_Enum;
typedef ::CIAO::RACE::Planner_I::Domain_Change      Domain_Change;
typedef ::CIAO::RACE::Planner_I::Domain_Changes     Domain_Change_List;
typedef ::CIAO::RACE::Planner_I                     Domain_Change_Types;

class ImportanceBinPacker
{
public:

  /// @name Constructor and Destructor
  //@{
  ImportanceBinPacker (char ** bin_names,
                       double ** bin_capacities);
  ~ImportanceBinPacker ();
  //@}

  void print_bins (std::ostream & out) const;

  bool convert_input (const Plan_Seq & in,
                      Plan_Seq & out);

  bool domain_change (const Domain_Change_List & changes,
                      char ** joined_nodes,
                      double ** joined_node_capacities,
                      Plan_Seq & out);

private:
  struct Impl;
  Impl * impl_; ///< Pimpl idiom pointer.

  /// @name Prohibited Functions
  //@{
  ImportanceBinPacker (const ImportanceBinPacker &);
  ImportanceBinPacker & operator = (const ImportanceBinPacker &);
  //@}
};

#include /**/ "ace/post.h"

#endif /* IMPORTANCEBINPACKER_H */
