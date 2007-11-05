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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"

#include "xercesc/util/XercesDefs.hpp"

namespace XERCES_CPP_NAMESPACE
{
  class DOMDocument;
  class DOMWriter;
  class DOMImplementation;
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
    class CIAO_XML_Utils_Export XML_Helper
    {
    public:
      XML_Helper (void);

      ~XML_Helper (void);

      /// Create a DOM tree
      XERCES_CPP_NAMESPACE::DOMDocument *
      create_dom (const ACE_TCHAR *uri);

      XERCES_CPP_NAMESPACE::DOMDocument *
      create_dom (const ACE_TCHAR *root,
                  const ACE_TCHAR *ns);

      //Writes out a DOMDocument to an XML file
      bool write_DOM (XERCES_CPP_NAMESPACE::DOMDocument *doc,
                      const ACE_TCHAR *file);

      bool is_initialized (void) const;

    protected:
      /// Intialize the parser
      void init_parser (void);

      /// Terminate the parser
      void terminate_parser (void);

    private:
      bool initialized_;
      XERCES_CPP_NAMESPACE::DOMImplementation *impl_;
    };
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


#include /**/ "ace/post.h"
#endif/*CIAO_CONFIG_HANDLERS_XML_HELPER_H*/
