/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    PG_Properties_Decoder.cpp
 *
 *  $Id$
 *
 *  This file implements classes to help manage the Properties
 *  defined in the Portable Object Group.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#include "PG_Properties_Decoder.h"

//////////////////////
// Properties_Decoder

TAO_PG::Properties_Decoder::Properties_Decoder()
  : defaults_ (0)
{
}


TAO_PG::Properties_Decoder::Properties_Decoder (const PortableGroup::Properties & property_set)
  : defaults_ (0)
{
  this->decode (property_set);
  // @@ do something about the no-memory exception
}

TAO_PG::Properties_Decoder::Properties_Decoder (
    const PortableGroup::Properties & property_set,
    Properties_Decoder * defaults)
  : defaults_ (defaults)
{
  this->decode (property_set);
  // @@ do something about the no-memory exception
}


TAO_PG::Properties_Decoder::Properties_Decoder (
    Properties_Decoder * defaults)
  : defaults_ (defaults)
{
}



TAO_PG::Properties_Decoder::~Properties_Decoder ()
{
  this->clear ();
}

void TAO_PG::Properties_Decoder::decode (const PortableGroup::Properties & property_set ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))

{
  size_t count = property_set.length ();
  for (size_t nItem = 0; nItem < count; ++nItem)
  {
    const PortableGroup::Property & property = property_set[nItem];
    const CosNaming::Name & nsName = property.nam;
    // note assumption one level name with no kind
    // @@ TODO: fix this
    const CosNaming::NameComponent & nc = nsName[0];
    ACE_CString name = ACE_static_cast (const char *, nc.id);

    const PortableGroup::Value * value_copy;
    ACE_NEW_THROW_EX (value_copy, PortableGroup::Value (property.val), CORBA::NO_MEMORY ());
    const PortableGroup::Value * replaced_value;
    if (0 == this->values_.rebind (name, value_copy, replaced_value))
    {
      if (0 != replaced_value)
      {
        delete replaced_value;
      }
    }
    else
    {
      if (TAO_debug_level > 3)
      {
        ACE_ERROR ( (LM_ERROR,
          "%n\n%T: Property_set: rebind failed.\n"
          ));
      }
      // @@ should throw something here
      ACE_THROW (CORBA::NO_MEMORY ());
    }
  }
}

void TAO_PG::Properties_Decoder::clear ()
{
  for (ValueMapIterator it = this->values_.begin ();
        it != this->values_.end ();
        ++it)
  {
    ACE_TRY_NEW_ENV
    {
      delete (*it).int_id_;
    }
    ACE_CATCHANY
    {
      // ignore this (we might log it?)
    }
    ACE_ENDTRY;
  }
  this->values_.unbind_all ();
}

void TAO_PG::Properties_Decoder::remove (const PortableGroup::Properties & property_set)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  size_t count = property_set.length ();
  for (size_t nItem = 0; nItem < count; ++nItem)
  {
    const PortableGroup::Property & property = property_set[nItem];
    const CosNaming::Name & nsName = property.nam;
    // note assumption one level name with no kind
    // @@ TODO: fix this
    const CosNaming::NameComponent & nc = nsName[0];
    ACE_CString name = ACE_static_cast (const char *, nc.id);

    const PortableGroup::Value * deleted_value;
    if ( 0 == this->values_.unbind (name, deleted_value))
    {
      delete deleted_value;
    }
    else
    {
      // don't worry about it.
    }
  }
}



void TAO_PG::Properties_Decoder::export_properties(PortableGroup::Properties & property_set) const
{
  ValueMap merged_values;
  this->merge_properties (merged_values);

  property_set.length (merged_values.current_size ());

  size_t pos = 0;
  // note AFICT ACE_Hash_Map does not support const iterators, hence the const cast.
  ValueMap & mutable_values = ACE_const_cast (ValueMap &, this->values_);
  for (ValueMapIterator it = mutable_values.begin ();
        it != mutable_values.end ();
        ++it)
  {
    const ACE_CString & name = (*it).ext_id_;
    const PortableGroup::Value * value = (*it).int_id_;

    PortableGroup::Property & property = property_set[pos];
    CosNaming::Name & nsName = property.nam;
    //@@ note assumption: single level name, no kind
    nsName.length(1);
    CosNaming::NameComponent & nc = nsName[0];
    nc.id = CORBA::string_dup (name.c_str ());
    PortableGroup::Value & val = property.val;
    val = *value;  // NOTE: Any assignment does a deep copy
    ++pos;
  }
  ACE_ASSERT (pos == property_set.length ());
}

void TAO_PG::Properties_Decoder::merge_properties (ValueMap & merged_values) const
{
  if (0 != this->defaults_)
  {
    this->defaults_->merge_properties (merged_values);
  }
  // note AFICT ACE_Hash_Map does not support const iterators, hence the const cast.
  ValueMap & mutable_values = ACE_const_cast (ValueMap &, this->values_);
  for (ValueMapIterator it = mutable_values.begin ();
        it != mutable_values.end ();
        ++it)
  {
    merged_values.rebind ( (*it).ext_id_, (*it).int_id_);
  }
}



int TAO_PG::Properties_Decoder::find (
  const ACE_CString & key,
  const PortableGroup::Value *& pValue) const
{
  int found = (0 == this->values_.find (key, pValue));
  if (! found)
  {
    if (0 != this->defaults_)
    {
      found = this->defaults_->find (key, pValue);
    }
  }
  return found;
}

//#define PG_PS_UNIT_TEST
#ifdef PG_PS_UNIT_TEST
#include "PG_Properties_Encoder.h"

int TAO_PG::test_encode_decode ()
{
  int result = 1;
  static const long testLong = 123456L;
  static const char * testLongKey = "MyLong";

  static const char * testString = "Now is the time for all good people.";
  static const char * testStringKey = "plover";

  static const double testDouble = 3.1415;
  static const char * testDoubleKey = "pi";

  PortableGroup::Properties_var property_set = new PortableGroup::Properties;
  //scope encoder to be sure its gone before decoding
  {
    TAO_PG::Encoder encoder;
    PortableGroup::Value value;
    value <<= (CORBA::Long) testLong;
    encoder.add (testLongKey, value);

    value <<= testString;
    encoder.add (testStringKey, value);

    value <<= (CORBA::Double) testDouble;
    encoder.add (testDoubleKey, value);

    encoder.encode (property_set);
  }

  TAO_PG::Properties_Decoder decoder (property_set);

  CORBA::Long longResult = 0;
  if (find (decoder, testLongKey, longResult) )
  {
    if (longResult != testLong)
    {
      ACE_ERROR ( (LM_ERROR,
        "%n\n%T: %s = %d expecting %d\n",
          testLongKey,
          (int)longResult,
          (int)testLong
        ));
      result = 0;
    }
  }
  else
  {
    ACE_ERROR ( (LM_ERROR,
      "%n\n%T: Can't find value for %s\n", testLongKey
      ));
    result = 0;
  }

  const char * stringResult = "";
  if (find (decoder, testStringKey, stringResult))
  {
    if (0 != ACE_OS::strcmp (testString, stringResult))
    {
      ACE_ERROR ( (LM_ERROR,
        "%n\n%T: %s = \"%s\" expecting \"%s\"\n",
          testStringKey,
          (int)stringResult,
          (int)testString
        ));
      result = 0;
    }
  }
  else
  {
    ACE_ERROR ( (LM_ERROR,
      "%n\n%T: Can't find value for %s\n", testStringKey
      ));
    result = 0;
  }


  CORBA::Double doubleResult = 0.0;
  if (find (decoder, testDoubleKey, doubleResult))
  {
    if (doubleResult != testDouble)
    {
      ACE_ERROR ( (LM_ERROR,
        "%n\n%T: %s = \"%f\" expecting \"%f\"\n",
          testDoubleKey,
          doubleResult,
          testDouble
        ));
      result = 0;
    }
  }
  else
  {
    ACE_ERROR ( (LM_ERROR,
      "%n\n%T: Can't find value for %s\n", testDoubleKey
      ));
    result = 0;
  }

  return result;
}
#endif // PG_PS_UNIT_TEST

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template class ACE_Hash_Map_Manager<
    ACE_CString,
    const PortableGroup::Value *,
    ACE_SYNCH_NULL_MUTEX>;

    typedef class ACE_Hash_Map_Iterator<
      ACE_CString,
      const PortableGroup::Value *,
      ACE_SYNCH_NULL_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate ACE_Hash_Map_Manager<
    ACE_CString,
    const PortableGroup::Value *,
    ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<
      ACE_CString,
      const PortableGroup::Value *,
      ACE_SYNCH_NULL_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
