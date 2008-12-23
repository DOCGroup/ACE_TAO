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

#include "CIAO_XML_Utils_Export.h"
#include "XML_Error_Handler.h"
#include "XML_Schema_Resolver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"
#include "ace/Auto_Ptr.h"

#include "xercesc/util/XercesDefs.hpp"

namespace XERCES_CPP_NAMESPACE
{
  class DOMDocument;
  class DOMWriter;
  class DOMImplementation;
  class DOMDocumentType;
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
    template <typename Resolver = CIAO_Schema_Resolver <>,
              typename Error_Handler = XML_Error_Handler>
    class CIAO_XML_Utils_Export XML_Helper_T
    {
    public:
      XML_Helper_T (void);

      ~XML_Helper_T (void);

      /// Create a DOM tree
      XERCES_CPP_NAMESPACE::DOMDocument *
      create_dom (const ACE_TCHAR *uri);

      XERCES_CPP_NAMESPACE::DOMDocument *
      create_dom (const ACE_TCHAR *root,
                  const ACE_TCHAR *ns,
                  XERCES_CPP_NAMESPACE::DOMDocumentType * doctype = 0);

      XERCES_CPP_NAMESPACE::DOMDocumentType *
      create_doctype (const ACE_TCHAR *qn,
                      const ACE_TCHAR *pid,
                      const ACE_TCHAR *sid);

      //Writes out a DOMDocument to an XML file
      bool write_DOM (XERCES_CPP_NAMESPACE::DOMDocument *doc,
                      const ACE_TCHAR *file) const;

      bool is_initialized (void) const;

    protected:
      /// Intialize the parser
      void init_parser (void);

      /// Terminate the parser
      void terminate_parser (void);

    private:
      bool initialized_;
      XERCES_CPP_NAMESPACE::DOMImplementation *impl_;
      Resolver resolver_;
      Error_Handler e_handler_;
    };

    typedef XML_Helper_T<> XML_Helper;
  }
}



CIAO_XML_UTILS_SINGLETON_DECLARE (ACE_Singleton,
                                  CIAO::Config_Handlers::XML_Helper,
                                  ACE_Null_Mutex)
namespace CIAO
{
  namespace Config_Handlers
  {
    typedef ACE_Singleton < XML_Helper, ACE_Null_Mutex > XML_Helper_Singleton;
  }
}


#define XML_HELPER CIAO::Config_Handlers::XML_Helper_Singleton::instance ()

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "XML_Helper.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("XML_Helper.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif/*CIAO_CONFIG_HANDLERS_XML_HELPER_H*/
