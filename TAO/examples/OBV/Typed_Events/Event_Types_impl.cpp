// $Id$

#include "Event_Types_impl.h"

// Implementation of the valuetype member functions.

Event_impl::Event_impl ()
  // initializers (': foo ()') don't work for OBV state members
  // since we should only access the state through modifier functions
{
  // Put a timestamp on event's birth.
  ACE_Time_Value now (ACE_OS::gettimeofday ());
  this->time_ (now.sec ());
}

Event_impl::~Event_impl ()
{
}

void
Event_impl::do_print (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC (( CORBA::SystemException ))
{
  ACE_DEBUG((LM_DEBUG, "(time %d origin %d)  ",
             (CORBA::ULong) this->time_(),
             (CORBA::ULong) this->origin_id_() ));
}


/* Not defined, see header
Event_factory::~Event_factory ()
{
}

TAO_OBV_CREATE_RETURN_TYPE (Event)
Event_factory::create_for_unmarshal ()
{
  return new Event_impl;
}
*/

// Temperature implementation  ===================================

Temperature_impl::Temperature_impl ()
{
}

Temperature_impl::Temperature_impl (CORBA::Float temp)
{
  this->temperature_ (temp);
}

Temperature_impl::~Temperature_impl ()
{
}

void
Temperature_impl::do_print (ACE_ENV_SINGLE_ARG_DECL)
	ACE_THROW_SPEC (( CORBA::SystemException ))
{
  // The timestamp
  Event_impl::do_print (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG((LM_DEBUG, "Temperature is %f\n", this->temperature_() ));
}

Temperature_factory::~Temperature_factory ()
{
}

TAO_OBV_CREATE_RETURN_TYPE (Temperature)
Temperature_factory::create_for_unmarshal ()
{
  return new Temperature_impl;
}


// Position implementation  ======================================

Position_impl::Position_impl ()
{
}

Position_impl::Position_impl (Point &p)
{
  this->xyz (p);
  // Does a copy of the p array
}

Position_impl::~Position_impl ()
{
}

void
Position_impl::do_print (ACE_ENV_SINGLE_ARG_DECL)
	ACE_THROW_SPEC (( CORBA::SystemException ))
{
  // The timestamp
  Event_impl::do_print (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Float x =
    this->x(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  CORBA::Float y =
    this->y(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  CORBA::Float z =
    this->z(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  ACE_DEBUG((LM_DEBUG, "Position is (%f, %f, %f)\n",
             x, y, z));
}

CORBA::Float Position_impl::x (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC(( CORBA::SystemException ))
{
  return this->xyz()[0];
}

void Position_impl::x (CORBA::Float x ACE_ENV_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC(( CORBA::SystemException ))
{
  this->xyz()[0] = x;
}

CORBA::Float Position_impl::y (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC(( CORBA::SystemException ))
{
  return this->xyz()[1];
}

void Position_impl::y (CORBA::Float y ACE_ENV_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC(( CORBA::SystemException ))
{
  this->xyz()[1] = y;
}

CORBA::Float Position_impl::z (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC(( CORBA::SystemException ))
{
  return this->xyz()[2];
}

void Position_impl::z (CORBA::Float z ACE_ENV_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC(( CORBA::SystemException ))
{
  this->xyz()[2] = z;
}


Position_factory::~Position_factory ()
{
}

TAO_OBV_CREATE_RETURN_TYPE (Position)
Position_factory::create_for_unmarshal ()
{
  return new Position_impl;
}


// Log_Msg implementation  ===================================

Log_Msg_impl::Log_Msg_impl ()
{
}

Log_Msg_impl::Log_Msg_impl (CORBA::Short u, const char *m)
{
  this->urgency (u);
  this->message (CORBA::string_dup (m));
}

Log_Msg_impl::~Log_Msg_impl ()
{
}

void
Log_Msg_impl::do_print (ACE_ENV_SINGLE_ARG_DECL)
	ACE_THROW_SPEC (( CORBA::SystemException ))
{
  // The timestamp
  Event_impl::do_print (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->urgency () > 0)
    {
      ACE_DEBUG((LM_DEBUG, "**** %s ****\n", this->message () ));
    }
  else
    {
      ACE_DEBUG((LM_DEBUG, "%s\n", this->message () ));
    }
}

Log_Msg_factory::~Log_Msg_factory ()
{
}

TAO_OBV_CREATE_RETURN_TYPE (Log_Msg)
Log_Msg_factory::create_for_unmarshal ()
{
  return new Log_Msg_impl;
}



// Event_List_Link implementation  ===================================

Event_List_Link_impl::Event_List_Link_impl ()
{
  this->my_event (0);
  this->next (0);
}

Event_List_Link_impl::Event_List_Link_impl (Event* e)
{
  this->my_event (e);
  // Note that the modifier increases the reference counter of e.

  this->next (0);
}

Event_List_Link_impl::~Event_List_Link_impl ()
{
  // Destructor does nothing explicit, because my_event and next are
  // _var types, which destroy (decrement the reference counter of)
  // the held event resp. the next links.
}

Event*
Event_List_Link_impl::get_event (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC (( CORBA::SystemException ))
{
  return this->my_event ();
}

Event_List_Link*
Event_List_Link_impl::get_next_link (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC(( CORBA::SystemException ))
{
  return this->next ();
}

void
Event_List_Link_impl::attach_next_link (Event_List_Link *n
                                        ACE_ENV_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC(( CORBA::SystemException ))
{
  this->next (n);
}

Event_List_Link_factory::~Event_List_Link_factory ()
{
}

TAO_OBV_CREATE_RETURN_TYPE (Event_List_Link)
Event_List_Link_factory::create_for_unmarshal ()
{
  return new Event_List_Link_impl;
}


// Event_List implementation  ===================================

Event_List_impl::Event_List_impl ()
{
  this->first_link (0);
  last_link_cache_ = 0;
}

Event_List_impl::~Event_List_impl ()
{
  // Destructor does nothing explicit, because the _var types do care.
}

void
Event_List_impl::store_event (Event* e ACE_ENV_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC(( CORBA::SystemException ))
{
  // This operation should perform atomically and should
  // guard against the access to the list from another thread.
  // But this is omitted in this example.

  Event_List_Link_var new_link (ACE_static_cast(Event_List_Link*,
                                                new Event_List_Link_impl (e)));

   // We need a new link to store the reference to the event e.
   // But if we'd had assigned the newly created instance directly through
   // a modifier function to a valuetype member, it would never be released.
   // From pointer to _var the reference count of the pointed to never
   // increased, but the modifier does.


  if (last_link_cache_ == 0)
    {
      // Search the end.
      for (Event_List_Link *i = this->first_link ();
           i != 0;
           i = i->get_next_link ())
        {
          last_link_cache_ = i;
        }
    }


  // If still null then the list is empty.
  if (last_link_cache_ == 0)
    {
      this->first_link (new_link);
    }
  else
    {
      last_link_cache_->attach_next_link (new_link);
      last_link_cache_ = new_link;
    }
}

Event_List_factory::~Event_List_factory ()
{
}

TAO_OBV_CREATE_RETURN_TYPE (Event_List)
Event_List_factory::create_for_unmarshal ()
{
  return new Event_List_impl;
}


Event_List_Iterator::Event_List_Iterator (Event_List *list)
{
  this->init (list);
}

Event_List_Iterator::Event_List_Iterator ()
{
  // current_ is a _var and set itself to null.
}

Event_List_Iterator::~Event_List_Iterator ()
{
  // nothing
}


void
Event_List_Iterator::init (Event_List *list)
{
  Event_List_Link *tmp = list->first_link ();
  CORBA::add_ref (tmp);
  current_ = tmp;
}


CORBA::Boolean
Event_List_Iterator::next (Event_var &next)
{
  if (this->current_.in ())
    {
      Event *e = current_->get_event ();
      CORBA::add_ref (e);
      next = e;
      return 1;
    }
  else
    {
      return 0;
    }
}

Event *
Event_List_Iterator::next ()
{
  if (this->current_.in ())
    {
      return current_->get_event ();
    }
  return 0;
}

void
Event_List_Iterator::advance ()
{
  if (this->current_.in ())
    {
      Event_List_Link *tmp = current_->get_next_link ();
      CORBA::add_ref (tmp);
      current_ = tmp;
    }
}



// Checkpoint server side --------------------------------------------


// Criterion classes implementation ----------------------------------



Temperature_Criterion_impl::Temperature_Criterion_impl ()
{
}

Temperature_Criterion_impl::
Temperature_Criterion_impl (CORBA::ULong origin_id, CORBA::Float temp)
{
  this->origin_id_ (origin_id);
  Temperature_var tmp (ACE_static_cast(Temperature*,
                                       new Temperature_impl (temp)));
  this->meltingpoint (tmp.in ());
}

Temperature_Criterion_impl::~Temperature_Criterion_impl ()
{
}


CORBA::Boolean
Temperature_Criterion_impl::is_critical (Event* e
                                         ACE_ENV_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC(( CORBA::SystemException ))
{
  // Downcast to a temperature.
  Temperature* t = Temperature::_downcast (e);
  // Is Event really a Temperature ?
  if (t)
    {
      // Now return the comparison with the meltingpoint.
      return
        (t->temperature_ () > this->meltingpoint ()->temperature_ ()) ?
        1 : 0;
    }
  return 0;
}


void
Temperature_Criterion_impl::do_print (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC (( CORBA::SystemException ))
{
  ACE_DEBUG((LM_DEBUG, "Alarm boundary for events with origin id %d is\n",
             this->origin_id_ () ));
  this->meltingpoint ()->do_print();
}


Temperature_Criterion_factory::~Temperature_Criterion_factory ()
{
}

TAO_OBV_CREATE_RETURN_TYPE (Temperature_Criterion)
Temperature_Criterion_factory::create_for_unmarshal ()
{
  return new Temperature_Criterion_impl;
}



Position_Criterion_impl::Position_Criterion_impl ()
{
}

Position_Criterion_impl::Position_Criterion_impl (CORBA::ULong origin_id,
                                                  Position *lb,
                                                  Position *tr)
{
  this->origin_id_ (origin_id);
  this->leftbottom(lb);
  this->topright(tr);
}

Position_Criterion_impl::~Position_Criterion_impl ()
{
}


CORBA::Boolean
Position_Criterion_impl::is_critical (Event* e ACE_ENV_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC (( CORBA::SystemException ))
{
  Position* p = Position::_downcast (e);
  // Is Event really a Position ?
  if (p)
    {
      // If the position of p is out of the box return true.
      return (
              (p->x () > this->leftbottom ()->x ()) &&
              (p->y () > this->leftbottom ()->y ()) &&
              (p->z () > this->leftbottom ()->z ()) &&
              (p->x () < this->topright ()->x ()) &&
              (p->y () < this->topright ()->y ()) &&
              (p->z () < this->topright ()->z ())
              ) ? 0 : 1;
    }
  return 0;
}


void
Position_Criterion_impl::do_print (ACE_ENV_SINGLE_ARG_DECL)
	ACE_THROW_SPEC (( CORBA::SystemException ))
{
  ACE_DEBUG((LM_DEBUG,
             "Alarm boundary for events with origin id %d is the box\n",
             this->origin_id_ () ));
  this->leftbottom ()->do_print(ACE_ENV_SINGLE_ARG_PARAMETER);
  this->topright ()->do_print(ACE_ENV_SINGLE_ARG_PARAMETER);
}

Position_Criterion_factory::~Position_Criterion_factory ()
{
}

TAO_OBV_CREATE_RETURN_TYPE (Position_Criterion)
Position_Criterion_factory::create_for_unmarshal ()
{
  return new Position_Criterion_impl;
}



Log_Msg_Criterion_impl::Log_Msg_Criterion_impl ()
{
}


Log_Msg_Criterion_impl::~Log_Msg_Criterion_impl ()
{
}


CORBA::Boolean
Log_Msg_Criterion_impl::is_critical (Event* e
                                     ACE_ENV_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC (( CORBA::SystemException ))
{
  Log_Msg* lm = Log_Msg::_downcast (e);

  // Is Event really a Log_Msg ?
  if (lm)
    {
      return (lm->urgency () ? 1 : 0);
    }
  return 0;
}


void
Log_Msg_Criterion_impl::do_print (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC (( CORBA::SystemException ))
{
  ACE_DEBUG((LM_DEBUG,
             "All log messages with urgency greater zero are registered.\n" ));
}

Log_Msg_Criterion_factory::~Log_Msg_Criterion_factory ()
{
}

TAO_OBV_CREATE_RETURN_TYPE (Log_Msg_Criterion)
Log_Msg_Criterion_factory::create_for_unmarshal ()
{
  return new Log_Msg_Criterion_impl;
}



Criterion_List_impl::Criterion_List_impl ()
{
  this->my_list (0);
  // We don't want create our list member here, because this constructor
  // is called before unmarshalling too. We cant't distinguish that.
  // If we transmit this type eventually. This is not done in this example.
  // (That is no weakness of OBV since the instance should be created and
  // initialized through the factory create (),
  // which is not yet implemented %!)
}

Criterion_List_impl::~Criterion_List_impl ()
{
}

void
Criterion_List_impl::store_criterion (Criterion *c
                                      ACE_ENV_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC (( CORBA::SystemException ))
{
  if (!my_list ())
    {
      Event_List_var ev(ACE_static_cast(Event_List*,new Event_List_impl));
      my_list (ev);
    }

  Event *e = Event::_downcast (c);
  my_list ()->store_event (e);
}

CORBA::Boolean
Criterion_List_impl::is_critical (Event *e ACE_ENV_ARG_DECL_NOT_USED)
	ACE_THROW_SPEC (( CORBA::SystemException ))
{
  // Try all criterions. Walking the list is efficient enough for
  // demonstration.

  for (Criterion_List_Iterator i (this); i.next (); i.advance ())
    {
      Criterion *c = i.next ();

      // Let e_c point to the Event part of the Criterion.
      // We know that c has an Event part.
      Event *e_c = Event::_downcast (c);


      // A criterion is applied if the origin id is matching the event.
      // A null id flags that it should be applied to all events.

      if (e->origin_id_() == e_c->origin_id_ () ||
          e_c->origin_id_ () == 0)
        {
          if (c->is_critical (e))
            return 1;
        }
    }
  return 0;
}


Criterion_List_Iterator::Criterion_List_Iterator (Criterion_List *list)
{
  this->init (list);
}

Criterion_List_Iterator::~Criterion_List_Iterator ()
{
  // nothing
}


void
Criterion_List_Iterator::init (Criterion_List *list)
{
  iterator_.init (list->my_list());
}


Criterion *
Criterion_List_Iterator::next ()
{
  return (Criterion::_downcast (iterator_.next ()));
}

void
Criterion_List_Iterator::advance ()
{
  (void) iterator_.advance ();
}
