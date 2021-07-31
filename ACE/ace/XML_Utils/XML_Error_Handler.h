//==============================================================
/**
 *  @file  XML_Error_Handler.h
 *
 *  @brief Error handler for Xerces
 *
 *  @author Bala Natarajan <bala@dre.vanderbilt.edu>
 */
//================================================================
#ifndef ACE_XML_ERROR_HANDLER_H
#define ACE_XML_ERROR_HANDLER_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "XML_Utils_Export.h"

#include <xercesc/sax/ErrorHandler.hpp>

using xercesc::SAXParseException;

namespace XML
{
  /**
    * @class XML_Error_Hander
    *
    * @brief Error handler for XERCES
    *
    */
  class XML_Utils_Export XML_Error_Handler
    : public xercesc::ErrorHandler
  {
  public:
    XML_Error_Handler () = default;
    ~XML_Error_Handler () = default;

    void warning(const SAXParseException& toCatch) override;
    void error(const SAXParseException& toCatch) override;
    void fatalError(const SAXParseException& toCatch) override;
    void resetErrors() override;
    bool getErrors () const;
  private :
    XML_Error_Handler (const XML_Error_Handler&) = delete;
    XML_Error_Handler& operator= (const XML_Error_Handler&) = delete;
    XML_Error_Handler (XML_Error_Handler&&) = delete;
    XML_Error_Handler& operator= (XML_Error_Handler&&) = delete;

    bool errors_ { false };
  };
}

#include /**/ "ace/post.h"

#endif /* ACE_XML_ERROR_HANDLER_H */
