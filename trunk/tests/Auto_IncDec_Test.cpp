// $Id$

//============================================================================
//
// = LIBRARY
//     tests
//
// = FILENAME
//     Auto_IncDec_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of the Auto Increment/Decrement Class in
//     ACE.  On platforms like Win32, ACE uses template specialization
//     to use native implementations provided by the OS to accelarate
//     these operations.
//
// = AUTHOR
//     Edan Ayal <EdanA@cti2.com>
//
//============================================================================

#include "ace/Auto_IncDec_T.h"
#include "tests/test_config.h"

ACE_RCSID(tests, Auto_IncDec_Test, "Auto_IncDec_Test.cpp, by Edan Ayal")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Auto_IncDec_Test"));

  int counter = 0;
  {
    ACE_Auto_IncDec<int> Auto_IncDec1 (counter);
    ACE_ASSERT (counter == 1);

    ACE_Auto_IncDec<int> Auto_IncDec2 (counter);
    ACE_ASSERT (counter == 2);

    {
      ACE_ASSERT (counter == 2);
      ACE_Auto_IncDec<int> Auto_IncDec3 (counter);
      ACE_ASSERT (counter == 3);
    }

    ACE_ASSERT (counter == 2);
  }

  ACE_ASSERT (counter == 0);

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_IncDec<int>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_IncDec<int>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
