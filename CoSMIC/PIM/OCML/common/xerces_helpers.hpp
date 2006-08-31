#ifndef XERCES_HELPERS_HPP
#define XERCES_HELPERS_HPP

/**
 * @file xerces_helpers.hpp
 *
 * $Id$
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#include "OCML_Common_Export.h"
#include <xercesc/dom/DOM.hpp>

class OCML_Common_Export XercesSystem
{
public:
  XercesSystem();

  static xercesc::DOMImplementation* impl();

  static xercesc::DOMImplementationLS* impl_ls();

  static xercesc::DOMBuilder* new_parser();

  static xercesc::DOMDocument* parse_string(xercesc::DOMBuilder* parser,
                                            const char* systemId,
                                            const char* xml_data,
                                            size_t size);

private:
  static void initialize();

private:
  static bool initialized_;
  static xercesc::DOMImplementation* impl_;
  static xercesc::DOMImplementationLS* impl_ls_;
};

template <typename T>
class XercesAutoPtr
{
public:
  typedef T element_type;

  XercesAutoPtr(element_type* t=0) throw();

  XercesAutoPtr(const XercesAutoPtr<element_type>& p) throw();

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

#include "xerces_helpers.ipp"

#endif // XERCES_HELPERS_HPP
