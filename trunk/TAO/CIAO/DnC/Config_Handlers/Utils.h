//====================================================================
/**
 *  @file  Utils.h
 *
 *  $Id$
 *
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 *  @author Tao Lu <lu@dre.vanderbilt.edu>,
 *  @author Boris <boris@dre.vanderbilt.edu>
 *
 */
//=====================================================================

#ifndef UTILS_H
#define UTILS_H

#include /**/ "ace/pre.h"

#include "Config_Handler_export.h"

#include "tao/Basic_Types.h"
#include "XercesString.h"
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLURL.hpp>

using xercesc::XMLURL;
using xercesc::DOMNodeIterator;
using xercesc::DOMNode;
using xercesc::DOMText;
using xercesc::XMLString;
using xercesc::DOMDocument;
using Config_Handler::XStr;

namespace CIAO {

  namespace Config_Handler
  {

    /**
     * @class Utils
     *
     * @brief Utility class for parsing basic types such as string float
     * double etc.
     *
     * Helper methods for each type are defined as parse_<type> where type
     * equals string, float, double etc. The corresponding C++ types are
     * returned. These follow the IDL to C++ mapping defined by the CORBA
     * specification.
     *
     */

    class Utils
    {
    public:
      static char * parse_string (DOMNodeIterator * iter);
      // Parse string type

      static CORBA::ULong parse_ulong (DOMNodeIterator * iter);
      // Parse ulong type

      static CORBA::Long parse_long (DOMNodeIterator * iter);
      // Parse long type

      static CORBA::Short parse_short (DOMNodeIterator * iter);
      // Parse short type

      static CORBA::Float parse_float (DOMNodeIterator * iter);
      // Parse float type

      static CORBA::Double parse_double (DOMNodeIterator * iter);
      // Parse double type

      static CORBA::Boolean parse_bool (DOMNodeIterator * iter);
      // Parse bool type

      static CORBA::Char parse_char (DOMNodeIterator * iter);
      // Parse char type

      static CORBA::Octet parse_octet (DOMNodeIterator * iter);
      // Parse octect type

      static DOMDocument * create_document (const char * url);
      // Create a document from the URL

      static DOMNodeIterator * parse_href_tag (XMLURL xml_url,
                                               DOMDocument * doc);
      // Process the <href = "URL"> and return an iterator to the
      // new document
    };
  }

}
#endif /* UTILS_H */
