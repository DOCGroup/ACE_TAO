//=============================================================================
/**
 *  @file    Parser.i
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

ACEXML_INLINE ACEXML_ContentHandler *
ACEXML_Parser::getContentHandler (void) const
{
  return this->content_handler_;
}

ACEXML_INLINE ACEXML_DTDHandler *
ACEXML_Parser::getDTDHandler (void) const
{
  return this->dtd_handler_;
}

ACEXML_INLINE ACEXML_EntityResolver *
ACEXML_Parser::getEntityResolver (void) const
{
  return this->entity_resolver_;
}

ACEXML_INLINE ACEXML_ErrorHandler *
ACEXML_Parser::getErrorHandler (void) const
{
  return this->error_handler_;
}

ACEXML_INLINE void
ACEXML_Parser::setContentHandler (ACEXML_ContentHandler *handler)
{
  this->content_handler_ = handler;
}

ACEXML_INLINE void
ACEXML_Parser::setDTDHandler (ACEXML_DTDHandler *handler)
{
  this->dtd_handler_ = handler;
}

ACEXML_INLINE void
ACEXML_Parser::setEntityResolver (ACEXML_EntityResolver *resolver)
{
  this->entity_resolver_ = resolver;
}

ACEXML_INLINE void
ACEXML_Parser::setErrorHandler (ACEXML_ErrorHandler *handler)
{
  this->error_handler_ = handler;
}

ACEXML_INLINE int
ACEXML_Parser::is_whitespace (ACEXML_Char c)
{
  switch (c)
    {
    case 0xa:
    case 0x20:
    case 0x9:
    case 0xd:
      return 1;
    default:
      return 0;
    }
}


ACEXML_INLINE int
ACEXML_Parser::is_whitespace_or_equal (ACEXML_Char c)
{
  return (is_whitespace (c) || c == '=') ? 1 : 0;
}

ACEXML_INLINE int
ACEXML_Parser::is_nonname (ACEXML_Char c)
{
  // Handle this separately as doing so avoids code duplication and enables
  // setting of line and column numbers in one place.
  if (is_whitespace_or_equal (c))
    return 1;

  switch (c)
    {
    case '/':
    case '?':
    case '>':
    case '<':
    case ')':
    case '(':
    case '+':
    case '*':
    case '\'':
    case '"':
    case ',':
    case '|':
      return 1;
    default:
      return 0;
    }
}

ACEXML_INLINE ACEXML_Char
ACEXML_Parser::get (void)
{
  // Using an extra level of indirection so we can
  // manage document location in the future.

  if (this->instream_ != 0)
    {
      ACEXML_Char ch;
      if (this->instream_->get (ch) == -1)
        return 0;
      this->locator_.incrColumnNumber();
      if (ch == 0x0A) {
        this->locator_.incrLineNumber();
        this->locator_.setColumnNumber (0);
      }
      return ch;
    }
  return 0;
}

ACEXML_INLINE ACEXML_Char
ACEXML_Parser::peek (void)
{
  // Using an extra level of indirection so we can
  // manage document location in the future.

  if (this->instream_ != 0)
    return this->instream_->peek ();
  return 0;

}
