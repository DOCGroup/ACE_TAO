// $Id$


ACEXML_INLINE
ACEXML_Parser_Context::ACEXML_Parser_Context()
  : instream_ (0),
    locator_ (0)
{

}

ACEXML_INLINE
ACEXML_Parser_Context::ACEXML_Parser_Context (ACEXML_InputSource* instream,
                                              ACEXML_LocatorImpl* locator)
  : instream_ (instream),
    locator_ (locator)
{

}

ACEXML_INLINE
ACEXML_Parser_Context::ACEXML_Parser_Context (const ACEXML_Parser_Context& src)
  : instream_ (src.instream_),
    locator_ (src.locator_)
{

}

ACEXML_INLINE int
ACEXML_Parser_Context::operator!= (const ACEXML_Parser_Context& src)
{
  return (this->instream_ != src.instream_ && this->locator_ != src.locator_);
}

ACEXML_INLINE ACEXML_Parser_Context&
ACEXML_Parser_Context::operator= (const ACEXML_Parser_Context& src)
{
  ACEXML_Parser_Context tmp (src);
  ACE_Swap<ACEXML_InputSource*>::swap (this->instream_, tmp.instream_);
  ACE_Swap<ACEXML_LocatorImpl*>::swap (this->locator_, tmp.locator_);
  return *this;
}


ACEXML_INLINE ACEXML_InputSource*
ACEXML_Parser_Context::getInputSource (void)
{
  return this->instream_;
};

ACEXML_INLINE ACEXML_LocatorImpl*
ACEXML_Parser_Context::getLocator (void)
{
  return this->locator_;
}

ACEXML_INLINE void
ACEXML_Parser_Context::reset (void)
{
  this->instream_ = 0;
  this->locator_ = 0;
}
