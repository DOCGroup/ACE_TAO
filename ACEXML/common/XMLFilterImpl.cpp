// -*- C++ -*-  $Id$

#include "common/XMLFilterImpl.h"

#if !defined (__ACEXML_INLINE__)
# include "common/XMLFilterImpl.i"
#endif /* __ACEXML_INLINE__ */

ACEXML_XMLFilterImpl::ACEXML_XMLFilterImpl (void)
  :  parent_ (0),
     locator_ (0),
     entityResolver_ (0),
     dtdHandler_ (0),
     contentHandler_ (0),
     errorHandler_ (0)
{
}

ACEXML_XMLFilterImpl::ACEXML_XMLFilterImpl (ACEXML_XMLReader *parent)
  : parent_ (parent),
    locator_ (0),
    entityResolver_ (0),
    dtdHandler_ (0),
    contentHandler_ (0),
    errorHandler_ (0)
{
}

ACEXML_XMLFilterImpl::~ACEXML_XMLFilterImpl (void)
{
  // @@ How are all the object lifecycles managed?
}

void
ACEXML_XMLFilterImpl::parse (ACEXML_InputSource *input,
                             ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->setupParser () < 0)
    {
      xmlenv.exception (new ACEXML_SAXException (ACE_LIB_TEXT ("No Parent available")));
      return;
    }
  this->parent_->parse (input, xmlenv);
  return;
}

void
ACEXML_XMLFilterImpl::parse (const ACEXML_Char *systemId,
                             ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->setupParser () < 0)
    {
      xmlenv.exception (new ACEXML_SAXException (ACE_LIB_TEXT ("No Parent available")));
      return;
    }

  this->parent_->parse (new ACEXML_InputSource (systemId),
                        xmlenv);
  return;
}

int
ACEXML_XMLFilterImpl::getFeature (const ACEXML_Char *name,
                                  ACEXML_Env &xmlenv)
  //      ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
  //                       ACEXML_SAXNotSupportedException))
{
  if (this->parent_ != 0)
    return this->parent_->getFeature (name,
                                      xmlenv);

  xmlenv.exception (new ACEXML_SAXNotRecognizedException (name));
  return 0;
}

void *
ACEXML_XMLFilterImpl::getProperty (const ACEXML_Char *name,
                                   ACEXML_Env &xmlenv)
  //      ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
  //                       ACEXML_SAXNotSupportedException))
{
  if (this->parent_ != 0)
    return this->parent_->getProperty (name,
                                       xmlenv);

  xmlenv.exception (new ACEXML_SAXNotRecognizedException (name));
  return 0;
}

void
ACEXML_XMLFilterImpl::setFeature (const ACEXML_Char *name,
                                  int boolean_value,
                                  ACEXML_Env &xmlenv)
  //      ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
  //                       ACEXML_SAXNotSupportedException))
{
  if (this->parent_ != 0)
    {
      this->parent_->setFeature (name,
                                 boolean_value,
                                 xmlenv);
      return;
    }

  xmlenv.exception (new ACEXML_SAXNotRecognizedException (name));
}

void
ACEXML_XMLFilterImpl::setProperty (const ACEXML_Char *name,
                                   void *value,
                                   ACEXML_Env &xmlenv)
  //      ACE_THROW_SPEC ((ACEXML_SAXNotRecognizedException,
  //                       ACEXML_SAXNotSupportedException))
{
  if (this->parent_ != 0)
    {
      this->parent_->setProperty (name,
                                  value,
                                  xmlenv);
      return;
    }

  xmlenv.exception (new ACEXML_SAXNotRecognizedException (name));
}

ACEXML_XMLReader *
ACEXML_XMLFilterImpl::getParent (void) const
{
  return this->parent_;
}

void
ACEXML_XMLFilterImpl::setParent (ACEXML_XMLReader *parent)
{
  this->parent_ = parent;
}

void
ACEXML_XMLFilterImpl::characters (const ACEXML_Char *ch,
                                  int start,
                                  int length,
                                  ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->contentHandler_ != 0)
    this->contentHandler_->characters (ch, start, length, xmlenv);
}

void
ACEXML_XMLFilterImpl::endDocument (ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->contentHandler_ != 0)
    this->contentHandler_->endDocument (xmlenv);
}

void
ACEXML_XMLFilterImpl::endElement (const ACEXML_Char *namespaceURI,
                                  const ACEXML_Char *localName,
                                  const ACEXML_Char *qName,
                                  ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->contentHandler_ != 0)
    this->contentHandler_->endElement (namespaceURI,
                                       localName,
                                       qName,
                                       xmlenv);
}

void
ACEXML_XMLFilterImpl::endPrefixMapping (const ACEXML_Char *prefix,
                                        ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->contentHandler_ != 0)
    this->contentHandler_->endPrefixMapping (prefix, xmlenv);
}

void
ACEXML_XMLFilterImpl::ignorableWhitespace (const ACEXML_Char *ch,
                                           int start,
                                           int length,
                                           ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->contentHandler_ != 0)
    this->contentHandler_->ignorableWhitespace (ch,
                                                start,
                                                length,
                                                xmlenv);
}

void
ACEXML_XMLFilterImpl::processingInstruction (const ACEXML_Char *target,
                                             const ACEXML_Char *data,
                                             ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->contentHandler_ != 0)
    this->contentHandler_->processingInstruction (target,
                                                  data,
                                                  xmlenv);
}

void
ACEXML_XMLFilterImpl::setDocumentLocator (ACEXML_Locator *locator,
                                          ACEXML_Env &xmlenv)
{
  if (this->contentHandler_ != 0)
    this->contentHandler_->setDocumentLocator (locator, xmlenv);
}

void
ACEXML_XMLFilterImpl::skippedEntity (const ACEXML_Char *name,
                                     ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->contentHandler_ != 0)
    this->contentHandler_->skippedEntity (name, xmlenv);
}

void
ACEXML_XMLFilterImpl::startDocument (ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->contentHandler_ != 0)
    this->contentHandler_->startDocument (xmlenv);
}

void
ACEXML_XMLFilterImpl::startElement (const ACEXML_Char *namespaceURI,
                                    const ACEXML_Char *localName,
                                    const ACEXML_Char *qName,
                                    ACEXML_Attributes *atts,
                                    ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->contentHandler_ != 0)
    this->contentHandler_->startElement (namespaceURI,
                                         localName,
                                         qName,
                                         atts,
                                         xmlenv);
}

void
ACEXML_XMLFilterImpl::startPrefixMapping (const ACEXML_Char *prefix,
                                          const ACEXML_Char *uri,
                                          ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->contentHandler_ != 0)
    this->contentHandler_->startPrefixMapping (prefix,
                                               uri,
                                               xmlenv);
}

void
ACEXML_XMLFilterImpl::notationDecl (const ACEXML_Char *name,
                                    const ACEXML_Char *publicId,
                                    const ACEXML_Char *systemId,
                                    ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->dtdHandler_ != 0)
    this->dtdHandler_->notationDecl (name,
                                     publicId,
                                     systemId,
                                     xmlenv);
}

void
ACEXML_XMLFilterImpl::unparsedEntityDecl (const ACEXML_Char *name,
                                          const ACEXML_Char *publicId,
                                          const ACEXML_Char *systemId,
                                          const ACEXML_Char *notationName,
                                          ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->dtdHandler_ != 0)
    this->dtdHandler_->unparsedEntityDecl (name,
                                           publicId,
                                           systemId,
                                           notationName,
                                           xmlenv);
}

ACEXML_InputSource *
ACEXML_XMLFilterImpl::resolveEntity (const ACEXML_Char *publicId,
                                     const ACEXML_Char *systemId,
                                     ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->entityResolver_ != 0)
    return this->entityResolver_->resolveEntity (publicId,
                                                 systemId,
                                                 xmlenv);
  return 0;
}

void
ACEXML_XMLFilterImpl::error (ACEXML_SAXParseException &exception,
                             ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->errorHandler_ != 0)
    this->errorHandler_->error (exception,
                                xmlenv);
}

void
ACEXML_XMLFilterImpl::fatalError (ACEXML_SAXParseException &exception,
                                  ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->errorHandler_ != 0)
    this->errorHandler_->fatalError (exception,
                                     xmlenv);
}

void
ACEXML_XMLFilterImpl::warning (ACEXML_SAXParseException &exception,
                               ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->errorHandler_ != 0)
    this->errorHandler_->warning (exception,
                                  xmlenv);
}

ACEXML_DTDHandler *
ACEXML_XMLFilterImpl::getDTDHandler (void) const
{
  return this->dtdHandler_;
}

ACEXML_ContentHandler *
ACEXML_XMLFilterImpl::getContentHandler (void) const
{
  return this->contentHandler_;
}

ACEXML_EntityResolver *
ACEXML_XMLFilterImpl::getEntityResolver (void) const
{
  return this->entityResolver_;
}

ACEXML_ErrorHandler *
ACEXML_XMLFilterImpl::getErrorHandler (void) const
{
  return this->errorHandler_;
}

void
ACEXML_XMLFilterImpl::setDTDHandler (ACEXML_DTDHandler *handler)
{
  this->dtdHandler_ = handler;
}

void
ACEXML_XMLFilterImpl::setContentHandler (ACEXML_ContentHandler *handler)
{
  this->contentHandler_ = handler;
}

void
ACEXML_XMLFilterImpl::setEntityResolver (ACEXML_EntityResolver *handler)
{
  this->entityResolver_ = handler;
}

void
ACEXML_XMLFilterImpl::setErrorHandler (ACEXML_ErrorHandler *handler)
{
  this->errorHandler_ = handler;
}
