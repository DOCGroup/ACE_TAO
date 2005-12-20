//$Id$
#ifndef BASIC_DEPLOYMENT_DATA_HPP
#define BASIC_DEPLOYMENT_DATA_HPP

// Forward declarations.
//
namespace CIAO
{
  namespace Config_Handlers
  {
    class TCKind;
    class DataType;
    class DataValue;
    class Any;
    class Property;
    class Node;
    class SatisfierPropertyKind;
    class SatisfierProperty;
    class Resource;
    class SharedResource;
    class Requirement;
    class ResourceDeploymentDescription;
    class ArtifactDeploymentDescription;
    class MonolithicDeploymentDescription;
    class ResourceUsageKind;
    class InstanceResourceDeploymentDescription;
    class InstanceDeploymentDescription;
    class CCMComponentPortKind;
    class ComponentPortDescription;
    class ComponentPropertyDescription;
    class ComponentExternalPortEndpoint;
    class PlanSubcomponentPortEndpoint;
    class ExternalReferenceEndpoint;
    class ConnectionResourceDeploymentDescription;
    class PlanConnectionDescription;
    class ImplementationDependency;
    class Capability;
    class ImplementationRequirement;
    class ComponentPackageReference;
    class SubcomponentPortEndpoint;
    class AssemblyConnectionDescription;
  }
}

#include <memory>
#include <vector>
#include "XSCRT/XMLSchema.hpp"
#include "XSCRT/Parser.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    class TCKind : public ::XSCRT::Type
    {
      public:
      TCKind ();
      TCKind (::XSCRT::XML::Element< char > const&);
      TCKind (::XSCRT::XML::Attribute< char > const&);

      static TCKind const tk_null;
      static TCKind const tk_void;
      static TCKind const tk_short;
      static TCKind const tk_long;
      static TCKind const tk_ushort;
      static TCKind const tk_ulong;
      static TCKind const tk_float;
      static TCKind const tk_double;
      static TCKind const tk_boolean;
      static TCKind const tk_char;
      static TCKind const tk_octet;
      static TCKind const tk_any;
      static TCKind const tk_TypeCode;
      static TCKind const tk_Principal;
      static TCKind const tk_objref;
      static TCKind const tk_struct;
      static TCKind const tk_union;
      static TCKind const tk_enum;
      static TCKind const tk_string;
      static TCKind const tk_sequence;
      static TCKind const tk_array;
      static TCKind const tk_alias;
      static TCKind const tk_except;
      static TCKind const tk_longlong;
      static TCKind const tk_ulonglong;
      static TCKind const tk_longdouble;
      static TCKind const tk_wchar;
      static TCKind const tk_wstring;
      static TCKind const tk_wfixed;
      static TCKind const tk_value;
      static TCKind const tk_value_box;
      static TCKind const tk_native;
      static TCKind const tk_abstract_interface;
      static TCKind const tk_local_interface;
      static TCKind const tk_component;
      static TCKind const tk_home;
      static TCKind const tk_event;

      enum Value
      {
        tk_null_l,
        tk_void_l,
        tk_short_l,
        tk_long_l,
        tk_ushort_l,
        tk_ulong_l,
        tk_float_l,
        tk_double_l,
        tk_boolean_l,
        tk_char_l,
        tk_octet_l,
        tk_any_l,
        tk_TypeCode_l,
        tk_Principal_l,
        tk_objref_l,
        tk_struct_l,
        tk_union_l,
        tk_enum_l,
        tk_string_l,
        tk_sequence_l,
        tk_array_l,
        tk_alias_l,
        tk_except_l,
        tk_longlong_l,
        tk_ulonglong_l,
        tk_longdouble_l,
        tk_wchar_l,
        tk_wstring_l,
        tk_wfixed_l,
        tk_value_l,
        tk_value_box_l,
        tk_native_l,
        tk_abstract_interface_l,
        tk_local_interface_l,
        tk_component_l,
        tk_home_l,
        tk_event_l,
      };


      Value
      integral () const;

      friend bool
      operator== (TCKind const& a, TCKind const& b);

      friend bool
      operator!= (TCKind const& a, TCKind const& b);

      private:
      TCKind (Value v);

      Value v_;
    };

    class DataType : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // kind
      //
      public:
      bool kind_p () const;
      ::CIAO::Config_Handlers::TCKind const& kind () const;
      ::CIAO::Config_Handlers::TCKind& kind ();
      void kind (::CIAO::Config_Handlers::TCKind const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::TCKind > kind_;

      public:
      DataType ();
      DataType (::XSCRT::XML::Element< char > const&);
      DataType (DataType const& s);

      DataType&
      operator= (DataType const& s);

      private:
      char regulator__;
    };

    class DataValue : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // short
      //
      public:
      bool short_p () const;
      ::XMLSchema::short_ const& short_ () const;
      ::XMLSchema::short_& short_ ();
      void short_ (::XMLSchema::short_ const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::short_ > short__;

      // long
      //
      public:
      bool long_p () const;
      ::XMLSchema::int_ const& long_ () const;
      ::XMLSchema::int_& long_ ();
      void long_ (::XMLSchema::int_ const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::int_ > long__;

      // ushort
      //
      public:
      bool ushort_p () const;
      ::XMLSchema::unsignedShort const& ushort () const;
      ::XMLSchema::unsignedShort& ushort ();
      void ushort (::XMLSchema::unsignedShort const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::unsignedShort > ushort_;

      // ulong
      //
      public:
      bool ulong_p () const;
      ::XMLSchema::unsignedInt const& ulong () const;
      ::XMLSchema::unsignedInt& ulong ();
      void ulong (::XMLSchema::unsignedInt const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::unsignedInt > ulong_;

      // float
      //
      public:
      bool float_p () const;
      ::XMLSchema::float_ const& float_ () const;
      ::XMLSchema::float_& float_ ();
      void float_ (::XMLSchema::float_ const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::float_ > float__;

      // double
      //
      public:
      bool double_p () const;
      ::XMLSchema::double_ const& double_ () const;
      ::XMLSchema::double_& double_ ();
      void double_ (::XMLSchema::double_ const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::double_ > double__;

      // boolean
      //
      public:
      bool boolean_p () const;
      ::XMLSchema::boolean const& boolean () const;
      ::XMLSchema::boolean& boolean ();
      void boolean (::XMLSchema::boolean const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::boolean > boolean_;

      // octet
      //
      public:
      bool octet_p () const;
      ::XMLSchema::unsignedByte const& octet () const;
      ::XMLSchema::unsignedByte& octet ();
      void octet (::XMLSchema::unsignedByte const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::unsignedByte > octet_;

      // objref
      //
      public:
      bool objref_p () const;
      ::XMLSchema::string< char > const& objref () const;
      ::XMLSchema::string< char >& objref ();
      void objref (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > objref_;

      // enum
      //
      public:
      bool enum_p () const;
      ::XMLSchema::string< char > const& enum_ () const;
      ::XMLSchema::string< char >& enum_ ();
      void enum_ (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > enum__;

      // string
      //
      public:
      bool string_p () const;
      ::XMLSchema::string< char > const& string () const;
      ::XMLSchema::string< char >& string ();
      void string (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > string_;

      // longlong
      //
      public:
      bool longlong_p () const;
      ::XMLSchema::long_ const& longlong () const;
      ::XMLSchema::long_& longlong ();
      void longlong (::XMLSchema::long_ const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::long_ > longlong_;

      // ulonglong
      //
      public:
      bool ulonglong_p () const;
      ::XMLSchema::unsignedLong const& ulonglong () const;
      ::XMLSchema::unsignedLong& ulonglong ();
      void ulonglong (::XMLSchema::unsignedLong const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::unsignedLong > ulonglong_;

      // longdouble
      //
      public:
      bool longdouble_p () const;
      ::XMLSchema::double_ const& longdouble () const;
      ::XMLSchema::double_& longdouble ();
      void longdouble (::XMLSchema::double_ const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::double_ > longdouble_;

      // fixed
      //
      public:
      bool fixed_p () const;
      ::XMLSchema::string< char > const& fixed () const;
      ::XMLSchema::string< char >& fixed ();
      void fixed (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > fixed_;

      // typecode
      //
      public:
      bool typecode_p () const;
      ::CIAO::Config_Handlers::DataType const& typecode () const;
      ::CIAO::Config_Handlers::DataType& typecode ();
      void typecode (::CIAO::Config_Handlers::DataType const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::DataType > typecode_;

      public:
      DataValue ();
      DataValue (::XSCRT::XML::Element< char > const&);
      DataValue (DataValue const& s);

      DataValue&
      operator= (DataValue const& s);

      private:
      char regulator__;
    };

    class Any : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // type
      //
      public:
      ::CIAO::Config_Handlers::DataType const& type () const;
      ::CIAO::Config_Handlers::DataType& type ();
      void type (::CIAO::Config_Handlers::DataType const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::DataType > type_;

      // value
      //
      public:
      ::CIAO::Config_Handlers::DataValue const& value () const;
      ::CIAO::Config_Handlers::DataValue& value ();
      void value (::CIAO::Config_Handlers::DataValue const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::DataValue > value_;

      public:
      Any ();
      Any (::XSCRT::XML::Element< char > const&);
      Any (Any const& s);

      Any&
      operator= (Any const& s);

      private:
      char regulator__;
    };

    class Property : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // value
      //
      public:
      ::CIAO::Config_Handlers::Any const& value () const;
      ::CIAO::Config_Handlers::Any& value ();
      void value (::CIAO::Config_Handlers::Any const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Any > value_;

      public:
      Property ();
      Property (::XSCRT::XML::Element< char > const&);
      Property (Property const& s);

      Property&
      operator= (Property const& s);

      private:
      char regulator__;
    };

    class Node : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // label
      //
      public:
      ::XMLSchema::string< char > const& label () const;
      ::XMLSchema::string< char >& label ();
      void label (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > label_;

      public:
      Node ();
      Node (::XSCRT::XML::Element< char > const&);
      Node (Node const& s);

      Node&
      operator= (Node const& s);

      private:
      char regulator__;
    };

    class SatisfierPropertyKind : public ::XSCRT::Type
    {
      public:
      SatisfierPropertyKind ();
      SatisfierPropertyKind (::XSCRT::XML::Element< char > const&);
      SatisfierPropertyKind (::XSCRT::XML::Attribute< char > const&);

      static SatisfierPropertyKind const Quantity;
      static SatisfierPropertyKind const Capacity;
      static SatisfierPropertyKind const Minimum;
      static SatisfierPropertyKind const Maximum;
      static SatisfierPropertyKind const Attribute;
      static SatisfierPropertyKind const Selection;

      enum Value
      {
        Quantity_l,
        Capacity_l,
        Minimum_l,
        Maximum_l,
        Attribute_l,
        Selection_l,
      };


      Value
      integral () const;

      friend bool
      operator== (SatisfierPropertyKind const& a, SatisfierPropertyKind const& b);

      friend bool
      operator!= (SatisfierPropertyKind const& a, SatisfierPropertyKind const& b);

      private:
      SatisfierPropertyKind (Value v);

      Value v_;
    };

    class SatisfierProperty : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // kind
      //
      public:
      ::CIAO::Config_Handlers::SatisfierPropertyKind const& kind () const;
      ::CIAO::Config_Handlers::SatisfierPropertyKind& kind ();
      void kind (::CIAO::Config_Handlers::SatisfierPropertyKind const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::SatisfierPropertyKind > kind_;

      // value
      //
      public:
      ::CIAO::Config_Handlers::Any const& value () const;
      ::CIAO::Config_Handlers::Any& value ();
      void value (::CIAO::Config_Handlers::Any const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Any > value_;

      public:
      SatisfierProperty ();
      SatisfierProperty (::XSCRT::XML::Element< char > const&);
      SatisfierProperty (SatisfierProperty const& s);

      SatisfierProperty&
      operator= (SatisfierProperty const& s);

      private:
      char regulator__;
    };

    class Resource : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // resourceType
      //
      public:
      ::XMLSchema::string< char > const& resourceType () const;
      ::XMLSchema::string< char >& resourceType ();
      void resourceType (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > resourceType_;

      // property
      //
      public:
      ::CIAO::Config_Handlers::SatisfierProperty const& property () const;
      ::CIAO::Config_Handlers::SatisfierProperty& property ();
      void property (::CIAO::Config_Handlers::SatisfierProperty const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::SatisfierProperty > property_;

      public:
      Resource ();
      Resource (::XSCRT::XML::Element< char > const&);
      Resource (Resource const& s);

      Resource&
      operator= (Resource const& s);

      private:
      char regulator__;
    };

    class SharedResource : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // resourceType
      //
      public:
      ::XMLSchema::string< char > const& resourceType () const;
      ::XMLSchema::string< char >& resourceType ();
      void resourceType (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > resourceType_;

      // node
      //
      public:
      ::CIAO::Config_Handlers::Node const& node () const;
      ::CIAO::Config_Handlers::Node& node ();
      void node (::CIAO::Config_Handlers::Node const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Node > node_;

      // property
      //
      public:
      ::CIAO::Config_Handlers::SatisfierProperty const& property () const;
      ::CIAO::Config_Handlers::SatisfierProperty& property ();
      void property (::CIAO::Config_Handlers::SatisfierProperty const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::SatisfierProperty > property_;

      public:
      SharedResource ();
      SharedResource (::XSCRT::XML::Element< char > const&);
      SharedResource (SharedResource const& s);

      SharedResource&
      operator= (SharedResource const& s);

      private:
      char regulator__;
    };

    class Requirement : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // resourceType
      //
      public:
      ::XMLSchema::string< char > const& resourceType () const;
      ::XMLSchema::string< char >& resourceType ();
      void resourceType (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > resourceType_;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // property
      //
      public:
      ::CIAO::Config_Handlers::Property const& property () const;
      ::CIAO::Config_Handlers::Property& property ();
      void property (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > property_;

      public:
      Requirement ();
      Requirement (::XSCRT::XML::Element< char > const&);
      Requirement (Requirement const& s);

      Requirement&
      operator= (Requirement const& s);

      private:
      char regulator__;
    };

    class ResourceDeploymentDescription : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // requirementName
      //
      public:
      ::XMLSchema::string< char > const& requirementName () const;
      ::XMLSchema::string< char >& requirementName ();
      void requirementName (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > requirementName_;

      // resourceName
      //
      public:
      ::XMLSchema::string< char > const& resourceName () const;
      ::XMLSchema::string< char >& resourceName ();
      void resourceName (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > resourceName_;

      // resourceValue
      //
      public:
      ::CIAO::Config_Handlers::Any const& resourceValue () const;
      ::CIAO::Config_Handlers::Any& resourceValue ();
      void resourceValue (::CIAO::Config_Handlers::Any const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Any > resourceValue_;

      public:
      ResourceDeploymentDescription ();
      ResourceDeploymentDescription (::XSCRT::XML::Element< char > const&);
      ResourceDeploymentDescription (ResourceDeploymentDescription const& s);

      ResourceDeploymentDescription&
      operator= (ResourceDeploymentDescription const& s);

      private:
      char regulator__;
    };

    class ArtifactDeploymentDescription : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // source
      //
      public:
      ::XMLSchema::string< char > const& source () const;
      ::XMLSchema::string< char >& source ();
      void source (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > source_;

      // node
      //
      public:
      bool node_p () const;
      ::XMLSchema::string< char > const& node () const;
      ::XMLSchema::string< char >& node ();
      void node (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > node_;

      // location
      //
      public:
      typedef ::std::vector< ::XMLSchema::string< char > >::iterator location_iterator;
      typedef ::std::vector< ::XMLSchema::string< char > >::const_iterator location_const_iterator;
      location_iterator begin_location ();
      location_iterator end_location ();
      location_const_iterator begin_location () const;
      location_const_iterator end_location () const;
      void add_location (::XMLSchema::string< char > const& );

      protected:
      ::std::vector< ::XMLSchema::string< char > > location_;

      // execParameter
      //
      public:
      bool execParameter_p () const;
      ::CIAO::Config_Handlers::Property const& execParameter () const;
      ::CIAO::Config_Handlers::Property& execParameter ();
      void execParameter (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > execParameter_;

      // deployRequirement
      //
      public:
      bool deployRequirement_p () const;
      ::CIAO::Config_Handlers::Requirement const& deployRequirement () const;
      ::CIAO::Config_Handlers::Requirement& deployRequirement ();
      void deployRequirement (::CIAO::Config_Handlers::Requirement const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > deployRequirement_;

      // deployedResource
      //
      public:
      bool deployedResource_p () const;
      ::CIAO::Config_Handlers::ResourceDeploymentDescription const& deployedResource () const;
      ::CIAO::Config_Handlers::ResourceDeploymentDescription& deployedResource ();
      void deployedResource (::CIAO::Config_Handlers::ResourceDeploymentDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ResourceDeploymentDescription > deployedResource_;

      // id
      //
      public:
      bool id_p () const;
      ::XMLSchema::ID< char > const& id () const;
      ::XMLSchema::ID< char >& id ();
      void id (::XMLSchema::ID< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::ID< char > > id_;

      public:
      ArtifactDeploymentDescription ();
      ArtifactDeploymentDescription (::XSCRT::XML::Element< char > const&);
      ArtifactDeploymentDescription (ArtifactDeploymentDescription const& s);

      ArtifactDeploymentDescription&
      operator= (ArtifactDeploymentDescription const& s);

      private:
      char regulator__;
    };

    class MonolithicDeploymentDescription : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // source
      //
      public:
      ::XMLSchema::string< char > const& source () const;
      ::XMLSchema::string< char >& source ();
      void source (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > source_;

      // artifact
      //
      public:
      typedef ::std::vector< ::XMLSchema::IDREF< char > >::iterator artifact_iterator;
      typedef ::std::vector< ::XMLSchema::IDREF< char > >::const_iterator artifact_const_iterator;
      artifact_iterator begin_artifact ();
      artifact_iterator end_artifact ();
      artifact_const_iterator begin_artifact () const;
      artifact_const_iterator end_artifact () const;
      void add_artifact (::XMLSchema::IDREF< char > const& );

      protected:
      ::std::vector< ::XMLSchema::IDREF< char > > artifact_;

      // execParameter
      //
      public:
      bool execParameter_p () const;
      ::CIAO::Config_Handlers::Property const& execParameter () const;
      ::CIAO::Config_Handlers::Property& execParameter ();
      void execParameter (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > execParameter_;

      // deployRequirement
      //
      public:
      bool deployRequirement_p () const;
      ::CIAO::Config_Handlers::Requirement const& deployRequirement () const;
      ::CIAO::Config_Handlers::Requirement& deployRequirement ();
      void deployRequirement (::CIAO::Config_Handlers::Requirement const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > deployRequirement_;

      // id
      //
      public:
      bool id_p () const;
      ::XMLSchema::ID< char > const& id () const;
      ::XMLSchema::ID< char >& id ();
      void id (::XMLSchema::ID< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::ID< char > > id_;

      public:
      MonolithicDeploymentDescription ();
      MonolithicDeploymentDescription (::XSCRT::XML::Element< char > const&);
      MonolithicDeploymentDescription (MonolithicDeploymentDescription const& s);

      MonolithicDeploymentDescription&
      operator= (MonolithicDeploymentDescription const& s);

      private:
      char regulator__;
    };

    class ResourceUsageKind : public ::XSCRT::Type
    {
      public:
      ResourceUsageKind ();
      ResourceUsageKind (::XSCRT::XML::Element< char > const&);
      ResourceUsageKind (::XSCRT::XML::Attribute< char > const&);

      static ResourceUsageKind const None;
      static ResourceUsageKind const InstanceUsesResource;
      static ResourceUsageKind const ResourceUsesInstance;
      static ResourceUsageKind const PortUsesResource;
      static ResourceUsageKind const ResourceUsesPort;

      enum Value
      {
        None_l,
        InstanceUsesResource_l,
        ResourceUsesInstance_l,
        PortUsesResource_l,
        ResourceUsesPort_l,
      };


      Value
      integral () const;

      friend bool
      operator== (ResourceUsageKind const& a, ResourceUsageKind const& b);

      friend bool
      operator!= (ResourceUsageKind const& a, ResourceUsageKind const& b);

      private:
      ResourceUsageKind (Value v);

      Value v_;
    };

    class InstanceResourceDeploymentDescription : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // resourceUsage
      //
      public:
      ::CIAO::Config_Handlers::ResourceUsageKind const& resourceUsage () const;
      ::CIAO::Config_Handlers::ResourceUsageKind& resourceUsage ();
      void resourceUsage (::CIAO::Config_Handlers::ResourceUsageKind const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ResourceUsageKind > resourceUsage_;

      // requirementName
      //
      public:
      ::XMLSchema::string< char > const& requirementName () const;
      ::XMLSchema::string< char >& requirementName ();
      void requirementName (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > requirementName_;

      // resourceName
      //
      public:
      ::XMLSchema::string< char > const& resourceName () const;
      ::XMLSchema::string< char >& resourceName ();
      void resourceName (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > resourceName_;

      // resourceValue
      //
      public:
      ::CIAO::Config_Handlers::Any const& resourceValue () const;
      ::CIAO::Config_Handlers::Any& resourceValue ();
      void resourceValue (::CIAO::Config_Handlers::Any const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Any > resourceValue_;

      public:
      InstanceResourceDeploymentDescription ();
      InstanceResourceDeploymentDescription (::XSCRT::XML::Element< char > const&);
      InstanceResourceDeploymentDescription (InstanceResourceDeploymentDescription const& s);

      InstanceResourceDeploymentDescription&
      operator= (InstanceResourceDeploymentDescription const& s);

      private:
      char regulator__;
    };

    class InstanceDeploymentDescription : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // node
      //
      public:
      ::XMLSchema::string< char > const& node () const;
      ::XMLSchema::string< char >& node ();
      void node (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > node_;

      // source
      //
      public:
      ::XMLSchema::string< char > const& source () const;
      ::XMLSchema::string< char >& source ();
      void source (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > source_;

      // implementation
      //
      public:
      ::XMLSchema::IDREF< char > const& implementation () const;
      ::XMLSchema::IDREF< char >& implementation ();
      void implementation (::XMLSchema::IDREF< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::IDREF< char > > implementation_;

      // configProperty
      //
      public:
      bool configProperty_p () const;
      ::CIAO::Config_Handlers::Property const& configProperty () const;
      ::CIAO::Config_Handlers::Property& configProperty ();
      void configProperty (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > configProperty_;

      // deployedResource
      //
      public:
      bool deployedResource_p () const;
      ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription const& deployedResource () const;
      ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription& deployedResource ();
      void deployedResource (::CIAO::Config_Handlers::InstanceResourceDeploymentDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription > deployedResource_;

      // deployedSharedResource
      //
      public:
      bool deployedSharedResource_p () const;
      ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription const& deployedSharedResource () const;
      ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription& deployedSharedResource ();
      void deployedSharedResource (::CIAO::Config_Handlers::InstanceResourceDeploymentDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription > deployedSharedResource_;

      // id
      //
      public:
      bool id_p () const;
      ::XMLSchema::ID< char > const& id () const;
      ::XMLSchema::ID< char >& id ();
      void id (::XMLSchema::ID< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::ID< char > > id_;

      public:
      InstanceDeploymentDescription ();
      InstanceDeploymentDescription (::XSCRT::XML::Element< char > const&);
      InstanceDeploymentDescription (InstanceDeploymentDescription const& s);

      InstanceDeploymentDescription&
      operator= (InstanceDeploymentDescription const& s);

      private:
      char regulator__;
    };

    class CCMComponentPortKind : public ::XSCRT::Type
    {
      public:
      CCMComponentPortKind ();
      CCMComponentPortKind (::XSCRT::XML::Element< char > const&);
      CCMComponentPortKind (::XSCRT::XML::Attribute< char > const&);

      static CCMComponentPortKind const Facet;
      static CCMComponentPortKind const SimplexReceptacle;
      static CCMComponentPortKind const MultiplexReceptacle;
      static CCMComponentPortKind const EventEmitter;
      static CCMComponentPortKind const EventPublisher;
      static CCMComponentPortKind const EventConsumer;

      enum Value
      {
        Facet_l,
        SimplexReceptacle_l,
        MultiplexReceptacle_l,
        EventEmitter_l,
        EventPublisher_l,
        EventConsumer_l,
      };


      Value
      integral () const;

      friend bool
      operator== (CCMComponentPortKind const& a, CCMComponentPortKind const& b);

      friend bool
      operator!= (CCMComponentPortKind const& a, CCMComponentPortKind const& b);

      private:
      CCMComponentPortKind (Value v);

      Value v_;
    };

    class ComponentPortDescription : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // specificType
      //
      public:
      ::XMLSchema::string< char > const& specificType () const;
      ::XMLSchema::string< char >& specificType ();
      void specificType (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > specificType_;

      // supportedType
      //
      public:
      ::XMLSchema::string< char > const& supportedType () const;
      ::XMLSchema::string< char >& supportedType ();
      void supportedType (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > supportedType_;

      // provider
      //
      public:
      ::XMLSchema::string< char > const& provider () const;
      ::XMLSchema::string< char >& provider ();
      void provider (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > provider_;

      // exclusiveProvider
      //
      public:
      ::XMLSchema::string< char > const& exclusiveProvider () const;
      ::XMLSchema::string< char >& exclusiveProvider ();
      void exclusiveProvider (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > exclusiveProvider_;

      // exclusiveUser
      //
      public:
      ::XMLSchema::string< char > const& exclusiveUser () const;
      ::XMLSchema::string< char >& exclusiveUser ();
      void exclusiveUser (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > exclusiveUser_;

      // optional
      //
      public:
      ::XMLSchema::string< char > const& optional () const;
      ::XMLSchema::string< char >& optional ();
      void optional (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > optional_;

      // kind
      //
      public:
      ::CIAO::Config_Handlers::CCMComponentPortKind const& kind () const;
      ::CIAO::Config_Handlers::CCMComponentPortKind& kind ();
      void kind (::CIAO::Config_Handlers::CCMComponentPortKind const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::CCMComponentPortKind > kind_;

      public:
      ComponentPortDescription ();
      ComponentPortDescription (::XSCRT::XML::Element< char > const&);
      ComponentPortDescription (ComponentPortDescription const& s);

      ComponentPortDescription&
      operator= (ComponentPortDescription const& s);

      private:
      char regulator__;
    };

    class ComponentPropertyDescription : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // type
      //
      public:
      ::CIAO::Config_Handlers::DataType const& type () const;
      ::CIAO::Config_Handlers::DataType& type ();
      void type (::CIAO::Config_Handlers::DataType const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::DataType > type_;

      public:
      ComponentPropertyDescription ();
      ComponentPropertyDescription (::XSCRT::XML::Element< char > const&);
      ComponentPropertyDescription (ComponentPropertyDescription const& s);

      ComponentPropertyDescription&
      operator= (ComponentPropertyDescription const& s);

      private:
      char regulator__;
    };

    class ComponentExternalPortEndpoint : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // portName
      //
      public:
      ::XMLSchema::string< char > const& portName () const;
      ::XMLSchema::string< char >& portName ();
      void portName (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > portName_;

      public:
      ComponentExternalPortEndpoint ();
      ComponentExternalPortEndpoint (::XSCRT::XML::Element< char > const&);
      ComponentExternalPortEndpoint (ComponentExternalPortEndpoint const& s);

      ComponentExternalPortEndpoint&
      operator= (ComponentExternalPortEndpoint const& s);

      private:
      char regulator__;
    };

    class PlanSubcomponentPortEndpoint : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // portName
      //
      public:
      ::XMLSchema::string< char > const& portName () const;
      ::XMLSchema::string< char >& portName ();
      void portName (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > portName_;

      // provider
      //
      public:
      bool provider_p () const;
      ::XMLSchema::string< char > const& provider () const;
      ::XMLSchema::string< char >& provider ();
      void provider (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > provider_;

      // kind
      //
      public:
      ::CIAO::Config_Handlers::CCMComponentPortKind const& kind () const;
      ::CIAO::Config_Handlers::CCMComponentPortKind& kind ();
      void kind (::CIAO::Config_Handlers::CCMComponentPortKind const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::CCMComponentPortKind > kind_;

      // instance
      //
      public:
      ::XMLSchema::IDREF< char > const& instance () const;
      ::XMLSchema::IDREF< char >& instance ();
      void instance (::XMLSchema::IDREF< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::IDREF< char > > instance_;

      public:
      PlanSubcomponentPortEndpoint ();
      PlanSubcomponentPortEndpoint (::XSCRT::XML::Element< char > const&);
      PlanSubcomponentPortEndpoint (PlanSubcomponentPortEndpoint const& s);

      PlanSubcomponentPortEndpoint&
      operator= (PlanSubcomponentPortEndpoint const& s);

      private:
      char regulator__;
    };

    class ExternalReferenceEndpoint : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // location
      //
      public:
      ::XMLSchema::string< char > const& location () const;
      ::XMLSchema::string< char >& location ();
      void location (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > location_;

      public:
      ExternalReferenceEndpoint ();
      ExternalReferenceEndpoint (::XSCRT::XML::Element< char > const&);
      ExternalReferenceEndpoint (ExternalReferenceEndpoint const& s);

      ExternalReferenceEndpoint&
      operator= (ExternalReferenceEndpoint const& s);

      private:
      char regulator__;
    };

    class ConnectionResourceDeploymentDescription : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // targetName
      //
      public:
      ::XMLSchema::string< char > const& targetName () const;
      ::XMLSchema::string< char >& targetName ();
      void targetName (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > targetName_;

      // requirementName
      //
      public:
      ::XMLSchema::string< char > const& requirementName () const;
      ::XMLSchema::string< char >& requirementName ();
      void requirementName (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > requirementName_;

      // resourceName
      //
      public:
      ::XMLSchema::string< char > const& resourceName () const;
      ::XMLSchema::string< char >& resourceName ();
      void resourceName (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > resourceName_;

      // resourceValue
      //
      public:
      ::CIAO::Config_Handlers::Any const& resourceValue () const;
      ::CIAO::Config_Handlers::Any& resourceValue ();
      void resourceValue (::CIAO::Config_Handlers::Any const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Any > resourceValue_;

      public:
      ConnectionResourceDeploymentDescription ();
      ConnectionResourceDeploymentDescription (::XSCRT::XML::Element< char > const&);
      ConnectionResourceDeploymentDescription (ConnectionResourceDeploymentDescription const& s);

      ConnectionResourceDeploymentDescription&
      operator= (ConnectionResourceDeploymentDescription const& s);

      private:
      char regulator__;
    };

    class PlanConnectionDescription : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // source
      //
      public:
      bool source_p () const;
      ::XMLSchema::string< char > const& source () const;
      ::XMLSchema::string< char >& source ();
      void source (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > source_;

      // deployRequirement
      //
      public:
      bool deployRequirement_p () const;
      ::CIAO::Config_Handlers::Requirement const& deployRequirement () const;
      ::CIAO::Config_Handlers::Requirement& deployRequirement ();
      void deployRequirement (::CIAO::Config_Handlers::Requirement const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > deployRequirement_;

      // externalEndpoint
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::ComponentExternalPortEndpoint >::iterator externalEndpoint_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::ComponentExternalPortEndpoint >::const_iterator externalEndpoint_const_iterator;
      externalEndpoint_iterator begin_externalEndpoint ();
      externalEndpoint_iterator end_externalEndpoint ();
      externalEndpoint_const_iterator begin_externalEndpoint () const;
      externalEndpoint_const_iterator end_externalEndpoint () const;
      void add_externalEndpoint (::CIAO::Config_Handlers::ComponentExternalPortEndpoint const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::ComponentExternalPortEndpoint > externalEndpoint_;

      // internalEndpoint
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::PlanSubcomponentPortEndpoint >::iterator internalEndpoint_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::PlanSubcomponentPortEndpoint >::const_iterator internalEndpoint_const_iterator;
      internalEndpoint_iterator begin_internalEndpoint ();
      internalEndpoint_iterator end_internalEndpoint ();
      internalEndpoint_const_iterator begin_internalEndpoint () const;
      internalEndpoint_const_iterator end_internalEndpoint () const;
      void add_internalEndpoint (::CIAO::Config_Handlers::PlanSubcomponentPortEndpoint const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::PlanSubcomponentPortEndpoint > internalEndpoint_;

      // externalReference
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::ExternalReferenceEndpoint >::iterator externalReference_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::ExternalReferenceEndpoint >::const_iterator externalReference_const_iterator;
      externalReference_iterator begin_externalReference ();
      externalReference_iterator end_externalReference ();
      externalReference_const_iterator begin_externalReference () const;
      externalReference_const_iterator end_externalReference () const;
      void add_externalReference (::CIAO::Config_Handlers::ExternalReferenceEndpoint const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::ExternalReferenceEndpoint > externalReference_;

      // deployedResource
      //
      public:
      bool deployedResource_p () const;
      ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription const& deployedResource () const;
      ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription& deployedResource ();
      void deployedResource (::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription > deployedResource_;

      public:
      PlanConnectionDescription ();
      PlanConnectionDescription (::XSCRT::XML::Element< char > const&);
      PlanConnectionDescription (PlanConnectionDescription const& s);

      PlanConnectionDescription&
      operator= (PlanConnectionDescription const& s);

      private:
      char regulator__;
    };

    class ImplementationDependency : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // requiredType
      //
      public:
      ::XMLSchema::string< char > const& requiredType () const;
      ::XMLSchema::string< char >& requiredType ();
      void requiredType (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > requiredType_;

      public:
      ImplementationDependency ();
      ImplementationDependency (::XSCRT::XML::Element< char > const&);
      ImplementationDependency (ImplementationDependency const& s);

      ImplementationDependency&
      operator= (ImplementationDependency const& s);

      private:
      char regulator__;
    };

    class Capability : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // resourceType
      //
      public:
      ::XMLSchema::string< char > const& resourceType () const;
      ::XMLSchema::string< char >& resourceType ();
      void resourceType (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > resourceType_;

      // property
      //
      public:
      ::CIAO::Config_Handlers::SatisfierProperty const& property () const;
      ::CIAO::Config_Handlers::SatisfierProperty& property ();
      void property (::CIAO::Config_Handlers::SatisfierProperty const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::SatisfierProperty > property_;

      public:
      Capability ();
      Capability (::XSCRT::XML::Element< char > const&);
      Capability (Capability const& s);

      Capability&
      operator= (Capability const& s);

      private:
      char regulator__;
    };

    class ImplementationRequirement : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // resourceUsage
      //
      public:
      ::CIAO::Config_Handlers::ResourceUsageKind const& resourceUsage () const;
      ::CIAO::Config_Handlers::ResourceUsageKind& resourceUsage ();
      void resourceUsage (::CIAO::Config_Handlers::ResourceUsageKind const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ResourceUsageKind > resourceUsage_;

      // resourcePort
      //
      public:
      ::XMLSchema::string< char > const& resourcePort () const;
      ::XMLSchema::string< char >& resourcePort ();
      void resourcePort (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > resourcePort_;

      // componentPort
      //
      public:
      ::XMLSchema::string< char > const& componentPort () const;
      ::XMLSchema::string< char >& componentPort ();
      void componentPort (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > componentPort_;

      // resourceType
      //
      public:
      ::XMLSchema::string< char > const& resourceType () const;
      ::XMLSchema::string< char >& resourceType ();
      void resourceType (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > resourceType_;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // property
      //
      public:
      ::CIAO::Config_Handlers::Property const& property () const;
      ::CIAO::Config_Handlers::Property& property ();
      void property (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > property_;

      public:
      ImplementationRequirement ();
      ImplementationRequirement (::XSCRT::XML::Element< char > const&);
      ImplementationRequirement (ImplementationRequirement const& s);

      ImplementationRequirement&
      operator= (ImplementationRequirement const& s);

      private:
      char regulator__;
    };

    class ComponentPackageReference : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // requiredUUID
      //
      public:
      ::XMLSchema::string< char > const& requiredUUID () const;
      ::XMLSchema::string< char >& requiredUUID ();
      void requiredUUID (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > requiredUUID_;

      // requiredName
      //
      public:
      ::XMLSchema::string< char > const& requiredName () const;
      ::XMLSchema::string< char >& requiredName ();
      void requiredName (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > requiredName_;

      // requiredType
      //
      public:
      ::XMLSchema::string< char > const& requiredType () const;
      ::XMLSchema::string< char >& requiredType ();
      void requiredType (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > requiredType_;

      public:
      ComponentPackageReference ();
      ComponentPackageReference (::XSCRT::XML::Element< char > const&);
      ComponentPackageReference (ComponentPackageReference const& s);

      ComponentPackageReference&
      operator= (ComponentPackageReference const& s);

      private:
      char regulator__;
    };

    class SubcomponentPortEndpoint : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // portName
      //
      public:
      ::XMLSchema::string< char > const& portName () const;
      ::XMLSchema::string< char >& portName ();
      void portName (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > portName_;

      // instance
      //
      public:
      ::XMLSchema::IDREF< char > const& instance () const;
      ::XMLSchema::IDREF< char >& instance ();
      void instance (::XMLSchema::IDREF< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::IDREF< char > > instance_;

      public:
      SubcomponentPortEndpoint ();
      SubcomponentPortEndpoint (::XSCRT::XML::Element< char > const&);
      SubcomponentPortEndpoint (SubcomponentPortEndpoint const& s);

      SubcomponentPortEndpoint&
      operator= (SubcomponentPortEndpoint const& s);

      private:
      char regulator__;
    };

    class AssemblyConnectionDescription : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // name
      //
      public:
      ::XMLSchema::string< char > const& name () const;
      ::XMLSchema::string< char >& name ();
      void name (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > name_;

      // deployRequirement
      //
      public:
      bool deployRequirement_p () const;
      ::CIAO::Config_Handlers::Requirement const& deployRequirement () const;
      ::CIAO::Config_Handlers::Requirement& deployRequirement ();
      void deployRequirement (::CIAO::Config_Handlers::Requirement const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > deployRequirement_;

      // externalEndpoint
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::ComponentExternalPortEndpoint >::iterator externalEndpoint_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::ComponentExternalPortEndpoint >::const_iterator externalEndpoint_const_iterator;
      externalEndpoint_iterator begin_externalEndpoint ();
      externalEndpoint_iterator end_externalEndpoint ();
      externalEndpoint_const_iterator begin_externalEndpoint () const;
      externalEndpoint_const_iterator end_externalEndpoint () const;
      void add_externalEndpoint (::CIAO::Config_Handlers::ComponentExternalPortEndpoint const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::ComponentExternalPortEndpoint > externalEndpoint_;

      // internalEndpoint
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::SubcomponentPortEndpoint >::iterator internalEndpoint_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::SubcomponentPortEndpoint >::const_iterator internalEndpoint_const_iterator;
      internalEndpoint_iterator begin_internalEndpoint ();
      internalEndpoint_iterator end_internalEndpoint ();
      internalEndpoint_const_iterator begin_internalEndpoint () const;
      internalEndpoint_const_iterator end_internalEndpoint () const;
      void add_internalEndpoint (::CIAO::Config_Handlers::SubcomponentPortEndpoint const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::SubcomponentPortEndpoint > internalEndpoint_;

      // externalReference
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::ExternalReferenceEndpoint >::iterator externalReference_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::ExternalReferenceEndpoint >::const_iterator externalReference_const_iterator;
      externalReference_iterator begin_externalReference ();
      externalReference_iterator end_externalReference ();
      externalReference_const_iterator begin_externalReference () const;
      externalReference_const_iterator end_externalReference () const;
      void add_externalReference (::CIAO::Config_Handlers::ExternalReferenceEndpoint const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::ExternalReferenceEndpoint > externalReference_;

      public:
      AssemblyConnectionDescription ();
      AssemblyConnectionDescription (::XSCRT::XML::Element< char > const&);
      AssemblyConnectionDescription (AssemblyConnectionDescription const& s);

      AssemblyConnectionDescription&
      operator= (AssemblyConnectionDescription const& s);

      private:
      char regulator__;
    };
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
  }
}

#endif // BASIC_DEPLOYMENT_DATA_HPP
