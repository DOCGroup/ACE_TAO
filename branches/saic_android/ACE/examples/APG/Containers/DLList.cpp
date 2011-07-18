// $Id$

#include "ace/OS_Memory.h"
#include "ace/Log_Msg.h"

// Listing 1 code/ch05
#include "ace/Containers.h"
#include "DataElement.h"

// Create a new type of list that can store only DataElements.
typedef ACE_DLList<DataElement> MyList;
// Listing 1

// Listing 2 code/ch05
class ListTest
{
public:
  int run (void);
  void displayList (MyList & list); // Display all elements.
  void destroyList (MyList& list);  // Destroy all elements.
};
// Listing 2
// Listing 3 code/ch05
int
ListTest::run (void)
{
  ACE_TRACE ("ListTest::run");

  // Create a list and insert 100 elements.
  MyList list1;

  for (int i = 0; i < 100; i++)
    {
      DataElement *element;
      ACE_NEW_RETURN (element, DataElement (i), -1);
      list1.insert_tail (element);
    }

  // Iterate through and display to output.
  this->displayList (list1);

  // Create a copy of list1.
  MyList list2;
  list2 = list1;

  // Iterate over the copy and display it to output.
  this->displayList(list2);

  // Get rid of the copy list and all its elements.
  // Since both lists had the *same* elements
  // this will cause list1 to contain pointers that
  // point to data elements that have already been destroyed!
  this->destroyList (list2);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("# of live objects: %d\n"),
              DataElement::numOfActiveObjects()));

  // The lists themselves are destroyed here. Note that the
  // list destructor will destroy copies of whatever data the
  // list contained. Since in this case the list contained
  // copies of pointers to the data elements these are the
  // only thing that gets destroyed here.
  return 0;
}
// Listing 3
// Listing 4 code/ch05
void
ListTest::destroyList (MyList& list)
{
  ACE_TRACE ("ListTest::destroyList");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Destroying data elements\n")));

  // Iterate through and delete all the data elements on the list.
  for (ACE_DLList_Iterator<DataElement> iter (list);
       !iter.done ();
       iter++)
    {
      DataElement *de = iter.next ();
      delete de;
    }
}
// Listing 4
// Listing 5 code/ch05
void
ListTest::displayList (MyList& list)
{
  ACE_TRACE ("ListTest::displayList");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Forward iteration\n")));
  ACE_DLList_Iterator<DataElement> iter (list);
  while (!iter.done ())
    {
      ACE_DEBUG
        ((LM_DEBUG, ACE_TEXT ("%d:"), iter.next()->getData()));
      iter++;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Reverse Iteration\n")));
  ACE_DLList_Reverse_Iterator<DataElement> riter (list);
  while (!riter.done ())
    {
      ACE_DEBUG
        ((LM_DEBUG, ACE_TEXT ("%d:"), riter.next()->getData()));
      riter++;
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
}
// Listing 5
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ListTest test;
  return test.run ();
}

