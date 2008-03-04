// $Id$

#include "XML_Typedefs.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    XML_Helper::PATH_RESOLVER XML_Helper::_path_resolver;
    XML_Helper::XML_RESOLVER XML_Helper::_xml_resolver (XML_Helper::_path_resolver);
    XML_Helper::ERROR_HANDLER XML_Helper::_xml_error_handler;
    XML_Helper::HELPER XML_Helper::XML_HELPER (XML_Helper::_xml_resolver, XML_Helper::_xml_error_handler);
  }
}
      
