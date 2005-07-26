// $Id$
//============================================================
/**
 * @file XML_Helper.h
 *
 * @brief Some helper functions for XML
 *
 * @author Bala Natarajan <bala@dre.vanderbilt.edu>
 */
//============================================================
#ifndef CIAO_CONFIG_HANDLERS_XML_HELPER_H
#define CIAO_CONFIG_HANDLERS_XML_HELPER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers/Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "xercesc/util/XercesDefs.hpp"

namespace XERCES_CPP_NAMESPACE
{
  class DOMDocument;
}


namespace CIAO
{
  namespace Config_Handlers
  {
    /**
     * @class XML_Helper
     *
     * @brief Helper class for some routine XML stuff.
     */
    class Config_Handlers_Export XML_Helper
    {
    public:
      XML_Helper (void);

      ~XML_Helper (void);

      /// Create a DOM tree
      XERCES_CPP_NAMESPACE::DOMDocument *
      create_dom (const ACE_TCHAR *uri);

      bool is_initialized (void) const;

    protected:
      /// Intialize the parser
      void init_parser (void);

      /// Terminate the parser
      void terminate_parser (void);

    private:
      bool initialized_;
    };
  }
}

#include /**/ "ace/post.h"
#endif/*CIAO_CONFIG_HANDLERS_XML_HELPER_H*/
