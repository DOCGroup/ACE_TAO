// $Id$
//=============================================================================
/**
 *  @file    Detector.h
 *
 *  $Id$
 *
 *  This file implements classes to help manage the Property_Sets 
 *  defined in the Portable Object Group.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#include "ace/pre.h"
#include "PG_Property_Set_Helper.h"

/////////////////
// Everything you need to know about properties
//  typedef sequence<Property> Properties;
//  struct Property {
//      Name nam;
//    Value val;
//  };
//  typedef CosNaming::Name Name;
//  typedef any Value;
//  typedef sequence <NameComponent> CosNaming::Name;
//  struct NameComponent
//  {
//    Istring id;
//    // This is the name that is used to identify object references.
//
//    Istring kind;
//    // Stores any addtional info about the object reference.
//  };



//////////
// Encoder
Portable_Group::Property_Set::Encoder::Encoder ()
{
}

Portable_Group::Property_Set::Encoder::~Encoder ()
{
}


void Portable_Group::Property_Set::Encoder::add (
  const char * name,
  const CORBA::Any & value)
{
  NamedValue nv(name, value);
  values_.push_back(nv);
}

void Portable_Group::Property_Set::Encoder::encode (
  FT::Properties_var & property_set) const
{
  size_t count = values_.size();
  property_set->length(count);
  for( size_t nItem = 0; nItem < count; ++nItem )
  {
    const NamedValue & nv = values_[nItem];
    FT::Property & property = property_set[nItem];
    CosNaming::Name & nsName = property.nam;
    CORBA::Any & anyValue = property.val;
    // assign the value
    anyValue = (nv.second());

    // assign the name
    // TODO: This restricts the name to a single level with no "kind"
    // TODO: remove this restriction (?)
    nsName.length(1);
    CosNaming::NameComponent & nc = nsName[0];

    nc.id = CORBA::string_dup (nv.first().c_str());
    // nc.kind defaults to empty.  Leave it that way (for now)

  }
}

//////////
// Decoder
Portable_Group::Property_Set::Decoder::Decoder (const FT::Properties & property_set)
{
  size_t count = property_set.length();
  for (size_t nItem = 0; nItem < count; ++nItem)
  {
    const FT::Property & property = property_set[nItem];
    const CosNaming::Name & nsName = property.nam;
    // note assumption one level name with no kind
    // TODO: fix this
    const CosNaming::NameComponent & nc = nsName[0];
    ACE_CString name = nc.id;
    if (0 != values_.bind(name, property.val))
    {
      ACE_ERROR ((LM_ERROR,
        "%T %n: Property_set: bind failed.\n"
        ));
    }
  }
}

Portable_Group::Property_Set::Decoder::~Decoder ()
{
}

int Portable_Group::Property_Set::Decoder::find (
  const ACE_CString & key,
  CORBA::Any & anyValue)const
{
  int found = 0;
  if (0 == values_.find(key, anyValue))
  {
    found = 1;
  }
  return found;
}

int Portable_Group::Property_Set::test_encode_decode ()
{
  int result = 1;
  static const long testLong = 123456L;
  static const char * testLongKey = "MyLong";

  static const char * testString = "Now is the time for all good people.";
  static const char * testStringKey = "plover";

  static const double testDouble = 3.1415;
  static const char * testDoubleKey = "pi";

  FT::Properties_var property_set = new FT::Properties;
  //scope encoder to be sure its gone before decoding
  {
    Portable_Group::Property_Set::Encoder encoder;
    CORBA::Any value;
    value <<= (CORBA::Long) testLong;
    encoder.add (testLongKey, value);

    value <<= testString;
    encoder.add (testStringKey, value);

    value <<= (CORBA::Double) testDouble;
    encoder.add (testDoubleKey, value);

    encoder.encode(property_set);
  }

  Portable_Group::Property_Set::Decoder decoder(property_set);

  CORBA::Long longResult = 0;
  if (find (decoder, testLongKey, longResult) )
  {
    if (longResult != testLong)
    {
      ACE_ERROR ((LM_ERROR,
        "%T %n: %s = %d expecting %d\n",
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
      "%T %n: Can't find value for %s\n", testLongKey
      ));
    result = 0;
  }

  const char * stringResult = "";
  if (find (decoder, testStringKey, stringResult))
  {
    if(0 != ACE_OS::strcmp(testString, stringResult))
    {
      ACE_ERROR ((LM_ERROR,
        "%T %n: %s = \"%s\" expecting \"%s\"\n",
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
      "%T %n: Can't find value for %s\n", testStringKey
      ));
    result = 0;
  }


  CORBA::Double doubleResult = 0.0;
  if (find (decoder, testDoubleKey, doubleResult))
  {
    if(doubleResult != testDouble)
    {
      // ACE_ERROR doesn't format doubles, so...
      char buffer[200];
      ACE_OS::snprintf(buffer, sizeof(buffer),
              "%s = \"%f\" expecting \"%f\"\n",
          testDoubleKey,
          doubleResult,
          testDouble
        );
      ACE_ERROR ((LM_ERROR, 
        "%T %n: %s", buffer
        ));
      result = 0;
    }
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
      "%T %n: Can't find value for %s\n", testDoubleKey
      ));
    result = 0;
  }

  return result;
}
