// $Id$

#include "Basic_Handler.h"

BEGIN_DEPLOYMENT_NAMESPACE

Basic_Handler::Basic_Handler (DOMDocument* doc, unsigned long filter)
  : traverse_ (doc),
    doc_ (doc),
    root_ (doc->getDocumentElement()),
    filter_ (filter),
    iter_ (traverse_->createNodeIterator (this->root_,
                                          this->filter_,
                                          0,
                                          true)),
    release_ (true),
    index_ (0)
{
}

Basic_Handler::Basic_Handler (DOMNodeIterator* iter, bool release)
  : traverse_ (0), doc_ (0), root_ (0), filter_ (0), iter_ (iter),
    release_ (release)
{
}

Basic_Handler::Basic_Handler (DOMDocument* doc, DOMNodeIterator* iter, 
                              bool release)
  : traverse_ (0), doc_ (doc), root_ (0), filter_ (0), iter_ (iter),
    release_ (release)
{
}

Basic_Handler::~Basic_Handler()
{
  if (this->release_)
    this->iter_->release();
}

END_DEPLOYMENT_NAMESPACE
