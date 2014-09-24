// $Id$

#include "ace/OS_Memory.h"
#include "ace/Log_Msg.h"
#include "ace/Containers.h"
#include "DataElement.h"

class SetExample
{
public:
  // Illustrate all ACE set types.
  int run (void);

private:
  // Illustrate the ACE Bounded Sets.
  int runBoundedSet (void);

  // Illustrate the ACE Unbounded sets.
  int runUnboundedSet (void);
};

int SetExample::run (void)
{
  ACE_TRACE ("SetExample::run");

  ACE_ASSERT (!this->runBoundedSet ());
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n# of live objects %d\n"),
              DataElement::numOfActiveObjects ()));

  ACE_ASSERT (!this->runUnboundedSet ());
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n# of live objects %d\n"),
              DataElement::numOfActiveObjects ()));

  return 0;
}
// Listing 1 code/ch05
int SetExample::runBoundedSet ()
{
  ACE_TRACE ("SetExample::runBoundedSet");
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Using a bounded set\n")));
  ACE_Bounded_Set<DataElement> bset (100);

  DataElement elem[100];
  for (int i = 0; i < 100; i++)
    {
      elem[i].setData (i);

      // Inserting two copies of the same element isn't allowed.
      bset.insert (elem[i]);
      if (bset.insert (elem[i]) == -1)
        {
          ACE_DEBUG ((LM_ERROR, ACE_TEXT ("%p\n"),
                      ACE_TEXT ("insert set")));
        }
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d\n"),
              DataElement::numOfActiveObjects ()));

  DataElement elem1 (10), elem2 (99);
  if (!bset.find (elem1) && !bset.find (elem2))
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("The elements %d and %d are ")
                  ACE_TEXT ("in the set!\n"),
                  elem1.getData (), elem2.getData ()));
    }

  for (int j = 0; j < 50; j++)
    {
      bset.remove (elem[j]);  // Remove the element from the set.
      ACE_DEBUG
        ((LM_DEBUG, ACE_TEXT ("%d:"), elem[j].getData ()));
    }

  if ((bset.find (elem[0]) == -1) && (bset.find (elem[49]) == -1))
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("The elements %d and %d are ")
                  ACE_TEXT ("NOT in the set!\n"),
                  elem[0].getData (), elem[99].getData ()));
    }

  return 0;
}
// Listing 1
// Listing 2 code/ch05
int SetExample::runUnboundedSet ()
{
  ACE_TRACE ("SetExample::runUnboundedSet");
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Using an unbounded set.\n")));
  ACE_Unbounded_Set<DataElement*> uset;
  for (int m = 0; m < 100; m++)
    {
      DataElement *elem;
      ACE_NEW_RETURN (elem, DataElement (m), -1);
      uset.insert (elem);
    }
  DataElement deBegin (0), deEnd (99);
  if (!uset.find (&deBegin) && !uset.find (&deEnd))
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Found the elements\n")));
    }

  // Iterate and destroy the elements in the set.
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Deleting the elements\n")));
  ACE_Unbounded_Set_Iterator<DataElement*> iter (uset);
  for (iter = uset.begin (); iter != uset.end (); iter++)
    {
      DataElement* elem = (*iter);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"), elem->getData ()));
      delete elem;
    }

  return 0;
}
// Listing 2

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  SetExample se;
  se.run ();
  return 0;
}
