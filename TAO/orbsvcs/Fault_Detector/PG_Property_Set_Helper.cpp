// $Id$
#include "PG_Property_Set_Helper.h"

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
Portable_Group::Property_Set::Decoder::Decoder (FT::Properties_var & property_set)
{
  size_t count = property_set->length();
  for (size_t nItem = 0; nItem < count; ++nItem)
  {
    const FT::Property & property = property_set[nItem];
    const CosNaming::Name & nsName = property.nam;
    // note assumption one level name with no kind
    // TODO: fix this
    const CosNaming::NameComponent & nc = nsName[0];
    std::string name = nc.id;
    if (0 != values_.bind(name, property.val))
    {
      std::cerr << "property_set: bind failed." << std::endl;
    }
  }
}

Portable_Group::Property_Set::Decoder::~Decoder ()
{
}

int Portable_Group::Property_Set::Decoder::find (
  const std::string & key,
  CORBA::Any & anyValue)const
{
  int found = 0;
  if (0 == values_.find(key, anyValue))
  {
    found = 1;
  }
  return found;
}

// convenience methods
int Portable_Group::Property_Set::Decoder::find (
  const std::string & key,
  long & value)const
{
  CORBA::Any any;
  int found = find (key, any);
  if (found)
  {
    CORBA::Long v;
    if ( any >>= v )
    {
      value = v;
    }
    else
    {
      found = 0;
    }
  }
  return found;
}

int Portable_Group::Property_Set::Decoder::find (
  const std::string & key,
  double & value)const
{
  CORBA::Any any;
  int found = find (key, any);
  if (found)
  {
    CORBA::Double v;
    if ( any >>= v )
    {
      value = v;
    }
    else
    {
      found = 0;
    }
  }
  return found;
}

int Portable_Group::Property_Set::Decoder::find (
  const std::string & key,
  std::string & value)const
{
  CORBA::Any any;
  int found = find (key, any);
  if (found)
  {
    const char * v;
    if ( any >>= v )
    {
      value = v;
    }
    else
    {
      found = 0;
    }
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

  long longResult;
  if (decoder.find (testLongKey, longResult) )
  {
    if (longResult != testLong)
    {
      std::cerr << testLongKey << " = " << longResult
        << " expecting " << testLong << std::endl;
      result = 0;
    }
  }
  else
  {
    std::cerr << "Can't find value for " << testLongKey << endl;
    result = 0;
  }

  std::string stringResult;
  if (decoder.find (testStringKey, stringResult))
  {
    if(stringResult != testString)
    {
      std::cerr << testStringKey << " = " << stringResult
        << " expecting " << testString << std::endl;
      result = 0;
    }
  }
  else
  {
    std::cerr << "Can't find value for " << testStringKey << endl;
    result = 0;
  }


  double doubleResult;
  if (decoder.find (testDoubleKey, doubleResult))
  {
    if(doubleResult != testDouble)
    {
      std::cerr << testDoubleKey << " = " << doubleResult
        << " expecting " << testDouble << std::endl;
      result = 0;
    }
  }
  else
  {
    std::cerr << "Can't find value for " << testDoubleKey << endl;
    result = 0;
  }

  return result;
}
