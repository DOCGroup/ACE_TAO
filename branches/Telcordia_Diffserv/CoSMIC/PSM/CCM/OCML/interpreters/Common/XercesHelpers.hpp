#ifndef XERCES_UTILS_HPP
#define XERCES_UTILS_HPP

#pragma warning ( disable: 4786 )

#include <xercesc/dom/DOM.hpp>

class XercesSystem
{
public:
  void initialize();

  xercesc::DOMImplementation* impl();

  xercesc::DOMImplementationLS* impl_ls();

  xercesc::DOMBuilder* new_parser();

  xercesc::DOMDocument* parse_string(xercesc::DOMBuilder* parser,
                                     const char* systemId,
                                     const char* xml_data,
                                     size_t size);

private:
  xercesc::DOMImplementation* impl_;
  xercesc::DOMImplementationLS* impl_ls_;
};

template <typename T>
class XercesAutoPtr
{
public:
  typedef T element_type;

  XercesAutoPtr(element_type* t=0) throw();

  XercesAutoPtr(XercesAutoPtr& p) throw();

  XercesAutoPtr<element_type>& operator=(XercesAutoPtr<element_type>& p)
    throw();

  ~XercesAutoPtr();

  element_type& operator*() const throw();

  element_type* operator->() const throw();

  element_type* get() const throw();

  element_type* release() throw();
  
  void reset(element_type* p=0) throw();

private:
  element_type* t_;
};

#include "XercesHelpers.ipp"

#endif // XERCES_UTILS_HPP
