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

#include "ace/pre.h"

#include "portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNamingC.h"

/// Global CosNaming::Name equality operator.
/**
 * Used for checking equality of PortableGroup::Location and
 * PortableGroup::Property variables.
 */
int TAO_PortableGroup_Export operator== (const CosNaming::Name & lhs,
                                         const CosNaming::Name & rhs);

/// Global CosNaming::Name inequality operator.
/**
 * This simple implementation simply negates the result of the
 * equality operator.
 */
int  TAO_PortableGroup_Export operator!= (const CosNaming::Name & lhs,
                                          const CosNaming::Name & rhs);


#include "ace/post.h"

#endif  /* TAO_PG_OPERATORS_H */
