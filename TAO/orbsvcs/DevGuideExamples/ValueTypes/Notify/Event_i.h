#ifndef EVENT_H_
#define EVENT_H_

#include "EventS.h"

#include "tao/Valuetype/ValueFactory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class MyEvent_i
  : public virtual OBV_MyEvent
  , public virtual CORBA::DefaultValueRefCountBase
{
public:
  MyEvent_i (char const *name, CORBA::Long kind)
  {
    this->name (name);
    this->kind (kind);
  }

  MyEvent_i (MyEvent_i const &) = delete;
  MyEvent_i (MyEvent_i &&) = delete;
  MyEvent_i &operator= (MyEvent_i const &) = delete;
  MyEvent_i &operator= (MyEvent_i &&) = delete;

  ::CORBA::ValueBase * _copy_value () override
  {
    ::CORBA::ValueBase *ret_val {};
    ACE_NEW_THROW_EX (
      ret_val,
      MyEvent_i (this->name (), this->kind ()),
      ::CORBA::NO_MEMORY ()
    );
    return ret_val;
  }

  void dump () override
  {
    CORBA::LongSeq &pl = this->payload ();
    ACE_DEBUG ((LM_DEBUG, "\nPayload contents.\n"));
    for (CORBA::ULong i {}; i < pl.length (); ++i)
      ACE_DEBUG ((LM_DEBUG, "%d = %d\n", i, pl[i]));
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }

  CORBA::Long size () override
  {
    return this->payload ().length ();
  }

  void add_long (CORBA::Long n) override
  {
    CORBA::LongSeq &pl = this->payload ();
    CORBA::ULong const idx = pl.length ();
    pl.length (idx + 1);
    pl[idx] = n;
  }
};

class MyEventFactory
  : public virtual CORBA::ValueFactoryBase
{
public:
  MyEventFactory () = default;

  MyEventFactory (MyEventFactory const &) = delete;
  MyEventFactory (MyEventFactory &&) = delete;
  MyEventFactory &operator= (MyEventFactory const &) = delete;
  MyEventFactory &operator= (MyEventFactory &&) = delete;

  CORBA::ValueBase * create_for_unmarshal () override
  {
    // It doesn't matter what values we construct it with
    // because they will be overwritten with the demarshaled values.
    return new MyEvent_i ("", -1);
  }
};

#endif /* EVENT_H_  */
