#include "Event_Types_impl.h"
#include "ace/OS_NS_sys_time.h"

// Implementation of the valuetype member functions.

Event_impl::Event_impl ()
  // initializers (': foo ()') don't work for OBV state members
  // since we should only access the state through modifier functions
{
  // Put a timestamp on event's birth.
  ACE_Time_Value const now (ACE_OS::gettimeofday ());
  this->time_ (static_cast<CORBA::Long> (now.sec ()));
}

::CORBA::ValueBase *
Event_impl::_copy_value ()
{
  // Not implemented
  return nullptr;
}

void
Event_impl::do_print ()
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

Temperature_impl::Temperature_impl (CORBA::Float temp)
{
  this->temperature_ (temp);
}

::CORBA::ValueBase *
Temperature_impl::_copy_value ()
{
  // Not implemented
  return nullptr;
}

void
Temperature_impl::do_print ()
{
  // The timestamp
  Event_impl::do_print ();

  ACE_DEBUG((LM_DEBUG, "Temperature is %f\n", this->temperature_() ));
}

TAO_OBV_CREATE_RETURN_TYPE (Temperature)
Temperature_factory::create_for_unmarshal ()
{
  return new Temperature_impl;
}

// Position implementation  ======================================

Position_impl::Position_impl (Point const &p)
{
  this->xyz (p);
  // Does a copy of the p array
}

::CORBA::ValueBase *
Position_impl::_copy_value ()
{
  // Not implemented
  return nullptr;
}

void
Position_impl::do_print ()
{
  // The timestamp
  Event_impl::do_print ();

  CORBA::Float x =
    this->x();
  CORBA::Float y =
    this->y();
  CORBA::Float z =
    this->z();
  ACE_DEBUG((LM_DEBUG, "Position is (%f, %f, %f)\n",
             x, y, z));
}

CORBA::Float Position_impl::x ()
{
  return this->xyz()[0];
}

void Position_impl::x (CORBA::Float x)
{
  this->xyz()[0] = x;
}

CORBA::Float Position_impl::y ()
{
  return this->xyz()[1];
}

void Position_impl::y (CORBA::Float y)
{
  this->xyz()[1] = y;
}

CORBA::Float Position_impl::z ()
{
  return this->xyz()[2];
}

void Position_impl::z (CORBA::Float z)
{
  this->xyz()[2] = z;
}

TAO_OBV_CREATE_RETURN_TYPE (Position)
Position_factory::create_for_unmarshal ()
{
  return new Position_impl;
}

// Log_Msg implementation  ===================================

Log_Msg_impl::Log_Msg_impl (CORBA::Short u, char const *m)
{
  this->urgency (u);
  this->message (CORBA::string_dup (m));
}

::CORBA::ValueBase *
Log_Msg_impl::_copy_value ()
{
  // Not implemented
  return nullptr;
}

void
Log_Msg_impl::do_print ()
{
  // The timestamp
  Event_impl::do_print ();

  if (this->urgency () > 0)
    {
      ACE_DEBUG((LM_DEBUG, "**** %C ****\n", this->message () ));
    }
  else
    {
      ACE_DEBUG((LM_DEBUG, "%C\n", this->message () ));
    }
}

TAO_OBV_CREATE_RETURN_TYPE (Log_Msg)
Log_Msg_factory::create_for_unmarshal ()
{
  return new Log_Msg_impl;
}

// Event_List_Link implementation  ===================================

Event_List_Link_impl::Event_List_Link_impl ()
{
  this->my_event (nullptr);
  this->next (nullptr);
}

Event_List_Link_impl::Event_List_Link_impl (Event *e)
{
  this->my_event (e);
  // Note that the modifier increases the reference counter of e.

  this->next (nullptr);
}

::CORBA::ValueBase *
Event_List_Link_impl::_copy_value ()
{
  // Not implemented
  return nullptr;
}

Event*
Event_List_Link_impl::get_event ()
{
  return this->my_event ();
}

Event_List_Link*
Event_List_Link_impl::get_next_link ()
{
  return this->next ();
}

void
Event_List_Link_impl::attach_next_link (Event_List_Link *n)
{
  this->next (n);
}

TAO_OBV_CREATE_RETURN_TYPE (Event_List_Link)
Event_List_Link_factory::create_for_unmarshal ()
{
  return new Event_List_Link_impl;
}

// Event_List implementation  ===================================

Event_List_impl::Event_List_impl ()
{
  this->first_link (nullptr);
}

::CORBA::ValueBase *
Event_List_impl::_copy_value ()
{
  // Not implemented
  return nullptr;
}

void
Event_List_impl::store_event (Event *e)
{
  // This operation should perform atomically and should
  // guard against the access to the list from another thread.
  // But this is omitted in this example.

  Event_List_Link_impl *new_link_impl = nullptr;
  ACE_NEW (new_link_impl,
           Event_List_Link_impl (e));
  Event_List_Link_var new_link = new_link_impl;

   // We need a new link to store the reference to the event e.
   // But if we'd had assigned the newly created instance directly through
   // a modifier function to a valuetype member, it would never be released.
   // From pointer to _var the reference count of the pointed to never
   // increased, but the modifier does.

  if (this->last_link_cache_ == nullptr)
    {
      // Search the end.
      for (Event_List_Link *i = this->first_link ();
           i != nullptr;
           i = i->get_next_link ())
        {
          this->last_link_cache_ = i;
        }
    }

  // If still null then the list is empty.
  if (this->last_link_cache_ == nullptr)
    {
      this->first_link (new_link);
    }
  else
    {
      this->last_link_cache_->attach_next_link (new_link);
      this->last_link_cache_ = new_link.in ();
    }
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
      return true;
    }
  else
    {
      return false;
    }
}

Event *
Event_List_Iterator::next ()
{
  if (this->current_.in ())
    {
      return current_->get_event ();
    }
  return nullptr;
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

Temperature_Criterion_impl::
Temperature_Criterion_impl (CORBA::ULong origin_id, CORBA::Float temp)
{
  this->origin_id_ (origin_id);
  Temperature_var tmp (static_cast<Temperature*> (new Temperature_impl (temp)));
  this->meltingpoint (tmp.in ());
}

::CORBA::ValueBase *
Temperature_Criterion_impl::_copy_value ()
{
  // Not implemented
  return nullptr;
}

CORBA::Boolean
Temperature_Criterion_impl::is_critical (Event *e)
{
  // Downcast to a temperature.
  Temperature* t = Temperature::_downcast (e);
  // Is Event really a Temperature ?
  if (t)
    {
      // Now return the comparison with the meltingpoint.
      return t->temperature_ () > this->meltingpoint ()->temperature_ ();
    }
  return false;
}

void
Temperature_Criterion_impl::do_print ()
{
  ACE_DEBUG((LM_DEBUG, "Alarm boundary for events with origin id %d is\n",
             this->origin_id_ () ));
  this->meltingpoint ()->do_print();
}

TAO_OBV_CREATE_RETURN_TYPE (Temperature_Criterion)
Temperature_Criterion_factory::create_for_unmarshal ()
{
  return new Temperature_Criterion_impl;
}

Position_Criterion_impl::Position_Criterion_impl (CORBA::ULong origin_id,
                                                  Position *lb,
                                                  Position *tr)
{
  this->origin_id_ (origin_id);
  this->leftbottom(lb);
  this->topright(tr);
}

::CORBA::ValueBase *
Position_Criterion_impl::_copy_value ()
{
  // Not implemented
  return nullptr;
}

CORBA::Boolean
Position_Criterion_impl::is_critical (Event *e)
{
  Position* p = Position::_downcast (e);
  // Is Event really a Position ?
  if (p)
    {
      // If the position of p is out of the box return true.
      return !(
              (p->x () > this->leftbottom ()->x ()) &&
              (p->y () > this->leftbottom ()->y ()) &&
              (p->z () > this->leftbottom ()->z ()) &&
              (p->x () < this->topright ()->x ()) &&
              (p->y () < this->topright ()->y ()) &&
              (p->z () < this->topright ()->z ())
              );
    }
  return false;
}

void
Position_Criterion_impl::do_print ( )
{
  ACE_DEBUG((LM_DEBUG,
             "Alarm boundary for events with origin id %d is the box\n",
             this->origin_id_ () ));
  this->leftbottom ()->do_print();
  this->topright ()->do_print();
}

TAO_OBV_CREATE_RETURN_TYPE (Position_Criterion)
Position_Criterion_factory::create_for_unmarshal ()
{
  return new Position_Criterion_impl;
}

::CORBA::ValueBase *
Log_Msg_Criterion_impl::_copy_value ()
{
  // Not implemented
  return nullptr;
}

CORBA::Boolean
Log_Msg_Criterion_impl::is_critical (Event *e)
{
  Log_Msg* lm = Log_Msg::_downcast (e);

  // Is Event really a Log_Msg ?
  if (lm)
    {
      return lm->urgency () != 0;
    }
  return false;
}

void
Log_Msg_Criterion_impl::do_print ()
{
  ACE_DEBUG((LM_DEBUG,
             "All log messages with urgency greater zero are registered.\n" ));
}

TAO_OBV_CREATE_RETURN_TYPE (Log_Msg_Criterion)
Log_Msg_Criterion_factory::create_for_unmarshal ()
{
  return new Log_Msg_Criterion_impl;
}

Criterion_List_impl::Criterion_List_impl ()
{
  this->my_list (nullptr);
  // We don't want create our list member here, because this constructor
  // is called before unmarshalling too. We cant't distinguish that.
  // If we transmit this type eventually. This is not done in this example.
  // (That is no weakness of OBV since the instance should be created and
  // initialized through the factory create (),
  // which is not yet implemented %!)
}

::CORBA::ValueBase *
Criterion_List_impl::_copy_value ()
{
  // Not implemented
  return nullptr;
}

void
Criterion_List_impl::store_criterion (Criterion *c)
{
  if (!my_list ())
    {
      Event_List_var ev(static_cast<Event_List*> (new Event_List_impl));
      my_list (ev);
    }

  Event *e = Event::_downcast (c);
  my_list ()->store_event (e);
}

CORBA::Boolean
Criterion_List_impl::is_critical (Event *e)
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
            return true;
        }
    }
  return false;
}

Criterion_List_Iterator::Criterion_List_Iterator (Criterion_List *list)
{
  this->init (list);
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
