#include "ace/OS_Memory.h"
#include "ace/Log_Msg.h"
// Listing 1 code/ch05
#include "ace/Containers.h"
#include "DataElement.h"
#include <memory>

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Array<DataElement *> arr (10);
  DataElement *elem {};
  // Allocate and insert elements.
  for (int i = 0; i < 10; ++i)
    {
      ACE_NEW_RETURN (elem, DataElement (i), -1);
      // ACE_Array stores raw pointers; this example owns and deletes each pointee.
      arr[i] = elem;
    }

  // Checked access.
  ACE_ASSERT (arr.set (elem, 11) == -1);
  ACE_ASSERT (arr.get (elem, 11) == -1);

  // Make a copy and compare to the original.
  ACE_Array<DataElement *> copy = arr;
  ACE_ASSERT (copy == arr);

  ACE_Array<DataElement *>::ITERATOR iter (arr);
  while (!iter.done ())
    {
      DataElement **data {};
      iter.next (data);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%d\n"), (*data)->getData ()));
      std::unique_ptr<DataElement> element_owner {*data};
      iter.advance ();
    }
  return 0;
}
// Listing 1

