// $Id$

#include "ace/OS_Memory.h"
#include "ace/Log_Msg.h"
#include "ace/Containers.h"
#include "DataElement.h"

class StackExample
{
public:
  StackExample (): privateStack_(100) {}

  // Illustrate all the differnet
  // types of stacks provided by ACE.
  int run (void);

private:
  // Illustrate the use of a bounded stack.
  int runBoundedStack (void);

  // Illustrate the use of an unbounded stack.
  int runUnboundedStack (void);

  // Illustrate the use of a compile time fixed stack.
  int runFixedStack (void);

private:
  ACE_Bounded_Stack<DataElement*> privateStack_;
};

int StackExample::run (void)
{
  ACE_TRACE ("StackExample::run");

  ACE_ASSERT(!this->runBoundedStack());
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n# of live objects %d\n"),
              DataElement::numOfActiveObjects()));

  ACE_ASSERT(!this->runFixedStack());
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n# of live objects %d\n"),
              DataElement::numOfActiveObjects()));

  ACE_ASSERT(!this->runUnboundedStack());
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n# of live objects %d\n"),
              DataElement::numOfActiveObjects()));

  return 0;
}
// Listing 1 code/ch05
int StackExample::runBoundedStack (void)
{
  ACE_TRACE ("StackExample::runBoundedStack");
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Using a bounded stack\n")));

  ACE_Bounded_Stack<DataElement> bstack1 (100);

  // The element array is constrained to this scope.
  {
    DataElement elem[100];
    for (int i = 0; i < 100; i++)
      {
        elem[i].setData(i);
        // Push the element on the stack.
        bstack1.push (elem[i]);
      }
  }

  ACE_Bounded_Stack<DataElement> bstack2 (100);

  // Make a copy!
  bstack2 = bstack1;
  for (int j = 0; j < 100; j++)
    {
      DataElement elem;
      bstack2.pop (elem);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"), elem.getData ()));
    }

  return 0;
}
// Listing 1
// Listing 2 code/ch05
int StackExample::runFixedStack (void)
{
  ACE_TRACE ("StackExample::runFixedStack");
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Using a fixed stack\n")));

  ACE_Fixed_Stack<DataElement*, 100> fstack;
  for (int k = 0; k < 100; k++)
    {
      DataElement* elem;
      ACE_NEW_RETURN(elem, DataElement (k), -1);
      fstack.push (elem);    // Push the element on the stack.
    }

  for (int l = 0; l < 100; l++)
    {
      DataElement* elem = 0;
      fstack.pop (elem);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"), elem->getData ()));
      delete elem;
    }

  return 0;
}

int StackExample::runUnboundedStack (void)
{
  ACE_TRACE ("StackExample::runUnboundedStack");
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Using an unbounded stack\n")));

  ACE_Unbounded_Stack<DataElement*> ustack;
  for (int m = 0; m < 100; m++)
    {
      DataElement *elem;
      ACE_NEW_RETURN(elem, DataElement (m), -1);
      // Push the element on both stacks.
      ustack.push (elem);
      privateStack_.push (elem);
    }

  // Oddly enough, you can actually iterate through an
  // unbounded stack! This is because underneath the covers
  // the unbounded stack is a linked list.

  // This will cause the elements in the private stack to
  // also disappear!
  ACE_Unbounded_Stack_Iterator<DataElement*> iter (ustack);
  for (iter.first (); !iter.done (); iter.advance ())
    {
      DataElement** elem = 0;
      iter.next (elem);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"),
                  (*elem)->getData ()));
      delete (*elem);
    }

  return 0;
}
// Listing 2

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  StackExample se;
  return se.run ();
}

