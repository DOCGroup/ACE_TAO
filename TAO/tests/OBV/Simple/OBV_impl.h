// -*- C++ -*-
#if !defined (OBV_IMPL_H)
#define OBV_IMPL_H

#include "OBVC.h"
#include "tao/Valuetype/ValueFactory.h"

class Event_impl : public virtual OBV_Event,
                   public virtual CORBA::DefaultValueRefCountBase
{
 public:
  Event_impl () = default;
  explicit Event_impl (CORBA::Long value);
  ~Event_impl () override = default;

  Event_impl (Event_impl const&) = delete;
  Event_impl (Event_impl&&) = delete;
  Event_impl& operator= (Event_impl const&) = delete;
  Event_impl& operator= (Event_impl&&) = delete;

  CORBA::ValueBase *_copy_value () override;
  void do_print () override;
};

class Event_factory : public CORBA::ValueFactoryBase
{
public:
  Event_factory () = default;

  Event_factory (Event_factory const&) = delete;
  Event_factory (Event_factory&&) = delete;
  Event_factory& operator= (Event_factory const&) = delete;
  Event_factory& operator= (Event_factory&&) = delete;

  // create (...) would go here

protected:
  ~Event_factory () override = default;

private:
  TAO_OBV_CREATE_RETURN_TYPE (Event) create_for_unmarshal () override;
};

#endif /* OBV_IMPL_H */
