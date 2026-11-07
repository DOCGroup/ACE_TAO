// -*- C++ -*-
ACEXML_INLINE
ACEXML_Parser_Context::ACEXML_Parser_Context (ACEXML_InputSource* instream,
                                              ACEXML_LocatorImpl* locator)
  : instream_ (instream),
    locator_ (locator)
{

}

ACEXML_INLINE bool
ACEXML_Parser_Context::operator!= (ACEXML_Parser_Context const& src) const
{
  return (this->instream_.get () != src.instream_.get ()
          && this->locator_.get () != src.locator_.get ());
}


ACEXML_INLINE ACEXML_InputSource*
ACEXML_Parser_Context::getInputSource ()
{
  return this->instream_.get ();
}

ACEXML_INLINE ACEXML_LocatorImpl*
ACEXML_Parser_Context::getLocator ()
{
  return this->locator_.get ();
}

ACEXML_INLINE void
ACEXML_Parser_Context::setInputSource (ACEXML_InputSource* ip)
{
  this->instream_.release ();
  this->instream_.reset (ip);
}

ACEXML_INLINE void
ACEXML_Parser_Context::setLocator (ACEXML_LocatorImpl* locator)
{
  this->locator_.release ();
  this->locator_.reset (locator);
}

ACEXML_INLINE void
ACEXML_Parser_Context::reset ()
{
  this->instream_.release ();
  this->locator_.release ();
}
