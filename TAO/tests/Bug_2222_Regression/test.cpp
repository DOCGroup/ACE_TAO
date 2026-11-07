#include "TestCaseC.h"
#include <memory>

/**
 * This test exercises generated slice allocation, duplication, and free APIs.
 */
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  using Array_Typedef_Owner =
    std::unique_ptr<Array_Typedef_slice, decltype (&Array_Typedef_free)>;
  Array_Typedef_Owner const body3 (Array_Typedef_alloc (), Array_Typedef_free);
  Array_Typedef_Owner const body31 (Array_Typedef_dup (body3.get ()), Array_Typedef_free);

  Array_Typedef_Owner const body4 (
    Typedef_Of_Imported_Array_Typedef_alloc (),
    Typedef_Of_Imported_Array_Typedef_free);
  Array_Typedef_Owner const body41 (
    Typedef_Of_Imported_Array_Typedef_dup (body4.get ()),
    Typedef_Of_Imported_Array_Typedef_free);

  return 0;
}
