// -*- C++ -*-

//=============================================================================
/**
 *  @file    Cascadable_DocHandler.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef CIAO_XMLHELPERS_CASCADABLE_DOCHANDLER_H
#define CIAO_XMLHELPERS_CASCADABLE_DOCHANDLER_H

#include /**/ "ace/pre.h"
#include "ACEXML/common/DefaultHandler.h"
#include "ACEXML/common/XMLReader.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  namespace XMLHelpers
  {
    /**
     * @class Cascadable_DocHandler
     *
     * @brief A helper class for implementing cascadable XML Document Handler.
     *
     * This class is available as a convenience base class for writing
     * cascadable XML Document DefaultHandler.  This class implement
     * the basic functionality for DocHandlers that can be cascaded
     * together to handling nested XML tags.  Users can write specific
     * handlers to handle specific XML elements so they can be
     * composed together to handle complex XML definitions.  This
     * class utilize the "Interpreter" design pattern as described in
     * GoF.
     *
     * Application writers should extend this class to implement a
     * handler that interpret a specific XML element.  There are just
     * a few rules to use the Cascadable_DocHandler.
     *
     * 1. A Cascadable_DocHandler determines whether a new
     *    Cascadable_DocHandler should be used to handle a subelement
     *    in its startElement method.  When it encounter a subelement
     *    that requires a different implementation of
     *    Cascadable_DocHandler, it should create the new
     *    Cascadable_DocHandler, and then invoke the @c push_handler
     *    method to inform the @c parser to use the new handler (this
     *    in effect switches all four handlers the parse uses to the
     *    new handler.)
     *
     * 2. A Cascadable_DocHandler determines whether it has done
     *    handling an element structure in @a endElement method, it
     *    invoke the pop_handler method of its parent handler.  The
     *    pop_handler in turn invokes the endElement eventhandler of
     *    the parent handler so it has a chance to harvest the parse
     *    result from the child handler.
     *
     * See the Handler-Interaction.png for how handlers interact with
     * each other.
     *
     * @sa ACEXML_DefaultHandler
     * @sa DocHandler_Context
     */
    class Cascadable_DocHandler : public ACEXML_DefaultHandler
    {
    public:
      /**
       * Constructor.
       */
      Cascadable_DocHandler (ACEXML_XMLReader *parser,
                             Cascadable_DocHandler *parent,
                             const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName,
                             ACEXML_Attributes *atts
                             ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException)) ;

      /**
       * destructor.
       */
      virtual ~Cascadable_DocHandler (void);

      /**
       * Destroy method.
       */
      virtual void destroy ();

      /**
       * THIS function should be called by the @c startElement method
       * of the parent Handler to replace it (the current *Handlers)
       * the parser is using with the this Handler.  This method will
       * also invoke the startElement operations of this Handler.
       *
       * @@ I haven't figured out what should happen if an exception
       * occurs in push_handler.  It can either restore the original
       * handler, or let the user decide what to do.
       */
      void push_handler (Cascadable_DocHandler *new_handler,
                         ACEXML_Attributes *atts
                         ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException)) ;

      /**
       * This function should be called at the end of @c endElement
       * method of this Handler to restore the Handler's used by the
       * parser.  This method aldo invokes the @c endElement method of
       * the parent Handler before restoring the parser Handlers.
       * This allows the parent handler to have a chance to harvest
       * the parse result from this Handler.  @c pop_handler will
       * eventually invoke the @c destroy method of child handler.
       * Since @c pop_handler should be invoked from child handler's
       * @c endElement method, care should be taken to avoid accessing
       * free'ed memory after calling parent's @c pop_handler method.
       */
      void pop_handler (const ACEXML_Char *namespaceURI,
                        const ACEXML_Char *localName,
                        const ACEXML_Char *qName
                        ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException)) ;

      /**
       * Accessors for the element Context we are in.  Make copy if
       * you need to store the return strings for later use.
       */
      const ACEXML_Char *namespaceURI (void) const;
      const ACEXML_Char *localName (void) const;
      const ACEXML_Char *qName (void) const;

      /**
       * Receive an object for locating the origin of SAX document events.
       */
      virtual void setDocumentLocator (ACEXML_Locator *locator) ;

      // Methods inherit from ACEXML_ErrorHandler.

      /**
       * Receive notification of a recoverable error.
       */
      virtual void error (ACEXML_SAXParseException &exception
                          ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
        ;

      /**
       * Receive notification of a non-recoverable error.
       */
      virtual void fatalError (ACEXML_SAXParseException &exception
                               ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
        ;

      /**
       * Receive notification of a warning.
       */
      virtual void warning (ACEXML_SAXParseException &exception
                            ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
        ;

    protected:
      /**
       * Print out an error/warning message
       */
      virtual void print_warning (const ACEXML_Char *level,
                                  ACEXML_SAXParseException &exception
                                  ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
        ;

      /**
       * Element tag information.
       */
      ACEXML_Char *namespaceURI_;
      ACEXML_Char *localName_;
      ACEXML_Char *qName_;

      /// Pointer to the parser that send us events.
      ACEXML_XMLReader *parser_;

      /// Pointer to the parent event handler that handles the parent
      /// element.
      Cascadable_DocHandler *parent_;

      /// Pointer to the child event handler that handles a sub-element.
      Cascadable_DocHandler *child_;

    private:
      /// No implement
      Cascadable_DocHandler ();

      /// Pointer to Locator.
      ACEXML_Locator* locator_;
    };

    /**
     * @class Skip_DocHandler
     *
     * @brief This DocHandler ignore and skip over a specific tag.
     */
    class Skip_DocHandler : public Cascadable_DocHandler
    {
    public:
      /// Constructor.
      Skip_DocHandler (ACEXML_XMLReader *parser,
                       Cascadable_DocHandler *parent,
                       const ACEXML_Char *namespaceURI,
                       const ACEXML_Char *localName,
                       const ACEXML_Char *qName,
                       ACEXML_Attributes *atts
                       ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException));

      /// Destructor
      virtual ~Skip_DocHandler ();

      /**
       * Receive notification of the end of an element.
       */
      virtual void endElement (const ACEXML_Char *namespaceURI,
                               const ACEXML_Char *localName,
                               const ACEXML_Char *qName
                               ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException)) ;

      /**
       * Receive notification of the beginning of an element.
       */
      virtual void startElement (const ACEXML_Char *namespaceURI,
                                 const ACEXML_Char *localName,
                                 const ACEXML_Char *qName,
                                 ACEXML_Attributes *atts
                                 ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException)) ;

    protected:
      long element_count_;
    };

  }
}

#if defined (__ACE_INLINE__)
# include "Cascadable_DocHandler.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_XMLHELPERS_CASCADABLE_DOCHANDLER_H */
