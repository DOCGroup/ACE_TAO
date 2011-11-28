// $Id$

#include "orbsvcs/Notify/PropertySeq.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/PropertySeq.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_PropertySeq::TAO_Notify_PropertySeq (void)
{
}

TAO_Notify_PropertySeq::~TAO_Notify_PropertySeq ()
{
}

int
TAO_Notify_PropertySeq::init (const CosNotification::PropertySeq& prop_seq)
{
  ACE_CString name;

  for (CORBA::ULong i = 0; i < prop_seq.length (); ++i)
    {
      name = prop_seq[i].name.in ();

      if (this->property_map_.rebind (name, prop_seq[i].value) == -1)
        return -1;
    }
  // Note call to rebind. This allows to call <init> to set updates.

  return 0;
}

int
TAO_Notify_PropertySeq::populate (CosNotification::PropertySeq_var& prop_seq) const
{
  PROPERTY_MAP::CONST_ITERATOR iterator (this->property_map_);

  int index = prop_seq->length ();
  prop_seq->length (static_cast<CORBA::ULong> (index + this->property_map_.current_size ()));

  for (PROPERTY_MAP::ENTRY *entry = 0;
       iterator.next (entry) != 0;
       iterator.advance (), ++index)
    {
      (*prop_seq)[index].name = CORBA::string_dup (entry->ext_id_.c_str ());
      (*prop_seq)[index].value = entry->int_id_;
    }

  return 0;
}

void
TAO_Notify_PropertySeq::add(const ACE_CString& name, const CORBA::Any& val)
{
  int ret = this->property_map_.rebind (name, val);
  ACE_ASSERT(ret >= 0);
  ACE_UNUSED_ARG (ret); // because the assert disappears in release builds
}

TAO_END_VERSIONED_NAMESPACE_DECL
