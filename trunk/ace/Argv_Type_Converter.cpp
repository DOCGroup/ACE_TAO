// $Id$

#include "ace/Argv_Type_Converter.h"

#if !defined (__ACE_INLINE__)
#include "ace/Argv_Type_Converter.inl"
#endif  // __ACE_INLINE__


#if defined (ACE_USES_WCHAR)
ACE_Argv_Type_Converter::ACE_Argv_Type_Converter(int& argc, wchar_t** argv)
: saved_argc_(argc)
, char_argv_(0)
, wchar_argv_(argv)
, before_pass_argc_(argc)
, original_type_(true)
, wchar_passed_(false)
, char_passed_(false)
{
    initialize();

    for (int i = 0; i < argc; ++i) {
        char_argv_[i] = ACE_OS::strdup(ACE_TEXT_ALWAYS_CHAR(argv[i]));
    }
}
#endif  // ACE_USES_WCHAR


ACE_Argv_Type_Converter::ACE_Argv_Type_Converter(int& argc, char** argv)
: saved_argc_(argc)
, char_argv_(argv)
#if defined (ACE_USES_WCHAR)
, wchar_argv_(0)
, before_pass_argc_(argc)
, original_type_(false)
, wchar_passed_(false)
, char_passed_(false)
{
    initialize();

    for (int i = 0; i < argc; ++i) {
        wchar_argv_[i] = ACE_OS::strdup(ACE_TEXT_ANTI_TO_TCHAR(argv[i]));
    }
}
#else
{
}
#endif  // ACE_USES_WCHAR


ACE_Argv_Type_Converter::~ACE_Argv_Type_Converter(void)
{
#if defined (ACE_USES_WCHAR)
    // selectively delete the 'copy' of argv
    if (original_type_) {
        // if original type is wchar_t
        if (char_passed_) {
            align_wchar_with_char();
        }
        for (int i = 0; i < saved_argc_; ++i) {
            delete [] char_argv_[i];
        }
        delete [] char_argv_;
    }
    else {
        // if original type is char
        if (wchar_passed_) {
            align_char_with_wchar();
        }
        for (int i = 0; i < saved_argc_; ++i) {
            delete [] wchar_argv_[i];
        }
        delete [] wchar_argv_;
    }
#endif  // ACE_USES_WCHAR
}


#if defined (ACE_USES_WCHAR)
void ACE_Argv_Type_Converter::initialize()
{
    if (original_type_) {  // make a copy of argv in 'char' type
    // Create one more argv entry than original argc for the NULL.
        ACE_NEW(char_argv_, char*[saved_argc_ + 1]);
        char_argv_[saved_argc_] = 0;  // last entry of argv is always a NULL
    }
    else {  // make a copy of argv in 'wchar_t' type
        ACE_NEW(wchar_argv_, wchar_t*[saved_argc_ + 1]);
        wchar_argv_[saved_argc_] = 0;
    }
}


void ACE_Argv_Type_Converter::align_char_with_wchar()
{
    int wchar_argv_index = 0;
    wchar_t* match_argv = wchar_argv_[0];  // pick the initial entry

    while (wchar_argv_index < saved_argc_) {
        // if n'th entries of both argv lists are different
        if (ACE_OS::strcmp(char_argv_[wchar_argv_index], ACE_TEXT_ALWAYS_CHAR(match_argv)) != 0) {
            // loop through the wchar argv list entries that are after wchar_argv_index
            for (int i = wchar_argv_index + 1; i < before_pass_argc_; ++i) {
                if (ACE_OS::strcmp(char_argv_[i], ACE_TEXT_ALWAYS_CHAR(match_argv)) == 0) {
                    // swap the pointers in the char argv list
                    char* temp = char_argv_[wchar_argv_index];
                    char_argv_[wchar_argv_index] = char_argv_[i];
                    char_argv_[i] = temp;
                    break;
                }
            }
        }

        // move to the next wchar argv list entry
        match_argv = wchar_argv_[++wchar_argv_index];
    }

    cleanup();
}


void ACE_Argv_Type_Converter::align_wchar_with_char()
{
    int char_argv_index = 0;
    char* match_argv = char_argv_[0];  // pick the initial entry

    while (char_argv_index < saved_argc_) {
        // if n'th entries of both argv lists are different
        if (ACE_OS::strcmp(ACE_TEXT_ALWAYS_CHAR(wchar_argv_[char_argv_index]), match_argv) != 0) {
            // loop through the wchar argv list entries that are after wchar_argv_index
            for (int i = char_argv_index + 1; i < before_pass_argc_; ++i) {
                if (ACE_OS::strcmp(ACE_TEXT_ALWAYS_CHAR(wchar_argv_[i]), match_argv) == 0) {
                    // swap the pointers in the char argv list
                    wchar_t* temp = wchar_argv_[char_argv_index];
                    wchar_argv_[char_argv_index] = wchar_argv_[i];
                    wchar_argv_[i] = temp;
                    break;
                }
            }
        }

        // move to the next wchar argv list entry
        match_argv = char_argv_[++char_argv_index];
    }

    cleanup();
}


void ACE_Argv_Type_Converter::cleanup()
{
    for (int i = saved_argc_; i < before_pass_argc_; ++i) {
        delete [] char_argv_[i];
        delete [] wchar_argv_[i];

        char_argv_[i] = 0;
        wchar_argv_[i] = 0;
    }

    before_pass_argc_ = saved_argc_;

    wchar_passed_ = false;
    char_passed_ = false;
}
#endif  // ACE_USES_WCHAR
