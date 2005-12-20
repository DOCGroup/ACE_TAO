//==================================================================
/**
 *  @file  PDL_Handler.h
 *
 *  $Id$
 *
 *  @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */
//==================================================================

#ifndef PDL_HANDLER_H
#define PDL_HANDLER_H
#include /**/ "ace/pre.h"

#include "ace/SString.h"
#include <list>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "pdl.hpp"

namespace CIAO
{
  namespace Component_Packager
  {
  /**
   * @class PDL_Handler
   *
   * @brief Handler class for PackageDescriptionList
   *
   * This class defines handler methods to parse the
   * PackageDescriptionList descriptor file
   */
  class PDL_Handler
  {
  public:
    typedef std::list<ACE_TString>  DESC_LIST;

    PDL_Handler ();
    virtual ~PDL_Handler (void);

    /// Process the pdl file and get the descriptors list
    int process_pdl (const char *pdl_url,
                     DESC_LIST &desc_list);

    static void get_DescriptorsList (Package::PackageDescriptorsList &list,
                                     DESC_LIST &desc_list);
  };

  }
}

#include /**/ "ace/post.h"

#endif /* PDL_HANDLER_H */
