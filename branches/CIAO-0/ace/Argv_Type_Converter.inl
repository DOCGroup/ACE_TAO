// $Id$


ACE_INLINE ACE_TCHAR**
ACE_Argv_Type_Converter::get_TCHAR_argv(void)
{
#if defined (ACE_USES_WCHAR)
    if (char_passed_) {
        align_wchar_with_char();
    }

    wchar_passed_ = true;
    return wchar_argv_;
#else
    return char_argv_;
#endif  // ACE_USES_WCHAR
}


ACE_INLINE char**
ACE_Argv_Type_Converter::get_ASCII_argv(void)
{
#if defined (ACE_USES_WCHAR)
    if (wchar_passed_) {
        align_char_with_wchar();
    }

    char_passed_ = true;
#endif  // ACE_USES_WCHAR

    return char_argv_;
}


ACE_INLINE int&
ACE_Argv_Type_Converter::get_argc(void)
{
    return saved_argc_;
}
