// Dump_T.cpp
// $Id$

#if !defined (ACE_DUMP_T_C)
#define ACE_DUMP_T_C

#include "ace/Dump_T.h"

template <class Concrete>
ACE_Dumpable_Adapter<Concrete>::~ACE_Dumpable_Adapter (void)
{
  ACE_TRACE ("ACE_Dumpable_Adapter<Concrete>::~ACE_Dumpable_Adapter");
}

template <class Concrete>
ACE_Dumpable_Adapter<Concrete>::ACE_Dumpable_Adapter (const Concrete *t)
  : this_ (t), ACE_Dumpable ((const void *) t)
{
  ACE_TRACE ("ACE_Dumpable_Adapter<Concrete>::ACE_Dumpable_Adapter");
}

template <class Concrete> Concrete *
ACE_Dumpable_Adapter<Concrete>::operator->() const
{
  return (Concrete *) this->this_;
}

template <class Concrete> void 
ACE_Dumpable_Adapter<Concrete>::dump (void) const
{ 
  ACE_TRACE ("ACE_Dumpable_Adapter<Concrete>::dump");
  this->this_->dump ();
}

#endif /* ACE_DUMP_T_C */
