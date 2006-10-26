/* -*- C++ -*- $Id$ */

#include "orbsvcs/Notify/FilterAdmin.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/FilterAdmin.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, FilterAdmin, "$Id$")

#include "orbsvcs/Notify/Topology_Saver.h"
#include "orbsvcs/Notify/Properties.h"
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
TAO_Notify_FilterAdmin::add_filter (CosNotifyFilter::Filter_ptr new_filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (new_filter))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  CosNotifyFilter::FilterID new_id = this->filter_ids_.id ();

  CosNotifyFilter::Filter_var new_filter_var =
    CosNotifyFilter::Filter::_duplicate (new_filter);

  if (this->filter_list_.bind (new_id, new_filter_var) == -1)
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
  else
    return new_id;
}

void
TAO_Notify_FilterAdmin::remove_filter (CosNotifyFilter::FilterID filter_id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  if (this->filter_list_.unbind (filter_id) == -1)
    ACE_THROW (CosNotifyFilter::FilterNotFound ());
}

CosNotifyFilter::Filter_ptr
TAO_Notify_FilterAdmin::get_filter (CosNotifyFilter::FilterID filter_id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CosNotifyFilter::Filter::_nil ());

  CosNotifyFilter::Filter_var filter_var;

  if (this->filter_list_.find (filter_id,
                               filter_var) == -1)
    ACE_THROW_RETURN (CosNotifyFilter::FilterNotFound (),
                      0);

  return filter_var._retn ();
}

CosNotifyFilter::FilterIDSeq*
TAO_Notify_FilterAdmin::get_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  // Figure out the length of the list.
  size_t len = this->filter_list_.current_size ();

  CosNotifyFilter::FilterIDSeq* list_ptr;

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list_ptr,
                    CosNotifyFilter::FilterIDSeq,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

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
TAO_Notify_FilterAdmin::remove_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  this->filter_list_.unbind_all ();
}

void
TAO_Notify_FilterAdmin::save_persistent (TAO_Notify::Topology_Saver& saver ACE_ENV_ARG_DECL)
{
  if (this->filter_list_.current_size() == 0)
    return;

  bool changed = true;

  TAO_Notify::NVPList attrs;
  bool want_children = saver.begin_object(0, "filter_admin", attrs, changed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
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
      CORBA::String_var ior = orb->object_to_string(entry->int_id_.in() ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      fattrs.push_back(TAO_Notify::NVP("IOR", ior.in()));
      saver.begin_object(id, "filter", fattrs, changed ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      saver.end_object(id, "filter" ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  }

  saver.end_object(0, "filter_admin" ACE_ENV_ARG_PARAMETER);
}

TAO_Notify::Topology_Object*
TAO_Notify_FilterAdmin::load_child (const ACE_CString &type, CORBA::Long id,
  const TAO_Notify::NVPList& attrs ACE_ENV_ARG_DECL)
{
  if (type == "filter")
  {
    TAO_Notify_Properties* properties = TAO_Notify_PROPERTIES::instance();
    CORBA::ORB_var orb = properties->orb();
    ACE_ASSERT(! CORBA::is_nil(orb.in()));
    ACE_CString ior;
    attrs.load("IOR", ior);

    CORBA::Object_var obj = orb->string_to_object(ior.c_str() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);
    CosNotifyFilter::Filter_var filter = CosNotifyFilter::Filter::_unchecked_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);
    if (! CORBA::is_nil(filter.in()))
    {
      this->filter_ids_.set_last_used(id);
      if (this->filter_list_.bind (id, filter) != 0)
        ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }
  }
  return this;
}

void
TAO_Notify_FilterAdmin::release (void)
{
  delete this;
}

TAO_END_VERSIONED_NAMESPACE_DECL
