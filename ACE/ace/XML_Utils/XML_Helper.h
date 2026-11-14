//============================================================
/**
 * @file XML_Helper.h
 *
 * @brief Some helper functions for XML
 *
 * @author Bala Natarajan <bala@dre.vanderbilt.edu>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */
//============================================================
#ifndef ACE_XML_HELPER_H
#define ACE_XML_HELPER_H
#include /**/ "ace/pre.h"

#if !defined(ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "XML_Error_Handler.h"
#include "XML_Schema_Resolver.h"

#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/util/XercesDefs.hpp"

#include <memory>

namespace XERCES_CPP_NAMESPACE
{
  class DOMDocument;
  class DOMWriter;
  class DOMImplementation;
  class DOMDocumentType;
}

namespace XML
{
  /**
   * @class XML_Helper
   *
   * @brief Helper class for some routine XML stuff.
   *
   * Resolver and error-handler pointers supplied to the constructor are
   * borrowed. When omitted, XML_Helper creates and owns those objects.
   */
  template <typename Resolver = XML_Schema_Resolver<>, typename Error_Handler = XML_Error_Handler>
  class XML_Helper
  {
  public:
    explicit XML_Helper (Resolver *resolver = nullptr, Error_Handler *eh = nullptr);

    ~XML_Helper () noexcept;

    XML_Helper (XML_Helper const &) = delete;
    XML_Helper &operator= (XML_Helper const &) = delete;
    XML_Helper (XML_Helper &&) = delete;
    XML_Helper &operator= (XML_Helper &&) = delete;

    /// Parse a DOM tree. The parser owns the returned document.
    XERCES_CPP_NAMESPACE::DOMDocument *create_dom (ACE_TCHAR const *uri) const;

    /// Create a DOM tree. The caller owns the returned document and must release it.
    XERCES_CPP_NAMESPACE::DOMDocument *create_dom (ACE_TCHAR const *root, ACE_TCHAR const *ns,
                                                   XERCES_CPP_NAMESPACE::DOMDocumentType *doctype = nullptr) const;

    /// Create a document type. The caller owns the returned object and must release it.
    XERCES_CPP_NAMESPACE::DOMDocumentType *create_doctype (ACE_TCHAR const *qn, ACE_TCHAR const *pid,
                                                           ACE_TCHAR const *sid) const;

    /// Write a borrowed DOM document to an XML file.
    bool write_DOM (XERCES_CPP_NAMESPACE::DOMDocument *doc, ACE_TCHAR const *file) const;

    bool is_initialized () const;

    /// Terminate the parser
    void terminate_parser ();

    Resolver &get_resolver ();

    Error_Handler &get_error_handler ();

  protected:
    /// Initialize the parser
    void init_parser ();

  private:
    void cleanup_parser (bool runtime_started);

    bool initialized_{};
    // Xerces owns impl_; parser_ owns the parser and its parsed document.
    XERCES_CPP_NAMESPACE::DOMImplementation *impl_{};
    mutable std::unique_ptr<XERCES_CPP_NAMESPACE::XercesDOMParser> parser_{};

    std::unique_ptr<Resolver> owned_resolver_{};
    // resolver_ and e_handler_ either borrow caller objects or point into the
    // corresponding owned unique_ptr.
    Resolver *resolver_{};
    std::unique_ptr<Error_Handler> owned_e_handler_{};
    Error_Handler *e_handler_{};
  };
}

#include "XML_Helper.tpp"

#include /**/ "ace/post.h"
#endif /*ACE_XML_HELPER_H*/
