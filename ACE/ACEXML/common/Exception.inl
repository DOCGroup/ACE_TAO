ACEXML_INLINE ACEXML_Char const *
ACEXML_Exception::id () const
{
  return ACEXML_Exception::exception_name_;
}

ACEXML_INLINE ACEXML_Exception*
ACEXML_Exception::_downcast (ACEXML_Exception* x)
{
  return x;
}
