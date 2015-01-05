
//=============================================================================
/**
 *  @file    SString_Test.cpp
 *
 *  $Id$
 *
 *    This is a simple test that illustrates the use of ACE_CString
 *    and ACE_WString. No command line arguments are needed to run
 *    the test.
 *
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/Auto_Ptr.h"
#include "ace/SString.h"



static int testConcatenation() {
#ifdef ACE_HAS_WCHAR
  ACE_WString s1;
  s1 += L'H';
  if (s1 != ACE_WString(L"H")) {
    ACE_ERROR((LM_ERROR, "Concat wchar_t\n"));
    return 1;
  }
  s1 = ACE_WString(L"Hello");
  s1 += L" World";
  if (s1 != ACE_WString(L"Hello World")) {
    ACE_ERROR((LM_ERROR, "Concat wchar_t*\n"));
    return 1;
  }
  s1 = L"Hello";
  s1 += ACE_WString(L" World");
  if (s1 != ACE_WString(L"Hello World")) {
    ACE_ERROR((LM_ERROR, "Concat wstring\n"));
    return 1;
  }
  s1 = L"Hello";
  s1.append(L" World", 6);
  if (s1 != ACE_WString(L"Hello World")) {
    ACE_ERROR((LM_ERROR, "Concat wchar_t* 2\n"));
    return 1;
  }
  s1 += L'.';
  if (s1 != ACE_WString(L"Hello World.")) {
    ACE_ERROR((LM_ERROR, "Concat wchar_t\n"));
    return 1;
  }
  ACE_WString s2(L"Hello World");
  s2 += L'.';
  if (s2 != ACE_WString(L"Hello World.")) {
    ACE_ERROR((LM_ERROR, "Concat wchar_t 2\n"));
    return 1;
  }
#endif /* ACE_HAS_WCHAR */
  return 0;
}

int testIterator()
{
  ACE_CString s1 ("Hello, World");

  // Use the advance () method to count number of characters.
  size_t count = 0;
  for (ACE_CString::ITERATOR iter (s1); !iter.done (); iter.advance ())
    ++ count;

  if (count != s1.length ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("advance () failed")),
                       1);

  // Use the prefix operator to count number of characters.
  count = 0;
  for (ACE_CString::ITERATOR iter (s1); !iter.done (); ++ iter)
    ++ count;

  if (count != s1.length ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("++ operator failed")),
                       1);

  count = 0;

  for (ACE_CString::iterator iter = s1.begin (), iter_end = s1.end ();
       iter != iter_end; iter ++)
  {
    ++ count;
  }

  if (count != s1.length ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("end () failed")),
                       1);

  ACE_CString::iterator iter1 (s1);

  if (*iter1 != s1[0])
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("dereference operator failed")),
                       1);

  return 0;
}

int testConstIterator()
{
  const ACE_CString s1 ("Hello, World");

  // Use the advance () method to count number of characters.
  size_t count = 0;
  for (ACE_CString::CONST_ITERATOR iter (s1); !iter.done (); iter.advance ())
    ++ count;

  if (count != s1.length ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("advance () failed")),
                       1);

  // Use the prefix operator to count number of characters.
  count = 0;
  for (ACE_CString::CONST_ITERATOR iter (s1); !iter.done (); ++ iter)
    ++ count;

  if (count != s1.length ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("++ operator failed")),
                       1);

  count = 0;

  for (ACE_CString::const_iterator iter = s1.begin (), iter_end = s1.end ();
       iter != iter_end; iter ++)
  {
    ++ count;
  }

  if (count != s1.length ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("end () failed")),
                       1);

  ACE_CString::const_iterator iter1 (s1);

  if (*iter1 != s1[0])
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("dereference operator failed")),
                       1);

  return 0;
}


int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("SString_Test"));

  {

    /* Set #1 */
    ACE_CString s0 ("hello");
    ACE_CString s1 ("hello");
    ACE_CString s2 ("world");
    ACE_CString s3 ("ll");
    ACE_CString s4 ("ello");
    ACE_CString s5 = s1 + " " + s2;

    char single_character = 'z';
    ACE_CString single_character_string (single_character);

    ACE_CString empty_string;
    ACE_CString zero_size_string (s1.c_str (), 0, 0, 1);

    if (ACE_CString::npos == 0)
      ACE_ERROR((LM_ERROR,"Set #1: npos is incorrect.\n"));

    // Not equal comparisons. Error if they are equal
    if (s1 == s2){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s1 == s5){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}

    // Equal comparisons. Error if they are not equal
    if (s1 != s1){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s1 != s0){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}

    // Substring match. Error if they are not equal
    if (s1.strstr (s2) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s1.strstr (s3) != 2){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s3.strstr (s1) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s1.strstr (s4) != 1){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}

    // Substring creation. Error if they are not equal
    if (s1.substring (0) != s1){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s1.substring (1) != s4){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s1.substring (2, 2) != s3){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s1.substring (0, 0) != empty_string){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s1.substring (4, 10).length () != 1){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}

    // Forward search. Error if they are not equal
    if (s1.find (s3) != 2){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s3.find (s1) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s1.find (s3, 2) != 2){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s3.find (s1, 1) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s1.find (s2) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s1.find ('o') != 4){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}

    // Reverse search. Error if they are not equal
    if (s1.rfind ('l') != 3){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    if (s1.rfind ('l', 3) != 2){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}

    // Assignment. Error if they are not equal
    ACE_CString s6;
    s6 = s0;
    if (s6 != s0){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    s6 = s4;
    if (s4 != s6){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
    s6 = s5;
    if (s6 != s5){ACE_ERROR((LM_ERROR,"Set #1:\n"));return 1;}
  }

  {
    /* Set #2 */
    ACE_CString s0 = "hello";
    ACE_CString s1 ("hello", 0, false);
    ACE_CString s2 ("world", 0, false);
    ACE_CString s3 ("ll", 0, false);
    ACE_CString s4 ("ello", 0, false);
    ACE_CString s5 = s1 + " " + s2;

    char single_character = 'z';
    ACE_CString single_character_string (single_character);

    ACE_CString empty_string (0, 0, false);
    ACE_CString zero_size_string (s1.c_str (), 0, 0, false);

    // Not equal comparisons. Error if they are equal
    if (s1 == s2){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    if (s1 == s5){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}

    // Equal comparisons. Error if they are not equal
    if (s1 != s1){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    if (s1 != s0){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}

    // Substring match. Error if they are not equal
    if (s1.strstr (s2) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    if (s1.strstr (s3) != 2){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    if (s3.strstr (s1) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    if (s1.strstr (s4) != 1){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}

    // Substring creation. Error if they are not equal
    if (s1.substring (0) != s1){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    if (s1.substring (1) != s4){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    if (s1.substring (2, 2) != s3){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    if (s1.substring (0, 0) != empty_string){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}

    // Forward search. Error if they are not equal
    if (s1.find (s3) != 2){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    if (s3.find (s1) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    if (s1.find (s3, 2) != 2){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    if (s3.find (s1, 1) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    if (s1.find (s2) != ACE_CString::npos){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    if (s1.find ('o') != 4){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}

    // Reverse search. Error if they are not equal
    if (s1.rfind ('l') != 3){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    if (s1.rfind ('l', 3) != 2){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}

    // Assignment. Error if they are not equal
    ACE_CString s6;
    s6 = s0;
    if (s6 != s0){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    s6 = s4;
    if (s4 != s6){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
    s6 = s5;
    if (s6 != s5){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}

    // Clear. Error if they are not equal
    s0.clear();
    if (s0.length() != 0){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}

    // Rep. Error if they are not equal
    ACE_Auto_Basic_Array_Ptr<char> s (s1.rep ());
    if (ACE_OS::strlen (s.get ()) != s1.length ())
      {
        ACE_ERROR((LM_ERROR,"Auto_ptr s:\n"));
      };

    ACE_CString s7 (s.get ());
    if (s1 != s7){ACE_ERROR((LM_ERROR,"Set #2:\n"));return 1;}
  }

  {
    /* Set #3 */
    ACE_NS_WString s0 ("hello");
    ACE_NS_WString s1 ("hello");
    ACE_NS_WString s2 ("world");
    ACE_NS_WString s3 ("ll");
    ACE_NS_WString s4 ("ello");
    ACE_NS_WString s5 = s1 + " " + s2;
    ACE_NS_WString s6 = ("hella"); // Same length as s1, off by one char.

    ACE_WCHAR_T single_character = 'z';
    ACE_NS_WString single_character_string (single_character);

    ACE_NS_WString empty_string;
    ACE_NS_WString zero_size_string (s1.c_str (), 0, 0);

    // Not equal comparisons. Error if they are equal
    if (s1 == s2){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s1 == s5){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s1 == s6){ACE_ERROR((LM_ERROR,"Set #3: off-by-one failed\n"));return 1;}

    // Equal comparisons. Error if they are not equal
    if (s1 != s1){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s1 != s0){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}

    // Substring match. Error if they are not equal
    if (s1.strstr (s2) != ACE_NS_WString::npos){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s1.strstr (s3) != 2){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s3.strstr (s1) != ACE_NS_WString::npos){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s1.strstr (s4) != 1){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}

    // Substring creation. Error if they are not equal
    if (s1.substring (0) != s1){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s1.substring (1) != s4){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s1.substring (2, 2) != s3){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s1.substring (0, 0) != empty_string){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}

    // Forward search. Error if they are not equal
    if (s1.find (s3) != 2){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s3.find (s1) != ACE_NS_WString::npos){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s1.find (s3, 2) != 2){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s3.find (s1, 1) != ACE_NS_WString::npos){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s1.find (s2) != ACE_NS_WString::npos){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s1.find ('o') != 4){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}

    // Reverse search. Error if they are not equal
    if (s1.rfind ('l') != 3){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    if (s1.rfind ('l', 3) != 2){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}

    // Assignment. Error if they are not equal
    ACE_NS_WString s7;
    s7 = s0;
    if (s7 != s0){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    s7 = s4;
    if (s4 != s7){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
    s7 = s5;
    if (s7 != s5){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}

    // Clear. Error if they are not equal
    s0.clear();
    if (s0.length() != 0){ACE_ERROR((LM_ERROR,"Set #3:\n"));return 1;}
  }

  {
    /* Set #4 */
    ACE_CString s1("dog");
    ACE_CString s2("d");

    if (s1 == s2){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}
    if (!(s1 > s2)){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}
    if (s1 < s2){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}

    ACE_CString s3 ("dog");
    ACE_CString s4 ("dogbert");

    if (s3 == s4){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}
    if (!(s3 < s4)){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}
    if (s3 > s4){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}

    ACE_CString s5 ("dogbert",3);
    ACE_CString s6 ("dogbert",5);

    if(s5 == s6){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}
    if(!(s5 < s6)){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}
    if(s5 > s6){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}

    ACE_CString s7 ("dogbert",4);
    ACE_CString s8 ("dogbert",2);

    if(s7 == s8){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}
    if(!(s7 > s8)){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}
    if(s7 < s8){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}

    ACE_CString s9 ("dogbert",3);
    ACE_CString s10 ("dogbert");

    if(s9 == s10){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}
    if(!(s9 < s10)){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}
    if(s9 > s10){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}

    ACE_CString s11 ("dogbert",5);
    ACE_CString s12 ("dog");

    if(s11 == s12){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}
    if(!(s11 > s12)){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}
    if(s11 < s12){ACE_ERROR((LM_ERROR,"Set #4:\n"));return 1;}

    s11.fast_clear ();
    if (s11.length () != 0)
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("fast_clear didn't yield 0 length\n")));
  }

  {
    // Set 1 for ACE_SString, which is not tested
    ACE_SString sstr;

    const char *old = sstr.rep ();
    const char *str = "What_a_day_it_has_been";

    sstr.rep (const_cast<char *>(str));

    ACE_SString tmp =
      sstr.substring (2, 300);

    if (tmp.length () == 300)
      ACE_ERROR ((LM_ERROR, "SString substring\n"));

    // Constring an ACE_SString without a character pointer or from an
    // existing ACE_SString causes memory to be allocated that will not
    // be delete (apparently by design).
    ACE_Allocator::instance ()->free (const_cast<char *> (old));
    ACE_Allocator::instance ()->free (const_cast<char *> (tmp.rep ()));
  }

  int err = testConcatenation ();
  err += testIterator ();
  err += testConstIterator ();

  ACE_END_TEST;
  return err;
}
