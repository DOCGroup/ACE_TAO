//==================================================================
/**
 *  @file  Base_Packager.h
 *
 *  $Id$
 *
 *  @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */
//==================================================================

#ifndef BASE_PACKAGER_H
#define BASE_PACKAGER_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include <map>

namespace CIAO
{
  namespace Component_Packager
  {

  typedef std::map <ACE_TString, ACE_TString>::const_iterator DESC_PLAN_CONST_ITER;
  typedef std::map <ACE_TString, ACE_TString>::const_iterator IMPL_PLAN_CONST_ITER;

  struct packageplan
  {
    std::map <ACE_TString, ACE_TString> descriptors;
    std::map <ACE_TString, ACE_TString> implementations;
  };

  /**
   * @class Base_Package
   *
   * @brief Base class for Packager
   *
   */
  class Base_Packager
  {
  public:
    typedef packageplan  PACKAGE_PLAN;

    /// constructor
    Base_Packager (const char *pkg_url,
                   const char *cdp_url,
                   const char *pdl_url = 0,
                   bool include_std_desc = true);

    /// Prepare the package plan
    int preparePackage (PACKAGE_PLAN &pkg_plan);

    /// Create the archive
    int createPackage (const PACKAGE_PLAN &pkg_plan);

  protected:
    // URL for package
    const char *pkg_url_;

    // URL for flattened_deploymentplan
    const char *cdp_url_;

    // URL for PackageDescriptorsList file
    const char *pdl_url_;

    // Indicate whether to include separate descriptors
    bool include_std_desc_;
  };
  }
}

#include /**/ "ace/post.h"

#endif /* BASE_PACKAGER_H */
