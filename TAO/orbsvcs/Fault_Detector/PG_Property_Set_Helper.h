// $Id$
#ifndef PROPERTY_SET_HELPER_H_
#define PROPERTY_SET_HELPER_H_
#include "FT_DetectorFactory_i.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/PortableServer/ORB_Manager.h"

#include "ace/Map_T.h"
#include "ace/Vector_T.h"
#include "ace/Pair_T.h"

//////////////
// Notes comparing stl containers to ACE containers:
// objects stored in ACE_Vector must support operator =
// this means const objects cannot be stored in an ACE_Vector
// ACE_Pair uses accessor functions rather than public members
// for first and second.  The names have not been changed to confuse
// the innocent.
// ACE_Map_Manager is very un-stl like.  It's easier to understand
// if you first forget everything you know about stl::map.
//  Note: this is not a value judgement. ACE_Map_Manager has some
//  nice features.  Just don't expect to behave like an stl container.

namespace Portable_Group
{
  namespace Property_Set
  {

    // todo use ACE Containers
    typedef ACE_Map_Manager<std::string, CORBA::Any, ACE_SYNCH_NULL_MUTEX> ValueMap;
    typedef ACE_Pair< std::string, CORBA::Any> NamedValue;
    typedef ACE_Vector< NamedValue, 10 > NamedValueVec;

    class Encoder
    {
    public:
      Encoder ();
      ~Encoder ();

      void add (const char * name, const CORBA::Any & value);

      void encode (FT::Properties_var & property_set) const;

    private:
      Encoder (const Encoder & rhs);
      Encoder & operator = (const Encoder & rhs);
    private:
      NamedValueVec values_;
    };

    class Decoder
    {
    public:

      Decoder (FT::Properties_var & property_set);
      ~Decoder ();

      // general purpose method
      int find (const std::string & key, CORBA::Any & pValue)const;

      // convenience methods
      int find (const std::string & key, long & value)const;
      int find (const std::string & key, double & value)const;
      int find (const std::string & key, std::string & value)const;


    private:
      Decoder();
      Decoder(const Decoder & rhs);
      Decoder & operator = (const Decoder & rhs);

    private:
      ValueMap values_;
      FT::Properties_var property_set_;
    };

    /**
     * unit test: encode and decode properties.
     * Initialize CORBA before calling this function.
     * Success is silent, failure prints on cerr.
     * @returns 1 if test passed; 0 if test failed.
     */
    int test_encode_decode();

  } // namespace Property_Set
} //namespace Portable_Group

#endif // PROPERTY_SET_HELPER_H_

