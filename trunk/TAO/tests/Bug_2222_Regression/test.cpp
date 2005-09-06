// $Id$

#include "TestCaseC.h"

/**
* This test does not run - successful compile and link == successful test.
*/
int
main (int, char *[])
{
  ACE_TRY_NEW_ENV
    {
      Array_Typedef_slice*  body3 = Array_Typedef_alloc();
      Array_Typedef_slice* body31 = Array_Typedef_dup (body3);
      Array_Typedef_free (body3);
      Array_Typedef_free (body31);

      Typedef_Of_Imported_Array_Typedef_slice*  body4 = Typedef_Of_Imported_Array_Typedef_alloc ();
      Typedef_Of_Imported_Array_Typedef_slice*  body41 = Typedef_Of_Imported_Array_Typedef_dup (body4);
      Typedef_Of_Imported_Array_Typedef_free (body4);
      Typedef_Of_Imported_Array_Typedef_free (body41);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
