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
#include "ace/pre.h"
#include "PG_Properties_Decoder.h"


//////////////////////
// Properties_Decoder
TAO_PG::Properties_Decoder::Properties_Decoder (const PortableGroup::Properties & property_set)
{
  size_t count = property_set.length();
  for (size_t nItem = 0; nItem < count; ++nItem)
  {
    const PortableGroup::Property & property = property_set[nItem];
    const CosNaming::Name & nsName = property.nam;
    // note assumption one level name with no kind
    // TODO: fix this
    const CosNaming::NameComponent & nc = nsName[0];
    ACE_CString name = nc.id;
    if (0 != values_.bind(name, & property.val))
    {
      ACE_ERROR ((LM_ERROR,
        "%n\n%T: Property_set: bind failed.\n"
        ));
    }
  }
}

TAO_PG::Properties_Decoder::~Properties_Decoder ()
{
}

int TAO_PG::Properties_Decoder::find (
  const ACE_CString & key,
  PortableGroup::Value *& pValue)const
{
  int found = 0;
  if (0 == values_.find(key, pValue))
  {
    found = 1;
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

    encoder.encode(property_set);
  }

  TAO_PG::Properties_Decoder decoder(property_set);

  CORBA::Long longResult = 0;
  if (find (decoder, testLongKey, longResult) )
  {
    if (longResult != testLong)
    {
      ACE_ERROR ((LM_ERROR,
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
    ACE_ERROR ((LM_ERROR,
      "%n\n%T: Can't find value for %s\n", testLongKey
      ));
    result = 0;
  }

  const char * stringResult = "";
  if (find (decoder, testStringKey, stringResult))
  {
    if(0 != ACE_OS::strcmp(testString, stringResult))
    {
      ACE_ERROR ((LM_ERROR,
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
    ACE_ERROR ((LM_ERROR,
      "%n\n%T: Can't find value for %s\n", testStringKey
      ));
    result = 0;
  }


  CORBA::Double doubleResult = 0.0;
  if (find (decoder, testDoubleKey, doubleResult))
  {
    if(doubleResult != testDouble)
    {
      ACE_ERROR ((LM_ERROR,
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
    ACE_ERROR ((LM_ERROR,
      "%n\n%T: Can't find value for %s\n", testDoubleKey
      ));
    result = 0;
  }

  return result;
}
#endif // PG_PS_UNIT_TEST

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template ACE_Hash_Map_Manager<ACE_CString, PortableGroup::Value *, ACE_SYNCH_NULL_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Hash_Map_Manager<ACE_CString, PortableGroup::Value *, ACE_SYNCH_NULL_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
