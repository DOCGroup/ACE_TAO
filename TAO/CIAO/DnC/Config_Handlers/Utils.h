//====================================================================
/**
 *  @file  Utils.h
 *
 *  $Id$
 *
 *  @author Tao Lu <lu@dre.vanderbilt.edu>,
 *  @author Boris <boris@dre.vanderbilt.edu>
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
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

using xercesc::DOMNodeIterator;
using xercesc::DOMNode;
using xercesc::DOMText;
using xercesc::XMLString;
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

      static CORBA::ULong parse_long (DOMNodeIterator * iter);
      // Parse long type

      static CORBA::Boolean parse_bool (DOMNodeIterator * iter);
      // Parse bool type

    };
  }

}

#endif /* UTILS_H */
