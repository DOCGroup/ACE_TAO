// $Id$
#include "Basic_Deployment_Data.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    // TCKind
    // 

    TCKind::Value TCKind::
    integral () const
    {
      return v_;
    }

    bool
    operator== (::CIAO::Config_Handlers::TCKind const& a, ::CIAO::Config_Handlers::TCKind const& b)
    {
      return a.v_ == b.v_;
    }

    bool
    operator!= (::CIAO::Config_Handlers::TCKind const& a, ::CIAO::Config_Handlers::TCKind const& b)
    {
      return a.v_ != b.v_;
    }

    TCKind::
    TCKind (TCKind::Value v)
    : v_ (v)
    {
    }

    // DataType
    // 

    DataType::
    DataType ()
    :
    regulator__ ()
    {
    }

    DataType::
    DataType (::CIAO::Config_Handlers::DataType const& s)
    :
    kind_ (s.kind_.get () ? new ::CIAO::Config_Handlers::TCKind (*s.kind_) : 0),
    regulator__ ()
    {
      if (kind_.get ()) kind_->container (this);
    }

    ::CIAO::Config_Handlers::DataType& DataType::
    operator= (::CIAO::Config_Handlers::DataType const& s)
    {
      if (s.kind_.get ()) kind (*(s.kind_));
      else kind_ = ::std::auto_ptr< ::CIAO::Config_Handlers::TCKind > (0);

      return *this;
    }


    // DataType
    // 
    bool DataType::
    kind_p () const
    {
      return kind_.get () != 0;
    }

    ::CIAO::Config_Handlers::TCKind const& DataType::
    kind () const
    {
      return *kind_;
    }

    ::CIAO::Config_Handlers::TCKind& DataType::
    kind ()
    {
      return *kind_;
    }

    void DataType::
    kind (::CIAO::Config_Handlers::TCKind const& e)
    {
      if (kind_.get ())
      {
        *kind_ = e;
      }

      else
      {
        kind_ = ::std::auto_ptr< ::CIAO::Config_Handlers::TCKind > (new ::CIAO::Config_Handlers::TCKind (e));
        kind_->container (this);
      }
    }


    // DataValue
    // 

    DataValue::
    DataValue ()
    :
    regulator__ ()
    {
    }

    DataValue::
    DataValue (::CIAO::Config_Handlers::DataValue const& s)
    :
    short__ (s.short__.get () ? new ::XMLSchema::short_ (*s.short__) : 0),
    long__ (s.long__.get () ? new ::XMLSchema::int_ (*s.long__) : 0),
    ushort_ (s.ushort_.get () ? new ::XMLSchema::unsignedShort (*s.ushort_) : 0),
    ulong_ (s.ulong_.get () ? new ::XMLSchema::unsignedInt (*s.ulong_) : 0),
    float__ (s.float__.get () ? new ::XMLSchema::float_ (*s.float__) : 0),
    double__ (s.double__.get () ? new ::XMLSchema::double_ (*s.double__) : 0),
    boolean_ (s.boolean_.get () ? new ::XMLSchema::boolean (*s.boolean_) : 0),
    octet_ (s.octet_.get () ? new ::XMLSchema::unsignedByte (*s.octet_) : 0),
    objref_ (s.objref_.get () ? new ::XMLSchema::string< char > (*s.objref_) : 0),
    enum__ (s.enum__.get () ? new ::XMLSchema::string< char > (*s.enum__) : 0),
    string_ (s.string_.get () ? new ::XMLSchema::string< char > (*s.string_) : 0),
    longlong_ (s.longlong_.get () ? new ::XMLSchema::long_ (*s.longlong_) : 0),
    ulonglong_ (s.ulonglong_.get () ? new ::XMLSchema::unsignedLong (*s.ulonglong_) : 0),
    longdouble_ (s.longdouble_.get () ? new ::XMLSchema::double_ (*s.longdouble_) : 0),
    fixed_ (s.fixed_.get () ? new ::XMLSchema::string< char > (*s.fixed_) : 0),
    typecode_ (s.typecode_.get () ? new ::CIAO::Config_Handlers::DataType (*s.typecode_) : 0),
    regulator__ ()
    {
      if (short__.get ()) short__->container (this);
      if (long__.get ()) long__->container (this);
      if (ushort_.get ()) ushort_->container (this);
      if (ulong_.get ()) ulong_->container (this);
      if (float__.get ()) float__->container (this);
      if (double__.get ()) double__->container (this);
      if (boolean_.get ()) boolean_->container (this);
      if (octet_.get ()) octet_->container (this);
      if (objref_.get ()) objref_->container (this);
      if (enum__.get ()) enum__->container (this);
      if (string_.get ()) string_->container (this);
      if (longlong_.get ()) longlong_->container (this);
      if (ulonglong_.get ()) ulonglong_->container (this);
      if (longdouble_.get ()) longdouble_->container (this);
      if (fixed_.get ()) fixed_->container (this);
      if (typecode_.get ()) typecode_->container (this);
    }

    ::CIAO::Config_Handlers::DataValue& DataValue::
    operator= (::CIAO::Config_Handlers::DataValue const& s)
    {
      if (s.short__.get ()) short_ (*(s.short__));
      else short__ = ::std::auto_ptr< ::XMLSchema::short_ > (0);

      if (s.long__.get ()) long_ (*(s.long__));
      else long__ = ::std::auto_ptr< ::XMLSchema::int_ > (0);

      if (s.ushort_.get ()) ushort (*(s.ushort_));
      else ushort_ = ::std::auto_ptr< ::XMLSchema::unsignedShort > (0);

      if (s.ulong_.get ()) ulong (*(s.ulong_));
      else ulong_ = ::std::auto_ptr< ::XMLSchema::unsignedInt > (0);

      if (s.float__.get ()) float_ (*(s.float__));
      else float__ = ::std::auto_ptr< ::XMLSchema::float_ > (0);

      if (s.double__.get ()) double_ (*(s.double__));
      else double__ = ::std::auto_ptr< ::XMLSchema::double_ > (0);

      if (s.boolean_.get ()) boolean (*(s.boolean_));
      else boolean_ = ::std::auto_ptr< ::XMLSchema::boolean > (0);

      if (s.octet_.get ()) octet (*(s.octet_));
      else octet_ = ::std::auto_ptr< ::XMLSchema::unsignedByte > (0);

      if (s.objref_.get ()) objref (*(s.objref_));
      else objref_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      if (s.enum__.get ()) enum_ (*(s.enum__));
      else enum__ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      if (s.string_.get ()) string (*(s.string_));
      else string_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      if (s.longlong_.get ()) longlong (*(s.longlong_));
      else longlong_ = ::std::auto_ptr< ::XMLSchema::long_ > (0);

      if (s.ulonglong_.get ()) ulonglong (*(s.ulonglong_));
      else ulonglong_ = ::std::auto_ptr< ::XMLSchema::unsignedLong > (0);

      if (s.longdouble_.get ()) longdouble (*(s.longdouble_));
      else longdouble_ = ::std::auto_ptr< ::XMLSchema::double_ > (0);

      if (s.fixed_.get ()) fixed (*(s.fixed_));
      else fixed_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      if (s.typecode_.get ()) typecode (*(s.typecode_));
      else typecode_ = ::std::auto_ptr< ::CIAO::Config_Handlers::DataType > (0);

      return *this;
    }


    // DataValue
    // 
    bool DataValue::
    short_p () const
    {
      return short__.get () != 0;
    }

    ::XMLSchema::short_ const& DataValue::
    short_ () const
    {
      return *short__;
    }

    ::XMLSchema::short_& DataValue::
    short_ ()
    {
      return *short__;
    }

    void DataValue::
    short_ (::XMLSchema::short_ const& e)
    {
      if (short__.get ())
      {
        *short__ = e;
      }

      else
      {
        short__ = ::std::auto_ptr< ::XMLSchema::short_ > (new ::XMLSchema::short_ (e));
        short__->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    long_p () const
    {
      return long__.get () != 0;
    }

    ::XMLSchema::int_ const& DataValue::
    long_ () const
    {
      return *long__;
    }

    ::XMLSchema::int_& DataValue::
    long_ ()
    {
      return *long__;
    }

    void DataValue::
    long_ (::XMLSchema::int_ const& e)
    {
      if (long__.get ())
      {
        *long__ = e;
      }

      else
      {
        long__ = ::std::auto_ptr< ::XMLSchema::int_ > (new ::XMLSchema::int_ (e));
        long__->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    ushort_p () const
    {
      return ushort_.get () != 0;
    }

    ::XMLSchema::unsignedShort const& DataValue::
    ushort () const
    {
      return *ushort_;
    }

    ::XMLSchema::unsignedShort& DataValue::
    ushort ()
    {
      return *ushort_;
    }

    void DataValue::
    ushort (::XMLSchema::unsignedShort const& e)
    {
      if (ushort_.get ())
      {
        *ushort_ = e;
      }

      else
      {
        ushort_ = ::std::auto_ptr< ::XMLSchema::unsignedShort > (new ::XMLSchema::unsignedShort (e));
        ushort_->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    ulong_p () const
    {
      return ulong_.get () != 0;
    }

    ::XMLSchema::unsignedInt const& DataValue::
    ulong () const
    {
      return *ulong_;
    }

    ::XMLSchema::unsignedInt& DataValue::
    ulong ()
    {
      return *ulong_;
    }

    void DataValue::
    ulong (::XMLSchema::unsignedInt const& e)
    {
      if (ulong_.get ())
      {
        *ulong_ = e;
      }

      else
      {
        ulong_ = ::std::auto_ptr< ::XMLSchema::unsignedInt > (new ::XMLSchema::unsignedInt (e));
        ulong_->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    float_p () const
    {
      return float__.get () != 0;
    }

    ::XMLSchema::float_ const& DataValue::
    float_ () const
    {
      return *float__;
    }

    ::XMLSchema::float_& DataValue::
    float_ ()
    {
      return *float__;
    }

    void DataValue::
    float_ (::XMLSchema::float_ const& e)
    {
      if (float__.get ())
      {
        *float__ = e;
      }

      else
      {
        float__ = ::std::auto_ptr< ::XMLSchema::float_ > (new ::XMLSchema::float_ (e));
        float__->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    double_p () const
    {
      return double__.get () != 0;
    }

    ::XMLSchema::double_ const& DataValue::
    double_ () const
    {
      return *double__;
    }

    ::XMLSchema::double_& DataValue::
    double_ ()
    {
      return *double__;
    }

    void DataValue::
    double_ (::XMLSchema::double_ const& e)
    {
      if (double__.get ())
      {
        *double__ = e;
      }

      else
      {
        double__ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
        double__->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    boolean_p () const
    {
      return boolean_.get () != 0;
    }

    ::XMLSchema::boolean const& DataValue::
    boolean () const
    {
      return *boolean_;
    }

    ::XMLSchema::boolean& DataValue::
    boolean ()
    {
      return *boolean_;
    }

    void DataValue::
    boolean (::XMLSchema::boolean const& e)
    {
      if (boolean_.get ())
      {
        *boolean_ = e;
      }

      else
      {
        boolean_ = ::std::auto_ptr< ::XMLSchema::boolean > (new ::XMLSchema::boolean (e));
        boolean_->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    octet_p () const
    {
      return octet_.get () != 0;
    }

    ::XMLSchema::unsignedByte const& DataValue::
    octet () const
    {
      return *octet_;
    }

    ::XMLSchema::unsignedByte& DataValue::
    octet ()
    {
      return *octet_;
    }

    void DataValue::
    octet (::XMLSchema::unsignedByte const& e)
    {
      if (octet_.get ())
      {
        *octet_ = e;
      }

      else
      {
        octet_ = ::std::auto_ptr< ::XMLSchema::unsignedByte > (new ::XMLSchema::unsignedByte (e));
        octet_->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    objref_p () const
    {
      return objref_.get () != 0;
    }

    ::XMLSchema::string< char > const& DataValue::
    objref () const
    {
      return *objref_;
    }

    ::XMLSchema::string< char >& DataValue::
    objref ()
    {
      return *objref_;
    }

    void DataValue::
    objref (::XMLSchema::string< char > const& e)
    {
      if (objref_.get ())
      {
        *objref_ = e;
      }

      else
      {
        objref_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
        objref_->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    enum_p () const
    {
      return enum__.get () != 0;
    }

    ::XMLSchema::string< char > const& DataValue::
    enum_ () const
    {
      return *enum__;
    }

    ::XMLSchema::string< char >& DataValue::
    enum_ ()
    {
      return *enum__;
    }

    void DataValue::
    enum_ (::XMLSchema::string< char > const& e)
    {
      if (enum__.get ())
      {
        *enum__ = e;
      }

      else
      {
        enum__ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
        enum__->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    string_p () const
    {
      return string_.get () != 0;
    }

    ::XMLSchema::string< char > const& DataValue::
    string () const
    {
      return *string_;
    }

    ::XMLSchema::string< char >& DataValue::
    string ()
    {
      return *string_;
    }

    void DataValue::
    string (::XMLSchema::string< char > const& e)
    {
      if (string_.get ())
      {
        *string_ = e;
      }

      else
      {
        string_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
        string_->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    longlong_p () const
    {
      return longlong_.get () != 0;
    }

    ::XMLSchema::long_ const& DataValue::
    longlong () const
    {
      return *longlong_;
    }

    ::XMLSchema::long_& DataValue::
    longlong ()
    {
      return *longlong_;
    }

    void DataValue::
    longlong (::XMLSchema::long_ const& e)
    {
      if (longlong_.get ())
      {
        *longlong_ = e;
      }

      else
      {
        longlong_ = ::std::auto_ptr< ::XMLSchema::long_ > (new ::XMLSchema::long_ (e));
        longlong_->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    ulonglong_p () const
    {
      return ulonglong_.get () != 0;
    }

    ::XMLSchema::unsignedLong const& DataValue::
    ulonglong () const
    {
      return *ulonglong_;
    }

    ::XMLSchema::unsignedLong& DataValue::
    ulonglong ()
    {
      return *ulonglong_;
    }

    void DataValue::
    ulonglong (::XMLSchema::unsignedLong const& e)
    {
      if (ulonglong_.get ())
      {
        *ulonglong_ = e;
      }

      else
      {
        ulonglong_ = ::std::auto_ptr< ::XMLSchema::unsignedLong > (new ::XMLSchema::unsignedLong (e));
        ulonglong_->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    longdouble_p () const
    {
      return longdouble_.get () != 0;
    }

    ::XMLSchema::double_ const& DataValue::
    longdouble () const
    {
      return *longdouble_;
    }

    ::XMLSchema::double_& DataValue::
    longdouble ()
    {
      return *longdouble_;
    }

    void DataValue::
    longdouble (::XMLSchema::double_ const& e)
    {
      if (longdouble_.get ())
      {
        *longdouble_ = e;
      }

      else
      {
        longdouble_ = ::std::auto_ptr< ::XMLSchema::double_ > (new ::XMLSchema::double_ (e));
        longdouble_->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    fixed_p () const
    {
      return fixed_.get () != 0;
    }

    ::XMLSchema::string< char > const& DataValue::
    fixed () const
    {
      return *fixed_;
    }

    ::XMLSchema::string< char >& DataValue::
    fixed ()
    {
      return *fixed_;
    }

    void DataValue::
    fixed (::XMLSchema::string< char > const& e)
    {
      if (fixed_.get ())
      {
        *fixed_ = e;
      }

      else
      {
        fixed_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
        fixed_->container (this);
      }
    }

    // DataValue
    // 
    bool DataValue::
    typecode_p () const
    {
      return typecode_.get () != 0;
    }

    ::CIAO::Config_Handlers::DataType const& DataValue::
    typecode () const
    {
      return *typecode_;
    }

    ::CIAO::Config_Handlers::DataType& DataValue::
    typecode ()
    {
      return *typecode_;
    }

    void DataValue::
    typecode (::CIAO::Config_Handlers::DataType const& e)
    {
      if (typecode_.get ())
      {
        *typecode_ = e;
      }

      else
      {
        typecode_ = ::std::auto_ptr< ::CIAO::Config_Handlers::DataType > (new ::CIAO::Config_Handlers::DataType (e));
        typecode_->container (this);
      }
    }


    // Any
    // 

    Any::
    Any (::CIAO::Config_Handlers::DataType const& type__,
    ::CIAO::Config_Handlers::DataValue const& value__)
    :
    type_ (new ::CIAO::Config_Handlers::DataType (type__)),
    value_ (new ::CIAO::Config_Handlers::DataValue (value__)),
    regulator__ ()
    {
      type_->container (this);
      value_->container (this);
    }

    Any::
    Any (::CIAO::Config_Handlers::Any const& s)
    :
    type_ (new ::CIAO::Config_Handlers::DataType (*s.type_)),
    value_ (new ::CIAO::Config_Handlers::DataValue (*s.value_)),
    regulator__ ()
    {
      type_->container (this);
      value_->container (this);
    }

    ::CIAO::Config_Handlers::Any& Any::
    operator= (::CIAO::Config_Handlers::Any const& s)
    {
      type (s.type ());

      value (s.value ());

      return *this;
    }


    // Any
    // 
    ::CIAO::Config_Handlers::DataType const& Any::
    type () const
    {
      return *type_;
    }

    ::CIAO::Config_Handlers::DataType& Any::
    type ()
    {
      return *type_;
    }

    void Any::
    type (::CIAO::Config_Handlers::DataType const& e)
    {
      *type_ = e;
    }

    // Any
    // 
    ::CIAO::Config_Handlers::DataValue const& Any::
    value () const
    {
      return *value_;
    }

    ::CIAO::Config_Handlers::DataValue& Any::
    value ()
    {
      return *value_;
    }

    void Any::
    value (::CIAO::Config_Handlers::DataValue const& e)
    {
      *value_ = e;
    }


    // Property
    // 

    Property::
    Property (::XMLSchema::string< char > const& name__,
    ::CIAO::Config_Handlers::Any const& value__)
    :
    name_ (new ::XMLSchema::string< char > (name__)),
    value_ (new ::CIAO::Config_Handlers::Any (value__)),
    regulator__ ()
    {
      name_->container (this);
      value_->container (this);
    }

    Property::
    Property (::CIAO::Config_Handlers::Property const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    value_ (new ::CIAO::Config_Handlers::Any (*s.value_)),
    regulator__ ()
    {
      name_->container (this);
      value_->container (this);
    }

    ::CIAO::Config_Handlers::Property& Property::
    operator= (::CIAO::Config_Handlers::Property const& s)
    {
      name (s.name ());

      value (s.value ());

      return *this;
    }


    // Property
    // 
    ::XMLSchema::string< char > const& Property::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& Property::
    name ()
    {
      return *name_;
    }

    void Property::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // Property
    // 
    ::CIAO::Config_Handlers::Any const& Property::
    value () const
    {
      return *value_;
    }

    ::CIAO::Config_Handlers::Any& Property::
    value ()
    {
      return *value_;
    }

    void Property::
    value (::CIAO::Config_Handlers::Any const& e)
    {
      *value_ = e;
    }


    // Node
    // 

    Node::
    Node (::XMLSchema::string< char > const& name__,
    ::XMLSchema::string< char > const& label__)
    :
    name_ (new ::XMLSchema::string< char > (name__)),
    label_ (new ::XMLSchema::string< char > (label__)),
    regulator__ ()
    {
      name_->container (this);
      label_->container (this);
    }

    Node::
    Node (::CIAO::Config_Handlers::Node const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    label_ (new ::XMLSchema::string< char > (*s.label_)),
    regulator__ ()
    {
      name_->container (this);
      label_->container (this);
    }

    ::CIAO::Config_Handlers::Node& Node::
    operator= (::CIAO::Config_Handlers::Node const& s)
    {
      name (s.name ());

      label (s.label ());

      return *this;
    }


    // Node
    // 
    ::XMLSchema::string< char > const& Node::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& Node::
    name ()
    {
      return *name_;
    }

    void Node::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // Node
    // 
    ::XMLSchema::string< char > const& Node::
    label () const
    {
      return *label_;
    }

    ::XMLSchema::string< char >& Node::
    label ()
    {
      return *label_;
    }

    void Node::
    label (::XMLSchema::string< char > const& e)
    {
      *label_ = e;
    }


    // SatisfierPropertyKind
    // 

    SatisfierPropertyKind::Value SatisfierPropertyKind::
    integral () const
    {
      return v_;
    }

    bool
    operator== (::CIAO::Config_Handlers::SatisfierPropertyKind const& a, ::CIAO::Config_Handlers::SatisfierPropertyKind const& b)
    {
      return a.v_ == b.v_;
    }

    bool
    operator!= (::CIAO::Config_Handlers::SatisfierPropertyKind const& a, ::CIAO::Config_Handlers::SatisfierPropertyKind const& b)
    {
      return a.v_ != b.v_;
    }

    SatisfierPropertyKind::
    SatisfierPropertyKind (SatisfierPropertyKind::Value v)
    : v_ (v)
    {
    }

    // SatisfierProperty
    // 

    SatisfierProperty::
    SatisfierProperty (::XMLSchema::string< char > const& name__,
    ::CIAO::Config_Handlers::SatisfierPropertyKind const& kind__,
    ::CIAO::Config_Handlers::Any const& value__)
    :
    name_ (new ::XMLSchema::string< char > (name__)),
    kind_ (new ::CIAO::Config_Handlers::SatisfierPropertyKind (kind__)),
    value_ (new ::CIAO::Config_Handlers::Any (value__)),
    regulator__ ()
    {
      name_->container (this);
      kind_->container (this);
      value_->container (this);
    }

    SatisfierProperty::
    SatisfierProperty (::CIAO::Config_Handlers::SatisfierProperty const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    kind_ (new ::CIAO::Config_Handlers::SatisfierPropertyKind (*s.kind_)),
    value_ (new ::CIAO::Config_Handlers::Any (*s.value_)),
    regulator__ ()
    {
      name_->container (this);
      kind_->container (this);
      value_->container (this);
    }

    ::CIAO::Config_Handlers::SatisfierProperty& SatisfierProperty::
    operator= (::CIAO::Config_Handlers::SatisfierProperty const& s)
    {
      name (s.name ());

      kind (s.kind ());

      value (s.value ());

      return *this;
    }


    // SatisfierProperty
    // 
    ::XMLSchema::string< char > const& SatisfierProperty::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& SatisfierProperty::
    name ()
    {
      return *name_;
    }

    void SatisfierProperty::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // SatisfierProperty
    // 
    ::CIAO::Config_Handlers::SatisfierPropertyKind const& SatisfierProperty::
    kind () const
    {
      return *kind_;
    }

    ::CIAO::Config_Handlers::SatisfierPropertyKind& SatisfierProperty::
    kind ()
    {
      return *kind_;
    }

    void SatisfierProperty::
    kind (::CIAO::Config_Handlers::SatisfierPropertyKind const& e)
    {
      *kind_ = e;
    }

    // SatisfierProperty
    // 
    ::CIAO::Config_Handlers::Any const& SatisfierProperty::
    value () const
    {
      return *value_;
    }

    ::CIAO::Config_Handlers::Any& SatisfierProperty::
    value ()
    {
      return *value_;
    }

    void SatisfierProperty::
    value (::CIAO::Config_Handlers::Any const& e)
    {
      *value_ = e;
    }


    // Resource
    // 

    Resource::
    Resource (::XMLSchema::string< char > const& name__,
    ::XMLSchema::string< char > const& resourceType__,
    ::CIAO::Config_Handlers::SatisfierProperty const& property__)
    :
    name_ (new ::XMLSchema::string< char > (name__)),
    resourceType_ (new ::XMLSchema::string< char > (resourceType__)),
    property_ (new ::CIAO::Config_Handlers::SatisfierProperty (property__)),
    regulator__ ()
    {
      name_->container (this);
      resourceType_->container (this);
      property_->container (this);
    }

    Resource::
    Resource (::CIAO::Config_Handlers::Resource const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    resourceType_ (new ::XMLSchema::string< char > (*s.resourceType_)),
    property_ (new ::CIAO::Config_Handlers::SatisfierProperty (*s.property_)),
    regulator__ ()
    {
      name_->container (this);
      resourceType_->container (this);
      property_->container (this);
    }

    ::CIAO::Config_Handlers::Resource& Resource::
    operator= (::CIAO::Config_Handlers::Resource const& s)
    {
      name (s.name ());

      resourceType (s.resourceType ());

      property (s.property ());

      return *this;
    }


    // Resource
    // 
    ::XMLSchema::string< char > const& Resource::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& Resource::
    name ()
    {
      return *name_;
    }

    void Resource::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // Resource
    // 
    ::XMLSchema::string< char > const& Resource::
    resourceType () const
    {
      return *resourceType_;
    }

    ::XMLSchema::string< char >& Resource::
    resourceType ()
    {
      return *resourceType_;
    }

    void Resource::
    resourceType (::XMLSchema::string< char > const& e)
    {
      *resourceType_ = e;
    }

    // Resource
    // 
    ::CIAO::Config_Handlers::SatisfierProperty const& Resource::
    property () const
    {
      return *property_;
    }

    ::CIAO::Config_Handlers::SatisfierProperty& Resource::
    property ()
    {
      return *property_;
    }

    void Resource::
    property (::CIAO::Config_Handlers::SatisfierProperty const& e)
    {
      *property_ = e;
    }


    // SharedResource
    // 

    SharedResource::
    SharedResource (::XMLSchema::string< char > const& name__,
    ::XMLSchema::string< char > const& resourceType__,
    ::CIAO::Config_Handlers::Node const& node__,
    ::CIAO::Config_Handlers::SatisfierProperty const& property__)
    :
    name_ (new ::XMLSchema::string< char > (name__)),
    resourceType_ (new ::XMLSchema::string< char > (resourceType__)),
    node_ (new ::CIAO::Config_Handlers::Node (node__)),
    property_ (new ::CIAO::Config_Handlers::SatisfierProperty (property__)),
    regulator__ ()
    {
      name_->container (this);
      resourceType_->container (this);
      node_->container (this);
      property_->container (this);
    }

    SharedResource::
    SharedResource (::CIAO::Config_Handlers::SharedResource const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    resourceType_ (new ::XMLSchema::string< char > (*s.resourceType_)),
    node_ (new ::CIAO::Config_Handlers::Node (*s.node_)),
    property_ (new ::CIAO::Config_Handlers::SatisfierProperty (*s.property_)),
    regulator__ ()
    {
      name_->container (this);
      resourceType_->container (this);
      node_->container (this);
      property_->container (this);
    }

    ::CIAO::Config_Handlers::SharedResource& SharedResource::
    operator= (::CIAO::Config_Handlers::SharedResource const& s)
    {
      name (s.name ());

      resourceType (s.resourceType ());

      node (s.node ());

      property (s.property ());

      return *this;
    }


    // SharedResource
    // 
    ::XMLSchema::string< char > const& SharedResource::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& SharedResource::
    name ()
    {
      return *name_;
    }

    void SharedResource::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // SharedResource
    // 
    ::XMLSchema::string< char > const& SharedResource::
    resourceType () const
    {
      return *resourceType_;
    }

    ::XMLSchema::string< char >& SharedResource::
    resourceType ()
    {
      return *resourceType_;
    }

    void SharedResource::
    resourceType (::XMLSchema::string< char > const& e)
    {
      *resourceType_ = e;
    }

    // SharedResource
    // 
    ::CIAO::Config_Handlers::Node const& SharedResource::
    node () const
    {
      return *node_;
    }

    ::CIAO::Config_Handlers::Node& SharedResource::
    node ()
    {
      return *node_;
    }

    void SharedResource::
    node (::CIAO::Config_Handlers::Node const& e)
    {
      *node_ = e;
    }

    // SharedResource
    // 
    ::CIAO::Config_Handlers::SatisfierProperty const& SharedResource::
    property () const
    {
      return *property_;
    }

    ::CIAO::Config_Handlers::SatisfierProperty& SharedResource::
    property ()
    {
      return *property_;
    }

    void SharedResource::
    property (::CIAO::Config_Handlers::SatisfierProperty const& e)
    {
      *property_ = e;
    }


    // Requirement
    // 

    Requirement::
    Requirement (::XMLSchema::string< char > const& resourceType__,
    ::XMLSchema::string< char > const& name__,
    ::CIAO::Config_Handlers::Property const& property__)
    :
    resourceType_ (new ::XMLSchema::string< char > (resourceType__)),
    name_ (new ::XMLSchema::string< char > (name__)),
    property_ (new ::CIAO::Config_Handlers::Property (property__)),
    regulator__ ()
    {
      resourceType_->container (this);
      name_->container (this);
      property_->container (this);
    }

    Requirement::
    Requirement (::CIAO::Config_Handlers::Requirement const& s)
    :
    resourceType_ (new ::XMLSchema::string< char > (*s.resourceType_)),
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    property_ (new ::CIAO::Config_Handlers::Property (*s.property_)),
    regulator__ ()
    {
      resourceType_->container (this);
      name_->container (this);
      property_->container (this);
    }

    ::CIAO::Config_Handlers::Requirement& Requirement::
    operator= (::CIAO::Config_Handlers::Requirement const& s)
    {
      resourceType (s.resourceType ());

      name (s.name ());

      property (s.property ());

      return *this;
    }


    // Requirement
    // 
    ::XMLSchema::string< char > const& Requirement::
    resourceType () const
    {
      return *resourceType_;
    }

    ::XMLSchema::string< char >& Requirement::
    resourceType ()
    {
      return *resourceType_;
    }

    void Requirement::
    resourceType (::XMLSchema::string< char > const& e)
    {
      *resourceType_ = e;
    }

    // Requirement
    // 
    ::XMLSchema::string< char > const& Requirement::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& Requirement::
    name ()
    {
      return *name_;
    }

    void Requirement::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // Requirement
    // 
    ::CIAO::Config_Handlers::Property const& Requirement::
    property () const
    {
      return *property_;
    }

    ::CIAO::Config_Handlers::Property& Requirement::
    property ()
    {
      return *property_;
    }

    void Requirement::
    property (::CIAO::Config_Handlers::Property const& e)
    {
      *property_ = e;
    }


    // ResourceDeploymentDescription
    // 

    ResourceDeploymentDescription::
    ResourceDeploymentDescription (::XMLSchema::string< char > const& requirementName__,
    ::XMLSchema::string< char > const& resourceName__,
    ::CIAO::Config_Handlers::Any const& resourceValue__)
    :
    requirementName_ (new ::XMLSchema::string< char > (requirementName__)),
    resourceName_ (new ::XMLSchema::string< char > (resourceName__)),
    resourceValue_ (new ::CIAO::Config_Handlers::Any (resourceValue__)),
    regulator__ ()
    {
      requirementName_->container (this);
      resourceName_->container (this);
      resourceValue_->container (this);
    }

    ResourceDeploymentDescription::
    ResourceDeploymentDescription (::CIAO::Config_Handlers::ResourceDeploymentDescription const& s)
    :
    requirementName_ (new ::XMLSchema::string< char > (*s.requirementName_)),
    resourceName_ (new ::XMLSchema::string< char > (*s.resourceName_)),
    resourceValue_ (new ::CIAO::Config_Handlers::Any (*s.resourceValue_)),
    regulator__ ()
    {
      requirementName_->container (this);
      resourceName_->container (this);
      resourceValue_->container (this);
    }

    ::CIAO::Config_Handlers::ResourceDeploymentDescription& ResourceDeploymentDescription::
    operator= (::CIAO::Config_Handlers::ResourceDeploymentDescription const& s)
    {
      requirementName (s.requirementName ());

      resourceName (s.resourceName ());

      resourceValue (s.resourceValue ());

      return *this;
    }


    // ResourceDeploymentDescription
    // 
    ::XMLSchema::string< char > const& ResourceDeploymentDescription::
    requirementName () const
    {
      return *requirementName_;
    }

    ::XMLSchema::string< char >& ResourceDeploymentDescription::
    requirementName ()
    {
      return *requirementName_;
    }

    void ResourceDeploymentDescription::
    requirementName (::XMLSchema::string< char > const& e)
    {
      *requirementName_ = e;
    }

    // ResourceDeploymentDescription
    // 
    ::XMLSchema::string< char > const& ResourceDeploymentDescription::
    resourceName () const
    {
      return *resourceName_;
    }

    ::XMLSchema::string< char >& ResourceDeploymentDescription::
    resourceName ()
    {
      return *resourceName_;
    }

    void ResourceDeploymentDescription::
    resourceName (::XMLSchema::string< char > const& e)
    {
      *resourceName_ = e;
    }

    // ResourceDeploymentDescription
    // 
    ::CIAO::Config_Handlers::Any const& ResourceDeploymentDescription::
    resourceValue () const
    {
      return *resourceValue_;
    }

    ::CIAO::Config_Handlers::Any& ResourceDeploymentDescription::
    resourceValue ()
    {
      return *resourceValue_;
    }

    void ResourceDeploymentDescription::
    resourceValue (::CIAO::Config_Handlers::Any const& e)
    {
      *resourceValue_ = e;
    }


    // ArtifactDeploymentDescription
    // 

    ArtifactDeploymentDescription::
    ArtifactDeploymentDescription (::XMLSchema::string< char > const& name__,
    ::XMLSchema::string< char > const& node__)
    :
    name_ (new ::XMLSchema::string< char > (name__)),
    node_ (new ::XMLSchema::string< char > (node__)),
    regulator__ ()
    {
      name_->container (this);
      node_->container (this);
    }

    ArtifactDeploymentDescription::
    ArtifactDeploymentDescription (::CIAO::Config_Handlers::ArtifactDeploymentDescription const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    node_ (new ::XMLSchema::string< char > (*s.node_)),
    id_ (s.id_.get () ? new ::XMLSchema::ID< char > (*s.id_) : 0),
    regulator__ ()
    {
      name_->container (this);
      source_.reserve (s.source_.size ());
      {
        for (source_const_iterator i (s.source_.begin ());
        i != s.source_.end ();
        ++i) add_source (*i);
      }

      node_->container (this);
      location_.reserve (s.location_.size ());
      {
        for (location_const_iterator i (s.location_.begin ());
        i != s.location_.end ();
        ++i) add_location (*i);
      }

      execParameter_.reserve (s.execParameter_.size ());
      {
        for (execParameter_const_iterator i (s.execParameter_.begin ());
        i != s.execParameter_.end ();
        ++i) add_execParameter (*i);
      }

      deployRequirement_.reserve (s.deployRequirement_.size ());
      {
        for (deployRequirement_const_iterator i (s.deployRequirement_.begin ());
        i != s.deployRequirement_.end ();
        ++i) add_deployRequirement (*i);
      }

      deployedResource_.reserve (s.deployedResource_.size ());
      {
        for (deployedResource_const_iterator i (s.deployedResource_.begin ());
        i != s.deployedResource_.end ();
        ++i) add_deployedResource (*i);
      }

      if (id_.get ()) id_->container (this);
    }

    ::CIAO::Config_Handlers::ArtifactDeploymentDescription& ArtifactDeploymentDescription::
    operator= (::CIAO::Config_Handlers::ArtifactDeploymentDescription const& s)
    {
      name (s.name ());

      source_.clear ();
      source_.reserve (s.source_.size ());
      {
        for (source_const_iterator i (s.source_.begin ());
        i != s.source_.end ();
        ++i) add_source (*i);
      }

      node (s.node ());

      location_.clear ();
      location_.reserve (s.location_.size ());
      {
        for (location_const_iterator i (s.location_.begin ());
        i != s.location_.end ();
        ++i) add_location (*i);
      }

      execParameter_.clear ();
      execParameter_.reserve (s.execParameter_.size ());
      {
        for (execParameter_const_iterator i (s.execParameter_.begin ());
        i != s.execParameter_.end ();
        ++i) add_execParameter (*i);
      }

      deployRequirement_.clear ();
      deployRequirement_.reserve (s.deployRequirement_.size ());
      {
        for (deployRequirement_const_iterator i (s.deployRequirement_.begin ());
        i != s.deployRequirement_.end ();
        ++i) add_deployRequirement (*i);
      }

      deployedResource_.clear ();
      deployedResource_.reserve (s.deployedResource_.size ());
      {
        for (deployedResource_const_iterator i (s.deployedResource_.begin ());
        i != s.deployedResource_.end ();
        ++i) add_deployedResource (*i);
      }

      if (s.id_.get ()) id (*(s.id_));
      else id_ = ::std::auto_ptr< ::XMLSchema::ID< char > > (0);

      return *this;
    }


    // ArtifactDeploymentDescription
    // 
    ::XMLSchema::string< char > const& ArtifactDeploymentDescription::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& ArtifactDeploymentDescription::
    name ()
    {
      return *name_;
    }

    void ArtifactDeploymentDescription::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // ArtifactDeploymentDescription
    // 
    ArtifactDeploymentDescription::source_iterator ArtifactDeploymentDescription::
    begin_source ()
    {
      return source_.begin ();
    }

    ArtifactDeploymentDescription::source_iterator ArtifactDeploymentDescription::
    end_source ()
    {
      return source_.end ();
    }

    ArtifactDeploymentDescription::source_const_iterator ArtifactDeploymentDescription::
    begin_source () const
    {
      return source_.begin ();
    }

    ArtifactDeploymentDescription::source_const_iterator ArtifactDeploymentDescription::
    end_source () const
    {
      return source_.end ();
    }

    void ArtifactDeploymentDescription::
    add_source (::XMLSchema::string< char > const& e)
    {
      if (source_.capacity () < source_.size () + 1)
      {
        ::std::vector< ::XMLSchema::string< char > > v;
        v.reserve (source_.size () + 1);

        while (source_.size ())
        {
          //@@ VC6
          ::XMLSchema::string< char >& t = source_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          source_.pop_back ();
        }

        source_.swap (v);
      }

      source_.push_back (e);
      source_.back ().container (this);
    }

    // ArtifactDeploymentDescription
    // 
    ::XMLSchema::string< char > const& ArtifactDeploymentDescription::
    node () const
    {
      return *node_;
    }

    ::XMLSchema::string< char >& ArtifactDeploymentDescription::
    node ()
    {
      return *node_;
    }

    void ArtifactDeploymentDescription::
    node (::XMLSchema::string< char > const& e)
    {
      *node_ = e;
    }

    // ArtifactDeploymentDescription
    // 
    ArtifactDeploymentDescription::location_iterator ArtifactDeploymentDescription::
    begin_location ()
    {
      return location_.begin ();
    }

    ArtifactDeploymentDescription::location_iterator ArtifactDeploymentDescription::
    end_location ()
    {
      return location_.end ();
    }

    ArtifactDeploymentDescription::location_const_iterator ArtifactDeploymentDescription::
    begin_location () const
    {
      return location_.begin ();
    }

    ArtifactDeploymentDescription::location_const_iterator ArtifactDeploymentDescription::
    end_location () const
    {
      return location_.end ();
    }

    void ArtifactDeploymentDescription::
    add_location (::XMLSchema::string< char > const& e)
    {
      if (location_.capacity () < location_.size () + 1)
      {
        ::std::vector< ::XMLSchema::string< char > > v;
        v.reserve (location_.size () + 1);

        while (location_.size ())
        {
          //@@ VC6
          ::XMLSchema::string< char >& t = location_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          location_.pop_back ();
        }

        location_.swap (v);
      }

      location_.push_back (e);
      location_.back ().container (this);
    }

    // ArtifactDeploymentDescription
    // 
    ArtifactDeploymentDescription::execParameter_iterator ArtifactDeploymentDescription::
    begin_execParameter ()
    {
      return execParameter_.begin ();
    }

    ArtifactDeploymentDescription::execParameter_iterator ArtifactDeploymentDescription::
    end_execParameter ()
    {
      return execParameter_.end ();
    }

    ArtifactDeploymentDescription::execParameter_const_iterator ArtifactDeploymentDescription::
    begin_execParameter () const
    {
      return execParameter_.begin ();
    }

    ArtifactDeploymentDescription::execParameter_const_iterator ArtifactDeploymentDescription::
    end_execParameter () const
    {
      return execParameter_.end ();
    }

    void ArtifactDeploymentDescription::
    add_execParameter (::CIAO::Config_Handlers::Property const& e)
    {
      if (execParameter_.capacity () < execParameter_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::Property > v;
        v.reserve (execParameter_.size () + 1);

        while (execParameter_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::Property& t = execParameter_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          execParameter_.pop_back ();
        }

        execParameter_.swap (v);
      }

      execParameter_.push_back (e);
      execParameter_.back ().container (this);
    }

    // ArtifactDeploymentDescription
    // 
    ArtifactDeploymentDescription::deployRequirement_iterator ArtifactDeploymentDescription::
    begin_deployRequirement ()
    {
      return deployRequirement_.begin ();
    }

    ArtifactDeploymentDescription::deployRequirement_iterator ArtifactDeploymentDescription::
    end_deployRequirement ()
    {
      return deployRequirement_.end ();
    }

    ArtifactDeploymentDescription::deployRequirement_const_iterator ArtifactDeploymentDescription::
    begin_deployRequirement () const
    {
      return deployRequirement_.begin ();
    }

    ArtifactDeploymentDescription::deployRequirement_const_iterator ArtifactDeploymentDescription::
    end_deployRequirement () const
    {
      return deployRequirement_.end ();
    }

    void ArtifactDeploymentDescription::
    add_deployRequirement (::CIAO::Config_Handlers::Requirement const& e)
    {
      if (deployRequirement_.capacity () < deployRequirement_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::Requirement > v;
        v.reserve (deployRequirement_.size () + 1);

        while (deployRequirement_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::Requirement& t = deployRequirement_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          deployRequirement_.pop_back ();
        }

        deployRequirement_.swap (v);
      }

      deployRequirement_.push_back (e);
      deployRequirement_.back ().container (this);
    }

    // ArtifactDeploymentDescription
    // 
    ArtifactDeploymentDescription::deployedResource_iterator ArtifactDeploymentDescription::
    begin_deployedResource ()
    {
      return deployedResource_.begin ();
    }

    ArtifactDeploymentDescription::deployedResource_iterator ArtifactDeploymentDescription::
    end_deployedResource ()
    {
      return deployedResource_.end ();
    }

    ArtifactDeploymentDescription::deployedResource_const_iterator ArtifactDeploymentDescription::
    begin_deployedResource () const
    {
      return deployedResource_.begin ();
    }

    ArtifactDeploymentDescription::deployedResource_const_iterator ArtifactDeploymentDescription::
    end_deployedResource () const
    {
      return deployedResource_.end ();
    }

    void ArtifactDeploymentDescription::
    add_deployedResource (::CIAO::Config_Handlers::ResourceDeploymentDescription const& e)
    {
      if (deployedResource_.capacity () < deployedResource_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::ResourceDeploymentDescription > v;
        v.reserve (deployedResource_.size () + 1);

        while (deployedResource_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::ResourceDeploymentDescription& t = deployedResource_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          deployedResource_.pop_back ();
        }

        deployedResource_.swap (v);
      }

      deployedResource_.push_back (e);
      deployedResource_.back ().container (this);
    }

    // ArtifactDeploymentDescription
    // 
    bool ArtifactDeploymentDescription::
    id_p () const
    {
      return id_.get () != 0;
    }

    ::XMLSchema::ID< char > const& ArtifactDeploymentDescription::
    id () const
    {
      return *id_;
    }

    ::XMLSchema::ID< char >& ArtifactDeploymentDescription::
    id ()
    {
      return *id_;
    }

    void ArtifactDeploymentDescription::
    id (::XMLSchema::ID< char > const& e)
    {
      if (id_.get ())
      {
        *id_ = e;
      }

      else
      {
        id_ = ::std::auto_ptr< ::XMLSchema::ID< char > > (new ::XMLSchema::ID< char > (e));
        id_->container (this);
      }
    }


    // MonolithicDeploymentDescription
    // 

    MonolithicDeploymentDescription::
    MonolithicDeploymentDescription (::XMLSchema::string< char > const& name__)
    :
    name_ (new ::XMLSchema::string< char > (name__)),
    regulator__ ()
    {
      name_->container (this);
    }

    MonolithicDeploymentDescription::
    MonolithicDeploymentDescription (::CIAO::Config_Handlers::MonolithicDeploymentDescription const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    id_ (s.id_.get () ? new ::XMLSchema::ID< char > (*s.id_) : 0),
    regulator__ ()
    {
      name_->container (this);
      source_.reserve (s.source_.size ());
      {
        for (source_const_iterator i (s.source_.begin ());
        i != s.source_.end ();
        ++i) add_source (*i);
      }

      artifact_.reserve (s.artifact_.size ());
      {
        for (artifact_const_iterator i (s.artifact_.begin ());
        i != s.artifact_.end ();
        ++i) add_artifact (*i);
      }

      execParameter_.reserve (s.execParameter_.size ());
      {
        for (execParameter_const_iterator i (s.execParameter_.begin ());
        i != s.execParameter_.end ();
        ++i) add_execParameter (*i);
      }

      deployRequirement_.reserve (s.deployRequirement_.size ());
      {
        for (deployRequirement_const_iterator i (s.deployRequirement_.begin ());
        i != s.deployRequirement_.end ();
        ++i) add_deployRequirement (*i);
      }

      if (id_.get ()) id_->container (this);
    }

    ::CIAO::Config_Handlers::MonolithicDeploymentDescription& MonolithicDeploymentDescription::
    operator= (::CIAO::Config_Handlers::MonolithicDeploymentDescription const& s)
    {
      name (s.name ());

      source_.clear ();
      source_.reserve (s.source_.size ());
      {
        for (source_const_iterator i (s.source_.begin ());
        i != s.source_.end ();
        ++i) add_source (*i);
      }

      artifact_.clear ();
      artifact_.reserve (s.artifact_.size ());
      {
        for (artifact_const_iterator i (s.artifact_.begin ());
        i != s.artifact_.end ();
        ++i) add_artifact (*i);
      }

      execParameter_.clear ();
      execParameter_.reserve (s.execParameter_.size ());
      {
        for (execParameter_const_iterator i (s.execParameter_.begin ());
        i != s.execParameter_.end ();
        ++i) add_execParameter (*i);
      }

      deployRequirement_.clear ();
      deployRequirement_.reserve (s.deployRequirement_.size ());
      {
        for (deployRequirement_const_iterator i (s.deployRequirement_.begin ());
        i != s.deployRequirement_.end ();
        ++i) add_deployRequirement (*i);
      }

      if (s.id_.get ()) id (*(s.id_));
      else id_ = ::std::auto_ptr< ::XMLSchema::ID< char > > (0);

      return *this;
    }


    // MonolithicDeploymentDescription
    // 
    ::XMLSchema::string< char > const& MonolithicDeploymentDescription::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& MonolithicDeploymentDescription::
    name ()
    {
      return *name_;
    }

    void MonolithicDeploymentDescription::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // MonolithicDeploymentDescription
    // 
    MonolithicDeploymentDescription::source_iterator MonolithicDeploymentDescription::
    begin_source ()
    {
      return source_.begin ();
    }

    MonolithicDeploymentDescription::source_iterator MonolithicDeploymentDescription::
    end_source ()
    {
      return source_.end ();
    }

    MonolithicDeploymentDescription::source_const_iterator MonolithicDeploymentDescription::
    begin_source () const
    {
      return source_.begin ();
    }

    MonolithicDeploymentDescription::source_const_iterator MonolithicDeploymentDescription::
    end_source () const
    {
      return source_.end ();
    }

    void MonolithicDeploymentDescription::
    add_source (::XMLSchema::string< char > const& e)
    {
      if (source_.capacity () < source_.size () + 1)
      {
        ::std::vector< ::XMLSchema::string< char > > v;
        v.reserve (source_.size () + 1);

        while (source_.size ())
        {
          //@@ VC6
          ::XMLSchema::string< char >& t = source_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          source_.pop_back ();
        }

        source_.swap (v);
      }

      source_.push_back (e);
      source_.back ().container (this);
    }

    // MonolithicDeploymentDescription
    // 
    MonolithicDeploymentDescription::artifact_iterator MonolithicDeploymentDescription::
    begin_artifact ()
    {
      return artifact_.begin ();
    }

    MonolithicDeploymentDescription::artifact_iterator MonolithicDeploymentDescription::
    end_artifact ()
    {
      return artifact_.end ();
    }

    MonolithicDeploymentDescription::artifact_const_iterator MonolithicDeploymentDescription::
    begin_artifact () const
    {
      return artifact_.begin ();
    }

    MonolithicDeploymentDescription::artifact_const_iterator MonolithicDeploymentDescription::
    end_artifact () const
    {
      return artifact_.end ();
    }

    void MonolithicDeploymentDescription::
    add_artifact (::XMLSchema::IDREF< char > const& e)
    {
      if (artifact_.capacity () < artifact_.size () + 1)
      {
        ::std::vector< ::XMLSchema::IDREF< char > > v;
        v.reserve (artifact_.size () + 1);

        while (artifact_.size ())
        {
          //@@ VC6
          ::XMLSchema::IDREF< char >& t = artifact_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          artifact_.pop_back ();
        }

        artifact_.swap (v);
      }

      artifact_.push_back (e);
      artifact_.back ().container (this);
    }

    // MonolithicDeploymentDescription
    // 
    MonolithicDeploymentDescription::execParameter_iterator MonolithicDeploymentDescription::
    begin_execParameter ()
    {
      return execParameter_.begin ();
    }

    MonolithicDeploymentDescription::execParameter_iterator MonolithicDeploymentDescription::
    end_execParameter ()
    {
      return execParameter_.end ();
    }

    MonolithicDeploymentDescription::execParameter_const_iterator MonolithicDeploymentDescription::
    begin_execParameter () const
    {
      return execParameter_.begin ();
    }

    MonolithicDeploymentDescription::execParameter_const_iterator MonolithicDeploymentDescription::
    end_execParameter () const
    {
      return execParameter_.end ();
    }

    void MonolithicDeploymentDescription::
    add_execParameter (::CIAO::Config_Handlers::Property const& e)
    {
      if (execParameter_.capacity () < execParameter_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::Property > v;
        v.reserve (execParameter_.size () + 1);

        while (execParameter_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::Property& t = execParameter_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          execParameter_.pop_back ();
        }

        execParameter_.swap (v);
      }

      execParameter_.push_back (e);
      execParameter_.back ().container (this);
    }

    // MonolithicDeploymentDescription
    // 
    MonolithicDeploymentDescription::deployRequirement_iterator MonolithicDeploymentDescription::
    begin_deployRequirement ()
    {
      return deployRequirement_.begin ();
    }

    MonolithicDeploymentDescription::deployRequirement_iterator MonolithicDeploymentDescription::
    end_deployRequirement ()
    {
      return deployRequirement_.end ();
    }

    MonolithicDeploymentDescription::deployRequirement_const_iterator MonolithicDeploymentDescription::
    begin_deployRequirement () const
    {
      return deployRequirement_.begin ();
    }

    MonolithicDeploymentDescription::deployRequirement_const_iterator MonolithicDeploymentDescription::
    end_deployRequirement () const
    {
      return deployRequirement_.end ();
    }

    void MonolithicDeploymentDescription::
    add_deployRequirement (::CIAO::Config_Handlers::Requirement const& e)
    {
      if (deployRequirement_.capacity () < deployRequirement_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::Requirement > v;
        v.reserve (deployRequirement_.size () + 1);

        while (deployRequirement_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::Requirement& t = deployRequirement_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          deployRequirement_.pop_back ();
        }

        deployRequirement_.swap (v);
      }

      deployRequirement_.push_back (e);
      deployRequirement_.back ().container (this);
    }

    // MonolithicDeploymentDescription
    // 
    bool MonolithicDeploymentDescription::
    id_p () const
    {
      return id_.get () != 0;
    }

    ::XMLSchema::ID< char > const& MonolithicDeploymentDescription::
    id () const
    {
      return *id_;
    }

    ::XMLSchema::ID< char >& MonolithicDeploymentDescription::
    id ()
    {
      return *id_;
    }

    void MonolithicDeploymentDescription::
    id (::XMLSchema::ID< char > const& e)
    {
      if (id_.get ())
      {
        *id_ = e;
      }

      else
      {
        id_ = ::std::auto_ptr< ::XMLSchema::ID< char > > (new ::XMLSchema::ID< char > (e));
        id_->container (this);
      }
    }


    // ResourceUsageKind
    // 

    ResourceUsageKind::Value ResourceUsageKind::
    integral () const
    {
      return v_;
    }

    bool
    operator== (::CIAO::Config_Handlers::ResourceUsageKind const& a, ::CIAO::Config_Handlers::ResourceUsageKind const& b)
    {
      return a.v_ == b.v_;
    }

    bool
    operator!= (::CIAO::Config_Handlers::ResourceUsageKind const& a, ::CIAO::Config_Handlers::ResourceUsageKind const& b)
    {
      return a.v_ != b.v_;
    }

    ResourceUsageKind::
    ResourceUsageKind (ResourceUsageKind::Value v)
    : v_ (v)
    {
    }

    // InstanceResourceDeploymentDescription
    // 

    InstanceResourceDeploymentDescription::
    InstanceResourceDeploymentDescription (::CIAO::Config_Handlers::ResourceUsageKind const& resourceUsage__,
    ::XMLSchema::string< char > const& requirementName__,
    ::XMLSchema::string< char > const& resourceName__,
    ::CIAO::Config_Handlers::Any const& resourceValue__)
    :
    resourceUsage_ (new ::CIAO::Config_Handlers::ResourceUsageKind (resourceUsage__)),
    requirementName_ (new ::XMLSchema::string< char > (requirementName__)),
    resourceName_ (new ::XMLSchema::string< char > (resourceName__)),
    resourceValue_ (new ::CIAO::Config_Handlers::Any (resourceValue__)),
    regulator__ ()
    {
      resourceUsage_->container (this);
      requirementName_->container (this);
      resourceName_->container (this);
      resourceValue_->container (this);
    }

    InstanceResourceDeploymentDescription::
    InstanceResourceDeploymentDescription (::CIAO::Config_Handlers::InstanceResourceDeploymentDescription const& s)
    :
    resourceUsage_ (new ::CIAO::Config_Handlers::ResourceUsageKind (*s.resourceUsage_)),
    requirementName_ (new ::XMLSchema::string< char > (*s.requirementName_)),
    resourceName_ (new ::XMLSchema::string< char > (*s.resourceName_)),
    resourceValue_ (new ::CIAO::Config_Handlers::Any (*s.resourceValue_)),
    regulator__ ()
    {
      resourceUsage_->container (this);
      requirementName_->container (this);
      resourceName_->container (this);
      resourceValue_->container (this);
    }

    ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription& InstanceResourceDeploymentDescription::
    operator= (::CIAO::Config_Handlers::InstanceResourceDeploymentDescription const& s)
    {
      resourceUsage (s.resourceUsage ());

      requirementName (s.requirementName ());

      resourceName (s.resourceName ());

      resourceValue (s.resourceValue ());

      return *this;
    }


    // InstanceResourceDeploymentDescription
    // 
    ::CIAO::Config_Handlers::ResourceUsageKind const& InstanceResourceDeploymentDescription::
    resourceUsage () const
    {
      return *resourceUsage_;
    }

    ::CIAO::Config_Handlers::ResourceUsageKind& InstanceResourceDeploymentDescription::
    resourceUsage ()
    {
      return *resourceUsage_;
    }

    void InstanceResourceDeploymentDescription::
    resourceUsage (::CIAO::Config_Handlers::ResourceUsageKind const& e)
    {
      *resourceUsage_ = e;
    }

    // InstanceResourceDeploymentDescription
    // 
    ::XMLSchema::string< char > const& InstanceResourceDeploymentDescription::
    requirementName () const
    {
      return *requirementName_;
    }

    ::XMLSchema::string< char >& InstanceResourceDeploymentDescription::
    requirementName ()
    {
      return *requirementName_;
    }

    void InstanceResourceDeploymentDescription::
    requirementName (::XMLSchema::string< char > const& e)
    {
      *requirementName_ = e;
    }

    // InstanceResourceDeploymentDescription
    // 
    ::XMLSchema::string< char > const& InstanceResourceDeploymentDescription::
    resourceName () const
    {
      return *resourceName_;
    }

    ::XMLSchema::string< char >& InstanceResourceDeploymentDescription::
    resourceName ()
    {
      return *resourceName_;
    }

    void InstanceResourceDeploymentDescription::
    resourceName (::XMLSchema::string< char > const& e)
    {
      *resourceName_ = e;
    }

    // InstanceResourceDeploymentDescription
    // 
    ::CIAO::Config_Handlers::Any const& InstanceResourceDeploymentDescription::
    resourceValue () const
    {
      return *resourceValue_;
    }

    ::CIAO::Config_Handlers::Any& InstanceResourceDeploymentDescription::
    resourceValue ()
    {
      return *resourceValue_;
    }

    void InstanceResourceDeploymentDescription::
    resourceValue (::CIAO::Config_Handlers::Any const& e)
    {
      *resourceValue_ = e;
    }


    // InstanceDeploymentDescription
    // 

    InstanceDeploymentDescription::
    InstanceDeploymentDescription (::XMLSchema::string< char > const& name__,
    ::XMLSchema::string< char > const& node__,
    ::XMLSchema::string< char > const& source__,
    ::XMLSchema::IDREF< char > const& implementation__)
    :
    name_ (new ::XMLSchema::string< char > (name__)),
    node_ (new ::XMLSchema::string< char > (node__)),
    source_ (new ::XMLSchema::string< char > (source__)),
    implementation_ (new ::XMLSchema::IDREF< char > (implementation__)),
    regulator__ ()
    {
      name_->container (this);
      node_->container (this);
      source_->container (this);
      implementation_->container (this);
    }

    InstanceDeploymentDescription::
    InstanceDeploymentDescription (::CIAO::Config_Handlers::InstanceDeploymentDescription const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    node_ (new ::XMLSchema::string< char > (*s.node_)),
    source_ (new ::XMLSchema::string< char > (*s.source_)),
    implementation_ (new ::XMLSchema::IDREF< char > (*s.implementation_)),
    deployedSharedResource_ (s.deployedSharedResource_.get () ? new ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription (*s.deployedSharedResource_) : 0),
    id_ (s.id_.get () ? new ::XMLSchema::ID< char > (*s.id_) : 0),
    regulator__ ()
    {
      name_->container (this);
      node_->container (this);
      source_->container (this);
      implementation_->container (this);
      configProperty_.reserve (s.configProperty_.size ());
      {
        for (configProperty_const_iterator i (s.configProperty_.begin ());
        i != s.configProperty_.end ();
        ++i) add_configProperty (*i);
      }

      deployedResource_.reserve (s.deployedResource_.size ());
      {
        for (deployedResource_const_iterator i (s.deployedResource_.begin ());
        i != s.deployedResource_.end ();
        ++i) add_deployedResource (*i);
      }

      if (deployedSharedResource_.get ()) deployedSharedResource_->container (this);
      if (id_.get ()) id_->container (this);
    }

    ::CIAO::Config_Handlers::InstanceDeploymentDescription& InstanceDeploymentDescription::
    operator= (::CIAO::Config_Handlers::InstanceDeploymentDescription const& s)
    {
      name (s.name ());

      node (s.node ());

      source (s.source ());

      implementation (s.implementation ());

      configProperty_.clear ();
      configProperty_.reserve (s.configProperty_.size ());
      {
        for (configProperty_const_iterator i (s.configProperty_.begin ());
        i != s.configProperty_.end ();
        ++i) add_configProperty (*i);
      }

      deployedResource_.clear ();
      deployedResource_.reserve (s.deployedResource_.size ());
      {
        for (deployedResource_const_iterator i (s.deployedResource_.begin ());
        i != s.deployedResource_.end ();
        ++i) add_deployedResource (*i);
      }

      if (s.deployedSharedResource_.get ()) deployedSharedResource (*(s.deployedSharedResource_));
      else deployedSharedResource_ = ::std::auto_ptr< ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription > (0);

      if (s.id_.get ()) id (*(s.id_));
      else id_ = ::std::auto_ptr< ::XMLSchema::ID< char > > (0);

      return *this;
    }


    // InstanceDeploymentDescription
    // 
    ::XMLSchema::string< char > const& InstanceDeploymentDescription::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& InstanceDeploymentDescription::
    name ()
    {
      return *name_;
    }

    void InstanceDeploymentDescription::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // InstanceDeploymentDescription
    // 
    ::XMLSchema::string< char > const& InstanceDeploymentDescription::
    node () const
    {
      return *node_;
    }

    ::XMLSchema::string< char >& InstanceDeploymentDescription::
    node ()
    {
      return *node_;
    }

    void InstanceDeploymentDescription::
    node (::XMLSchema::string< char > const& e)
    {
      *node_ = e;
    }

    // InstanceDeploymentDescription
    // 
    ::XMLSchema::string< char > const& InstanceDeploymentDescription::
    source () const
    {
      return *source_;
    }

    ::XMLSchema::string< char >& InstanceDeploymentDescription::
    source ()
    {
      return *source_;
    }

    void InstanceDeploymentDescription::
    source (::XMLSchema::string< char > const& e)
    {
      *source_ = e;
    }

    // InstanceDeploymentDescription
    // 
    ::XMLSchema::IDREF< char > const& InstanceDeploymentDescription::
    implementation () const
    {
      return *implementation_;
    }

    ::XMLSchema::IDREF< char >& InstanceDeploymentDescription::
    implementation ()
    {
      return *implementation_;
    }

    void InstanceDeploymentDescription::
    implementation (::XMLSchema::IDREF< char > const& e)
    {
      *implementation_ = e;
    }

    // InstanceDeploymentDescription
    // 
    InstanceDeploymentDescription::configProperty_iterator InstanceDeploymentDescription::
    begin_configProperty ()
    {
      return configProperty_.begin ();
    }

    InstanceDeploymentDescription::configProperty_iterator InstanceDeploymentDescription::
    end_configProperty ()
    {
      return configProperty_.end ();
    }

    InstanceDeploymentDescription::configProperty_const_iterator InstanceDeploymentDescription::
    begin_configProperty () const
    {
      return configProperty_.begin ();
    }

    InstanceDeploymentDescription::configProperty_const_iterator InstanceDeploymentDescription::
    end_configProperty () const
    {
      return configProperty_.end ();
    }

    void InstanceDeploymentDescription::
    add_configProperty (::CIAO::Config_Handlers::Property const& e)
    {
      if (configProperty_.capacity () < configProperty_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::Property > v;
        v.reserve (configProperty_.size () + 1);

        while (configProperty_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::Property& t = configProperty_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          configProperty_.pop_back ();
        }

        configProperty_.swap (v);
      }

      configProperty_.push_back (e);
      configProperty_.back ().container (this);
    }

    // InstanceDeploymentDescription
    // 
    InstanceDeploymentDescription::deployedResource_iterator InstanceDeploymentDescription::
    begin_deployedResource ()
    {
      return deployedResource_.begin ();
    }

    InstanceDeploymentDescription::deployedResource_iterator InstanceDeploymentDescription::
    end_deployedResource ()
    {
      return deployedResource_.end ();
    }

    InstanceDeploymentDescription::deployedResource_const_iterator InstanceDeploymentDescription::
    begin_deployedResource () const
    {
      return deployedResource_.begin ();
    }

    InstanceDeploymentDescription::deployedResource_const_iterator InstanceDeploymentDescription::
    end_deployedResource () const
    {
      return deployedResource_.end ();
    }

    void InstanceDeploymentDescription::
    add_deployedResource (::CIAO::Config_Handlers::InstanceResourceDeploymentDescription const& e)
    {
      if (deployedResource_.capacity () < deployedResource_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription > v;
        v.reserve (deployedResource_.size () + 1);

        while (deployedResource_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription& t = deployedResource_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          deployedResource_.pop_back ();
        }

        deployedResource_.swap (v);
      }

      deployedResource_.push_back (e);
      deployedResource_.back ().container (this);
    }

    // InstanceDeploymentDescription
    // 
    bool InstanceDeploymentDescription::
    deployedSharedResource_p () const
    {
      return deployedSharedResource_.get () != 0;
    }

    ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription const& InstanceDeploymentDescription::
    deployedSharedResource () const
    {
      return *deployedSharedResource_;
    }

    ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription& InstanceDeploymentDescription::
    deployedSharedResource ()
    {
      return *deployedSharedResource_;
    }

    void InstanceDeploymentDescription::
    deployedSharedResource (::CIAO::Config_Handlers::InstanceResourceDeploymentDescription const& e)
    {
      if (deployedSharedResource_.get ())
      {
        *deployedSharedResource_ = e;
      }

      else
      {
        deployedSharedResource_ = ::std::auto_ptr< ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription > (new ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription (e));
        deployedSharedResource_->container (this);
      }
    }

    // InstanceDeploymentDescription
    // 
    bool InstanceDeploymentDescription::
    id_p () const
    {
      return id_.get () != 0;
    }

    ::XMLSchema::ID< char > const& InstanceDeploymentDescription::
    id () const
    {
      return *id_;
    }

    ::XMLSchema::ID< char >& InstanceDeploymentDescription::
    id ()
    {
      return *id_;
    }

    void InstanceDeploymentDescription::
    id (::XMLSchema::ID< char > const& e)
    {
      if (id_.get ())
      {
        *id_ = e;
      }

      else
      {
        id_ = ::std::auto_ptr< ::XMLSchema::ID< char > > (new ::XMLSchema::ID< char > (e));
        id_->container (this);
      }
    }


    // CCMComponentPortKind
    // 

    CCMComponentPortKind::Value CCMComponentPortKind::
    integral () const
    {
      return v_;
    }

    bool
    operator== (::CIAO::Config_Handlers::CCMComponentPortKind const& a, ::CIAO::Config_Handlers::CCMComponentPortKind const& b)
    {
      return a.v_ == b.v_;
    }

    bool
    operator!= (::CIAO::Config_Handlers::CCMComponentPortKind const& a, ::CIAO::Config_Handlers::CCMComponentPortKind const& b)
    {
      return a.v_ != b.v_;
    }

    CCMComponentPortKind::
    CCMComponentPortKind (CCMComponentPortKind::Value v)
    : v_ (v)
    {
    }

    // ComponentPortDescription
    // 

    ComponentPortDescription::
    ComponentPortDescription (::XMLSchema::string< char > const& name__,
    ::XMLSchema::string< char > const& specificType__,
    ::XMLSchema::string< char > const& supportedType__,
    ::XMLSchema::string< char > const& provider__,
    ::XMLSchema::string< char > const& exclusiveProvider__,
    ::XMLSchema::string< char > const& exclusiveUser__,
    ::XMLSchema::string< char > const& optional__,
    ::CIAO::Config_Handlers::CCMComponentPortKind const& kind__)
    :
    name_ (new ::XMLSchema::string< char > (name__)),
    specificType_ (new ::XMLSchema::string< char > (specificType__)),
    supportedType_ (new ::XMLSchema::string< char > (supportedType__)),
    provider_ (new ::XMLSchema::string< char > (provider__)),
    exclusiveProvider_ (new ::XMLSchema::string< char > (exclusiveProvider__)),
    exclusiveUser_ (new ::XMLSchema::string< char > (exclusiveUser__)),
    optional_ (new ::XMLSchema::string< char > (optional__)),
    kind_ (new ::CIAO::Config_Handlers::CCMComponentPortKind (kind__)),
    regulator__ ()
    {
      name_->container (this);
      specificType_->container (this);
      supportedType_->container (this);
      provider_->container (this);
      exclusiveProvider_->container (this);
      exclusiveUser_->container (this);
      optional_->container (this);
      kind_->container (this);
    }

    ComponentPortDescription::
    ComponentPortDescription (::CIAO::Config_Handlers::ComponentPortDescription const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    specificType_ (new ::XMLSchema::string< char > (*s.specificType_)),
    supportedType_ (new ::XMLSchema::string< char > (*s.supportedType_)),
    provider_ (new ::XMLSchema::string< char > (*s.provider_)),
    exclusiveProvider_ (new ::XMLSchema::string< char > (*s.exclusiveProvider_)),
    exclusiveUser_ (new ::XMLSchema::string< char > (*s.exclusiveUser_)),
    optional_ (new ::XMLSchema::string< char > (*s.optional_)),
    kind_ (new ::CIAO::Config_Handlers::CCMComponentPortKind (*s.kind_)),
    regulator__ ()
    {
      name_->container (this);
      specificType_->container (this);
      supportedType_->container (this);
      provider_->container (this);
      exclusiveProvider_->container (this);
      exclusiveUser_->container (this);
      optional_->container (this);
      kind_->container (this);
    }

    ::CIAO::Config_Handlers::ComponentPortDescription& ComponentPortDescription::
    operator= (::CIAO::Config_Handlers::ComponentPortDescription const& s)
    {
      name (s.name ());

      specificType (s.specificType ());

      supportedType (s.supportedType ());

      provider (s.provider ());

      exclusiveProvider (s.exclusiveProvider ());

      exclusiveUser (s.exclusiveUser ());

      optional (s.optional ());

      kind (s.kind ());

      return *this;
    }


    // ComponentPortDescription
    // 
    ::XMLSchema::string< char > const& ComponentPortDescription::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& ComponentPortDescription::
    name ()
    {
      return *name_;
    }

    void ComponentPortDescription::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // ComponentPortDescription
    // 
    ::XMLSchema::string< char > const& ComponentPortDescription::
    specificType () const
    {
      return *specificType_;
    }

    ::XMLSchema::string< char >& ComponentPortDescription::
    specificType ()
    {
      return *specificType_;
    }

    void ComponentPortDescription::
    specificType (::XMLSchema::string< char > const& e)
    {
      *specificType_ = e;
    }

    // ComponentPortDescription
    // 
    ::XMLSchema::string< char > const& ComponentPortDescription::
    supportedType () const
    {
      return *supportedType_;
    }

    ::XMLSchema::string< char >& ComponentPortDescription::
    supportedType ()
    {
      return *supportedType_;
    }

    void ComponentPortDescription::
    supportedType (::XMLSchema::string< char > const& e)
    {
      *supportedType_ = e;
    }

    // ComponentPortDescription
    // 
    ::XMLSchema::string< char > const& ComponentPortDescription::
    provider () const
    {
      return *provider_;
    }

    ::XMLSchema::string< char >& ComponentPortDescription::
    provider ()
    {
      return *provider_;
    }

    void ComponentPortDescription::
    provider (::XMLSchema::string< char > const& e)
    {
      *provider_ = e;
    }

    // ComponentPortDescription
    // 
    ::XMLSchema::string< char > const& ComponentPortDescription::
    exclusiveProvider () const
    {
      return *exclusiveProvider_;
    }

    ::XMLSchema::string< char >& ComponentPortDescription::
    exclusiveProvider ()
    {
      return *exclusiveProvider_;
    }

    void ComponentPortDescription::
    exclusiveProvider (::XMLSchema::string< char > const& e)
    {
      *exclusiveProvider_ = e;
    }

    // ComponentPortDescription
    // 
    ::XMLSchema::string< char > const& ComponentPortDescription::
    exclusiveUser () const
    {
      return *exclusiveUser_;
    }

    ::XMLSchema::string< char >& ComponentPortDescription::
    exclusiveUser ()
    {
      return *exclusiveUser_;
    }

    void ComponentPortDescription::
    exclusiveUser (::XMLSchema::string< char > const& e)
    {
      *exclusiveUser_ = e;
    }

    // ComponentPortDescription
    // 
    ::XMLSchema::string< char > const& ComponentPortDescription::
    optional () const
    {
      return *optional_;
    }

    ::XMLSchema::string< char >& ComponentPortDescription::
    optional ()
    {
      return *optional_;
    }

    void ComponentPortDescription::
    optional (::XMLSchema::string< char > const& e)
    {
      *optional_ = e;
    }

    // ComponentPortDescription
    // 
    ::CIAO::Config_Handlers::CCMComponentPortKind const& ComponentPortDescription::
    kind () const
    {
      return *kind_;
    }

    ::CIAO::Config_Handlers::CCMComponentPortKind& ComponentPortDescription::
    kind ()
    {
      return *kind_;
    }

    void ComponentPortDescription::
    kind (::CIAO::Config_Handlers::CCMComponentPortKind const& e)
    {
      *kind_ = e;
    }


    // ComponentPropertyDescription
    // 

    ComponentPropertyDescription::
    ComponentPropertyDescription (::XMLSchema::string< char > const& name__,
    ::CIAO::Config_Handlers::DataType const& type__)
    :
    name_ (new ::XMLSchema::string< char > (name__)),
    type_ (new ::CIAO::Config_Handlers::DataType (type__)),
    regulator__ ()
    {
      name_->container (this);
      type_->container (this);
    }

    ComponentPropertyDescription::
    ComponentPropertyDescription (::CIAO::Config_Handlers::ComponentPropertyDescription const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    type_ (new ::CIAO::Config_Handlers::DataType (*s.type_)),
    regulator__ ()
    {
      name_->container (this);
      type_->container (this);
    }

    ::CIAO::Config_Handlers::ComponentPropertyDescription& ComponentPropertyDescription::
    operator= (::CIAO::Config_Handlers::ComponentPropertyDescription const& s)
    {
      name (s.name ());

      type (s.type ());

      return *this;
    }


    // ComponentPropertyDescription
    // 
    ::XMLSchema::string< char > const& ComponentPropertyDescription::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& ComponentPropertyDescription::
    name ()
    {
      return *name_;
    }

    void ComponentPropertyDescription::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // ComponentPropertyDescription
    // 
    ::CIAO::Config_Handlers::DataType const& ComponentPropertyDescription::
    type () const
    {
      return *type_;
    }

    ::CIAO::Config_Handlers::DataType& ComponentPropertyDescription::
    type ()
    {
      return *type_;
    }

    void ComponentPropertyDescription::
    type (::CIAO::Config_Handlers::DataType const& e)
    {
      *type_ = e;
    }


    // ComponentExternalPortEndpoint
    // 

    ComponentExternalPortEndpoint::
    ComponentExternalPortEndpoint (::XMLSchema::string< char > const& portName__)
    :
    portName_ (new ::XMLSchema::string< char > (portName__)),
    regulator__ ()
    {
      portName_->container (this);
    }

    ComponentExternalPortEndpoint::
    ComponentExternalPortEndpoint (::CIAO::Config_Handlers::ComponentExternalPortEndpoint const& s)
    :
    portName_ (new ::XMLSchema::string< char > (*s.portName_)),
    regulator__ ()
    {
      portName_->container (this);
    }

    ::CIAO::Config_Handlers::ComponentExternalPortEndpoint& ComponentExternalPortEndpoint::
    operator= (::CIAO::Config_Handlers::ComponentExternalPortEndpoint const& s)
    {
      portName (s.portName ());

      return *this;
    }


    // ComponentExternalPortEndpoint
    // 
    ::XMLSchema::string< char > const& ComponentExternalPortEndpoint::
    portName () const
    {
      return *portName_;
    }

    ::XMLSchema::string< char >& ComponentExternalPortEndpoint::
    portName ()
    {
      return *portName_;
    }

    void ComponentExternalPortEndpoint::
    portName (::XMLSchema::string< char > const& e)
    {
      *portName_ = e;
    }


    // PlanSubcomponentPortEndpoint
    // 

    PlanSubcomponentPortEndpoint::
    PlanSubcomponentPortEndpoint (::XMLSchema::string< char > const& portName__,
    ::CIAO::Config_Handlers::CCMComponentPortKind const& kind__,
    ::XMLSchema::IDREF< char > const& instance__)
    :
    portName_ (new ::XMLSchema::string< char > (portName__)),
    kind_ (new ::CIAO::Config_Handlers::CCMComponentPortKind (kind__)),
    instance_ (new ::XMLSchema::IDREF< char > (instance__)),
    regulator__ ()
    {
      portName_->container (this);
      kind_->container (this);
      instance_->container (this);
    }

    PlanSubcomponentPortEndpoint::
    PlanSubcomponentPortEndpoint (::CIAO::Config_Handlers::PlanSubcomponentPortEndpoint const& s)
    :
    portName_ (new ::XMLSchema::string< char > (*s.portName_)),
    provider_ (s.provider_.get () ? new ::XMLSchema::string< char > (*s.provider_) : 0),
    kind_ (new ::CIAO::Config_Handlers::CCMComponentPortKind (*s.kind_)),
    instance_ (new ::XMLSchema::IDREF< char > (*s.instance_)),
    regulator__ ()
    {
      portName_->container (this);
      if (provider_.get ()) provider_->container (this);
      kind_->container (this);
      instance_->container (this);
    }

    ::CIAO::Config_Handlers::PlanSubcomponentPortEndpoint& PlanSubcomponentPortEndpoint::
    operator= (::CIAO::Config_Handlers::PlanSubcomponentPortEndpoint const& s)
    {
      portName (s.portName ());

      if (s.provider_.get ()) provider (*(s.provider_));
      else provider_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      kind (s.kind ());

      instance (s.instance ());

      return *this;
    }


    // PlanSubcomponentPortEndpoint
    // 
    ::XMLSchema::string< char > const& PlanSubcomponentPortEndpoint::
    portName () const
    {
      return *portName_;
    }

    ::XMLSchema::string< char >& PlanSubcomponentPortEndpoint::
    portName ()
    {
      return *portName_;
    }

    void PlanSubcomponentPortEndpoint::
    portName (::XMLSchema::string< char > const& e)
    {
      *portName_ = e;
    }

    // PlanSubcomponentPortEndpoint
    // 
    bool PlanSubcomponentPortEndpoint::
    provider_p () const
    {
      return provider_.get () != 0;
    }

    ::XMLSchema::string< char > const& PlanSubcomponentPortEndpoint::
    provider () const
    {
      return *provider_;
    }

    ::XMLSchema::string< char >& PlanSubcomponentPortEndpoint::
    provider ()
    {
      return *provider_;
    }

    void PlanSubcomponentPortEndpoint::
    provider (::XMLSchema::string< char > const& e)
    {
      if (provider_.get ())
      {
        *provider_ = e;
      }

      else
      {
        provider_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
        provider_->container (this);
      }
    }

    // PlanSubcomponentPortEndpoint
    // 
    ::CIAO::Config_Handlers::CCMComponentPortKind const& PlanSubcomponentPortEndpoint::
    kind () const
    {
      return *kind_;
    }

    ::CIAO::Config_Handlers::CCMComponentPortKind& PlanSubcomponentPortEndpoint::
    kind ()
    {
      return *kind_;
    }

    void PlanSubcomponentPortEndpoint::
    kind (::CIAO::Config_Handlers::CCMComponentPortKind const& e)
    {
      *kind_ = e;
    }

    // PlanSubcomponentPortEndpoint
    // 
    ::XMLSchema::IDREF< char > const& PlanSubcomponentPortEndpoint::
    instance () const
    {
      return *instance_;
    }

    ::XMLSchema::IDREF< char >& PlanSubcomponentPortEndpoint::
    instance ()
    {
      return *instance_;
    }

    void PlanSubcomponentPortEndpoint::
    instance (::XMLSchema::IDREF< char > const& e)
    {
      *instance_ = e;
    }


    // ExternalReferenceEndpoint
    // 

    ExternalReferenceEndpoint::
    ExternalReferenceEndpoint (::XMLSchema::string< char > const& location__)
    :
    location_ (new ::XMLSchema::string< char > (location__)),
    regulator__ ()
    {
      location_->container (this);
    }

    ExternalReferenceEndpoint::
    ExternalReferenceEndpoint (::CIAO::Config_Handlers::ExternalReferenceEndpoint const& s)
    :
    location_ (new ::XMLSchema::string< char > (*s.location_)),
    regulator__ ()
    {
      location_->container (this);
    }

    ::CIAO::Config_Handlers::ExternalReferenceEndpoint& ExternalReferenceEndpoint::
    operator= (::CIAO::Config_Handlers::ExternalReferenceEndpoint const& s)
    {
      location (s.location ());

      return *this;
    }


    // ExternalReferenceEndpoint
    // 
    ::XMLSchema::string< char > const& ExternalReferenceEndpoint::
    location () const
    {
      return *location_;
    }

    ::XMLSchema::string< char >& ExternalReferenceEndpoint::
    location ()
    {
      return *location_;
    }

    void ExternalReferenceEndpoint::
    location (::XMLSchema::string< char > const& e)
    {
      *location_ = e;
    }


    // ConnectionResourceDeploymentDescription
    // 

    ConnectionResourceDeploymentDescription::
    ConnectionResourceDeploymentDescription (::XMLSchema::string< char > const& targetName__,
    ::XMLSchema::string< char > const& requirementName__,
    ::XMLSchema::string< char > const& resourceName__,
    ::CIAO::Config_Handlers::Any const& resourceValue__)
    :
    targetName_ (new ::XMLSchema::string< char > (targetName__)),
    requirementName_ (new ::XMLSchema::string< char > (requirementName__)),
    resourceName_ (new ::XMLSchema::string< char > (resourceName__)),
    resourceValue_ (new ::CIAO::Config_Handlers::Any (resourceValue__)),
    regulator__ ()
    {
      targetName_->container (this);
      requirementName_->container (this);
      resourceName_->container (this);
      resourceValue_->container (this);
    }

    ConnectionResourceDeploymentDescription::
    ConnectionResourceDeploymentDescription (::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription const& s)
    :
    targetName_ (new ::XMLSchema::string< char > (*s.targetName_)),
    requirementName_ (new ::XMLSchema::string< char > (*s.requirementName_)),
    resourceName_ (new ::XMLSchema::string< char > (*s.resourceName_)),
    resourceValue_ (new ::CIAO::Config_Handlers::Any (*s.resourceValue_)),
    regulator__ ()
    {
      targetName_->container (this);
      requirementName_->container (this);
      resourceName_->container (this);
      resourceValue_->container (this);
    }

    ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription& ConnectionResourceDeploymentDescription::
    operator= (::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription const& s)
    {
      targetName (s.targetName ());

      requirementName (s.requirementName ());

      resourceName (s.resourceName ());

      resourceValue (s.resourceValue ());

      return *this;
    }


    // ConnectionResourceDeploymentDescription
    // 
    ::XMLSchema::string< char > const& ConnectionResourceDeploymentDescription::
    targetName () const
    {
      return *targetName_;
    }

    ::XMLSchema::string< char >& ConnectionResourceDeploymentDescription::
    targetName ()
    {
      return *targetName_;
    }

    void ConnectionResourceDeploymentDescription::
    targetName (::XMLSchema::string< char > const& e)
    {
      *targetName_ = e;
    }

    // ConnectionResourceDeploymentDescription
    // 
    ::XMLSchema::string< char > const& ConnectionResourceDeploymentDescription::
    requirementName () const
    {
      return *requirementName_;
    }

    ::XMLSchema::string< char >& ConnectionResourceDeploymentDescription::
    requirementName ()
    {
      return *requirementName_;
    }

    void ConnectionResourceDeploymentDescription::
    requirementName (::XMLSchema::string< char > const& e)
    {
      *requirementName_ = e;
    }

    // ConnectionResourceDeploymentDescription
    // 
    ::XMLSchema::string< char > const& ConnectionResourceDeploymentDescription::
    resourceName () const
    {
      return *resourceName_;
    }

    ::XMLSchema::string< char >& ConnectionResourceDeploymentDescription::
    resourceName ()
    {
      return *resourceName_;
    }

    void ConnectionResourceDeploymentDescription::
    resourceName (::XMLSchema::string< char > const& e)
    {
      *resourceName_ = e;
    }

    // ConnectionResourceDeploymentDescription
    // 
    ::CIAO::Config_Handlers::Any const& ConnectionResourceDeploymentDescription::
    resourceValue () const
    {
      return *resourceValue_;
    }

    ::CIAO::Config_Handlers::Any& ConnectionResourceDeploymentDescription::
    resourceValue ()
    {
      return *resourceValue_;
    }

    void ConnectionResourceDeploymentDescription::
    resourceValue (::CIAO::Config_Handlers::Any const& e)
    {
      *resourceValue_ = e;
    }


    // PlanConnectionDescription
    // 

    PlanConnectionDescription::
    PlanConnectionDescription (::XMLSchema::string< char > const& name__)
    :
    name_ (new ::XMLSchema::string< char > (name__)),
    regulator__ ()
    {
      name_->container (this);
    }

    PlanConnectionDescription::
    PlanConnectionDescription (::CIAO::Config_Handlers::PlanConnectionDescription const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    source_ (s.source_.get () ? new ::XMLSchema::string< char > (*s.source_) : 0),
    regulator__ ()
    {
      name_->container (this);
      if (source_.get ()) source_->container (this);
      deployRequirement_.reserve (s.deployRequirement_.size ());
      {
        for (deployRequirement_const_iterator i (s.deployRequirement_.begin ());
        i != s.deployRequirement_.end ();
        ++i) add_deployRequirement (*i);
      }

      externalEndpoint_.reserve (s.externalEndpoint_.size ());
      {
        for (externalEndpoint_const_iterator i (s.externalEndpoint_.begin ());
        i != s.externalEndpoint_.end ();
        ++i) add_externalEndpoint (*i);
      }

      internalEndpoint_.reserve (s.internalEndpoint_.size ());
      {
        for (internalEndpoint_const_iterator i (s.internalEndpoint_.begin ());
        i != s.internalEndpoint_.end ();
        ++i) add_internalEndpoint (*i);
      }

      externalReference_.reserve (s.externalReference_.size ());
      {
        for (externalReference_const_iterator i (s.externalReference_.begin ());
        i != s.externalReference_.end ();
        ++i) add_externalReference (*i);
      }

      deployedResource_.reserve (s.deployedResource_.size ());
      {
        for (deployedResource_const_iterator i (s.deployedResource_.begin ());
        i != s.deployedResource_.end ();
        ++i) add_deployedResource (*i);
      }
    }

    ::CIAO::Config_Handlers::PlanConnectionDescription& PlanConnectionDescription::
    operator= (::CIAO::Config_Handlers::PlanConnectionDescription const& s)
    {
      name (s.name ());

      if (s.source_.get ()) source (*(s.source_));
      else source_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      deployRequirement_.clear ();
      deployRequirement_.reserve (s.deployRequirement_.size ());
      {
        for (deployRequirement_const_iterator i (s.deployRequirement_.begin ());
        i != s.deployRequirement_.end ();
        ++i) add_deployRequirement (*i);
      }

      externalEndpoint_.clear ();
      externalEndpoint_.reserve (s.externalEndpoint_.size ());
      {
        for (externalEndpoint_const_iterator i (s.externalEndpoint_.begin ());
        i != s.externalEndpoint_.end ();
        ++i) add_externalEndpoint (*i);
      }

      internalEndpoint_.clear ();
      internalEndpoint_.reserve (s.internalEndpoint_.size ());
      {
        for (internalEndpoint_const_iterator i (s.internalEndpoint_.begin ());
        i != s.internalEndpoint_.end ();
        ++i) add_internalEndpoint (*i);
      }

      externalReference_.clear ();
      externalReference_.reserve (s.externalReference_.size ());
      {
        for (externalReference_const_iterator i (s.externalReference_.begin ());
        i != s.externalReference_.end ();
        ++i) add_externalReference (*i);
      }

      deployedResource_.clear ();
      deployedResource_.reserve (s.deployedResource_.size ());
      {
        for (deployedResource_const_iterator i (s.deployedResource_.begin ());
        i != s.deployedResource_.end ();
        ++i) add_deployedResource (*i);
      }

      return *this;
    }


    // PlanConnectionDescription
    // 
    ::XMLSchema::string< char > const& PlanConnectionDescription::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& PlanConnectionDescription::
    name ()
    {
      return *name_;
    }

    void PlanConnectionDescription::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // PlanConnectionDescription
    // 
    bool PlanConnectionDescription::
    source_p () const
    {
      return source_.get () != 0;
    }

    ::XMLSchema::string< char > const& PlanConnectionDescription::
    source () const
    {
      return *source_;
    }

    ::XMLSchema::string< char >& PlanConnectionDescription::
    source ()
    {
      return *source_;
    }

    void PlanConnectionDescription::
    source (::XMLSchema::string< char > const& e)
    {
      if (source_.get ())
      {
        *source_ = e;
      }

      else
      {
        source_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
        source_->container (this);
      }
    }

    // PlanConnectionDescription
    // 
    PlanConnectionDescription::deployRequirement_iterator PlanConnectionDescription::
    begin_deployRequirement ()
    {
      return deployRequirement_.begin ();
    }

    PlanConnectionDescription::deployRequirement_iterator PlanConnectionDescription::
    end_deployRequirement ()
    {
      return deployRequirement_.end ();
    }

    PlanConnectionDescription::deployRequirement_const_iterator PlanConnectionDescription::
    begin_deployRequirement () const
    {
      return deployRequirement_.begin ();
    }

    PlanConnectionDescription::deployRequirement_const_iterator PlanConnectionDescription::
    end_deployRequirement () const
    {
      return deployRequirement_.end ();
    }

    void PlanConnectionDescription::
    add_deployRequirement (::CIAO::Config_Handlers::Requirement const& e)
    {
      if (deployRequirement_.capacity () < deployRequirement_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::Requirement > v;
        v.reserve (deployRequirement_.size () + 1);

        while (deployRequirement_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::Requirement& t = deployRequirement_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          deployRequirement_.pop_back ();
        }

        deployRequirement_.swap (v);
      }

      deployRequirement_.push_back (e);
      deployRequirement_.back ().container (this);
    }

    // PlanConnectionDescription
    // 
    PlanConnectionDescription::externalEndpoint_iterator PlanConnectionDescription::
    begin_externalEndpoint ()
    {
      return externalEndpoint_.begin ();
    }

    PlanConnectionDescription::externalEndpoint_iterator PlanConnectionDescription::
    end_externalEndpoint ()
    {
      return externalEndpoint_.end ();
    }

    PlanConnectionDescription::externalEndpoint_const_iterator PlanConnectionDescription::
    begin_externalEndpoint () const
    {
      return externalEndpoint_.begin ();
    }

    PlanConnectionDescription::externalEndpoint_const_iterator PlanConnectionDescription::
    end_externalEndpoint () const
    {
      return externalEndpoint_.end ();
    }

    void PlanConnectionDescription::
    add_externalEndpoint (::CIAO::Config_Handlers::ComponentExternalPortEndpoint const& e)
    {
      if (externalEndpoint_.capacity () < externalEndpoint_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::ComponentExternalPortEndpoint > v;
        v.reserve (externalEndpoint_.size () + 1);

        while (externalEndpoint_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::ComponentExternalPortEndpoint& t = externalEndpoint_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          externalEndpoint_.pop_back ();
        }

        externalEndpoint_.swap (v);
      }

      externalEndpoint_.push_back (e);
      externalEndpoint_.back ().container (this);
    }

    // PlanConnectionDescription
    // 
    PlanConnectionDescription::internalEndpoint_iterator PlanConnectionDescription::
    begin_internalEndpoint ()
    {
      return internalEndpoint_.begin ();
    }

    PlanConnectionDescription::internalEndpoint_iterator PlanConnectionDescription::
    end_internalEndpoint ()
    {
      return internalEndpoint_.end ();
    }

    PlanConnectionDescription::internalEndpoint_const_iterator PlanConnectionDescription::
    begin_internalEndpoint () const
    {
      return internalEndpoint_.begin ();
    }

    PlanConnectionDescription::internalEndpoint_const_iterator PlanConnectionDescription::
    end_internalEndpoint () const
    {
      return internalEndpoint_.end ();
    }

    void PlanConnectionDescription::
    add_internalEndpoint (::CIAO::Config_Handlers::PlanSubcomponentPortEndpoint const& e)
    {
      if (internalEndpoint_.capacity () < internalEndpoint_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::PlanSubcomponentPortEndpoint > v;
        v.reserve (internalEndpoint_.size () + 1);

        while (internalEndpoint_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::PlanSubcomponentPortEndpoint& t = internalEndpoint_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          internalEndpoint_.pop_back ();
        }

        internalEndpoint_.swap (v);
      }

      internalEndpoint_.push_back (e);
      internalEndpoint_.back ().container (this);
    }

    // PlanConnectionDescription
    // 
    PlanConnectionDescription::externalReference_iterator PlanConnectionDescription::
    begin_externalReference ()
    {
      return externalReference_.begin ();
    }

    PlanConnectionDescription::externalReference_iterator PlanConnectionDescription::
    end_externalReference ()
    {
      return externalReference_.end ();
    }

    PlanConnectionDescription::externalReference_const_iterator PlanConnectionDescription::
    begin_externalReference () const
    {
      return externalReference_.begin ();
    }

    PlanConnectionDescription::externalReference_const_iterator PlanConnectionDescription::
    end_externalReference () const
    {
      return externalReference_.end ();
    }

    void PlanConnectionDescription::
    add_externalReference (::CIAO::Config_Handlers::ExternalReferenceEndpoint const& e)
    {
      if (externalReference_.capacity () < externalReference_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::ExternalReferenceEndpoint > v;
        v.reserve (externalReference_.size () + 1);

        while (externalReference_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::ExternalReferenceEndpoint& t = externalReference_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          externalReference_.pop_back ();
        }

        externalReference_.swap (v);
      }

      externalReference_.push_back (e);
      externalReference_.back ().container (this);
    }

    // PlanConnectionDescription
    // 
    PlanConnectionDescription::deployedResource_iterator PlanConnectionDescription::
    begin_deployedResource ()
    {
      return deployedResource_.begin ();
    }

    PlanConnectionDescription::deployedResource_iterator PlanConnectionDescription::
    end_deployedResource ()
    {
      return deployedResource_.end ();
    }

    PlanConnectionDescription::deployedResource_const_iterator PlanConnectionDescription::
    begin_deployedResource () const
    {
      return deployedResource_.begin ();
    }

    PlanConnectionDescription::deployedResource_const_iterator PlanConnectionDescription::
    end_deployedResource () const
    {
      return deployedResource_.end ();
    }

    void PlanConnectionDescription::
    add_deployedResource (::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription const& e)
    {
      if (deployedResource_.capacity () < deployedResource_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription > v;
        v.reserve (deployedResource_.size () + 1);

        while (deployedResource_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription& t = deployedResource_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          deployedResource_.pop_back ();
        }

        deployedResource_.swap (v);
      }

      deployedResource_.push_back (e);
      deployedResource_.back ().container (this);
    }


    // ImplementationDependency
    // 

    ImplementationDependency::
    ImplementationDependency (::XMLSchema::string< char > const& requiredType__)
    :
    requiredType_ (new ::XMLSchema::string< char > (requiredType__)),
    regulator__ ()
    {
      requiredType_->container (this);
    }

    ImplementationDependency::
    ImplementationDependency (::CIAO::Config_Handlers::ImplementationDependency const& s)
    :
    requiredType_ (new ::XMLSchema::string< char > (*s.requiredType_)),
    regulator__ ()
    {
      requiredType_->container (this);
    }

    ::CIAO::Config_Handlers::ImplementationDependency& ImplementationDependency::
    operator= (::CIAO::Config_Handlers::ImplementationDependency const& s)
    {
      requiredType (s.requiredType ());

      return *this;
    }


    // ImplementationDependency
    // 
    ::XMLSchema::string< char > const& ImplementationDependency::
    requiredType () const
    {
      return *requiredType_;
    }

    ::XMLSchema::string< char >& ImplementationDependency::
    requiredType ()
    {
      return *requiredType_;
    }

    void ImplementationDependency::
    requiredType (::XMLSchema::string< char > const& e)
    {
      *requiredType_ = e;
    }


    // Capability
    // 

    Capability::
    Capability (::XMLSchema::string< char > const& name__,
    ::XMLSchema::string< char > const& resourceType__,
    ::CIAO::Config_Handlers::SatisfierProperty const& property__)
    :
    name_ (new ::XMLSchema::string< char > (name__)),
    resourceType_ (new ::XMLSchema::string< char > (resourceType__)),
    property_ (new ::CIAO::Config_Handlers::SatisfierProperty (property__)),
    regulator__ ()
    {
      name_->container (this);
      resourceType_->container (this);
      property_->container (this);
    }

    Capability::
    Capability (::CIAO::Config_Handlers::Capability const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    resourceType_ (new ::XMLSchema::string< char > (*s.resourceType_)),
    property_ (new ::CIAO::Config_Handlers::SatisfierProperty (*s.property_)),
    regulator__ ()
    {
      name_->container (this);
      resourceType_->container (this);
      property_->container (this);
    }

    ::CIAO::Config_Handlers::Capability& Capability::
    operator= (::CIAO::Config_Handlers::Capability const& s)
    {
      name (s.name ());

      resourceType (s.resourceType ());

      property (s.property ());

      return *this;
    }


    // Capability
    // 
    ::XMLSchema::string< char > const& Capability::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& Capability::
    name ()
    {
      return *name_;
    }

    void Capability::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // Capability
    // 
    ::XMLSchema::string< char > const& Capability::
    resourceType () const
    {
      return *resourceType_;
    }

    ::XMLSchema::string< char >& Capability::
    resourceType ()
    {
      return *resourceType_;
    }

    void Capability::
    resourceType (::XMLSchema::string< char > const& e)
    {
      *resourceType_ = e;
    }

    // Capability
    // 
    ::CIAO::Config_Handlers::SatisfierProperty const& Capability::
    property () const
    {
      return *property_;
    }

    ::CIAO::Config_Handlers::SatisfierProperty& Capability::
    property ()
    {
      return *property_;
    }

    void Capability::
    property (::CIAO::Config_Handlers::SatisfierProperty const& e)
    {
      *property_ = e;
    }


    // ImplementationRequirement
    // 

    ImplementationRequirement::
    ImplementationRequirement (::CIAO::Config_Handlers::ResourceUsageKind const& resourceUsage__,
    ::XMLSchema::string< char > const& resourcePort__,
    ::XMLSchema::string< char > const& componentPort__,
    ::XMLSchema::string< char > const& resourceType__,
    ::XMLSchema::string< char > const& name__,
    ::CIAO::Config_Handlers::Property const& property__)
    :
    resourceUsage_ (new ::CIAO::Config_Handlers::ResourceUsageKind (resourceUsage__)),
    resourcePort_ (new ::XMLSchema::string< char > (resourcePort__)),
    componentPort_ (new ::XMLSchema::string< char > (componentPort__)),
    resourceType_ (new ::XMLSchema::string< char > (resourceType__)),
    name_ (new ::XMLSchema::string< char > (name__)),
    property_ (new ::CIAO::Config_Handlers::Property (property__)),
    regulator__ ()
    {
      resourceUsage_->container (this);
      resourcePort_->container (this);
      componentPort_->container (this);
      resourceType_->container (this);
      name_->container (this);
      property_->container (this);
    }

    ImplementationRequirement::
    ImplementationRequirement (::CIAO::Config_Handlers::ImplementationRequirement const& s)
    :
    resourceUsage_ (new ::CIAO::Config_Handlers::ResourceUsageKind (*s.resourceUsage_)),
    resourcePort_ (new ::XMLSchema::string< char > (*s.resourcePort_)),
    componentPort_ (new ::XMLSchema::string< char > (*s.componentPort_)),
    resourceType_ (new ::XMLSchema::string< char > (*s.resourceType_)),
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    property_ (new ::CIAO::Config_Handlers::Property (*s.property_)),
    regulator__ ()
    {
      resourceUsage_->container (this);
      resourcePort_->container (this);
      componentPort_->container (this);
      resourceType_->container (this);
      name_->container (this);
      property_->container (this);
    }

    ::CIAO::Config_Handlers::ImplementationRequirement& ImplementationRequirement::
    operator= (::CIAO::Config_Handlers::ImplementationRequirement const& s)
    {
      resourceUsage (s.resourceUsage ());

      resourcePort (s.resourcePort ());

      componentPort (s.componentPort ());

      resourceType (s.resourceType ());

      name (s.name ());

      property (s.property ());

      return *this;
    }


    // ImplementationRequirement
    // 
    ::CIAO::Config_Handlers::ResourceUsageKind const& ImplementationRequirement::
    resourceUsage () const
    {
      return *resourceUsage_;
    }

    ::CIAO::Config_Handlers::ResourceUsageKind& ImplementationRequirement::
    resourceUsage ()
    {
      return *resourceUsage_;
    }

    void ImplementationRequirement::
    resourceUsage (::CIAO::Config_Handlers::ResourceUsageKind const& e)
    {
      *resourceUsage_ = e;
    }

    // ImplementationRequirement
    // 
    ::XMLSchema::string< char > const& ImplementationRequirement::
    resourcePort () const
    {
      return *resourcePort_;
    }

    ::XMLSchema::string< char >& ImplementationRequirement::
    resourcePort ()
    {
      return *resourcePort_;
    }

    void ImplementationRequirement::
    resourcePort (::XMLSchema::string< char > const& e)
    {
      *resourcePort_ = e;
    }

    // ImplementationRequirement
    // 
    ::XMLSchema::string< char > const& ImplementationRequirement::
    componentPort () const
    {
      return *componentPort_;
    }

    ::XMLSchema::string< char >& ImplementationRequirement::
    componentPort ()
    {
      return *componentPort_;
    }

    void ImplementationRequirement::
    componentPort (::XMLSchema::string< char > const& e)
    {
      *componentPort_ = e;
    }

    // ImplementationRequirement
    // 
    ::XMLSchema::string< char > const& ImplementationRequirement::
    resourceType () const
    {
      return *resourceType_;
    }

    ::XMLSchema::string< char >& ImplementationRequirement::
    resourceType ()
    {
      return *resourceType_;
    }

    void ImplementationRequirement::
    resourceType (::XMLSchema::string< char > const& e)
    {
      *resourceType_ = e;
    }

    // ImplementationRequirement
    // 
    ::XMLSchema::string< char > const& ImplementationRequirement::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& ImplementationRequirement::
    name ()
    {
      return *name_;
    }

    void ImplementationRequirement::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // ImplementationRequirement
    // 
    ::CIAO::Config_Handlers::Property const& ImplementationRequirement::
    property () const
    {
      return *property_;
    }

    ::CIAO::Config_Handlers::Property& ImplementationRequirement::
    property ()
    {
      return *property_;
    }

    void ImplementationRequirement::
    property (::CIAO::Config_Handlers::Property const& e)
    {
      *property_ = e;
    }


    // ComponentPackageReference
    // 

    ComponentPackageReference::
    ComponentPackageReference (::XMLSchema::string< char > const& requiredUUID__,
    ::XMLSchema::string< char > const& requiredName__,
    ::XMLSchema::string< char > const& requiredType__)
    :
    requiredUUID_ (new ::XMLSchema::string< char > (requiredUUID__)),
    requiredName_ (new ::XMLSchema::string< char > (requiredName__)),
    requiredType_ (new ::XMLSchema::string< char > (requiredType__)),
    regulator__ ()
    {
      requiredUUID_->container (this);
      requiredName_->container (this);
      requiredType_->container (this);
    }

    ComponentPackageReference::
    ComponentPackageReference (::CIAO::Config_Handlers::ComponentPackageReference const& s)
    :
    requiredUUID_ (new ::XMLSchema::string< char > (*s.requiredUUID_)),
    requiredName_ (new ::XMLSchema::string< char > (*s.requiredName_)),
    requiredType_ (new ::XMLSchema::string< char > (*s.requiredType_)),
    regulator__ ()
    {
      requiredUUID_->container (this);
      requiredName_->container (this);
      requiredType_->container (this);
    }

    ::CIAO::Config_Handlers::ComponentPackageReference& ComponentPackageReference::
    operator= (::CIAO::Config_Handlers::ComponentPackageReference const& s)
    {
      requiredUUID (s.requiredUUID ());

      requiredName (s.requiredName ());

      requiredType (s.requiredType ());

      return *this;
    }


    // ComponentPackageReference
    // 
    ::XMLSchema::string< char > const& ComponentPackageReference::
    requiredUUID () const
    {
      return *requiredUUID_;
    }

    ::XMLSchema::string< char >& ComponentPackageReference::
    requiredUUID ()
    {
      return *requiredUUID_;
    }

    void ComponentPackageReference::
    requiredUUID (::XMLSchema::string< char > const& e)
    {
      *requiredUUID_ = e;
    }

    // ComponentPackageReference
    // 
    ::XMLSchema::string< char > const& ComponentPackageReference::
    requiredName () const
    {
      return *requiredName_;
    }

    ::XMLSchema::string< char >& ComponentPackageReference::
    requiredName ()
    {
      return *requiredName_;
    }

    void ComponentPackageReference::
    requiredName (::XMLSchema::string< char > const& e)
    {
      *requiredName_ = e;
    }

    // ComponentPackageReference
    // 
    ::XMLSchema::string< char > const& ComponentPackageReference::
    requiredType () const
    {
      return *requiredType_;
    }

    ::XMLSchema::string< char >& ComponentPackageReference::
    requiredType ()
    {
      return *requiredType_;
    }

    void ComponentPackageReference::
    requiredType (::XMLSchema::string< char > const& e)
    {
      *requiredType_ = e;
    }


    // SubcomponentPortEndpoint
    // 

    SubcomponentPortEndpoint::
    SubcomponentPortEndpoint (::XMLSchema::string< char > const& portName__,
    ::XMLSchema::IDREF< char > const& instance__)
    :
    portName_ (new ::XMLSchema::string< char > (portName__)),
    instance_ (new ::XMLSchema::IDREF< char > (instance__)),
    regulator__ ()
    {
      portName_->container (this);
      instance_->container (this);
    }

    SubcomponentPortEndpoint::
    SubcomponentPortEndpoint (::CIAO::Config_Handlers::SubcomponentPortEndpoint const& s)
    :
    portName_ (new ::XMLSchema::string< char > (*s.portName_)),
    instance_ (new ::XMLSchema::IDREF< char > (*s.instance_)),
    regulator__ ()
    {
      portName_->container (this);
      instance_->container (this);
    }

    ::CIAO::Config_Handlers::SubcomponentPortEndpoint& SubcomponentPortEndpoint::
    operator= (::CIAO::Config_Handlers::SubcomponentPortEndpoint const& s)
    {
      portName (s.portName ());

      instance (s.instance ());

      return *this;
    }


    // SubcomponentPortEndpoint
    // 
    ::XMLSchema::string< char > const& SubcomponentPortEndpoint::
    portName () const
    {
      return *portName_;
    }

    ::XMLSchema::string< char >& SubcomponentPortEndpoint::
    portName ()
    {
      return *portName_;
    }

    void SubcomponentPortEndpoint::
    portName (::XMLSchema::string< char > const& e)
    {
      *portName_ = e;
    }

    // SubcomponentPortEndpoint
    // 
    ::XMLSchema::IDREF< char > const& SubcomponentPortEndpoint::
    instance () const
    {
      return *instance_;
    }

    ::XMLSchema::IDREF< char >& SubcomponentPortEndpoint::
    instance ()
    {
      return *instance_;
    }

    void SubcomponentPortEndpoint::
    instance (::XMLSchema::IDREF< char > const& e)
    {
      *instance_ = e;
    }


    // AssemblyConnectionDescription
    // 

    AssemblyConnectionDescription::
    AssemblyConnectionDescription (::XMLSchema::string< char > const& name__)
    :
    name_ (new ::XMLSchema::string< char > (name__)),
    regulator__ ()
    {
      name_->container (this);
    }

    AssemblyConnectionDescription::
    AssemblyConnectionDescription (::CIAO::Config_Handlers::AssemblyConnectionDescription const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    deployRequirement_ (s.deployRequirement_.get () ? new ::CIAO::Config_Handlers::Requirement (*s.deployRequirement_) : 0),
    regulator__ ()
    {
      name_->container (this);
      if (deployRequirement_.get ()) deployRequirement_->container (this);
      externalEndpoint_.reserve (s.externalEndpoint_.size ());
      {
        for (externalEndpoint_const_iterator i (s.externalEndpoint_.begin ());
        i != s.externalEndpoint_.end ();
        ++i) add_externalEndpoint (*i);
      }

      internalEndpoint_.reserve (s.internalEndpoint_.size ());
      {
        for (internalEndpoint_const_iterator i (s.internalEndpoint_.begin ());
        i != s.internalEndpoint_.end ();
        ++i) add_internalEndpoint (*i);
      }

      externalReference_.reserve (s.externalReference_.size ());
      {
        for (externalReference_const_iterator i (s.externalReference_.begin ());
        i != s.externalReference_.end ();
        ++i) add_externalReference (*i);
      }
    }

    ::CIAO::Config_Handlers::AssemblyConnectionDescription& AssemblyConnectionDescription::
    operator= (::CIAO::Config_Handlers::AssemblyConnectionDescription const& s)
    {
      name (s.name ());

      if (s.deployRequirement_.get ()) deployRequirement (*(s.deployRequirement_));
      else deployRequirement_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > (0);

      externalEndpoint_.clear ();
      externalEndpoint_.reserve (s.externalEndpoint_.size ());
      {
        for (externalEndpoint_const_iterator i (s.externalEndpoint_.begin ());
        i != s.externalEndpoint_.end ();
        ++i) add_externalEndpoint (*i);
      }

      internalEndpoint_.clear ();
      internalEndpoint_.reserve (s.internalEndpoint_.size ());
      {
        for (internalEndpoint_const_iterator i (s.internalEndpoint_.begin ());
        i != s.internalEndpoint_.end ();
        ++i) add_internalEndpoint (*i);
      }

      externalReference_.clear ();
      externalReference_.reserve (s.externalReference_.size ());
      {
        for (externalReference_const_iterator i (s.externalReference_.begin ());
        i != s.externalReference_.end ();
        ++i) add_externalReference (*i);
      }

      return *this;
    }


    // AssemblyConnectionDescription
    // 
    ::XMLSchema::string< char > const& AssemblyConnectionDescription::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& AssemblyConnectionDescription::
    name ()
    {
      return *name_;
    }

    void AssemblyConnectionDescription::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // AssemblyConnectionDescription
    // 
    bool AssemblyConnectionDescription::
    deployRequirement_p () const
    {
      return deployRequirement_.get () != 0;
    }

    ::CIAO::Config_Handlers::Requirement const& AssemblyConnectionDescription::
    deployRequirement () const
    {
      return *deployRequirement_;
    }

    ::CIAO::Config_Handlers::Requirement& AssemblyConnectionDescription::
    deployRequirement ()
    {
      return *deployRequirement_;
    }

    void AssemblyConnectionDescription::
    deployRequirement (::CIAO::Config_Handlers::Requirement const& e)
    {
      if (deployRequirement_.get ())
      {
        *deployRequirement_ = e;
      }

      else
      {
        deployRequirement_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > (new ::CIAO::Config_Handlers::Requirement (e));
        deployRequirement_->container (this);
      }
    }

    // AssemblyConnectionDescription
    // 
    AssemblyConnectionDescription::externalEndpoint_iterator AssemblyConnectionDescription::
    begin_externalEndpoint ()
    {
      return externalEndpoint_.begin ();
    }

    AssemblyConnectionDescription::externalEndpoint_iterator AssemblyConnectionDescription::
    end_externalEndpoint ()
    {
      return externalEndpoint_.end ();
    }

    AssemblyConnectionDescription::externalEndpoint_const_iterator AssemblyConnectionDescription::
    begin_externalEndpoint () const
    {
      return externalEndpoint_.begin ();
    }

    AssemblyConnectionDescription::externalEndpoint_const_iterator AssemblyConnectionDescription::
    end_externalEndpoint () const
    {
      return externalEndpoint_.end ();
    }

    void AssemblyConnectionDescription::
    add_externalEndpoint (::CIAO::Config_Handlers::ComponentExternalPortEndpoint const& e)
    {
      if (externalEndpoint_.capacity () < externalEndpoint_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::ComponentExternalPortEndpoint > v;
        v.reserve (externalEndpoint_.size () + 1);

        while (externalEndpoint_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::ComponentExternalPortEndpoint& t = externalEndpoint_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          externalEndpoint_.pop_back ();
        }

        externalEndpoint_.swap (v);
      }

      externalEndpoint_.push_back (e);
      externalEndpoint_.back ().container (this);
    }

    // AssemblyConnectionDescription
    // 
    AssemblyConnectionDescription::internalEndpoint_iterator AssemblyConnectionDescription::
    begin_internalEndpoint ()
    {
      return internalEndpoint_.begin ();
    }

    AssemblyConnectionDescription::internalEndpoint_iterator AssemblyConnectionDescription::
    end_internalEndpoint ()
    {
      return internalEndpoint_.end ();
    }

    AssemblyConnectionDescription::internalEndpoint_const_iterator AssemblyConnectionDescription::
    begin_internalEndpoint () const
    {
      return internalEndpoint_.begin ();
    }

    AssemblyConnectionDescription::internalEndpoint_const_iterator AssemblyConnectionDescription::
    end_internalEndpoint () const
    {
      return internalEndpoint_.end ();
    }

    void AssemblyConnectionDescription::
    add_internalEndpoint (::CIAO::Config_Handlers::SubcomponentPortEndpoint const& e)
    {
      if (internalEndpoint_.capacity () < internalEndpoint_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::SubcomponentPortEndpoint > v;
        v.reserve (internalEndpoint_.size () + 1);

        while (internalEndpoint_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::SubcomponentPortEndpoint& t = internalEndpoint_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          internalEndpoint_.pop_back ();
        }

        internalEndpoint_.swap (v);
      }

      internalEndpoint_.push_back (e);
      internalEndpoint_.back ().container (this);
    }

    // AssemblyConnectionDescription
    // 
    AssemblyConnectionDescription::externalReference_iterator AssemblyConnectionDescription::
    begin_externalReference ()
    {
      return externalReference_.begin ();
    }

    AssemblyConnectionDescription::externalReference_iterator AssemblyConnectionDescription::
    end_externalReference ()
    {
      return externalReference_.end ();
    }

    AssemblyConnectionDescription::externalReference_const_iterator AssemblyConnectionDescription::
    begin_externalReference () const
    {
      return externalReference_.begin ();
    }

    AssemblyConnectionDescription::externalReference_const_iterator AssemblyConnectionDescription::
    end_externalReference () const
    {
      return externalReference_.end ();
    }

    void AssemblyConnectionDescription::
    add_externalReference (::CIAO::Config_Handlers::ExternalReferenceEndpoint const& e)
    {
      if (externalReference_.capacity () < externalReference_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::ExternalReferenceEndpoint > v;
        v.reserve (externalReference_.size () + 1);

        while (externalReference_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::ExternalReferenceEndpoint& t = externalReference_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          externalReference_.pop_back ();
        }

        externalReference_.swap (v);
      }

      externalReference_.push_back (e);
      externalReference_.back ().container (this);
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    // TCKind
    //

    TCKind::
    TCKind (::XSCRT::XML::Element< char > const& e)
    : Type (e)
    {
      ::std::basic_string< char > v (e.value ());

      if (v == "tk_null") v_ = tk_null_l;
      else if (v == "tk_void") v_ = tk_void_l;
      else if (v == "tk_short") v_ = tk_short_l;
      else if (v == "tk_long") v_ = tk_long_l;
      else if (v == "tk_ushort") v_ = tk_ushort_l;
      else if (v == "tk_ulong") v_ = tk_ulong_l;
      else if (v == "tk_float") v_ = tk_float_l;
      else if (v == "tk_double") v_ = tk_double_l;
      else if (v == "tk_boolean") v_ = tk_boolean_l;
      else if (v == "tk_char") v_ = tk_char_l;
      else if (v == "tk_octet") v_ = tk_octet_l;
      else if (v == "tk_any") v_ = tk_any_l;
      else if (v == "tk_TypeCode") v_ = tk_TypeCode_l;
      else if (v == "tk_Principal") v_ = tk_Principal_l;
      else if (v == "tk_objref") v_ = tk_objref_l;
      else if (v == "tk_struct") v_ = tk_struct_l;
      else if (v == "tk_union") v_ = tk_union_l;
      else if (v == "tk_enum") v_ = tk_enum_l;
      else if (v == "tk_string") v_ = tk_string_l;
      else if (v == "tk_sequence") v_ = tk_sequence_l;
      else if (v == "tk_array") v_ = tk_array_l;
      else if (v == "tk_alias") v_ = tk_alias_l;
      else if (v == "tk_except") v_ = tk_except_l;
      else if (v == "tk_longlong") v_ = tk_longlong_l;
      else if (v == "tk_ulonglong") v_ = tk_ulonglong_l;
      else if (v == "tk_longdouble") v_ = tk_longdouble_l;
      else if (v == "tk_wchar") v_ = tk_wchar_l;
      else if (v == "tk_wstring") v_ = tk_wstring_l;
      else if (v == "tk_wfixed") v_ = tk_wfixed_l;
      else if (v == "tk_value") v_ = tk_value_l;
      else if (v == "tk_value_box") v_ = tk_value_box_l;
      else if (v == "tk_native") v_ = tk_native_l;
      else if (v == "tk_abstract_interface") v_ = tk_abstract_interface_l;
      else if (v == "tk_local_interface") v_ = tk_local_interface_l;
      else if (v == "tk_component") v_ = tk_component_l;
      else if (v == "tk_home") v_ = tk_home_l;
      else if (v == "tk_event") v_ = tk_event_l;
      else 
      {
      }
    }

    TCKind::
    TCKind (::XSCRT::XML::Attribute< char > const& a)
    : Type (a)
    {
      ::std::basic_string< char > v (a.value ());

      if (v == "tk_null") v_ = tk_null_l;
      else if (v == "tk_void") v_ = tk_void_l;
      else if (v == "tk_short") v_ = tk_short_l;
      else if (v == "tk_long") v_ = tk_long_l;
      else if (v == "tk_ushort") v_ = tk_ushort_l;
      else if (v == "tk_ulong") v_ = tk_ulong_l;
      else if (v == "tk_float") v_ = tk_float_l;
      else if (v == "tk_double") v_ = tk_double_l;
      else if (v == "tk_boolean") v_ = tk_boolean_l;
      else if (v == "tk_char") v_ = tk_char_l;
      else if (v == "tk_octet") v_ = tk_octet_l;
      else if (v == "tk_any") v_ = tk_any_l;
      else if (v == "tk_TypeCode") v_ = tk_TypeCode_l;
      else if (v == "tk_Principal") v_ = tk_Principal_l;
      else if (v == "tk_objref") v_ = tk_objref_l;
      else if (v == "tk_struct") v_ = tk_struct_l;
      else if (v == "tk_union") v_ = tk_union_l;
      else if (v == "tk_enum") v_ = tk_enum_l;
      else if (v == "tk_string") v_ = tk_string_l;
      else if (v == "tk_sequence") v_ = tk_sequence_l;
      else if (v == "tk_array") v_ = tk_array_l;
      else if (v == "tk_alias") v_ = tk_alias_l;
      else if (v == "tk_except") v_ = tk_except_l;
      else if (v == "tk_longlong") v_ = tk_longlong_l;
      else if (v == "tk_ulonglong") v_ = tk_ulonglong_l;
      else if (v == "tk_longdouble") v_ = tk_longdouble_l;
      else if (v == "tk_wchar") v_ = tk_wchar_l;
      else if (v == "tk_wstring") v_ = tk_wstring_l;
      else if (v == "tk_wfixed") v_ = tk_wfixed_l;
      else if (v == "tk_value") v_ = tk_value_l;
      else if (v == "tk_value_box") v_ = tk_value_box_l;
      else if (v == "tk_native") v_ = tk_native_l;
      else if (v == "tk_abstract_interface") v_ = tk_abstract_interface_l;
      else if (v == "tk_local_interface") v_ = tk_local_interface_l;
      else if (v == "tk_component") v_ = tk_component_l;
      else if (v == "tk_home") v_ = tk_home_l;
      else if (v == "tk_event") v_ = tk_event_l;
      else 
      {
      }
    }

    TCKind const TCKind::tk_null (TCKind::tk_null_l);
    TCKind const TCKind::tk_void (TCKind::tk_void_l);
    TCKind const TCKind::tk_short (TCKind::tk_short_l);
    TCKind const TCKind::tk_long (TCKind::tk_long_l);
    TCKind const TCKind::tk_ushort (TCKind::tk_ushort_l);
    TCKind const TCKind::tk_ulong (TCKind::tk_ulong_l);
    TCKind const TCKind::tk_float (TCKind::tk_float_l);
    TCKind const TCKind::tk_double (TCKind::tk_double_l);
    TCKind const TCKind::tk_boolean (TCKind::tk_boolean_l);
    TCKind const TCKind::tk_char (TCKind::tk_char_l);
    TCKind const TCKind::tk_octet (TCKind::tk_octet_l);
    TCKind const TCKind::tk_any (TCKind::tk_any_l);
    TCKind const TCKind::tk_TypeCode (TCKind::tk_TypeCode_l);
    TCKind const TCKind::tk_Principal (TCKind::tk_Principal_l);
    TCKind const TCKind::tk_objref (TCKind::tk_objref_l);
    TCKind const TCKind::tk_struct (TCKind::tk_struct_l);
    TCKind const TCKind::tk_union (TCKind::tk_union_l);
    TCKind const TCKind::tk_enum (TCKind::tk_enum_l);
    TCKind const TCKind::tk_string (TCKind::tk_string_l);
    TCKind const TCKind::tk_sequence (TCKind::tk_sequence_l);
    TCKind const TCKind::tk_array (TCKind::tk_array_l);
    TCKind const TCKind::tk_alias (TCKind::tk_alias_l);
    TCKind const TCKind::tk_except (TCKind::tk_except_l);
    TCKind const TCKind::tk_longlong (TCKind::tk_longlong_l);
    TCKind const TCKind::tk_ulonglong (TCKind::tk_ulonglong_l);
    TCKind const TCKind::tk_longdouble (TCKind::tk_longdouble_l);
    TCKind const TCKind::tk_wchar (TCKind::tk_wchar_l);
    TCKind const TCKind::tk_wstring (TCKind::tk_wstring_l);
    TCKind const TCKind::tk_wfixed (TCKind::tk_wfixed_l);
    TCKind const TCKind::tk_value (TCKind::tk_value_l);
    TCKind const TCKind::tk_value_box (TCKind::tk_value_box_l);
    TCKind const TCKind::tk_native (TCKind::tk_native_l);
    TCKind const TCKind::tk_abstract_interface (TCKind::tk_abstract_interface_l);
    TCKind const TCKind::tk_local_interface (TCKind::tk_local_interface_l);
    TCKind const TCKind::tk_component (TCKind::tk_component_l);
    TCKind const TCKind::tk_home (TCKind::tk_home_l);
    TCKind const TCKind::tk_event (TCKind::tk_event_l);

    // DataType
    //

    DataType::
    DataType (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "kind")
        {
          ::CIAO::Config_Handlers::TCKind t (e);
          kind (t);
        }

        else 
        {
        }
      }
    }

    // DataValue
    //

    DataValue::
    DataValue (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "short")
        {
          ::XMLSchema::short_ t (e);
          short_ (t);
        }

        else if (n == "long")
        {
          ::XMLSchema::int_ t (e);
          long_ (t);
        }

        else if (n == "ushort")
        {
          ::XMLSchema::unsignedShort t (e);
          ushort (t);
        }

        else if (n == "ulong")
        {
          ::XMLSchema::unsignedInt t (e);
          ulong (t);
        }

        else if (n == "float")
        {
          ::XMLSchema::float_ t (e);
          float_ (t);
        }

        else if (n == "double")
        {
          ::XMLSchema::double_ t (e);
          double_ (t);
        }

        else if (n == "boolean")
        {
          ::XMLSchema::boolean t (e);
          boolean (t);
        }

        else if (n == "octet")
        {
          ::XMLSchema::unsignedByte t (e);
          octet (t);
        }

        else if (n == "objref")
        {
          ::XMLSchema::string< char > t (e);
          objref (t);
        }

        else if (n == "enum")
        {
          ::XMLSchema::string< char > t (e);
          enum_ (t);
        }

        else if (n == "string")
        {
          ::XMLSchema::string< char > t (e);
          string (t);
        }

        else if (n == "longlong")
        {
          ::XMLSchema::long_ t (e);
          longlong (t);
        }

        else if (n == "ulonglong")
        {
          ::XMLSchema::unsignedLong t (e);
          ulonglong (t);
        }

        else if (n == "longdouble")
        {
          ::XMLSchema::double_ t (e);
          longdouble (t);
        }

        else if (n == "fixed")
        {
          ::XMLSchema::string< char > t (e);
          fixed (t);
        }

        else if (n == "typecode")
        {
          ::CIAO::Config_Handlers::DataType t (e);
          typecode (t);
        }

        else 
        {
        }
      }
    }

    // Any
    //

    Any::
    Any (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "type")
        {
          type_ = ::std::auto_ptr< ::CIAO::Config_Handlers::DataType > (new ::CIAO::Config_Handlers::DataType (e));
          type_->container (this);
        }

        else if (n == "value")
        {
          value_ = ::std::auto_ptr< ::CIAO::Config_Handlers::DataValue > (new ::CIAO::Config_Handlers::DataValue (e));
          value_->container (this);
        }

        else 
        {
        }
      }
    }

    // Property
    //

    Property::
    Property (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "value")
        {
          value_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Any > (new ::CIAO::Config_Handlers::Any (e));
          value_->container (this);
        }

        else 
        {
        }
      }
    }

    // Node
    //

    Node::
    Node (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "label")
        {
          label_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          label_->container (this);
        }

        else 
        {
        }
      }
    }

    // SatisfierPropertyKind
    //

    SatisfierPropertyKind::
    SatisfierPropertyKind (::XSCRT::XML::Element< char > const& e)
    : Type (e)
    {
      ::std::basic_string< char > v (e.value ());

      if (v == "Quantity") v_ = Quantity_l;
      else if (v == "Capacity") v_ = Capacity_l;
      else if (v == "Minimum") v_ = Minimum_l;
      else if (v == "Maximum") v_ = Maximum_l;
      else if (v == "Attribute") v_ = Attribute_l;
      else if (v == "Selection") v_ = Selection_l;
      else 
      {
      }
    }

    SatisfierPropertyKind::
    SatisfierPropertyKind (::XSCRT::XML::Attribute< char > const& a)
    : Type (a)
    {
      ::std::basic_string< char > v (a.value ());

      if (v == "Quantity") v_ = Quantity_l;
      else if (v == "Capacity") v_ = Capacity_l;
      else if (v == "Minimum") v_ = Minimum_l;
      else if (v == "Maximum") v_ = Maximum_l;
      else if (v == "Attribute") v_ = Attribute_l;
      else if (v == "Selection") v_ = Selection_l;
      else 
      {
      }
    }

    SatisfierPropertyKind const SatisfierPropertyKind::Quantity (SatisfierPropertyKind::Quantity_l);
    SatisfierPropertyKind const SatisfierPropertyKind::Capacity (SatisfierPropertyKind::Capacity_l);
    SatisfierPropertyKind const SatisfierPropertyKind::Minimum (SatisfierPropertyKind::Minimum_l);
    SatisfierPropertyKind const SatisfierPropertyKind::Maximum (SatisfierPropertyKind::Maximum_l);
    SatisfierPropertyKind const SatisfierPropertyKind::Attribute (SatisfierPropertyKind::Attribute_l);
    SatisfierPropertyKind const SatisfierPropertyKind::Selection (SatisfierPropertyKind::Selection_l);

    // SatisfierProperty
    //

    SatisfierProperty::
    SatisfierProperty (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "kind")
        {
          kind_ = ::std::auto_ptr< ::CIAO::Config_Handlers::SatisfierPropertyKind > (new ::CIAO::Config_Handlers::SatisfierPropertyKind (e));
          kind_->container (this);
        }

        else if (n == "value")
        {
          value_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Any > (new ::CIAO::Config_Handlers::Any (e));
          value_->container (this);
        }

        else 
        {
        }
      }
    }

    // Resource
    //

    Resource::
    Resource (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "resourceType")
        {
          resourceType_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          resourceType_->container (this);
        }

        else if (n == "property")
        {
          property_ = ::std::auto_ptr< ::CIAO::Config_Handlers::SatisfierProperty > (new ::CIAO::Config_Handlers::SatisfierProperty (e));
          property_->container (this);
        }

        else 
        {
        }
      }
    }

    // SharedResource
    //

    SharedResource::
    SharedResource (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "resourceType")
        {
          resourceType_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          resourceType_->container (this);
        }

        else if (n == "node")
        {
          node_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Node > (new ::CIAO::Config_Handlers::Node (e));
          node_->container (this);
        }

        else if (n == "property")
        {
          property_ = ::std::auto_ptr< ::CIAO::Config_Handlers::SatisfierProperty > (new ::CIAO::Config_Handlers::SatisfierProperty (e));
          property_->container (this);
        }

        else 
        {
        }
      }
    }

    // Requirement
    //

    Requirement::
    Requirement (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "resourceType")
        {
          resourceType_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          resourceType_->container (this);
        }

        else if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "property")
        {
          property_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (new ::CIAO::Config_Handlers::Property (e));
          property_->container (this);
        }

        else 
        {
        }
      }
    }

    // ResourceDeploymentDescription
    //

    ResourceDeploymentDescription::
    ResourceDeploymentDescription (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "requirementName")
        {
          requirementName_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          requirementName_->container (this);
        }

        else if (n == "resourceName")
        {
          resourceName_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          resourceName_->container (this);
        }

        else if (n == "resourceValue")
        {
          resourceValue_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Any > (new ::CIAO::Config_Handlers::Any (e));
          resourceValue_->container (this);
        }

        else 
        {
        }
      }
    }

    // ArtifactDeploymentDescription
    //

    ArtifactDeploymentDescription::
    ArtifactDeploymentDescription (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "source")
        {
          ::XMLSchema::string< char > t (e);
          add_source (t);
        }

        else if (n == "node")
        {
          node_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          node_->container (this);
        }

        else if (n == "location")
        {
          ::XMLSchema::string< char > t (e);
          add_location (t);
        }

        else if (n == "execParameter")
        {
          ::CIAO::Config_Handlers::Property t (e);
          add_execParameter (t);
        }

        else if (n == "deployRequirement")
        {
          ::CIAO::Config_Handlers::Requirement t (e);
          add_deployRequirement (t);
        }

        else if (n == "deployedResource")
        {
          ::CIAO::Config_Handlers::ResourceDeploymentDescription t (e);
          add_deployedResource (t);
        }

        else 
        {
        }
      }

      while (p.more_attributes ())
      {
        ::XSCRT::XML::Attribute< char > a (p.next_attribute ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (a.name ()));
        if (n == "id")
        {
          ::XMLSchema::ID< char > t (a);
          id (t);
        }

        else 
        {
        }
      }
    }

    // MonolithicDeploymentDescription
    //

    MonolithicDeploymentDescription::
    MonolithicDeploymentDescription (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "source")
        {
          ::XMLSchema::string< char > t (e);
          add_source (t);
        }

        else if (n == "artifact")
        {
          ::XMLSchema::IDREF< char > t (e);
          add_artifact (t);
        }

        else if (n == "execParameter")
        {
          ::CIAO::Config_Handlers::Property t (e);
          add_execParameter (t);
        }

        else if (n == "deployRequirement")
        {
          ::CIAO::Config_Handlers::Requirement t (e);
          add_deployRequirement (t);
        }

        else 
        {
        }
      }

      while (p.more_attributes ())
      {
        ::XSCRT::XML::Attribute< char > a (p.next_attribute ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (a.name ()));
        if (n == "id")
        {
          ::XMLSchema::ID< char > t (a);
          id (t);
        }

        else 
        {
        }
      }
    }

    // ResourceUsageKind
    //

    ResourceUsageKind::
    ResourceUsageKind (::XSCRT::XML::Element< char > const& e)
    : Type (e)
    {
      ::std::basic_string< char > v (e.value ());

      if (v == "None") v_ = None_l;
      else if (v == "InstanceUsesResource") v_ = InstanceUsesResource_l;
      else if (v == "ResourceUsesInstance") v_ = ResourceUsesInstance_l;
      else if (v == "PortUsesResource") v_ = PortUsesResource_l;
      else if (v == "ResourceUsesPort") v_ = ResourceUsesPort_l;
      else 
      {
      }
    }

    ResourceUsageKind::
    ResourceUsageKind (::XSCRT::XML::Attribute< char > const& a)
    : Type (a)
    {
      ::std::basic_string< char > v (a.value ());

      if (v == "None") v_ = None_l;
      else if (v == "InstanceUsesResource") v_ = InstanceUsesResource_l;
      else if (v == "ResourceUsesInstance") v_ = ResourceUsesInstance_l;
      else if (v == "PortUsesResource") v_ = PortUsesResource_l;
      else if (v == "ResourceUsesPort") v_ = ResourceUsesPort_l;
      else 
      {
      }
    }

    ResourceUsageKind const ResourceUsageKind::None (ResourceUsageKind::None_l);
    ResourceUsageKind const ResourceUsageKind::InstanceUsesResource (ResourceUsageKind::InstanceUsesResource_l);
    ResourceUsageKind const ResourceUsageKind::ResourceUsesInstance (ResourceUsageKind::ResourceUsesInstance_l);
    ResourceUsageKind const ResourceUsageKind::PortUsesResource (ResourceUsageKind::PortUsesResource_l);
    ResourceUsageKind const ResourceUsageKind::ResourceUsesPort (ResourceUsageKind::ResourceUsesPort_l);

    // InstanceResourceDeploymentDescription
    //

    InstanceResourceDeploymentDescription::
    InstanceResourceDeploymentDescription (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "resourceUsage")
        {
          resourceUsage_ = ::std::auto_ptr< ::CIAO::Config_Handlers::ResourceUsageKind > (new ::CIAO::Config_Handlers::ResourceUsageKind (e));
          resourceUsage_->container (this);
        }

        else if (n == "requirementName")
        {
          requirementName_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          requirementName_->container (this);
        }

        else if (n == "resourceName")
        {
          resourceName_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          resourceName_->container (this);
        }

        else if (n == "resourceValue")
        {
          resourceValue_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Any > (new ::CIAO::Config_Handlers::Any (e));
          resourceValue_->container (this);
        }

        else 
        {
        }
      }
    }

    // InstanceDeploymentDescription
    //

    InstanceDeploymentDescription::
    InstanceDeploymentDescription (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "node")
        {
          node_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          node_->container (this);
        }

        else if (n == "source")
        {
          source_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          source_->container (this);
        }

        else if (n == "implementation")
        {
          implementation_ = ::std::auto_ptr< ::XMLSchema::IDREF< char > > (new ::XMLSchema::IDREF< char > (e));
          implementation_->container (this);
        }

        else if (n == "configProperty")
        {
          ::CIAO::Config_Handlers::Property t (e);
          add_configProperty (t);
        }

        else if (n == "deployedResource")
        {
          ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription t (e);
          add_deployedResource (t);
        }

        else if (n == "deployedSharedResource")
        {
          ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription t (e);
          deployedSharedResource (t);
        }

        else 
        {
        }
      }

      while (p.more_attributes ())
      {
        ::XSCRT::XML::Attribute< char > a (p.next_attribute ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (a.name ()));
        if (n == "id")
        {
          ::XMLSchema::ID< char > t (a);
          id (t);
        }

        else 
        {
        }
      }
    }

    // CCMComponentPortKind
    //

    CCMComponentPortKind::
    CCMComponentPortKind (::XSCRT::XML::Element< char > const& e)
    : Type (e)
    {
      ::std::basic_string< char > v (e.value ());

      if (v == "Facet") v_ = Facet_l;
      else if (v == "SimplexReceptacle") v_ = SimplexReceptacle_l;
      else if (v == "MultiplexReceptacle") v_ = MultiplexReceptacle_l;
      else if (v == "EventEmitter") v_ = EventEmitter_l;
      else if (v == "EventPublisher") v_ = EventPublisher_l;
      else if (v == "EventConsumer") v_ = EventConsumer_l;
      else 
      {
      }
    }

    CCMComponentPortKind::
    CCMComponentPortKind (::XSCRT::XML::Attribute< char > const& a)
    : Type (a)
    {
      ::std::basic_string< char > v (a.value ());

      if (v == "Facet") v_ = Facet_l;
      else if (v == "SimplexReceptacle") v_ = SimplexReceptacle_l;
      else if (v == "MultiplexReceptacle") v_ = MultiplexReceptacle_l;
      else if (v == "EventEmitter") v_ = EventEmitter_l;
      else if (v == "EventPublisher") v_ = EventPublisher_l;
      else if (v == "EventConsumer") v_ = EventConsumer_l;
      else 
      {
      }
    }

    CCMComponentPortKind const CCMComponentPortKind::Facet (CCMComponentPortKind::Facet_l);
    CCMComponentPortKind const CCMComponentPortKind::SimplexReceptacle (CCMComponentPortKind::SimplexReceptacle_l);
    CCMComponentPortKind const CCMComponentPortKind::MultiplexReceptacle (CCMComponentPortKind::MultiplexReceptacle_l);
    CCMComponentPortKind const CCMComponentPortKind::EventEmitter (CCMComponentPortKind::EventEmitter_l);
    CCMComponentPortKind const CCMComponentPortKind::EventPublisher (CCMComponentPortKind::EventPublisher_l);
    CCMComponentPortKind const CCMComponentPortKind::EventConsumer (CCMComponentPortKind::EventConsumer_l);

    // ComponentPortDescription
    //

    ComponentPortDescription::
    ComponentPortDescription (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "specificType")
        {
          specificType_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          specificType_->container (this);
        }

        else if (n == "supportedType")
        {
          supportedType_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          supportedType_->container (this);
        }

        else if (n == "provider")
        {
          provider_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          provider_->container (this);
        }

        else if (n == "exclusiveProvider")
        {
          exclusiveProvider_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          exclusiveProvider_->container (this);
        }

        else if (n == "exclusiveUser")
        {
          exclusiveUser_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          exclusiveUser_->container (this);
        }

        else if (n == "optional")
        {
          optional_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          optional_->container (this);
        }

        else if (n == "kind")
        {
          kind_ = ::std::auto_ptr< ::CIAO::Config_Handlers::CCMComponentPortKind > (new ::CIAO::Config_Handlers::CCMComponentPortKind (e));
          kind_->container (this);
        }

        else 
        {
        }
      }
    }

    // ComponentPropertyDescription
    //

    ComponentPropertyDescription::
    ComponentPropertyDescription (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "type")
        {
          type_ = ::std::auto_ptr< ::CIAO::Config_Handlers::DataType > (new ::CIAO::Config_Handlers::DataType (e));
          type_->container (this);
        }

        else 
        {
        }
      }
    }

    // ComponentExternalPortEndpoint
    //

    ComponentExternalPortEndpoint::
    ComponentExternalPortEndpoint (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "portName")
        {
          portName_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          portName_->container (this);
        }

        else 
        {
        }
      }
    }

    // PlanSubcomponentPortEndpoint
    //

    PlanSubcomponentPortEndpoint::
    PlanSubcomponentPortEndpoint (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "portName")
        {
          portName_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          portName_->container (this);
        }

        else if (n == "provider")
        {
          ::XMLSchema::string< char > t (e);
          provider (t);
        }

        else if (n == "kind")
        {
          kind_ = ::std::auto_ptr< ::CIAO::Config_Handlers::CCMComponentPortKind > (new ::CIAO::Config_Handlers::CCMComponentPortKind (e));
          kind_->container (this);
        }

        else if (n == "instance")
        {
          instance_ = ::std::auto_ptr< ::XMLSchema::IDREF< char > > (new ::XMLSchema::IDREF< char > (e));
          instance_->container (this);
        }

        else 
        {
        }
      }
    }

    // ExternalReferenceEndpoint
    //

    ExternalReferenceEndpoint::
    ExternalReferenceEndpoint (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "location")
        {
          location_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          location_->container (this);
        }

        else 
        {
        }
      }
    }

    // ConnectionResourceDeploymentDescription
    //

    ConnectionResourceDeploymentDescription::
    ConnectionResourceDeploymentDescription (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "targetName")
        {
          targetName_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          targetName_->container (this);
        }

        else if (n == "requirementName")
        {
          requirementName_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          requirementName_->container (this);
        }

        else if (n == "resourceName")
        {
          resourceName_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          resourceName_->container (this);
        }

        else if (n == "resourceValue")
        {
          resourceValue_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Any > (new ::CIAO::Config_Handlers::Any (e));
          resourceValue_->container (this);
        }

        else 
        {
        }
      }
    }

    // PlanConnectionDescription
    //

    PlanConnectionDescription::
    PlanConnectionDescription (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "source")
        {
          ::XMLSchema::string< char > t (e);
          source (t);
        }

        else if (n == "deployRequirement")
        {
          ::CIAO::Config_Handlers::Requirement t (e);
          add_deployRequirement (t);
        }

        else if (n == "externalEndpoint")
        {
          ::CIAO::Config_Handlers::ComponentExternalPortEndpoint t (e);
          add_externalEndpoint (t);
        }

        else if (n == "internalEndpoint")
        {
          ::CIAO::Config_Handlers::PlanSubcomponentPortEndpoint t (e);
          add_internalEndpoint (t);
        }

        else if (n == "externalReference")
        {
          ::CIAO::Config_Handlers::ExternalReferenceEndpoint t (e);
          add_externalReference (t);
        }

        else if (n == "deployedResource")
        {
          ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription t (e);
          add_deployedResource (t);
        }

        else 
        {
        }
      }
    }

    // ImplementationDependency
    //

    ImplementationDependency::
    ImplementationDependency (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "requiredType")
        {
          requiredType_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          requiredType_->container (this);
        }

        else 
        {
        }
      }
    }

    // Capability
    //

    Capability::
    Capability (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "resourceType")
        {
          resourceType_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          resourceType_->container (this);
        }

        else if (n == "property")
        {
          property_ = ::std::auto_ptr< ::CIAO::Config_Handlers::SatisfierProperty > (new ::CIAO::Config_Handlers::SatisfierProperty (e));
          property_->container (this);
        }

        else 
        {
        }
      }
    }

    // ImplementationRequirement
    //

    ImplementationRequirement::
    ImplementationRequirement (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "resourceUsage")
        {
          resourceUsage_ = ::std::auto_ptr< ::CIAO::Config_Handlers::ResourceUsageKind > (new ::CIAO::Config_Handlers::ResourceUsageKind (e));
          resourceUsage_->container (this);
        }

        else if (n == "resourcePort")
        {
          resourcePort_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          resourcePort_->container (this);
        }

        else if (n == "componentPort")
        {
          componentPort_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          componentPort_->container (this);
        }

        else if (n == "resourceType")
        {
          resourceType_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          resourceType_->container (this);
        }

        else if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "property")
        {
          property_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (new ::CIAO::Config_Handlers::Property (e));
          property_->container (this);
        }

        else 
        {
        }
      }
    }

    // ComponentPackageReference
    //

    ComponentPackageReference::
    ComponentPackageReference (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "requiredUUID")
        {
          requiredUUID_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          requiredUUID_->container (this);
        }

        else if (n == "requiredName")
        {
          requiredName_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          requiredName_->container (this);
        }

        else if (n == "requiredType")
        {
          requiredType_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          requiredType_->container (this);
        }

        else 
        {
        }
      }
    }

    // SubcomponentPortEndpoint
    //

    SubcomponentPortEndpoint::
    SubcomponentPortEndpoint (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "portName")
        {
          portName_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          portName_->container (this);
        }

        else if (n == "instance")
        {
          instance_ = ::std::auto_ptr< ::XMLSchema::IDREF< char > > (new ::XMLSchema::IDREF< char > (e));
          instance_->container (this);
        }

        else 
        {
        }
      }
    }

    // AssemblyConnectionDescription
    //

    AssemblyConnectionDescription::
    AssemblyConnectionDescription (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "deployRequirement")
        {
          ::CIAO::Config_Handlers::Requirement t (e);
          deployRequirement (t);
        }

        else if (n == "externalEndpoint")
        {
          ::CIAO::Config_Handlers::ComponentExternalPortEndpoint t (e);
          add_externalEndpoint (t);
        }

        else if (n == "internalEndpoint")
        {
          ::CIAO::Config_Handlers::SubcomponentPortEndpoint t (e);
          add_internalEndpoint (t);
        }

        else if (n == "externalReference")
        {
          ::CIAO::Config_Handlers::ExternalReferenceEndpoint t (e);
          add_externalReference (t);
        }

        else 
        {
        }
      }
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
  }
}

#include <XSCRT/ExtendedTypeInfo.hpp>

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace
    {
      ::XMLSchema::TypeInfoInitializer < char > XMLSchemaTypeInfoInitializer_ (::XSCRT::extended_type_info_map ());

      struct TCKindTypeInfoInitializer
      {
        TCKindTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (TCKind));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));

          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      TCKindTypeInfoInitializer TCKindTypeInfoInitializer_;

      struct DataTypeTypeInfoInitializer
      {
        DataTypeTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (DataType));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      DataTypeTypeInfoInitializer DataTypeTypeInfoInitializer_;

      struct DataValueTypeInfoInitializer
      {
        DataValueTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (DataValue));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      DataValueTypeInfoInitializer DataValueTypeInfoInitializer_;

      struct AnyTypeInfoInitializer
      {
        AnyTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (Any));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      AnyTypeInfoInitializer AnyTypeInfoInitializer_;

      struct PropertyTypeInfoInitializer
      {
        PropertyTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (Property));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      PropertyTypeInfoInitializer PropertyTypeInfoInitializer_;

      struct NodeTypeInfoInitializer
      {
        NodeTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (Node));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      NodeTypeInfoInitializer NodeTypeInfoInitializer_;

      struct SatisfierPropertyKindTypeInfoInitializer
      {
        SatisfierPropertyKindTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (SatisfierPropertyKind));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));

          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      SatisfierPropertyKindTypeInfoInitializer SatisfierPropertyKindTypeInfoInitializer_;

      struct SatisfierPropertyTypeInfoInitializer
      {
        SatisfierPropertyTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (SatisfierProperty));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      SatisfierPropertyTypeInfoInitializer SatisfierPropertyTypeInfoInitializer_;

      struct ResourceTypeInfoInitializer
      {
        ResourceTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (Resource));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ResourceTypeInfoInitializer ResourceTypeInfoInitializer_;

      struct SharedResourceTypeInfoInitializer
      {
        SharedResourceTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (SharedResource));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      SharedResourceTypeInfoInitializer SharedResourceTypeInfoInitializer_;

      struct RequirementTypeInfoInitializer
      {
        RequirementTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (Requirement));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      RequirementTypeInfoInitializer RequirementTypeInfoInitializer_;

      struct ResourceDeploymentDescriptionTypeInfoInitializer
      {
        ResourceDeploymentDescriptionTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ResourceDeploymentDescription));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ResourceDeploymentDescriptionTypeInfoInitializer ResourceDeploymentDescriptionTypeInfoInitializer_;

      struct ArtifactDeploymentDescriptionTypeInfoInitializer
      {
        ArtifactDeploymentDescriptionTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ArtifactDeploymentDescription));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ArtifactDeploymentDescriptionTypeInfoInitializer ArtifactDeploymentDescriptionTypeInfoInitializer_;

      struct MonolithicDeploymentDescriptionTypeInfoInitializer
      {
        MonolithicDeploymentDescriptionTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (MonolithicDeploymentDescription));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      MonolithicDeploymentDescriptionTypeInfoInitializer MonolithicDeploymentDescriptionTypeInfoInitializer_;

      struct ResourceUsageKindTypeInfoInitializer
      {
        ResourceUsageKindTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ResourceUsageKind));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));

          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ResourceUsageKindTypeInfoInitializer ResourceUsageKindTypeInfoInitializer_;

      struct InstanceResourceDeploymentDescriptionTypeInfoInitializer
      {
        InstanceResourceDeploymentDescriptionTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (InstanceResourceDeploymentDescription));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      InstanceResourceDeploymentDescriptionTypeInfoInitializer InstanceResourceDeploymentDescriptionTypeInfoInitializer_;

      struct InstanceDeploymentDescriptionTypeInfoInitializer
      {
        InstanceDeploymentDescriptionTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (InstanceDeploymentDescription));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      InstanceDeploymentDescriptionTypeInfoInitializer InstanceDeploymentDescriptionTypeInfoInitializer_;

      struct CCMComponentPortKindTypeInfoInitializer
      {
        CCMComponentPortKindTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (CCMComponentPortKind));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));

          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      CCMComponentPortKindTypeInfoInitializer CCMComponentPortKindTypeInfoInitializer_;

      struct ComponentPortDescriptionTypeInfoInitializer
      {
        ComponentPortDescriptionTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ComponentPortDescription));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ComponentPortDescriptionTypeInfoInitializer ComponentPortDescriptionTypeInfoInitializer_;

      struct ComponentPropertyDescriptionTypeInfoInitializer
      {
        ComponentPropertyDescriptionTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ComponentPropertyDescription));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ComponentPropertyDescriptionTypeInfoInitializer ComponentPropertyDescriptionTypeInfoInitializer_;

      struct ComponentExternalPortEndpointTypeInfoInitializer
      {
        ComponentExternalPortEndpointTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ComponentExternalPortEndpoint));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ComponentExternalPortEndpointTypeInfoInitializer ComponentExternalPortEndpointTypeInfoInitializer_;

      struct PlanSubcomponentPortEndpointTypeInfoInitializer
      {
        PlanSubcomponentPortEndpointTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (PlanSubcomponentPortEndpoint));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      PlanSubcomponentPortEndpointTypeInfoInitializer PlanSubcomponentPortEndpointTypeInfoInitializer_;

      struct ExternalReferenceEndpointTypeInfoInitializer
      {
        ExternalReferenceEndpointTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ExternalReferenceEndpoint));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ExternalReferenceEndpointTypeInfoInitializer ExternalReferenceEndpointTypeInfoInitializer_;

      struct ConnectionResourceDeploymentDescriptionTypeInfoInitializer
      {
        ConnectionResourceDeploymentDescriptionTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ConnectionResourceDeploymentDescription));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ConnectionResourceDeploymentDescriptionTypeInfoInitializer ConnectionResourceDeploymentDescriptionTypeInfoInitializer_;

      struct PlanConnectionDescriptionTypeInfoInitializer
      {
        PlanConnectionDescriptionTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (PlanConnectionDescription));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      PlanConnectionDescriptionTypeInfoInitializer PlanConnectionDescriptionTypeInfoInitializer_;

      struct ImplementationDependencyTypeInfoInitializer
      {
        ImplementationDependencyTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ImplementationDependency));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ImplementationDependencyTypeInfoInitializer ImplementationDependencyTypeInfoInitializer_;

      struct CapabilityTypeInfoInitializer
      {
        CapabilityTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (Capability));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      CapabilityTypeInfoInitializer CapabilityTypeInfoInitializer_;

      struct ImplementationRequirementTypeInfoInitializer
      {
        ImplementationRequirementTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ImplementationRequirement));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ImplementationRequirementTypeInfoInitializer ImplementationRequirementTypeInfoInitializer_;

      struct ComponentPackageReferenceTypeInfoInitializer
      {
        ComponentPackageReferenceTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (ComponentPackageReference));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      ComponentPackageReferenceTypeInfoInitializer ComponentPackageReferenceTypeInfoInitializer_;

      struct SubcomponentPortEndpointTypeInfoInitializer
      {
        SubcomponentPortEndpointTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (SubcomponentPortEndpoint));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      SubcomponentPortEndpointTypeInfoInitializer SubcomponentPortEndpointTypeInfoInitializer_;

      struct AssemblyConnectionDescriptionTypeInfoInitializer
      {
        AssemblyConnectionDescriptionTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (AssemblyConnectionDescription));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      AssemblyConnectionDescriptionTypeInfoInitializer AssemblyConnectionDescriptionTypeInfoInitializer_;
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Traversal
    {
      // DataType
      //
      //

      void DataType::
      traverse (Type& o)
      {
        pre (o);
        if (o.kind_p ()) kind (o);
        else kind_none (o);
        post (o);
      }

      void DataType::
      traverse (Type const& o)
      {
        pre (o);
        if (o.kind_p ()) kind (o);
        else kind_none (o);
        post (o);
      }

      void DataType::
      pre (Type&)
      {
      }

      void DataType::
      pre (Type const&)
      {
      }

      void DataType::
      kind (Type& o)
      {
        dispatch (o.kind ());
      }

      void DataType::
      kind (Type const& o)
      {
        dispatch (o.kind ());
      }

      void DataType::
      kind_none (Type&)
      {
      }

      void DataType::
      kind_none (Type const&)
      {
      }

      void DataType::
      post (Type&)
      {
      }

      void DataType::
      post (Type const&)
      {
      }

      // DataValue
      //
      //

      void DataValue::
      traverse (Type& o)
      {
        pre (o);
        if (o.short_p ()) short_ (o);
        else short_none (o);
        if (o.long_p ()) long_ (o);
        else long_none (o);
        if (o.ushort_p ()) ushort (o);
        else ushort_none (o);
        if (o.ulong_p ()) ulong (o);
        else ulong_none (o);
        if (o.float_p ()) float_ (o);
        else float_none (o);
        if (o.double_p ()) double_ (o);
        else double_none (o);
        if (o.boolean_p ()) boolean (o);
        else boolean_none (o);
        if (o.octet_p ()) octet (o);
        else octet_none (o);
        if (o.objref_p ()) objref (o);
        else objref_none (o);
        if (o.enum_p ()) enum_ (o);
        else enum_none (o);
        if (o.string_p ()) string (o);
        else string_none (o);
        if (o.longlong_p ()) longlong (o);
        else longlong_none (o);
        if (o.ulonglong_p ()) ulonglong (o);
        else ulonglong_none (o);
        if (o.longdouble_p ()) longdouble (o);
        else longdouble_none (o);
        if (o.fixed_p ()) fixed (o);
        else fixed_none (o);
        if (o.typecode_p ()) typecode (o);
        else typecode_none (o);
        post (o);
      }

      void DataValue::
      traverse (Type const& o)
      {
        pre (o);
        if (o.short_p ()) short_ (o);
        else short_none (o);
        if (o.long_p ()) long_ (o);
        else long_none (o);
        if (o.ushort_p ()) ushort (o);
        else ushort_none (o);
        if (o.ulong_p ()) ulong (o);
        else ulong_none (o);
        if (o.float_p ()) float_ (o);
        else float_none (o);
        if (o.double_p ()) double_ (o);
        else double_none (o);
        if (o.boolean_p ()) boolean (o);
        else boolean_none (o);
        if (o.octet_p ()) octet (o);
        else octet_none (o);
        if (o.objref_p ()) objref (o);
        else objref_none (o);
        if (o.enum_p ()) enum_ (o);
        else enum_none (o);
        if (o.string_p ()) string (o);
        else string_none (o);
        if (o.longlong_p ()) longlong (o);
        else longlong_none (o);
        if (o.ulonglong_p ()) ulonglong (o);
        else ulonglong_none (o);
        if (o.longdouble_p ()) longdouble (o);
        else longdouble_none (o);
        if (o.fixed_p ()) fixed (o);
        else fixed_none (o);
        if (o.typecode_p ()) typecode (o);
        else typecode_none (o);
        post (o);
      }

      void DataValue::
      pre (Type&)
      {
      }

      void DataValue::
      pre (Type const&)
      {
      }

      void DataValue::
      short_ (Type& o)
      {
        dispatch (o.short_ ());
      }

      void DataValue::
      short_ (Type const& o)
      {
        dispatch (o.short_ ());
      }

      void DataValue::
      short_none (Type&)
      {
      }

      void DataValue::
      short_none (Type const&)
      {
      }

      void DataValue::
      long_ (Type& o)
      {
        dispatch (o.long_ ());
      }

      void DataValue::
      long_ (Type const& o)
      {
        dispatch (o.long_ ());
      }

      void DataValue::
      long_none (Type&)
      {
      }

      void DataValue::
      long_none (Type const&)
      {
      }

      void DataValue::
      ushort (Type& o)
      {
        dispatch (o.ushort ());
      }

      void DataValue::
      ushort (Type const& o)
      {
        dispatch (o.ushort ());
      }

      void DataValue::
      ushort_none (Type&)
      {
      }

      void DataValue::
      ushort_none (Type const&)
      {
      }

      void DataValue::
      ulong (Type& o)
      {
        dispatch (o.ulong ());
      }

      void DataValue::
      ulong (Type const& o)
      {
        dispatch (o.ulong ());
      }

      void DataValue::
      ulong_none (Type&)
      {
      }

      void DataValue::
      ulong_none (Type const&)
      {
      }

      void DataValue::
      float_ (Type& o)
      {
        dispatch (o.float_ ());
      }

      void DataValue::
      float_ (Type const& o)
      {
        dispatch (o.float_ ());
      }

      void DataValue::
      float_none (Type&)
      {
      }

      void DataValue::
      float_none (Type const&)
      {
      }

      void DataValue::
      double_ (Type& o)
      {
        dispatch (o.double_ ());
      }

      void DataValue::
      double_ (Type const& o)
      {
        dispatch (o.double_ ());
      }

      void DataValue::
      double_none (Type&)
      {
      }

      void DataValue::
      double_none (Type const&)
      {
      }

      void DataValue::
      boolean (Type& o)
      {
        dispatch (o.boolean ());
      }

      void DataValue::
      boolean (Type const& o)
      {
        dispatch (o.boolean ());
      }

      void DataValue::
      boolean_none (Type&)
      {
      }

      void DataValue::
      boolean_none (Type const&)
      {
      }

      void DataValue::
      octet (Type& o)
      {
        dispatch (o.octet ());
      }

      void DataValue::
      octet (Type const& o)
      {
        dispatch (o.octet ());
      }

      void DataValue::
      octet_none (Type&)
      {
      }

      void DataValue::
      octet_none (Type const&)
      {
      }

      void DataValue::
      objref (Type& o)
      {
        dispatch (o.objref ());
      }

      void DataValue::
      objref (Type const& o)
      {
        dispatch (o.objref ());
      }

      void DataValue::
      objref_none (Type&)
      {
      }

      void DataValue::
      objref_none (Type const&)
      {
      }

      void DataValue::
      enum_ (Type& o)
      {
        dispatch (o.enum_ ());
      }

      void DataValue::
      enum_ (Type const& o)
      {
        dispatch (o.enum_ ());
      }

      void DataValue::
      enum_none (Type&)
      {
      }

      void DataValue::
      enum_none (Type const&)
      {
      }

      void DataValue::
      string (Type& o)
      {
        dispatch (o.string ());
      }

      void DataValue::
      string (Type const& o)
      {
        dispatch (o.string ());
      }

      void DataValue::
      string_none (Type&)
      {
      }

      void DataValue::
      string_none (Type const&)
      {
      }

      void DataValue::
      longlong (Type& o)
      {
        dispatch (o.longlong ());
      }

      void DataValue::
      longlong (Type const& o)
      {
        dispatch (o.longlong ());
      }

      void DataValue::
      longlong_none (Type&)
      {
      }

      void DataValue::
      longlong_none (Type const&)
      {
      }

      void DataValue::
      ulonglong (Type& o)
      {
        dispatch (o.ulonglong ());
      }

      void DataValue::
      ulonglong (Type const& o)
      {
        dispatch (o.ulonglong ());
      }

      void DataValue::
      ulonglong_none (Type&)
      {
      }

      void DataValue::
      ulonglong_none (Type const&)
      {
      }

      void DataValue::
      longdouble (Type& o)
      {
        dispatch (o.longdouble ());
      }

      void DataValue::
      longdouble (Type const& o)
      {
        dispatch (o.longdouble ());
      }

      void DataValue::
      longdouble_none (Type&)
      {
      }

      void DataValue::
      longdouble_none (Type const&)
      {
      }

      void DataValue::
      fixed (Type& o)
      {
        dispatch (o.fixed ());
      }

      void DataValue::
      fixed (Type const& o)
      {
        dispatch (o.fixed ());
      }

      void DataValue::
      fixed_none (Type&)
      {
      }

      void DataValue::
      fixed_none (Type const&)
      {
      }

      void DataValue::
      typecode (Type& o)
      {
        dispatch (o.typecode ());
      }

      void DataValue::
      typecode (Type const& o)
      {
        dispatch (o.typecode ());
      }

      void DataValue::
      typecode_none (Type&)
      {
      }

      void DataValue::
      typecode_none (Type const&)
      {
      }

      void DataValue::
      post (Type&)
      {
      }

      void DataValue::
      post (Type const&)
      {
      }

      // Any
      //
      //

      void Any::
      traverse (Type& o)
      {
        pre (o);
        type (o);
        value (o);
        post (o);
      }

      void Any::
      traverse (Type const& o)
      {
        pre (o);
        type (o);
        value (o);
        post (o);
      }

      void Any::
      pre (Type&)
      {
      }

      void Any::
      pre (Type const&)
      {
      }

      void Any::
      type (Type& o)
      {
        dispatch (o.type ());
      }

      void Any::
      type (Type const& o)
      {
        dispatch (o.type ());
      }

      void Any::
      value (Type& o)
      {
        dispatch (o.value ());
      }

      void Any::
      value (Type const& o)
      {
        dispatch (o.value ());
      }

      void Any::
      post (Type&)
      {
      }

      void Any::
      post (Type const&)
      {
      }

      // Property
      //
      //

      void Property::
      traverse (Type& o)
      {
        pre (o);
        name (o);
        value (o);
        post (o);
      }

      void Property::
      traverse (Type const& o)
      {
        pre (o);
        name (o);
        value (o);
        post (o);
      }

      void Property::
      pre (Type&)
      {
      }

      void Property::
      pre (Type const&)
      {
      }

      void Property::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void Property::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void Property::
      value (Type& o)
      {
        dispatch (o.value ());
      }

      void Property::
      value (Type const& o)
      {
        dispatch (o.value ());
      }

      void Property::
      post (Type&)
      {
      }

      void Property::
      post (Type const&)
      {
      }

      // Node
      //
      //

      void Node::
      traverse (Type& o)
      {
        pre (o);
        name (o);
        label (o);
        post (o);
      }

      void Node::
      traverse (Type const& o)
      {
        pre (o);
        name (o);
        label (o);
        post (o);
      }

      void Node::
      pre (Type&)
      {
      }

      void Node::
      pre (Type const&)
      {
      }

      void Node::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void Node::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void Node::
      label (Type& o)
      {
        dispatch (o.label ());
      }

      void Node::
      label (Type const& o)
      {
        dispatch (o.label ());
      }

      void Node::
      post (Type&)
      {
      }

      void Node::
      post (Type const&)
      {
      }

      // SatisfierProperty
      //
      //

      void SatisfierProperty::
      traverse (Type& o)
      {
        pre (o);
        name (o);
        kind (o);
        value (o);
        post (o);
      }

      void SatisfierProperty::
      traverse (Type const& o)
      {
        pre (o);
        name (o);
        kind (o);
        value (o);
        post (o);
      }

      void SatisfierProperty::
      pre (Type&)
      {
      }

      void SatisfierProperty::
      pre (Type const&)
      {
      }

      void SatisfierProperty::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void SatisfierProperty::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void SatisfierProperty::
      kind (Type& o)
      {
        dispatch (o.kind ());
      }

      void SatisfierProperty::
      kind (Type const& o)
      {
        dispatch (o.kind ());
      }

      void SatisfierProperty::
      value (Type& o)
      {
        dispatch (o.value ());
      }

      void SatisfierProperty::
      value (Type const& o)
      {
        dispatch (o.value ());
      }

      void SatisfierProperty::
      post (Type&)
      {
      }

      void SatisfierProperty::
      post (Type const&)
      {
      }

      // Resource
      //
      //

      void Resource::
      traverse (Type& o)
      {
        pre (o);
        name (o);
        resourceType (o);
        property (o);
        post (o);
      }

      void Resource::
      traverse (Type const& o)
      {
        pre (o);
        name (o);
        resourceType (o);
        property (o);
        post (o);
      }

      void Resource::
      pre (Type&)
      {
      }

      void Resource::
      pre (Type const&)
      {
      }

      void Resource::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void Resource::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void Resource::
      resourceType (Type& o)
      {
        dispatch (o.resourceType ());
      }

      void Resource::
      resourceType (Type const& o)
      {
        dispatch (o.resourceType ());
      }

      void Resource::
      property (Type& o)
      {
        dispatch (o.property ());
      }

      void Resource::
      property (Type const& o)
      {
        dispatch (o.property ());
      }

      void Resource::
      post (Type&)
      {
      }

      void Resource::
      post (Type const&)
      {
      }

      // SharedResource
      //
      //

      void SharedResource::
      traverse (Type& o)
      {
        pre (o);
        name (o);
        resourceType (o);
        node (o);
        property (o);
        post (o);
      }

      void SharedResource::
      traverse (Type const& o)
      {
        pre (o);
        name (o);
        resourceType (o);
        node (o);
        property (o);
        post (o);
      }

      void SharedResource::
      pre (Type&)
      {
      }

      void SharedResource::
      pre (Type const&)
      {
      }

      void SharedResource::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void SharedResource::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void SharedResource::
      resourceType (Type& o)
      {
        dispatch (o.resourceType ());
      }

      void SharedResource::
      resourceType (Type const& o)
      {
        dispatch (o.resourceType ());
      }

      void SharedResource::
      node (Type& o)
      {
        dispatch (o.node ());
      }

      void SharedResource::
      node (Type const& o)
      {
        dispatch (o.node ());
      }

      void SharedResource::
      property (Type& o)
      {
        dispatch (o.property ());
      }

      void SharedResource::
      property (Type const& o)
      {
        dispatch (o.property ());
      }

      void SharedResource::
      post (Type&)
      {
      }

      void SharedResource::
      post (Type const&)
      {
      }

      // Requirement
      //
      //

      void Requirement::
      traverse (Type& o)
      {
        pre (o);
        resourceType (o);
        name (o);
        property (o);
        post (o);
      }

      void Requirement::
      traverse (Type const& o)
      {
        pre (o);
        resourceType (o);
        name (o);
        property (o);
        post (o);
      }

      void Requirement::
      pre (Type&)
      {
      }

      void Requirement::
      pre (Type const&)
      {
      }

      void Requirement::
      resourceType (Type& o)
      {
        dispatch (o.resourceType ());
      }

      void Requirement::
      resourceType (Type const& o)
      {
        dispatch (o.resourceType ());
      }

      void Requirement::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void Requirement::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void Requirement::
      property (Type& o)
      {
        dispatch (o.property ());
      }

      void Requirement::
      property (Type const& o)
      {
        dispatch (o.property ());
      }

      void Requirement::
      post (Type&)
      {
      }

      void Requirement::
      post (Type const&)
      {
      }

      // ResourceDeploymentDescription
      //
      //

      void ResourceDeploymentDescription::
      traverse (Type& o)
      {
        pre (o);
        requirementName (o);
        resourceName (o);
        resourceValue (o);
        post (o);
      }

      void ResourceDeploymentDescription::
      traverse (Type const& o)
      {
        pre (o);
        requirementName (o);
        resourceName (o);
        resourceValue (o);
        post (o);
      }

      void ResourceDeploymentDescription::
      pre (Type&)
      {
      }

      void ResourceDeploymentDescription::
      pre (Type const&)
      {
      }

      void ResourceDeploymentDescription::
      requirementName (Type& o)
      {
        dispatch (o.requirementName ());
      }

      void ResourceDeploymentDescription::
      requirementName (Type const& o)
      {
        dispatch (o.requirementName ());
      }

      void ResourceDeploymentDescription::
      resourceName (Type& o)
      {
        dispatch (o.resourceName ());
      }

      void ResourceDeploymentDescription::
      resourceName (Type const& o)
      {
        dispatch (o.resourceName ());
      }

      void ResourceDeploymentDescription::
      resourceValue (Type& o)
      {
        dispatch (o.resourceValue ());
      }

      void ResourceDeploymentDescription::
      resourceValue (Type const& o)
      {
        dispatch (o.resourceValue ());
      }

      void ResourceDeploymentDescription::
      post (Type&)
      {
      }

      void ResourceDeploymentDescription::
      post (Type const&)
      {
      }

      // ArtifactDeploymentDescription
      //
      //

      void ArtifactDeploymentDescription::
      traverse (Type& o)
      {
        pre (o);
        name (o);
        source (o);
        node (o);
        location (o);
        execParameter (o);
        deployRequirement (o);
        deployedResource (o);
        if (o.id_p ()) id (o);
        else id_none (o);
        post (o);
      }

      void ArtifactDeploymentDescription::
      traverse (Type const& o)
      {
        pre (o);
        name (o);
        source (o);
        node (o);
        location (o);
        execParameter (o);
        deployRequirement (o);
        deployedResource (o);
        if (o.id_p ()) id (o);
        else id_none (o);
        post (o);
      }

      void ArtifactDeploymentDescription::
      pre (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      pre (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void ArtifactDeploymentDescription::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void ArtifactDeploymentDescription::
      source (Type& o)
      {
        // VC6 anathema strikes again
        //
        ArtifactDeploymentDescription::Type::source_iterator b (o.begin_source()), e (o.end_source());

        if (b != e)
        {
          source_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) source_next (o);
          }

          source_post (o);
        }
      }

      void ArtifactDeploymentDescription::
      source (Type const& o)
      {
        // VC6 anathema strikes again
        //
        ArtifactDeploymentDescription::Type::source_const_iterator b (o.begin_source()), e (o.end_source());

        if (b != e)
        {
          source_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) source_next (o);
          }

          source_post (o);
        }
      }

      void ArtifactDeploymentDescription::
      source_pre (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      source_pre (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      source_next (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      source_next (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      source_post (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      source_post (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      node (Type& o)
      {
        dispatch (o.node ());
      }

      void ArtifactDeploymentDescription::
      node (Type const& o)
      {
        dispatch (o.node ());
      }

      void ArtifactDeploymentDescription::
      location (Type& o)
      {
        // VC6 anathema strikes again
        //
        ArtifactDeploymentDescription::Type::location_iterator b (o.begin_location()), e (o.end_location());

        if (b != e)
        {
          location_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) location_next (o);
          }

          location_post (o);
        }
      }

      void ArtifactDeploymentDescription::
      location (Type const& o)
      {
        // VC6 anathema strikes again
        //
        ArtifactDeploymentDescription::Type::location_const_iterator b (o.begin_location()), e (o.end_location());

        if (b != e)
        {
          location_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) location_next (o);
          }

          location_post (o);
        }
      }

      void ArtifactDeploymentDescription::
      location_pre (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      location_pre (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      location_next (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      location_next (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      location_post (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      location_post (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      execParameter (Type& o)
      {
        // VC6 anathema strikes again
        //
        ArtifactDeploymentDescription::Type::execParameter_iterator b (o.begin_execParameter()), e (o.end_execParameter());

        if (b != e)
        {
          execParameter_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) execParameter_next (o);
          }

          execParameter_post (o);
        }

        else execParameter_none (o);
      }

      void ArtifactDeploymentDescription::
      execParameter (Type const& o)
      {
        // VC6 anathema strikes again
        //
        ArtifactDeploymentDescription::Type::execParameter_const_iterator b (o.begin_execParameter()), e (o.end_execParameter());

        if (b != e)
        {
          execParameter_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) execParameter_next (o);
          }

          execParameter_post (o);
        }

        else execParameter_none (o);
      }

      void ArtifactDeploymentDescription::
      execParameter_pre (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      execParameter_pre (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      execParameter_next (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      execParameter_next (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      execParameter_post (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      execParameter_post (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      execParameter_none (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      execParameter_none (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      deployRequirement (Type& o)
      {
        // VC6 anathema strikes again
        //
        ArtifactDeploymentDescription::Type::deployRequirement_iterator b (o.begin_deployRequirement()), e (o.end_deployRequirement());

        if (b != e)
        {
          deployRequirement_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) deployRequirement_next (o);
          }

          deployRequirement_post (o);
        }

        else deployRequirement_none (o);
      }

      void ArtifactDeploymentDescription::
      deployRequirement (Type const& o)
      {
        // VC6 anathema strikes again
        //
        ArtifactDeploymentDescription::Type::deployRequirement_const_iterator b (o.begin_deployRequirement()), e (o.end_deployRequirement());

        if (b != e)
        {
          deployRequirement_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) deployRequirement_next (o);
          }

          deployRequirement_post (o);
        }

        else deployRequirement_none (o);
      }

      void ArtifactDeploymentDescription::
      deployRequirement_pre (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      deployRequirement_pre (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      deployRequirement_next (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      deployRequirement_next (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      deployRequirement_post (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      deployRequirement_post (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      deployRequirement_none (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      deployRequirement_none (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      deployedResource (Type& o)
      {
        // VC6 anathema strikes again
        //
        ArtifactDeploymentDescription::Type::deployedResource_iterator b (o.begin_deployedResource()), e (o.end_deployedResource());

        if (b != e)
        {
          deployedResource_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) deployedResource_next (o);
          }

          deployedResource_post (o);
        }

        else deployedResource_none (o);
      }

      void ArtifactDeploymentDescription::
      deployedResource (Type const& o)
      {
        // VC6 anathema strikes again
        //
        ArtifactDeploymentDescription::Type::deployedResource_const_iterator b (o.begin_deployedResource()), e (o.end_deployedResource());

        if (b != e)
        {
          deployedResource_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) deployedResource_next (o);
          }

          deployedResource_post (o);
        }

        else deployedResource_none (o);
      }

      void ArtifactDeploymentDescription::
      deployedResource_pre (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      deployedResource_pre (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      deployedResource_next (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      deployedResource_next (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      deployedResource_post (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      deployedResource_post (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      deployedResource_none (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      deployedResource_none (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      id (Type& o)
      {
        dispatch (o.id ());
      }

      void ArtifactDeploymentDescription::
      id (Type const& o)
      {
        dispatch (o.id ());
      }

      void ArtifactDeploymentDescription::
      id_none (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      id_none (Type const&)
      {
      }

      void ArtifactDeploymentDescription::
      post (Type&)
      {
      }

      void ArtifactDeploymentDescription::
      post (Type const&)
      {
      }

      // MonolithicDeploymentDescription
      //
      //

      void MonolithicDeploymentDescription::
      traverse (Type& o)
      {
        pre (o);
        name (o);
        source (o);
        artifact (o);
        execParameter (o);
        deployRequirement (o);
        if (o.id_p ()) id (o);
        else id_none (o);
        post (o);
      }

      void MonolithicDeploymentDescription::
      traverse (Type const& o)
      {
        pre (o);
        name (o);
        source (o);
        artifact (o);
        execParameter (o);
        deployRequirement (o);
        if (o.id_p ()) id (o);
        else id_none (o);
        post (o);
      }

      void MonolithicDeploymentDescription::
      pre (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      pre (Type const&)
      {
      }

      void MonolithicDeploymentDescription::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void MonolithicDeploymentDescription::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void MonolithicDeploymentDescription::
      source (Type& o)
      {
        // VC6 anathema strikes again
        //
        MonolithicDeploymentDescription::Type::source_iterator b (o.begin_source()), e (o.end_source());

        if (b != e)
        {
          source_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) source_next (o);
          }

          source_post (o);
        }
      }

      void MonolithicDeploymentDescription::
      source (Type const& o)
      {
        // VC6 anathema strikes again
        //
        MonolithicDeploymentDescription::Type::source_const_iterator b (o.begin_source()), e (o.end_source());

        if (b != e)
        {
          source_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) source_next (o);
          }

          source_post (o);
        }
      }

      void MonolithicDeploymentDescription::
      source_pre (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      source_pre (Type const&)
      {
      }

      void MonolithicDeploymentDescription::
      source_next (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      source_next (Type const&)
      {
      }

      void MonolithicDeploymentDescription::
      source_post (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      source_post (Type const&)
      {
      }

      void MonolithicDeploymentDescription::
      artifact (Type& o)
      {
        // VC6 anathema strikes again
        //
        MonolithicDeploymentDescription::Type::artifact_iterator b (o.begin_artifact()), e (o.end_artifact());

        if (b != e)
        {
          artifact_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) artifact_next (o);
          }

          artifact_post (o);
        }
      }

      void MonolithicDeploymentDescription::
      artifact (Type const& o)
      {
        // VC6 anathema strikes again
        //
        MonolithicDeploymentDescription::Type::artifact_const_iterator b (o.begin_artifact()), e (o.end_artifact());

        if (b != e)
        {
          artifact_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) artifact_next (o);
          }

          artifact_post (o);
        }
      }

      void MonolithicDeploymentDescription::
      artifact_pre (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      artifact_pre (Type const&)
      {
      }

      void MonolithicDeploymentDescription::
      artifact_next (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      artifact_next (Type const&)
      {
      }

      void MonolithicDeploymentDescription::
      artifact_post (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      artifact_post (Type const&)
      {
      }

      void MonolithicDeploymentDescription::
      execParameter (Type& o)
      {
        // VC6 anathema strikes again
        //
        MonolithicDeploymentDescription::Type::execParameter_iterator b (o.begin_execParameter()), e (o.end_execParameter());

        if (b != e)
        {
          execParameter_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) execParameter_next (o);
          }

          execParameter_post (o);
        }
      }

      void MonolithicDeploymentDescription::
      execParameter (Type const& o)
      {
        // VC6 anathema strikes again
        //
        MonolithicDeploymentDescription::Type::execParameter_const_iterator b (o.begin_execParameter()), e (o.end_execParameter());

        if (b != e)
        {
          execParameter_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) execParameter_next (o);
          }

          execParameter_post (o);
        }
      }

      void MonolithicDeploymentDescription::
      execParameter_pre (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      execParameter_pre (Type const&)
      {
      }

      void MonolithicDeploymentDescription::
      execParameter_next (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      execParameter_next (Type const&)
      {
      }

      void MonolithicDeploymentDescription::
      execParameter_post (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      execParameter_post (Type const&)
      {
      }

      void MonolithicDeploymentDescription::
      deployRequirement (Type& o)
      {
        // VC6 anathema strikes again
        //
        MonolithicDeploymentDescription::Type::deployRequirement_iterator b (o.begin_deployRequirement()), e (o.end_deployRequirement());

        if (b != e)
        {
          deployRequirement_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) deployRequirement_next (o);
          }

          deployRequirement_post (o);
        }
      }

      void MonolithicDeploymentDescription::
      deployRequirement (Type const& o)
      {
        // VC6 anathema strikes again
        //
        MonolithicDeploymentDescription::Type::deployRequirement_const_iterator b (o.begin_deployRequirement()), e (o.end_deployRequirement());

        if (b != e)
        {
          deployRequirement_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) deployRequirement_next (o);
          }

          deployRequirement_post (o);
        }
      }

      void MonolithicDeploymentDescription::
      deployRequirement_pre (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      deployRequirement_pre (Type const&)
      {
      }

      void MonolithicDeploymentDescription::
      deployRequirement_next (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      deployRequirement_next (Type const&)
      {
      }

      void MonolithicDeploymentDescription::
      deployRequirement_post (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      deployRequirement_post (Type const&)
      {
      }

      void MonolithicDeploymentDescription::
      id (Type& o)
      {
        dispatch (o.id ());
      }

      void MonolithicDeploymentDescription::
      id (Type const& o)
      {
        dispatch (o.id ());
      }

      void MonolithicDeploymentDescription::
      id_none (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      id_none (Type const&)
      {
      }

      void MonolithicDeploymentDescription::
      post (Type&)
      {
      }

      void MonolithicDeploymentDescription::
      post (Type const&)
      {
      }

      // InstanceResourceDeploymentDescription
      //
      //

      void InstanceResourceDeploymentDescription::
      traverse (Type& o)
      {
        pre (o);
        resourceUsage (o);
        requirementName (o);
        resourceName (o);
        resourceValue (o);
        post (o);
      }

      void InstanceResourceDeploymentDescription::
      traverse (Type const& o)
      {
        pre (o);
        resourceUsage (o);
        requirementName (o);
        resourceName (o);
        resourceValue (o);
        post (o);
      }

      void InstanceResourceDeploymentDescription::
      pre (Type&)
      {
      }

      void InstanceResourceDeploymentDescription::
      pre (Type const&)
      {
      }

      void InstanceResourceDeploymentDescription::
      resourceUsage (Type& o)
      {
        dispatch (o.resourceUsage ());
      }

      void InstanceResourceDeploymentDescription::
      resourceUsage (Type const& o)
      {
        dispatch (o.resourceUsage ());
      }

      void InstanceResourceDeploymentDescription::
      requirementName (Type& o)
      {
        dispatch (o.requirementName ());
      }

      void InstanceResourceDeploymentDescription::
      requirementName (Type const& o)
      {
        dispatch (o.requirementName ());
      }

      void InstanceResourceDeploymentDescription::
      resourceName (Type& o)
      {
        dispatch (o.resourceName ());
      }

      void InstanceResourceDeploymentDescription::
      resourceName (Type const& o)
      {
        dispatch (o.resourceName ());
      }

      void InstanceResourceDeploymentDescription::
      resourceValue (Type& o)
      {
        dispatch (o.resourceValue ());
      }

      void InstanceResourceDeploymentDescription::
      resourceValue (Type const& o)
      {
        dispatch (o.resourceValue ());
      }

      void InstanceResourceDeploymentDescription::
      post (Type&)
      {
      }

      void InstanceResourceDeploymentDescription::
      post (Type const&)
      {
      }

      // InstanceDeploymentDescription
      //
      //

      void InstanceDeploymentDescription::
      traverse (Type& o)
      {
        pre (o);
        name (o);
        node (o);
        source (o);
        implementation (o);
        configProperty (o);
        deployedResource (o);
        if (o.deployedSharedResource_p ()) deployedSharedResource (o);
        else deployedSharedResource_none (o);
        if (o.id_p ()) id (o);
        else id_none (o);
        post (o);
      }

      void InstanceDeploymentDescription::
      traverse (Type const& o)
      {
        pre (o);
        name (o);
        node (o);
        source (o);
        implementation (o);
        configProperty (o);
        deployedResource (o);
        if (o.deployedSharedResource_p ()) deployedSharedResource (o);
        else deployedSharedResource_none (o);
        if (o.id_p ()) id (o);
        else id_none (o);
        post (o);
      }

      void InstanceDeploymentDescription::
      pre (Type&)
      {
      }

      void InstanceDeploymentDescription::
      pre (Type const&)
      {
      }

      void InstanceDeploymentDescription::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void InstanceDeploymentDescription::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void InstanceDeploymentDescription::
      node (Type& o)
      {
        dispatch (o.node ());
      }

      void InstanceDeploymentDescription::
      node (Type const& o)
      {
        dispatch (o.node ());
      }

      void InstanceDeploymentDescription::
      source (Type& o)
      {
        dispatch (o.source ());
      }

      void InstanceDeploymentDescription::
      source (Type const& o)
      {
        dispatch (o.source ());
      }

      void InstanceDeploymentDescription::
      implementation (Type& o)
      {
        dispatch (o.implementation ());
      }

      void InstanceDeploymentDescription::
      implementation (Type const& o)
      {
        dispatch (o.implementation ());
      }

      void InstanceDeploymentDescription::
      configProperty (Type& o)
      {
        // VC6 anathema strikes again
        //
        InstanceDeploymentDescription::Type::configProperty_iterator b (o.begin_configProperty()), e (o.end_configProperty());

        if (b != e)
        {
          configProperty_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) configProperty_next (o);
          }

          configProperty_post (o);
        }

        else configProperty_none (o);
      }

      void InstanceDeploymentDescription::
      configProperty (Type const& o)
      {
        // VC6 anathema strikes again
        //
        InstanceDeploymentDescription::Type::configProperty_const_iterator b (o.begin_configProperty()), e (o.end_configProperty());

        if (b != e)
        {
          configProperty_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) configProperty_next (o);
          }

          configProperty_post (o);
        }

        else configProperty_none (o);
      }

      void InstanceDeploymentDescription::
      configProperty_pre (Type&)
      {
      }

      void InstanceDeploymentDescription::
      configProperty_pre (Type const&)
      {
      }

      void InstanceDeploymentDescription::
      configProperty_next (Type&)
      {
      }

      void InstanceDeploymentDescription::
      configProperty_next (Type const&)
      {
      }

      void InstanceDeploymentDescription::
      configProperty_post (Type&)
      {
      }

      void InstanceDeploymentDescription::
      configProperty_post (Type const&)
      {
      }

      void InstanceDeploymentDescription::
      configProperty_none (Type&)
      {
      }

      void InstanceDeploymentDescription::
      configProperty_none (Type const&)
      {
      }

      void InstanceDeploymentDescription::
      deployedResource (Type& o)
      {
        // VC6 anathema strikes again
        //
        InstanceDeploymentDescription::Type::deployedResource_iterator b (o.begin_deployedResource()), e (o.end_deployedResource());

        if (b != e)
        {
          deployedResource_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) deployedResource_next (o);
          }

          deployedResource_post (o);
        }

        else deployedResource_none (o);
      }

      void InstanceDeploymentDescription::
      deployedResource (Type const& o)
      {
        // VC6 anathema strikes again
        //
        InstanceDeploymentDescription::Type::deployedResource_const_iterator b (o.begin_deployedResource()), e (o.end_deployedResource());

        if (b != e)
        {
          deployedResource_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) deployedResource_next (o);
          }

          deployedResource_post (o);
        }

        else deployedResource_none (o);
      }

      void InstanceDeploymentDescription::
      deployedResource_pre (Type&)
      {
      }

      void InstanceDeploymentDescription::
      deployedResource_pre (Type const&)
      {
      }

      void InstanceDeploymentDescription::
      deployedResource_next (Type&)
      {
      }

      void InstanceDeploymentDescription::
      deployedResource_next (Type const&)
      {
      }

      void InstanceDeploymentDescription::
      deployedResource_post (Type&)
      {
      }

      void InstanceDeploymentDescription::
      deployedResource_post (Type const&)
      {
      }

      void InstanceDeploymentDescription::
      deployedResource_none (Type&)
      {
      }

      void InstanceDeploymentDescription::
      deployedResource_none (Type const&)
      {
      }

      void InstanceDeploymentDescription::
      deployedSharedResource (Type& o)
      {
        dispatch (o.deployedSharedResource ());
      }

      void InstanceDeploymentDescription::
      deployedSharedResource (Type const& o)
      {
        dispatch (o.deployedSharedResource ());
      }

      void InstanceDeploymentDescription::
      deployedSharedResource_none (Type&)
      {
      }

      void InstanceDeploymentDescription::
      deployedSharedResource_none (Type const&)
      {
      }

      void InstanceDeploymentDescription::
      id (Type& o)
      {
        dispatch (o.id ());
      }

      void InstanceDeploymentDescription::
      id (Type const& o)
      {
        dispatch (o.id ());
      }

      void InstanceDeploymentDescription::
      id_none (Type&)
      {
      }

      void InstanceDeploymentDescription::
      id_none (Type const&)
      {
      }

      void InstanceDeploymentDescription::
      post (Type&)
      {
      }

      void InstanceDeploymentDescription::
      post (Type const&)
      {
      }

      // ComponentPortDescription
      //
      //

      void ComponentPortDescription::
      traverse (Type& o)
      {
        pre (o);
        name (o);
        specificType (o);
        supportedType (o);
        provider (o);
        exclusiveProvider (o);
        exclusiveUser (o);
        optional (o);
        kind (o);
        post (o);
      }

      void ComponentPortDescription::
      traverse (Type const& o)
      {
        pre (o);
        name (o);
        specificType (o);
        supportedType (o);
        provider (o);
        exclusiveProvider (o);
        exclusiveUser (o);
        optional (o);
        kind (o);
        post (o);
      }

      void ComponentPortDescription::
      pre (Type&)
      {
      }

      void ComponentPortDescription::
      pre (Type const&)
      {
      }

      void ComponentPortDescription::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void ComponentPortDescription::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void ComponentPortDescription::
      specificType (Type& o)
      {
        dispatch (o.specificType ());
      }

      void ComponentPortDescription::
      specificType (Type const& o)
      {
        dispatch (o.specificType ());
      }

      void ComponentPortDescription::
      supportedType (Type& o)
      {
        dispatch (o.supportedType ());
      }

      void ComponentPortDescription::
      supportedType (Type const& o)
      {
        dispatch (o.supportedType ());
      }

      void ComponentPortDescription::
      provider (Type& o)
      {
        dispatch (o.provider ());
      }

      void ComponentPortDescription::
      provider (Type const& o)
      {
        dispatch (o.provider ());
      }

      void ComponentPortDescription::
      exclusiveProvider (Type& o)
      {
        dispatch (o.exclusiveProvider ());
      }

      void ComponentPortDescription::
      exclusiveProvider (Type const& o)
      {
        dispatch (o.exclusiveProvider ());
      }

      void ComponentPortDescription::
      exclusiveUser (Type& o)
      {
        dispatch (o.exclusiveUser ());
      }

      void ComponentPortDescription::
      exclusiveUser (Type const& o)
      {
        dispatch (o.exclusiveUser ());
      }

      void ComponentPortDescription::
      optional (Type& o)
      {
        dispatch (o.optional ());
      }

      void ComponentPortDescription::
      optional (Type const& o)
      {
        dispatch (o.optional ());
      }

      void ComponentPortDescription::
      kind (Type& o)
      {
        dispatch (o.kind ());
      }

      void ComponentPortDescription::
      kind (Type const& o)
      {
        dispatch (o.kind ());
      }

      void ComponentPortDescription::
      post (Type&)
      {
      }

      void ComponentPortDescription::
      post (Type const&)
      {
      }

      // ComponentPropertyDescription
      //
      //

      void ComponentPropertyDescription::
      traverse (Type& o)
      {
        pre (o);
        name (o);
        type (o);
        post (o);
      }

      void ComponentPropertyDescription::
      traverse (Type const& o)
      {
        pre (o);
        name (o);
        type (o);
        post (o);
      }

      void ComponentPropertyDescription::
      pre (Type&)
      {
      }

      void ComponentPropertyDescription::
      pre (Type const&)
      {
      }

      void ComponentPropertyDescription::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void ComponentPropertyDescription::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void ComponentPropertyDescription::
      type (Type& o)
      {
        dispatch (o.type ());
      }

      void ComponentPropertyDescription::
      type (Type const& o)
      {
        dispatch (o.type ());
      }

      void ComponentPropertyDescription::
      post (Type&)
      {
      }

      void ComponentPropertyDescription::
      post (Type const&)
      {
      }

      // ComponentExternalPortEndpoint
      //
      //

      void ComponentExternalPortEndpoint::
      traverse (Type& o)
      {
        pre (o);
        portName (o);
        post (o);
      }

      void ComponentExternalPortEndpoint::
      traverse (Type const& o)
      {
        pre (o);
        portName (o);
        post (o);
      }

      void ComponentExternalPortEndpoint::
      pre (Type&)
      {
      }

      void ComponentExternalPortEndpoint::
      pre (Type const&)
      {
      }

      void ComponentExternalPortEndpoint::
      portName (Type& o)
      {
        dispatch (o.portName ());
      }

      void ComponentExternalPortEndpoint::
      portName (Type const& o)
      {
        dispatch (o.portName ());
      }

      void ComponentExternalPortEndpoint::
      post (Type&)
      {
      }

      void ComponentExternalPortEndpoint::
      post (Type const&)
      {
      }

      // PlanSubcomponentPortEndpoint
      //
      //

      void PlanSubcomponentPortEndpoint::
      traverse (Type& o)
      {
        pre (o);
        portName (o);
        if (o.provider_p ()) provider (o);
        else provider_none (o);
        kind (o);
        instance (o);
        post (o);
      }

      void PlanSubcomponentPortEndpoint::
      traverse (Type const& o)
      {
        pre (o);
        portName (o);
        if (o.provider_p ()) provider (o);
        else provider_none (o);
        kind (o);
        instance (o);
        post (o);
      }

      void PlanSubcomponentPortEndpoint::
      pre (Type&)
      {
      }

      void PlanSubcomponentPortEndpoint::
      pre (Type const&)
      {
      }

      void PlanSubcomponentPortEndpoint::
      portName (Type& o)
      {
        dispatch (o.portName ());
      }

      void PlanSubcomponentPortEndpoint::
      portName (Type const& o)
      {
        dispatch (o.portName ());
      }

      void PlanSubcomponentPortEndpoint::
      provider (Type& o)
      {
        dispatch (o.provider ());
      }

      void PlanSubcomponentPortEndpoint::
      provider (Type const& o)
      {
        dispatch (o.provider ());
      }

      void PlanSubcomponentPortEndpoint::
      provider_none (Type&)
      {
      }

      void PlanSubcomponentPortEndpoint::
      provider_none (Type const&)
      {
      }

      void PlanSubcomponentPortEndpoint::
      kind (Type& o)
      {
        dispatch (o.kind ());
      }

      void PlanSubcomponentPortEndpoint::
      kind (Type const& o)
      {
        dispatch (o.kind ());
      }

      void PlanSubcomponentPortEndpoint::
      instance (Type& o)
      {
        dispatch (o.instance ());
      }

      void PlanSubcomponentPortEndpoint::
      instance (Type const& o)
      {
        dispatch (o.instance ());
      }

      void PlanSubcomponentPortEndpoint::
      post (Type&)
      {
      }

      void PlanSubcomponentPortEndpoint::
      post (Type const&)
      {
      }

      // ExternalReferenceEndpoint
      //
      //

      void ExternalReferenceEndpoint::
      traverse (Type& o)
      {
        pre (o);
        location (o);
        post (o);
      }

      void ExternalReferenceEndpoint::
      traverse (Type const& o)
      {
        pre (o);
        location (o);
        post (o);
      }

      void ExternalReferenceEndpoint::
      pre (Type&)
      {
      }

      void ExternalReferenceEndpoint::
      pre (Type const&)
      {
      }

      void ExternalReferenceEndpoint::
      location (Type& o)
      {
        dispatch (o.location ());
      }

      void ExternalReferenceEndpoint::
      location (Type const& o)
      {
        dispatch (o.location ());
      }

      void ExternalReferenceEndpoint::
      post (Type&)
      {
      }

      void ExternalReferenceEndpoint::
      post (Type const&)
      {
      }

      // ConnectionResourceDeploymentDescription
      //
      //

      void ConnectionResourceDeploymentDescription::
      traverse (Type& o)
      {
        pre (o);
        targetName (o);
        requirementName (o);
        resourceName (o);
        resourceValue (o);
        post (o);
      }

      void ConnectionResourceDeploymentDescription::
      traverse (Type const& o)
      {
        pre (o);
        targetName (o);
        requirementName (o);
        resourceName (o);
        resourceValue (o);
        post (o);
      }

      void ConnectionResourceDeploymentDescription::
      pre (Type&)
      {
      }

      void ConnectionResourceDeploymentDescription::
      pre (Type const&)
      {
      }

      void ConnectionResourceDeploymentDescription::
      targetName (Type& o)
      {
        dispatch (o.targetName ());
      }

      void ConnectionResourceDeploymentDescription::
      targetName (Type const& o)
      {
        dispatch (o.targetName ());
      }

      void ConnectionResourceDeploymentDescription::
      requirementName (Type& o)
      {
        dispatch (o.requirementName ());
      }

      void ConnectionResourceDeploymentDescription::
      requirementName (Type const& o)
      {
        dispatch (o.requirementName ());
      }

      void ConnectionResourceDeploymentDescription::
      resourceName (Type& o)
      {
        dispatch (o.resourceName ());
      }

      void ConnectionResourceDeploymentDescription::
      resourceName (Type const& o)
      {
        dispatch (o.resourceName ());
      }

      void ConnectionResourceDeploymentDescription::
      resourceValue (Type& o)
      {
        dispatch (o.resourceValue ());
      }

      void ConnectionResourceDeploymentDescription::
      resourceValue (Type const& o)
      {
        dispatch (o.resourceValue ());
      }

      void ConnectionResourceDeploymentDescription::
      post (Type&)
      {
      }

      void ConnectionResourceDeploymentDescription::
      post (Type const&)
      {
      }

      // PlanConnectionDescription
      //
      //

      void PlanConnectionDescription::
      traverse (Type& o)
      {
        pre (o);
        name (o);
        if (o.source_p ()) source (o);
        else source_none (o);
        deployRequirement (o);
        externalEndpoint (o);
        internalEndpoint (o);
        externalReference (o);
        deployedResource (o);
        post (o);
      }

      void PlanConnectionDescription::
      traverse (Type const& o)
      {
        pre (o);
        name (o);
        if (o.source_p ()) source (o);
        else source_none (o);
        deployRequirement (o);
        externalEndpoint (o);
        internalEndpoint (o);
        externalReference (o);
        deployedResource (o);
        post (o);
      }

      void PlanConnectionDescription::
      pre (Type&)
      {
      }

      void PlanConnectionDescription::
      pre (Type const&)
      {
      }

      void PlanConnectionDescription::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void PlanConnectionDescription::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void PlanConnectionDescription::
      source (Type& o)
      {
        dispatch (o.source ());
      }

      void PlanConnectionDescription::
      source (Type const& o)
      {
        dispatch (o.source ());
      }

      void PlanConnectionDescription::
      source_none (Type&)
      {
      }

      void PlanConnectionDescription::
      source_none (Type const&)
      {
      }

      void PlanConnectionDescription::
      deployRequirement (Type& o)
      {
        // VC6 anathema strikes again
        //
        PlanConnectionDescription::Type::deployRequirement_iterator b (o.begin_deployRequirement()), e (o.end_deployRequirement());

        if (b != e)
        {
          deployRequirement_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) deployRequirement_next (o);
          }

          deployRequirement_post (o);
        }

        else deployRequirement_none (o);
      }

      void PlanConnectionDescription::
      deployRequirement (Type const& o)
      {
        // VC6 anathema strikes again
        //
        PlanConnectionDescription::Type::deployRequirement_const_iterator b (o.begin_deployRequirement()), e (o.end_deployRequirement());

        if (b != e)
        {
          deployRequirement_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) deployRequirement_next (o);
          }

          deployRequirement_post (o);
        }

        else deployRequirement_none (o);
      }

      void PlanConnectionDescription::
      deployRequirement_pre (Type&)
      {
      }

      void PlanConnectionDescription::
      deployRequirement_pre (Type const&)
      {
      }

      void PlanConnectionDescription::
      deployRequirement_next (Type&)
      {
      }

      void PlanConnectionDescription::
      deployRequirement_next (Type const&)
      {
      }

      void PlanConnectionDescription::
      deployRequirement_post (Type&)
      {
      }

      void PlanConnectionDescription::
      deployRequirement_post (Type const&)
      {
      }

      void PlanConnectionDescription::
      deployRequirement_none (Type&)
      {
      }

      void PlanConnectionDescription::
      deployRequirement_none (Type const&)
      {
      }

      void PlanConnectionDescription::
      externalEndpoint (Type& o)
      {
        // VC6 anathema strikes again
        //
        PlanConnectionDescription::Type::externalEndpoint_iterator b (o.begin_externalEndpoint()), e (o.end_externalEndpoint());

        if (b != e)
        {
          externalEndpoint_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) externalEndpoint_next (o);
          }

          externalEndpoint_post (o);
        }

        else externalEndpoint_none (o);
      }

      void PlanConnectionDescription::
      externalEndpoint (Type const& o)
      {
        // VC6 anathema strikes again
        //
        PlanConnectionDescription::Type::externalEndpoint_const_iterator b (o.begin_externalEndpoint()), e (o.end_externalEndpoint());

        if (b != e)
        {
          externalEndpoint_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) externalEndpoint_next (o);
          }

          externalEndpoint_post (o);
        }

        else externalEndpoint_none (o);
      }

      void PlanConnectionDescription::
      externalEndpoint_pre (Type&)
      {
      }

      void PlanConnectionDescription::
      externalEndpoint_pre (Type const&)
      {
      }

      void PlanConnectionDescription::
      externalEndpoint_next (Type&)
      {
      }

      void PlanConnectionDescription::
      externalEndpoint_next (Type const&)
      {
      }

      void PlanConnectionDescription::
      externalEndpoint_post (Type&)
      {
      }

      void PlanConnectionDescription::
      externalEndpoint_post (Type const&)
      {
      }

      void PlanConnectionDescription::
      externalEndpoint_none (Type&)
      {
      }

      void PlanConnectionDescription::
      externalEndpoint_none (Type const&)
      {
      }

      void PlanConnectionDescription::
      internalEndpoint (Type& o)
      {
        // VC6 anathema strikes again
        //
        PlanConnectionDescription::Type::internalEndpoint_iterator b (o.begin_internalEndpoint()), e (o.end_internalEndpoint());

        if (b != e)
        {
          internalEndpoint_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) internalEndpoint_next (o);
          }

          internalEndpoint_post (o);
        }

        else internalEndpoint_none (o);
      }

      void PlanConnectionDescription::
      internalEndpoint (Type const& o)
      {
        // VC6 anathema strikes again
        //
        PlanConnectionDescription::Type::internalEndpoint_const_iterator b (o.begin_internalEndpoint()), e (o.end_internalEndpoint());

        if (b != e)
        {
          internalEndpoint_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) internalEndpoint_next (o);
          }

          internalEndpoint_post (o);
        }

        else internalEndpoint_none (o);
      }

      void PlanConnectionDescription::
      internalEndpoint_pre (Type&)
      {
      }

      void PlanConnectionDescription::
      internalEndpoint_pre (Type const&)
      {
      }

      void PlanConnectionDescription::
      internalEndpoint_next (Type&)
      {
      }

      void PlanConnectionDescription::
      internalEndpoint_next (Type const&)
      {
      }

      void PlanConnectionDescription::
      internalEndpoint_post (Type&)
      {
      }

      void PlanConnectionDescription::
      internalEndpoint_post (Type const&)
      {
      }

      void PlanConnectionDescription::
      internalEndpoint_none (Type&)
      {
      }

      void PlanConnectionDescription::
      internalEndpoint_none (Type const&)
      {
      }

      void PlanConnectionDescription::
      externalReference (Type& o)
      {
        // VC6 anathema strikes again
        //
        PlanConnectionDescription::Type::externalReference_iterator b (o.begin_externalReference()), e (o.end_externalReference());

        if (b != e)
        {
          externalReference_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) externalReference_next (o);
          }

          externalReference_post (o);
        }

        else externalReference_none (o);
      }

      void PlanConnectionDescription::
      externalReference (Type const& o)
      {
        // VC6 anathema strikes again
        //
        PlanConnectionDescription::Type::externalReference_const_iterator b (o.begin_externalReference()), e (o.end_externalReference());

        if (b != e)
        {
          externalReference_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) externalReference_next (o);
          }

          externalReference_post (o);
        }

        else externalReference_none (o);
      }

      void PlanConnectionDescription::
      externalReference_pre (Type&)
      {
      }

      void PlanConnectionDescription::
      externalReference_pre (Type const&)
      {
      }

      void PlanConnectionDescription::
      externalReference_next (Type&)
      {
      }

      void PlanConnectionDescription::
      externalReference_next (Type const&)
      {
      }

      void PlanConnectionDescription::
      externalReference_post (Type&)
      {
      }

      void PlanConnectionDescription::
      externalReference_post (Type const&)
      {
      }

      void PlanConnectionDescription::
      externalReference_none (Type&)
      {
      }

      void PlanConnectionDescription::
      externalReference_none (Type const&)
      {
      }

      void PlanConnectionDescription::
      deployedResource (Type& o)
      {
        // VC6 anathema strikes again
        //
        PlanConnectionDescription::Type::deployedResource_iterator b (o.begin_deployedResource()), e (o.end_deployedResource());

        if (b != e)
        {
          deployedResource_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) deployedResource_next (o);
          }

          deployedResource_post (o);
        }

        else deployedResource_none (o);
      }

      void PlanConnectionDescription::
      deployedResource (Type const& o)
      {
        // VC6 anathema strikes again
        //
        PlanConnectionDescription::Type::deployedResource_const_iterator b (o.begin_deployedResource()), e (o.end_deployedResource());

        if (b != e)
        {
          deployedResource_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) deployedResource_next (o);
          }

          deployedResource_post (o);
        }

        else deployedResource_none (o);
      }

      void PlanConnectionDescription::
      deployedResource_pre (Type&)
      {
      }

      void PlanConnectionDescription::
      deployedResource_pre (Type const&)
      {
      }

      void PlanConnectionDescription::
      deployedResource_next (Type&)
      {
      }

      void PlanConnectionDescription::
      deployedResource_next (Type const&)
      {
      }

      void PlanConnectionDescription::
      deployedResource_post (Type&)
      {
      }

      void PlanConnectionDescription::
      deployedResource_post (Type const&)
      {
      }

      void PlanConnectionDescription::
      deployedResource_none (Type&)
      {
      }

      void PlanConnectionDescription::
      deployedResource_none (Type const&)
      {
      }

      void PlanConnectionDescription::
      post (Type&)
      {
      }

      void PlanConnectionDescription::
      post (Type const&)
      {
      }

      // ImplementationDependency
      //
      //

      void ImplementationDependency::
      traverse (Type& o)
      {
        pre (o);
        requiredType (o);
        post (o);
      }

      void ImplementationDependency::
      traverse (Type const& o)
      {
        pre (o);
        requiredType (o);
        post (o);
      }

      void ImplementationDependency::
      pre (Type&)
      {
      }

      void ImplementationDependency::
      pre (Type const&)
      {
      }

      void ImplementationDependency::
      requiredType (Type& o)
      {
        dispatch (o.requiredType ());
      }

      void ImplementationDependency::
      requiredType (Type const& o)
      {
        dispatch (o.requiredType ());
      }

      void ImplementationDependency::
      post (Type&)
      {
      }

      void ImplementationDependency::
      post (Type const&)
      {
      }

      // Capability
      //
      //

      void Capability::
      traverse (Type& o)
      {
        pre (o);
        name (o);
        resourceType (o);
        property (o);
        post (o);
      }

      void Capability::
      traverse (Type const& o)
      {
        pre (o);
        name (o);
        resourceType (o);
        property (o);
        post (o);
      }

      void Capability::
      pre (Type&)
      {
      }

      void Capability::
      pre (Type const&)
      {
      }

      void Capability::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void Capability::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void Capability::
      resourceType (Type& o)
      {
        dispatch (o.resourceType ());
      }

      void Capability::
      resourceType (Type const& o)
      {
        dispatch (o.resourceType ());
      }

      void Capability::
      property (Type& o)
      {
        dispatch (o.property ());
      }

      void Capability::
      property (Type const& o)
      {
        dispatch (o.property ());
      }

      void Capability::
      post (Type&)
      {
      }

      void Capability::
      post (Type const&)
      {
      }

      // ImplementationRequirement
      //
      //

      void ImplementationRequirement::
      traverse (Type& o)
      {
        pre (o);
        resourceUsage (o);
        resourcePort (o);
        componentPort (o);
        resourceType (o);
        name (o);
        property (o);
        post (o);
      }

      void ImplementationRequirement::
      traverse (Type const& o)
      {
        pre (o);
        resourceUsage (o);
        resourcePort (o);
        componentPort (o);
        resourceType (o);
        name (o);
        property (o);
        post (o);
      }

      void ImplementationRequirement::
      pre (Type&)
      {
      }

      void ImplementationRequirement::
      pre (Type const&)
      {
      }

      void ImplementationRequirement::
      resourceUsage (Type& o)
      {
        dispatch (o.resourceUsage ());
      }

      void ImplementationRequirement::
      resourceUsage (Type const& o)
      {
        dispatch (o.resourceUsage ());
      }

      void ImplementationRequirement::
      resourcePort (Type& o)
      {
        dispatch (o.resourcePort ());
      }

      void ImplementationRequirement::
      resourcePort (Type const& o)
      {
        dispatch (o.resourcePort ());
      }

      void ImplementationRequirement::
      componentPort (Type& o)
      {
        dispatch (o.componentPort ());
      }

      void ImplementationRequirement::
      componentPort (Type const& o)
      {
        dispatch (o.componentPort ());
      }

      void ImplementationRequirement::
      resourceType (Type& o)
      {
        dispatch (o.resourceType ());
      }

      void ImplementationRequirement::
      resourceType (Type const& o)
      {
        dispatch (o.resourceType ());
      }

      void ImplementationRequirement::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void ImplementationRequirement::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void ImplementationRequirement::
      property (Type& o)
      {
        dispatch (o.property ());
      }

      void ImplementationRequirement::
      property (Type const& o)
      {
        dispatch (o.property ());
      }

      void ImplementationRequirement::
      post (Type&)
      {
      }

      void ImplementationRequirement::
      post (Type const&)
      {
      }

      // ComponentPackageReference
      //
      //

      void ComponentPackageReference::
      traverse (Type& o)
      {
        pre (o);
        requiredUUID (o);
        requiredName (o);
        requiredType (o);
        post (o);
      }

      void ComponentPackageReference::
      traverse (Type const& o)
      {
        pre (o);
        requiredUUID (o);
        requiredName (o);
        requiredType (o);
        post (o);
      }

      void ComponentPackageReference::
      pre (Type&)
      {
      }

      void ComponentPackageReference::
      pre (Type const&)
      {
      }

      void ComponentPackageReference::
      requiredUUID (Type& o)
      {
        dispatch (o.requiredUUID ());
      }

      void ComponentPackageReference::
      requiredUUID (Type const& o)
      {
        dispatch (o.requiredUUID ());
      }

      void ComponentPackageReference::
      requiredName (Type& o)
      {
        dispatch (o.requiredName ());
      }

      void ComponentPackageReference::
      requiredName (Type const& o)
      {
        dispatch (o.requiredName ());
      }

      void ComponentPackageReference::
      requiredType (Type& o)
      {
        dispatch (o.requiredType ());
      }

      void ComponentPackageReference::
      requiredType (Type const& o)
      {
        dispatch (o.requiredType ());
      }

      void ComponentPackageReference::
      post (Type&)
      {
      }

      void ComponentPackageReference::
      post (Type const&)
      {
      }

      // SubcomponentPortEndpoint
      //
      //

      void SubcomponentPortEndpoint::
      traverse (Type& o)
      {
        pre (o);
        portName (o);
        instance (o);
        post (o);
      }

      void SubcomponentPortEndpoint::
      traverse (Type const& o)
      {
        pre (o);
        portName (o);
        instance (o);
        post (o);
      }

      void SubcomponentPortEndpoint::
      pre (Type&)
      {
      }

      void SubcomponentPortEndpoint::
      pre (Type const&)
      {
      }

      void SubcomponentPortEndpoint::
      portName (Type& o)
      {
        dispatch (o.portName ());
      }

      void SubcomponentPortEndpoint::
      portName (Type const& o)
      {
        dispatch (o.portName ());
      }

      void SubcomponentPortEndpoint::
      instance (Type& o)
      {
        dispatch (o.instance ());
      }

      void SubcomponentPortEndpoint::
      instance (Type const& o)
      {
        dispatch (o.instance ());
      }

      void SubcomponentPortEndpoint::
      post (Type&)
      {
      }

      void SubcomponentPortEndpoint::
      post (Type const&)
      {
      }

      // AssemblyConnectionDescription
      //
      //

      void AssemblyConnectionDescription::
      traverse (Type& o)
      {
        pre (o);
        name (o);
        if (o.deployRequirement_p ()) deployRequirement (o);
        else deployRequirement_none (o);
        externalEndpoint (o);
        internalEndpoint (o);
        externalReference (o);
        post (o);
      }

      void AssemblyConnectionDescription::
      traverse (Type const& o)
      {
        pre (o);
        name (o);
        if (o.deployRequirement_p ()) deployRequirement (o);
        else deployRequirement_none (o);
        externalEndpoint (o);
        internalEndpoint (o);
        externalReference (o);
        post (o);
      }

      void AssemblyConnectionDescription::
      pre (Type&)
      {
      }

      void AssemblyConnectionDescription::
      pre (Type const&)
      {
      }

      void AssemblyConnectionDescription::
      name (Type& o)
      {
        dispatch (o.name ());
      }

      void AssemblyConnectionDescription::
      name (Type const& o)
      {
        dispatch (o.name ());
      }

      void AssemblyConnectionDescription::
      deployRequirement (Type& o)
      {
        dispatch (o.deployRequirement ());
      }

      void AssemblyConnectionDescription::
      deployRequirement (Type const& o)
      {
        dispatch (o.deployRequirement ());
      }

      void AssemblyConnectionDescription::
      deployRequirement_none (Type&)
      {
      }

      void AssemblyConnectionDescription::
      deployRequirement_none (Type const&)
      {
      }

      void AssemblyConnectionDescription::
      externalEndpoint (Type& o)
      {
        // VC6 anathema strikes again
        //
        AssemblyConnectionDescription::Type::externalEndpoint_iterator b (o.begin_externalEndpoint()), e (o.end_externalEndpoint());

        if (b != e)
        {
          externalEndpoint_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) externalEndpoint_next (o);
          }

          externalEndpoint_post (o);
        }

        else externalEndpoint_none (o);
      }

      void AssemblyConnectionDescription::
      externalEndpoint (Type const& o)
      {
        // VC6 anathema strikes again
        //
        AssemblyConnectionDescription::Type::externalEndpoint_const_iterator b (o.begin_externalEndpoint()), e (o.end_externalEndpoint());

        if (b != e)
        {
          externalEndpoint_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) externalEndpoint_next (o);
          }

          externalEndpoint_post (o);
        }

        else externalEndpoint_none (o);
      }

      void AssemblyConnectionDescription::
      externalEndpoint_pre (Type&)
      {
      }

      void AssemblyConnectionDescription::
      externalEndpoint_pre (Type const&)
      {
      }

      void AssemblyConnectionDescription::
      externalEndpoint_next (Type&)
      {
      }

      void AssemblyConnectionDescription::
      externalEndpoint_next (Type const&)
      {
      }

      void AssemblyConnectionDescription::
      externalEndpoint_post (Type&)
      {
      }

      void AssemblyConnectionDescription::
      externalEndpoint_post (Type const&)
      {
      }

      void AssemblyConnectionDescription::
      externalEndpoint_none (Type&)
      {
      }

      void AssemblyConnectionDescription::
      externalEndpoint_none (Type const&)
      {
      }

      void AssemblyConnectionDescription::
      internalEndpoint (Type& o)
      {
        // VC6 anathema strikes again
        //
        AssemblyConnectionDescription::Type::internalEndpoint_iterator b (o.begin_internalEndpoint()), e (o.end_internalEndpoint());

        if (b != e)
        {
          internalEndpoint_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) internalEndpoint_next (o);
          }

          internalEndpoint_post (o);
        }

        else internalEndpoint_none (o);
      }

      void AssemblyConnectionDescription::
      internalEndpoint (Type const& o)
      {
        // VC6 anathema strikes again
        //
        AssemblyConnectionDescription::Type::internalEndpoint_const_iterator b (o.begin_internalEndpoint()), e (o.end_internalEndpoint());

        if (b != e)
        {
          internalEndpoint_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) internalEndpoint_next (o);
          }

          internalEndpoint_post (o);
        }

        else internalEndpoint_none (o);
      }

      void AssemblyConnectionDescription::
      internalEndpoint_pre (Type&)
      {
      }

      void AssemblyConnectionDescription::
      internalEndpoint_pre (Type const&)
      {
      }

      void AssemblyConnectionDescription::
      internalEndpoint_next (Type&)
      {
      }

      void AssemblyConnectionDescription::
      internalEndpoint_next (Type const&)
      {
      }

      void AssemblyConnectionDescription::
      internalEndpoint_post (Type&)
      {
      }

      void AssemblyConnectionDescription::
      internalEndpoint_post (Type const&)
      {
      }

      void AssemblyConnectionDescription::
      internalEndpoint_none (Type&)
      {
      }

      void AssemblyConnectionDescription::
      internalEndpoint_none (Type const&)
      {
      }

      void AssemblyConnectionDescription::
      externalReference (Type& o)
      {
        // VC6 anathema strikes again
        //
        AssemblyConnectionDescription::Type::externalReference_iterator b (o.begin_externalReference()), e (o.end_externalReference());

        if (b != e)
        {
          externalReference_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) externalReference_next (o);
          }

          externalReference_post (o);
        }

        else externalReference_none (o);
      }

      void AssemblyConnectionDescription::
      externalReference (Type const& o)
      {
        // VC6 anathema strikes again
        //
        AssemblyConnectionDescription::Type::externalReference_const_iterator b (o.begin_externalReference()), e (o.end_externalReference());

        if (b != e)
        {
          externalReference_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) externalReference_next (o);
          }

          externalReference_post (o);
        }

        else externalReference_none (o);
      }

      void AssemblyConnectionDescription::
      externalReference_pre (Type&)
      {
      }

      void AssemblyConnectionDescription::
      externalReference_pre (Type const&)
      {
      }

      void AssemblyConnectionDescription::
      externalReference_next (Type&)
      {
      }

      void AssemblyConnectionDescription::
      externalReference_next (Type const&)
      {
      }

      void AssemblyConnectionDescription::
      externalReference_post (Type&)
      {
      }

      void AssemblyConnectionDescription::
      externalReference_post (Type const&)
      {
      }

      void AssemblyConnectionDescription::
      externalReference_none (Type&)
      {
      }

      void AssemblyConnectionDescription::
      externalReference_none (Type const&)
      {
      }

      void AssemblyConnectionDescription::
      post (Type&)
      {
      }

      void AssemblyConnectionDescription::
      post (Type const&)
      {
      }
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Writer
    {
      // TCKind
      //
      //

      TCKind::
      TCKind (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      TCKind::
      TCKind ()
      {
      }

      void TCKind::
      traverse (Type const& o)
      {
        ::std::basic_string< char > s;

        if (o == ::CIAO::Config_Handlers::TCKind::tk_null) s = "tk_null";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_void) s = "tk_void";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_short) s = "tk_short";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_long) s = "tk_long";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_ushort) s = "tk_ushort";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_ulong) s = "tk_ulong";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_float) s = "tk_float";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_double) s = "tk_double";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_boolean) s = "tk_boolean";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_char) s = "tk_char";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_octet) s = "tk_octet";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_any) s = "tk_any";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_TypeCode) s = "tk_TypeCode";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_Principal) s = "tk_Principal";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_objref) s = "tk_objref";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_struct) s = "tk_struct";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_union) s = "tk_union";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_enum) s = "tk_enum";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_string) s = "tk_string";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_sequence) s = "tk_sequence";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_array) s = "tk_array";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_alias) s = "tk_alias";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_except) s = "tk_except";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_longlong) s = "tk_longlong";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_ulonglong) s = "tk_ulonglong";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_longdouble) s = "tk_longdouble";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_wchar) s = "tk_wchar";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_wstring) s = "tk_wstring";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_wfixed) s = "tk_wfixed";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_value) s = "tk_value";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_value_box) s = "tk_value_box";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_native) s = "tk_native";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_abstract_interface) s = "tk_abstract_interface";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_local_interface) s = "tk_local_interface";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_component) s = "tk_component";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_home) s = "tk_home";
        else if (o == ::CIAO::Config_Handlers::TCKind::tk_event) s = "tk_event";
        else 
        {
        }

        if (::XSCRT::XML::Attribute< char >* a = attr_ ())
        {
          a->value (s);
        }

        else
        {
          top_().value (s);
        }
      }

      // DataType
      //
      //

      DataType::
      DataType (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      DataType::
      DataType ()
      {
      }

      void DataType::
      traverse (Type const& o)
      {
        Traversal::DataType::traverse (o);
      }

      void DataType::
      kind (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("kind", top_ ()));
        Traversal::DataType::kind (o);
        pop_ ();
      }

      // DataValue
      //
      //

      DataValue::
      DataValue (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      DataValue::
      DataValue ()
      {
      }

      void DataValue::
      traverse (Type const& o)
      {
        Traversal::DataValue::traverse (o);
      }

      void DataValue::
      short_ (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("short", top_ ()));
        Traversal::DataValue::short_ (o);
        pop_ ();
      }

      void DataValue::
      long_ (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("long", top_ ()));
        Traversal::DataValue::long_ (o);
        pop_ ();
      }

      void DataValue::
      ushort (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("ushort", top_ ()));
        Traversal::DataValue::ushort (o);
        pop_ ();
      }

      void DataValue::
      ulong (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("ulong", top_ ()));
        Traversal::DataValue::ulong (o);
        pop_ ();
      }

      void DataValue::
      float_ (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("float", top_ ()));
        Traversal::DataValue::float_ (o);
        pop_ ();
      }

      void DataValue::
      double_ (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("double", top_ ()));
        Traversal::DataValue::double_ (o);
        pop_ ();
      }

      void DataValue::
      boolean (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("boolean", top_ ()));
        Traversal::DataValue::boolean (o);
        pop_ ();
      }

      void DataValue::
      octet (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("octet", top_ ()));
        Traversal::DataValue::octet (o);
        pop_ ();
      }

      void DataValue::
      objref (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("objref", top_ ()));
        Traversal::DataValue::objref (o);
        pop_ ();
      }

      void DataValue::
      enum_ (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("enum", top_ ()));
        Traversal::DataValue::enum_ (o);
        pop_ ();
      }

      void DataValue::
      string (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("string", top_ ()));
        Traversal::DataValue::string (o);
        pop_ ();
      }

      void DataValue::
      longlong (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("longlong", top_ ()));
        Traversal::DataValue::longlong (o);
        pop_ ();
      }

      void DataValue::
      ulonglong (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("ulonglong", top_ ()));
        Traversal::DataValue::ulonglong (o);
        pop_ ();
      }

      void DataValue::
      longdouble (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("longdouble", top_ ()));
        Traversal::DataValue::longdouble (o);
        pop_ ();
      }

      void DataValue::
      fixed (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("fixed", top_ ()));
        Traversal::DataValue::fixed (o);
        pop_ ();
      }

      void DataValue::
      typecode (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("typecode", top_ ()));
        Traversal::DataValue::typecode (o);
        pop_ ();
      }

      // Any
      //
      //

      Any::
      Any (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      Any::
      Any ()
      {
      }

      void Any::
      traverse (Type const& o)
      {
        Traversal::Any::traverse (o);
      }

      void Any::
      type (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("type", top_ ()));
        Traversal::Any::type (o);
        pop_ ();
      }

      void Any::
      value (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("value", top_ ()));
        Traversal::Any::value (o);
        pop_ ();
      }

      // Property
      //
      //

      Property::
      Property (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      Property::
      Property ()
      {
      }

      void Property::
      traverse (Type const& o)
      {
        Traversal::Property::traverse (o);
      }

      void Property::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::Property::name (o);
        pop_ ();
      }

      void Property::
      value (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("value", top_ ()));
        Traversal::Property::value (o);
        pop_ ();
      }

      // Node
      //
      //

      Node::
      Node (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      Node::
      Node ()
      {
      }

      void Node::
      traverse (Type const& o)
      {
        Traversal::Node::traverse (o);
      }

      void Node::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::Node::name (o);
        pop_ ();
      }

      void Node::
      label (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("label", top_ ()));
        Traversal::Node::label (o);
        pop_ ();
      }

      // SatisfierPropertyKind
      //
      //

      SatisfierPropertyKind::
      SatisfierPropertyKind (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      SatisfierPropertyKind::
      SatisfierPropertyKind ()
      {
      }

      void SatisfierPropertyKind::
      traverse (Type const& o)
      {
        ::std::basic_string< char > s;

        if (o == ::CIAO::Config_Handlers::SatisfierPropertyKind::Quantity) s = "Quantity";
        else if (o == ::CIAO::Config_Handlers::SatisfierPropertyKind::Capacity) s = "Capacity";
        else if (o == ::CIAO::Config_Handlers::SatisfierPropertyKind::Minimum) s = "Minimum";
        else if (o == ::CIAO::Config_Handlers::SatisfierPropertyKind::Maximum) s = "Maximum";
        else if (o == ::CIAO::Config_Handlers::SatisfierPropertyKind::Attribute) s = "Attribute";
        else if (o == ::CIAO::Config_Handlers::SatisfierPropertyKind::Selection) s = "Selection";
        else 
        {
        }

        if (::XSCRT::XML::Attribute< char >* a = attr_ ())
        {
          a->value (s);
        }

        else
        {
          top_().value (s);
        }
      }

      // SatisfierProperty
      //
      //

      SatisfierProperty::
      SatisfierProperty (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      SatisfierProperty::
      SatisfierProperty ()
      {
      }

      void SatisfierProperty::
      traverse (Type const& o)
      {
        Traversal::SatisfierProperty::traverse (o);
      }

      void SatisfierProperty::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::SatisfierProperty::name (o);
        pop_ ();
      }

      void SatisfierProperty::
      kind (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("kind", top_ ()));
        Traversal::SatisfierProperty::kind (o);
        pop_ ();
      }

      void SatisfierProperty::
      value (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("value", top_ ()));
        Traversal::SatisfierProperty::value (o);
        pop_ ();
      }

      // Resource
      //
      //

      Resource::
      Resource (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      Resource::
      Resource ()
      {
      }

      void Resource::
      traverse (Type const& o)
      {
        Traversal::Resource::traverse (o);
      }

      void Resource::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::Resource::name (o);
        pop_ ();
      }

      void Resource::
      resourceType (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("resourceType", top_ ()));
        Traversal::Resource::resourceType (o);
        pop_ ();
      }

      void Resource::
      property (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("property", top_ ()));
        Traversal::Resource::property (o);
        pop_ ();
      }

      // SharedResource
      //
      //

      SharedResource::
      SharedResource (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      SharedResource::
      SharedResource ()
      {
      }

      void SharedResource::
      traverse (Type const& o)
      {
        Traversal::SharedResource::traverse (o);
      }

      void SharedResource::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::SharedResource::name (o);
        pop_ ();
      }

      void SharedResource::
      resourceType (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("resourceType", top_ ()));
        Traversal::SharedResource::resourceType (o);
        pop_ ();
      }

      void SharedResource::
      node (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("node", top_ ()));
        Traversal::SharedResource::node (o);
        pop_ ();
      }

      void SharedResource::
      property (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("property", top_ ()));
        Traversal::SharedResource::property (o);
        pop_ ();
      }

      // Requirement
      //
      //

      Requirement::
      Requirement (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      Requirement::
      Requirement ()
      {
      }

      void Requirement::
      traverse (Type const& o)
      {
        Traversal::Requirement::traverse (o);
      }

      void Requirement::
      resourceType (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("resourceType", top_ ()));
        Traversal::Requirement::resourceType (o);
        pop_ ();
      }

      void Requirement::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::Requirement::name (o);
        pop_ ();
      }

      void Requirement::
      property (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("property", top_ ()));
        Traversal::Requirement::property (o);
        pop_ ();
      }

      // ResourceDeploymentDescription
      //
      //

      ResourceDeploymentDescription::
      ResourceDeploymentDescription (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      ResourceDeploymentDescription::
      ResourceDeploymentDescription ()
      {
      }

      void ResourceDeploymentDescription::
      traverse (Type const& o)
      {
        Traversal::ResourceDeploymentDescription::traverse (o);
      }

      void ResourceDeploymentDescription::
      requirementName (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("requirementName", top_ ()));
        Traversal::ResourceDeploymentDescription::requirementName (o);
        pop_ ();
      }

      void ResourceDeploymentDescription::
      resourceName (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("resourceName", top_ ()));
        Traversal::ResourceDeploymentDescription::resourceName (o);
        pop_ ();
      }

      void ResourceDeploymentDescription::
      resourceValue (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("resourceValue", top_ ()));
        Traversal::ResourceDeploymentDescription::resourceValue (o);
        pop_ ();
      }

      // ArtifactDeploymentDescription
      //
      //

      ArtifactDeploymentDescription::
      ArtifactDeploymentDescription (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      ArtifactDeploymentDescription::
      ArtifactDeploymentDescription ()
      {
      }

      void ArtifactDeploymentDescription::
      traverse (Type const& o)
      {
        Traversal::ArtifactDeploymentDescription::traverse (o);
      }

      void ArtifactDeploymentDescription::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::ArtifactDeploymentDescription::name (o);
        pop_ ();
      }

      void ArtifactDeploymentDescription::
      source_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("source", top_ ()));
      }

      void ArtifactDeploymentDescription::
      source_next (Type const& o)
      {
        source_post (o);
        source_pre (o);
      }

      void ArtifactDeploymentDescription::
      source_post (Type const&)
      {
        pop_ ();
      }

      void ArtifactDeploymentDescription::
      node (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("node", top_ ()));
        Traversal::ArtifactDeploymentDescription::node (o);
        pop_ ();
      }

      void ArtifactDeploymentDescription::
      location_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("location", top_ ()));
      }

      void ArtifactDeploymentDescription::
      location_next (Type const& o)
      {
        location_post (o);
        location_pre (o);
      }

      void ArtifactDeploymentDescription::
      location_post (Type const&)
      {
        pop_ ();
      }

      void ArtifactDeploymentDescription::
      execParameter_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("execParameter", top_ ()));
      }

      void ArtifactDeploymentDescription::
      execParameter_next (Type const& o)
      {
        execParameter_post (o);
        execParameter_pre (o);
      }

      void ArtifactDeploymentDescription::
      execParameter_post (Type const&)
      {
        pop_ ();
      }

      void ArtifactDeploymentDescription::
      deployRequirement_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("deployRequirement", top_ ()));
      }

      void ArtifactDeploymentDescription::
      deployRequirement_next (Type const& o)
      {
        deployRequirement_post (o);
        deployRequirement_pre (o);
      }

      void ArtifactDeploymentDescription::
      deployRequirement_post (Type const&)
      {
        pop_ ();
      }

      void ArtifactDeploymentDescription::
      deployedResource_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("deployedResource", top_ ()));
      }

      void ArtifactDeploymentDescription::
      deployedResource_next (Type const& o)
      {
        deployedResource_post (o);
        deployedResource_pre (o);
      }

      void ArtifactDeploymentDescription::
      deployedResource_post (Type const&)
      {
        pop_ ();
      }

      void ArtifactDeploymentDescription::
      id (Type const& o)
      {
        ::XSCRT::XML::Attribute< char > a ("id", "", top_ ());
        attr_ (&a);
        Traversal::ArtifactDeploymentDescription::id (o);
        attr_ (0);
      }

      // MonolithicDeploymentDescription
      //
      //

      MonolithicDeploymentDescription::
      MonolithicDeploymentDescription (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      MonolithicDeploymentDescription::
      MonolithicDeploymentDescription ()
      {
      }

      void MonolithicDeploymentDescription::
      traverse (Type const& o)
      {
        Traversal::MonolithicDeploymentDescription::traverse (o);
      }

      void MonolithicDeploymentDescription::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::MonolithicDeploymentDescription::name (o);
        pop_ ();
      }

      void MonolithicDeploymentDescription::
      source_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("source", top_ ()));
      }

      void MonolithicDeploymentDescription::
      source_next (Type const& o)
      {
        source_post (o);
        source_pre (o);
      }

      void MonolithicDeploymentDescription::
      source_post (Type const&)
      {
        pop_ ();
      }

      void MonolithicDeploymentDescription::
      artifact_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("artifact", top_ ()));
      }

      void MonolithicDeploymentDescription::
      artifact_next (Type const& o)
      {
        artifact_post (o);
        artifact_pre (o);
      }

      void MonolithicDeploymentDescription::
      artifact_post (Type const&)
      {
        pop_ ();
      }

      void MonolithicDeploymentDescription::
      execParameter_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("execParameter", top_ ()));
      }

      void MonolithicDeploymentDescription::
      execParameter_next (Type const& o)
      {
        execParameter_post (o);
        execParameter_pre (o);
      }

      void MonolithicDeploymentDescription::
      execParameter_post (Type const&)
      {
        pop_ ();
      }

      void MonolithicDeploymentDescription::
      deployRequirement_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("deployRequirement", top_ ()));
      }

      void MonolithicDeploymentDescription::
      deployRequirement_next (Type const& o)
      {
        deployRequirement_post (o);
        deployRequirement_pre (o);
      }

      void MonolithicDeploymentDescription::
      deployRequirement_post (Type const&)
      {
        pop_ ();
      }

      void MonolithicDeploymentDescription::
      id (Type const& o)
      {
        ::XSCRT::XML::Attribute< char > a ("id", "", top_ ());
        attr_ (&a);
        Traversal::MonolithicDeploymentDescription::id (o);
        attr_ (0);
      }

      // ResourceUsageKind
      //
      //

      ResourceUsageKind::
      ResourceUsageKind (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      ResourceUsageKind::
      ResourceUsageKind ()
      {
      }

      void ResourceUsageKind::
      traverse (Type const& o)
      {
        ::std::basic_string< char > s;

        if (o == ::CIAO::Config_Handlers::ResourceUsageKind::None) s = "None";
        else if (o == ::CIAO::Config_Handlers::ResourceUsageKind::InstanceUsesResource) s = "InstanceUsesResource";
        else if (o == ::CIAO::Config_Handlers::ResourceUsageKind::ResourceUsesInstance) s = "ResourceUsesInstance";
        else if (o == ::CIAO::Config_Handlers::ResourceUsageKind::PortUsesResource) s = "PortUsesResource";
        else if (o == ::CIAO::Config_Handlers::ResourceUsageKind::ResourceUsesPort) s = "ResourceUsesPort";
        else 
        {
        }

        if (::XSCRT::XML::Attribute< char >* a = attr_ ())
        {
          a->value (s);
        }

        else
        {
          top_().value (s);
        }
      }

      // InstanceResourceDeploymentDescription
      //
      //

      InstanceResourceDeploymentDescription::
      InstanceResourceDeploymentDescription (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      InstanceResourceDeploymentDescription::
      InstanceResourceDeploymentDescription ()
      {
      }

      void InstanceResourceDeploymentDescription::
      traverse (Type const& o)
      {
        Traversal::InstanceResourceDeploymentDescription::traverse (o);
      }

      void InstanceResourceDeploymentDescription::
      resourceUsage (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("resourceUsage", top_ ()));
        Traversal::InstanceResourceDeploymentDescription::resourceUsage (o);
        pop_ ();
      }

      void InstanceResourceDeploymentDescription::
      requirementName (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("requirementName", top_ ()));
        Traversal::InstanceResourceDeploymentDescription::requirementName (o);
        pop_ ();
      }

      void InstanceResourceDeploymentDescription::
      resourceName (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("resourceName", top_ ()));
        Traversal::InstanceResourceDeploymentDescription::resourceName (o);
        pop_ ();
      }

      void InstanceResourceDeploymentDescription::
      resourceValue (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("resourceValue", top_ ()));
        Traversal::InstanceResourceDeploymentDescription::resourceValue (o);
        pop_ ();
      }

      // InstanceDeploymentDescription
      //
      //

      InstanceDeploymentDescription::
      InstanceDeploymentDescription (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      InstanceDeploymentDescription::
      InstanceDeploymentDescription ()
      {
      }

      void InstanceDeploymentDescription::
      traverse (Type const& o)
      {
        Traversal::InstanceDeploymentDescription::traverse (o);
      }

      void InstanceDeploymentDescription::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::InstanceDeploymentDescription::name (o);
        pop_ ();
      }

      void InstanceDeploymentDescription::
      node (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("node", top_ ()));
        Traversal::InstanceDeploymentDescription::node (o);
        pop_ ();
      }

      void InstanceDeploymentDescription::
      source (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("source", top_ ()));
        Traversal::InstanceDeploymentDescription::source (o);
        pop_ ();
      }

      void InstanceDeploymentDescription::
      implementation (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("implementation", top_ ()));
        Traversal::InstanceDeploymentDescription::implementation (o);
        pop_ ();
      }

      void InstanceDeploymentDescription::
      configProperty_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("configProperty", top_ ()));
      }

      void InstanceDeploymentDescription::
      configProperty_next (Type const& o)
      {
        configProperty_post (o);
        configProperty_pre (o);
      }

      void InstanceDeploymentDescription::
      configProperty_post (Type const&)
      {
        pop_ ();
      }

      void InstanceDeploymentDescription::
      deployedResource_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("deployedResource", top_ ()));
      }

      void InstanceDeploymentDescription::
      deployedResource_next (Type const& o)
      {
        deployedResource_post (o);
        deployedResource_pre (o);
      }

      void InstanceDeploymentDescription::
      deployedResource_post (Type const&)
      {
        pop_ ();
      }

      void InstanceDeploymentDescription::
      deployedSharedResource (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("deployedSharedResource", top_ ()));
        Traversal::InstanceDeploymentDescription::deployedSharedResource (o);
        pop_ ();
      }

      void InstanceDeploymentDescription::
      id (Type const& o)
      {
        ::XSCRT::XML::Attribute< char > a ("id", "", top_ ());
        attr_ (&a);
        Traversal::InstanceDeploymentDescription::id (o);
        attr_ (0);
      }

      // CCMComponentPortKind
      //
      //

      CCMComponentPortKind::
      CCMComponentPortKind (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      CCMComponentPortKind::
      CCMComponentPortKind ()
      {
      }

      void CCMComponentPortKind::
      traverse (Type const& o)
      {
        ::std::basic_string< char > s;

        if (o == ::CIAO::Config_Handlers::CCMComponentPortKind::Facet) s = "Facet";
        else if (o == ::CIAO::Config_Handlers::CCMComponentPortKind::SimplexReceptacle) s = "SimplexReceptacle";
        else if (o == ::CIAO::Config_Handlers::CCMComponentPortKind::MultiplexReceptacle) s = "MultiplexReceptacle";
        else if (o == ::CIAO::Config_Handlers::CCMComponentPortKind::EventEmitter) s = "EventEmitter";
        else if (o == ::CIAO::Config_Handlers::CCMComponentPortKind::EventPublisher) s = "EventPublisher";
        else if (o == ::CIAO::Config_Handlers::CCMComponentPortKind::EventConsumer) s = "EventConsumer";
        else 
        {
        }

        if (::XSCRT::XML::Attribute< char >* a = attr_ ())
        {
          a->value (s);
        }

        else
        {
          top_().value (s);
        }
      }

      // ComponentPortDescription
      //
      //

      ComponentPortDescription::
      ComponentPortDescription (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      ComponentPortDescription::
      ComponentPortDescription ()
      {
      }

      void ComponentPortDescription::
      traverse (Type const& o)
      {
        Traversal::ComponentPortDescription::traverse (o);
      }

      void ComponentPortDescription::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::ComponentPortDescription::name (o);
        pop_ ();
      }

      void ComponentPortDescription::
      specificType (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("specificType", top_ ()));
        Traversal::ComponentPortDescription::specificType (o);
        pop_ ();
      }

      void ComponentPortDescription::
      supportedType (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("supportedType", top_ ()));
        Traversal::ComponentPortDescription::supportedType (o);
        pop_ ();
      }

      void ComponentPortDescription::
      provider (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("provider", top_ ()));
        Traversal::ComponentPortDescription::provider (o);
        pop_ ();
      }

      void ComponentPortDescription::
      exclusiveProvider (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("exclusiveProvider", top_ ()));
        Traversal::ComponentPortDescription::exclusiveProvider (o);
        pop_ ();
      }

      void ComponentPortDescription::
      exclusiveUser (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("exclusiveUser", top_ ()));
        Traversal::ComponentPortDescription::exclusiveUser (o);
        pop_ ();
      }

      void ComponentPortDescription::
      optional (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("optional", top_ ()));
        Traversal::ComponentPortDescription::optional (o);
        pop_ ();
      }

      void ComponentPortDescription::
      kind (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("kind", top_ ()));
        Traversal::ComponentPortDescription::kind (o);
        pop_ ();
      }

      // ComponentPropertyDescription
      //
      //

      ComponentPropertyDescription::
      ComponentPropertyDescription (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      ComponentPropertyDescription::
      ComponentPropertyDescription ()
      {
      }

      void ComponentPropertyDescription::
      traverse (Type const& o)
      {
        Traversal::ComponentPropertyDescription::traverse (o);
      }

      void ComponentPropertyDescription::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::ComponentPropertyDescription::name (o);
        pop_ ();
      }

      void ComponentPropertyDescription::
      type (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("type", top_ ()));
        Traversal::ComponentPropertyDescription::type (o);
        pop_ ();
      }

      // ComponentExternalPortEndpoint
      //
      //

      ComponentExternalPortEndpoint::
      ComponentExternalPortEndpoint (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      ComponentExternalPortEndpoint::
      ComponentExternalPortEndpoint ()
      {
      }

      void ComponentExternalPortEndpoint::
      traverse (Type const& o)
      {
        Traversal::ComponentExternalPortEndpoint::traverse (o);
      }

      void ComponentExternalPortEndpoint::
      portName (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("portName", top_ ()));
        Traversal::ComponentExternalPortEndpoint::portName (o);
        pop_ ();
      }

      // PlanSubcomponentPortEndpoint
      //
      //

      PlanSubcomponentPortEndpoint::
      PlanSubcomponentPortEndpoint (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      PlanSubcomponentPortEndpoint::
      PlanSubcomponentPortEndpoint ()
      {
      }

      void PlanSubcomponentPortEndpoint::
      traverse (Type const& o)
      {
        Traversal::PlanSubcomponentPortEndpoint::traverse (o);
      }

      void PlanSubcomponentPortEndpoint::
      portName (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("portName", top_ ()));
        Traversal::PlanSubcomponentPortEndpoint::portName (o);
        pop_ ();
      }

      void PlanSubcomponentPortEndpoint::
      provider (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("provider", top_ ()));
        Traversal::PlanSubcomponentPortEndpoint::provider (o);
        pop_ ();
      }

      void PlanSubcomponentPortEndpoint::
      kind (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("kind", top_ ()));
        Traversal::PlanSubcomponentPortEndpoint::kind (o);
        pop_ ();
      }

      void PlanSubcomponentPortEndpoint::
      instance (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("instance", top_ ()));
        Traversal::PlanSubcomponentPortEndpoint::instance (o);
        pop_ ();
      }

      // ExternalReferenceEndpoint
      //
      //

      ExternalReferenceEndpoint::
      ExternalReferenceEndpoint (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      ExternalReferenceEndpoint::
      ExternalReferenceEndpoint ()
      {
      }

      void ExternalReferenceEndpoint::
      traverse (Type const& o)
      {
        Traversal::ExternalReferenceEndpoint::traverse (o);
      }

      void ExternalReferenceEndpoint::
      location (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("location", top_ ()));
        Traversal::ExternalReferenceEndpoint::location (o);
        pop_ ();
      }

      // ConnectionResourceDeploymentDescription
      //
      //

      ConnectionResourceDeploymentDescription::
      ConnectionResourceDeploymentDescription (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      ConnectionResourceDeploymentDescription::
      ConnectionResourceDeploymentDescription ()
      {
      }

      void ConnectionResourceDeploymentDescription::
      traverse (Type const& o)
      {
        Traversal::ConnectionResourceDeploymentDescription::traverse (o);
      }

      void ConnectionResourceDeploymentDescription::
      targetName (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("targetName", top_ ()));
        Traversal::ConnectionResourceDeploymentDescription::targetName (o);
        pop_ ();
      }

      void ConnectionResourceDeploymentDescription::
      requirementName (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("requirementName", top_ ()));
        Traversal::ConnectionResourceDeploymentDescription::requirementName (o);
        pop_ ();
      }

      void ConnectionResourceDeploymentDescription::
      resourceName (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("resourceName", top_ ()));
        Traversal::ConnectionResourceDeploymentDescription::resourceName (o);
        pop_ ();
      }

      void ConnectionResourceDeploymentDescription::
      resourceValue (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("resourceValue", top_ ()));
        Traversal::ConnectionResourceDeploymentDescription::resourceValue (o);
        pop_ ();
      }

      // PlanConnectionDescription
      //
      //

      PlanConnectionDescription::
      PlanConnectionDescription (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      PlanConnectionDescription::
      PlanConnectionDescription ()
      {
      }

      void PlanConnectionDescription::
      traverse (Type const& o)
      {
        Traversal::PlanConnectionDescription::traverse (o);
      }

      void PlanConnectionDescription::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::PlanConnectionDescription::name (o);
        pop_ ();
      }

      void PlanConnectionDescription::
      source (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("source", top_ ()));
        Traversal::PlanConnectionDescription::source (o);
        pop_ ();
      }

      void PlanConnectionDescription::
      deployRequirement_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("deployRequirement", top_ ()));
      }

      void PlanConnectionDescription::
      deployRequirement_next (Type const& o)
      {
        deployRequirement_post (o);
        deployRequirement_pre (o);
      }

      void PlanConnectionDescription::
      deployRequirement_post (Type const&)
      {
        pop_ ();
      }

      void PlanConnectionDescription::
      externalEndpoint_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("externalEndpoint", top_ ()));
      }

      void PlanConnectionDescription::
      externalEndpoint_next (Type const& o)
      {
        externalEndpoint_post (o);
        externalEndpoint_pre (o);
      }

      void PlanConnectionDescription::
      externalEndpoint_post (Type const&)
      {
        pop_ ();
      }

      void PlanConnectionDescription::
      internalEndpoint_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("internalEndpoint", top_ ()));
      }

      void PlanConnectionDescription::
      internalEndpoint_next (Type const& o)
      {
        internalEndpoint_post (o);
        internalEndpoint_pre (o);
      }

      void PlanConnectionDescription::
      internalEndpoint_post (Type const&)
      {
        pop_ ();
      }

      void PlanConnectionDescription::
      externalReference_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("externalReference", top_ ()));
      }

      void PlanConnectionDescription::
      externalReference_next (Type const& o)
      {
        externalReference_post (o);
        externalReference_pre (o);
      }

      void PlanConnectionDescription::
      externalReference_post (Type const&)
      {
        pop_ ();
      }

      void PlanConnectionDescription::
      deployedResource_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("deployedResource", top_ ()));
      }

      void PlanConnectionDescription::
      deployedResource_next (Type const& o)
      {
        deployedResource_post (o);
        deployedResource_pre (o);
      }

      void PlanConnectionDescription::
      deployedResource_post (Type const&)
      {
        pop_ ();
      }

      // ImplementationDependency
      //
      //

      ImplementationDependency::
      ImplementationDependency (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      ImplementationDependency::
      ImplementationDependency ()
      {
      }

      void ImplementationDependency::
      traverse (Type const& o)
      {
        Traversal::ImplementationDependency::traverse (o);
      }

      void ImplementationDependency::
      requiredType (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("requiredType", top_ ()));
        Traversal::ImplementationDependency::requiredType (o);
        pop_ ();
      }

      // Capability
      //
      //

      Capability::
      Capability (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      Capability::
      Capability ()
      {
      }

      void Capability::
      traverse (Type const& o)
      {
        Traversal::Capability::traverse (o);
      }

      void Capability::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::Capability::name (o);
        pop_ ();
      }

      void Capability::
      resourceType (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("resourceType", top_ ()));
        Traversal::Capability::resourceType (o);
        pop_ ();
      }

      void Capability::
      property (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("property", top_ ()));
        Traversal::Capability::property (o);
        pop_ ();
      }

      // ImplementationRequirement
      //
      //

      ImplementationRequirement::
      ImplementationRequirement (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      ImplementationRequirement::
      ImplementationRequirement ()
      {
      }

      void ImplementationRequirement::
      traverse (Type const& o)
      {
        Traversal::ImplementationRequirement::traverse (o);
      }

      void ImplementationRequirement::
      resourceUsage (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("resourceUsage", top_ ()));
        Traversal::ImplementationRequirement::resourceUsage (o);
        pop_ ();
      }

      void ImplementationRequirement::
      resourcePort (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("resourcePort", top_ ()));
        Traversal::ImplementationRequirement::resourcePort (o);
        pop_ ();
      }

      void ImplementationRequirement::
      componentPort (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("componentPort", top_ ()));
        Traversal::ImplementationRequirement::componentPort (o);
        pop_ ();
      }

      void ImplementationRequirement::
      resourceType (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("resourceType", top_ ()));
        Traversal::ImplementationRequirement::resourceType (o);
        pop_ ();
      }

      void ImplementationRequirement::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::ImplementationRequirement::name (o);
        pop_ ();
      }

      void ImplementationRequirement::
      property (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("property", top_ ()));
        Traversal::ImplementationRequirement::property (o);
        pop_ ();
      }

      // ComponentPackageReference
      //
      //

      ComponentPackageReference::
      ComponentPackageReference (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      ComponentPackageReference::
      ComponentPackageReference ()
      {
      }

      void ComponentPackageReference::
      traverse (Type const& o)
      {
        Traversal::ComponentPackageReference::traverse (o);
      }

      void ComponentPackageReference::
      requiredUUID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("requiredUUID", top_ ()));
        Traversal::ComponentPackageReference::requiredUUID (o);
        pop_ ();
      }

      void ComponentPackageReference::
      requiredName (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("requiredName", top_ ()));
        Traversal::ComponentPackageReference::requiredName (o);
        pop_ ();
      }

      void ComponentPackageReference::
      requiredType (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("requiredType", top_ ()));
        Traversal::ComponentPackageReference::requiredType (o);
        pop_ ();
      }

      // SubcomponentPortEndpoint
      //
      //

      SubcomponentPortEndpoint::
      SubcomponentPortEndpoint (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      SubcomponentPortEndpoint::
      SubcomponentPortEndpoint ()
      {
      }

      void SubcomponentPortEndpoint::
      traverse (Type const& o)
      {
        Traversal::SubcomponentPortEndpoint::traverse (o);
      }

      void SubcomponentPortEndpoint::
      portName (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("portName", top_ ()));
        Traversal::SubcomponentPortEndpoint::portName (o);
        pop_ ();
      }

      void SubcomponentPortEndpoint::
      instance (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("instance", top_ ()));
        Traversal::SubcomponentPortEndpoint::instance (o);
        pop_ ();
      }

      // AssemblyConnectionDescription
      //
      //

      AssemblyConnectionDescription::
      AssemblyConnectionDescription (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      AssemblyConnectionDescription::
      AssemblyConnectionDescription ()
      {
      }

      void AssemblyConnectionDescription::
      traverse (Type const& o)
      {
        Traversal::AssemblyConnectionDescription::traverse (o);
      }

      void AssemblyConnectionDescription::
      name (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
        Traversal::AssemblyConnectionDescription::name (o);
        pop_ ();
      }

      void AssemblyConnectionDescription::
      deployRequirement (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("deployRequirement", top_ ()));
        Traversal::AssemblyConnectionDescription::deployRequirement (o);
        pop_ ();
      }

      void AssemblyConnectionDescription::
      externalEndpoint_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("externalEndpoint", top_ ()));
      }

      void AssemblyConnectionDescription::
      externalEndpoint_next (Type const& o)
      {
        externalEndpoint_post (o);
        externalEndpoint_pre (o);
      }

      void AssemblyConnectionDescription::
      externalEndpoint_post (Type const&)
      {
        pop_ ();
      }

      void AssemblyConnectionDescription::
      internalEndpoint_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("internalEndpoint", top_ ()));
      }

      void AssemblyConnectionDescription::
      internalEndpoint_next (Type const& o)
      {
        internalEndpoint_post (o);
        internalEndpoint_pre (o);
      }

      void AssemblyConnectionDescription::
      internalEndpoint_post (Type const&)
      {
        pop_ ();
      }

      void AssemblyConnectionDescription::
      externalReference_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("externalReference", top_ ()));
      }

      void AssemblyConnectionDescription::
      externalReference_next (Type const& o)
      {
        externalReference_post (o);
        externalReference_pre (o);
      }

      void AssemblyConnectionDescription::
      externalReference_post (Type const&)
      {
        pop_ ();
      }
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
  }
}

