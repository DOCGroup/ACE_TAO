// -*- C++ -*-
//
// $Id$


ACE_INLINE ACE_TCHAR**
ACE_Argv_Type_Converter::get_TCHAR_argv (void)
{
#if defined (ACE_USES_WCHAR)
  if (this->char_passed_)
    {
      this->align_wchar_with_char ();
    }

  this->wchar_passed_ = true;
  return this->wchar_argv_;
#else
  return this->char_argv_;
#endif  // ACE_USES_WCHAR
}

ACE_INLINE char**
ACE_Argv_Type_Converter::get_ASCII_argv (void)
{
#if defined (ACE_USES_WCHAR)
  if (this->wchar_passed_)
    {
      this->align_char_with_wchar ();
    }

  this->char_passed_ = 1;
#endif  // ACE_USES_WCHAR

  return this->char_argv_;
}

ACE_INLINE int&
ACE_Argv_Type_Converter::get_argc (void)
{
  return this->saved_argc_;
}
