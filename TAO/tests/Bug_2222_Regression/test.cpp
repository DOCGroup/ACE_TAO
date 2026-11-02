#include "TestCaseC.h"

/**
* This test does not run - successful compile and link == successful test.
*/
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Generated slice APIs use raw pointers paired with their matching free functions.
  Array_Typedef_slice * const body3 = Array_Typedef_alloc ();
  Array_Typedef_slice * const body31 = Array_Typedef_dup (body3);
  Array_Typedef_free (body3);
  Array_Typedef_free (body31);

  Typedef_Of_Imported_Array_Typedef_slice * const body4 =
    Typedef_Of_Imported_Array_Typedef_alloc ();
  Typedef_Of_Imported_Array_Typedef_slice * const body41 =
    Typedef_Of_Imported_Array_Typedef_dup (body4);
  Typedef_Of_Imported_Array_Typedef_free (body4);
  Typedef_Of_Imported_Array_Typedef_free (body41);

  return 0;
}
