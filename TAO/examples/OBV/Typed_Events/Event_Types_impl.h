// -*- C++ -*-
// $Id$

#if !defined (EVENT_TYPES_IMPL_H)
#define EVENT_TYPES_IMPL_H
#include "Event_TypesC.h"

// Event hierarchy implementation classes ======================

// The implementation of a valuetype in C++ needs to derive from
// the IDL generated OBV_Event, which provides the data members
// and their accessor/modifier.
// In TAO it is possible to have inline instead the standard virtual
// accessor/modifier functions. But then they can't be overridden for
// marshalling. (doesn't yet work anyway but will come soon %!).
//
// The 'client' of a valuetype (client means here the parts
// of an application which locally access the valuetype through its interface)
// uses pointer to the 'Event' class, or better the Event_var type
// which is similar an object reference _var.
//
// Specs say that the instances of Event_impl should be created by means
// of Event_init::create (), which are equivalents to the init () operations
// in the IDL of the valuetype --- its not yet impl. and will come later on %!
// Today you have to declare your own create () member.
//
// One more step to make (far way from Event to Event_impl, isn't it?):
// We must mix in a class to have a reference counter implementation
// (: Sit down and type twenty times CORBA::DefaultValueRefCountBase :)
// A plus is its on your hand to choose possibly a reference counter
// with (like the standard supplied one) or without lock. But I hate
// the fact that reference counting is based on virtual _add_ref ()/
// _remove_ref () functions, which may cause substantial overhead.
// I'm thinking of a TAO option to let tao_idl mix in the reference counter
// previous and generate _var classes which access inline functions.
// ... And I wonder how the OMG want to manage cyclic graphs with
// reference counting.

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Event_impl : public virtual OBV_Event,
                   public virtual CORBA::DefaultValueRefCountBase
{
 public:
  Event_impl ();
  virtual ~Event_impl ();

  virtual void do_print (ACE_ENV_SINGLE_ARG_DECL);
  // Implementation of the do_print () operation the valuetype should have.
  // All operations in valuetypes are virtual.

  // The state member for the event time is yet implemented in OBV_Event
  // (or in the Event class, if -Wb,obv_opt_accessor is given to tao_idl).
};

// An instance of the Event_factory class has to be registered
// in the ORB with ORB::register_value_factory ().

// Then the unmarshal engine can obtain a fresh instance of Event_impl
// from the (user implemented) Event_factory::create_for_unmarshal ()
// function. These should return a pointer to Event, but as long as
// covariant return types are not commonly supported one has to
// return a pointer to CORBA::ValueBase. This option handles the macro
// in front of the create_for_unmarshal () declaration. Now the code
// is compiler portable, but not ORB vendor portable ...

// ... but stop a moment. We don't want an instance of Event.
// It is only the base class for Temperature, Position and Log_Msg.
// We didn't declared Event as abstract valuetype, cause we want to
// store the timestamp in it. So we express that no instance can
// exists while we don't make a factory for it (and don't register one).

#ifdef is_certainly_not_defined
class Event_factory : public CORBA::ValueFactoryBase
{
 public:

  // create (...) would go here
private:
  virtual ~Event_factory ();
  TAO_OBV_CREATE_RETURN_TYPE (Event)  create_for_unmarshal ();
};
#endif /* is not defined */


// The procedure to register your instance is currently not much like
// OBV specs says. This implementation has currently one
// process-wide map for the registered factories. The specs want one
// per ORB. To run first OBV tests, and that is what we do yet, it is
// more practicable to have simply just one map. (%!)
// Further on it is non-standard how the repository id of the valuetype
// is obtained. Look at the macro TAO_OBV_REGISTER_FACTORY(your_factory_type)
// in tao/ValueFactory.h (explained there) how it is done or just use it
// for first experiments.
// Exceptions are not yet implemented in this area.


// Temperature implementation classes ===================================

// The Temperature_impl should inherit its own OBV_Temperature and
// the implementation of Event. (so-called ladder style implementation
// inheritance, I guess.) Its yet there, but purists
// can additionally inherit public virtual from the CORBA::DefaultVal...


class Temperature_impl : public virtual OBV_Temperature,
                         public virtual Event_impl
{
public:
  Temperature_impl ();
  // Constructor for the factory

  Temperature_impl (CORBA::Float temp);
  // Constructor, should regularly be a factory create () method.
  // But it is more simple to do so here.

  virtual ~Temperature_impl ();

  virtual void do_print (ACE_ENV_SINGLE_ARG_DECL);
  // Overrides Event_impl::do_print (). Note that a new declaration
  // in IDL in a derived class is not allowed.
};

class Temperature_factory : public CORBA::ValueFactoryBase
{
  friend class Temperature;
 public:
  // create (...) would go here
private:
  virtual ~Temperature_factory ();
  TAO_OBV_CREATE_RETURN_TYPE (Temperature)  create_for_unmarshal ();
};


// Position implementation classes =====================================

class Position_impl : public virtual OBV_Position,
                      public virtual Event_impl
{
public:
  Position_impl ();
  Position_impl (Point &p);
  virtual ~Position_impl ();

  virtual CORBA::Float x (ACE_ENV_SINGLE_ARG_DECL);
  virtual void x (CORBA::Float ACE_ENV_ARG_DECL);
  virtual CORBA::Float y (ACE_ENV_SINGLE_ARG_DECL);
  virtual void y (CORBA::Float ACE_ENV_ARG_DECL);
  virtual CORBA::Float z (ACE_ENV_SINGLE_ARG_DECL);
  virtual void z (CORBA::Float ACE_ENV_ARG_DECL);
  //These are the attributes

  virtual void do_print (ACE_ENV_SINGLE_ARG_DECL);
};

class Position_factory : public CORBA::ValueFactoryBase
{
  friend class Position;
 public:

  // create (...) would go here
private:
  virtual ~Position_factory ();
  TAO_OBV_CREATE_RETURN_TYPE (Position)  create_for_unmarshal ();
};


// Log_Msg implementation classes =====================================

class Log_Msg_impl : public virtual OBV_Log_Msg,
                     public virtual Event_impl
{
public:
  Log_Msg_impl ();
  Log_Msg_impl (CORBA::Short urgency_p, const char *message_p);
  virtual ~Log_Msg_impl ();

  virtual void do_print (ACE_ENV_SINGLE_ARG_DECL);
};

class Log_Msg_factory : public CORBA::ValueFactoryBase
{
  friend class Log_Msg;
 public:

  // create (...) would go here
private:
  virtual ~Log_Msg_factory ();
  TAO_OBV_CREATE_RETURN_TYPE (Log_Msg)  create_for_unmarshal ();
};


// Event_List_Link implementation classes =====================================

class Event_List_Link_factory;
class Event_List_Iterator;

class Event_List_Link_impl : public virtual OBV_Event_List_Link,
                         public virtual CORBA::DefaultValueRefCountBase
{
  friend class Event_List_Link_factory;
  friend class Event_List;
  friend class Event_List_Iterator;

 public:
  Event_List_Link_impl ();
  Event_List_Link_impl (Event *e);
  virtual ~Event_List_Link_impl ();

  Event *get_event (ACE_ENV_SINGLE_ARG_DECL);

 private:
  Event_List_Link *get_next_link (ACE_ENV_SINGLE_ARG_DECL);

  void attach_next_link (Event_List_Link * chain ACE_ENV_ARG_DECL);
  // Attach a chain at the end.
};


class Event_List_Link_factory : public CORBA::ValueFactoryBase
{
  friend class Event_List_Link;
 private:
  virtual ~Event_List_Link_factory ();
  TAO_OBV_CREATE_RETURN_TYPE (Event_List_Link)  create_for_unmarshal ();
};


// The event list itself.   ----------------------------------

class Event_List_impl : public virtual OBV_Event_List,
                        public virtual CORBA::DefaultValueRefCountBase
{
  friend class Event_List_Iterator;
 public:
  Event_List_impl ();
  virtual ~Event_List_impl ();

  void store_event (Event* e ACE_ENV_ARG_DECL);

  Event_List_Link *get_first_link(ACE_ENV_SINGLE_ARG_DECL);
  // The iterator needs it.

 private:
  Event_List_Link *last_link_cache_;
  // For fast attachment. Need not to be a _var cause we hold at least one
  // reference through my_first_event_list_link which is a state member and
  // is mapped to a _var.

  // And it isn't a state member of valuetype.
  // Currently we can't yet do this, because sharing of valuetypes is not
  // yet impl ...%!.
  // Without the availability to share we would get two different instances
  // of the last list link at the receiving end of an invocation.
};


class Event_List_factory : public CORBA::ValueFactoryBase
{
  friend class Event_List;
 private:
  virtual ~Event_List_factory ();
  TAO_OBV_CREATE_RETURN_TYPE (Event_List)  create_for_unmarshal ();
};


class Event_List_Iterator
{
 public:
  Event_List_Iterator (void);
  Event_List_Iterator (Event_List *list);
  // Construct it to point to the first list link.

  virtual ~Event_List_Iterator ();

  void init (Event_List *list);

  Event *next ();
  // Get pointer to the current event.

  CORBA::Boolean next (Event_var &event);
  // Sets the Event_var argument to the current event.
  // This forces proper memory management in the user code, as the Event_var
  // could be stored beyond the life time of the iterator.

  void advance ();
  // Walks one event ahead.

 private:
  Event_List_Link_var current_;
};


// Checkpoint server side --------------------------------------------


// Criterion classes -------------------------------------------------


// Criterion itself has no implementation since it is abstract.

// It is necessary to inherit from OBV_Event (or an Event implementation)
// for implementing the state members of Event.


class Temperature_Criterion_impl :
                           public virtual OBV_Temperature_Criterion,
                           public virtual OBV_Event,
                           public virtual CORBA::DefaultValueRefCountBase
{
public:
  Temperature_Criterion_impl ();
  Temperature_Criterion_impl (CORBA::ULong origin_id, CORBA::Float temp);

  virtual ~Temperature_Criterion_impl ();

  CORBA::Boolean is_critical (Event* e ACE_ENV_ARG_DECL);
  virtual void do_print (ACE_ENV_SINGLE_ARG_DECL);
};

class Temperature_Criterion_factory : public CORBA::ValueFactoryBase
{
  friend class Temperature;
private:
  virtual ~Temperature_Criterion_factory ();
  TAO_OBV_CREATE_RETURN_TYPE (Temperature_Criterion) create_for_unmarshal ();
};



class Position_Criterion_impl :
                            public virtual OBV_Position_Criterion,
                            public virtual OBV_Event,
                            public virtual CORBA::DefaultValueRefCountBase
{
public:
  Position_Criterion_impl ();
  Position_Criterion_impl (CORBA::ULong origin_id,
                           Position *lb,
                           Position *tr);
  virtual ~Position_Criterion_impl ();

  CORBA::Boolean is_critical (Event* e ACE_ENV_ARG_DECL);
  virtual void do_print (ACE_ENV_SINGLE_ARG_DECL);
};

class Position_Criterion_factory : public CORBA::ValueFactoryBase
{
  friend class Position;
private:
  virtual ~Position_Criterion_factory ();
  TAO_OBV_CREATE_RETURN_TYPE (Position_Criterion)  create_for_unmarshal ();
};



class Log_Msg_Criterion_impl : public virtual OBV_Log_Msg_Criterion,
                               public virtual OBV_Event,
                               public virtual CORBA::DefaultValueRefCountBase
{
public:
  Log_Msg_Criterion_impl ();
  virtual ~Log_Msg_Criterion_impl ();

  CORBA::Boolean is_critical (Event* e ACE_ENV_ARG_DECL);
  virtual void do_print (ACE_ENV_SINGLE_ARG_DECL);
};

class Log_Msg_Criterion_factory : public CORBA::ValueFactoryBase
{
  friend class Log_Msg;
private:
  virtual ~Log_Msg_Criterion_factory ();
  TAO_OBV_CREATE_RETURN_TYPE (Log_Msg_Criterion)  create_for_unmarshal ();
};


class Criterion_List_impl : public virtual OBV_Criterion_List,
                        public virtual CORBA::DefaultValueRefCountBase
{
 public:
  Criterion_List_impl ();
  virtual ~Criterion_List_impl ();

  void store_criterion (Criterion *c ACE_ENV_ARG_DECL);
  CORBA::Boolean is_critical (Event *e ACE_ENV_ARG_DECL);
};


// This is just a wrapper and it uses the Event_List_Iterator on
// the underlaying Event_List. Better would be a template for all the
// iterators.

class Criterion_List_Iterator
{
 public:
  Criterion_List_Iterator (Criterion_List *list);
  // Construct it to point to the first list link.

  virtual ~Criterion_List_Iterator ();

  void init (Criterion_List *list);

  Criterion *next ();
  // Get pointer to the current Criterion.

  void advance ();
  // Walks one Criterion ahead.

 private:
  Event_List_Iterator iterator_;
};


// Some origin_id's

#define KITCHEN 1001
#define BATHROOM 1002
#define JONAS 1

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */
#endif /* EVENT_TYPES_IMPL_H */
