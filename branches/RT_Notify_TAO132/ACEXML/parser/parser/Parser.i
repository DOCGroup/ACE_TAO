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
ACEXML_Parser::isChar (const ACEXML_UCS4 c) const
{
  return (c == 0x9 || c == 0xA || c == 0xD ||
          c >= 0x20 && c <= 0xD7FF ||
          c >= 0xE000 && c <= 0xFFFD ||
          c >= 0x10000 && c <= 0x10FFFF);
}

ACEXML_INLINE int
ACEXML_Parser::isCharRef (const ACEXML_Char c) const
    {
  return ((c >= 'a' && c <= 'f') ||
          (c >= 'A' && c <= 'F'));
}

ACEXML_INLINE int
ACEXML_Parser::isNormalDigit (const ACEXML_Char c) const
{
  return (c >= '\x30' && c <= '\x39');
}

ACEXML_INLINE int
ACEXML_Parser::isBasechar (const ACEXML_Char c) const
{
#if defined (ACE_USES_WCHAR)
  return ACEXML_ParserInt::isBasechar_i (c);
#else
  return ACEXML_ParserInt::base_char_table_[c];
#endif /* ACE_USES_WCHAR */
}

ACEXML_INLINE int
ACEXML_Parser::isIdeographic (const ACEXML_Char c) const
{
#if defined (ACE_USES_WCHAR)
  return ACEXML_ParserInt::isIdeographic_i (c);
#else
  ACE_UNUSED_ARG (c);
  return 0;
#endif /* ACE_USES_WCHAR */
}

ACEXML_INLINE int
ACEXML_Parser::isCombiningchar (const ACEXML_Char c) const
{
#if defined (ACE_USES_WCHAR)
  return ACEXML_ParserInt::isCombiningchar_i (c);
#else
  ACE_UNUSED_ARG (c);
      return 0;
#endif /* ACE_USES_WCHAR */
    }

ACEXML_INLINE int
ACEXML_Parser::isDigit (const ACEXML_Char c) const
{
#if defined (ACE_USES_WCHAR)
  return ACEXML_ParserInt::isDigit_i (c);
#else
  return (this->isNormalDigit (c));
#endif /* ACE_USES_WCHAR */
}

ACEXML_INLINE int
ACEXML_Parser::isExtender (const ACEXML_Char c) const
{
#if defined (ACE_USES_WCHAR)
  return ACEXML_ParserInt::isExtender_i (c);
#else
  return (c == '\xB7');
#endif /* ACE_USES_WCHAR */
}

ACEXML_INLINE int
ACEXML_Parser::isLetter (const ACEXML_Char c) const
{
  return (this->isBasechar (c) || this->isIdeographic (c));
}

ACEXML_INLINE int
ACEXML_Parser::isNameChar (const ACEXML_Char c) const
{
  return (this->isLetter (c) || this->isDigit (c) || c == '.' || c == '-' ||
          c == '_' || c == ':' || this->isCombiningchar (c) ||
          this->isExtender (c));
}

ACEXML_INLINE int
ACEXML_Parser::isPubidChar (const ACEXML_Char c) const
{
  return (c == '\x20' || c == '\x0D' || c == '\x0A' ||
          (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') || c == '-' || c == '\'' || c == '(' ||
          c == ')' || c == '+' || c == ',' || c == '.' || c == '/' ||
          c == ':' || c == '=' || c == '?' || c == ';' || c == '!' ||
          c == '*' || c == '#' || c == '@' || c == '$' || c == '_' ||
          c == '%');
}


ACEXML_INLINE int
ACEXML_Parser::is_whitespace (const ACEXML_Char c) const
{
  switch (c)
    {
      case '\x0A': case '\x20':
      case '\x09': case '\x0D':
      return 1;
    default:
      return 0;
    }
}

ACEXML_INLINE ACEXML_Char
ACEXML_Parser::skip_whitespace (void)
{
  ACEXML_Char ch = this->get();
  while (this->is_whitespace (ch))
    ch = this->get ();
  return ch;
}


ACEXML_INLINE int
ACEXML_Parser::skip_whitespace_count (ACEXML_Char *peeky)
{
  int wscount = 0;
  ACEXML_Char dummy;
  ACEXML_Char &forward = (peeky == 0 ? dummy : *peeky);

  for (;this->is_whitespace ((forward = this->peek ())); ++wscount)
    this->get ();
  return wscount;
}

ACEXML_INLINE int
ACEXML_Parser::skip_equal (void)
{
  if (this->skip_whitespace() != '=')
    return -1;
  while (this->is_whitespace (this->peek()))
    this->get();
  return 0;
}

ACEXML_INLINE ACEXML_Char
ACEXML_Parser::get (void)
{
  ACEXML_Char ch = 0;
  const ACEXML_InputSource* ip = this->current_->getInputSource();
  ACEXML_CharStream* instream = ip->getCharStream();

  if (instream->get (ch) != -1)
    {
      this->current_->getLocator()->incrColumnNumber();
      // Normalize white-space
      if (ch == '\x0D')
        {
          if (instream->peek() == 0x0A)
            instream->get (ch);
          ch = '\x0A';
        }
      if (ch == '\x0A')
        {
          // Reset column number and increment Line Number.
          this->current_->getLocator()->incrLineNumber();
          this->current_->getLocator()->setColumnNumber (0);
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
  ACEXML_Char ch = 0;
  const ACEXML_InputSource* ip = this->current_->getInputSource();
  ACEXML_CharStream* instream = ip->getCharStream();
  ch = instream->peek ();
  return (ch > 0 ? ch : 0);
}

ACEXML_INLINE int
ACEXML_Parser::parse_token (const ACEXML_Char* keyword)
{
  if (keyword == 0)
    return -1;
  const ACEXML_Char* ptr = keyword;
  ACEXML_Char ch;
  for (; *ptr != 0 && ((ch = this->get()) == *ptr); ++ptr)
    ;
  if (*ptr == 0)
  return 0;
  else
    return -1;
}
