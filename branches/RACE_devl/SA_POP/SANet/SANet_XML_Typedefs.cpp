// $Id$

#include "SANet_XML_Typedefs.h"

namespace SA_POP
{
  namespace XML
  {
    XML_Helper::PATH_RESOLVER XML_Helper::_path_resolver;
    XML_Helper::XML_RESOLVER XML_Helper::_xml_resolver (XML_Helper::_path_resolver);
    XML_Helper::ERROR_HANDLER XML_Helper::_xml_error_handler;
    XML_Helper::HELPER XML_Helper::XML_HELPER (XML_Helper::_xml_resolver, XML_Helper::_xml_error_handler);
  }
}
