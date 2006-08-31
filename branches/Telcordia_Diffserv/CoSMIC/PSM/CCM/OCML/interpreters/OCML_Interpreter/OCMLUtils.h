#ifndef _OCML_UTILS_H_
#define _OCML_UTILS_H_

#include <iostream>
#include <string>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>

#include <BON.h>
#include <BONImpl.h>

/*
 *  This is a simple class that lets us do easy (though not terribly efficient)
 *  trancoding of char* data to XMLCh data.
 */
class XStr 
{
public :
  // -----------------------------------------------------------------------
  //  Constructors and Destructor
  // -----------------------------------------------------------------------
  XStr(const char* const toTranscode)
  {
    // Call the private transcoding method
    fUnicodeForm = xercesc::XMLString::transcode(toTranscode);
  }

  XStr(const std::string& toTranscode)
  {
    // Call the private transcoding method
    fUnicodeForm = xercesc::XMLString::transcode(toTranscode.c_str());
  }

  ~XStr()
  {
    xercesc::XMLString::release(&fUnicodeForm);
  }


  // -----------------------------------------------------------------------
  //  Getter methods
  // -----------------------------------------------------------------------
  const XMLCh* unicodeForm() const
  {
    return fUnicodeForm;
  }

  operator const XMLCh*() const
  {
    return unicodeForm();
  }

private :
  // -----------------------------------------------------------------------
  //  Private data members
  //
  //  fUnicodeForm
  //      This is the Unicode XMLCh format of the string.
  // -----------------------------------------------------------------------
  XMLCh*   fUnicodeForm;
};

/*
 *  Makes a lexical comparison for two FCOs according to their names
 */
class FCO_Lexical_Comparator {
public:
  bool operator()(const BON::FCO& fco1, const BON::FCO& fco2)	{
    return fco1->getName() < fco2->getName();
  }
};

template<typename T>
void children_xml_exporter(const std::set<T>& unsorted_children,
			   xercesc::DOMDocument* doc,
                           xercesc::DOMElement* node)
{
  // sort the children
  std::set<T, FCO_Lexical_Comparator>
    children(unsorted_children.begin(), unsorted_children.end());
  // call xml_export and append to XML node
  for (std::set<T, FCO_Lexical_Comparator>::const_iterator iter =
         children.begin();
       iter != children.end(); ++iter)
    node->appendChild((*iter)->xml_export(doc));
}

#endif // _OCML_UTILS_H_
