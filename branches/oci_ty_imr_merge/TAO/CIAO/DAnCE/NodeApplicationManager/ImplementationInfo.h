//$Id$
/* -*- C++ -*- */
/***************************************************************
 * @file   ImplementationInfo.h
 *
 * @brief  This file defines some helper functions for paring the
 *         Component ImplementationInfo and properties from
 *         the plan.
 *
 * @author Tao Lu <lu@dre.vanderbilt.edu>
 ***************************************************************/

#ifndef IMPLEMENTATIONIFO_H
#define IMPLEMENTATIONIFO_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//#include "Deployment_CoreS.h"
//#include "ace/os_types.h"
#include "ciao/DeploymentC.h"

//@@ I wish we could have precompiled header support soon!!!

// This mthod is a helper function to parse the plan into the
// ImplementationInfo struct.

namespace CIAO
{
  bool operator<<(Deployment::ImplementationInfos & info,
		  const Deployment::DeploymentPlan & plan);
}

/*
// Helper function to parse the property.
template < typename OUTPUT>
bool operator<< (OUTPUT & info,
			   const Deployment::Property & property);

// Sepcialized for the entry point property
template < >
bool operator<< < TAO_String_Manager>
				(TAO_String_Manager & info,
				 const Deployment::Property & property)
{
  const char * tmp;
  if (property.value >>= tmp)
    info = CORBA::string_duplicate (tmp);
  else
  {
    ACE_DEBUG ((LM_DEBUG, "Property::value >>= TAO_String_Manager failed!\n"));
    return 0;
  }

  return 1;
}
*/

//#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
//#include "ImplementationInfo.cpp"
//#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

//#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
//#pragma implementation ("ImplementationInfo.cpp")
//#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"
#endif /* IMPLEMENTATIONIFO_H */
