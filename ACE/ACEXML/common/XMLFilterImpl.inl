// -*- C++ -*-

ACEXML_INLINE int
ACEXML_XMLFilterImpl::setupParser ()
{
  if (this->parent_ == 0)
    return -1;

  this->parent_->setEntityResolver (this);
  this->parent_->setDTDHandler (this);
  this->parent_->setContentHandler (this);
  this->parent_->setErrorHandler (this);
  return 0;
}
