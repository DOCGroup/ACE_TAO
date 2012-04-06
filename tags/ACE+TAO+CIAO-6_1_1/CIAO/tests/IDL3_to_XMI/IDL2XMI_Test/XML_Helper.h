// $Id$
//============================================================
/**
 * @file XML_Helper.h
 *
 * @brief Some helper functions for XML
 *
 * @author Bala Natarajan <bala@dre.vanderbilt.edu>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */
//============================================================

#ifndef CIAO_XML_HELPER_H
#define CIAO_XML_HELPER_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "XML/XML_Error_Handler.h"

#include "xercesc/util/XercesDefs.hpp"

namespace XERCES_CPP_NAMESPACE
{
  class DOMImplementation;
  class XercesDOMParser;
}

namespace Test
{
  namespace XML
  {
    /**
     * @class Helper
     *
     * @brief Helper class for some routine XML stuff.
     */
    class Helper
    {
    public:
      // Initialize the resolver we should use.
      Helper (DAnCE::XML::XML_Error_Handler &eh);

      ~Helper (void);

      int validate_dom (const ACE_TCHAR *uri);

    protected:
      /// Intialize the parser
      void init_parser (void);

      /// Terminate the parser
      void terminate_parser (void);

    private:
      XERCES_CPP_NAMESPACE::DOMImplementation *impl_;
      XERCES_CPP_NAMESPACE::XercesDOMParser *parser_;

      DAnCE::XML::XML_Error_Handler &e_handler_;
    };
  }
}

#include /**/ "ace/post.h"
#endif/*CIAO_XML_HELPER_H*/
