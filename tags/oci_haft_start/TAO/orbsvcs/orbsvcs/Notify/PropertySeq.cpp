// $Id$

#include "PropertySeq.h"

#if ! defined (__ACE_INLINE__)
#include "PropertySeq.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_PropertySeq, "$Id$")

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
TAO_Notify_PropertySeq::populate (CosNotification::PropertySeq_var& prop_seq)
{
  PROPERTY_MAP::ITERATOR iterator (this->property_map_);

  int index = prop_seq->length ();
  prop_seq->length (ACE_static_cast (CORBA::ULong,
                        index + this->property_map_.current_size ()));

  for (PROPERTY_MAP::ENTRY *entry = 0;
       iterator.next (entry) != 0;
       iterator.advance (), ++index)
    {
      (*prop_seq)[index].name = CORBA::string_dup (entry->ext_id_.c_str ());
      (*prop_seq)[index].value = entry->int_id_;
    }

  return 0;
}
