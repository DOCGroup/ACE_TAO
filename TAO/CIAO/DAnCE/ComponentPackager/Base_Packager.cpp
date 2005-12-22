// $Id$

#include "Base_Packager.h"

namespace CIAO
{
  namespace Component_Packager
  {
  Base_Packager::Base_Packager (const char *pkg_url,
                                const char *cdp_url,
                                const char *pdl_url,
                                bool include_std_desc)

    :pkg_url_ (pkg_url),
     cdp_url_ (cdp_url),
     pdl_url_ (pdl_url),
     include_std_desc_ (include_std_desc)
  {}

  int
  Base_Packager::preparePackage (PACKAGE_PLAN & /*pkg_plan*/)
  {
    return 0;
  }

  int
  Base_Packager::createPackage (const PACKAGE_PLAN &/*pkg_plan*/)
  {
    return 0;
  }

  }
}
