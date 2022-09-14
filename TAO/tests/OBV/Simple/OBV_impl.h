// -*- C++ -*-
#if !defined (OBV_IMPL_H)
#define OBV_IMPL_H

#include "OBVC.h"
#include "tao/Valuetype/ValueFactory.h"

class Event_impl : public virtual OBV_Event,
                   public virtual CORBA::DefaultValueRefCountBase
{
 public:
  Event_impl ();
  Event_impl (CORBA::Long value);
  virtual ~Event_impl ();
  virtual CORBA::ValueBase* _copy_value ();
  virtual void do_print ();
};

class Event_factory : public CORBA::ValueFactoryBase
{
public:
  // create (...) would go here

protected:
  virtual ~Event_factory ();

private:
  TAO_OBV_CREATE_RETURN_TYPE (Event) create_for_unmarshal ();
};

#endif /* OBV_IMPL_H */
