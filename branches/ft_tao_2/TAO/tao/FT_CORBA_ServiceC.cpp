/* -*- C++ -*- $Id$ */
#include "FT_CORBA_ServiceC.h"

// This has been got from the generated code. So, the file may look
// messy. Need to clean it slowly -- Bala

#if (TAO_HAS_FT_CORBA == 1)

#if !defined (__ACE_INLINE__)
#include "FT_CORBA_ServiceC.i"
#endif /* !defined INLINE */

static const CORBA::Long _oc_FT_Name[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  24,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4e),
  ACE_NTOHL (0x616d653a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_alias, // typecode kind for typedefs
  312, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d653a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_sequence, // typecode kind
  252, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  236, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d65436f),
  ACE_NTOHL (0x6d706f6e),
  ACE_NTOHL (0x656e743a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
  14,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x436f6d70),
  ACE_NTOHL (0x6f6e656e),
  ACE_NTOHL (0x74000000),  // name = NameComponent
  2, // member count
  3, ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  5,
  ACE_NTOHL (0x6b696e64),
  ACE_NTOHL (0x0),  // name = kind
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  0U,
};


static CORBA::TypeCode _tc_TAO_tc_FT_Name (CORBA::tk_alias, sizeof (_oc_FT_Name), (char *) &_oc_FT_Name, 0, sizeof (FT::Name));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_Name, &_tc_TAO_tc_FT_Name)
TAO_NAMESPACE_END

static const CORBA::Long _oc_FT_Property[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  28,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f50),
  ACE_NTOHL (0x726f7065),
  ACE_NTOHL (0x7274793a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Property:1.0
  9,
  ACE_NTOHL (0x50726f70),
  ACE_NTOHL (0x65727479),
  ACE_NTOHL (0x0),  // name = Property
  2, // member count
  4,
  ACE_NTOHL (0x6e616d00),  // name = nam
  CORBA::tk_alias, // typecode kind for typedefs
  364, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  24,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4e),
  ACE_NTOHL (0x616d653a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_alias, // typecode kind for typedefs
  312, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d653a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_sequence, // typecode kind
  252, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  236, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d65436f),
  ACE_NTOHL (0x6d706f6e),
  ACE_NTOHL (0x656e743a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
  14,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x436f6d70),
  ACE_NTOHL (0x6f6e656e),
  ACE_NTOHL (0x74000000),  // name = NameComponent
  2, // member count
  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  5,
  ACE_NTOHL (0x6b696e64),
  ACE_NTOHL (0x0),  // name = kind
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  0U,
  4,
  ACE_NTOHL (0x76616c00),  // name = val
  CORBA::tk_alias, // typecode kind for typedefs
  52, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  25,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f56),
  ACE_NTOHL (0x616c7565),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/Value:1.0
  6,
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x65000000),  // name = Value
  CORBA::tk_any,
};

static CORBA::TypeCode _tc_TAO_tc_FT_Property (CORBA::tk_struct, sizeof (_oc_FT_Property), (char *) &_oc_FT_Property, 0, sizeof (FT::Property));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_Property, &_tc_TAO_tc_FT_Property)
TAO_NAMESPACE_END
void FT::Property::_tao_any_destructor (void *x)
{
  FT_Property *tmp = ACE_static_cast (FT_Property*,x);
  delete tmp;
}

// *************************************************************
// FT_Name
// *************************************************************

FT_Name::FT_Name (void)
{}

FT_Name::FT_Name (CORBA::ULong max) // uses max size
  : CosNaming::Name (max)
{}

FT_Name::FT_Name (CORBA::ULong max,
                  CORBA::ULong length,
                  CosNaming::NameComponent *buffer,
                  CORBA::Boolean release)
  : CosNaming::Name (max, length, buffer, release)
{}


FT_Name::FT_Name (const FT_Name &seq) // copy ctor
  :CosNaming::Name (seq)
{}


FT_Name::~FT_Name (void) // dtor
{}


void FT_Name::_tao_any_destructor (void *x)
{
  FT_Name *tmp = ACE_static_cast (FT_Name*,x);
  delete tmp;
}

#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

void
_TAO_Unbounded_Sequence_FT_Properties::_allocate_buffer (CORBA::ULong length)
{
  FT::Property* tmp = 0;
  tmp = _TAO_Unbounded_Sequence_FT_Properties::allocbuf (length);

  if (this->buffer_ != 0)
    {
      FT::Property *old = ACE_reinterpret_cast (FT::Property *,this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        tmp[i] = old[i];

      if (this->release_)
        _TAO_Unbounded_Sequence_FT_Properties::freebuf (old);

    }
  this->buffer_ = tmp;
}

void
_TAO_Unbounded_Sequence_FT_Properties::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;

  FT::Property *tmp = ACE_reinterpret_cast (FT::Property *,this->buffer_);

  _TAO_Unbounded_Sequence_FT_Properties::freebuf (tmp);
  this->buffer_ = 0;
}

_TAO_Unbounded_Sequence_FT_Properties::~_TAO_Unbounded_Sequence_FT_Properties (void) // Dtor.
{
  this->_deallocate_buffer ();
}

#endif /* !TAO_USE_SEQUENCE_TEMPLATES */


// *************************************************************
// FT::Properties
// *************************************************************

FT_Properties::FT_Properties (void)
{}
FT_Properties::FT_Properties (CORBA::ULong max) // uses max size
  :
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
_TAO_Unbounded_Sequence_FT_Properties
#else /* TAO_USE_SEQUENCE_TEMPLATES */
TAO_Unbounded_Sequence<FT::Property>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
(max)
{}

FT_Properties::FT_Properties (CORBA::ULong max,
                               CORBA::ULong length,
                               FT::Property *buffer,
                               CORBA::Boolean release)
  :
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
  _TAO_Unbounded_Sequence_FT_Properties
#else /* TAO_USE_SEQUENCE_TEMPLATES */
  TAO_Unbounded_Sequence<FT::Property>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
 (max, length, buffer, release)
{}
FT_Properties::FT_Properties (const FT_Properties &seq) // copy ctor
  :
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
_TAO_Unbounded_Sequence_FT_Properties
#else /* TAO_USE_SEQUENCE_TEMPLATES */
TAO_Unbounded_Sequence<FT::Property>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
 (seq)
{}

FT_Properties::~FT_Properties (void) // dtor
{}

void FT_Properties::_tao_any_destructor (void *x)
{
  FT_Properties *tmp = ACE_static_cast (FT_Properties*,x);
  delete tmp;
}


static const CORBA::Long _oc_FT_Properties[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  30,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f50),
  ACE_NTOHL (0x726f7065),
  ACE_NTOHL (0x72746965),
  ACE_NTOHL (0x733a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/Properties:1.0
  11,
  ACE_NTOHL (0x50726f70),
  ACE_NTOHL (0x65727469),
  ACE_NTOHL (0x65730000),  // name = Properties
  CORBA::tk_sequence, // typecode kind
  520, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  504, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  28,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f50),
  ACE_NTOHL (0x726f7065),
  ACE_NTOHL (0x7274793a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Property:1.0
  9,
  ACE_NTOHL (0x50726f70),
  ACE_NTOHL (0x65727479),
  ACE_NTOHL (0x0),  // name = Property
  2, // member count
  4,
  ACE_NTOHL (0x6e616d00),  // name = nam
  CORBA::tk_alias, // typecode kind for typedefs
  364, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  24,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4e),
  ACE_NTOHL (0x616d653a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
  5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_alias, // typecode kind for typedefs
  312, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d653a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
  5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_sequence, // typecode kind
  252, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  236, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d65436f),
  ACE_NTOHL (0x6d706f6e),
  ACE_NTOHL (0x656e743a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
  14,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x436f6d70),
  ACE_NTOHL (0x6f6e656e),
  ACE_NTOHL (0x74000000),  // name = NameComponent
  2, // member count
  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  5,
  ACE_NTOHL (0x6b696e64),
  ACE_NTOHL (0x0),  // name = kind
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  0U,
  4,
  ACE_NTOHL (0x76616c00),  // name = val
  CORBA::tk_alias, // typecode kind for typedefs
  52, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  25, ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f56),
  ACE_NTOHL (0x616c7565),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/Value:1.0
  6,
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x65000000),  // name = Value
  CORBA::tk_any,
  0U,
};

static CORBA::TypeCode _tc_TAO_tc_FT_Properties (CORBA::tk_alias, sizeof (_oc_FT_Properties), (char *) &_oc_FT_Properties, 0, sizeof (FT_Properties));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_Properties, &_tc_TAO_tc_FT_Properties)
TAO_NAMESPACE_END
static const CORBA::Long _oc_FT_Location[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  28,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4c),
  ACE_NTOHL (0x6f636174),
  ACE_NTOHL (0x696f6e3a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Location:1.0
  9,
  ACE_NTOHL (0x4c6f6361),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x0),  // name = Location
  CORBA::tk_alias, // typecode kind for typedefs
  364, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  24,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4e),
  ACE_NTOHL (0x616d653a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_alias, // typecode kind for typedefs
  312, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d653a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_sequence, // typecode kind
  252, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  236, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d65436f),
  ACE_NTOHL (0x6d706f6e),
  ACE_NTOHL (0x656e743a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
  14,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x436f6d70),
  ACE_NTOHL (0x6f6e656e),
  ACE_NTOHL (0x74000000),  // name = NameComponent
  2, // member count
  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  5,
  ACE_NTOHL (0x6b696e64),
  ACE_NTOHL (0x0),  // name = kind
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  0U,
};


static CORBA::TypeCode _tc_TAO_tc_FT_Location (CORBA::tk_alias,
                                               sizeof
                                               (_oc_FT_Location),
                                               (char *)
                                               &_oc_FT_Location,
                                               0,
                                               sizeof (FT::Location));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_Location, &_tc_TAO_tc_FT_Location)
TAO_NAMESPACE_END

#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

void
_TAO_Unbounded_Sequence_FT_Locations::_allocate_buffer (CORBA::ULong length)
{
  FT::Location* tmp = 0;
  tmp = _TAO_Unbounded_Sequence_FT_Locations::allocbuf (length);

  if (this->buffer_ != 0)
    {
      FT::Location *old = ACE_reinterpret_cast (FT::Location *,this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        tmp[i] = old[i];

      if (this->release_)
        _TAO_Unbounded_Sequence_FT_Locations::freebuf (old);

    }
  this->buffer_ = tmp;
}

void
_TAO_Unbounded_Sequence_FT_Locations::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;

  FT::Location *tmp = ACE_reinterpret_cast (FT::Location *,this->buffer_);

  _TAO_Unbounded_Sequence_FT_Locations::freebuf (tmp);
  this->buffer_ = 0;
}

_TAO_Unbounded_Sequence_FT_Locations::~_TAO_Unbounded_Sequence_FT_Locations (void) // Dtor.
{
  this->_deallocate_buffer ();
}

#endif /* !TAO_USE_SEQUENCE_TEMPLATES */


// *************************************************************
// FT_Locations
// *************************************************************

FT_Locations::FT_Locations (void)
{}
FT_Locations::FT_Locations (CORBA::ULong max) // uses max size
  :
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
  _TAO_Unbounded_Sequence_FT_Locations
#else /* TAO_USE_SEQUENCE_TEMPLATES */
  TAO_Unbounded_Sequence<FT::Location>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
 (max)
{}
FT_Locations::FT_Locations (CORBA::ULong max,
                            CORBA::ULong length,
                            FT::Location *buffer,
                            CORBA::Boolean release)
  :
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
  _TAO_Unbounded_Sequence_FT_Locations
#else /* TAO_USE_SEQUENCE_TEMPLATES */
  TAO_Unbounded_Sequence<FT::Location>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
 (max, length, buffer, release)
{}
FT_Locations::FT_Locations (const FT_Locations &seq) // copy ctor
  :
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
  _TAO_Unbounded_Sequence_FT_Locations
#else /* TAO_USE_SEQUENCE_TEMPLATES */
  TAO_Unbounded_Sequence<FT::Location>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
 (seq)
{}
FT_Locations::~FT_Locations (void) // dtor
{}
void FT_Locations::_tao_any_destructor (void *x)
{
  FT_Locations *tmp = ACE_static_cast (FT_Locations*,x);
  delete tmp;
}


static const CORBA::Long _oc_FT_Locations[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  29,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4c),
  ACE_NTOHL (0x6f636174),
  ACE_NTOHL (0x696f6e73),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/Locations:1.0
  10,
  ACE_NTOHL (0x4c6f6361),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x73000000),  // name = Locations
  CORBA::tk_sequence, // typecode kind
  440, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_alias, // typecode kind for typedefs
  424, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  28,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4c),
  ACE_NTOHL (0x6f636174),
  ACE_NTOHL (0x696f6e3a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Location:1.0
  9,
  ACE_NTOHL (0x4c6f6361),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x0),  // name = Location
  CORBA::tk_alias, // typecode kind for typedefs
  364, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  24,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4e),
  ACE_NTOHL (0x616d653a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_alias, // typecode kind for typedefs
  312, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d653a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_sequence, // typecode kind
  252, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  236, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d65436f),
  ACE_NTOHL (0x6d706f6e),
  ACE_NTOHL (0x656e743a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
  14,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x436f6d70),
  ACE_NTOHL (0x6f6e656e),
  ACE_NTOHL (0x74000000),  // name = NameComponent
  2, // member count
  3, ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  5,
  ACE_NTOHL (0x6b696e64),
  ACE_NTOHL (0x0),  // name = kind
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  0U,
  0U,
};



static CORBA::TypeCode _tc_TAO_tc_FT_Locations (CORBA::tk_alias, sizeof (_oc_FT_Locations), (char *) &_oc_FT_Locations, 0, sizeof (FT_Locations));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_Locations, &_tc_TAO_tc_FT_Locations)
TAO_NAMESPACE_END
static const CORBA::Long _oc_FT_Criteria[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  28, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f43), ACE_NTOHL (0x72697465), ACE_NTOHL (0x7269613a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Criteria:1.0
  9, ACE_NTOHL (0x43726974), ACE_NTOHL (0x65726961), ACE_NTOHL (0x0),  // name = Criteria
  CORBA::tk_alias, // typecode kind for typedefs
  584, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    30, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f50), ACE_NTOHL (0x726f7065), ACE_NTOHL (0x72746965), ACE_NTOHL (0x733a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/Properties:1.0
    11, ACE_NTOHL (0x50726f70), ACE_NTOHL (0x65727469), ACE_NTOHL (0x65730000),  // name = Properties
    CORBA::tk_sequence, // typecode kind
    520, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      CORBA::tk_struct, // typecode kind
      504, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        28, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f50), ACE_NTOHL (0x726f7065), ACE_NTOHL (0x7274793a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Property:1.0
        9, ACE_NTOHL (0x50726f70), ACE_NTOHL (0x65727479), ACE_NTOHL (0x0),  // name = Property
        2, // member count
        4, ACE_NTOHL (0x6e616d00),  // name = nam
        CORBA::tk_alias, // typecode kind for typedefs
        364, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          24, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4e), ACE_NTOHL (0x616d653a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
          5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
          CORBA::tk_alias, // typecode kind for typedefs
          312, // encapsulation length
            TAO_ENCAP_BYTE_ORDER, // byte order
            31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d653a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
            5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
            CORBA::tk_sequence, // typecode kind
            252, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              CORBA::tk_struct, // typecode kind
              236, // encapsulation length
                TAO_ENCAP_BYTE_ORDER, // byte order
                40, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d65436f), ACE_NTOHL (0x6d706f6e), ACE_NTOHL (0x656e743a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
                14, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x436f6d70), ACE_NTOHL (0x6f6e656e), ACE_NTOHL (0x74000000),  // name = NameComponent
                2, // member count
                3, ACE_NTOHL (0x69640000),  // name = id
                CORBA::tk_alias, // typecode kind for typedefs
                64, // encapsulation length
                  TAO_ENCAP_BYTE_ORDER, // byte order
                  34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4973), ACE_NTOHL (0x7472696e), ACE_NTOHL (0x673a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
                  8, ACE_NTOHL (0x49737472), ACE_NTOHL (0x696e6700),  // name = Istring
                  CORBA::tk_string,
                  0U, // string length

                5, ACE_NTOHL (0x6b696e64), ACE_NTOHL (0x0),  // name = kind
                CORBA::tk_alias, // typecode kind for typedefs
                64, // encapsulation length
                  TAO_ENCAP_BYTE_ORDER, // byte order
                  34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4973), ACE_NTOHL (0x7472696e), ACE_NTOHL (0x673a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
                  8, ACE_NTOHL (0x49737472), ACE_NTOHL (0x696e6700),  // name = Istring
                  CORBA::tk_string,
                  0U, // string length


              0U,



        4, ACE_NTOHL (0x76616c00),  // name = val
        CORBA::tk_alias, // typecode kind for typedefs
        52, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          25, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f56), ACE_NTOHL (0x616c7565), ACE_NTOHL (0x3a312e30), ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/Value:1.0
          6, ACE_NTOHL (0x56616c75), ACE_NTOHL (0x65000000),  // name = Value
          CORBA::tk_any,



      0U,


};



static CORBA::TypeCode _tc_TAO_tc_FT_Criteria (CORBA::tk_alias, sizeof (_oc_FT_Criteria), (char *) &_oc_FT_Criteria, 0, sizeof (FT_Criteria));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_Criteria, &_tc_TAO_tc_FT_Criteria)
TAO_NAMESPACE_END
static const CORBA::Long _oc_FT_FactoryInfo[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f46), ACE_NTOHL (0x6163746f), ACE_NTOHL (0x7279496e), ACE_NTOHL (0x666f3a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/FT/FactoryInfo:1.0
  12, ACE_NTOHL (0x46616374), ACE_NTOHL (0x6f727949), ACE_NTOHL (0x6e666f00),  // name = FactoryInfo
  3, // member count
  8, ACE_NTOHL (0x66616374), ACE_NTOHL (0x6f727900),  // name = factory
  CORBA::tk_objref, // typecode kind
  64, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f47), ACE_NTOHL (0x656e6572), ACE_NTOHL (0x69634661), ACE_NTOHL (0x63746f72), ACE_NTOHL (0x793a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/GenericFactory:1.0
    15, ACE_NTOHL (0x47656e65), ACE_NTOHL (0x72696346), ACE_NTOHL (0x6163746f), ACE_NTOHL (0x72790000),  // name = GenericFactory

  13, ACE_NTOHL (0x7468655f), ACE_NTOHL (0x6c6f6361), ACE_NTOHL (0x74696f6e), ACE_NTOHL (0x0),  // name = the_location
  CORBA::tk_alias, // typecode kind for typedefs
  424, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    28, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4c), ACE_NTOHL (0x6f636174), ACE_NTOHL (0x696f6e3a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Location:1.0
    9, ACE_NTOHL (0x4c6f6361), ACE_NTOHL (0x74696f6e), ACE_NTOHL (0x0),  // name = Location
    CORBA::tk_alias, // typecode kind for typedefs
    364, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      24, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4e), ACE_NTOHL (0x616d653a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
      5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
      CORBA::tk_alias, // typecode kind for typedefs
      312, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d653a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
        5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
        CORBA::tk_sequence, // typecode kind
        252, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          CORBA::tk_struct, // typecode kind
          236, // encapsulation length
            TAO_ENCAP_BYTE_ORDER, // byte order
            40, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d65436f), ACE_NTOHL (0x6d706f6e), ACE_NTOHL (0x656e743a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
            14, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x436f6d70), ACE_NTOHL (0x6f6e656e), ACE_NTOHL (0x74000000),  // name = NameComponent
            2, // member count
            3, ACE_NTOHL (0x69640000),  // name = id
            CORBA::tk_alias, // typecode kind for typedefs
            64, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4973), ACE_NTOHL (0x7472696e), ACE_NTOHL (0x673a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
              8, ACE_NTOHL (0x49737472), ACE_NTOHL (0x696e6700),  // name = Istring
              CORBA::tk_string,
              0U, // string length

            5, ACE_NTOHL (0x6b696e64), ACE_NTOHL (0x0),  // name = kind
            CORBA::tk_alias, // typecode kind for typedefs
            64, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4973), ACE_NTOHL (0x7472696e), ACE_NTOHL (0x673a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
              8, ACE_NTOHL (0x49737472), ACE_NTOHL (0x696e6700),  // name = Istring
              CORBA::tk_string,
              0U, // string length


          0U,




  13, ACE_NTOHL (0x7468655f), ACE_NTOHL (0x63726974), ACE_NTOHL (0x65726961), ACE_NTOHL (0x0),  // name = the_criteria
  CORBA::tk_alias, // typecode kind for typedefs
  644, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    28, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f43), ACE_NTOHL (0x72697465), ACE_NTOHL (0x7269613a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Criteria:1.0
    9, ACE_NTOHL (0x43726974), ACE_NTOHL (0x65726961), ACE_NTOHL (0x0),  // name = Criteria
    CORBA::tk_alias, // typecode kind for typedefs
    584, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      30, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f50), ACE_NTOHL (0x726f7065), ACE_NTOHL (0x72746965), ACE_NTOHL (0x733a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/Properties:1.0
      11, ACE_NTOHL (0x50726f70), ACE_NTOHL (0x65727469), ACE_NTOHL (0x65730000),  // name = Properties
      CORBA::tk_sequence, // typecode kind
      520, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        CORBA::tk_struct, // typecode kind
        504, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          28, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f50), ACE_NTOHL (0x726f7065), ACE_NTOHL (0x7274793a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Property:1.0
          9, ACE_NTOHL (0x50726f70), ACE_NTOHL (0x65727479), ACE_NTOHL (0x0),  // name = Property
          2, // member count
          4, ACE_NTOHL (0x6e616d00),  // name = nam
          CORBA::tk_alias, // typecode kind for typedefs
          364, // encapsulation length
            TAO_ENCAP_BYTE_ORDER, // byte order
            24, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4e), ACE_NTOHL (0x616d653a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
            5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
            CORBA::tk_alias, // typecode kind for typedefs
            312, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d653a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
              5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
              CORBA::tk_sequence, // typecode kind
              252, // encapsulation length
                TAO_ENCAP_BYTE_ORDER, // byte order
                CORBA::tk_struct, // typecode kind
                236, // encapsulation length
                  TAO_ENCAP_BYTE_ORDER, // byte order
                  40, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d65436f), ACE_NTOHL (0x6d706f6e), ACE_NTOHL (0x656e743a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
                  14, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x436f6d70), ACE_NTOHL (0x6f6e656e), ACE_NTOHL (0x74000000),  // name = NameComponent
                  2, // member count
                  3, ACE_NTOHL (0x69640000),  // name = id
                  CORBA::tk_alias, // typecode kind for typedefs
                  64, // encapsulation length
                    TAO_ENCAP_BYTE_ORDER, // byte order
                    34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4973), ACE_NTOHL (0x7472696e), ACE_NTOHL (0x673a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
                    8, ACE_NTOHL (0x49737472), ACE_NTOHL (0x696e6700),  // name = Istring
                    CORBA::tk_string,
                    0U, // string length

                  5, ACE_NTOHL (0x6b696e64), ACE_NTOHL (0x0),  // name = kind
                  CORBA::tk_alias, // typecode kind for typedefs
                  64, // encapsulation length
                    TAO_ENCAP_BYTE_ORDER, // byte order
                    34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4973), ACE_NTOHL (0x7472696e), ACE_NTOHL (0x673a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
                    8, ACE_NTOHL (0x49737472), ACE_NTOHL (0x696e6700),  // name = Istring
                    CORBA::tk_string,
                    0U, // string length


                0U,



          4, ACE_NTOHL (0x76616c00),  // name = val
          CORBA::tk_alias, // typecode kind for typedefs
          52, // encapsulation length
            TAO_ENCAP_BYTE_ORDER, // byte order
            25, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f56), ACE_NTOHL (0x616c7565), ACE_NTOHL (0x3a312e30), ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/Value:1.0
            6, ACE_NTOHL (0x56616c75), ACE_NTOHL (0x65000000),  // name = Value
            CORBA::tk_any,



        0U,



};




static CORBA::TypeCode _tc_TAO_tc_FT_FactoryInfo (CORBA::tk_struct, sizeof (_oc_FT_FactoryInfo), (char *) &_oc_FT_FactoryInfo, 0, sizeof (FT_FactoryInfo));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_FactoryInfo, &_tc_TAO_tc_FT_FactoryInfo)
TAO_NAMESPACE_END


void FT_FactoryInfo::_tao_any_destructor (void *x)
{
  FT_FactoryInfo *tmp = ACE_static_cast (FT_FactoryInfo*,x);
  delete tmp;
}


#if !defined (TAO_USE_SEQUENCE_TEMPLATES)

void
_TAO_Unbounded_Sequence_FT_FactoryInfos::_allocate_buffer (CORBA::ULong length)
{
  FT_FactoryInfo* tmp = 0;
  tmp = _TAO_Unbounded_Sequence_FT_FactoryInfos::allocbuf (length);

  if (this->buffer_ != 0)
    {
      FT_FactoryInfo *old = ACE_reinterpret_cast (FT_FactoryInfo *,this->buffer_);

      for (CORBA::ULong i = 0; i < this->length_; ++i)
        tmp[i] = old[i];

      if (this->release_)
        _TAO_Unbounded_Sequence_FT_FactoryInfos::freebuf (old);

    }
  this->buffer_ = tmp;
}

void
_TAO_Unbounded_Sequence_FT_FactoryInfos::_deallocate_buffer (void)
{
  if (this->buffer_ == 0 || this->release_ == 0)
    return;

  FT_FactoryInfo *tmp = ACE_reinterpret_cast (FT_FactoryInfo *,this->buffer_);

  _TAO_Unbounded_Sequence_FT_FactoryInfos::freebuf (tmp);
  this->buffer_ = 0;
}

_TAO_Unbounded_Sequence_FT_FactoryInfos::~_TAO_Unbounded_Sequence_FT_FactoryInfos (void) // Dtor.
{
    this->_deallocate_buffer ();
}


#endif /* !TAO_USE_SEQUENCE_TEMPLATES */

#if !defined (_FT_FACTORYINFOS_CS_)
#define _FT_FACTORYINFOS_CS_

// *************************************************************
// FT_FactoryInfos
// *************************************************************

FT_FactoryInfos::FT_FactoryInfos (void)
{}
FT_FactoryInfos::FT_FactoryInfos (CORBA::ULong max) // uses max size
  :
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
  _TAO_Unbounded_Sequence_FT_FactoryInfos
#else /* TAO_USE_SEQUENCE_TEMPLATES */
  TAO_Unbounded_Sequence<FT_FactoryInfo>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
 (max)
{}
FT_FactoryInfos::FT_FactoryInfos (CORBA::ULong max, CORBA::ULong length, FT_FactoryInfo *buffer, CORBA::Boolean release)
  :
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
  _TAO_Unbounded_Sequence_FT_FactoryInfos
#else /* TAO_USE_SEQUENCE_TEMPLATES */
  TAO_Unbounded_Sequence<FT_FactoryInfo>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
 (max, length, buffer, release)
{}


FT_FactoryInfos::FT_FactoryInfos (const FT_FactoryInfos &seq) // copy ctor
  :
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
  _TAO_Unbounded_Sequence_FT_FactoryInfos
#else /* TAO_USE_SEQUENCE_TEMPLATES */
  TAO_Unbounded_Sequence<FT_FactoryInfo>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
 (seq)
{}

FT_FactoryInfos::~FT_FactoryInfos (void) // dtor
{}

void FT_FactoryInfos::_tao_any_destructor (void *x)
{
  FT_FactoryInfos *tmp = ACE_static_cast (FT_FactoryInfos*,x);
  delete tmp;
}


#endif /* end #if !defined */

static const CORBA::Long _oc_FT_FactoryInfos[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  32, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f46), ACE_NTOHL (0x6163746f), ACE_NTOHL (0x7279496e), ACE_NTOHL (0x666f733a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/FactoryInfos:1.0
  13, ACE_NTOHL (0x46616374), ACE_NTOHL (0x6f727949), ACE_NTOHL (0x6e666f73), ACE_NTOHL (0x0),  // name = FactoryInfos
  CORBA::tk_sequence, // typecode kind
  1284, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_struct, // typecode kind
    1268, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f46), ACE_NTOHL (0x6163746f), ACE_NTOHL (0x7279496e), ACE_NTOHL (0x666f3a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/FT/FactoryInfo:1.0
      12, ACE_NTOHL (0x46616374), ACE_NTOHL (0x6f727949), ACE_NTOHL (0x6e666f00),  // name = FactoryInfo
      3, // member count
      8, ACE_NTOHL (0x66616374), ACE_NTOHL (0x6f727900),  // name = factory
      CORBA::tk_objref, // typecode kind
      64, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f47), ACE_NTOHL (0x656e6572), ACE_NTOHL (0x69634661), ACE_NTOHL (0x63746f72), ACE_NTOHL (0x793a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/GenericFactory:1.0
        15, ACE_NTOHL (0x47656e65), ACE_NTOHL (0x72696346), ACE_NTOHL (0x6163746f), ACE_NTOHL (0x72790000),  // name = GenericFactory

      13, ACE_NTOHL (0x7468655f), ACE_NTOHL (0x6c6f6361), ACE_NTOHL (0x74696f6e), ACE_NTOHL (0x0),  // name = the_location
      CORBA::tk_alias, // typecode kind for typedefs
      424, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        28, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4c), ACE_NTOHL (0x6f636174), ACE_NTOHL (0x696f6e3a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Location:1.0
        9, ACE_NTOHL (0x4c6f6361), ACE_NTOHL (0x74696f6e), ACE_NTOHL (0x0),  // name = Location
        CORBA::tk_alias, // typecode kind for typedefs
        364, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          24, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4e), ACE_NTOHL (0x616d653a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
          5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
          CORBA::tk_alias, // typecode kind for typedefs
          312, // encapsulation length
            TAO_ENCAP_BYTE_ORDER, // byte order
            31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d653a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
            5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
            CORBA::tk_sequence, // typecode kind
            252, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              CORBA::tk_struct, // typecode kind
              236, // encapsulation length
                TAO_ENCAP_BYTE_ORDER, // byte order
                40, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d65436f), ACE_NTOHL (0x6d706f6e), ACE_NTOHL (0x656e743a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
                14, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x436f6d70), ACE_NTOHL (0x6f6e656e), ACE_NTOHL (0x74000000),  // name = NameComponent
                2, // member count
                3, ACE_NTOHL (0x69640000),  // name = id
                CORBA::tk_alias, // typecode kind for typedefs
                64, // encapsulation length
                  TAO_ENCAP_BYTE_ORDER, // byte order
                  34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4973), ACE_NTOHL (0x7472696e), ACE_NTOHL (0x673a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
                  8, ACE_NTOHL (0x49737472), ACE_NTOHL (0x696e6700),  // name = Istring
                  CORBA::tk_string,
                  0U, // string length

                5, ACE_NTOHL (0x6b696e64), ACE_NTOHL (0x0),  // name = kind
                CORBA::tk_alias, // typecode kind for typedefs
                64, // encapsulation length
                  TAO_ENCAP_BYTE_ORDER, // byte order
                  34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4973), ACE_NTOHL (0x7472696e), ACE_NTOHL (0x673a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
                  8, ACE_NTOHL (0x49737472), ACE_NTOHL (0x696e6700),  // name = Istring
                  CORBA::tk_string,
                  0U, // string length


              0U,




      13, ACE_NTOHL (0x7468655f), ACE_NTOHL (0x63726974), ACE_NTOHL (0x65726961), ACE_NTOHL (0x0),  // name = the_criteria
      CORBA::tk_alias, // typecode kind for typedefs
      644, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        28, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f43), ACE_NTOHL (0x72697465), ACE_NTOHL (0x7269613a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Criteria:1.0
        9, ACE_NTOHL (0x43726974), ACE_NTOHL (0x65726961), ACE_NTOHL (0x0),  // name = Criteria
        CORBA::tk_alias, // typecode kind for typedefs
        584, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          30, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f50), ACE_NTOHL (0x726f7065), ACE_NTOHL (0x72746965), ACE_NTOHL (0x733a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/Properties:1.0
          11, ACE_NTOHL (0x50726f70), ACE_NTOHL (0x65727469), ACE_NTOHL (0x65730000),  // name = Properties
          CORBA::tk_sequence, // typecode kind
          520, // encapsulation length
            TAO_ENCAP_BYTE_ORDER, // byte order
            CORBA::tk_struct, // typecode kind
            504, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              28, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f50), ACE_NTOHL (0x726f7065), ACE_NTOHL (0x7274793a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Property:1.0
              9, ACE_NTOHL (0x50726f70), ACE_NTOHL (0x65727479), ACE_NTOHL (0x0),  // name = Property
              2, // member count
              4, ACE_NTOHL (0x6e616d00),  // name = nam
              CORBA::tk_alias, // typecode kind for typedefs
              364, // encapsulation length
                TAO_ENCAP_BYTE_ORDER, // byte order
                24, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4e), ACE_NTOHL (0x616d653a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
                5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
                CORBA::tk_alias, // typecode kind for typedefs
                312, // encapsulation length
                  TAO_ENCAP_BYTE_ORDER, // byte order
                  31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d653a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
                  5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
                  CORBA::tk_sequence, // typecode kind
                  252, // encapsulation length
                    TAO_ENCAP_BYTE_ORDER, // byte order
                    CORBA::tk_struct, // typecode kind
                    236, // encapsulation length
                      TAO_ENCAP_BYTE_ORDER, // byte order
                      40, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d65436f), ACE_NTOHL (0x6d706f6e), ACE_NTOHL (0x656e743a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
                      14, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x436f6d70), ACE_NTOHL (0x6f6e656e), ACE_NTOHL (0x74000000),  // name = NameComponent
                      2, // member count
                      3, ACE_NTOHL (0x69640000),  // name = id
                      CORBA::tk_alias, // typecode kind for typedefs
                      64, // encapsulation length
                        TAO_ENCAP_BYTE_ORDER, // byte order
                        34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4973), ACE_NTOHL (0x7472696e), ACE_NTOHL (0x673a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
                        8, ACE_NTOHL (0x49737472), ACE_NTOHL (0x696e6700),  // name = Istring
                        CORBA::tk_string,
                        0U, // string length

                      5, ACE_NTOHL (0x6b696e64), ACE_NTOHL (0x0),  // name = kind
                      CORBA::tk_alias, // typecode kind for typedefs
                      64, // encapsulation length
                        TAO_ENCAP_BYTE_ORDER, // byte order
                        34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4973), ACE_NTOHL (0x7472696e), ACE_NTOHL (0x673a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
                        8, ACE_NTOHL (0x49737472), ACE_NTOHL (0x696e6700),  // name = Istring
                        CORBA::tk_string,
                        0U, // string length


                    0U,



              4, ACE_NTOHL (0x76616c00),  // name = val
              CORBA::tk_alias, // typecode kind for typedefs
              52, // encapsulation length
                TAO_ENCAP_BYTE_ORDER, // byte order
                25, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f56), ACE_NTOHL (0x616c7565), ACE_NTOHL (0x3a312e30), ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/Value:1.0
                6, ACE_NTOHL (0x56616c75), ACE_NTOHL (0x65000000),  // name = Value
                CORBA::tk_any,



            0U,




    0U,

};
static CORBA::TypeCode _tc_TAO_tc_FT_FactoryInfos (CORBA::tk_alias, sizeof (_oc_FT_FactoryInfos), (char *) &_oc_FT_FactoryInfos, 0, sizeof (FT_FactoryInfos));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_FactoryInfos, &_tc_TAO_tc_FT_FactoryInfos)
TAO_NAMESPACE_END
static const CORBA::Long _oc_FT_ReplicationStyleValue[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  41, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f52), ACE_NTOHL (0x65706c69), ACE_NTOHL (0x63617469), ACE_NTOHL (0x6f6e5374), ACE_NTOHL (0x796c6556), ACE_NTOHL (0x616c7565), ACE_NTOHL (0x3a312e30), ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/ReplicationStyleValue:1.0
  22, ACE_NTOHL (0x5265706c), ACE_NTOHL (0x69636174), ACE_NTOHL (0x696f6e53), ACE_NTOHL (0x74796c65), ACE_NTOHL (0x56616c75), ACE_NTOHL (0x65000000),  // name = ReplicationStyleValue
  CORBA::tk_long,

};

static CORBA::TypeCode _tc_TAO_tc_FT_ReplicationStyleValue (
    CORBA::tk_alias,
    sizeof (_oc_FT_ReplicationStyleValue),
    (char *) &_oc_FT_ReplicationStyleValue,
    0,
    sizeof (FT::ReplicationStyleValue)
    );
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_ReplicationStyleValue, &_tc_TAO_tc_FT_ReplicationStyleValue)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, STATELESS, 0)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, COLD_PASSIVE, 1)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, WARM_PASSIVE, 2)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, ACTIVE, 3)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, ACTIVE_WITH_VOTING, 4)
TAO_NAMESPACE_END
static const CORBA::Long _oc_FT_MembershipStyleValue[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4d),
  ACE_NTOHL (0x656d6265),
  ACE_NTOHL (0x72736869),
  ACE_NTOHL (0x70537479),
  ACE_NTOHL (0x6c655661),
  ACE_NTOHL (0x6c75653a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/MembershipStyleValue:1.0
  21,
  ACE_NTOHL (0x4d656d62),
  ACE_NTOHL (0x65727368),
  ACE_NTOHL (0x69705374),
  ACE_NTOHL (0x796c6556),
  ACE_NTOHL (0x616c7565),
  ACE_NTOHL (0x0),  // name = MembershipStyleValue
  CORBA::tk_long,
};


static CORBA::TypeCode _tc_TAO_tc_FT_MembershipStyleValue (
    CORBA::tk_alias,
    sizeof (_oc_FT_MembershipStyleValue),
    (char *) &_oc_FT_MembershipStyleValue,
    0,
    sizeof (FT::MembershipStyleValue)
    );

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_MembershipStyleValue, &_tc_TAO_tc_FT_MembershipStyleValue)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, MEMB_APP_CTRL, 0)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, MEMB_INF_CTRL, 1)
TAO_NAMESPACE_END

static const CORBA::Long _oc_FT_ConsistencyStyleValue[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  41,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f43),
  ACE_NTOHL (0x6f6e7369),
  ACE_NTOHL (0x7374656e),
  ACE_NTOHL (0x63795374),
  ACE_NTOHL (0x796c6556),
  ACE_NTOHL (0x616c7565),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/ConsistencyStyleValue:1.0
  22,
  ACE_NTOHL (0x436f6e73),
  ACE_NTOHL (0x69737465),
  ACE_NTOHL (0x6e637953),
  ACE_NTOHL (0x74796c65),
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x65000000),  // name = ConsistencyStyleValue
  CORBA::tk_long,
};


static CORBA::TypeCode _tc_TAO_tc_FT_ConsistencyStyleValue (
    CORBA::tk_alias,
    sizeof (_oc_FT_ConsistencyStyleValue),
    (char *) &_oc_FT_ConsistencyStyleValue,
    0,
    sizeof (FT::ConsistencyStyleValue)
    );

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_ConsistencyStyleValue, &_tc_TAO_tc_FT_ConsistencyStyleValue)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, CONS_APP_CTRL, 0)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, CONS_INF_CTRL, 1)
TAO_NAMESPACE_END

static const CORBA::Long _oc_FT_FaultMonitoringStyleValue[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  45,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f46),
  ACE_NTOHL (0x61756c74),
  ACE_NTOHL (0x4d6f6e69),
  ACE_NTOHL (0x746f7269),
  ACE_NTOHL (0x6e675374),
  ACE_NTOHL (0x796c6556),
  ACE_NTOHL (0x616c7565),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/FaultMonitoringStyleValue:1.0
  26,
  ACE_NTOHL (0x4661756c),
  ACE_NTOHL (0x744d6f6e),
  ACE_NTOHL (0x69746f72),
  ACE_NTOHL (0x696e6753),
  ACE_NTOHL (0x74796c65),
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x65000000),  // name = FaultMonitoringStyleValue
  CORBA::tk_long,
};

static CORBA::TypeCode _tc_TAO_tc_FT_FaultMonitoringStyleValue (
    CORBA::tk_alias,
    sizeof (_oc_FT_FaultMonitoringStyleValue),
    (char *) &_oc_FT_FaultMonitoringStyleValue,
    0, sizeof (FT::FaultMonitoringStyleValue));

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_FaultMonitoringStyleValue, &_tc_TAO_tc_FT_FaultMonitoringStyleValue)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, PULL, 0)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, PUSH, 1)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, NOT_MONITORED, 2)
TAO_NAMESPACE_END
static const CORBA::Long _oc_FT_FaultMonitoringGranularityValue[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  51,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f46),
  ACE_NTOHL (0x61756c74),
  ACE_NTOHL (0x4d6f6e69),
  ACE_NTOHL (0x746f7269),
  ACE_NTOHL (0x6e674772),
  ACE_NTOHL (0x616e756c),
  ACE_NTOHL (0x61726974),
  ACE_NTOHL (0x7956616c),
  ACE_NTOHL (0x75653a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/FT/FaultMonitoringGranularityValue:1.0
  32,
  ACE_NTOHL (0x4661756c),
  ACE_NTOHL (0x744d6f6e),
  ACE_NTOHL (0x69746f72),
  ACE_NTOHL (0x696e6747),
  ACE_NTOHL (0x72616e75),
  ACE_NTOHL (0x6c617269),
  ACE_NTOHL (0x74795661),
  ACE_NTOHL (0x6c756500),  // name = FaultMonitoringGranularityValue
  CORBA::tk_long,
};


static CORBA::TypeCode _tc_TAO_tc_FT_FaultMonitoringGranularityValue (
    CORBA::tk_alias,
    sizeof (_oc_FT_FaultMonitoringGranularityValue),
    (char *) &_oc_FT_FaultMonitoringGranularityValue,
    0,
    sizeof (FT::FaultMonitoringGranularityValue)
    );

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_FaultMonitoringGranularityValue, &_tc_TAO_tc_FT_FaultMonitoringGranularityValue)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, MEMB, 0)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, LOC, 1)
TAO_NAMESPACE_END
TAO_NAMESPACE_TYPE (const CORBA::Long)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (const CORBA::Long, LOC_AND_TYPE, 2)
TAO_NAMESPACE_END


static const CORBA::Long _oc_FT_FactoriesValue[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f46),
  ACE_NTOHL (0x6163746f),
  ACE_NTOHL (0x72696573),
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x653a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/FactoriesValue:1.0
  15,
  ACE_NTOHL (0x46616374),
  ACE_NTOHL (0x6f726965),
  ACE_NTOHL (0x7356616c),
  ACE_NTOHL (0x75650000),  // name = FactoriesValue
  CORBA::tk_alias, // typecode kind for typedefs
  1352, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  32,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f46),
  ACE_NTOHL (0x6163746f),
  ACE_NTOHL (0x7279496e),
  ACE_NTOHL (0x666f733a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/FactoryInfos:1.0
  13,
  ACE_NTOHL (0x46616374),
  ACE_NTOHL (0x6f727949),
  ACE_NTOHL (0x6e666f73),
  ACE_NTOHL (0x0),  // name = FactoryInfos
  CORBA::tk_sequence, // typecode kind
  1284, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  1268, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f46),
  ACE_NTOHL (0x6163746f),
  ACE_NTOHL (0x7279496e),
  ACE_NTOHL (0x666f3a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/FT/FactoryInfo:1.0
  12,
  ACE_NTOHL (0x46616374),
  ACE_NTOHL (0x6f727949),
  ACE_NTOHL (0x6e666f00),// name = FactoryInfo
  3, // member count
  8, ACE_NTOHL (0x66616374),
  ACE_NTOHL (0x6f727900),  // name = factory
  CORBA::tk_objref, // typecode kind
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f47),
  ACE_NTOHL (0x656e6572),
  ACE_NTOHL (0x69634661),
  ACE_NTOHL (0x63746f72),
  ACE_NTOHL (0x793a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/GenericFactory:1.0
  15,
  ACE_NTOHL (0x47656e65),
  ACE_NTOHL (0x72696346),
  ACE_NTOHL (0x6163746f),
  ACE_NTOHL (0x72790000),  // name = GenericFactory
  13,
  ACE_NTOHL (0x7468655f),
  ACE_NTOHL (0x6c6f6361),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x0),  // name = the_location
  CORBA::tk_alias, // typecode kind for typedefs
  424, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  28,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4c),
  ACE_NTOHL (0x6f636174),
  ACE_NTOHL (0x696f6e3a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Location:1.0
  9,
  ACE_NTOHL (0x4c6f6361),
  ACE_NTOHL (0x74696f6e),
  ACE_NTOHL (0x0),  // name = Location
  CORBA::tk_alias, // typecode kind for typedefs
  364, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  24,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4e),
  ACE_NTOHL (0x616d653a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_alias, // typecode kind for typedefs
  312, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d653a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_sequence, // typecode kind
  252, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  236, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d65436f),
  ACE_NTOHL (0x6d706f6e),
  ACE_NTOHL (0x656e743a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
  14,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x436f6d70),
  ACE_NTOHL (0x6f6e656e),
  ACE_NTOHL (0x74000000),  // name = NameComponent
  2, // member count
  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  5,
  ACE_NTOHL (0x6b696e64),
  ACE_NTOHL (0x0),  // name = kind
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  0U,
  13,
  ACE_NTOHL (0x7468655f),
  ACE_NTOHL (0x63726974),
  ACE_NTOHL (0x65726961),
  ACE_NTOHL (0x0),  // name = the_criteria
  CORBA::tk_alias, // typecode kind for typedefs
  644, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  28,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f43),
  ACE_NTOHL (0x72697465),
  ACE_NTOHL (0x7269613a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Criteria:1.0
  9,
  ACE_NTOHL (0x43726974),
  ACE_NTOHL (0x65726961),
  ACE_NTOHL (0x0),  // name = Criteria
  CORBA::tk_alias, // typecode kind for typedefs
  584, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  30,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f50),
  ACE_NTOHL (0x726f7065),
  ACE_NTOHL (0x72746965),
  ACE_NTOHL (0x733a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/Properties:1.0
  11,
  ACE_NTOHL (0x50726f70),
  ACE_NTOHL (0x65727469),
  ACE_NTOHL (0x65730000),  // name = Properties
  CORBA::tk_sequence, // typecode kind
  520, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  504, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  28,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f50),
  ACE_NTOHL (0x726f7065),
  ACE_NTOHL (0x7274793a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Property:1.0
  9,
  ACE_NTOHL (0x50726f70),
  ACE_NTOHL (0x65727479),
  ACE_NTOHL (0x0),  // name = Property
  2, // member count
  4,
  ACE_NTOHL (0x6e616d00),  // name = nam
  CORBA::tk_alias, // typecode kind for typedefs
  364, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  24,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4e),
  ACE_NTOHL (0x616d653a),
  ACE_NTOHL (0x312e3000), // repository ID = IDL:omg.org/FT/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_alias, // typecode kind for typedefs
  312, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d653a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_sequence, // typecode kind
  252, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  236, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  236, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d65436f),
  ACE_NTOHL (0x6d706f6e),
  ACE_NTOHL (0x656e743a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
  14,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x436f6d70),
  ACE_NTOHL (0x6f6e656e),
  ACE_NTOHL (0x74000000),  // name = NameComponent
  2, // member count
  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  5,
  ACE_NTOHL (0x6b696e64),
  ACE_NTOHL (0x0),  // name = kind
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  0U,
  4,
  ACE_NTOHL (0x76616c00),  // name = val
  CORBA::tk_alias, // typecode kind for typedefs
  52, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  25,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f56),
  ACE_NTOHL (0x616c7565),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/Value:1.0
  6,
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x65000000),  // name = Value
  CORBA::tk_any,
  0U,
  0U,
};


static CORBA::TypeCode _tc_TAO_tc_FT_FactoriesValue (
    CORBA::tk_alias,
    sizeof (_oc_FT_FactoriesValue),
    (char *) &_oc_FT_FactoriesValue,
    0,
    sizeof (FT::FactoriesValue));

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr,
                      _tc_FactoriesValue,
                      &_tc_TAO_tc_FT_FactoriesValue)
TAO_NAMESPACE_END

static const CORBA::Long _oc_FT_InitialNumberReplicasValue[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  46,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f49),
  ACE_NTOHL (0x6e697469),
  ACE_NTOHL (0x616c4e75),
  ACE_NTOHL (0x6d626572),
  ACE_NTOHL (0x5265706c),
  ACE_NTOHL (0x69636173),
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x653a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/InitialNumberReplicasValue:1.0
  27,
  ACE_NTOHL (0x496e6974),
  ACE_NTOHL (0x69616c4e),
  ACE_NTOHL (0x756d6265),
  ACE_NTOHL (0x72526570),
  ACE_NTOHL (0x6c696361),
  ACE_NTOHL (0x7356616c),
  ACE_NTOHL (0x75650000),  // name = InitialNumberReplicasValue
  CORBA::tk_ushort,
};


static CORBA::TypeCode _tc_TAO_tc_FT_InitialNumberReplicasValue (
    CORBA::tk_alias,
    sizeof (_oc_FT_InitialNumberReplicasValue),
    (char *) &_oc_FT_InitialNumberReplicasValue,
    0,
    sizeof (FT::InitialNumberReplicasValue));

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_InitialNumberReplicasValue, &_tc_TAO_tc_FT_InitialNumberReplicasValue)
  TAO_NAMESPACE_END

static const CORBA::Long _oc_FT_MinimumNumberReplicasValue[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  46,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4d),
  ACE_NTOHL (0x696e696d),
  ACE_NTOHL (0x756d4e75),
  ACE_NTOHL (0x6d626572),
  ACE_NTOHL (0x5265706c),
  ACE_NTOHL (0x69636173),
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x653a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/MinimumNumberReplicasValue:1.0
  27,
  ACE_NTOHL (0x4d696e69),
  ACE_NTOHL (0x6d756d4e),
  ACE_NTOHL (0x756d6265),
  ACE_NTOHL (0x72526570),
  ACE_NTOHL (0x6c696361),
  ACE_NTOHL (0x7356616c),
  ACE_NTOHL (0x75650000),  // name = MinimumNumberReplicasValue
  CORBA::tk_ushort,
};


static CORBA::TypeCode _tc_TAO_tc_FT_MinimumNumberReplicasValue (
    CORBA::tk_alias,
    sizeof (_oc_FT_MinimumNumberReplicasValue),
    (char *) &_oc_FT_MinimumNumberReplicasValue,
    0,
    sizeof (FT::MinimumNumberReplicasValue)
    );

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_MinimumNumberReplicasValue, &_tc_TAO_tc_FT_MinimumNumberReplicasValue)
TAO_NAMESPACE_END

static const CORBA::Long _oc_FT_FaultMonitoringIntervalAndTimeoutValue[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  58,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f46),
  ACE_NTOHL (0x61756c74),
  ACE_NTOHL (0x4d6f6e69),
  ACE_NTOHL (0x746f7269),
  ACE_NTOHL (0x6e67496e),
  ACE_NTOHL (0x74657276),
  ACE_NTOHL (0x616c416e),
  ACE_NTOHL (0x6454696d),
  ACE_NTOHL (0x656f7574),
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x653a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/FaultMonitoringIntervalAndTimeoutValue:1.0
  39,
  ACE_NTOHL (0x4661756c),
  ACE_NTOHL (0x744d6f6e),
  ACE_NTOHL (0x69746f72),
  ACE_NTOHL (0x696e6749),
  ACE_NTOHL (0x6e746572),
  ACE_NTOHL (0x76616c41),
  ACE_NTOHL (0x6e645469),
  ACE_NTOHL (0x6d656f75),
  ACE_NTOHL (0x7456616c),
  ACE_NTOHL (0x75650000),  // name = FaultMonitoringIntervalAndTimeoutValue
  2, // member count
  20,
  ACE_NTOHL (0x6d6f6e69),
  ACE_NTOHL (0x746f7269),
  ACE_NTOHL (0x6e675f69),
  ACE_NTOHL (0x6e746572),
  ACE_NTOHL (0x76616c00),  // name = monitoring_interval
  CORBA::tk_alias, // typecode kind for typedefs
  56, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x54696d65),
  ACE_NTOHL (0x42617365),
  ACE_NTOHL (0x2f54696d),
  ACE_NTOHL (0x65543a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/TimeBase/TimeT:1.0
  6,
  ACE_NTOHL (0x54696d65),
  ACE_NTOHL (0x54000000),  // name = TimeT
  CORBA::tk_ulonglong,
  8,
  ACE_NTOHL (0x74696d65),
  ACE_NTOHL (0x6f757400),  // name = timeout
  CORBA::tk_alias, // typecode kind for typedefs
  56, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x54696d65),
  ACE_NTOHL (0x42617365),
  ACE_NTOHL (0x2f54696d),
  ACE_NTOHL (0x65543a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/TimeBase/TimeT:1.0
  6,
  ACE_NTOHL (0x54696d65),
  ACE_NTOHL (0x54000000),  // name = TimeT
  CORBA::tk_ulonglong,
};

static CORBA::TypeCode _tc_TAO_tc_FT_FaultMonitoringIntervalAndTimeoutValue (
    CORBA::tk_struct,
    sizeof (_oc_FT_FaultMonitoringIntervalAndTimeoutValue),
    (char *) &_oc_FT_FaultMonitoringIntervalAndTimeoutValue,
    0,
    sizeof (FT_FaultMonitoringIntervalAndTimeoutValue));


TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr,
                      _tc_FaultMonitoringIntervalAndTimeoutValue,
                      &_tc_TAO_tc_FT_FaultMonitoringIntervalAndTimeoutValue)
TAO_NAMESPACE_END

void FT_FaultMonitoringIntervalAndTimeoutValue::_tao_any_destructor (void *x)
{
  FT_FaultMonitoringIntervalAndTimeoutValue *tmp =
    ACE_static_cast (FT_FaultMonitoringIntervalAndTimeoutValue*,x);
  delete tmp;
}

static const CORBA::Long _oc_FT_CheckpointIntervalValue[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  43,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f43),
  ACE_NTOHL (0x6865636b),
  ACE_NTOHL (0x706f696e),
  ACE_NTOHL (0x74496e74),
  ACE_NTOHL (0x65727661),
  ACE_NTOHL (0x6c56616c),
  ACE_NTOHL (0x75653a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/FT/CheckpointIntervalValue:1.0
  24,
  ACE_NTOHL (0x43686563),
  ACE_NTOHL (0x6b706f69),
  ACE_NTOHL (0x6e74496e),
  ACE_NTOHL (0x74657276),
  ACE_NTOHL (0x616c5661),
  ACE_NTOHL (0x6c756500),  // name = CheckpointIntervalValue
  CORBA::tk_alias, // typecode kind for typedefs
  56, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x54696d65),
  ACE_NTOHL (0x42617365),
  ACE_NTOHL (0x2f54696d),
  ACE_NTOHL (0x65543a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/TimeBase/TimeT:1.0
  6,
  ACE_NTOHL (0x54696d65),
  ACE_NTOHL (0x54000000),  // name = TimeT
  CORBA::tk_ulonglong,
};

static CORBA::TypeCode _tc_TAO_tc_FT_CheckpointIntervalValue (
    CORBA::tk_alias,
    sizeof (_oc_FT_CheckpointIntervalValue),
    (char *) &_oc_FT_CheckpointIntervalValue,
    0,
    sizeof (FT::CheckpointIntervalValue)
    );

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr,
                      _tc_CheckpointIntervalValue,
                      &_tc_TAO_tc_FT_CheckpointIntervalValue)
TAO_NAMESPACE_END


// default constructor
FT_InterfaceNotFound::FT_InterfaceNotFound (void)
  : CORBA_UserException (::FT::_tc_InterfaceNotFound)
{
}

// destructor - all members are of self managing types
FT_InterfaceNotFound::~FT_InterfaceNotFound (void)
{
}

void FT_InterfaceNotFound::_tao_any_destructor (void *x)
{
  FT_InterfaceNotFound *tmp = ACE_static_cast (FT_InterfaceNotFound*,x);
  delete tmp;
}

// copy constructor
FT_InterfaceNotFound::FT_InterfaceNotFound (const ::FT_InterfaceNotFound &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
  }

// assignment operator
FT_InterfaceNotFound&
FT_InterfaceNotFound::operator= (const ::FT_InterfaceNotFound &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
FT_InterfaceNotFound *
FT_InterfaceNotFound::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/InterfaceNotFound:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_InterfaceNotFound *, exc);
  else
    return 0;
}


void FT_InterfaceNotFound::_raise ()
{
  TAO_RAISE(*this);
}


void FT_InterfaceNotFound::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_InterfaceNotFound::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_InterfaceNotFound::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_InterfaceNotFound, 0);
  return retval;
}

static const CORBA::Long _oc_FT_InterfaceNotFound[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  37,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f49),
  ACE_NTOHL (0x6e746572),
  ACE_NTOHL (0x66616365),
  ACE_NTOHL (0x4e6f7446),
  ACE_NTOHL (0x6f756e64),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/InterfaceNotFound:1.0
  18,
  ACE_NTOHL (0x496e7465),
  ACE_NTOHL (0x72666163),
  ACE_NTOHL (0x654e6f74),
  ACE_NTOHL (0x466f756e),
  ACE_NTOHL (0x64000000),  // name = InterfaceNotFound
  0, // member count
};
static CORBA::TypeCode _tc_TAO_tc_FT_InterfaceNotFound (
    CORBA::tk_except,
    sizeof (_oc_FT_InterfaceNotFound),
    (char *) &_oc_FT_InterfaceNotFound,
    0,
    sizeof (FT::InterfaceNotFound));

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_InterfaceNotFound, &_tc_TAO_tc_FT_InterfaceNotFound)
TAO_NAMESPACE_END
// default constructor
FT_ObjectGroupNotFound::FT_ObjectGroupNotFound (void)
  : CORBA_UserException (::FT::_tc_ObjectGroupNotFound)
{
}

// destructor - all members are of self managing types
FT_ObjectGroupNotFound::~FT_ObjectGroupNotFound (void)
{
}

void FT_ObjectGroupNotFound::_tao_any_destructor (void *x)
{
  FT_ObjectGroupNotFound *tmp = ACE_static_cast (FT_ObjectGroupNotFound*,x);
  delete tmp;
}

// copy constructor
FT_ObjectGroupNotFound::FT_ObjectGroupNotFound (const ::FT_ObjectGroupNotFound &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
  }

// assignment operator
FT_ObjectGroupNotFound&
FT_ObjectGroupNotFound::operator= (const ::FT_ObjectGroupNotFound &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
FT_ObjectGroupNotFound *
FT_ObjectGroupNotFound::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/ObjectGroupNotFound:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_ObjectGroupNotFound *, exc);
  else
    return 0;
}


void FT_ObjectGroupNotFound::_raise ()
{
  TAO_RAISE(*this);
}


void FT_ObjectGroupNotFound::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_ObjectGroupNotFound::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_ObjectGroupNotFound::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_ObjectGroupNotFound, 0);
  return retval;
}

static const CORBA::Long _oc_FT_ObjectGroupNotFound[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  39,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4f),
  ACE_NTOHL (0x626a6563),
  ACE_NTOHL (0x7447726f),
  ACE_NTOHL (0x75704e6f),
  ACE_NTOHL (0x74466f75),
  ACE_NTOHL (0x6e643a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/FT/ObjectGroupNotFound:1.0
  20,
  ACE_NTOHL (0x4f626a65),
  ACE_NTOHL (0x63744772),
  ACE_NTOHL (0x6f75704e),
  ACE_NTOHL (0x6f74466f),
  ACE_NTOHL (0x756e6400),  // name = ObjectGroupNotFound
  0, // member count
};


static CORBA::TypeCode _tc_TAO_tc_FT_ObjectGroupNotFound (
    CORBA::tk_except,
    sizeof (_oc_FT_ObjectGroupNotFound),
    (char *) &_oc_FT_ObjectGroupNotFound,
    0,
    sizeof (FT::ObjectGroupNotFound));

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_ObjectGroupNotFound, &_tc_TAO_tc_FT_ObjectGroupNotFound)
TAO_NAMESPACE_END
// default constructor
FT_MemberNotFound::FT_MemberNotFound (void)
  : CORBA_UserException (::FT::_tc_MemberNotFound)
{
}

// destructor - all members are of self managing types
FT_MemberNotFound::~FT_MemberNotFound (void)
{
}

void FT_MemberNotFound::_tao_any_destructor (void *x)
{
  FT_MemberNotFound *tmp =
    ACE_static_cast (FT_MemberNotFound*,x);
  delete tmp;
}

// copy constructor
FT_MemberNotFound::FT_MemberNotFound (const ::FT_MemberNotFound &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
  }

// assignment operator
FT_MemberNotFound&
FT_MemberNotFound::operator= (const ::FT_MemberNotFound &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
FT_MemberNotFound *
FT_MemberNotFound::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/MemberNotFound:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_MemberNotFound *, exc);
  else
    return 0;
}


void FT_MemberNotFound::_raise ()
{
  TAO_RAISE(*this);
}


void FT_MemberNotFound::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_MemberNotFound::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_MemberNotFound::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_MemberNotFound, 0);
  return retval;
}

static const CORBA::Long _oc_FT_MemberNotFound[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4d), ACE_NTOHL (0x656d6265), ACE_NTOHL (0x724e6f74), ACE_NTOHL (0x466f756e), ACE_NTOHL (0x643a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/MemberNotFound:1.0
  15, ACE_NTOHL (0x4d656d62), ACE_NTOHL (0x65724e6f), ACE_NTOHL (0x74466f75), ACE_NTOHL (0x6e640000),  // name = MemberNotFound
  0, // member count
};
static CORBA::TypeCode _tc_TAO_tc_FT_MemberNotFound (
    CORBA::tk_except,
    sizeof (_oc_FT_MemberNotFound),
    (char *) &_oc_FT_MemberNotFound,
    0,
    sizeof (FT::MemberNotFound)
    );

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_MemberNotFound, &_tc_TAO_tc_FT_MemberNotFound)
TAO_NAMESPACE_END
// default constructor

FT_ObjectNotFound::FT_ObjectNotFound (void)
  : CORBA_UserException (::FT::_tc_ObjectNotFound)
{
}

// destructor - all members are of self managing types
FT_ObjectNotFound::~FT_ObjectNotFound (void)
{
}

void FT_ObjectNotFound::_tao_any_destructor (void *x)
{
  FT_ObjectNotFound *tmp = ACE_static_cast (FT_ObjectNotFound*,x);
  delete tmp;
}

// copy constructor
FT_ObjectNotFound::FT_ObjectNotFound (const ::FT_ObjectNotFound &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
  }

// assignment operator
FT_ObjectNotFound&
FT_ObjectNotFound::operator= (const ::FT_ObjectNotFound &_tao_excp)
{
  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
FT_ObjectNotFound *
FT_ObjectNotFound::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/ObjectNotFound:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_ObjectNotFound *, exc);
  else
    return 0;
}


void FT_ObjectNotFound::_raise ()
{
  TAO_RAISE(*this);
}


void FT_ObjectNotFound::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_ObjectNotFound::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_ObjectNotFound::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_ObjectNotFound, 0);
  return retval;
}

static const CORBA::Long _oc_FT_ObjectNotFound[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4f),
  ACE_NTOHL (0x626a6563),
  ACE_NTOHL (0x744e6f74),
  ACE_NTOHL (0x466f756e),
  ACE_NTOHL (0x643a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/ObjectNotFound:1.0
  15,
  ACE_NTOHL (0x4f626a65),
  ACE_NTOHL (0x63744e6f),
  ACE_NTOHL (0x74466f75),
  ACE_NTOHL (0x6e640000),  // name = ObjectNotFound
  0, // member count
};


static CORBA::TypeCode _tc_TAO_tc_FT_ObjectNotFound (
    CORBA::tk_except,
    sizeof (_oc_FT_ObjectNotFound),
    (char *) &_oc_FT_ObjectNotFound,
    0,
    sizeof (FT::ObjectNotFound));

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_ObjectNotFound, &_tc_TAO_tc_FT_ObjectNotFound)
TAO_NAMESPACE_END

// default constructor
FT_MemberAlreadyPresent::FT_MemberAlreadyPresent (void)
  : CORBA_UserException (::FT::_tc_MemberAlreadyPresent)
{
}

// destructor - all members are of self managing types
FT_MemberAlreadyPresent::~FT_MemberAlreadyPresent (void)
{
}

void FT_MemberAlreadyPresent::_tao_any_destructor (void *x)
{
  FT_MemberAlreadyPresent *tmp =
    ACE_static_cast (FT_MemberAlreadyPresent*,x);
  delete tmp;
}

// copy constructor
FT_MemberAlreadyPresent::FT_MemberAlreadyPresent (const ::FT_MemberAlreadyPresent &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
}

// assignment operator
FT_MemberAlreadyPresent&
FT_MemberAlreadyPresent::operator= (const ::FT_MemberAlreadyPresent &_tao_excp)
{
  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
FT_MemberAlreadyPresent *
FT_MemberAlreadyPresent::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/MemberAlreadyPresent:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_MemberAlreadyPresent *, exc);
  else
    return 0;
}


void FT_MemberAlreadyPresent::_raise ()
{
  TAO_RAISE(*this);
}


void FT_MemberAlreadyPresent::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_MemberAlreadyPresent::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_MemberAlreadyPresent::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_MemberAlreadyPresent, 0);
  return retval;
}

static const CORBA::Long _oc_FT_MemberAlreadyPresent[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4d),
  ACE_NTOHL (0x656d6265),
  ACE_NTOHL (0x72416c72),
  ACE_NTOHL (0x65616479),
  ACE_NTOHL (0x50726573),
  ACE_NTOHL (0x656e743a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/MemberAlreadyPresent:1.0
  21,
  ACE_NTOHL (0x4d656d62),
  ACE_NTOHL (0x6572416c),
  ACE_NTOHL (0x72656164),
  ACE_NTOHL (0x79507265),
  ACE_NTOHL (0x73656e74),
  ACE_NTOHL (0x0),  // name = MemberAlreadyPresent
  0, // member count
};


static CORBA::TypeCode _tc_TAO_tc_FT_MemberAlreadyPresent (
    CORBA::tk_except,
    sizeof (_oc_FT_MemberAlreadyPresent),
    (char *) &_oc_FT_MemberAlreadyPresent,
    0,
    sizeof (FT::MemberAlreadyPresent));

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_MemberAlreadyPresent, &_tc_TAO_tc_FT_MemberAlreadyPresent)
TAO_NAMESPACE_END

// default constructor
FT_BadReplicationStyle::FT_BadReplicationStyle (void)
  : CORBA_UserException (::FT::_tc_BadReplicationStyle)
{
}

// destructor - all members are of self managing types
FT_BadReplicationStyle::~FT_BadReplicationStyle (void)
{
}

void FT_BadReplicationStyle::_tao_any_destructor (void *x)
{
  FT_BadReplicationStyle *tmp = ACE_static_cast (FT_BadReplicationStyle*,x);
  delete tmp;
}

// copy constructor
FT_BadReplicationStyle::FT_BadReplicationStyle (const ::FT_BadReplicationStyle &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
  }

// assignment operator
FT_BadReplicationStyle&
FT_BadReplicationStyle::operator= (const ::FT_BadReplicationStyle &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
FT_BadReplicationStyle *
FT_BadReplicationStyle::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/BadReplicationStyle:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_BadReplicationStyle *, exc);
  else
    return 0;
}


void FT_BadReplicationStyle::_raise ()
{
  TAO_RAISE(*this);
}


void FT_BadReplicationStyle::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_BadReplicationStyle::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_BadReplicationStyle::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_BadReplicationStyle, 0);
  return retval;
}

static const CORBA::Long _oc_FT_BadReplicationStyle[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  39, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f42), ACE_NTOHL (0x61645265), ACE_NTOHL (0x706c6963), ACE_NTOHL (0x6174696f), ACE_NTOHL (0x6e537479), ACE_NTOHL (0x6c653a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/FT/BadReplicationStyle:1.0
  20, ACE_NTOHL (0x42616452), ACE_NTOHL (0x65706c69), ACE_NTOHL (0x63617469), ACE_NTOHL (0x6f6e5374), ACE_NTOHL (0x796c6500),  // name = BadReplicationStyle
  0, // member count
};
static CORBA::TypeCode _tc_TAO_tc_FT_BadReplicationStyle (CORBA::tk_except, sizeof (_oc_FT_BadReplicationStyle), (char *) &_oc_FT_BadReplicationStyle, 0, sizeof (FT_BadReplicationStyle));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_BadReplicationStyle, &_tc_TAO_tc_FT_BadReplicationStyle)
TAO_NAMESPACE_END
// default constructor
FT_ObjectNotCreated::FT_ObjectNotCreated (void)
  : CORBA_UserException (::FT::_tc_ObjectNotCreated)
{
}

// destructor - all members are of self managing types
FT_ObjectNotCreated::~FT_ObjectNotCreated (void)
{
}

void FT_ObjectNotCreated::_tao_any_destructor (void *x)
{
  FT_ObjectNotCreated *tmp = ACE_static_cast (FT_ObjectNotCreated*,x);
  delete tmp;
}

// copy constructor
FT_ObjectNotCreated::FT_ObjectNotCreated (const ::FT_ObjectNotCreated &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
}

// assignment operator
FT_ObjectNotCreated&
FT_ObjectNotCreated::operator= (const ::FT_ObjectNotCreated &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
FT_ObjectNotCreated *
FT_ObjectNotCreated::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/ObjectNotCreated:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_ObjectNotCreated *, exc);
  else
    return 0;
}


void FT_ObjectNotCreated::_raise ()
{
  TAO_RAISE(*this);
}


void FT_ObjectNotCreated::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_ObjectNotCreated::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_ObjectNotCreated::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_ObjectNotCreated, 0);
  return retval;
}

static const CORBA::Long _oc_FT_ObjectNotCreated[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  36,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4f),
  ACE_NTOHL (0x626a6563),
  ACE_NTOHL (0x744e6f74),
  ACE_NTOHL (0x43726561),
  ACE_NTOHL (0x7465643a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/ObjectNotCreated:1.0
  17,
  ACE_NTOHL (0x4f626a65),
  ACE_NTOHL (0x63744e6f),
  ACE_NTOHL (0x74437265),
  ACE_NTOHL (0x61746564),
  ACE_NTOHL (0x0),  // name = ObjectNotCreated
  0, // member count
};


static CORBA::TypeCode _tc_TAO_tc_FT_ObjectNotCreated (
    CORBA::tk_except,
    sizeof (_oc_FT_ObjectNotCreated),
    (char *) &_oc_FT_ObjectNotCreated,
    0,
    sizeof (FT::ObjectNotCreated));

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_ObjectNotCreated, &_tc_TAO_tc_FT_ObjectNotCreated)
TAO_NAMESPACE_END

// default constructor
FT_ObjectNotAdded::FT_ObjectNotAdded (void)
  : CORBA_UserException (::FT::_tc_ObjectNotAdded)
{
}

// destructor - all members are of self managing types
FT_ObjectNotAdded::~FT_ObjectNotAdded (void)
{
}

void FT_ObjectNotAdded::_tao_any_destructor (void *x)
{
  FT_ObjectNotAdded *tmp = ACE_static_cast (FT_ObjectNotAdded*,x);
  delete tmp;
}

// copy constructor
FT_ObjectNotAdded::FT_ObjectNotAdded (const ::FT_ObjectNotAdded &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
  }

// assignment operator
FT_ObjectNotAdded&
FT_ObjectNotAdded::operator= (const ::FT_ObjectNotAdded &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
FT_ObjectNotAdded *
FT_ObjectNotAdded::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/ObjectNotAdded:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_ObjectNotAdded *, exc);
  else
    return 0;
}


void FT_ObjectNotAdded::_raise ()
{
  TAO_RAISE(*this);
}


void FT_ObjectNotAdded::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_ObjectNotAdded::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_ObjectNotAdded::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_ObjectNotAdded, 0);
  return retval;
}

static const CORBA::Long _oc_FT_ObjectNotAdded[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4f),
  ACE_NTOHL (0x626a6563),
  ACE_NTOHL (0x744e6f74),
  ACE_NTOHL (0x41646465),
  ACE_NTOHL (0x643a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/ObjectNotAdded:1.0
  15,
  ACE_NTOHL (0x4f626a65),
  ACE_NTOHL (0x63744e6f),
  ACE_NTOHL (0x74416464),
  ACE_NTOHL (0x65640000),  // name = ObjectNotAdded
  0, // member count
};

static CORBA::TypeCode _tc_TAO_tc_FT_ObjectNotAdded (
    CORBA::tk_except,
    sizeof (_oc_FT_ObjectNotAdded),
    (char *) &_oc_FT_ObjectNotAdded,
    0,
    sizeof (FT::ObjectNotAdded));

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_ObjectNotAdded, &_tc_TAO_tc_FT_ObjectNotAdded)
TAO_NAMESPACE_END

// default constructor
FT_PrimaryNotSet::FT_PrimaryNotSet (void)
  : CORBA_UserException (::FT::_tc_PrimaryNotSet)
{
}

// destructor - all members are of self managing types
FT_PrimaryNotSet::~FT_PrimaryNotSet (void)
{
}

void FT_PrimaryNotSet::_tao_any_destructor (void *x)
{
  FT_PrimaryNotSet *tmp = ACE_static_cast (FT_PrimaryNotSet*,x);
  delete tmp;
}

// copy constructor
FT_PrimaryNotSet::FT_PrimaryNotSet (const ::FT_PrimaryNotSet &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
  }

// assignment operator
FT_PrimaryNotSet&
FT_PrimaryNotSet::operator= (const ::FT_PrimaryNotSet &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
FT_PrimaryNotSet *
FT_PrimaryNotSet::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/PrimaryNotSet:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_PrimaryNotSet *, exc);
  else
    return 0;
}


void FT_PrimaryNotSet::_raise ()
{
  TAO_RAISE(*this);
}


void FT_PrimaryNotSet::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_PrimaryNotSet::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_PrimaryNotSet::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_PrimaryNotSet, 0);
  return retval;
}

static const CORBA::Long _oc_FT_PrimaryNotSet[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  33,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f50),
  ACE_NTOHL (0x72696d61),
  ACE_NTOHL (0x72794e6f),
  ACE_NTOHL (0x74536574),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/PrimaryNotSet:1.0
  14,
  ACE_NTOHL (0x5072696d),
  ACE_NTOHL (0x6172794e),
  ACE_NTOHL (0x6f745365),
  ACE_NTOHL (0x74000000),  // name = PrimaryNotSet
  0, // member count
};


static CORBA::TypeCode _tc_TAO_tc_FT_PrimaryNotSet (
    CORBA::tk_except,
    sizeof (_oc_FT_PrimaryNotSet),
    (char *) &_oc_FT_PrimaryNotSet,
    0,
    sizeof (FT_PrimaryNotSet));

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_PrimaryNotSet, &_tc_TAO_tc_FT_PrimaryNotSet)
TAO_NAMESPACE_END

// default constructor
FT_UnsupportedProperty::FT_UnsupportedProperty (void)
  : CORBA_UserException (::FT::_tc_UnsupportedProperty)
{
}

// destructor - all members are of self managing types
FT_UnsupportedProperty::~FT_UnsupportedProperty (void)
{
}

void FT_UnsupportedProperty::_tao_any_destructor (void *x)
{
  FT_UnsupportedProperty *tmp = ACE_static_cast (FT_UnsupportedProperty*,x);
  delete tmp;
}

// copy constructor
FT_UnsupportedProperty::FT_UnsupportedProperty (const ::FT_UnsupportedProperty &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
  this->nam = _tao_excp.nam;
  this->val = _tao_excp.val;
}

// assignment operator
FT_UnsupportedProperty&
FT_UnsupportedProperty::operator= (const ::FT_UnsupportedProperty &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  this->nam = _tao_excp.nam;
  this->val = _tao_excp.val;
  return *this;
}

// narrow
FT_UnsupportedProperty *
FT_UnsupportedProperty::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/UnsupportedProperty:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_UnsupportedProperty *, exc);
  else
    return 0;
}


void FT_UnsupportedProperty::_raise ()
{
  TAO_RAISE(*this);
}


void FT_UnsupportedProperty::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_UnsupportedProperty::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_UnsupportedProperty::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_UnsupportedProperty, 0);
  return retval;
}

FT_UnsupportedProperty::FT_UnsupportedProperty (
  const FT_Name & _tao_nam,
  const FT::Value & _tao_val
)
  : CORBA_UserException  (CORBA::TypeCode::_duplicate (FT::_tc_UnsupportedProperty))
{
  this->nam = _tao_nam;
  this->val = _tao_val;
}

static const CORBA::Long _oc_FT_UnsupportedProperty[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  39,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f55),
  ACE_NTOHL (0x6e737570),
  ACE_NTOHL (0x706f7274),
  ACE_NTOHL (0x65645072),
  ACE_NTOHL (0x6f706572),
  ACE_NTOHL (0x74793a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/FT/UnsupportedProperty:1.0
  20,
  ACE_NTOHL (0x556e7375),
  ACE_NTOHL (0x70706f72),
  ACE_NTOHL (0x74656450),
  ACE_NTOHL (0x726f7065),
  ACE_NTOHL (0x72747900),  // name = UnsupportedProperty
  2, // member count
  4,
  ACE_NTOHL (0x6e616d00),  // name = nam
  CORBA::tk_alias, // typecode kind for typedefs
  364, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  24,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4e),
  ACE_NTOHL (0x616d653a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_alias, // typecode kind for typedefs
  312, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d653a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_sequence, // typecode kind
  252, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  236, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d65436f),
  ACE_NTOHL (0x6d706f6e),
  ACE_NTOHL (0x656e743a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
  14,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x436f6d70),
  ACE_NTOHL (0x6f6e656e),
  ACE_NTOHL (0x74000000),  // name = NameComponent
  2, // member count
  3, ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  5,
  ACE_NTOHL (0x6b696e64),
  ACE_NTOHL (0x0),  // name = kind
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  0U,
  4,
  ACE_NTOHL (0x76616c00),  // name = val
  CORBA::tk_alias, // typecode kind for typedefs
  52, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  25,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f56),
  ACE_NTOHL (0x616c7565),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/Value:1.0
  6,
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x65000000),  // name = Value
  CORBA::tk_any,
};


static CORBA::TypeCode _tc_TAO_tc_FT_UnsupportedProperty (
    CORBA::tk_except,
    sizeof (_oc_FT_UnsupportedProperty),
    (char *) &_oc_FT_UnsupportedProperty,
    0,
    sizeof (FT_UnsupportedProperty));

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_UnsupportedProperty, &_tc_TAO_tc_FT_UnsupportedProperty)
TAO_NAMESPACE_END
// default constructor
FT_InvalidProperty::FT_InvalidProperty (void)
  : CORBA_UserException (::FT::_tc_InvalidProperty)
{
}

// destructor - all members are of self managing types
FT_InvalidProperty::~FT_InvalidProperty (void)
{
}

void FT_InvalidProperty::_tao_any_destructor (void *x)
{
  FT_InvalidProperty *tmp = ACE_static_cast (FT_InvalidProperty*,x);
  delete tmp;
}

// copy constructor
FT_InvalidProperty::FT_InvalidProperty (const ::FT_InvalidProperty &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
    this->nam = _tao_excp.nam;
  this->val = _tao_excp.val;
}

// assignment operator
FT_InvalidProperty&
FT_InvalidProperty::operator= (const ::FT_InvalidProperty &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  this->nam = _tao_excp.nam;
  this->val = _tao_excp.val;
  return *this;
}

// narrow
FT_InvalidProperty *
FT_InvalidProperty::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/InvalidProperty:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_InvalidProperty *, exc);
  else
    return 0;
}


void FT_InvalidProperty::_raise ()
{
  TAO_RAISE(*this);
}


void FT_InvalidProperty::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_InvalidProperty::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_InvalidProperty::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_InvalidProperty, 0);
  return retval;
}

FT_InvalidProperty::FT_InvalidProperty (
  const FT_Name & _tao_nam,
  const FT::Value & _tao_val)
  : CORBA_UserException  (CORBA::TypeCode::_duplicate (FT::_tc_InvalidProperty))
{
  this->nam = _tao_nam;
  this->val = _tao_val;
}

static const CORBA::Long _oc_FT_InvalidProperty[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  35,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f49),
  ACE_NTOHL (0x6e76616c),
  ACE_NTOHL (0x69645072),
  ACE_NTOHL (0x6f706572),
  ACE_NTOHL (0x74793a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/FT/InvalidProperty:1.0
  16,
  ACE_NTOHL (0x496e7661),
  ACE_NTOHL (0x6c696450),
  ACE_NTOHL (0x726f7065),
  ACE_NTOHL (0x72747900),  // name = InvalidProperty
  2, // member count
  4,
  ACE_NTOHL (0x6e616d00),  // name = nam
  CORBA::tk_alias, // typecode kind for typedefs
  364, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  24,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4e),
  ACE_NTOHL (0x616d653a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_alias, // typecode kind for typedefs
  312, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d653a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_sequence, // typecode kind
  252, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  236, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d65436f),
  ACE_NTOHL (0x6d706f6e),
  ACE_NTOHL (0x656e743a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
  14,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x436f6d70),
  ACE_NTOHL (0x6f6e656e),
  ACE_NTOHL (0x74000000),  // name = NameComponent
  2, // member count
  3, ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  5,
  ACE_NTOHL (0x6b696e64),
  ACE_NTOHL (0x0),  // name = kind
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  0U,
  4,
  ACE_NTOHL (0x76616c00),  // name = val
  CORBA::tk_alias, // typecode kind for typedefs
  52, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  25,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f56),
  ACE_NTOHL (0x616c7565),
  ACE_NTOHL (0x3a312e30),
  ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/Value:1.0
  6,
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x65000000),  // name = Value
  CORBA::tk_any,
};


static CORBA::TypeCode _tc_TAO_tc_FT_InvalidProperty (
    CORBA::tk_except,
    sizeof (_oc_FT_InvalidProperty),
    (char *) &_oc_FT_InvalidProperty,
    0,
    sizeof (FT_InvalidProperty));

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_InvalidProperty, &_tc_TAO_tc_FT_InvalidProperty)
TAO_NAMESPACE_END

// default constructor
FT_NoFactory::FT_NoFactory (void)
  : CORBA_UserException (::FT::_tc_NoFactory)
{
}

// destructor - all members are of self managing types
FT_NoFactory::~FT_NoFactory (void)
{
}

void FT_NoFactory::_tao_any_destructor (void *x)
{
  FT_NoFactory *tmp = ACE_static_cast (FT_NoFactory*,x);
  delete tmp;
}

// copy constructor
FT_NoFactory::FT_NoFactory (const ::FT_NoFactory &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
    this->the_location = _tao_excp.the_location;
  this->type_id = CORBA::string_dup (_tao_excp.type_id.in ());
}

// assignment operator
FT_NoFactory&
FT_NoFactory::operator= (const ::FT_NoFactory &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  this->the_location = _tao_excp.the_location;
  this->type_id = CORBA::string_dup (_tao_excp.type_id.in ());
  return *this;
}

// narrow
FT_NoFactory *
FT_NoFactory::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/NoFactory:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_NoFactory *, exc);
  else
    return 0;
}


void FT_NoFactory::_raise ()
{
  TAO_RAISE(*this);
}


void FT_NoFactory::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_NoFactory::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_NoFactory::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_NoFactory, 0);
  return retval;
}

FT_NoFactory::FT_NoFactory (
  const FT::Location & _tao_the_location,
  const char * _tao_type_id
)
  : CORBA_UserException  (CORBA::TypeCode::_duplicate (FT::_tc_NoFactory))
{
  this->the_location =_tao_the_location;
  this->type_id = CORBA::string_dup (_tao_type_id);
}

static const CORBA::Long _oc_FT_NoFactory[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  29, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4e), ACE_NTOHL (0x6f466163), ACE_NTOHL (0x746f7279), ACE_NTOHL (0x3a312e30), ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/NoFactory:1.0
  10, ACE_NTOHL (0x4e6f4661), ACE_NTOHL (0x63746f72), ACE_NTOHL (0x79000000),  // name = NoFactory
  2, // member count
  13, ACE_NTOHL (0x7468655f), ACE_NTOHL (0x6c6f6361), ACE_NTOHL (0x74696f6e), ACE_NTOHL (0x0),  // name = the_location
  CORBA::tk_alias, // typecode kind for typedefs
  424, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    28, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4c), ACE_NTOHL (0x6f636174), ACE_NTOHL (0x696f6e3a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Location:1.0
    9, ACE_NTOHL (0x4c6f6361), ACE_NTOHL (0x74696f6e), ACE_NTOHL (0x0),  // name = Location
    CORBA::tk_alias, // typecode kind for typedefs
    364, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      24, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4e), ACE_NTOHL (0x616d653a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
      5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
      CORBA::tk_alias, // typecode kind for typedefs
      312, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d653a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
        5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
        CORBA::tk_sequence, // typecode kind
        252, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          CORBA::tk_struct, // typecode kind
          236, // encapsulation length
            TAO_ENCAP_BYTE_ORDER, // byte order
            40, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d65436f), ACE_NTOHL (0x6d706f6e), ACE_NTOHL (0x656e743a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
            14, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x436f6d70), ACE_NTOHL (0x6f6e656e), ACE_NTOHL (0x74000000),  // name = NameComponent
            2, // member count
            3, ACE_NTOHL (0x69640000),  // name = id
            CORBA::tk_alias, // typecode kind for typedefs
            64, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4973), ACE_NTOHL (0x7472696e), ACE_NTOHL (0x673a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
              8, ACE_NTOHL (0x49737472), ACE_NTOHL (0x696e6700),  // name = Istring
              CORBA::tk_string,
              0U, // string length

            5, ACE_NTOHL (0x6b696e64), ACE_NTOHL (0x0),  // name = kind
            CORBA::tk_alias, // typecode kind for typedefs
            64, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4973), ACE_NTOHL (0x7472696e), ACE_NTOHL (0x673a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
              8, ACE_NTOHL (0x49737472), ACE_NTOHL (0x696e6700),  // name = Istring
              CORBA::tk_string,
              0U, // string length


          0U,




  8, ACE_NTOHL (0x74797065), ACE_NTOHL (0x5f696400),  // name = type_id
  CORBA::tk_alias, // typecode kind for typedefs
  128, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    26, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f54), ACE_NTOHL (0x79706549), ACE_NTOHL (0x643a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/TypeId:1.0
    7, ACE_NTOHL (0x54797065), ACE_NTOHL (0x49640000),  // name = TypeId
    CORBA::tk_alias, // typecode kind for typedefs
    72, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      35, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x434f5242), ACE_NTOHL (0x412f5265), ACE_NTOHL (0x706f7369), ACE_NTOHL (0x746f7279), ACE_NTOHL (0x49643a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CORBA/RepositoryId:1.0
      13, ACE_NTOHL (0x5265706f), ACE_NTOHL (0x7369746f), ACE_NTOHL (0x72794964), ACE_NTOHL (0x0),  // name = RepositoryId
      CORBA::tk_string,
      0U, // string length


};


static CORBA::TypeCode _tc_TAO_tc_FT_NoFactory (
    CORBA::tk_except,
    sizeof (_oc_FT_NoFactory),
    (char *) &_oc_FT_NoFactory,
    0,
    sizeof (FT_NoFactory));

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_NoFactory, &_tc_TAO_tc_FT_NoFactory)
TAO_NAMESPACE_END

// default constructor
FT_InvalidCriteria::FT_InvalidCriteria (void)
  : CORBA_UserException (::FT::_tc_InvalidCriteria)
{
}

// destructor - all members are of self managing types
FT_InvalidCriteria::~FT_InvalidCriteria (void)
{
}

void FT_InvalidCriteria::_tao_any_destructor (void *x)
{
  FT_InvalidCriteria *tmp = ACE_static_cast (FT_InvalidCriteria*,x);
  delete tmp;
}

// copy constructor
FT_InvalidCriteria::FT_InvalidCriteria (const ::FT_InvalidCriteria &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
    this->invalid_criteria = _tao_excp.invalid_criteria;
}

// assignment operator
FT_InvalidCriteria&
FT_InvalidCriteria::operator= (const ::FT_InvalidCriteria &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  this->invalid_criteria = _tao_excp.invalid_criteria;
  return *this;
}

// narrow
FT_InvalidCriteria *
FT_InvalidCriteria::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/InvalidCriteria:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_InvalidCriteria *, exc);
  else
    return 0;
}


void FT_InvalidCriteria::_raise ()
{
  TAO_RAISE(*this);
}


void FT_InvalidCriteria::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_InvalidCriteria::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_InvalidCriteria::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_InvalidCriteria, 0);
  return retval;
}

FT_InvalidCriteria::FT_InvalidCriteria (
  const FT_Criteria & _tao_invalid_criteria
)
  : CORBA_UserException  (CORBA::TypeCode::_duplicate (FT::_tc_InvalidCriteria))
{
    this->invalid_criteria = _tao_invalid_criteria;
}

static const CORBA::Long _oc_FT_InvalidCriteria[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  35,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f49),
  ACE_NTOHL (0x6e76616c),
  ACE_NTOHL (0x69644372),
  ACE_NTOHL (0x69746572),
  ACE_NTOHL (0x69613a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/FT/InvalidCriteria:1.0
  16,
  ACE_NTOHL (0x496e7661),
  ACE_NTOHL (0x6c696443),
  ACE_NTOHL (0x72697465),
  ACE_NTOHL (0x72696100),  // name = InvalidCriteria
  1, // member count
  17,
  ACE_NTOHL (0x696e7661),
  ACE_NTOHL (0x6c69645f),
  ACE_NTOHL (0x63726974),
  ACE_NTOHL (0x65726961),
  ACE_NTOHL (0x0),  // name = invalid_criteria
  CORBA::tk_alias, // typecode kind for typedefs
  644, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  28,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f43),
  ACE_NTOHL (0x72697465),
  ACE_NTOHL (0x7269613a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Criteria:1.0
  9,
  ACE_NTOHL (0x43726974),
  ACE_NTOHL (0x65726961),
  ACE_NTOHL (0x0),  // name = Criteria
  CORBA::tk_alias, // typecode kind for typedefs
  584, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  30,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f50),
  ACE_NTOHL (0x726f7065),
  ACE_NTOHL (0x72746965),
  ACE_NTOHL (0x733a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/Properties:1.0
  11,
  ACE_NTOHL (0x50726f70),
  ACE_NTOHL (0x65727469),
  ACE_NTOHL (0x65730000),  // name = Properties
  CORBA::tk_sequence, // typecode kind
  520, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  504, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  28,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f50),
  ACE_NTOHL (0x726f7065),
  ACE_NTOHL (0x7274793a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Property:1.0
  9,
  ACE_NTOHL (0x50726f70),
  ACE_NTOHL (0x65727479),
  ACE_NTOHL (0x0),  // name = Property
  2, // member count
  4,
  ACE_NTOHL (0x6e616d00),  // name = nam
  CORBA::tk_alias, // typecode kind for typedefs
  364, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  24,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f4e),
  ACE_NTOHL (0x616d653a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_alias, // typecode kind for typedefs
  312, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  31,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d653a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
  5,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x0),  // name = Name
  CORBA::tk_sequence, // typecode kind
  252, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  CORBA::tk_struct, // typecode kind
  236, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  40,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4e61),
  ACE_NTOHL (0x6d65436f),
  ACE_NTOHL (0x6d706f6e),
  ACE_NTOHL (0x656e743a),
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
  14,
  ACE_NTOHL (0x4e616d65),
  ACE_NTOHL (0x436f6d70),
  ACE_NTOHL (0x6f6e656e),
  ACE_NTOHL (0x74000000),  // name = NameComponent
  2, // member count
  3,
  ACE_NTOHL (0x69640000),  // name = id
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  5,
  ACE_NTOHL (0x6b696e64),
  ACE_NTOHL (0x0),  // name = kind
  CORBA::tk_alias, // typecode kind for typedefs
  64, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x436f734e),
  ACE_NTOHL (0x616d696e),
  ACE_NTOHL (0x672f4973),
  ACE_NTOHL (0x7472696e),
  ACE_NTOHL (0x673a312e),
  ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
  8,
  ACE_NTOHL (0x49737472),
  ACE_NTOHL (0x696e6700),  // name = Istring
  CORBA::tk_string,
  0U, // string length
  0U,
  4,
  ACE_NTOHL (0x76616c00),  // name = val
  CORBA::tk_alias, // typecode kind for typedefs
  52, // encapsulation length
  TAO_ENCAP_BYTE_ORDER, // byte order
  25,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f56),
  ACE_NTOHL (0x616c7565),
  ACE_NTOHL (0x3a312e30), ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/Value:1.0
  6,
  ACE_NTOHL (0x56616c75),
  ACE_NTOHL (0x65000000),  // name = Value
  CORBA::tk_any,
  0U,
};


static CORBA::TypeCode _tc_TAO_tc_FT_InvalidCriteria (CORBA::tk_except, sizeof (_oc_FT_InvalidCriteria), (char *) &_oc_FT_InvalidCriteria, 0, sizeof (FT_InvalidCriteria));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_InvalidCriteria, &_tc_TAO_tc_FT_InvalidCriteria)
TAO_NAMESPACE_END
// default constructor
FT_CannotMeetCriteria::FT_CannotMeetCriteria (void)
  : CORBA_UserException (::FT::_tc_CannotMeetCriteria)
{
}

// destructor - all members are of self managing types
FT_CannotMeetCriteria::~FT_CannotMeetCriteria (void)
{
}

void FT_CannotMeetCriteria::_tao_any_destructor (void *x)
{
  FT_CannotMeetCriteria *tmp = ACE_static_cast (FT_CannotMeetCriteria*,x);
  delete tmp;
}

// copy constructor
FT_CannotMeetCriteria::FT_CannotMeetCriteria (const ::FT_CannotMeetCriteria &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
    this->unmet_criteria = _tao_excp.unmet_criteria;
}

// assignment operator
FT_CannotMeetCriteria&
FT_CannotMeetCriteria::operator= (const ::FT_CannotMeetCriteria &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  this->unmet_criteria = _tao_excp.unmet_criteria;
  return *this;
}

// narrow
FT_CannotMeetCriteria *
FT_CannotMeetCriteria::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/CannotMeetCriteria:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_CannotMeetCriteria *, exc);
  else
    return 0;
}


void FT_CannotMeetCriteria::_raise ()
{
  TAO_RAISE(*this);
}


void FT_CannotMeetCriteria::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_CannotMeetCriteria::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_CannotMeetCriteria::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_CannotMeetCriteria, 0);
  return retval;
}

FT_CannotMeetCriteria::FT_CannotMeetCriteria (
  const FT_Criteria & _tao_unmet_criteria
)
  : CORBA_UserException  (CORBA::TypeCode::_duplicate (FT::_tc_CannotMeetCriteria))
{
    this->unmet_criteria = _tao_unmet_criteria;
}

static const CORBA::Long _oc_FT_CannotMeetCriteria[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  38, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f43), ACE_NTOHL (0x616e6e6f), ACE_NTOHL (0x744d6565), ACE_NTOHL (0x74437269), ACE_NTOHL (0x74657269), ACE_NTOHL (0x613a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/CannotMeetCriteria:1.0
  19, ACE_NTOHL (0x43616e6e), ACE_NTOHL (0x6f744d65), ACE_NTOHL (0x65744372), ACE_NTOHL (0x69746572), ACE_NTOHL (0x69610000),  // name = CannotMeetCriteria
  1, // member count
  15, ACE_NTOHL (0x756e6d65), ACE_NTOHL (0x745f6372), ACE_NTOHL (0x69746572), ACE_NTOHL (0x69610000),  // name = unmet_criteria
  CORBA::tk_alias, // typecode kind for typedefs
  644, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    28, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f43), ACE_NTOHL (0x72697465), ACE_NTOHL (0x7269613a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Criteria:1.0
    9, ACE_NTOHL (0x43726974), ACE_NTOHL (0x65726961), ACE_NTOHL (0x0),  // name = Criteria
    CORBA::tk_alias, // typecode kind for typedefs
    584, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      30, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f50), ACE_NTOHL (0x726f7065), ACE_NTOHL (0x72746965), ACE_NTOHL (0x733a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/Properties:1.0
      11, ACE_NTOHL (0x50726f70), ACE_NTOHL (0x65727469), ACE_NTOHL (0x65730000),  // name = Properties
      CORBA::tk_sequence, // typecode kind
      520, // encapsulation length
        TAO_ENCAP_BYTE_ORDER, // byte order
        CORBA::tk_struct, // typecode kind
        504, // encapsulation length
          TAO_ENCAP_BYTE_ORDER, // byte order
          28, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f50), ACE_NTOHL (0x726f7065), ACE_NTOHL (0x7274793a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Property:1.0
          9, ACE_NTOHL (0x50726f70), ACE_NTOHL (0x65727479), ACE_NTOHL (0x0),  // name = Property
          2, // member count
          4, ACE_NTOHL (0x6e616d00),  // name = nam
          CORBA::tk_alias, // typecode kind for typedefs
          364, // encapsulation length
            TAO_ENCAP_BYTE_ORDER, // byte order
            24, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4e), ACE_NTOHL (0x616d653a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/Name:1.0
            5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
            CORBA::tk_alias, // typecode kind for typedefs
            312, // encapsulation length
              TAO_ENCAP_BYTE_ORDER, // byte order
              31, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d653a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/CosNaming/Name:1.0
              5, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x0),  // name = Name
              CORBA::tk_sequence, // typecode kind
              252, // encapsulation length
                TAO_ENCAP_BYTE_ORDER, // byte order
                CORBA::tk_struct, // typecode kind
                236, // encapsulation length
                  TAO_ENCAP_BYTE_ORDER, // byte order
                  40, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4e61), ACE_NTOHL (0x6d65436f), ACE_NTOHL (0x6d706f6e), ACE_NTOHL (0x656e743a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/CosNaming/NameComponent:1.0
                  14, ACE_NTOHL (0x4e616d65), ACE_NTOHL (0x436f6d70), ACE_NTOHL (0x6f6e656e), ACE_NTOHL (0x74000000),  // name = NameComponent
                  2, // member count
                  3, ACE_NTOHL (0x69640000),  // name = id
                  CORBA::tk_alias, // typecode kind for typedefs
                  64, // encapsulation length
                    TAO_ENCAP_BYTE_ORDER, // byte order
                    34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4973), ACE_NTOHL (0x7472696e), ACE_NTOHL (0x673a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
                    8, ACE_NTOHL (0x49737472), ACE_NTOHL (0x696e6700),  // name = Istring
                    CORBA::tk_string,
                    0U, // string length

                  5, ACE_NTOHL (0x6b696e64), ACE_NTOHL (0x0),  // name = kind
                  CORBA::tk_alias, // typecode kind for typedefs
                  64, // encapsulation length
                    TAO_ENCAP_BYTE_ORDER, // byte order
                    34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x436f734e), ACE_NTOHL (0x616d696e), ACE_NTOHL (0x672f4973), ACE_NTOHL (0x7472696e), ACE_NTOHL (0x673a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/CosNaming/Istring:1.0
                    8, ACE_NTOHL (0x49737472), ACE_NTOHL (0x696e6700),  // name = Istring
                    CORBA::tk_string,
                    0U, // string length


                0U,



          4, ACE_NTOHL (0x76616c00),  // name = val
          CORBA::tk_alias, // typecode kind for typedefs
          52, // encapsulation length
            TAO_ENCAP_BYTE_ORDER, // byte order
            25, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f56), ACE_NTOHL (0x616c7565), ACE_NTOHL (0x3a312e30), ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/Value:1.0
            6, ACE_NTOHL (0x56616c75), ACE_NTOHL (0x65000000),  // name = Value
            CORBA::tk_any,



        0U,



};
static CORBA::TypeCode _tc_TAO_tc_FT_CannotMeetCriteria (CORBA::tk_except, sizeof (_oc_FT_CannotMeetCriteria), (char *) &_oc_FT_CannotMeetCriteria, 0, sizeof (FT_CannotMeetCriteria));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_CannotMeetCriteria, &_tc_TAO_tc_FT_CannotMeetCriteria)
TAO_NAMESPACE_END

// default constructor
FT_PropertyManager::FT_PropertyManager (void)
{}

// destructor
FT_PropertyManager::~FT_PropertyManager (void)
{}

void FT_PropertyManager::_tao_any_destructor (void *x)
{
  FT_PropertyManager *tmp = ACE_static_cast (FT_PropertyManager*,x);
  CORBA::release (tmp);
}

FT_PropertyManager_ptr FT_PropertyManager::_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (CORBA::is_nil (obj))
    return FT_PropertyManager::_nil ();
  if (! obj->_is_local ())
    {
      CORBA::Boolean is_a = obj->_is_a ("IDL:omg.org/FT/PropertyManager:1.0", ACE_TRY_ENV);
      ACE_CHECK_RETURN (FT_PropertyManager::_nil ());
      if (is_a == 0)
        return FT_PropertyManager::_nil ();
    }
  return FT_PropertyManager::_unchecked_narrow (obj, ACE_TRY_ENV);
}

FT_PropertyManager_ptr FT_PropertyManager::_unchecked_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &
  )
{
  if (CORBA::is_nil (obj))
    return FT_PropertyManager::_nil ();
  if (! obj->_is_local ())
    {
      TAO_Stub* stub = obj->_stubobj ();
      if (stub)
        stub->_incr_refcnt ();
      FT_PropertyManager_ptr default_proxy = FT_PropertyManager::_nil ();

      if (obj->_is_collocated () && _TAO_collocation_FT_PropertyManager_Stub_Factory_function_pointer != 0)
        {
          default_proxy = _TAO_collocation_FT_PropertyManager_Stub_Factory_function_pointer (obj);
        }
      if (CORBA::is_nil (default_proxy))
        ACE_NEW_RETURN (default_proxy, FT_PropertyManager (stub), FT_PropertyManager::_nil ());
#if (TAO_HAS_SMART_PROXIES == 1)
      return TAO_FT_PropertyManager_PROXY_FACTORY_ADAPTER::instance ()->create_proxy (default_proxy);
#else
      return default_proxy;
#endif /*TAO_HAS_SMART_PROXIES == 1*/
    }
  else
    return
      ACE_reinterpret_cast
        (
          FT_PropertyManager_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast
                  (
                    ptr_arith_t,
                    &FT_PropertyManager::_narrow
                  )
              )
        );
}

FT_PropertyManager_ptr
FT_PropertyManager::_duplicate (FT_PropertyManager_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

void FT_PropertyManager::set_default_properties (
    const FT_Properties & props,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_InvalidProperty,
    FT_UnsupportedProperty
  ))
{

  static TAO_Exception_Data _tao_FT_PropertyManager_set_default_properties_exceptiondata [] =
  {
    {FT::_tc_InvalidProperty, FT_InvalidProperty::_alloc},
    {FT::_tc_UnsupportedProperty, FT_UnsupportedProperty::_alloc}
  };




  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW (CORBA::INTERNAL ());


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "set_default_properties",
      22,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK;
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK;
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "set_default_properties",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << props)
          ))
          TAO_INTERCEPTOR_THROW (CORBA::MARSHAL ());
        int _invoke_status =
          _tao_call.invoke (_tao_FT_PropertyManager_set_default_properties_exceptiondata, 2, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK;

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
        }

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "set_default_properties",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "set_default_properties",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

}

FT_Properties * FT_PropertyManager::get_default_properties (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{

  FT_Properties *_tao_retval = 0;


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  ACE_NEW_RETURN (_tao_retval, FT_Properties, _tao_retval);
  FT_Properties_var _tao_safe_retval (_tao_retval);
  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "get_default_properties",
      22,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_default_properties",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        int _invoke_status =
          _tao_call.invoke (0, 0, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_default_properties",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "get_default_properties",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

void FT_PropertyManager::remove_default_properties (
    const FT_Properties & props,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_InvalidProperty,
    FT_UnsupportedProperty
  ))
{

  static TAO_Exception_Data _tao_FT_PropertyManager_remove_default_properties_exceptiondata [] =
  {
    {FT::_tc_InvalidProperty, FT_InvalidProperty::_alloc},
    {FT::_tc_UnsupportedProperty, FT_UnsupportedProperty::_alloc}
  };




  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW (CORBA::INTERNAL ());


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "remove_default_properties",
      25,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK;
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK;
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "remove_default_properties",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << props)
          ))
          TAO_INTERCEPTOR_THROW (CORBA::MARSHAL ());
        int _invoke_status =
          _tao_call.invoke (_tao_FT_PropertyManager_remove_default_properties_exceptiondata, 2, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK;

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
        }

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "remove_default_properties",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "remove_default_properties",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

}

void FT_PropertyManager::set_type_properties (
    const char * type_id,
    const FT_Properties & overrides,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_InvalidProperty,
    FT_UnsupportedProperty
  ))
{

  static TAO_Exception_Data _tao_FT_PropertyManager_set_type_properties_exceptiondata [] =
  {
    {FT::_tc_InvalidProperty, FT_InvalidProperty::_alloc},
    {FT::_tc_UnsupportedProperty, FT_UnsupportedProperty::_alloc}
  };




  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW (CORBA::INTERNAL ());


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "set_type_properties",
      19,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK;
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK;
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "set_type_properties",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << type_id) &&
              (_tao_out << overrides)
          ))
          TAO_INTERCEPTOR_THROW (CORBA::MARSHAL ());
        int _invoke_status =
          _tao_call.invoke (_tao_FT_PropertyManager_set_type_properties_exceptiondata, 2, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK;

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
        }

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "set_type_properties",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "set_type_properties",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

}

FT_Properties * FT_PropertyManager::get_type_properties (
    const char * type_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{

  FT_Properties *_tao_retval = 0;


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  ACE_NEW_RETURN (_tao_retval, FT_Properties, _tao_retval);
  FT_Properties_var _tao_safe_retval (_tao_retval);
  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "get_type_properties",
      19,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_type_properties",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << type_id)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (), 0);

        int _invoke_status =
          _tao_call.invoke (0, 0, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_type_properties",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "get_type_properties",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

void FT_PropertyManager::remove_type_properties (
    const char * type_id,
    const FT_Properties & props,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_InvalidProperty,
    FT_UnsupportedProperty
  ))
{

  static TAO_Exception_Data _tao_FT_PropertyManager_remove_type_properties_exceptiondata [] =
  {
    {FT::_tc_InvalidProperty, FT_InvalidProperty::_alloc},
    {FT::_tc_UnsupportedProperty, FT_UnsupportedProperty::_alloc}
  };


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW (CORBA::INTERNAL ());

  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "remove_type_properties",
      22,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK;
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK;
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "remove_type_properties",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << type_id) &&
              (_tao_out << props)
          ))
          TAO_INTERCEPTOR_THROW (CORBA::MARSHAL ());
        int _invoke_status =
          _tao_call.invoke (_tao_FT_PropertyManager_remove_type_properties_exceptiondata, 2, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK;

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
        }

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "remove_type_properties",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "remove_type_properties",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

}

void FT_PropertyManager::set_properties_dynamically (
    FT::ObjectGroup_ptr object_group,
    const FT_Properties & overrides,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_ObjectGroupNotFound,
    FT_InvalidProperty,
    FT_UnsupportedProperty
  ))
{

  static TAO_Exception_Data _tao_FT_PropertyManager_set_properties_dynamically_exceptiondata [] =
  {
    {FT::_tc_ObjectGroupNotFound, FT_ObjectGroupNotFound::_alloc},
    {FT::_tc_InvalidProperty, FT_InvalidProperty::_alloc},
    {FT::_tc_UnsupportedProperty, FT_UnsupportedProperty::_alloc}
  };

  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW (CORBA::INTERNAL ());


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "set_properties_dynamically",
      26,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK;
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK;
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "set_properties_dynamically",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << object_group) &&
              (_tao_out << overrides)
          ))
          TAO_INTERCEPTOR_THROW (CORBA::MARSHAL ());
        int _invoke_status =
          _tao_call.invoke (_tao_FT_PropertyManager_set_properties_dynamically_exceptiondata, 3, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK;

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
        }

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "set_properties_dynamically",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "set_properties_dynamically",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

}

FT_Properties * FT_PropertyManager::get_properties (
    FT::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_ObjectGroupNotFound
  ))
{

  static TAO_Exception_Data _tao_FT_PropertyManager_get_properties_exceptiondata [] =
  {
    {FT::_tc_ObjectGroupNotFound, FT_ObjectGroupNotFound::_alloc}
  };

  FT_Properties *_tao_retval = 0;


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  ACE_NEW_RETURN (_tao_retval, FT_Properties, _tao_retval);
  FT_Properties_var _tao_safe_retval (_tao_retval);
  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "get_properties",
      14,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_properties",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << object_group)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (), 0);

        int _invoke_status =
          _tao_call.invoke (_tao_FT_PropertyManager_get_properties_exceptiondata, 1, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_properties",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "get_properties",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

CORBA::Boolean FT_PropertyManager::_is_a (const CORBA::Char *value, CORBA::Environment &ACE_TRY_ENV)
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/PropertyManager:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/CORBA/Object:1.0")))
  return 1; // success using local knowledge
  else
    return this->CORBA_Object::_is_a (value, ACE_TRY_ENV);
}

void *FT_PropertyManager::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &FT_PropertyManager::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char* FT_PropertyManager::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/PropertyManager:1.0";
}

static const CORBA::Long _oc_FT_PropertyManager[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  35,
  ACE_NTOHL (0x49444c3a),
  ACE_NTOHL (0x6f6d672e),
  ACE_NTOHL (0x6f72672f),
  ACE_NTOHL (0x46542f50),
  ACE_NTOHL (0x726f7065),
  ACE_NTOHL (0x7274794d),
  ACE_NTOHL (0x616e6167),
  ACE_NTOHL (0x65723a31),
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/FT/PropertyManager:1.0
  16,
  ACE_NTOHL (0x50726f70),
  ACE_NTOHL (0x65727479),
  ACE_NTOHL (0x4d616e61),
  ACE_NTOHL (0x67657200),  // name = PropertyManager
};


static CORBA::TypeCode _tc_TAO_tc_FT_PropertyManager (CORBA::tk_objref, sizeof (_oc_FT_PropertyManager), (char *) &_oc_FT_PropertyManager, 0, sizeof (FT_PropertyManager));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_PropertyManager, &_tc_TAO_tc_FT_PropertyManager)
TAO_NAMESPACE_END

// default constructor
FT_ObjectGroupManager::FT_ObjectGroupManager (void)
{}

// destructor
FT_ObjectGroupManager::~FT_ObjectGroupManager (void)
{}

void FT_ObjectGroupManager::_tao_any_destructor (void *x)
{
  FT_ObjectGroupManager *tmp = ACE_static_cast (FT_ObjectGroupManager*,x);
  CORBA::release (tmp);
}

FT_ObjectGroupManager_ptr FT_ObjectGroupManager::_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (CORBA::is_nil (obj))
    return FT_ObjectGroupManager::_nil ();
  if (! obj->_is_local ())
    {
      CORBA::Boolean is_a = obj->_is_a ("IDL:omg.org/FT/ObjectGroupManager:1.0", ACE_TRY_ENV);
      ACE_CHECK_RETURN (FT_ObjectGroupManager::_nil ());
      if (is_a == 0)
        return FT_ObjectGroupManager::_nil ();
    }
  return FT_ObjectGroupManager::_unchecked_narrow (obj,
                                                   ACE_TRY_ENV);
}

FT::ObjectGroupManager_ptr FT_ObjectGroupManager::_unchecked_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &
  )
{
  if (CORBA::is_nil (obj))
    return FT_ObjectGroupManager::_nil ();
  if (! obj->_is_local ())
    {
      TAO_Stub* stub = obj->_stubobj ();
      if (stub)
        stub->_incr_refcnt ();
      FT_ObjectGroupManager_ptr default_proxy = FT_ObjectGroupManager::_nil ();

      if (obj->_is_collocated () && _TAO_collocation_FT_ObjectGroupManager_Stub_Factory_function_pointer != 0)
        {
          default_proxy = _TAO_collocation_FT_ObjectGroupManager_Stub_Factory_function_pointer (obj);
        }
      if (CORBA::is_nil (default_proxy))
        ACE_NEW_RETURN (default_proxy,
                        FT::ObjectGroupManager (stub),
                        FT::ObjectGroupManager::_nil ());
#if (TAO_HAS_SMART_PROXIES == 1)
      return TAO_FT_ObjectGroupManager_PROXY_FACTORY_ADAPTER::instance ()->create_proxy (default_proxy);
#else
      return default_proxy;
#endif /*TAO_HAS_SMART_PROXIES == 1*/
    }
  else
    return
      ACE_reinterpret_cast
        (
          FT_ObjectGroupManager_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast
                  (
                    ptr_arith_t,
                    &FT_ObjectGroupManager::_narrow
                  )
              )
        );
}

FT_ObjectGroupManager_ptr
FT_ObjectGroupManager::_duplicate (FT_ObjectGroupManager_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

FT::ObjectGroup_ptr FT_ObjectGroupManager::create_member (
    FT::ObjectGroup_ptr object_group,
    const FT::Location & the_location,
    const char * type_id,
    const FT_Criteria & the_criteria,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_ObjectGroupNotFound,
    FT_MemberAlreadyPresent,
    FT_NoFactory,
    FT_ObjectNotCreated,
    FT_InvalidCriteria,
    FT_CannotMeetCriteria
  ))
{

  static TAO_Exception_Data _tao_FT_ObjectGroupManager_create_member_exceptiondata [] =
  {
    {FT::_tc_ObjectGroupNotFound, FT_ObjectGroupNotFound::_alloc},
    {FT::_tc_MemberAlreadyPresent, FT_MemberAlreadyPresent::_alloc},
    {FT::_tc_NoFactory, FT_NoFactory::_alloc},
    {FT::_tc_ObjectNotCreated, FT_ObjectNotCreated::_alloc},
    {FT::_tc_InvalidCriteria, FT_InvalidCriteria::_alloc},
    {FT::_tc_CannotMeetCriteria, FT_CannotMeetCriteria::_alloc}
  };

  FT::ObjectGroup_ptr _tao_retval = FT::ObjectGroup::_nil ();
  FT::ObjectGroup_var _tao_safe_retval (_tao_retval);


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "create_member",
      13,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "create_member",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << object_group) &&
              (_tao_out << the_location) &&
              (_tao_out << type_id) &&
              (_tao_out << the_criteria)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (), 0);

        int _invoke_status =
          _tao_call.invoke (_tao_FT_ObjectGroupManager_create_member_exceptiondata, 6, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "create_member",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "create_member",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

FT::ObjectGroup_ptr FT_ObjectGroupManager::add_member (
    FT::ObjectGroup_ptr object_group,
    const FT::Location & the_location,
    CORBA::Object_ptr member,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_ObjectGroupNotFound,
    FT_MemberAlreadyPresent,
    FT_ObjectNotAdded
  ))
{

  static TAO_Exception_Data _tao_FT_ObjectGroupManager_add_member_exceptiondata [] =
  {
    {FT::_tc_ObjectGroupNotFound, FT_ObjectGroupNotFound::_alloc},
    {FT::_tc_MemberAlreadyPresent, FT_MemberAlreadyPresent::_alloc},
    {FT::_tc_ObjectNotAdded, FT_ObjectNotAdded::_alloc}
  };

  FT::ObjectGroup_ptr _tao_retval = FT::ObjectGroup::_nil ();
  FT::ObjectGroup_var _tao_safe_retval (_tao_retval);


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "add_member",
      10,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "add_member",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << object_group) &&
              (_tao_out << the_location) &&
              (_tao_out << member)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (), 0);

        int _invoke_status =
          _tao_call.invoke (_tao_FT_ObjectGroupManager_add_member_exceptiondata, 3, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "add_member",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "add_member",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

FT::ObjectGroup_ptr FT_ObjectGroupManager::remove_member (
    FT::ObjectGroup_ptr object_group,
    const FT::Location & the_location,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_ObjectGroupNotFound,
    FT_MemberNotFound
  ))
{

  static TAO_Exception_Data _tao_FT_ObjectGroupManager_remove_member_exceptiondata [] =
  {
    {FT::_tc_ObjectGroupNotFound, FT_ObjectGroupNotFound::_alloc},
    {FT::_tc_MemberNotFound, FT_MemberNotFound::_alloc}
  };

  FT::ObjectGroup_ptr _tao_retval = FT::ObjectGroup::_nil ();
  FT::ObjectGroup_var _tao_safe_retval (_tao_retval);


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "remove_member",
      13,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "remove_member",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << object_group) &&
              (_tao_out << the_location)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (), 0);

        int _invoke_status =
          _tao_call.invoke (_tao_FT_ObjectGroupManager_remove_member_exceptiondata, 2, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "remove_member",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "remove_member",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

FT::ObjectGroup_ptr FT_ObjectGroupManager::set_primary_member (
    FT::ObjectGroup_ptr object_group,
    const FT::Location & the_location,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_ObjectGroupNotFound,
    FT_MemberNotFound,
    FT_PrimaryNotSet,
    FT_BadReplicationStyle
  ))
{

  static TAO_Exception_Data _tao_FT_ObjectGroupManager_set_primary_member_exceptiondata [] =
  {
    {FT::_tc_ObjectGroupNotFound, FT_ObjectGroupNotFound::_alloc},
    {FT::_tc_MemberNotFound, FT_MemberNotFound::_alloc},
    {FT::_tc_PrimaryNotSet, FT_PrimaryNotSet::_alloc},
    {FT::_tc_BadReplicationStyle, FT_BadReplicationStyle::_alloc}
  };

  FT::ObjectGroup_ptr _tao_retval = FT::ObjectGroup::_nil ();
  FT::ObjectGroup_var _tao_safe_retval (_tao_retval);


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "set_primary_member",
      18,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "set_primary_member",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << object_group) &&
              (_tao_out << the_location)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (), 0);

        int _invoke_status =
          _tao_call.invoke (_tao_FT_ObjectGroupManager_set_primary_member_exceptiondata, 4, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "set_primary_member",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "set_primary_member",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

FT_Locations * FT_ObjectGroupManager::locations_of_members (
    FT::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_ObjectGroupNotFound
  ))
{

  static TAO_Exception_Data _tao_FT_ObjectGroupManager_locations_of_members_exceptiondata [] =
  {
    {FT::_tc_ObjectGroupNotFound, FT_ObjectGroupNotFound::_alloc}
  };

  FT_Locations *_tao_retval = 0;


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  ACE_NEW_RETURN (_tao_retval, FT_Locations, _tao_retval);
  FT_Locations_var _tao_safe_retval (_tao_retval);
  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "locations_of_members",
      20,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "locations_of_members",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << object_group)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (), 0);

        int _invoke_status =
          _tao_call.invoke (_tao_FT_ObjectGroupManager_locations_of_members_exceptiondata, 1, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "locations_of_members",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "locations_of_members",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

FT::ObjectGroupId FT_ObjectGroupManager::get_object_group_id (
    FT::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_ObjectGroupNotFound
  ))
{

  static TAO_Exception_Data _tao_FT_ObjectGroupManager_get_object_group_id_exceptiondata [] =
  {
    {FT::_tc_ObjectGroupNotFound, FT_ObjectGroupNotFound::_alloc}
  };

  FT::ObjectGroupId _tao_retval = 0;

  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), _tao_retval);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "get_object_group_id",
      19,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN  (_tao_retval);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN  (_tao_retval);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_object_group_id",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << object_group)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (), _tao_retval);

        int _invoke_status =
          _tao_call.invoke (_tao_FT_ObjectGroupManager_get_object_group_id_exceptiondata, 1, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), _tao_retval);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_retval)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), _tao_retval);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_object_group_id",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "get_object_group_id",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN  (_tao_retval);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_retval;
}

FT::ObjectGroup_ptr FT_ObjectGroupManager::get_object_group_ref (
    FT::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_ObjectGroupNotFound
  ))
{

  static TAO_Exception_Data _tao_FT_ObjectGroupManager_get_object_group_ref_exceptiondata [] =
  {
    {FT::_tc_ObjectGroupNotFound, FT_ObjectGroupNotFound::_alloc}
  };

  FT::ObjectGroup_ptr _tao_retval = FT::ObjectGroup::_nil ();
  FT::ObjectGroup_var _tao_safe_retval (_tao_retval);


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "get_object_group_ref",
      20,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_object_group_ref",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << object_group)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (), 0);

        int _invoke_status =
          _tao_call.invoke (_tao_FT_ObjectGroupManager_get_object_group_ref_exceptiondata, 1, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_object_group_ref",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "get_object_group_ref",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

CORBA::Object_ptr FT_ObjectGroupManager::get_member_ref (
    FT::ObjectGroup_ptr object_group,
    const FT::Location & loc,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_ObjectGroupNotFound,
    FT_MemberNotFound
  ))
{

  static TAO_Exception_Data _tao_FT_ObjectGroupManager_get_member_ref_exceptiondata [] =
  {
    {FT::_tc_ObjectGroupNotFound, FT_ObjectGroupNotFound::_alloc},
    {FT::_tc_MemberNotFound, FT_MemberNotFound::_alloc}
  };

  CORBA::Object_ptr _tao_retval = CORBA::Object::_nil ();
  CORBA::Object_var _tao_safe_retval (_tao_retval);


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "get_member_ref",
      14,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_member_ref",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << object_group) &&
              (_tao_out << loc)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (), 0);

        int _invoke_status =
          _tao_call.invoke (_tao_FT_ObjectGroupManager_get_member_ref_exceptiondata, 2, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_member_ref",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "get_member_ref",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

CORBA::Boolean FT_ObjectGroupManager::_is_a (const CORBA::Char *value, CORBA::Environment &ACE_TRY_ENV)
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/ObjectGroupManager:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/CORBA/Object:1.0")))
  return 1; // success using local knowledge
  else
    return this->CORBA_Object::_is_a (value, ACE_TRY_ENV);
}

void *FT_ObjectGroupManager::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &FT_ObjectGroupManager::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char* FT_ObjectGroupManager::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/ObjectGroupManager:1.0";
}

static const CORBA::Long _oc_FT_ObjectGroupManager[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  38, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4f), ACE_NTOHL (0x626a6563), ACE_NTOHL (0x7447726f), ACE_NTOHL (0x75704d61), ACE_NTOHL (0x6e616765), ACE_NTOHL (0x723a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/ObjectGroupManager:1.0
  19, ACE_NTOHL (0x4f626a65), ACE_NTOHL (0x63744772), ACE_NTOHL (0x6f75704d), ACE_NTOHL (0x616e6167), ACE_NTOHL (0x65720000),  // name = ObjectGroupManager
};
static CORBA::TypeCode _tc_TAO_tc_FT_ObjectGroupManager (CORBA::tk_objref, sizeof (_oc_FT_ObjectGroupManager), (char *) &_oc_FT_ObjectGroupManager, 0, sizeof (FT_ObjectGroupManager));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_ObjectGroupManager, &_tc_TAO_tc_FT_ObjectGroupManager)
TAO_NAMESPACE_END

// default constructor
FT_GenericFactory::FT_GenericFactory (void)
{}

// destructor
FT_GenericFactory::~FT_GenericFactory (void)
{}

void FT_GenericFactory::_tao_any_destructor (void *x)
{
  FT_GenericFactory *tmp = ACE_static_cast (FT_GenericFactory*,x);
  CORBA::release (tmp);
}

FT_GenericFactory_ptr FT_GenericFactory::_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (CORBA::is_nil (obj))
    return FT_GenericFactory::_nil ();
  if (! obj->_is_local ())
    {
      CORBA::Boolean is_a = obj->_is_a ("IDL:omg.org/FT/GenericFactory:1.0", ACE_TRY_ENV);
      ACE_CHECK_RETURN (FT_GenericFactory::_nil ());
      if (is_a == 0)
        return FT_GenericFactory::_nil ();
    }
  return FT_GenericFactory::_unchecked_narrow (obj, ACE_TRY_ENV);
}

FT_GenericFactory_ptr FT_GenericFactory::_unchecked_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &
  )
{
  if (CORBA::is_nil (obj))
    return FT_GenericFactory::_nil ();
  if (! obj->_is_local ())
    {
      TAO_Stub* stub = obj->_stubobj ();
      if (stub)
        stub->_incr_refcnt ();
      FT_GenericFactory_ptr default_proxy = FT_GenericFactory::_nil ();

      if (obj->_is_collocated () && _TAO_collocation_FT_GenericFactory_Stub_Factory_function_pointer != 0)
        {
          default_proxy = _TAO_collocation_FT_GenericFactory_Stub_Factory_function_pointer (obj);
        }
      if (CORBA::is_nil (default_proxy))
        ACE_NEW_RETURN (default_proxy, ::FT_GenericFactory (stub), FT_GenericFactory::_nil ());
#if (TAO_HAS_SMART_PROXIES == 1)
      return TAO_FT_GenericFactory_PROXY_FACTORY_ADAPTER::instance ()->create_proxy (default_proxy);
#else
      return default_proxy;
#endif /*TAO_HAS_SMART_PROXIES == 1*/
    }
  else
    return
      ACE_reinterpret_cast
        (
          FT_GenericFactory_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast
                  (
                    ptr_arith_t,
                    &FT_GenericFactory::_narrow
                  )
              )
        );
}

FT_GenericFactory_ptr
FT_GenericFactory::_duplicate (FT_GenericFactory_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

static const CORBA::Long _oc_FT_GenericFactory_FactoryCreationId[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  52, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f47), ACE_NTOHL (0x656e6572), ACE_NTOHL (0x69634661), ACE_NTOHL (0x63746f72), ACE_NTOHL (0x792f4661), ACE_NTOHL (0x63746f72), ACE_NTOHL (0x79437265), ACE_NTOHL (0x6174696f), ACE_NTOHL (0x6e49643a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/GenericFactory/FactoryCreationId:1.0
  18, ACE_NTOHL (0x46616374), ACE_NTOHL (0x6f727943), ACE_NTOHL (0x72656174), ACE_NTOHL (0x696f6e49), ACE_NTOHL (0x64000000),  // name = FactoryCreationId
  CORBA::tk_any,

};
static CORBA::TypeCode _tc_TAO_tc_FT_GenericFactory_FactoryCreationId (CORBA::tk_alias, sizeof (_oc_FT_GenericFactory_FactoryCreationId), (char *) &_oc_FT_GenericFactory_FactoryCreationId, 0, sizeof (FT_GenericFactory::FactoryCreationId));
CORBA::TypeCode_ptr FT_GenericFactory::_tc_FactoryCreationId = &_tc_TAO_tc_FT_GenericFactory_FactoryCreationId;

CORBA::Object_ptr FT_GenericFactory::create_object (
    const char * type_id,
    const FT_Criteria & the_criteria,
    FT_GenericFactory::FactoryCreationId_out factory_creation_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_NoFactory,
    FT_ObjectNotCreated,
    FT_InvalidCriteria,
    FT_InvalidProperty,
    FT_CannotMeetCriteria
  ))
{

  static TAO_Exception_Data _tao_FT_GenericFactory_create_object_exceptiondata [] =
  {
    {FT::_tc_NoFactory, FT_NoFactory::_alloc},
    {FT::_tc_ObjectNotCreated, FT_ObjectNotCreated::_alloc},
    {FT::_tc_InvalidCriteria, FT_InvalidCriteria::_alloc},
    {FT::_tc_InvalidProperty, FT_InvalidProperty::_alloc},
    {FT::_tc_CannotMeetCriteria, FT_CannotMeetCriteria::_alloc}
  };

  CORBA::Object_ptr _tao_retval = CORBA::Object::_nil ();
  CORBA::Object_var _tao_safe_retval (_tao_retval);


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  ACE_NEW_RETURN (factory_creation_id.ptr (), CORBA::Any, 0);
  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "create_object",
      13,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "create_object",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << type_id) &&
              (_tao_out << the_criteria)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (), 0);

        int _invoke_status =
          _tao_call.invoke (_tao_FT_GenericFactory_create_object_exceptiondata, 5, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ()) &&
                                          (_tao_in >> *factory_creation_id.ptr ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "create_object",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "create_object",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

void FT_GenericFactory::delete_object (
    const FT_GenericFactory::FactoryCreationId & factory_creation_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_ObjectNotFound
  ))
{

  static TAO_Exception_Data _tao_FT_GenericFactory_delete_object_exceptiondata [] =
  {
    {FT::_tc_ObjectNotFound, FT_ObjectNotFound::_alloc}
  };




  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW (CORBA::INTERNAL ());


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "delete_object",
      13,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK;
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK;
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "delete_object",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << factory_creation_id)
          ))
          TAO_INTERCEPTOR_THROW (CORBA::MARSHAL ());
        int _invoke_status =
          _tao_call.invoke (_tao_FT_GenericFactory_delete_object_exceptiondata, 1, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK;

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
        }

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "delete_object",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "delete_object",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

}

CORBA::Boolean FT_GenericFactory::_is_a (const CORBA::Char *value, CORBA::Environment &ACE_TRY_ENV)
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/GenericFactory:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/CORBA/Object:1.0")))
  return 1; // success using local knowledge
  else
    return this->CORBA_Object::_is_a (value, ACE_TRY_ENV);
}

void *FT_GenericFactory::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &FT_GenericFactory::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char* FT_GenericFactory::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/GenericFactory:1.0";
}

static const CORBA::Long _oc_FT_GenericFactory[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f47), ACE_NTOHL (0x656e6572), ACE_NTOHL (0x69634661), ACE_NTOHL (0x63746f72), ACE_NTOHL (0x793a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/GenericFactory:1.0
  15, ACE_NTOHL (0x47656e65), ACE_NTOHL (0x72696346), ACE_NTOHL (0x6163746f), ACE_NTOHL (0x72790000),  // name = GenericFactory
};
static CORBA::TypeCode _tc_TAO_tc_FT_GenericFactory (CORBA::tk_objref, sizeof (_oc_FT_GenericFactory), (char *) &_oc_FT_GenericFactory, 0, sizeof (FT_GenericFactory));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_GenericFactory, &_tc_TAO_tc_FT_GenericFactory)
TAO_NAMESPACE_END

// default constructor
FT_ReplicationManager::FT_ReplicationManager (void)
{}

// destructor
FT_ReplicationManager::~FT_ReplicationManager (void)
{}

void FT_ReplicationManager::_tao_any_destructor (void *x)
{
  FT_ReplicationManager *tmp = ACE_static_cast (FT_ReplicationManager*,x);
  CORBA::release (tmp);
}

FT_ReplicationManager_ptr FT_ReplicationManager::_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (CORBA::is_nil (obj))
    return FT_ReplicationManager::_nil ();
  if (! obj->_is_local ())
    {
      CORBA::Boolean is_a = obj->_is_a ("IDL:omg.org/FT/ReplicationManager:1.0", ACE_TRY_ENV);
      ACE_CHECK_RETURN (FT_ReplicationManager::_nil ());
      if (is_a == 0)
        return FT_ReplicationManager::_nil ();
    }
  return FT_ReplicationManager::_unchecked_narrow (obj, ACE_TRY_ENV);
}

FT_ReplicationManager_ptr FT_ReplicationManager::_unchecked_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &
  )
{
  if (CORBA::is_nil (obj))
    return FT_ReplicationManager::_nil ();
  if (! obj->_is_local ())
    {
      TAO_Stub* stub = obj->_stubobj ();
      if (stub)
        stub->_incr_refcnt ();
      FT_ReplicationManager_ptr default_proxy = FT_ReplicationManager::_nil ();

      if (obj->_is_collocated () && _TAO_collocation_FT_ReplicationManager_Stub_Factory_function_pointer != 0)
        {
          default_proxy = _TAO_collocation_FT_ReplicationManager_Stub_Factory_function_pointer (obj);
        }
      if (CORBA::is_nil (default_proxy))
        ACE_NEW_RETURN (default_proxy, ::FT_ReplicationManager (stub), FT_ReplicationManager::_nil ());
#if (TAO_HAS_SMART_PROXIES == 1)
      return TAO_FT_ReplicationManager_PROXY_FACTORY_ADAPTER::instance ()->create_proxy (default_proxy);
#else
      return default_proxy;
#endif /*TAO_HAS_SMART_PROXIES == 1*/
    }
  else
    return
      ACE_reinterpret_cast
        (
          FT_ReplicationManager_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast
                  (
                    ptr_arith_t,
                    &FT_ReplicationManager::_narrow
                  )
              )
        );
}

FT_ReplicationManager_ptr
FT_ReplicationManager::_duplicate (FT_ReplicationManager_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

void FT_ReplicationManager::register_fault_notifier (
    FT_FaultNotifier_ptr fault_notifier,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{




  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW (CORBA::INTERNAL ());


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "register_fault_notifier",
      23,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK;
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK;
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "register_fault_notifier",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << fault_notifier)
          ))
          TAO_INTERCEPTOR_THROW (CORBA::MARSHAL ());
        int _invoke_status =
          _tao_call.invoke (0, 0, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK;

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
        }

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "register_fault_notifier",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "register_fault_notifier",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

}

FT_FaultNotifier_ptr FT_ReplicationManager::get_fault_notifier (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_InterfaceNotFound
  ))
{

  static TAO_Exception_Data _tao_FT_ReplicationManager_get_fault_notifier_exceptiondata [] =
  {
    {FT::_tc_InterfaceNotFound, FT_InterfaceNotFound::_alloc}
  };

  FT_FaultNotifier_ptr _tao_retval = FT_FaultNotifier::_nil ();
  FT_FaultNotifier_var _tao_safe_retval (_tao_retval);


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "get_fault_notifier",
      18,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_fault_notifier",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        int _invoke_status =
          _tao_call.invoke (_tao_FT_ReplicationManager_get_fault_notifier_exceptiondata, 1, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_fault_notifier",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "get_fault_notifier",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

CORBA::Boolean FT_ReplicationManager::_is_a (const CORBA::Char *value, CORBA::Environment &ACE_TRY_ENV)
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/ReplicationManager:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/PropertyManager:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/ObjectGroupManager:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/GenericFactory:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/CORBA/Object:1.0")))
  return 1; // success using local knowledge
  else
    return this->CORBA_Object::_is_a (value, ACE_TRY_ENV);
}

void *FT_ReplicationManager::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &FT_ReplicationManager::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &FT_PropertyManager::_narrow))
    retv = ACE_reinterpret_cast
      (
        void *,
        ACE_static_cast
          (
            FT_PropertyManager_ptr,
            this
          )
      );
  else if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      & ::FT_ObjectGroupManager::_narrow))
    retv = ACE_reinterpret_cast
      (
        void *,
        ACE_static_cast
          (
            FT_ObjectGroupManager_ptr,
            this
          )
      );
  else if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &FT_GenericFactory::_narrow))
    retv = ACE_reinterpret_cast
      (
        void *,
        ACE_static_cast
          (
            FT_GenericFactory_ptr,
            this
          )
      );
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char* FT_ReplicationManager::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/ReplicationManager:1.0";
}

static const CORBA::Long _oc_FT_ReplicationManager[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  38, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f52), ACE_NTOHL (0x65706c69), ACE_NTOHL (0x63617469), ACE_NTOHL (0x6f6e4d61), ACE_NTOHL (0x6e616765), ACE_NTOHL (0x723a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/ReplicationManager:1.0
  19, ACE_NTOHL (0x5265706c), ACE_NTOHL (0x69636174), ACE_NTOHL (0x696f6e4d), ACE_NTOHL (0x616e6167), ACE_NTOHL (0x65720000),  // name = ReplicationManager
};
static CORBA::TypeCode _tc_TAO_tc_FT_ReplicationManager (CORBA::tk_objref, sizeof (_oc_FT_ReplicationManager), (char *) &_oc_FT_ReplicationManager, 0, sizeof (FT_ReplicationManager));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_ReplicationManager, &_tc_TAO_tc_FT_ReplicationManager)
TAO_NAMESPACE_END

// default constructor
FT_PullMonitorable::FT_PullMonitorable (void)
{}

// destructor
FT_PullMonitorable::~FT_PullMonitorable (void)
{}

void FT_PullMonitorable::_tao_any_destructor (void *x)
{
  FT_PullMonitorable *tmp = ACE_static_cast (FT_PullMonitorable*,x);
  CORBA::release (tmp);
}

FT_PullMonitorable_ptr FT_PullMonitorable::_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (CORBA::is_nil (obj))
    return FT_PullMonitorable::_nil ();
  if (! obj->_is_local ())
    {
      CORBA::Boolean is_a = obj->_is_a ("IDL:omg.org/FT/PullMonitorable:1.0", ACE_TRY_ENV);
      ACE_CHECK_RETURN (FT_PullMonitorable::_nil ());
      if (is_a == 0)
        return FT_PullMonitorable::_nil ();
    }
  return FT_PullMonitorable::_unchecked_narrow (obj, ACE_TRY_ENV);
}

FT_PullMonitorable_ptr FT_PullMonitorable::_unchecked_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &
  )
{
  if (CORBA::is_nil (obj))
    return FT_PullMonitorable::_nil ();
  if (! obj->_is_local ())
    {
      TAO_Stub* stub = obj->_stubobj ();
      if (stub)
        stub->_incr_refcnt ();
      FT_PullMonitorable_ptr default_proxy = FT_PullMonitorable::_nil ();

      if (obj->_is_collocated () && _TAO_collocation_FT_PullMonitorable_Stub_Factory_function_pointer != 0)
        {
          default_proxy = _TAO_collocation_FT_PullMonitorable_Stub_Factory_function_pointer (obj);
        }
      if (CORBA::is_nil (default_proxy))
        ACE_NEW_RETURN (default_proxy, ::FT_PullMonitorable (stub), FT_PullMonitorable::_nil ());
      #if (TAO_HAS_SMART_PROXIES == 1)
        return TAO_FT_PullMonitorable_PROXY_FACTORY_ADAPTER::instance ()->create_proxy (default_proxy);
      #else
        return default_proxy;
      #endif /*TAO_HAS_SMART_PROXIES == 1*/
    }
  else
    return
      ACE_reinterpret_cast
        (
          FT_PullMonitorable_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast
                  (
                    ptr_arith_t,
                    &FT_PullMonitorable::_narrow
                  )
              )
        );
}

FT_PullMonitorable_ptr
FT_PullMonitorable::_duplicate (FT_PullMonitorable_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

CORBA::Boolean FT_PullMonitorable::is_alive (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{

  CORBA::Boolean _tao_retval = 0;


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), _tao_retval);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "is_alive",
      8,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN  (_tao_retval);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN  (_tao_retval);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "is_alive",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);

        int _invoke_status =
          _tao_call.invoke (0, 0, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), _tao_retval);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> CORBA::Any::to_boolean (_tao_retval))
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), _tao_retval);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "is_alive",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "is_alive",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN  (_tao_retval);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_retval;
}

CORBA::Boolean FT_PullMonitorable::_is_a (const CORBA::Char *value, CORBA::Environment &ACE_TRY_ENV)
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/PullMonitorable:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/CORBA/Object:1.0")))
  return 1; // success using local knowledge
  else
    return this->CORBA_Object::_is_a (value, ACE_TRY_ENV);
}

void *FT_PullMonitorable::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &::FT_PullMonitorable::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char* FT_PullMonitorable::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/PullMonitorable:1.0";
}

static const CORBA::Long _oc_FT_PullMonitorable[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  35, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f50), ACE_NTOHL (0x756c6c4d), ACE_NTOHL (0x6f6e6974), ACE_NTOHL (0x6f726162), ACE_NTOHL (0x6c653a31), ACE_NTOHL (0x2e300000),  // repository ID = IDL:omg.org/FT/PullMonitorable:1.0
  16, ACE_NTOHL (0x50756c6c), ACE_NTOHL (0x4d6f6e69), ACE_NTOHL (0x746f7261), ACE_NTOHL (0x626c6500),  // name = PullMonitorable
};
static CORBA::TypeCode _tc_TAO_tc_FT_PullMonitorable (CORBA::tk_objref, sizeof (_oc_FT_PullMonitorable), (char *) &_oc_FT_PullMonitorable, 0, sizeof (FT_PullMonitorable));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_PullMonitorable, &_tc_TAO_tc_FT_PullMonitorable)
TAO_NAMESPACE_END

// default constructor
FT_FaultNotifier::FT_FaultNotifier (void)
{}

// destructor
FT_FaultNotifier::~FT_FaultNotifier (void)
{}

void FT_FaultNotifier::_tao_any_destructor (void *x)
{
  FT_FaultNotifier *tmp = ACE_static_cast (FT_FaultNotifier*,x);
  CORBA::release (tmp);
}

FT_FaultNotifier_ptr FT_FaultNotifier::_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (CORBA::is_nil (obj))
    return FT_FaultNotifier::_nil ();
  if (! obj->_is_local ())
    {
      CORBA::Boolean is_a = obj->_is_a ("IDL:omg.org/FT/FaultNotifier:1.0", ACE_TRY_ENV);
      ACE_CHECK_RETURN (FT_FaultNotifier::_nil ());
      if (is_a == 0)
        return FT_FaultNotifier::_nil ();
    }
  return FT_FaultNotifier::_unchecked_narrow (obj, ACE_TRY_ENV);
}

FT_FaultNotifier_ptr FT_FaultNotifier::_unchecked_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &
  )
{
  if (CORBA::is_nil (obj))
    return FT_FaultNotifier::_nil ();
  if (! obj->_is_local ())
    {
      TAO_Stub* stub = obj->_stubobj ();
      if (stub)
        stub->_incr_refcnt ();
      FT_FaultNotifier_ptr default_proxy = FT_FaultNotifier::_nil ();

      if (obj->_is_collocated () && _TAO_collocation_FT_FaultNotifier_Stub_Factory_function_pointer != 0)
        {
          default_proxy = _TAO_collocation_FT_FaultNotifier_Stub_Factory_function_pointer (obj);
        }
      if (CORBA::is_nil (default_proxy))
        ACE_NEW_RETURN (default_proxy, FT_FaultNotifier (stub), FT_FaultNotifier::_nil ());
#if (TAO_HAS_SMART_PROXIES == 1)
      return TAO_FT_FaultNotifier_PROXY_FACTORY_ADAPTER::instance ()->create_proxy (default_proxy);
#else
      return default_proxy;
#endif /*TAO_HAS_SMART_PROXIES == 1*/
    }
  else
    return
      ACE_reinterpret_cast
        (
          FT_FaultNotifier_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast
                  (
                    ptr_arith_t,
                    &FT_FaultNotifier::_narrow
                  )
              )
        );
}

FT_FaultNotifier_ptr
FT_FaultNotifier::_duplicate (FT_FaultNotifier_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

static const CORBA::Long _oc_FT_FaultNotifier_ConsumerId[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  44, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f46), ACE_NTOHL (0x61756c74), ACE_NTOHL (0x4e6f7469), ACE_NTOHL (0x66696572), ACE_NTOHL (0x2f436f6e), ACE_NTOHL (0x73756d65), ACE_NTOHL (0x7249643a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/FaultNotifier/ConsumerId:1.0
  11, ACE_NTOHL (0x436f6e73), ACE_NTOHL (0x756d6572), ACE_NTOHL (0x49640000),  // name = ConsumerId
  CORBA::tk_ulonglong,

};
static CORBA::TypeCode _tc_TAO_tc_FT_FaultNotifier_ConsumerId (CORBA::tk_alias, sizeof (_oc_FT_FaultNotifier_ConsumerId), (char *) &_oc_FT_FaultNotifier_ConsumerId, 0, sizeof (FT_FaultNotifier::ConsumerId));
CORBA::TypeCode_ptr FT_FaultNotifier::_tc_ConsumerId = &_tc_TAO_tc_FT_FaultNotifier_ConsumerId;

void FT_FaultNotifier::push_structured_fault (
    const CosNotification::StructuredEvent & event,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{




  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW (CORBA::INTERNAL ());


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "push_structured_fault",
      21,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK;
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK;
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "push_structured_fault",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << event)
          ))
          TAO_INTERCEPTOR_THROW (CORBA::MARSHAL ());
        int _invoke_status =
          _tao_call.invoke (0, 0, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK;

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
        }

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "push_structured_fault",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "push_structured_fault",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

}

void FT_FaultNotifier::push_sequence_fault (
    const CosNotification::EventBatch & events,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{




  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW (CORBA::INTERNAL ());


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "push_sequence_fault",
      19,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK;
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK;
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "push_sequence_fault",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << events)
          ))
          TAO_INTERCEPTOR_THROW (CORBA::MARSHAL ());
        int _invoke_status =
          _tao_call.invoke (0, 0, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK;

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
        }

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "push_sequence_fault",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "push_sequence_fault",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

}

CosNotifyFilter::Filter_ptr FT_FaultNotifier::create_subscription_filter (
    const char * constraint_grammar,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::InvalidGrammar
  ))
{

  static TAO_Exception_Data _tao_FT_FaultNotifier_create_subscription_filter_exceptiondata [] =
  {
    {CosNotifyFilter::_tc_InvalidGrammar, CosNotifyFilter::InvalidGrammar::_alloc}
  };

  CosNotifyFilter::Filter_ptr _tao_retval = CosNotifyFilter::Filter::_nil ();
  CosNotifyFilter::Filter_var _tao_safe_retval (_tao_retval);


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "create_subscription_filter",
      26,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "create_subscription_filter",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << constraint_grammar)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (), 0);

        int _invoke_status =
          _tao_call.invoke (_tao_FT_FaultNotifier_create_subscription_filter_exceptiondata, 1, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "create_subscription_filter",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "create_subscription_filter",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

FT_FaultNotifier::ConsumerId FT_FaultNotifier::connect_structured_fault_consumer (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{

  FT_FaultNotifier::ConsumerId _tao_retval = 0;


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), _tao_retval);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "connect_structured_fault_consumer",
      33,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN  (_tao_retval);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN  (_tao_retval);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "connect_structured_fault_consumer",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << push_consumer) &&
              (_tao_out << filter)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (), _tao_retval);

        int _invoke_status =
          _tao_call.invoke (0, 0, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), _tao_retval);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_retval)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), _tao_retval);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "connect_structured_fault_consumer",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "connect_structured_fault_consumer",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN  (_tao_retval);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_retval;
}

FT_FaultNotifier::ConsumerId FT_FaultNotifier::connect_sequence_fault_consumer (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{

  FT_FaultNotifier::ConsumerId _tao_retval = 0;


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), _tao_retval);


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "connect_sequence_fault_consumer",
      31,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN  (_tao_retval);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN  (_tao_retval);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "connect_sequence_fault_consumer",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << push_consumer) &&
              (_tao_out << filter)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (), _tao_retval);

        int _invoke_status =
          _tao_call.invoke (0, 0, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), _tao_retval);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_retval)
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), _tao_retval);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "connect_sequence_fault_consumer",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "connect_sequence_fault_consumer",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN  (_tao_retval);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_retval;
}

void FT_FaultNotifier::disconnect_consumer (
    FT_FaultNotifier::ConsumerId connection,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventComm::Disconnected
  ))
{

  static TAO_Exception_Data _tao_FT_FaultNotifier_disconnect_consumer_exceptiondata [] =
  {
    {CosEventComm::_tc_Disconnected, CosEventComm::Disconnected::_alloc}
  };




  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW (CORBA::INTERNAL ());


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "disconnect_consumer",
      19,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK;
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK;
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "disconnect_consumer",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << connection)
          ))
          TAO_INTERCEPTOR_THROW (CORBA::MARSHAL ());
        int _invoke_status =
          _tao_call.invoke (_tao_FT_FaultNotifier_disconnect_consumer_exceptiondata, 1, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK;

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
        }

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "disconnect_consumer",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "disconnect_consumer",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

}

CORBA::Boolean FT_FaultNotifier::_is_a (const CORBA::Char *value, CORBA::Environment &ACE_TRY_ENV)
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/FaultNotifier:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/CORBA/Object:1.0")))
  return 1; // success using local knowledge
  else
    return this->CORBA_Object::_is_a (value, ACE_TRY_ENV);
}

void *FT_FaultNotifier::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &FT_FaultNotifier::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char* FT_FaultNotifier::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/FaultNotifier:1.0";
}

static const CORBA::Long _oc_FT_FaultNotifier[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  33, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f46), ACE_NTOHL (0x61756c74), ACE_NTOHL (0x4e6f7469), ACE_NTOHL (0x66696572), ACE_NTOHL (0x3a312e30), ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/FaultNotifier:1.0
  14, ACE_NTOHL (0x4661756c), ACE_NTOHL (0x744e6f74), ACE_NTOHL (0x69666965), ACE_NTOHL (0x72000000),  // name = FaultNotifier
};
static CORBA::TypeCode _tc_TAO_tc_FT_FaultNotifier (CORBA::tk_objref, sizeof (_oc_FT_FaultNotifier), (char *) &_oc_FT_FaultNotifier, 0, sizeof (FT_FaultNotifier));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_FaultNotifier, &_tc_TAO_tc_FT_FaultNotifier)
TAO_NAMESPACE_END

#if !defined (_FT_STATE_CS_)
#define _FT_STATE_CS_

// *************************************************************
// FT_State
// *************************************************************

FT_State::FT_State (void)
{}
FT_State::FT_State (CORBA::ULong max) // uses max size
  :
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
  TAO_Unbounded_Sequence<CORBA::Octet>
#else /* TAO_USE_SEQUENCE_TEMPLATES */
  TAO_Unbounded_Sequence<CORBA::Octet>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
 (max)
{}
FT_State::FT_State (CORBA::ULong max, CORBA::ULong length, CORBA::Octet *buffer, CORBA::Boolean release)
  :
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
  TAO_Unbounded_Sequence<CORBA::Octet>
#else /* TAO_USE_SEQUENCE_TEMPLATES */
  TAO_Unbounded_Sequence<CORBA::Octet>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
 (max, length, buffer, release)
{}
FT_State::FT_State (const FT_State &seq) // copy ctor
  :
#if !defined (TAO_USE_SEQUENCE_TEMPLATES)
  TAO_Unbounded_Sequence<CORBA::Octet>
#else /* TAO_USE_SEQUENCE_TEMPLATES */
  TAO_Unbounded_Sequence<CORBA::Octet>
#endif /* !TAO_USE_SEQUENCE_TEMPLATES */
 (seq)
{}
FT_State::~FT_State (void) // dtor
{}
void FT_State::_tao_any_destructor (void *x)
{
  FT_State *tmp = ACE_static_cast (FT_State*,x);
  delete tmp;
}


#endif /* end #if !defined */

static const CORBA::Long _oc_FT_State[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  25, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f53), ACE_NTOHL (0x74617465), ACE_NTOHL (0x3a312e30), ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/State:1.0
  6, ACE_NTOHL (0x53746174), ACE_NTOHL (0x65000000),  // name = State
  CORBA::tk_sequence, // typecode kind
  12, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_octet,

    0U,

};
static CORBA::TypeCode _tc_TAO_tc_FT_State (CORBA::tk_alias, sizeof (_oc_FT_State), (char *) &_oc_FT_State, 0, sizeof (FT_State));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_State, &_tc_TAO_tc_FT_State)
TAO_NAMESPACE_END
// default constructor
FT_NoStateAvailable::FT_NoStateAvailable (void)
  : CORBA_UserException (::FT::_tc_NoStateAvailable)
{
}

// destructor - all members are of self managing types
FT_NoStateAvailable::~FT_NoStateAvailable (void)
{
}

void FT_NoStateAvailable::_tao_any_destructor (void *x)
{
  FT_NoStateAvailable *tmp = ACE_static_cast (FT_NoStateAvailable*,x);
  delete tmp;
}

// copy constructor
FT_NoStateAvailable::FT_NoStateAvailable (const ::FT_NoStateAvailable &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
  }

// assignment operator
FT_NoStateAvailable&
FT_NoStateAvailable::operator= (const ::FT_NoStateAvailable &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
FT_NoStateAvailable *
FT_NoStateAvailable::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/NoStateAvailable:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_NoStateAvailable *, exc);
  else
    return 0;
}


void FT_NoStateAvailable::_raise ()
{
  TAO_RAISE(*this);
}


void FT_NoStateAvailable::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_NoStateAvailable::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_NoStateAvailable::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_NoStateAvailable, 0);
  return retval;
}

static const CORBA::Long _oc_FT_NoStateAvailable[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  36, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4e), ACE_NTOHL (0x6f537461), ACE_NTOHL (0x74654176), ACE_NTOHL (0x61696c61), ACE_NTOHL (0x626c653a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/NoStateAvailable:1.0
  17, ACE_NTOHL (0x4e6f5374), ACE_NTOHL (0x61746541), ACE_NTOHL (0x7661696c), ACE_NTOHL (0x61626c65), ACE_NTOHL (0x0),  // name = NoStateAvailable
  0, // member count
};
static CORBA::TypeCode _tc_TAO_tc_FT_NoStateAvailable (CORBA::tk_except, sizeof (_oc_FT_NoStateAvailable), (char *) &_oc_FT_NoStateAvailable, 0, sizeof (FT_NoStateAvailable));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_NoStateAvailable, &_tc_TAO_tc_FT_NoStateAvailable)
TAO_NAMESPACE_END
// default constructor
FT_InvalidState::FT_InvalidState (void)
  : CORBA_UserException (::FT::_tc_InvalidState)
{
}

// destructor - all members are of self managing types
FT_InvalidState::~FT_InvalidState (void)
{
}

void FT_InvalidState::_tao_any_destructor (void *x)
{
  FT_InvalidState *tmp = ACE_static_cast (FT_InvalidState*,x);
  delete tmp;
}

// copy constructor
FT_InvalidState::FT_InvalidState (const ::FT_InvalidState &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
  }

// assignment operator
FT_InvalidState&
FT_InvalidState::operator= (const ::FT_InvalidState &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
FT_InvalidState *
FT_InvalidState::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/InvalidState:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_InvalidState *, exc);
  else
    return 0;
}


void FT_InvalidState::_raise ()
{
  TAO_RAISE(*this);
}


void FT_InvalidState::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_InvalidState::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_InvalidState::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_InvalidState, 0);
  return retval;
}

static const CORBA::Long _oc_FT_InvalidState[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  32, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f49), ACE_NTOHL (0x6e76616c), ACE_NTOHL (0x69645374), ACE_NTOHL (0x6174653a), ACE_NTOHL (0x312e3000),  // repository ID = IDL:omg.org/FT/InvalidState:1.0
  13, ACE_NTOHL (0x496e7661), ACE_NTOHL (0x6c696453), ACE_NTOHL (0x74617465), ACE_NTOHL (0x0),  // name = InvalidState
  0, // member count
};
static CORBA::TypeCode _tc_TAO_tc_FT_InvalidState (CORBA::tk_except, sizeof (_oc_FT_InvalidState), (char *) &_oc_FT_InvalidState, 0, sizeof (FT_InvalidState));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_InvalidState, &_tc_TAO_tc_FT_InvalidState)
TAO_NAMESPACE_END
// default constructor
FT_NoUpdateAvailable::FT_NoUpdateAvailable (void)
  : CORBA_UserException (::FT::_tc_NoUpdateAvailable)
{
}

// destructor - all members are of self managing types
FT_NoUpdateAvailable::~FT_NoUpdateAvailable (void)
{
}

void FT_NoUpdateAvailable::_tao_any_destructor (void *x)
{
  FT_NoUpdateAvailable *tmp = ACE_static_cast (FT_NoUpdateAvailable*,x);
  delete tmp;
}

// copy constructor
FT_NoUpdateAvailable::FT_NoUpdateAvailable (const ::FT_NoUpdateAvailable &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
  }

// assignment operator
FT_NoUpdateAvailable&
FT_NoUpdateAvailable::operator= (const ::FT_NoUpdateAvailable &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
FT_NoUpdateAvailable *
FT_NoUpdateAvailable::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/NoUpdateAvailable:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_NoUpdateAvailable *, exc);
  else
    return 0;
}


void FT_NoUpdateAvailable::_raise ()
{
  TAO_RAISE(*this);
}


void FT_NoUpdateAvailable::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_NoUpdateAvailable::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_NoUpdateAvailable::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_NoUpdateAvailable, 0);
  return retval;
}

static const CORBA::Long _oc_FT_NoUpdateAvailable[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  37, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f4e), ACE_NTOHL (0x6f557064), ACE_NTOHL (0x61746541), ACE_NTOHL (0x7661696c), ACE_NTOHL (0x61626c65), ACE_NTOHL (0x3a312e30), ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/NoUpdateAvailable:1.0
  18, ACE_NTOHL (0x4e6f5570), ACE_NTOHL (0x64617465), ACE_NTOHL (0x41766169), ACE_NTOHL (0x6c61626c), ACE_NTOHL (0x65000000),  // name = NoUpdateAvailable
  0, // member count
};
static CORBA::TypeCode _tc_TAO_tc_FT_NoUpdateAvailable (CORBA::tk_except, sizeof (_oc_FT_NoUpdateAvailable), (char *) &_oc_FT_NoUpdateAvailable, 0, sizeof (FT_NoUpdateAvailable));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_NoUpdateAvailable, &_tc_TAO_tc_FT_NoUpdateAvailable)
TAO_NAMESPACE_END
// default constructor
FT_InvalidUpdate::FT_InvalidUpdate (void)
  : CORBA_UserException (::FT::_tc_InvalidUpdate)
{
}

// destructor - all members are of self managing types
FT_InvalidUpdate::~FT_InvalidUpdate (void)
{
}

void FT_InvalidUpdate::_tao_any_destructor (void *x)
{
  FT_InvalidUpdate *tmp = ACE_static_cast (FT_InvalidUpdate*,x);
  delete tmp;
}

// copy constructor
FT_InvalidUpdate::FT_InvalidUpdate (const ::FT_InvalidUpdate &_tao_excp)
  : CORBA_UserException (_tao_excp._type ())
{
  }

// assignment operator
FT_InvalidUpdate&
FT_InvalidUpdate::operator= (const ::FT_InvalidUpdate &_tao_excp)
{

  this->CORBA_UserException::operator= (_tao_excp);
  return *this;
}

// narrow
FT_InvalidUpdate *
FT_InvalidUpdate::_downcast (CORBA::Exception *exc)
{
  if (!ACE_OS::strcmp ("IDL:omg.org/FT/InvalidUpdate:1.0", exc->_id ())) // same type
    return ACE_dynamic_cast (FT_InvalidUpdate *, exc);
  else
    return 0;
}


void FT_InvalidUpdate::_raise ()
{
  TAO_RAISE(*this);
}


void FT_InvalidUpdate::_tao_encode (
    TAO_OutputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV) const
{
  if (cdr << *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}


void FT_InvalidUpdate::_tao_decode (
    TAO_InputCDR &cdr,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (cdr >> *this)
    return;
  ACE_THROW (CORBA::MARSHAL ());
}

// TAO extension - the _alloc method
CORBA::Exception *FT_InvalidUpdate::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::FT_InvalidUpdate, 0);
  return retval;
}

static const CORBA::Long _oc_FT_InvalidUpdate[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  33, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f49), ACE_NTOHL (0x6e76616c), ACE_NTOHL (0x69645570), ACE_NTOHL (0x64617465), ACE_NTOHL (0x3a312e30), ACE_NTOHL (0x0),  // repository ID = IDL:omg.org/FT/InvalidUpdate:1.0
  14, ACE_NTOHL (0x496e7661), ACE_NTOHL (0x6c696455), ACE_NTOHL (0x70646174), ACE_NTOHL (0x65000000),  // name = InvalidUpdate
  0, // member count
};
static CORBA::TypeCode _tc_TAO_tc_FT_InvalidUpdate (CORBA::tk_except, sizeof (_oc_FT_InvalidUpdate), (char *) &_oc_FT_InvalidUpdate, 0, sizeof (FT_InvalidUpdate));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_InvalidUpdate, &_tc_TAO_tc_FT_InvalidUpdate)
TAO_NAMESPACE_END

// default constructor
FT_Checkpointable::FT_Checkpointable (void)
{}

// destructor
FT_Checkpointable::~FT_Checkpointable (void)
{}

void FT_Checkpointable::_tao_any_destructor (void *x)
{
  FT_Checkpointable *tmp = ACE_static_cast (FT_Checkpointable*,x);
  CORBA::release (tmp);
}

FT_Checkpointable_ptr FT_Checkpointable::_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (CORBA::is_nil (obj))
    return FT_Checkpointable::_nil ();
  if (! obj->_is_local ())
    {
      CORBA::Boolean is_a = obj->_is_a ("IDL:omg.org/FT/Checkpointable:1.0", ACE_TRY_ENV);
      ACE_CHECK_RETURN (FT_Checkpointable::_nil ());
      if (is_a == 0)
        return FT_Checkpointable::_nil ();
    }
  return FT_Checkpointable::_unchecked_narrow (obj, ACE_TRY_ENV);
}

FT_Checkpointable_ptr FT_Checkpointable::_unchecked_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &
  )
{
  if (CORBA::is_nil (obj))
    return FT_Checkpointable::_nil ();
  if (! obj->_is_local ())
    {
      TAO_Stub* stub = obj->_stubobj ();
      if (stub)
        stub->_incr_refcnt ();
      FT_Checkpointable_ptr default_proxy = FT_Checkpointable::_nil ();

      if (obj->_is_collocated () && _TAO_collocation_FT_Checkpointable_Stub_Factory_function_pointer != 0)
        {
          default_proxy = _TAO_collocation_FT_Checkpointable_Stub_Factory_function_pointer (obj);
        }
      if (CORBA::is_nil (default_proxy))
        ACE_NEW_RETURN (default_proxy, ::FT_Checkpointable (stub), FT_Checkpointable::_nil ());
      #if (TAO_HAS_SMART_PROXIES == 1)
        return TAO_FT_Checkpointable_PROXY_FACTORY_ADAPTER::instance ()->create_proxy (default_proxy);
      #else
        return default_proxy;
      #endif /*TAO_HAS_SMART_PROXIES == 1*/
    }
  else
    return
      ACE_reinterpret_cast
        (
          FT_Checkpointable_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast
                  (
                    ptr_arith_t,
                    &FT_Checkpointable::_narrow
                  )
              )
        );
}

FT_Checkpointable_ptr
FT_Checkpointable::_duplicate (FT_Checkpointable_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

FT_State * FT_Checkpointable::get_state (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_NoStateAvailable
  ))
{

  static TAO_Exception_Data _tao_FT_Checkpointable_get_state_exceptiondata [] =
  {
    {FT::_tc_NoStateAvailable, FT_NoStateAvailable::_alloc}
  };

  FT_State *_tao_retval = 0;


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  ACE_NEW_RETURN (_tao_retval, FT_State, _tao_retval);
  FT_State_var _tao_safe_retval (_tao_retval);
  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "get_state",
      9,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_state",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        int _invoke_status =
          _tao_call.invoke (_tao_FT_Checkpointable_get_state_exceptiondata, 1, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_state",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "get_state",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

void FT_Checkpointable::set_state (
    const FT_State & s,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_InvalidState
  ))
{

  static TAO_Exception_Data _tao_FT_Checkpointable_set_state_exceptiondata [] =
  {
    {FT::_tc_InvalidState, FT_InvalidState::_alloc}
  };




  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW (CORBA::INTERNAL ());


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "set_state",
      9,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK;
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK;
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "set_state",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << s)
          ))
          TAO_INTERCEPTOR_THROW (CORBA::MARSHAL ());
        int _invoke_status =
          _tao_call.invoke (_tao_FT_Checkpointable_set_state_exceptiondata, 1, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK;

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
        }

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "set_state",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "set_state",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

}

CORBA::Boolean FT_Checkpointable::_is_a (const CORBA::Char *value, CORBA::Environment &ACE_TRY_ENV)
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/Checkpointable:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/CORBA/Object:1.0")))
  return 1; // success using local knowledge
  else
    return this->CORBA_Object::_is_a (value, ACE_TRY_ENV);
}

void *FT_Checkpointable::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &FT_Checkpointable::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char* FT_Checkpointable::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/Checkpointable:1.0";
}

static const CORBA::Long _oc_FT_Checkpointable[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f43), ACE_NTOHL (0x6865636b), ACE_NTOHL (0x706f696e), ACE_NTOHL (0x7461626c), ACE_NTOHL (0x653a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/Checkpointable:1.0
  15, ACE_NTOHL (0x43686563), ACE_NTOHL (0x6b706f69), ACE_NTOHL (0x6e746162), ACE_NTOHL (0x6c650000),  // name = Checkpointable
};
static CORBA::TypeCode _tc_TAO_tc_FT_Checkpointable (CORBA::tk_objref, sizeof (_oc_FT_Checkpointable), (char *) &_oc_FT_Checkpointable, 0, sizeof (FT_Checkpointable));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_Checkpointable, &_tc_TAO_tc_FT_Checkpointable)
TAO_NAMESPACE_END

// default constructor
FT_Updateable::FT_Updateable (void)
{}

// destructor
FT_Updateable::~FT_Updateable (void)
{}

void FT_Updateable::_tao_any_destructor (void *x)
{
  FT_Updateable *tmp = ACE_static_cast (FT_Updateable*,x);
  CORBA::release (tmp);
}

FT_Updateable_ptr FT_Updateable::_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (CORBA::is_nil (obj))
    return FT_Updateable::_nil ();
  if (! obj->_is_local ())
    {
      CORBA::Boolean is_a = obj->_is_a ("IDL:omg.org/FT/Updateable:1.0", ACE_TRY_ENV);
      ACE_CHECK_RETURN (FT_Updateable::_nil ());
      if (is_a == 0)
        return FT_Updateable::_nil ();
    }
  return FT_Updateable::_unchecked_narrow (obj, ACE_TRY_ENV);
}

FT_Updateable_ptr FT_Updateable::_unchecked_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &
  )
{
  if (CORBA::is_nil (obj))
    return FT_Updateable::_nil ();
  if (! obj->_is_local ())
    {
      TAO_Stub* stub = obj->_stubobj ();
      if (stub)
        stub->_incr_refcnt ();
      FT_Updateable_ptr default_proxy = FT_Updateable::_nil ();

      if (obj->_is_collocated () && _TAO_collocation_FT_Updateable_Stub_Factory_function_pointer != 0)
        {
          default_proxy = _TAO_collocation_FT_Updateable_Stub_Factory_function_pointer (obj);
        }
      if (CORBA::is_nil (default_proxy))
        ACE_NEW_RETURN (default_proxy, ::FT_Updateable (stub), FT_Updateable::_nil ());
#if (TAO_HAS_SMART_PROXIES == 1)
      return TAO_FT_Updateable_PROXY_FACTORY_ADAPTER::instance ()->create_proxy (default_proxy);
#else
      return default_proxy;
#endif /*TAO_HAS_SMART_PROXIES == 1*/
    }
  else
    return
      ACE_reinterpret_cast
        (
          FT_Updateable_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast
                  (
                    ptr_arith_t,
                    &FT_Updateable::_narrow
                  )
              )
        );
}

FT_Updateable_ptr
FT_Updateable::_duplicate (FT_Updateable_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

FT_State * FT_Updateable::get_update (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_NoUpdateAvailable
  ))
{

  static TAO_Exception_Data _tao_FT_Updateable_get_update_exceptiondata [] =
  {
    {FT::_tc_NoUpdateAvailable, FT_NoUpdateAvailable::_alloc}
  };

  FT_State *_tao_retval = 0;


  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);


  ACE_NEW_RETURN (_tao_retval, FT_State, _tao_retval);
  FT_State_var _tao_safe_retval (_tao_retval);
  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "get_update",
      10,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK_RETURN (0);
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK_RETURN (0);
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_update",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK_RETURN (0);

        int _invoke_status =
          _tao_call.invoke (_tao_FT_Updateable_get_update_exceptiondata, 1, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK_RETURN (0);

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        }
        TAO_InputCDR &_tao_in = _tao_call.inp_stream ();
        if (!(
              (_tao_in >> _tao_safe_retval.inout ())
          ))
          TAO_INTERCEPTOR_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES), 0);

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "get_update",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK_RETURN (0);
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "get_update",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);
#endif /* TAO_HAS_INTERCEPTORS */
  return _tao_safe_retval._retn ();
}

void FT_Updateable::set_update (
    const FT_State & s,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    FT_InvalidUpdate
  ))
{

  static TAO_Exception_Data _tao_FT_Updateable_set_update_exceptiondata [] =
  {
    {FT::_tc_InvalidUpdate, FT_InvalidUpdate::_alloc}
  };




  TAO_Stub *istub = this->_stubobj ();
  if (istub == 0)
    ACE_THROW (CORBA::INTERNAL ());


  TAO_GIOP_Twoway_Invocation _tao_call (
      istub,
      "set_update",
      10,
      istub->orb_core ()
    );

#if (TAO_HAS_INTERCEPTORS == 1)
    TAO_ClientRequestInterceptor_Adapter
      _tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));
    ACE_CHECK;
    PortableInterceptor::Cookies _tao_cookies;
    CORBA::NVList_var _tao_interceptor_args;
    if (_tao_vfr.valid ())
      {
        istub->orb_core ()->orb ()->create_list (0, _tao_interceptor_args.inout (), ACE_TRY_ENV);
        ACE_CHECK;
      }

    ACE_TRY
      {
#endif /* TAO_HAS_INTERCEPTORS */

      for (;;)
      {
        _tao_call.start (ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_INTERCEPTOR (
            _tao_vfr.preinvoke (
                _tao_call.request_id (),
                1,
                this,
                "set_update",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

        _tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);
        TAO_INTERCEPTOR_CHECK;

        TAO_OutputCDR &_tao_out = _tao_call.out_stream ();
        if (!(
              (_tao_out << s)
          ))
          TAO_INTERCEPTOR_THROW (CORBA::MARSHAL ());
        int _invoke_status =
          _tao_call.invoke (_tao_FT_Updateable_set_update_exceptiondata, 1, ACE_TRY_ENV);
                TAO_INTERCEPTOR_CHECK;

        if (_invoke_status == TAO_INVOKE_RESTART)
          continue;
        if (_invoke_status != TAO_INVOKE_OK)
        {
          TAO_INTERCEPTOR_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
        }

        TAO_INTERCEPTOR (
            _tao_vfr.postinvoke (
                _tao_call.request_id (),
                1,
                this,
                "set_update",
                _tao_call.service_info (),
                _tao_interceptor_args.inout (),
                _tao_cookies,
                ACE_TRY_ENV
              )
          );
        TAO_INTERCEPTOR_CHECK;
        break;

      }
#if (TAO_HAS_INTERCEPTORS == 1)

    }
  ACE_CATCHANY
    {
      _tao_vfr.exception_occurred (
          _tao_call.request_id (),
          1,
          this,
          "set_update",
          _tao_cookies,
          ACE_TRY_ENV
        );
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
#endif /* TAO_HAS_INTERCEPTORS */

}

CORBA::Boolean FT_Updateable::_is_a (const CORBA::Char *value, CORBA::Environment &ACE_TRY_ENV)
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/Updateable:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/FT/Checkpointable:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/CORBA/Object:1.0")))
  return 1; // success using local knowledge
  else
    return this->CORBA_Object::_is_a (value, ACE_TRY_ENV);
}

void *FT_Updateable::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &FT_Updateable::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &FT_Checkpointable::_narrow))
    retv = ACE_reinterpret_cast
      (
        void *,
        ACE_static_cast
          (
            FT_Checkpointable_ptr,
            this
          )
      );
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char* FT_Updateable::_interface_repository_id (void) const
{
  return "IDL:omg.org/FT/Updateable:1.0";
}

static const CORBA::Long _oc_FT_Updateable[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  30, ACE_NTOHL (0x49444c3a), ACE_NTOHL (0x6f6d672e), ACE_NTOHL (0x6f72672f), ACE_NTOHL (0x46542f55), ACE_NTOHL (0x70646174), ACE_NTOHL (0x6561626c), ACE_NTOHL (0x653a312e), ACE_NTOHL (0x30000000),  // repository ID = IDL:omg.org/FT/Updateable:1.0
  11, ACE_NTOHL (0x55706461), ACE_NTOHL (0x74656162), ACE_NTOHL (0x6c650000),  // name = Updateable
};
static CORBA::TypeCode _tc_TAO_tc_FT_Updateable (CORBA::tk_objref, sizeof (_oc_FT_Updateable), (char *) &_oc_FT_Updateable, 0, sizeof (FT_Updateable));
TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (FT)
TAO_NAMESPACE_DEFINE (CORBA::TypeCode_ptr, _tc_Updateable, &_tc_TAO_tc_FT_Updateable)
TAO_NAMESPACE_END



void operator<<= (CORBA::Any &_tao_any, const FT_Property &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_Property,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_Property *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_Property,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_Property::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_Property *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(const FT_Property*&,_tao_elem);
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_Property *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_Property, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = ACE_static_cast(
          const FT_Property*,
          _tao_any.value ()
        );
      return 1;
    }
    else
    {
      FT_Property *tmp;
      ACE_NEW_RETURN (tmp, FT_Property, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_Property,
            1,
            ACE_static_cast (void *, tmp),
            FT_Property::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (
    CORBA::Any &_tao_any,
    const FT_Properties &_tao_elem
  ) // copying
{
  TAO_OutputCDR stream;
  if (stream << _tao_elem)
  {
    _tao_any._tao_replace (
        FT::_tc_Properties,
        TAO_ENCAP_BYTE_ORDER,
        stream.begin ()
      );
  }
}

void operator<<= (CORBA::Any &_tao_any, FT_Properties *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_Properties,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_Properties::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_Properties *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_Properties*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_Properties *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_Properties, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = ACE_static_cast(
          const FT_Properties*,
          _tao_any.value ()
        );
      return 1;
    }
    else
    {
      FT_Properties *tmp;
      ACE_NEW_RETURN (tmp, FT_Properties, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_Properties,
            1,
            ACE_static_cast (void *, tmp),
            FT_Properties::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (
    CORBA::Any &_tao_any,
    const FT_Locations &_tao_elem
  ) // copying
{
  TAO_OutputCDR stream;
  if (stream << _tao_elem)
  {
    _tao_any._tao_replace (
        FT::_tc_Locations,
        TAO_ENCAP_BYTE_ORDER,
        stream.begin ()
      );
  }
}

void operator<<= (CORBA::Any &_tao_any, FT_Locations *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_Locations,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_Locations::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_Locations *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_Locations*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_Locations *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_Locations, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = ACE_static_cast(
          const FT_Locations*,
          _tao_any.value ()
        );
      return 1;
    }
    else
    {
      FT_Locations *tmp;
      ACE_NEW_RETURN (tmp, FT_Locations, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_Locations,
            1,
            ACE_static_cast (void *, tmp),
            FT_Locations::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_FactoryInfo &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_FactoryInfo,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_FactoryInfo *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_FactoryInfo,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_FactoryInfo::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_FactoryInfo *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(const FT_FactoryInfo*&,_tao_elem);
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_FactoryInfo *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_FactoryInfo, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = ACE_static_cast(
          const FT_FactoryInfo*,
          _tao_any.value ()
        );
      return 1;
    }
    else
    {
      FT_FactoryInfo *tmp;
      ACE_NEW_RETURN (tmp, FT_FactoryInfo, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_FactoryInfo,
            1,
            ACE_static_cast (void *, tmp),
            FT_FactoryInfo::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (
    CORBA::Any &_tao_any,
    const FT_FactoryInfos &_tao_elem
  ) // copying
{
  TAO_OutputCDR stream;
  if (stream << _tao_elem)
  {
    _tao_any._tao_replace (
        FT::_tc_FactoryInfos,
        TAO_ENCAP_BYTE_ORDER,
        stream.begin ()
      );
  }
}

void operator<<= (CORBA::Any &_tao_any, FT_FactoryInfos *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_FactoryInfos,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_FactoryInfos::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_FactoryInfos *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_FactoryInfos*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_FactoryInfos *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_FactoryInfos, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = ACE_static_cast(
          const FT_FactoryInfos*,
          _tao_any.value ()
        );
      return 1;
    }
    else
    {
      FT_FactoryInfos *tmp;
      ACE_NEW_RETURN (tmp, FT_FactoryInfos, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_FactoryInfos,
            1,
            ACE_static_cast (void *, tmp),
            FT_FactoryInfos::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_FaultMonitoringIntervalAndTimeoutValue &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_FaultMonitoringIntervalAndTimeoutValue,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_FaultMonitoringIntervalAndTimeoutValue *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_FaultMonitoringIntervalAndTimeoutValue,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_FaultMonitoringIntervalAndTimeoutValue::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_FaultMonitoringIntervalAndTimeoutValue *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(const FT_FaultMonitoringIntervalAndTimeoutValue*&,_tao_elem);
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_FaultMonitoringIntervalAndTimeoutValue *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_FaultMonitoringIntervalAndTimeoutValue, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = ACE_static_cast(
          const FT_FaultMonitoringIntervalAndTimeoutValue*,
          _tao_any.value ()
        );
      return 1;
    }
    else
    {
      FT_FaultMonitoringIntervalAndTimeoutValue *tmp;
      ACE_NEW_RETURN (tmp, FT_FaultMonitoringIntervalAndTimeoutValue, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_FaultMonitoringIntervalAndTimeoutValue,
            1,
            ACE_static_cast (void *, tmp),
            FT_FaultMonitoringIntervalAndTimeoutValue::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_InterfaceNotFound &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_InterfaceNotFound,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_InterfaceNotFound *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_InterfaceNotFound,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_InterfaceNotFound::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_InterfaceNotFound *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_InterfaceNotFound*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_InterfaceNotFound *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_InterfaceNotFound, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_InterfaceNotFound *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_InterfaceNotFound *tmp;
      ACE_NEW_RETURN (tmp, FT_InterfaceNotFound, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/InterfaceNotFound:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_InterfaceNotFound,
            1,
            tmp,
            FT_InterfaceNotFound::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_ObjectGroupNotFound &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_ObjectGroupNotFound,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_ObjectGroupNotFound *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_ObjectGroupNotFound,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_ObjectGroupNotFound::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_ObjectGroupNotFound *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_ObjectGroupNotFound*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_ObjectGroupNotFound *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_ObjectGroupNotFound, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_ObjectGroupNotFound *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_ObjectGroupNotFound *tmp;
      ACE_NEW_RETURN (tmp, FT_ObjectGroupNotFound, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/ObjectGroupNotFound:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_ObjectGroupNotFound,
            1,
            tmp,
            FT_ObjectGroupNotFound::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_MemberNotFound &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_MemberNotFound,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_MemberNotFound *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_MemberNotFound,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_MemberNotFound::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_MemberNotFound *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_MemberNotFound*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_MemberNotFound *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_MemberNotFound, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_MemberNotFound *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_MemberNotFound *tmp;
      ACE_NEW_RETURN (tmp, FT_MemberNotFound, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/MemberNotFound:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_MemberNotFound,
            1,
            tmp,
            FT_MemberNotFound::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_ObjectNotFound &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_ObjectNotFound,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_ObjectNotFound *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_ObjectNotFound,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_ObjectNotFound::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_ObjectNotFound *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_ObjectNotFound*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_ObjectNotFound *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_ObjectNotFound, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_ObjectNotFound *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_ObjectNotFound *tmp;
      ACE_NEW_RETURN (tmp, FT_ObjectNotFound, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/ObjectNotFound:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_ObjectNotFound,
            1,
            tmp,
            FT_ObjectNotFound::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_MemberAlreadyPresent &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_MemberAlreadyPresent,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_MemberAlreadyPresent *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_MemberAlreadyPresent,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_MemberAlreadyPresent::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_MemberAlreadyPresent *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_MemberAlreadyPresent*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_MemberAlreadyPresent *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_MemberAlreadyPresent, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_MemberAlreadyPresent *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_MemberAlreadyPresent *tmp;
      ACE_NEW_RETURN (tmp, FT_MemberAlreadyPresent, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/MemberAlreadyPresent:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_MemberAlreadyPresent,
            1,
            tmp,
            FT_MemberAlreadyPresent::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_BadReplicationStyle &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_BadReplicationStyle,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_BadReplicationStyle *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_BadReplicationStyle,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_BadReplicationStyle::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_BadReplicationStyle *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_BadReplicationStyle*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_BadReplicationStyle *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_BadReplicationStyle, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_BadReplicationStyle *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_BadReplicationStyle *tmp;
      ACE_NEW_RETURN (tmp, FT_BadReplicationStyle, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/BadReplicationStyle:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_BadReplicationStyle,
            1,
            tmp,
            FT_BadReplicationStyle::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_ObjectNotCreated &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_ObjectNotCreated,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_ObjectNotCreated *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_ObjectNotCreated,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_ObjectNotCreated::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_ObjectNotCreated *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_ObjectNotCreated*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_ObjectNotCreated *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_ObjectNotCreated, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_ObjectNotCreated *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_ObjectNotCreated *tmp;
      ACE_NEW_RETURN (tmp, FT_ObjectNotCreated, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/ObjectNotCreated:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_ObjectNotCreated,
            1,
            tmp,
            FT_ObjectNotCreated::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_ObjectNotAdded &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_ObjectNotAdded,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_ObjectNotAdded *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_ObjectNotAdded,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_ObjectNotAdded::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_ObjectNotAdded *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_ObjectNotAdded*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_ObjectNotAdded *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_ObjectNotAdded, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_ObjectNotAdded *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_ObjectNotAdded *tmp;
      ACE_NEW_RETURN (tmp, FT_ObjectNotAdded, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/ObjectNotAdded:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_ObjectNotAdded,
            1,
            tmp,
            FT_ObjectNotAdded::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_PrimaryNotSet &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_PrimaryNotSet,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_PrimaryNotSet *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_PrimaryNotSet,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_PrimaryNotSet::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_PrimaryNotSet *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_PrimaryNotSet*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_PrimaryNotSet *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_PrimaryNotSet, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_PrimaryNotSet *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_PrimaryNotSet *tmp;
      ACE_NEW_RETURN (tmp, FT_PrimaryNotSet, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/PrimaryNotSet:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_PrimaryNotSet,
            1,
            tmp,
            FT_PrimaryNotSet::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_UnsupportedProperty &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_UnsupportedProperty,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_UnsupportedProperty *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_UnsupportedProperty,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_UnsupportedProperty::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_UnsupportedProperty *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_UnsupportedProperty*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_UnsupportedProperty *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_UnsupportedProperty, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_UnsupportedProperty *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_UnsupportedProperty *tmp;
      ACE_NEW_RETURN (tmp, FT_UnsupportedProperty, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/UnsupportedProperty:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_UnsupportedProperty,
            1,
            tmp,
            FT_UnsupportedProperty::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_InvalidProperty &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_InvalidProperty,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_InvalidProperty *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_InvalidProperty,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_InvalidProperty::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_InvalidProperty *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_InvalidProperty*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_InvalidProperty *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_InvalidProperty, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_InvalidProperty *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_InvalidProperty *tmp;
      ACE_NEW_RETURN (tmp, FT_InvalidProperty, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/InvalidProperty:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_InvalidProperty,
            1,
            tmp,
            FT_InvalidProperty::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_NoFactory &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_NoFactory,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_NoFactory *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_NoFactory,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_NoFactory::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_NoFactory *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_NoFactory*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_NoFactory *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_NoFactory, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_NoFactory *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_NoFactory *tmp;
      ACE_NEW_RETURN (tmp, FT_NoFactory, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/NoFactory:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_NoFactory,
            1,
            tmp,
            FT_NoFactory::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_InvalidCriteria &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_InvalidCriteria,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_InvalidCriteria *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_InvalidCriteria,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_InvalidCriteria::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_InvalidCriteria *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_InvalidCriteria*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_InvalidCriteria *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_InvalidCriteria, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_InvalidCriteria *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_InvalidCriteria *tmp;
      ACE_NEW_RETURN (tmp, FT_InvalidCriteria, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/InvalidCriteria:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_InvalidCriteria,
            1,
            tmp,
            FT_InvalidCriteria::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_CannotMeetCriteria &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_CannotMeetCriteria,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_CannotMeetCriteria *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_CannotMeetCriteria,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_CannotMeetCriteria::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_CannotMeetCriteria *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_CannotMeetCriteria*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_CannotMeetCriteria *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_CannotMeetCriteria, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_CannotMeetCriteria *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_CannotMeetCriteria *tmp;
      ACE_NEW_RETURN (tmp, FT_CannotMeetCriteria, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/CannotMeetCriteria:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_CannotMeetCriteria,
            1,
            tmp,
            FT_CannotMeetCriteria::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

FT_PropertyManager_ptr (*_TAO_collocation_FT_PropertyManager_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  ) = 0;
void operator<<= (CORBA::Any &_tao_any, FT_PropertyManager_ptr _tao_elem)
{
  TAO_OutputCDR stream;
  if (stream << _tao_elem)
  {
    _tao_any._tao_replace (
        FT::_tc_PropertyManager,
        TAO_ENCAP_BYTE_ORDER,
        stream.begin (),
        1,
        FT_PropertyManager::_duplicate (_tao_elem),
        FT_PropertyManager::_tao_any_destructor
      );
  }
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_PropertyManager_ptr &_tao_elem)
{
  ACE_TRY_NEW_ENV
  {
    _tao_elem = FT_PropertyManager::_nil ();
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_PropertyManager, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    TAO_InputCDR stream (
        _tao_any._tao_get_cdr (),
        _tao_any._tao_byte_order ()
      );
    if (stream >> _tao_elem)
    {
      ((CORBA::Any *)&_tao_any)->_tao_replace (
          FT::_tc_PropertyManager,
          1,
          _tao_elem,
          FT_PropertyManager::_tao_any_destructor
        );
      return 1;
    }
  }
  ACE_CATCHANY
  {
    _tao_elem = FT_PropertyManager::_nil ();
    return 0;
  }
  ACE_ENDTRY;
  _tao_elem = FT_PropertyManager::_nil ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)  || defined (ACE_HAS_GNU_REPO)
template class TAO_Object_Manager<FT_PropertyManager,FT_PropertyManager_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#  pragma instantiate TAO_Object_Manager<FT_PropertyManager,FT_PropertyManager_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

FT_ObjectGroupManager_ptr (*_TAO_collocation_FT_ObjectGroupManager_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  ) = 0;
void operator<<= (CORBA::Any &_tao_any, FT_ObjectGroupManager_ptr _tao_elem)
{
  TAO_OutputCDR stream;
  if (stream << _tao_elem)
  {
    _tao_any._tao_replace (
        FT::_tc_ObjectGroupManager,
        TAO_ENCAP_BYTE_ORDER,
        stream.begin (),
        1,
        FT_ObjectGroupManager::_duplicate (_tao_elem),
        FT_ObjectGroupManager::_tao_any_destructor
      );
  }
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_ObjectGroupManager_ptr &_tao_elem)
{
  ACE_TRY_NEW_ENV
  {
    _tao_elem = FT_ObjectGroupManager::_nil ();
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_ObjectGroupManager, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    TAO_InputCDR stream (
        _tao_any._tao_get_cdr (),
        _tao_any._tao_byte_order ()
      );
    if (stream >> _tao_elem)
    {
      ((CORBA::Any *)&_tao_any)->_tao_replace (
          FT::_tc_ObjectGroupManager,
          1,
          _tao_elem,
          FT_ObjectGroupManager::_tao_any_destructor
        );
      return 1;
    }
  }
  ACE_CATCHANY
  {
    _tao_elem = FT_ObjectGroupManager::_nil ();
    return 0;
  }
  ACE_ENDTRY;
  _tao_elem = FT_ObjectGroupManager::_nil ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)  || defined (ACE_HAS_GNU_REPO)
  template class TAO_Object_Manager<FT_ObjectGroupManager,FT_ObjectGroupManager_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#  pragma instantiate TAO_Object_Manager<FT_ObjectGroupManager,FT_ObjectGroupManager_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

FT_GenericFactory_ptr (*_TAO_collocation_FT_GenericFactory_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  ) = 0;
void operator<<= (CORBA::Any &_tao_any, FT_GenericFactory_ptr _tao_elem)
{
  TAO_OutputCDR stream;
  if (stream << _tao_elem)
  {
    _tao_any._tao_replace (
        FT::_tc_GenericFactory,
        TAO_ENCAP_BYTE_ORDER,
        stream.begin (),
        1,
        FT_GenericFactory::_duplicate (_tao_elem),
        FT_GenericFactory::_tao_any_destructor
      );
  }
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_GenericFactory_ptr &_tao_elem)
{
  ACE_TRY_NEW_ENV
  {
    _tao_elem = FT_GenericFactory::_nil ();
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_GenericFactory, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    TAO_InputCDR stream (
        _tao_any._tao_get_cdr (),
        _tao_any._tao_byte_order ()
      );
    if (stream >> _tao_elem)
    {
      ((CORBA::Any *)&_tao_any)->_tao_replace (
          FT::_tc_GenericFactory,
          1,
          _tao_elem,
          FT_GenericFactory::_tao_any_destructor
        );
      return 1;
    }
  }
  ACE_CATCHANY
  {
    _tao_elem = FT_GenericFactory::_nil ();
    return 0;
  }
  ACE_ENDTRY;
  _tao_elem = FT_GenericFactory::_nil ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)  || defined (ACE_HAS_GNU_REPO)
template class TAO_Object_Manager<FT_GenericFactory,FT_GenericFactory_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#  pragma instantiate TAO_Object_Manager<FT_GenericFactory,FT_GenericFactory_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

FT_ReplicationManager_ptr (*_TAO_collocation_FT_ReplicationManager_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  ) = 0;
void operator<<= (CORBA::Any &_tao_any, FT_ReplicationManager_ptr _tao_elem)
{
  TAO_OutputCDR stream;
  if (stream << _tao_elem)
  {
    _tao_any._tao_replace (
        FT::_tc_ReplicationManager,
        TAO_ENCAP_BYTE_ORDER,
        stream.begin (),
        1,
        FT_ReplicationManager::_duplicate (_tao_elem),
        FT_ReplicationManager::_tao_any_destructor
      );
  }
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_ReplicationManager_ptr &_tao_elem)
{
  ACE_TRY_NEW_ENV
  {
    _tao_elem = FT_ReplicationManager::_nil ();
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_ReplicationManager, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    TAO_InputCDR stream (
        _tao_any._tao_get_cdr (),
        _tao_any._tao_byte_order ()
      );
    if (stream >> _tao_elem)
    {
      ((CORBA::Any *)&_tao_any)->_tao_replace (
          FT::_tc_ReplicationManager,
          1,
          _tao_elem,
          FT_ReplicationManager::_tao_any_destructor
        );
      return 1;
    }
  }
  ACE_CATCHANY
  {
    _tao_elem = FT_ReplicationManager::_nil ();
    return 0;
  }
  ACE_ENDTRY;
  _tao_elem = FT_ReplicationManager::_nil ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)  || defined (ACE_HAS_GNU_REPO)
template class TAO_Object_Manager<FT_ReplicationManager,FT_ReplicationManager_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#  pragma instantiate TAO_Object_Manager<FT_ReplicationManager,FT_ReplicationManager_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

FT_PullMonitorable_ptr (*_TAO_collocation_FT_PullMonitorable_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  ) = 0;
void operator<<= (CORBA::Any &_tao_any, FT_PullMonitorable_ptr _tao_elem)
{
  TAO_OutputCDR stream;
  if (stream << _tao_elem)
  {
    _tao_any._tao_replace (
        FT::_tc_PullMonitorable,
        TAO_ENCAP_BYTE_ORDER,
        stream.begin (),
        1,
        FT_PullMonitorable::_duplicate (_tao_elem),
        FT_PullMonitorable::_tao_any_destructor
      );
  }
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_PullMonitorable_ptr &_tao_elem)
{
  ACE_TRY_NEW_ENV
  {
    _tao_elem = FT_PullMonitorable::_nil ();
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_PullMonitorable, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    TAO_InputCDR stream (
        _tao_any._tao_get_cdr (),
        _tao_any._tao_byte_order ()
      );
    if (stream >> _tao_elem)
    {
      ((CORBA::Any *)&_tao_any)->_tao_replace (
          FT::_tc_PullMonitorable,
          1,
          _tao_elem,
          FT_PullMonitorable::_tao_any_destructor
        );
      return 1;
    }
  }
  ACE_CATCHANY
  {
    _tao_elem = FT_PullMonitorable::_nil ();
    return 0;
  }
  ACE_ENDTRY;
  _tao_elem = FT_PullMonitorable::_nil ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)  || defined (ACE_HAS_GNU_REPO)
    template class TAO_Object_Manager<FT_PullMonitorable,FT_PullMonitorable_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#  pragma instantiate TAO_Object_Manager<FT_PullMonitorable,FT_PullMonitorable_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

FT_FaultNotifier_ptr (*_TAO_collocation_FT_FaultNotifier_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  ) = 0;
void operator<<= (CORBA::Any &_tao_any, FT_FaultNotifier_ptr _tao_elem)
{
  TAO_OutputCDR stream;
  if (stream << _tao_elem)
  {
    _tao_any._tao_replace (
        FT::_tc_FaultNotifier,
        TAO_ENCAP_BYTE_ORDER,
        stream.begin (),
        1,
        FT_FaultNotifier::_duplicate (_tao_elem),
        FT_FaultNotifier::_tao_any_destructor
      );
  }
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_FaultNotifier_ptr &_tao_elem)
{
  ACE_TRY_NEW_ENV
  {
    _tao_elem = FT_FaultNotifier::_nil ();
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_FaultNotifier, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    TAO_InputCDR stream (
        _tao_any._tao_get_cdr (),
        _tao_any._tao_byte_order ()
      );
    if (stream >> _tao_elem)
    {
      ((CORBA::Any *)&_tao_any)->_tao_replace (
          FT::_tc_FaultNotifier,
          1,
          _tao_elem,
          FT_FaultNotifier::_tao_any_destructor
        );
      return 1;
    }
  }
  ACE_CATCHANY
  {
    _tao_elem = FT_FaultNotifier::_nil ();
    return 0;
  }
  ACE_ENDTRY;
  _tao_elem = FT_FaultNotifier::_nil ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)  || defined (ACE_HAS_GNU_REPO)
template class TAO_Object_Manager<FT_FaultNotifier,FT_FaultNotifier_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#  pragma instantiate TAO_Object_Manager<FT_FaultNotifier,FT_FaultNotifier_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

void operator<<= (
    CORBA::Any &_tao_any,
    const FT_State &_tao_elem
  ) // copying
{
  TAO_OutputCDR stream;
  if (stream << _tao_elem)
  {
    _tao_any._tao_replace (
        FT::_tc_State,
        TAO_ENCAP_BYTE_ORDER,
        stream.begin ()
      );
  }
}

void operator<<= (CORBA::Any &_tao_any, FT_State *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_State,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_State::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_State *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_State*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_State *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_State, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = ACE_static_cast(
          const FT_State*,
          _tao_any.value ()
        );
      return 1;
    }
    else
    {
      FT_State *tmp;
      ACE_NEW_RETURN (tmp, FT_State, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_State,
            1,
            ACE_static_cast (void *, tmp),
            FT_State::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_NoStateAvailable &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_NoStateAvailable,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_NoStateAvailable *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_NoStateAvailable,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_NoStateAvailable::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_NoStateAvailable *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_NoStateAvailable*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_NoStateAvailable *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_NoStateAvailable, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_NoStateAvailable *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_NoStateAvailable *tmp;
      ACE_NEW_RETURN (tmp, FT_NoStateAvailable, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/NoStateAvailable:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_NoStateAvailable,
            1,
            tmp,
            FT_NoStateAvailable::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_InvalidState &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_InvalidState,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_InvalidState *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_InvalidState,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_InvalidState::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_InvalidState *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_InvalidState*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_InvalidState *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_InvalidState, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_InvalidState *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_InvalidState *tmp;
      ACE_NEW_RETURN (tmp, FT_InvalidState, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/InvalidState:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_InvalidState,
            1,
            tmp,
            FT_InvalidState::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_NoUpdateAvailable &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_NoUpdateAvailable,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_NoUpdateAvailable *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_NoUpdateAvailable,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_NoUpdateAvailable::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_NoUpdateAvailable *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_NoUpdateAvailable*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_NoUpdateAvailable *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_NoUpdateAvailable, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_NoUpdateAvailable *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_NoUpdateAvailable *tmp;
      ACE_NEW_RETURN (tmp, FT_NoUpdateAvailable, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/NoUpdateAvailable:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_NoUpdateAvailable,
            1,
            tmp,
            FT_NoUpdateAvailable::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

void operator<<= (CORBA::Any &_tao_any, const FT_InvalidUpdate &_tao_elem) // copying
{
  TAO_OutputCDR stream;
  stream << _tao_elem;
  _tao_any._tao_replace (
      FT::_tc_InvalidUpdate,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin ()
    );
}

void operator<<= (CORBA::Any &_tao_any, FT_InvalidUpdate *_tao_elem) // non copying
{
  TAO_OutputCDR stream;
  stream << *_tao_elem;
  _tao_any._tao_replace (
      FT::_tc_InvalidUpdate,
      TAO_ENCAP_BYTE_ORDER,
      stream.begin (),
      1,
      _tao_elem,
      FT_InvalidUpdate::_tao_any_destructor
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_InvalidUpdate *&_tao_elem)
{
  return _tao_any >>= ACE_const_cast(
      const FT_InvalidUpdate*&,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const FT_InvalidUpdate *&_tao_elem)
{
  _tao_elem = 0;
  ACE_TRY_NEW_ENV
  {
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_InvalidUpdate, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    if (_tao_any.any_owns_data ())
    {
      _tao_elem = (FT_InvalidUpdate *)_tao_any.value ();
      return 1;
    }
    else
    {
      FT_InvalidUpdate *tmp;
      ACE_NEW_RETURN (tmp, FT_InvalidUpdate, 0);
      TAO_InputCDR stream (
          _tao_any._tao_get_cdr (),
          _tao_any._tao_byte_order ()
        );
      CORBA::String_var interface_repository_id;
      if (!(stream >> interface_repository_id.out ()))
        return 0;
      if (ACE_OS::strcmp (
          interface_repository_id.in (),
          "IDL:omg.org/FT/InvalidUpdate:1.0"))
        return 0;
      if (stream >> *tmp)
      {
        ((CORBA::Any *)&_tao_any)->_tao_replace (
            FT::_tc_InvalidUpdate,
            1,
            tmp,
            FT_InvalidUpdate::_tao_any_destructor
          );
        _tao_elem = tmp;
        return 1;
      }
      else
      {
        delete tmp;
      }
    }
  }
  ACE_CATCHANY
  {
  }
  ACE_ENDTRY;
  return 0;
}

FT_Checkpointable_ptr (*_TAO_collocation_FT_Checkpointable_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  ) = 0;
void operator<<= (CORBA::Any &_tao_any, FT_Checkpointable_ptr _tao_elem)
{
  TAO_OutputCDR stream;
  if (stream << _tao_elem)
  {
    _tao_any._tao_replace (
        FT::_tc_Checkpointable,
        TAO_ENCAP_BYTE_ORDER,
        stream.begin (),
        1,
        FT_Checkpointable::_duplicate (_tao_elem),
        FT_Checkpointable::_tao_any_destructor
      );
  }
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_Checkpointable_ptr &_tao_elem)
{
  ACE_TRY_NEW_ENV
  {
    _tao_elem = FT_Checkpointable::_nil ();
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_Checkpointable, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    TAO_InputCDR stream (
        _tao_any._tao_get_cdr (),
        _tao_any._tao_byte_order ()
      );
    if (stream >> _tao_elem)
    {
      ((CORBA::Any *)&_tao_any)->_tao_replace (
          FT::_tc_Checkpointable,
          1,
          _tao_elem,
          FT_Checkpointable::_tao_any_destructor
        );
      return 1;
    }
  }
  ACE_CATCHANY
  {
    _tao_elem = FT_Checkpointable::_nil ();
    return 0;
  }
  ACE_ENDTRY;
  _tao_elem = FT_Checkpointable::_nil ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)  || defined (ACE_HAS_GNU_REPO)
template class TAO_Object_Manager<FT_Checkpointable,FT_Checkpointable_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#  pragma instantiate TAO_Object_Manager<FT_Checkpointable,FT_Checkpointable_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

FT_Updateable_ptr (*_TAO_collocation_FT_Updateable_Stub_Factory_function_pointer) (
    CORBA::Object_ptr obj
  ) = 0;
void operator<<= (CORBA::Any &_tao_any, FT_Updateable_ptr _tao_elem)
{
  TAO_OutputCDR stream;
  if (stream << _tao_elem)
  {
    _tao_any._tao_replace (
        FT::_tc_Updateable,
        TAO_ENCAP_BYTE_ORDER,
        stream.begin (),
        1,
        FT_Updateable::_duplicate (_tao_elem),
        FT_Updateable::_tao_any_destructor
      );
  }
}

CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, FT_Updateable_ptr &_tao_elem)
{
  ACE_TRY_NEW_ENV
  {
    _tao_elem = FT_Updateable::_nil ();
    CORBA::TypeCode_var type = _tao_any.type ();
    if (!type->equivalent (FT::_tc_Updateable, ACE_TRY_ENV)) // not equal
      {
        return 0;
      }
    ACE_TRY_CHECK;
    TAO_InputCDR stream (
        _tao_any._tao_get_cdr (),
        _tao_any._tao_byte_order ()
      );
    if (stream >> _tao_elem)
    {
      ((CORBA::Any *)&_tao_any)->_tao_replace (
          FT::_tc_Updateable,
          1,
          _tao_elem,
          FT_Updateable::_tao_any_destructor
        );
      return 1;
    }
  }
  ACE_CATCHANY
  {
    _tao_elem = FT_Updateable::_nil ();
    return 0;
  }
  ACE_ENDTRY;
  _tao_elem = FT_Updateable::_nil ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)  || defined (ACE_HAS_GNU_REPO)
template class TAO_Object_Manager<FT_Updateable,FT_Updateable_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#  pragma instantiate TAO_Object_Manager<FT_Updateable,FT_Updateable_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const FT_Name &_tao_sequence
  )
{
  if (strm << _tao_sequence.length ())
  {
    // encode all elements
    CORBA::Boolean _tao_marshal_flag = 1;
    for (CORBA::ULong i = 0; i < _tao_sequence.length () && _tao_marshal_flag; i++)
    {
      _tao_marshal_flag = (strm << _tao_sequence[i]);
    }
    return _tao_marshal_flag;
  }
  return 0; // error
}

CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    FT_Name &_tao_sequence
  )
{
  CORBA::ULong _tao_seq_len;
  if (strm >> _tao_seq_len)
  {
    // set the length of the sequence
    _tao_sequence.length (_tao_seq_len);
    // If length is 0 we return true.
    if (0 >= _tao_seq_len)
      return 1;
    // retrieve all the elements
    CORBA::Boolean _tao_marshal_flag = 1;
    for (CORBA::ULong i = 0; i < _tao_sequence.length () && _tao_marshal_flag; i++)
    {
      _tao_marshal_flag = (strm >> _tao_sequence[i]);
    }
    return _tao_marshal_flag;
  }
  return 0; // error
}

CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const FT_Properties &_tao_sequence
  )
{
  if (strm << _tao_sequence.length ())
  {
    // encode all elements
    CORBA::Boolean _tao_marshal_flag = 1;
    for (CORBA::ULong i = 0; i < _tao_sequence.length () && _tao_marshal_flag; i++)
    {
      _tao_marshal_flag = (strm << _tao_sequence[i]);
    }
    return _tao_marshal_flag;
  }
  return 0; // error
}

CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    FT_Properties &_tao_sequence
  )
{
  CORBA::ULong _tao_seq_len;
  if (strm >> _tao_seq_len)
  {
    // set the length of the sequence
    _tao_sequence.length (_tao_seq_len);
    // If length is 0 we return true.
    if (0 >= _tao_seq_len)
      return 1;
    // retrieve all the elements
    CORBA::Boolean _tao_marshal_flag = 1;
    for (CORBA::ULong i = 0; i < _tao_sequence.length () && _tao_marshal_flag; i++)
    {
      _tao_marshal_flag = (strm >> _tao_sequence[i]);
    }
    return _tao_marshal_flag;
  }
  return 0; // error
}

CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const FT_Locations &_tao_sequence
  )
{
  if (strm << _tao_sequence.length ())
  {
    // encode all elements
    CORBA::Boolean _tao_marshal_flag = 1;
    for (CORBA::ULong i = 0; i < _tao_sequence.length () && _tao_marshal_flag; i++)
    {
      _tao_marshal_flag = (strm << _tao_sequence[i]);
    }
    return _tao_marshal_flag;
  }
  return 0; // error
}

CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    FT_Locations &_tao_sequence
  )
{
  CORBA::ULong _tao_seq_len;
  if (strm >> _tao_seq_len)
  {
    // set the length of the sequence
    _tao_sequence.length (_tao_seq_len);
    // If length is 0 we return true.
    if (0 >= _tao_seq_len)
      return 1;
    // retrieve all the elements
    CORBA::Boolean _tao_marshal_flag = 1;
    for (CORBA::ULong i = 0; i < _tao_sequence.length () && _tao_marshal_flag; i++)
    {
      _tao_marshal_flag = (strm >> _tao_sequence[i]);
    }
    return _tao_marshal_flag;
  }
  return 0; // error
}

CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const FT_FactoryInfos &_tao_sequence
  )
{
  if (strm << _tao_sequence.length ())
  {
    // encode all elements
    CORBA::Boolean _tao_marshal_flag = 1;
    for (CORBA::ULong i = 0; i < _tao_sequence.length () && _tao_marshal_flag; i++)
    {
      _tao_marshal_flag = (strm << _tao_sequence[i]);
    }
    return _tao_marshal_flag;
  }
  return 0; // error
}

CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    FT_FactoryInfos &_tao_sequence
  )
{
  CORBA::ULong _tao_seq_len;
  if (strm >> _tao_seq_len)
  {
    // set the length of the sequence
    _tao_sequence.length (_tao_seq_len);
    // If length is 0 we return true.
    if (0 >= _tao_seq_len)
      return 1;
    // retrieve all the elements
    CORBA::Boolean _tao_marshal_flag = 1;
    for (CORBA::ULong i = 0; i < _tao_sequence.length () && _tao_marshal_flag; i++)
    {
      _tao_marshal_flag = (strm >> _tao_sequence[i]);
    }
    return _tao_marshal_flag;
  }
  return 0; // error
}

CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const FT_State &_tao_sequence
  )
{
  if (strm << _tao_sequence.length ())
  {
    // encode all elements

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
    {
      TAO_Unbounded_Sequence<CORBA::Octet> *oseq =
        ACE_static_cast (TAO_Unbounded_Sequence<CORBA::Octet>*, (FT_State *)&_tao_sequence);
      if (oseq->mb ())
        return strm.write_octet_array_mb (oseq->mb ());
      else
        return strm.write_octet_array (_tao_sequence.get_buffer (), _tao_sequence.length ());
    }

#else /* TAO_NO_COPY_OCTET_SEQUENCES == 0 */
    return strm.write_octet_array (_tao_sequence.get_buffer (), _tao_sequence.length ());

#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 0 */
  }
  return 0; // error
}

CORBA::Boolean operator>> (TAO_InputCDR &strm,
                           FT_State &_tao_sequence)
{
  CORBA::ULong _tao_seq_len;
  if (strm >> _tao_seq_len)
    {
      // set the length of the sequence
      _tao_sequence.length (_tao_seq_len);
      // If length is 0 we return true.
      if (0 >= _tao_seq_len)
        return 1;
      // retrieve all the elements

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
      if (ACE_BIT_DISABLED (strm.start ()->flags (),ACE_Message_Block::DONT_DELETE))
        {
          TAO_Unbounded_Sequence<CORBA::Octet> *oseq =
            ACE_static_cast(TAO_Unbounded_Sequence<CORBA::Octet>*, &_tao_sequence);
          oseq->replace (_tao_seq_len, strm.start ());
          oseq->mb ()->wr_ptr (oseq->mb()->rd_ptr () + _tao_seq_len);
          strm.skip_bytes (_tao_seq_len);
          return 1;
        }
      else
        return strm.read_octet_array (_tao_sequence.get_buffer (), _tao_seq_len);

#else /* TAO_NO_COPY_OCTET_SEQUENCES == 0 */
      return strm.read_octet_array (_tao_sequence.get_buffer (), _tao_sequence.length ());

#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 0 */
    }
  return 0; // error
}

#endif /*TAO_HAS_FT_CORBA == 1*/
