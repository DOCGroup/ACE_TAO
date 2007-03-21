// -*- C++ -*-


//=============================================================================
/**
 * @file PG_Operators.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_PG_OPERATORS_H
#define TAO_PG_OPERATORS_H

#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNamingC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Global CosNaming::Name equality operator.
/**
 * Used for checking equality of PortableGroup::Location and
 * PortableGroup::Property variables.
 */
bool TAO_PortableGroup_Export operator== (const CosNaming::Name & lhs,
                                          const CosNaming::Name & rhs);

/// Global CosNaming::Name inequality operator.
/**
 * This simple implementation simply negates the result of the
 * equality operator.
 */
bool TAO_PortableGroup_Export operator!= (const CosNaming::Name & lhs,
                                          const CosNaming::Name & rhs);

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_PG_OPERATORS_H */
