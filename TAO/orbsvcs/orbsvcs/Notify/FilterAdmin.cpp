/* -*- C++ -*- $Id$ */

#include "orbsvcs/Notify/FilterAdmin.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/FilterAdmin.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/Notify/Topology_Saver.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/EventChannelFactory.h"
#include "orbsvcs/Notify/FilterFactory.h"
#include "ace/Bound_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Implementation skeleton constructor
TAO_Notify_FilterAdmin::TAO_Notify_FilterAdmin (void)
{
}


// Implementation skeleton destructor
TAO_Notify_FilterAdmin::~TAO_Notify_FilterAdmin (void)
{
}

CosNotifyFilter::FilterID
TAO_Notify_FilterAdmin::add_filter (CosNotifyFilter::Filter_ptr new_filter)
{
  if (CORBA::is_nil (new_filter))
    throw CORBA::BAD_PARAM ();

  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  CosNotifyFilter::FilterID new_id = this->filter_ids_.id ();

  CosNotifyFilter::Filter_var new_filter_var =
    CosNotifyFilter::Filter::_duplicate (new_filter);

  if (this->filter_list_.bind (new_id, new_filter_var) == -1)
      throw CORBA::INTERNAL ();
  else
    return new_id;
}

void
TAO_Notify_FilterAdmin::remove_filter (CosNotifyFilter::FilterID filter_id)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  if (this->filter_list_.unbind (filter_id) == -1)
    throw CosNotifyFilter::FilterNotFound ();
}

CosNotifyFilter::Filter_ptr
TAO_Notify_FilterAdmin::get_filter (CosNotifyFilter::FilterID filter_id)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  CosNotifyFilter::Filter_var filter_var;

  if (this->filter_list_.find (filter_id,
                               filter_var) == -1)
    throw CosNotifyFilter::FilterNotFound ();

  return filter_var._retn ();
}

CosNotifyFilter::FilterIDSeq*
TAO_Notify_FilterAdmin::get_all_filters (void)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  // Figure out the length of the list.
  size_t len = this->filter_list_.current_size ();

  CosNotifyFilter::FilterIDSeq* list_ptr = 0;

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list_ptr,
                    CosNotifyFilter::FilterIDSeq,
                    CORBA::NO_MEMORY ());

  CosNotifyFilter::FilterIDSeq_var list (list_ptr);

  list->length (static_cast<CORBA::ULong> (len));

  FILTER_LIST::ITERATOR iter (this->filter_list_);
  FILTER_LIST::ENTRY *entry;

  u_int index;

  for (index = 0; iter.next (entry) != 0; iter.advance (), ++index)
    {
      list[index] = entry->ext_id_;
    }

  return list._retn ();
}

void
TAO_Notify_FilterAdmin::remove_all_filters (void)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  this->filter_list_.unbind_all ();
}

void
TAO_Notify_FilterAdmin::save_persistent (TAO_Notify::Topology_Saver& saver)
{
  if (this->filter_list_.current_size() == 0)
    return;

  bool changed = true;

  TAO_Notify::NVPList attrs;
  bool want_children = saver.begin_object(0, "filter_admin", attrs, changed);
  if (want_children)
  {
    FILTER_LIST::ITERATOR iter (this->filter_list_);
    FILTER_LIST::ENTRY* entry;

    TAO_Notify_Properties* properties = TAO_Notify_PROPERTIES::instance();
    CORBA::ORB_var orb = properties->orb();
    ACE_ASSERT(! CORBA::is_nil(orb.in()));

    for (; iter.next(entry) != 0; iter.advance())
    {
      TAO_Notify::NVPList fattrs;
      CORBA::Long id = entry->ext_id_;

      //TBD: this presume the filter always collocated.
      //otherwise we need modify the filter interface to add get_filter_id()

      TAO_Notify_FilterFactory* factory = ec_->default_filter_factory_servant ();
      TAO_Notify_Object::ID mapid = factory->get_filter_id (entry->int_id_.in ());

      fattrs.push_back(TAO_Notify::NVP("MapId", mapid));
      saver.begin_object(id, "filter", fattrs, changed);
      saver.end_object(id, "filter");
    }
  }

  saver.end_object(0, "filter_admin");
}

TAO_Notify::Topology_Object*
TAO_Notify_FilterAdmin::load_child (const ACE_CString &type, CORBA::Long id,
  const TAO_Notify::NVPList& attrs)
{
  if (type == "filter")
  {
   TAO_Notify_Object::ID mapid = 0;
   attrs.load("MapId", mapid);

   TAO_Notify_FilterFactory* factory = ec_->default_filter_factory_servant ();
   CosNotifyFilter::Filter_var filter = factory->get_filter (mapid);
 
    if (! CORBA::is_nil(filter.in()))
    {
      this->filter_ids_.set_last_used(id);
      if (this->filter_list_.bind (id, filter) != 0)
        throw CORBA::INTERNAL ();
    }
  }
  return this;
}

void
TAO_Notify_FilterAdmin::release (void)
{
  delete this;
}

void
TAO_Notify_FilterAdmin::event_channel (TAO_Notify_EventChannel* ec)
{
  this->ec_.reset (ec);
}


TAO_END_VERSIONED_NAMESPACE_DECL
