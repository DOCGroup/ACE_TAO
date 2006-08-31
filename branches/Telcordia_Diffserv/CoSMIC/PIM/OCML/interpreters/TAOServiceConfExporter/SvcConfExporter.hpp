#ifndef SVC_CONF_EXPORTER_HPP
#define SVC_CONF_EXPORTER_HPP

#include <string>
#include <xercesc/dom/DOM.hpp>
#include "extern_declarations.hpp"

class Svc_Conf_Exporter_API SvcConfExporter
{
public:
  char* generate_svc_conf(const char* values);

private:
  std::string parse_option(xercesc::DOMNode* node, xercesc::DOMDocument* doc);
  void parse_category(xercesc::DOMNode* source, xercesc::DOMDocument* target,
                      xercesc::DOMNode* parent);
};

#endif // SVC_CONF_EXPORTER_HPP
