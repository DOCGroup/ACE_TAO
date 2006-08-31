#include "xerces_helpers.hpp"
#include "string_utils.hpp"
#include <iostream>

int main(int, char*[])
{
  std::cout << "Test" << std::endl;

  XercesSystem system;

  XercesAutoPtr<xercesc::DOMDocument>
    doc(system.impl()->createDocument(0, XMLUnicodeString("test"), 0));

  XercesAutoPtr<xercesc::DOMWriter>
    writer(system.impl()->createDOMWriter());

  xercesc::DOMElement* element = doc->createElement(XMLUnicodeString("test"));

  std::cout << std::string(XMLUnicodeString(writer->writeToString(*element)))
            << std::endl;

  return 0;
}
