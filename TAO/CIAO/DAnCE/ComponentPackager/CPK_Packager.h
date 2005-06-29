//==================================================================
/**
 *  @file  CPK_Packager.h
 *
 *  $Id$
 *
 *  @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */
//==================================================================

#ifndef CPK_PACKAGE_H
#define CPK_PACKAGE_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include "Base_Packager.h"

#ifdef __GNUC__
    #include "ZipArchive.h"
    #include "ZipPlatform.h"
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <dirent.h>
    #include <fnmatch.h>
    #include <unistd.h>
#else
    #include "ZipArchive.h"
    #include "ZipPlatform.h"
#endif

#include <set>
#include "Base_Packager.h"

namespace CIAO
{
  namespace Component_Packager
  {
   using namespace CIAO::Component_Packager;

  /**
   * @class CPK_Package
   *
   * @brief This class defines operations
   *        for creating DAnCE component
   *        package
   *
   */
  class CPK_Packager : public Base_Packager
  {
  public:
    /// constructor
    CPK_Packager (const char *pkg_url,
                  const char *cdp_url,
                  const char *pdl_url,
                  bool include_std_desc)
     :Base_Packager (pkg_url, cdp_url, pdl_url, include_std_desc),
      cpk_file_ (this->pkg_url_) { }

    /// Collect the information for files to be archived
    int preparePackage (PACKAGE_PLAN &pkg_plan);

    /// Create a zip archive
    int createPackage (PACKAGE_PLAN &pkg_plan);

  protected:
    // A memory buffer to store package description
    CZipMemFile desc_mf_;

    // The actualy archive in zip format
    CZipArchive cpk_zip_;

    // String format used by CZipArchive
    CZipString cpk_file_;
  };

  // overloaded std::out operator
  CZipMemFile& operator << (CZipMemFile &mf,
                            const ACE_TCHAR *str);
  // overloaded std::out operator
  CZipMemFile& operator << (CZipMemFile &mf,
                            const ACE_TString &str);
  }
}

#include /**/ "ace/post.h"

#endif /* CPK_PACKAGE_H */
