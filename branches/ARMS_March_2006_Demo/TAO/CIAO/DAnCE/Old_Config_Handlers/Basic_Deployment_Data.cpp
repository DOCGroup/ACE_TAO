// $Id$
#include "Basic_Deployment_Data.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    // TCKind
    // 

    TCKind::
    TCKind ()
    {
    }

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
    Any ()
    :
    type_ (new ::CIAO::Config_Handlers::DataType ()),
    value_ (new ::CIAO::Config_Handlers::DataValue ()),
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
    Property ()
    :
    name_ (new ::XMLSchema::string< char > ()),
    value_ (new ::CIAO::Config_Handlers::Any ()),
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
    Node ()
    :
    name_ (new ::XMLSchema::string< char > ()),
    label_ (new ::XMLSchema::string< char > ()),
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

    SatisfierPropertyKind::
    SatisfierPropertyKind ()
    {
    }

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
    SatisfierProperty ()
    :
    name_ (new ::XMLSchema::string< char > ()),
    kind_ (new ::CIAO::Config_Handlers::SatisfierPropertyKind ()),
    value_ (new ::CIAO::Config_Handlers::Any ()),
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
    Resource ()
    :
    name_ (new ::XMLSchema::string< char > ()),
    resourceType_ (new ::XMLSchema::string< char > ()),
    property_ (new ::CIAO::Config_Handlers::SatisfierProperty ()),
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
    SharedResource ()
    :
    name_ (new ::XMLSchema::string< char > ()),
    resourceType_ (new ::XMLSchema::string< char > ()),
    node_ (new ::CIAO::Config_Handlers::Node ()),
    property_ (new ::CIAO::Config_Handlers::SatisfierProperty ()),
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
    Requirement ()
    :
    resourceType_ (new ::XMLSchema::string< char > ()),
    name_ (new ::XMLSchema::string< char > ()),
    property_ (new ::CIAO::Config_Handlers::Property ()),
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
    ResourceDeploymentDescription ()
    :
    requirementName_ (new ::XMLSchema::string< char > ()),
    resourceName_ (new ::XMLSchema::string< char > ()),
    resourceValue_ (new ::CIAO::Config_Handlers::Any ()),
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
    ArtifactDeploymentDescription ()
    :
    name_ (new ::XMLSchema::string< char > ()),
    source_ (new ::XMLSchema::string< char > ()),
    regulator__ ()
    {
      name_->container (this);
      source_->container (this);
    }

    ArtifactDeploymentDescription::
    ArtifactDeploymentDescription (::CIAO::Config_Handlers::ArtifactDeploymentDescription const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    source_ (new ::XMLSchema::string< char > (*s.source_)),
    node_ (s.node_.get () ? new ::XMLSchema::string< char > (*s.node_) : 0),
    execParameter_ (s.execParameter_.get () ? new ::CIAO::Config_Handlers::Property (*s.execParameter_) : 0),
    deployRequirement_ (s.deployRequirement_.get () ? new ::CIAO::Config_Handlers::Requirement (*s.deployRequirement_) : 0),
    deployedResource_ (s.deployedResource_.get () ? new ::CIAO::Config_Handlers::ResourceDeploymentDescription (*s.deployedResource_) : 0),
    id_ (s.id_.get () ? new ::XMLSchema::ID< char > (*s.id_) : 0),
    regulator__ ()
    {
      name_->container (this);
      source_->container (this);
      if (node_.get ()) node_->container (this);
      location_.reserve (s.location_.size ());
      {
        for (location_const_iterator i (s.location_.begin ());
        i != s.location_.end ();
        ++i) add_location (*i);
      }

      if (execParameter_.get ()) execParameter_->container (this);
      if (deployRequirement_.get ()) deployRequirement_->container (this);
      if (deployedResource_.get ()) deployedResource_->container (this);
      if (id_.get ()) id_->container (this);
    }

    ::CIAO::Config_Handlers::ArtifactDeploymentDescription& ArtifactDeploymentDescription::
    operator= (::CIAO::Config_Handlers::ArtifactDeploymentDescription const& s)
    {
      name (s.name ());

      source (s.source ());

      if (s.node_.get ()) node (*(s.node_));
      else node_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      location_.clear ();
      location_.reserve (s.location_.size ());
      {
        for (location_const_iterator i (s.location_.begin ());
        i != s.location_.end ();
        ++i) add_location (*i);
      }

      if (s.execParameter_.get ()) execParameter (*(s.execParameter_));
      else execParameter_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (0);

      if (s.deployRequirement_.get ()) deployRequirement (*(s.deployRequirement_));
      else deployRequirement_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > (0);

      if (s.deployedResource_.get ()) deployedResource (*(s.deployedResource_));
      else deployedResource_ = ::std::auto_ptr< ::CIAO::Config_Handlers::ResourceDeploymentDescription > (0);

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
    ::XMLSchema::string< char > const& ArtifactDeploymentDescription::
    source () const
    {
      return *source_;
    }

    ::XMLSchema::string< char >& ArtifactDeploymentDescription::
    source ()
    {
      return *source_;
    }

    void ArtifactDeploymentDescription::
    source (::XMLSchema::string< char > const& e)
    {
      *source_ = e;
    }

    // ArtifactDeploymentDescription
    // 
    bool ArtifactDeploymentDescription::
    node_p () const
    {
      return node_.get () != 0;
    }

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
      if (node_.get ())
      {
        *node_ = e;
      }

      else
      {
        node_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
        node_->container (this);
      }
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
    bool ArtifactDeploymentDescription::
    execParameter_p () const
    {
      return execParameter_.get () != 0;
    }

    ::CIAO::Config_Handlers::Property const& ArtifactDeploymentDescription::
    execParameter () const
    {
      return *execParameter_;
    }

    ::CIAO::Config_Handlers::Property& ArtifactDeploymentDescription::
    execParameter ()
    {
      return *execParameter_;
    }

    void ArtifactDeploymentDescription::
    execParameter (::CIAO::Config_Handlers::Property const& e)
    {
      if (execParameter_.get ())
      {
        *execParameter_ = e;
      }

      else
      {
        execParameter_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (new ::CIAO::Config_Handlers::Property (e));
        execParameter_->container (this);
      }
    }

    // ArtifactDeploymentDescription
    // 
    bool ArtifactDeploymentDescription::
    deployRequirement_p () const
    {
      return deployRequirement_.get () != 0;
    }

    ::CIAO::Config_Handlers::Requirement const& ArtifactDeploymentDescription::
    deployRequirement () const
    {
      return *deployRequirement_;
    }

    ::CIAO::Config_Handlers::Requirement& ArtifactDeploymentDescription::
    deployRequirement ()
    {
      return *deployRequirement_;
    }

    void ArtifactDeploymentDescription::
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

    // ArtifactDeploymentDescription
    // 
    bool ArtifactDeploymentDescription::
    deployedResource_p () const
    {
      return deployedResource_.get () != 0;
    }

    ::CIAO::Config_Handlers::ResourceDeploymentDescription const& ArtifactDeploymentDescription::
    deployedResource () const
    {
      return *deployedResource_;
    }

    ::CIAO::Config_Handlers::ResourceDeploymentDescription& ArtifactDeploymentDescription::
    deployedResource ()
    {
      return *deployedResource_;
    }

    void ArtifactDeploymentDescription::
    deployedResource (::CIAO::Config_Handlers::ResourceDeploymentDescription const& e)
    {
      if (deployedResource_.get ())
      {
        *deployedResource_ = e;
      }

      else
      {
        deployedResource_ = ::std::auto_ptr< ::CIAO::Config_Handlers::ResourceDeploymentDescription > (new ::CIAO::Config_Handlers::ResourceDeploymentDescription (e));
        deployedResource_->container (this);
      }
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
    MonolithicDeploymentDescription ()
    :
    name_ (new ::XMLSchema::string< char > ()),
    source_ (new ::XMLSchema::string< char > ()),
    regulator__ ()
    {
      name_->container (this);
      source_->container (this);
    }

    MonolithicDeploymentDescription::
    MonolithicDeploymentDescription (::CIAO::Config_Handlers::MonolithicDeploymentDescription const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    source_ (new ::XMLSchema::string< char > (*s.source_)),
    execParameter_ (s.execParameter_.get () ? new ::CIAO::Config_Handlers::Property (*s.execParameter_) : 0),
    deployRequirement_ (s.deployRequirement_.get () ? new ::CIAO::Config_Handlers::Requirement (*s.deployRequirement_) : 0),
    id_ (s.id_.get () ? new ::XMLSchema::ID< char > (*s.id_) : 0),
    regulator__ ()
    {
      name_->container (this);
      source_->container (this);
      artifact_.reserve (s.artifact_.size ());
      {
        for (artifact_const_iterator i (s.artifact_.begin ());
        i != s.artifact_.end ();
        ++i) add_artifact (*i);
      }

      if (execParameter_.get ()) execParameter_->container (this);
      if (deployRequirement_.get ()) deployRequirement_->container (this);
      if (id_.get ()) id_->container (this);
    }

    ::CIAO::Config_Handlers::MonolithicDeploymentDescription& MonolithicDeploymentDescription::
    operator= (::CIAO::Config_Handlers::MonolithicDeploymentDescription const& s)
    {
      name (s.name ());

      source (s.source ());

      artifact_.clear ();
      artifact_.reserve (s.artifact_.size ());
      {
        for (artifact_const_iterator i (s.artifact_.begin ());
        i != s.artifact_.end ();
        ++i) add_artifact (*i);
      }

      if (s.execParameter_.get ()) execParameter (*(s.execParameter_));
      else execParameter_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (0);

      if (s.deployRequirement_.get ()) deployRequirement (*(s.deployRequirement_));
      else deployRequirement_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > (0);

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
    ::XMLSchema::string< char > const& MonolithicDeploymentDescription::
    source () const
    {
      return *source_;
    }

    ::XMLSchema::string< char >& MonolithicDeploymentDescription::
    source ()
    {
      return *source_;
    }

    void MonolithicDeploymentDescription::
    source (::XMLSchema::string< char > const& e)
    {
      *source_ = e;
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
    bool MonolithicDeploymentDescription::
    execParameter_p () const
    {
      return execParameter_.get () != 0;
    }

    ::CIAO::Config_Handlers::Property const& MonolithicDeploymentDescription::
    execParameter () const
    {
      return *execParameter_;
    }

    ::CIAO::Config_Handlers::Property& MonolithicDeploymentDescription::
    execParameter ()
    {
      return *execParameter_;
    }

    void MonolithicDeploymentDescription::
    execParameter (::CIAO::Config_Handlers::Property const& e)
    {
      if (execParameter_.get ())
      {
        *execParameter_ = e;
      }

      else
      {
        execParameter_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (new ::CIAO::Config_Handlers::Property (e));
        execParameter_->container (this);
      }
    }

    // MonolithicDeploymentDescription
    // 
    bool MonolithicDeploymentDescription::
    deployRequirement_p () const
    {
      return deployRequirement_.get () != 0;
    }

    ::CIAO::Config_Handlers::Requirement const& MonolithicDeploymentDescription::
    deployRequirement () const
    {
      return *deployRequirement_;
    }

    ::CIAO::Config_Handlers::Requirement& MonolithicDeploymentDescription::
    deployRequirement ()
    {
      return *deployRequirement_;
    }

    void MonolithicDeploymentDescription::
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

    ResourceUsageKind::
    ResourceUsageKind ()
    {
    }

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
    InstanceResourceDeploymentDescription ()
    :
    resourceUsage_ (new ::CIAO::Config_Handlers::ResourceUsageKind ()),
    requirementName_ (new ::XMLSchema::string< char > ()),
    resourceName_ (new ::XMLSchema::string< char > ()),
    resourceValue_ (new ::CIAO::Config_Handlers::Any ()),
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
    InstanceDeploymentDescription ()
    :
    name_ (new ::XMLSchema::string< char > ()),
    node_ (new ::XMLSchema::string< char > ()),
    source_ (new ::XMLSchema::string< char > ()),
    implementation_ (new ::XMLSchema::IDREF< char > ()),
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
    configProperty_ (s.configProperty_.get () ? new ::CIAO::Config_Handlers::Property (*s.configProperty_) : 0),
    deployedResource_ (s.deployedResource_.get () ? new ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription (*s.deployedResource_) : 0),
    deployedSharedResource_ (s.deployedSharedResource_.get () ? new ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription (*s.deployedSharedResource_) : 0),
    id_ (s.id_.get () ? new ::XMLSchema::ID< char > (*s.id_) : 0),
    regulator__ ()
    {
      name_->container (this);
      node_->container (this);
      source_->container (this);
      implementation_->container (this);
      if (configProperty_.get ()) configProperty_->container (this);
      if (deployedResource_.get ()) deployedResource_->container (this);
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

      if (s.configProperty_.get ()) configProperty (*(s.configProperty_));
      else configProperty_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (0);

      if (s.deployedResource_.get ()) deployedResource (*(s.deployedResource_));
      else deployedResource_ = ::std::auto_ptr< ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription > (0);

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
    bool InstanceDeploymentDescription::
    configProperty_p () const
    {
      return configProperty_.get () != 0;
    }

    ::CIAO::Config_Handlers::Property const& InstanceDeploymentDescription::
    configProperty () const
    {
      return *configProperty_;
    }

    ::CIAO::Config_Handlers::Property& InstanceDeploymentDescription::
    configProperty ()
    {
      return *configProperty_;
    }

    void InstanceDeploymentDescription::
    configProperty (::CIAO::Config_Handlers::Property const& e)
    {
      if (configProperty_.get ())
      {
        *configProperty_ = e;
      }

      else
      {
        configProperty_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (new ::CIAO::Config_Handlers::Property (e));
        configProperty_->container (this);
      }
    }

    // InstanceDeploymentDescription
    // 
    bool InstanceDeploymentDescription::
    deployedResource_p () const
    {
      return deployedResource_.get () != 0;
    }

    ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription const& InstanceDeploymentDescription::
    deployedResource () const
    {
      return *deployedResource_;
    }

    ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription& InstanceDeploymentDescription::
    deployedResource ()
    {
      return *deployedResource_;
    }

    void InstanceDeploymentDescription::
    deployedResource (::CIAO::Config_Handlers::InstanceResourceDeploymentDescription const& e)
    {
      if (deployedResource_.get ())
      {
        *deployedResource_ = e;
      }

      else
      {
        deployedResource_ = ::std::auto_ptr< ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription > (new ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription (e));
        deployedResource_->container (this);
      }
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

    CCMComponentPortKind::
    CCMComponentPortKind ()
    {
    }

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
    ComponentPortDescription ()
    :
    name_ (new ::XMLSchema::string< char > ()),
    specificType_ (new ::XMLSchema::string< char > ()),
    supportedType_ (new ::XMLSchema::string< char > ()),
    provider_ (new ::XMLSchema::string< char > ()),
    exclusiveProvider_ (new ::XMLSchema::string< char > ()),
    exclusiveUser_ (new ::XMLSchema::string< char > ()),
    optional_ (new ::XMLSchema::string< char > ()),
    kind_ (new ::CIAO::Config_Handlers::CCMComponentPortKind ()),
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
    ComponentPropertyDescription ()
    :
    name_ (new ::XMLSchema::string< char > ()),
    type_ (new ::CIAO::Config_Handlers::DataType ()),
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
    ComponentExternalPortEndpoint ()
    :
    portName_ (new ::XMLSchema::string< char > ()),
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
    PlanSubcomponentPortEndpoint ()
    :
    portName_ (new ::XMLSchema::string< char > ()),
    kind_ (new ::CIAO::Config_Handlers::CCMComponentPortKind ()),
    instance_ (new ::XMLSchema::IDREF< char > ()),
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
    ExternalReferenceEndpoint ()
    :
    location_ (new ::XMLSchema::string< char > ()),
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
    ConnectionResourceDeploymentDescription ()
    :
    targetName_ (new ::XMLSchema::string< char > ()),
    requirementName_ (new ::XMLSchema::string< char > ()),
    resourceName_ (new ::XMLSchema::string< char > ()),
    resourceValue_ (new ::CIAO::Config_Handlers::Any ()),
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
    PlanConnectionDescription ()
    :
    name_ (new ::XMLSchema::string< char > ()),
    regulator__ ()
    {
      name_->container (this);
    }

    PlanConnectionDescription::
    PlanConnectionDescription (::CIAO::Config_Handlers::PlanConnectionDescription const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    source_ (s.source_.get () ? new ::XMLSchema::string< char > (*s.source_) : 0),
    deployRequirement_ (s.deployRequirement_.get () ? new ::CIAO::Config_Handlers::Requirement (*s.deployRequirement_) : 0),
    deployedResource_ (s.deployedResource_.get () ? new ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription (*s.deployedResource_) : 0),
    regulator__ ()
    {
      name_->container (this);
      if (source_.get ()) source_->container (this);
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

      if (deployedResource_.get ()) deployedResource_->container (this);
    }

    ::CIAO::Config_Handlers::PlanConnectionDescription& PlanConnectionDescription::
    operator= (::CIAO::Config_Handlers::PlanConnectionDescription const& s)
    {
      name (s.name ());

      if (s.source_.get ()) source (*(s.source_));
      else source_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

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

      if (s.deployedResource_.get ()) deployedResource (*(s.deployedResource_));
      else deployedResource_ = ::std::auto_ptr< ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription > (0);

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
    bool PlanConnectionDescription::
    deployRequirement_p () const
    {
      return deployRequirement_.get () != 0;
    }

    ::CIAO::Config_Handlers::Requirement const& PlanConnectionDescription::
    deployRequirement () const
    {
      return *deployRequirement_;
    }

    ::CIAO::Config_Handlers::Requirement& PlanConnectionDescription::
    deployRequirement ()
    {
      return *deployRequirement_;
    }

    void PlanConnectionDescription::
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
    bool PlanConnectionDescription::
    deployedResource_p () const
    {
      return deployedResource_.get () != 0;
    }

    ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription const& PlanConnectionDescription::
    deployedResource () const
    {
      return *deployedResource_;
    }

    ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription& PlanConnectionDescription::
    deployedResource ()
    {
      return *deployedResource_;
    }

    void PlanConnectionDescription::
    deployedResource (::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription const& e)
    {
      if (deployedResource_.get ())
      {
        *deployedResource_ = e;
      }

      else
      {
        deployedResource_ = ::std::auto_ptr< ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription > (new ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription (e));
        deployedResource_->container (this);
      }
    }


    // ImplementationDependency
    // 

    ImplementationDependency::
    ImplementationDependency ()
    :
    requiredType_ (new ::XMLSchema::string< char > ()),
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
    Capability ()
    :
    name_ (new ::XMLSchema::string< char > ()),
    resourceType_ (new ::XMLSchema::string< char > ()),
    property_ (new ::CIAO::Config_Handlers::SatisfierProperty ()),
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
    ImplementationRequirement ()
    :
    resourceUsage_ (new ::CIAO::Config_Handlers::ResourceUsageKind ()),
    resourcePort_ (new ::XMLSchema::string< char > ()),
    componentPort_ (new ::XMLSchema::string< char > ()),
    resourceType_ (new ::XMLSchema::string< char > ()),
    name_ (new ::XMLSchema::string< char > ()),
    property_ (new ::CIAO::Config_Handlers::Property ()),
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
    ComponentPackageReference ()
    :
    requiredUUID_ (new ::XMLSchema::string< char > ()),
    requiredName_ (new ::XMLSchema::string< char > ()),
    requiredType_ (new ::XMLSchema::string< char > ()),
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
    SubcomponentPortEndpoint ()
    :
    portName_ (new ::XMLSchema::string< char > ()),
    instance_ (new ::XMLSchema::IDREF< char > ()),
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
    AssemblyConnectionDescription ()
    :
    name_ (new ::XMLSchema::string< char > ()),
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
    :
    Base__ (e),
    regulator__ ()
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
    :
    Base__ (e),
    regulator__ ()
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
    :
    Base__ (e),
    type_ (new ::CIAO::Config_Handlers::DataType ()),
    value_ (new ::CIAO::Config_Handlers::DataValue ()),
    regulator__ ()
    {
      type_->container (this);
      value_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "type")
        {
          ::CIAO::Config_Handlers::DataType t (e);
          type (t);
        }

        else if (n == "value")
        {
          ::CIAO::Config_Handlers::DataValue t (e);
          value (t);
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
    :
    Base__ (e),
    name_ (new ::XMLSchema::string< char > ()),
    value_ (new ::CIAO::Config_Handlers::Any ()),
    regulator__ ()
    {
      name_->container (this);
      value_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "value")
        {
          ::CIAO::Config_Handlers::Any t (e);
          value (t);
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
    :
    Base__ (e),
    name_ (new ::XMLSchema::string< char > ()),
    label_ (new ::XMLSchema::string< char > ()),
    regulator__ ()
    {
      name_->container (this);
      label_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "label")
        {
          ::XMLSchema::string< char > t (e);
          label (t);
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
    :
    Base__ (e),
    name_ (new ::XMLSchema::string< char > ()),
    kind_ (new ::CIAO::Config_Handlers::SatisfierPropertyKind ()),
    value_ (new ::CIAO::Config_Handlers::Any ()),
    regulator__ ()
    {
      name_->container (this);
      kind_->container (this);
      value_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "kind")
        {
          ::CIAO::Config_Handlers::SatisfierPropertyKind t (e);
          kind (t);
        }

        else if (n == "value")
        {
          ::CIAO::Config_Handlers::Any t (e);
          value (t);
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
    :
    Base__ (e),
    name_ (new ::XMLSchema::string< char > ()),
    resourceType_ (new ::XMLSchema::string< char > ()),
    property_ (new ::CIAO::Config_Handlers::SatisfierProperty ()),
    regulator__ ()
    {
      name_->container (this);
      resourceType_->container (this);
      property_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "resourceType")
        {
          ::XMLSchema::string< char > t (e);
          resourceType (t);
        }

        else if (n == "property")
        {
          ::CIAO::Config_Handlers::SatisfierProperty t (e);
          property (t);
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
    :
    Base__ (e),
    name_ (new ::XMLSchema::string< char > ()),
    resourceType_ (new ::XMLSchema::string< char > ()),
    node_ (new ::CIAO::Config_Handlers::Node ()),
    property_ (new ::CIAO::Config_Handlers::SatisfierProperty ()),
    regulator__ ()
    {
      name_->container (this);
      resourceType_->container (this);
      node_->container (this);
      property_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "resourceType")
        {
          ::XMLSchema::string< char > t (e);
          resourceType (t);
        }

        else if (n == "node")
        {
          ::CIAO::Config_Handlers::Node t (e);
          node (t);
        }

        else if (n == "property")
        {
          ::CIAO::Config_Handlers::SatisfierProperty t (e);
          property (t);
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
    :
    Base__ (e),
    resourceType_ (new ::XMLSchema::string< char > ()),
    name_ (new ::XMLSchema::string< char > ()),
    property_ (new ::CIAO::Config_Handlers::Property ()),
    regulator__ ()
    {
      resourceType_->container (this);
      name_->container (this);
      property_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "resourceType")
        {
          ::XMLSchema::string< char > t (e);
          resourceType (t);
        }

        else if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "property")
        {
          ::CIAO::Config_Handlers::Property t (e);
          property (t);
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
    :
    Base__ (e),
    requirementName_ (new ::XMLSchema::string< char > ()),
    resourceName_ (new ::XMLSchema::string< char > ()),
    resourceValue_ (new ::CIAO::Config_Handlers::Any ()),
    regulator__ ()
    {
      requirementName_->container (this);
      resourceName_->container (this);
      resourceValue_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "requirementName")
        {
          ::XMLSchema::string< char > t (e);
          requirementName (t);
        }

        else if (n == "resourceName")
        {
          ::XMLSchema::string< char > t (e);
          resourceName (t);
        }

        else if (n == "resourceValue")
        {
          ::CIAO::Config_Handlers::Any t (e);
          resourceValue (t);
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
    :
    Base__ (e),
    name_ (new ::XMLSchema::string< char > ()),
    source_ (new ::XMLSchema::string< char > ()),
    regulator__ ()
    {
      name_->container (this);
      source_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "source")
        {
          ::XMLSchema::string< char > t (e);
          source (t);
        }

        else if (n == "node")
        {
          ::XMLSchema::string< char > t (e);
          node (t);
        }

        else if (n == "location")
        {
          ::XMLSchema::string< char > t (e);
          add_location (t);
        }

        else if (n == "execParameter")
        {
          ::CIAO::Config_Handlers::Property t (e);
          execParameter (t);
        }

        else if (n == "deployRequirement")
        {
          ::CIAO::Config_Handlers::Requirement t (e);
          deployRequirement (t);
        }

        else if (n == "deployedResource")
        {
          ::CIAO::Config_Handlers::ResourceDeploymentDescription t (e);
          deployedResource (t);
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
    :
    Base__ (e),
    name_ (new ::XMLSchema::string< char > ()),
    source_ (new ::XMLSchema::string< char > ()),
    regulator__ ()
    {
      name_->container (this);
      source_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "source")
        {
          ::XMLSchema::string< char > t (e);
          source (t);
        }

        else if (n == "artifact")
        {
          ::XMLSchema::IDREF< char > t (e);
          add_artifact (t);
        }

        else if (n == "execParameter")
        {
          ::CIAO::Config_Handlers::Property t (e);
          execParameter (t);
        }

        else if (n == "deployRequirement")
        {
          ::CIAO::Config_Handlers::Requirement t (e);
          deployRequirement (t);
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
    :
    Base__ (e),
    resourceUsage_ (new ::CIAO::Config_Handlers::ResourceUsageKind ()),
    requirementName_ (new ::XMLSchema::string< char > ()),
    resourceName_ (new ::XMLSchema::string< char > ()),
    resourceValue_ (new ::CIAO::Config_Handlers::Any ()),
    regulator__ ()
    {
      resourceUsage_->container (this);
      requirementName_->container (this);
      resourceName_->container (this);
      resourceValue_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "resourceUsage")
        {
          ::CIAO::Config_Handlers::ResourceUsageKind t (e);
          resourceUsage (t);
        }

        else if (n == "requirementName")
        {
          ::XMLSchema::string< char > t (e);
          requirementName (t);
        }

        else if (n == "resourceName")
        {
          ::XMLSchema::string< char > t (e);
          resourceName (t);
        }

        else if (n == "resourceValue")
        {
          ::CIAO::Config_Handlers::Any t (e);
          resourceValue (t);
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
    :
    Base__ (e),
    name_ (new ::XMLSchema::string< char > ()),
    node_ (new ::XMLSchema::string< char > ()),
    source_ (new ::XMLSchema::string< char > ()),
    implementation_ (new ::XMLSchema::IDREF< char > ()),
    regulator__ ()
    {
      name_->container (this);
      node_->container (this);
      source_->container (this);
      implementation_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "node")
        {
          ::XMLSchema::string< char > t (e);
          node (t);
        }

        else if (n == "source")
        {
          ::XMLSchema::string< char > t (e);
          source (t);
        }

        else if (n == "implementation")
        {
          ::XMLSchema::IDREF< char > t (e);
          implementation (t);
        }

        else if (n == "configProperty")
        {
          ::CIAO::Config_Handlers::Property t (e);
          configProperty (t);
        }

        else if (n == "deployedResource")
        {
          ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription t (e);
          deployedResource (t);
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
    :
    Base__ (e),
    name_ (new ::XMLSchema::string< char > ()),
    specificType_ (new ::XMLSchema::string< char > ()),
    supportedType_ (new ::XMLSchema::string< char > ()),
    provider_ (new ::XMLSchema::string< char > ()),
    exclusiveProvider_ (new ::XMLSchema::string< char > ()),
    exclusiveUser_ (new ::XMLSchema::string< char > ()),
    optional_ (new ::XMLSchema::string< char > ()),
    kind_ (new ::CIAO::Config_Handlers::CCMComponentPortKind ()),
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

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "specificType")
        {
          ::XMLSchema::string< char > t (e);
          specificType (t);
        }

        else if (n == "supportedType")
        {
          ::XMLSchema::string< char > t (e);
          supportedType (t);
        }

        else if (n == "provider")
        {
          ::XMLSchema::string< char > t (e);
          provider (t);
        }

        else if (n == "exclusiveProvider")
        {
          ::XMLSchema::string< char > t (e);
          exclusiveProvider (t);
        }

        else if (n == "exclusiveUser")
        {
          ::XMLSchema::string< char > t (e);
          exclusiveUser (t);
        }

        else if (n == "optional")
        {
          ::XMLSchema::string< char > t (e);
          optional (t);
        }

        else if (n == "kind")
        {
          ::CIAO::Config_Handlers::CCMComponentPortKind t (e);
          kind (t);
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
    :
    Base__ (e),
    name_ (new ::XMLSchema::string< char > ()),
    type_ (new ::CIAO::Config_Handlers::DataType ()),
    regulator__ ()
    {
      name_->container (this);
      type_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "type")
        {
          ::CIAO::Config_Handlers::DataType t (e);
          type (t);
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
    :
    Base__ (e),
    portName_ (new ::XMLSchema::string< char > ()),
    regulator__ ()
    {
      portName_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "portName")
        {
          ::XMLSchema::string< char > t (e);
          portName (t);
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
    :
    Base__ (e),
    portName_ (new ::XMLSchema::string< char > ()),
    kind_ (new ::CIAO::Config_Handlers::CCMComponentPortKind ()),
    instance_ (new ::XMLSchema::IDREF< char > ()),
    regulator__ ()
    {
      portName_->container (this);
      kind_->container (this);
      instance_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "portName")
        {
          ::XMLSchema::string< char > t (e);
          portName (t);
        }

        else if (n == "provider")
        {
          ::XMLSchema::string< char > t (e);
          provider (t);
        }

        else if (n == "kind")
        {
          ::CIAO::Config_Handlers::CCMComponentPortKind t (e);
          kind (t);
        }

        else if (n == "instance")
        {
          ::XMLSchema::IDREF< char > t (e);
          instance (t);
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
    :
    Base__ (e),
    location_ (new ::XMLSchema::string< char > ()),
    regulator__ ()
    {
      location_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "location")
        {
          ::XMLSchema::string< char > t (e);
          location (t);
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
    :
    Base__ (e),
    targetName_ (new ::XMLSchema::string< char > ()),
    requirementName_ (new ::XMLSchema::string< char > ()),
    resourceName_ (new ::XMLSchema::string< char > ()),
    resourceValue_ (new ::CIAO::Config_Handlers::Any ()),
    regulator__ ()
    {
      targetName_->container (this);
      requirementName_->container (this);
      resourceName_->container (this);
      resourceValue_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "targetName")
        {
          ::XMLSchema::string< char > t (e);
          targetName (t);
        }

        else if (n == "requirementName")
        {
          ::XMLSchema::string< char > t (e);
          requirementName (t);
        }

        else if (n == "resourceName")
        {
          ::XMLSchema::string< char > t (e);
          resourceName (t);
        }

        else if (n == "resourceValue")
        {
          ::CIAO::Config_Handlers::Any t (e);
          resourceValue (t);
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
    :
    Base__ (e),
    name_ (new ::XMLSchema::string< char > ()),
    regulator__ ()
    {
      name_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "source")
        {
          ::XMLSchema::string< char > t (e);
          source (t);
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
          deployedResource (t);
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
    :
    Base__ (e),
    requiredType_ (new ::XMLSchema::string< char > ()),
    regulator__ ()
    {
      requiredType_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "requiredType")
        {
          ::XMLSchema::string< char > t (e);
          requiredType (t);
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
    :
    Base__ (e),
    name_ (new ::XMLSchema::string< char > ()),
    resourceType_ (new ::XMLSchema::string< char > ()),
    property_ (new ::CIAO::Config_Handlers::SatisfierProperty ()),
    regulator__ ()
    {
      name_->container (this);
      resourceType_->container (this);
      property_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "resourceType")
        {
          ::XMLSchema::string< char > t (e);
          resourceType (t);
        }

        else if (n == "property")
        {
          ::CIAO::Config_Handlers::SatisfierProperty t (e);
          property (t);
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
    :
    Base__ (e),
    resourceUsage_ (new ::CIAO::Config_Handlers::ResourceUsageKind ()),
    resourcePort_ (new ::XMLSchema::string< char > ()),
    componentPort_ (new ::XMLSchema::string< char > ()),
    resourceType_ (new ::XMLSchema::string< char > ()),
    name_ (new ::XMLSchema::string< char > ()),
    property_ (new ::CIAO::Config_Handlers::Property ()),
    regulator__ ()
    {
      resourceUsage_->container (this);
      resourcePort_->container (this);
      componentPort_->container (this);
      resourceType_->container (this);
      name_->container (this);
      property_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "resourceUsage")
        {
          ::CIAO::Config_Handlers::ResourceUsageKind t (e);
          resourceUsage (t);
        }

        else if (n == "resourcePort")
        {
          ::XMLSchema::string< char > t (e);
          resourcePort (t);
        }

        else if (n == "componentPort")
        {
          ::XMLSchema::string< char > t (e);
          componentPort (t);
        }

        else if (n == "resourceType")
        {
          ::XMLSchema::string< char > t (e);
          resourceType (t);
        }

        else if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "property")
        {
          ::CIAO::Config_Handlers::Property t (e);
          property (t);
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
    :
    Base__ (e),
    requiredUUID_ (new ::XMLSchema::string< char > ()),
    requiredName_ (new ::XMLSchema::string< char > ()),
    requiredType_ (new ::XMLSchema::string< char > ()),
    regulator__ ()
    {
      requiredUUID_->container (this);
      requiredName_->container (this);
      requiredType_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "requiredUUID")
        {
          ::XMLSchema::string< char > t (e);
          requiredUUID (t);
        }

        else if (n == "requiredName")
        {
          ::XMLSchema::string< char > t (e);
          requiredName (t);
        }

        else if (n == "requiredType")
        {
          ::XMLSchema::string< char > t (e);
          requiredType (t);
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
    :
    Base__ (e),
    portName_ (new ::XMLSchema::string< char > ()),
    instance_ (new ::XMLSchema::IDREF< char > ()),
    regulator__ ()
    {
      portName_->container (this);
      instance_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "portName")
        {
          ::XMLSchema::string< char > t (e);
          portName (t);
        }

        else if (n == "instance")
        {
          ::XMLSchema::IDREF< char > t (e);
          instance (t);
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
    :
    Base__ (e),
    name_ (new ::XMLSchema::string< char > ()),
    regulator__ ()
    {
      name_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
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

