#include "XercesHelpers.hpp"

#include <iostream>

#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>

using namespace xercesc;

void
XercesSystem::initialize()
{
  try
    {
      XMLPlatformUtils::Initialize();
    }
  catch (const xercesc::XMLException&)
    {
      std::cerr << "XMLException occured at initialization" << std::endl;
      throw;
    }

  XMLCh tempStr[100];
  XMLString::transcode("LS", tempStr, 99);
  impl_ = DOMImplementationRegistry::getDOMImplementation(tempStr);
  impl_ls_ = (DOMImplementationLS*) impl_;
}

DOMDocument*
XercesSystem::parse_string(DOMBuilder* parser, const char* systemId,
                           const char* xml_data, size_t size)
{
  const XMLByte* xml_code = (const XMLByte*) xml_data;

  MemBufInputSource* mem_buf_is =
    new MemBufInputSource(xml_code, size, systemId);

  Wrapper4InputSource* input_source =
    new Wrapper4InputSource(mem_buf_is);
  
  return parser->parse(*input_source);
}

