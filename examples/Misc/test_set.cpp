// $Id$

#include "ace/Containers.h"
#include "ace/Log_Msg.h"

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Unbounded_Set<int> s1;

  ACE_ASSERT (s1.size () == 0);
  s1.insert_tail (10);
  s1.insert_tail (20);
  ACE_ASSERT (s1.size () == 2);

  ACE_Unbounded_Set<int> s2 (s1);
  ACE_ASSERT (s2.size () == 2);

  ACE_Unbounded_Set<int> s3;
  ACE_ASSERT (s3.size () == 0);

  s3 = s2;
  ACE_ASSERT (s3.size () == s2.size ());

  ACE_Unbounded_Set<int> s4 (s3);
  ACE_ASSERT (s4.size () == 2);

  int *ip = 0;

  ACE_DEBUG ((LM_DEBUG, "dumping s1\n"));
  for (ACE_Unbounded_Set_Iterator<int> iter1 (s1);
       iter1.next (ip) != 0;
       iter1.advance ())
    ACE_DEBUG ((LM_DEBUG, "item = %d\n", *ip));

  ACE_DEBUG ((LM_DEBUG, "dumping s2\n"));
  for (ACE_Unbounded_Set_Iterator<int> iter2 (s2);
       iter2.next (ip) != 0;
       iter2.advance ())
    ACE_DEBUG ((LM_DEBUG, "item = %d\n", *ip));

  ACE_DEBUG ((LM_DEBUG, "dumping s3\n"));
  for (ACE_Unbounded_Set_Iterator<int> iter3 (s3);
       iter3.next (ip) != 0;
       iter3.advance ())
    ACE_DEBUG ((LM_DEBUG, "item = %d\n", *ip));

  ACE_DEBUG ((LM_DEBUG, "dumping s4\n"));
  for (ACE_Unbounded_Set_Iterator<int> iter4 (s4);
       iter4.next (ip) != 0;
       iter4.advance ())
    ACE_DEBUG ((LM_DEBUG, "item = %d\n", *ip));

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Unbounded_Set<int>;
template class ACE_Unbounded_Set_Iterator<int>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Unbounded_Set<int>
#pragma instantiate ACE_Unbounded_Set_Iterator<int>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
