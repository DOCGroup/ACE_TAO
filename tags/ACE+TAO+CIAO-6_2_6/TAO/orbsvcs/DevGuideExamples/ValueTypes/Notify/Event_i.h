// $Id$

#ifndef EVENT_H_
#define EVENT_H_

#include "EventS.h"

#include <iostream>
#include "tao/Valuetype/ValueFactory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class MyEvent_i
  : public virtual OBV_MyEvent
  , public virtual CORBA::DefaultValueRefCountBase
{
public:
  MyEvent_i(const char* n, CORBA::Long k)
  {
    name(n);
    kind(k);
  }

  virtual ::CORBA::ValueBase *_copy_value (void)
  {
    ::CORBA::ValueBase *ret_val = 0;
    ACE_NEW_THROW_EX (
      ret_val,
      MyEvent_i (
        name (),
        kind ()
      ),
      ::CORBA::NO_MEMORY ()
    );
    return ret_val;
  }

  virtual void dump ()
  {
    CORBA::LongSeq& pl = payload();
    ACE_DEBUG((LM_DEBUG, "\nPayload contents.\n"));
    for (CORBA::ULong i = 0; i < pl.length(); ++i)
    {
      ACE_DEBUG((LM_DEBUG, "%d = %d\n", i, pl[i]));
    }
    ACE_DEBUG((LM_DEBUG, "\n"));
  }

  virtual CORBA::Long size ()
  {
    return payload().length();
  }

  virtual void add_long (CORBA::Long n)
  {
    CORBA::LongSeq& pl = payload();
    CORBA::ULong idx = pl.length();
    pl.length(idx + 1);
    pl[idx] = n;
  }
};

class MyEventFactory
  : public virtual CORBA::ValueFactoryBase
{
public:
  virtual CORBA::ValueBase * create_for_unmarshal ()
  {
    // It doesn't matter what values we construct it with
    // because they will be overwritten with the demarshaled values.
    return new MyEvent_i("", -1);
  }
};

#endif /* EVENT_H_  */
