// $Id$
#include "Config_Handlers_Export.h"
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
#include <XMLSchema/Types.hpp>

namespace CIAO
{
  namespace Config_Handlers
  {
    class Config_Handlers_Export TCKind : public ::XSCRT::Type
    {
      public:
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

    class Config_Handlers_Export DataType : public ::XSCRT::Type
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

    class Config_Handlers_Export DataValue : public ::XSCRT::Type
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

    class Config_Handlers_Export Any : public ::XSCRT::Type
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
      Any (::CIAO::Config_Handlers::DataType const& type__,
      ::CIAO::Config_Handlers::DataValue const& value__);

      Any (::XSCRT::XML::Element< char > const&);
      Any (Any const& s);

      Any&
      operator= (Any const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export Property : public ::XSCRT::Type
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
      Property (::XMLSchema::string< char > const& name__,
      ::CIAO::Config_Handlers::Any const& value__);

      Property (::XSCRT::XML::Element< char > const&);
      Property (Property const& s);

      Property&
      operator= (Property const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export Node : public ::XSCRT::Type
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
      Node (::XMLSchema::string< char > const& name__,
      ::XMLSchema::string< char > const& label__);

      Node (::XSCRT::XML::Element< char > const&);
      Node (Node const& s);

      Node&
      operator= (Node const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export SatisfierPropertyKind : public ::XSCRT::Type
    {
      public:
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

    class Config_Handlers_Export SatisfierProperty : public ::XSCRT::Type
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
      SatisfierProperty (::XMLSchema::string< char > const& name__,
      ::CIAO::Config_Handlers::SatisfierPropertyKind const& kind__,
      ::CIAO::Config_Handlers::Any const& value__);

      SatisfierProperty (::XSCRT::XML::Element< char > const&);
      SatisfierProperty (SatisfierProperty const& s);

      SatisfierProperty&
      operator= (SatisfierProperty const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export Resource : public ::XSCRT::Type
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
      Resource (::XMLSchema::string< char > const& name__,
      ::XMLSchema::string< char > const& resourceType__,
      ::CIAO::Config_Handlers::SatisfierProperty const& property__);

      Resource (::XSCRT::XML::Element< char > const&);
      Resource (Resource const& s);

      Resource&
      operator= (Resource const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export SharedResource : public ::XSCRT::Type
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
      SharedResource (::XMLSchema::string< char > const& name__,
      ::XMLSchema::string< char > const& resourceType__,
      ::CIAO::Config_Handlers::Node const& node__,
      ::CIAO::Config_Handlers::SatisfierProperty const& property__);

      SharedResource (::XSCRT::XML::Element< char > const&);
      SharedResource (SharedResource const& s);

      SharedResource&
      operator= (SharedResource const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export Requirement : public ::XSCRT::Type
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
      Requirement (::XMLSchema::string< char > const& resourceType__,
      ::XMLSchema::string< char > const& name__,
      ::CIAO::Config_Handlers::Property const& property__);

      Requirement (::XSCRT::XML::Element< char > const&);
      Requirement (Requirement const& s);

      Requirement&
      operator= (Requirement const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export ResourceDeploymentDescription : public ::XSCRT::Type
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
      ResourceDeploymentDescription (::XMLSchema::string< char > const& requirementName__,
      ::XMLSchema::string< char > const& resourceName__,
      ::CIAO::Config_Handlers::Any const& resourceValue__);

      ResourceDeploymentDescription (::XSCRT::XML::Element< char > const&);
      ResourceDeploymentDescription (ResourceDeploymentDescription const& s);

      ResourceDeploymentDescription&
      operator= (ResourceDeploymentDescription const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export ArtifactDeploymentDescription : public ::XSCRT::Type
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
      typedef ::std::vector< ::XMLSchema::string< char > >::iterator source_iterator;
      typedef ::std::vector< ::XMLSchema::string< char > >::const_iterator source_const_iterator;
      source_iterator begin_source ();
      source_iterator end_source ();
      source_const_iterator begin_source () const;
      source_const_iterator end_source () const;
      void add_source (::XMLSchema::string< char > const& );

      protected:
      ::std::vector< ::XMLSchema::string< char > > source_;

      // node
      // 
      public:
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
      typedef ::std::vector< ::CIAO::Config_Handlers::Property >::iterator execParameter_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::Property >::const_iterator execParameter_const_iterator;
      execParameter_iterator begin_execParameter ();
      execParameter_iterator end_execParameter ();
      execParameter_const_iterator begin_execParameter () const;
      execParameter_const_iterator end_execParameter () const;
      void add_execParameter (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::Property > execParameter_;

      // deployRequirement
      // 
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::Requirement >::iterator deployRequirement_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::Requirement >::const_iterator deployRequirement_const_iterator;
      deployRequirement_iterator begin_deployRequirement ();
      deployRequirement_iterator end_deployRequirement ();
      deployRequirement_const_iterator begin_deployRequirement () const;
      deployRequirement_const_iterator end_deployRequirement () const;
      void add_deployRequirement (::CIAO::Config_Handlers::Requirement const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::Requirement > deployRequirement_;

      // deployedResource
      // 
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::ResourceDeploymentDescription >::iterator deployedResource_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::ResourceDeploymentDescription >::const_iterator deployedResource_const_iterator;
      deployedResource_iterator begin_deployedResource ();
      deployedResource_iterator end_deployedResource ();
      deployedResource_const_iterator begin_deployedResource () const;
      deployedResource_const_iterator end_deployedResource () const;
      void add_deployedResource (::CIAO::Config_Handlers::ResourceDeploymentDescription const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::ResourceDeploymentDescription > deployedResource_;

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
      ArtifactDeploymentDescription (::XMLSchema::string< char > const& name__,
      ::XMLSchema::string< char > const& node__);

      ArtifactDeploymentDescription (::XSCRT::XML::Element< char > const&);
      ArtifactDeploymentDescription (ArtifactDeploymentDescription const& s);

      ArtifactDeploymentDescription&
      operator= (ArtifactDeploymentDescription const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export MonolithicDeploymentDescription : public ::XSCRT::Type
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
      typedef ::std::vector< ::XMLSchema::string< char > >::iterator source_iterator;
      typedef ::std::vector< ::XMLSchema::string< char > >::const_iterator source_const_iterator;
      source_iterator begin_source ();
      source_iterator end_source ();
      source_const_iterator begin_source () const;
      source_const_iterator end_source () const;
      void add_source (::XMLSchema::string< char > const& );

      protected:
      ::std::vector< ::XMLSchema::string< char > > source_;

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
      typedef ::std::vector< ::CIAO::Config_Handlers::Property >::iterator execParameter_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::Property >::const_iterator execParameter_const_iterator;
      execParameter_iterator begin_execParameter ();
      execParameter_iterator end_execParameter ();
      execParameter_const_iterator begin_execParameter () const;
      execParameter_const_iterator end_execParameter () const;
      void add_execParameter (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::Property > execParameter_;

      // deployRequirement
      // 
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::Requirement >::iterator deployRequirement_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::Requirement >::const_iterator deployRequirement_const_iterator;
      deployRequirement_iterator begin_deployRequirement ();
      deployRequirement_iterator end_deployRequirement ();
      deployRequirement_const_iterator begin_deployRequirement () const;
      deployRequirement_const_iterator end_deployRequirement () const;
      void add_deployRequirement (::CIAO::Config_Handlers::Requirement const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::Requirement > deployRequirement_;

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
      MonolithicDeploymentDescription (::XMLSchema::string< char > const& name__);

      MonolithicDeploymentDescription (::XSCRT::XML::Element< char > const&);
      MonolithicDeploymentDescription (MonolithicDeploymentDescription const& s);

      MonolithicDeploymentDescription&
      operator= (MonolithicDeploymentDescription const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export ResourceUsageKind : public ::XSCRT::Type
    {
      public:
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

    class Config_Handlers_Export InstanceResourceDeploymentDescription : public ::XSCRT::Type
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
      InstanceResourceDeploymentDescription (::CIAO::Config_Handlers::ResourceUsageKind const& resourceUsage__,
      ::XMLSchema::string< char > const& requirementName__,
      ::XMLSchema::string< char > const& resourceName__,
      ::CIAO::Config_Handlers::Any const& resourceValue__);

      InstanceResourceDeploymentDescription (::XSCRT::XML::Element< char > const&);
      InstanceResourceDeploymentDescription (InstanceResourceDeploymentDescription const& s);

      InstanceResourceDeploymentDescription&
      operator= (InstanceResourceDeploymentDescription const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export InstanceDeploymentDescription : public ::XSCRT::Type
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
      typedef ::std::vector< ::CIAO::Config_Handlers::Property >::iterator configProperty_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::Property >::const_iterator configProperty_const_iterator;
      configProperty_iterator begin_configProperty ();
      configProperty_iterator end_configProperty ();
      configProperty_const_iterator begin_configProperty () const;
      configProperty_const_iterator end_configProperty () const;
      void add_configProperty (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::Property > configProperty_;

      // deployedResource
      // 
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription >::iterator deployedResource_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription >::const_iterator deployedResource_const_iterator;
      deployedResource_iterator begin_deployedResource ();
      deployedResource_iterator end_deployedResource ();
      deployedResource_const_iterator begin_deployedResource () const;
      deployedResource_const_iterator end_deployedResource () const;
      void add_deployedResource (::CIAO::Config_Handlers::InstanceResourceDeploymentDescription const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription > deployedResource_;

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
      InstanceDeploymentDescription (::XMLSchema::string< char > const& name__,
      ::XMLSchema::string< char > const& node__,
      ::XMLSchema::string< char > const& source__,
      ::XMLSchema::IDREF< char > const& implementation__);

      InstanceDeploymentDescription (::XSCRT::XML::Element< char > const&);
      InstanceDeploymentDescription (InstanceDeploymentDescription const& s);

      InstanceDeploymentDescription&
      operator= (InstanceDeploymentDescription const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export CCMComponentPortKind : public ::XSCRT::Type
    {
      public:
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

    class Config_Handlers_Export ComponentPortDescription : public ::XSCRT::Type
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
      ComponentPortDescription (::XMLSchema::string< char > const& name__,
      ::XMLSchema::string< char > const& specificType__,
      ::XMLSchema::string< char > const& supportedType__,
      ::XMLSchema::string< char > const& provider__,
      ::XMLSchema::string< char > const& exclusiveProvider__,
      ::XMLSchema::string< char > const& exclusiveUser__,
      ::XMLSchema::string< char > const& optional__,
      ::CIAO::Config_Handlers::CCMComponentPortKind const& kind__);

      ComponentPortDescription (::XSCRT::XML::Element< char > const&);
      ComponentPortDescription (ComponentPortDescription const& s);

      ComponentPortDescription&
      operator= (ComponentPortDescription const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export ComponentPropertyDescription : public ::XSCRT::Type
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
      ComponentPropertyDescription (::XMLSchema::string< char > const& name__,
      ::CIAO::Config_Handlers::DataType const& type__);

      ComponentPropertyDescription (::XSCRT::XML::Element< char > const&);
      ComponentPropertyDescription (ComponentPropertyDescription const& s);

      ComponentPropertyDescription&
      operator= (ComponentPropertyDescription const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export ComponentExternalPortEndpoint : public ::XSCRT::Type
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
      ComponentExternalPortEndpoint (::XMLSchema::string< char > const& portName__);

      ComponentExternalPortEndpoint (::XSCRT::XML::Element< char > const&);
      ComponentExternalPortEndpoint (ComponentExternalPortEndpoint const& s);

      ComponentExternalPortEndpoint&
      operator= (ComponentExternalPortEndpoint const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export PlanSubcomponentPortEndpoint : public ::XSCRT::Type
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
      PlanSubcomponentPortEndpoint (::XMLSchema::string< char > const& portName__,
      ::CIAO::Config_Handlers::CCMComponentPortKind const& kind__,
      ::XMLSchema::IDREF< char > const& instance__);

      PlanSubcomponentPortEndpoint (::XSCRT::XML::Element< char > const&);
      PlanSubcomponentPortEndpoint (PlanSubcomponentPortEndpoint const& s);

      PlanSubcomponentPortEndpoint&
      operator= (PlanSubcomponentPortEndpoint const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export ExternalReferenceEndpoint : public ::XSCRT::Type
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
      ExternalReferenceEndpoint (::XMLSchema::string< char > const& location__);

      ExternalReferenceEndpoint (::XSCRT::XML::Element< char > const&);
      ExternalReferenceEndpoint (ExternalReferenceEndpoint const& s);

      ExternalReferenceEndpoint&
      operator= (ExternalReferenceEndpoint const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export ConnectionResourceDeploymentDescription : public ::XSCRT::Type
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
      ConnectionResourceDeploymentDescription (::XMLSchema::string< char > const& targetName__,
      ::XMLSchema::string< char > const& requirementName__,
      ::XMLSchema::string< char > const& resourceName__,
      ::CIAO::Config_Handlers::Any const& resourceValue__);

      ConnectionResourceDeploymentDescription (::XSCRT::XML::Element< char > const&);
      ConnectionResourceDeploymentDescription (ConnectionResourceDeploymentDescription const& s);

      ConnectionResourceDeploymentDescription&
      operator= (ConnectionResourceDeploymentDescription const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export PlanConnectionDescription : public ::XSCRT::Type
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
      typedef ::std::vector< ::CIAO::Config_Handlers::Requirement >::iterator deployRequirement_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::Requirement >::const_iterator deployRequirement_const_iterator;
      deployRequirement_iterator begin_deployRequirement ();
      deployRequirement_iterator end_deployRequirement ();
      deployRequirement_const_iterator begin_deployRequirement () const;
      deployRequirement_const_iterator end_deployRequirement () const;
      void add_deployRequirement (::CIAO::Config_Handlers::Requirement const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::Requirement > deployRequirement_;

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
      typedef ::std::vector< ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription >::iterator deployedResource_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription >::const_iterator deployedResource_const_iterator;
      deployedResource_iterator begin_deployedResource ();
      deployedResource_iterator end_deployedResource ();
      deployedResource_const_iterator begin_deployedResource () const;
      deployedResource_const_iterator end_deployedResource () const;
      void add_deployedResource (::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription > deployedResource_;

      public:
      PlanConnectionDescription (::XMLSchema::string< char > const& name__);

      PlanConnectionDescription (::XSCRT::XML::Element< char > const&);
      PlanConnectionDescription (PlanConnectionDescription const& s);

      PlanConnectionDescription&
      operator= (PlanConnectionDescription const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export ImplementationDependency : public ::XSCRT::Type
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
      ImplementationDependency (::XMLSchema::string< char > const& requiredType__);

      ImplementationDependency (::XSCRT::XML::Element< char > const&);
      ImplementationDependency (ImplementationDependency const& s);

      ImplementationDependency&
      operator= (ImplementationDependency const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export Capability : public ::XSCRT::Type
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
      Capability (::XMLSchema::string< char > const& name__,
      ::XMLSchema::string< char > const& resourceType__,
      ::CIAO::Config_Handlers::SatisfierProperty const& property__);

      Capability (::XSCRT::XML::Element< char > const&);
      Capability (Capability const& s);

      Capability&
      operator= (Capability const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export ImplementationRequirement : public ::XSCRT::Type
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
      ImplementationRequirement (::CIAO::Config_Handlers::ResourceUsageKind const& resourceUsage__,
      ::XMLSchema::string< char > const& resourcePort__,
      ::XMLSchema::string< char > const& componentPort__,
      ::XMLSchema::string< char > const& resourceType__,
      ::XMLSchema::string< char > const& name__,
      ::CIAO::Config_Handlers::Property const& property__);

      ImplementationRequirement (::XSCRT::XML::Element< char > const&);
      ImplementationRequirement (ImplementationRequirement const& s);

      ImplementationRequirement&
      operator= (ImplementationRequirement const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export ComponentPackageReference : public ::XSCRT::Type
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
      ComponentPackageReference (::XMLSchema::string< char > const& requiredUUID__,
      ::XMLSchema::string< char > const& requiredName__,
      ::XMLSchema::string< char > const& requiredType__);

      ComponentPackageReference (::XSCRT::XML::Element< char > const&);
      ComponentPackageReference (ComponentPackageReference const& s);

      ComponentPackageReference&
      operator= (ComponentPackageReference const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export SubcomponentPortEndpoint : public ::XSCRT::Type
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
      SubcomponentPortEndpoint (::XMLSchema::string< char > const& portName__,
      ::XMLSchema::IDREF< char > const& instance__);

      SubcomponentPortEndpoint (::XSCRT::XML::Element< char > const&);
      SubcomponentPortEndpoint (SubcomponentPortEndpoint const& s);

      SubcomponentPortEndpoint&
      operator= (SubcomponentPortEndpoint const& s);

      private:
      char regulator__;
    };

    class Config_Handlers_Export AssemblyConnectionDescription : public ::XSCRT::Type
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
      AssemblyConnectionDescription (::XMLSchema::string< char > const& name__);

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

#include <XMLSchema/Traversal.hpp>

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Traversal
    {
      typedef
      ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::TCKind >
      TCKind;

      struct Config_Handlers_Export DataType : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::DataType >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        kind (Type&);

        virtual void
        kind (Type const&);

        virtual void
        kind_none (Type&);

        virtual void
        kind_none (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export DataValue : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::DataValue >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        short_ (Type&);

        virtual void
        short_ (Type const&);

        virtual void
        short_none (Type&);

        virtual void
        short_none (Type const&);

        virtual void
        long_ (Type&);

        virtual void
        long_ (Type const&);

        virtual void
        long_none (Type&);

        virtual void
        long_none (Type const&);

        virtual void
        ushort (Type&);

        virtual void
        ushort (Type const&);

        virtual void
        ushort_none (Type&);

        virtual void
        ushort_none (Type const&);

        virtual void
        ulong (Type&);

        virtual void
        ulong (Type const&);

        virtual void
        ulong_none (Type&);

        virtual void
        ulong_none (Type const&);

        virtual void
        float_ (Type&);

        virtual void
        float_ (Type const&);

        virtual void
        float_none (Type&);

        virtual void
        float_none (Type const&);

        virtual void
        double_ (Type&);

        virtual void
        double_ (Type const&);

        virtual void
        double_none (Type&);

        virtual void
        double_none (Type const&);

        virtual void
        boolean (Type&);

        virtual void
        boolean (Type const&);

        virtual void
        boolean_none (Type&);

        virtual void
        boolean_none (Type const&);

        virtual void
        octet (Type&);

        virtual void
        octet (Type const&);

        virtual void
        octet_none (Type&);

        virtual void
        octet_none (Type const&);

        virtual void
        objref (Type&);

        virtual void
        objref (Type const&);

        virtual void
        objref_none (Type&);

        virtual void
        objref_none (Type const&);

        virtual void
        enum_ (Type&);

        virtual void
        enum_ (Type const&);

        virtual void
        enum_none (Type&);

        virtual void
        enum_none (Type const&);

        virtual void
        string (Type&);

        virtual void
        string (Type const&);

        virtual void
        string_none (Type&);

        virtual void
        string_none (Type const&);

        virtual void
        longlong (Type&);

        virtual void
        longlong (Type const&);

        virtual void
        longlong_none (Type&);

        virtual void
        longlong_none (Type const&);

        virtual void
        ulonglong (Type&);

        virtual void
        ulonglong (Type const&);

        virtual void
        ulonglong_none (Type&);

        virtual void
        ulonglong_none (Type const&);

        virtual void
        longdouble (Type&);

        virtual void
        longdouble (Type const&);

        virtual void
        longdouble_none (Type&);

        virtual void
        longdouble_none (Type const&);

        virtual void
        fixed (Type&);

        virtual void
        fixed (Type const&);

        virtual void
        fixed_none (Type&);

        virtual void
        fixed_none (Type const&);

        virtual void
        typecode (Type&);

        virtual void
        typecode (Type const&);

        virtual void
        typecode_none (Type&);

        virtual void
        typecode_none (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export Any : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::Any >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        type (Type&);

        virtual void
        type (Type const&);

        virtual void
        value (Type&);

        virtual void
        value (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export Property : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::Property >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        value (Type&);

        virtual void
        value (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export Node : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::Node >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        label (Type&);

        virtual void
        label (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      typedef
      ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::SatisfierPropertyKind >
      SatisfierPropertyKind;

      struct Config_Handlers_Export SatisfierProperty : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::SatisfierProperty >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        kind (Type&);

        virtual void
        kind (Type const&);

        virtual void
        value (Type&);

        virtual void
        value (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export Resource : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::Resource >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        resourceType (Type&);

        virtual void
        resourceType (Type const&);

        virtual void
        property (Type&);

        virtual void
        property (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export SharedResource : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::SharedResource >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        resourceType (Type&);

        virtual void
        resourceType (Type const&);

        virtual void
        node (Type&);

        virtual void
        node (Type const&);

        virtual void
        property (Type&);

        virtual void
        property (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export Requirement : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::Requirement >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        resourceType (Type&);

        virtual void
        resourceType (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        property (Type&);

        virtual void
        property (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export ResourceDeploymentDescription : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::ResourceDeploymentDescription >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        requirementName (Type&);

        virtual void
        requirementName (Type const&);

        virtual void
        resourceName (Type&);

        virtual void
        resourceName (Type const&);

        virtual void
        resourceValue (Type&);

        virtual void
        resourceValue (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export ArtifactDeploymentDescription : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::ArtifactDeploymentDescription >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        source (Type&);

        virtual void
        source (Type const&);

        virtual void
        source_pre (Type&);

        virtual void
        source_pre (Type const&);

        virtual void
        source_next (Type&);

        virtual void
        source_next (Type const&);

        virtual void
        source_post (Type&);

        virtual void
        source_post (Type const&);

        virtual void
        node (Type&);

        virtual void
        node (Type const&);

        virtual void
        location (Type&);

        virtual void
        location (Type const&);

        virtual void
        location_pre (Type&);

        virtual void
        location_pre (Type const&);

        virtual void
        location_next (Type&);

        virtual void
        location_next (Type const&);

        virtual void
        location_post (Type&);

        virtual void
        location_post (Type const&);

        virtual void
        execParameter (Type&);

        virtual void
        execParameter (Type const&);

        virtual void
        execParameter_pre (Type&);

        virtual void
        execParameter_pre (Type const&);

        virtual void
        execParameter_next (Type&);

        virtual void
        execParameter_next (Type const&);

        virtual void
        execParameter_post (Type&);

        virtual void
        execParameter_post (Type const&);

        virtual void
        execParameter_none (Type&);

        virtual void
        execParameter_none (Type const&);

        virtual void
        deployRequirement (Type&);

        virtual void
        deployRequirement (Type const&);

        virtual void
        deployRequirement_pre (Type&);

        virtual void
        deployRequirement_pre (Type const&);

        virtual void
        deployRequirement_next (Type&);

        virtual void
        deployRequirement_next (Type const&);

        virtual void
        deployRequirement_post (Type&);

        virtual void
        deployRequirement_post (Type const&);

        virtual void
        deployRequirement_none (Type&);

        virtual void
        deployRequirement_none (Type const&);

        virtual void
        deployedResource (Type&);

        virtual void
        deployedResource (Type const&);

        virtual void
        deployedResource_pre (Type&);

        virtual void
        deployedResource_pre (Type const&);

        virtual void
        deployedResource_next (Type&);

        virtual void
        deployedResource_next (Type const&);

        virtual void
        deployedResource_post (Type&);

        virtual void
        deployedResource_post (Type const&);

        virtual void
        deployedResource_none (Type&);

        virtual void
        deployedResource_none (Type const&);

        virtual void
        id (Type&);

        virtual void
        id (Type const&);

        virtual void
        id_none (Type&);

        virtual void
        id_none (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export MonolithicDeploymentDescription : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::MonolithicDeploymentDescription >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        source (Type&);

        virtual void
        source (Type const&);

        virtual void
        source_pre (Type&);

        virtual void
        source_pre (Type const&);

        virtual void
        source_next (Type&);

        virtual void
        source_next (Type const&);

        virtual void
        source_post (Type&);

        virtual void
        source_post (Type const&);

        virtual void
        artifact (Type&);

        virtual void
        artifact (Type const&);

        virtual void
        artifact_pre (Type&);

        virtual void
        artifact_pre (Type const&);

        virtual void
        artifact_next (Type&);

        virtual void
        artifact_next (Type const&);

        virtual void
        artifact_post (Type&);

        virtual void
        artifact_post (Type const&);

        virtual void
        execParameter (Type&);

        virtual void
        execParameter (Type const&);

        virtual void
        execParameter_pre (Type&);

        virtual void
        execParameter_pre (Type const&);

        virtual void
        execParameter_next (Type&);

        virtual void
        execParameter_next (Type const&);

        virtual void
        execParameter_post (Type&);

        virtual void
        execParameter_post (Type const&);

        virtual void
        deployRequirement (Type&);

        virtual void
        deployRequirement (Type const&);

        virtual void
        deployRequirement_pre (Type&);

        virtual void
        deployRequirement_pre (Type const&);

        virtual void
        deployRequirement_next (Type&);

        virtual void
        deployRequirement_next (Type const&);

        virtual void
        deployRequirement_post (Type&);

        virtual void
        deployRequirement_post (Type const&);

        virtual void
        id (Type&);

        virtual void
        id (Type const&);

        virtual void
        id_none (Type&);

        virtual void
        id_none (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      typedef
      ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::ResourceUsageKind >
      ResourceUsageKind;

      struct Config_Handlers_Export InstanceResourceDeploymentDescription : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        resourceUsage (Type&);

        virtual void
        resourceUsage (Type const&);

        virtual void
        requirementName (Type&);

        virtual void
        requirementName (Type const&);

        virtual void
        resourceName (Type&);

        virtual void
        resourceName (Type const&);

        virtual void
        resourceValue (Type&);

        virtual void
        resourceValue (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export InstanceDeploymentDescription : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::InstanceDeploymentDescription >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        node (Type&);

        virtual void
        node (Type const&);

        virtual void
        source (Type&);

        virtual void
        source (Type const&);

        virtual void
        implementation (Type&);

        virtual void
        implementation (Type const&);

        virtual void
        configProperty (Type&);

        virtual void
        configProperty (Type const&);

        virtual void
        configProperty_pre (Type&);

        virtual void
        configProperty_pre (Type const&);

        virtual void
        configProperty_next (Type&);

        virtual void
        configProperty_next (Type const&);

        virtual void
        configProperty_post (Type&);

        virtual void
        configProperty_post (Type const&);

        virtual void
        configProperty_none (Type&);

        virtual void
        configProperty_none (Type const&);

        virtual void
        deployedResource (Type&);

        virtual void
        deployedResource (Type const&);

        virtual void
        deployedResource_pre (Type&);

        virtual void
        deployedResource_pre (Type const&);

        virtual void
        deployedResource_next (Type&);

        virtual void
        deployedResource_next (Type const&);

        virtual void
        deployedResource_post (Type&);

        virtual void
        deployedResource_post (Type const&);

        virtual void
        deployedResource_none (Type&);

        virtual void
        deployedResource_none (Type const&);

        virtual void
        deployedSharedResource (Type&);

        virtual void
        deployedSharedResource (Type const&);

        virtual void
        deployedSharedResource_none (Type&);

        virtual void
        deployedSharedResource_none (Type const&);

        virtual void
        id (Type&);

        virtual void
        id (Type const&);

        virtual void
        id_none (Type&);

        virtual void
        id_none (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      typedef
      ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::CCMComponentPortKind >
      CCMComponentPortKind;

      struct Config_Handlers_Export ComponentPortDescription : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::ComponentPortDescription >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        specificType (Type&);

        virtual void
        specificType (Type const&);

        virtual void
        supportedType (Type&);

        virtual void
        supportedType (Type const&);

        virtual void
        provider (Type&);

        virtual void
        provider (Type const&);

        virtual void
        exclusiveProvider (Type&);

        virtual void
        exclusiveProvider (Type const&);

        virtual void
        exclusiveUser (Type&);

        virtual void
        exclusiveUser (Type const&);

        virtual void
        optional (Type&);

        virtual void
        optional (Type const&);

        virtual void
        kind (Type&);

        virtual void
        kind (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export ComponentPropertyDescription : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::ComponentPropertyDescription >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        type (Type&);

        virtual void
        type (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export ComponentExternalPortEndpoint : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::ComponentExternalPortEndpoint >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        portName (Type&);

        virtual void
        portName (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export PlanSubcomponentPortEndpoint : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::PlanSubcomponentPortEndpoint >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        portName (Type&);

        virtual void
        portName (Type const&);

        virtual void
        provider (Type&);

        virtual void
        provider (Type const&);

        virtual void
        provider_none (Type&);

        virtual void
        provider_none (Type const&);

        virtual void
        kind (Type&);

        virtual void
        kind (Type const&);

        virtual void
        instance (Type&);

        virtual void
        instance (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export ExternalReferenceEndpoint : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::ExternalReferenceEndpoint >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        location (Type&);

        virtual void
        location (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export ConnectionResourceDeploymentDescription : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        targetName (Type&);

        virtual void
        targetName (Type const&);

        virtual void
        requirementName (Type&);

        virtual void
        requirementName (Type const&);

        virtual void
        resourceName (Type&);

        virtual void
        resourceName (Type const&);

        virtual void
        resourceValue (Type&);

        virtual void
        resourceValue (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export PlanConnectionDescription : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::PlanConnectionDescription >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        source (Type&);

        virtual void
        source (Type const&);

        virtual void
        source_none (Type&);

        virtual void
        source_none (Type const&);

        virtual void
        deployRequirement (Type&);

        virtual void
        deployRequirement (Type const&);

        virtual void
        deployRequirement_pre (Type&);

        virtual void
        deployRequirement_pre (Type const&);

        virtual void
        deployRequirement_next (Type&);

        virtual void
        deployRequirement_next (Type const&);

        virtual void
        deployRequirement_post (Type&);

        virtual void
        deployRequirement_post (Type const&);

        virtual void
        deployRequirement_none (Type&);

        virtual void
        deployRequirement_none (Type const&);

        virtual void
        externalEndpoint (Type&);

        virtual void
        externalEndpoint (Type const&);

        virtual void
        externalEndpoint_pre (Type&);

        virtual void
        externalEndpoint_pre (Type const&);

        virtual void
        externalEndpoint_next (Type&);

        virtual void
        externalEndpoint_next (Type const&);

        virtual void
        externalEndpoint_post (Type&);

        virtual void
        externalEndpoint_post (Type const&);

        virtual void
        externalEndpoint_none (Type&);

        virtual void
        externalEndpoint_none (Type const&);

        virtual void
        internalEndpoint (Type&);

        virtual void
        internalEndpoint (Type const&);

        virtual void
        internalEndpoint_pre (Type&);

        virtual void
        internalEndpoint_pre (Type const&);

        virtual void
        internalEndpoint_next (Type&);

        virtual void
        internalEndpoint_next (Type const&);

        virtual void
        internalEndpoint_post (Type&);

        virtual void
        internalEndpoint_post (Type const&);

        virtual void
        internalEndpoint_none (Type&);

        virtual void
        internalEndpoint_none (Type const&);

        virtual void
        externalReference (Type&);

        virtual void
        externalReference (Type const&);

        virtual void
        externalReference_pre (Type&);

        virtual void
        externalReference_pre (Type const&);

        virtual void
        externalReference_next (Type&);

        virtual void
        externalReference_next (Type const&);

        virtual void
        externalReference_post (Type&);

        virtual void
        externalReference_post (Type const&);

        virtual void
        externalReference_none (Type&);

        virtual void
        externalReference_none (Type const&);

        virtual void
        deployedResource (Type&);

        virtual void
        deployedResource (Type const&);

        virtual void
        deployedResource_pre (Type&);

        virtual void
        deployedResource_pre (Type const&);

        virtual void
        deployedResource_next (Type&);

        virtual void
        deployedResource_next (Type const&);

        virtual void
        deployedResource_post (Type&);

        virtual void
        deployedResource_post (Type const&);

        virtual void
        deployedResource_none (Type&);

        virtual void
        deployedResource_none (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export ImplementationDependency : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::ImplementationDependency >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        requiredType (Type&);

        virtual void
        requiredType (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export Capability : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::Capability >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        resourceType (Type&);

        virtual void
        resourceType (Type const&);

        virtual void
        property (Type&);

        virtual void
        property (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export ImplementationRequirement : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::ImplementationRequirement >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        resourceUsage (Type&);

        virtual void
        resourceUsage (Type const&);

        virtual void
        resourcePort (Type&);

        virtual void
        resourcePort (Type const&);

        virtual void
        componentPort (Type&);

        virtual void
        componentPort (Type const&);

        virtual void
        resourceType (Type&);

        virtual void
        resourceType (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        property (Type&);

        virtual void
        property (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export ComponentPackageReference : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::ComponentPackageReference >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        requiredUUID (Type&);

        virtual void
        requiredUUID (Type const&);

        virtual void
        requiredName (Type&);

        virtual void
        requiredName (Type const&);

        virtual void
        requiredType (Type&);

        virtual void
        requiredType (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export SubcomponentPortEndpoint : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::SubcomponentPortEndpoint >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        portName (Type&);

        virtual void
        portName (Type const&);

        virtual void
        instance (Type&);

        virtual void
        instance (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };

      struct Config_Handlers_Export AssemblyConnectionDescription : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::AssemblyConnectionDescription >
      {
        virtual void
        traverse (Type&);

        virtual void
        traverse (Type const&);

        virtual void
        pre (Type&);

        virtual void
        pre (Type const&);

        virtual void
        name (Type&);

        virtual void
        name (Type const&);

        virtual void
        deployRequirement (Type&);

        virtual void
        deployRequirement (Type const&);

        virtual void
        deployRequirement_none (Type&);

        virtual void
        deployRequirement_none (Type const&);

        virtual void
        externalEndpoint (Type&);

        virtual void
        externalEndpoint (Type const&);

        virtual void
        externalEndpoint_pre (Type&);

        virtual void
        externalEndpoint_pre (Type const&);

        virtual void
        externalEndpoint_next (Type&);

        virtual void
        externalEndpoint_next (Type const&);

        virtual void
        externalEndpoint_post (Type&);

        virtual void
        externalEndpoint_post (Type const&);

        virtual void
        externalEndpoint_none (Type&);

        virtual void
        externalEndpoint_none (Type const&);

        virtual void
        internalEndpoint (Type&);

        virtual void
        internalEndpoint (Type const&);

        virtual void
        internalEndpoint_pre (Type&);

        virtual void
        internalEndpoint_pre (Type const&);

        virtual void
        internalEndpoint_next (Type&);

        virtual void
        internalEndpoint_next (Type const&);

        virtual void
        internalEndpoint_post (Type&);

        virtual void
        internalEndpoint_post (Type const&);

        virtual void
        internalEndpoint_none (Type&);

        virtual void
        internalEndpoint_none (Type const&);

        virtual void
        externalReference (Type&);

        virtual void
        externalReference (Type const&);

        virtual void
        externalReference_pre (Type&);

        virtual void
        externalReference_pre (Type const&);

        virtual void
        externalReference_next (Type&);

        virtual void
        externalReference_next (Type const&);

        virtual void
        externalReference_post (Type&);

        virtual void
        externalReference_post (Type const&);

        virtual void
        externalReference_none (Type&);

        virtual void
        externalReference_none (Type const&);

        virtual void
        post (Type&);

        virtual void
        post (Type const&);
      };
    }
  }
}

#include <XMLSchema/Writer.hpp>

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Writer
    {
      struct Config_Handlers_Export TCKind : Traversal::TCKind, 
      virtual ::XSCRT::Writer< char >
      {
        TCKind (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        protected:
        TCKind ();
      };

      struct Config_Handlers_Export DataType : Traversal::DataType, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::DataType Type;
        DataType (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        kind (Type const&);

        protected:
        DataType ();
      };

      struct Config_Handlers_Export DataValue : Traversal::DataValue, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::DataValue Type;
        DataValue (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        short_ (Type const&);

        virtual void
        long_ (Type const&);

        virtual void
        ushort (Type const&);

        virtual void
        ulong (Type const&);

        virtual void
        float_ (Type const&);

        virtual void
        double_ (Type const&);

        virtual void
        boolean (Type const&);

        virtual void
        octet (Type const&);

        virtual void
        objref (Type const&);

        virtual void
        enum_ (Type const&);

        virtual void
        string (Type const&);

        virtual void
        longlong (Type const&);

        virtual void
        ulonglong (Type const&);

        virtual void
        longdouble (Type const&);

        virtual void
        fixed (Type const&);

        virtual void
        typecode (Type const&);

        protected:
        DataValue ();
      };

      struct Config_Handlers_Export Any : Traversal::Any, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::Any Type;
        Any (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        type (Type const&);

        virtual void
        value (Type const&);

        protected:
        Any ();
      };

      struct Config_Handlers_Export Property : Traversal::Property, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::Property Type;
        Property (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        name (Type const&);

        virtual void
        value (Type const&);

        protected:
        Property ();
      };

      struct Config_Handlers_Export Node : Traversal::Node, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::Node Type;
        Node (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        name (Type const&);

        virtual void
        label (Type const&);

        protected:
        Node ();
      };

      struct Config_Handlers_Export SatisfierPropertyKind : Traversal::SatisfierPropertyKind, 
      virtual ::XSCRT::Writer< char >
      {
        SatisfierPropertyKind (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        protected:
        SatisfierPropertyKind ();
      };

      struct Config_Handlers_Export SatisfierProperty : Traversal::SatisfierProperty, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::SatisfierProperty Type;
        SatisfierProperty (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        name (Type const&);

        virtual void
        kind (Type const&);

        virtual void
        value (Type const&);

        protected:
        SatisfierProperty ();
      };

      struct Config_Handlers_Export Resource : Traversal::Resource, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::Resource Type;
        Resource (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        name (Type const&);

        virtual void
        resourceType (Type const&);

        virtual void
        property (Type const&);

        protected:
        Resource ();
      };

      struct Config_Handlers_Export SharedResource : Traversal::SharedResource, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::SharedResource Type;
        SharedResource (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        name (Type const&);

        virtual void
        resourceType (Type const&);

        virtual void
        node (Type const&);

        virtual void
        property (Type const&);

        protected:
        SharedResource ();
      };

      struct Config_Handlers_Export Requirement : Traversal::Requirement, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::Requirement Type;
        Requirement (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        resourceType (Type const&);

        virtual void
        name (Type const&);

        virtual void
        property (Type const&);

        protected:
        Requirement ();
      };

      struct Config_Handlers_Export ResourceDeploymentDescription : Traversal::ResourceDeploymentDescription, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::ResourceDeploymentDescription Type;
        ResourceDeploymentDescription (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        requirementName (Type const&);

        virtual void
        resourceName (Type const&);

        virtual void
        resourceValue (Type const&);

        protected:
        ResourceDeploymentDescription ();
      };

      struct Config_Handlers_Export ArtifactDeploymentDescription : Traversal::ArtifactDeploymentDescription, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::ArtifactDeploymentDescription Type;
        ArtifactDeploymentDescription (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        name (Type const&);

        virtual void
        source_pre (Type const&);

        virtual void
        source_next (Type const&);

        virtual void
        source_post (Type const&);

        virtual void
        node (Type const&);

        virtual void
        location_pre (Type const&);

        virtual void
        location_next (Type const&);

        virtual void
        location_post (Type const&);

        virtual void
        execParameter_pre (Type const&);

        virtual void
        execParameter_next (Type const&);

        virtual void
        execParameter_post (Type const&);

        virtual void
        deployRequirement_pre (Type const&);

        virtual void
        deployRequirement_next (Type const&);

        virtual void
        deployRequirement_post (Type const&);

        virtual void
        deployedResource_pre (Type const&);

        virtual void
        deployedResource_next (Type const&);

        virtual void
        deployedResource_post (Type const&);

        virtual void
        id (Type const&);

        protected:
        ArtifactDeploymentDescription ();
      };

      struct Config_Handlers_Export MonolithicDeploymentDescription : Traversal::MonolithicDeploymentDescription, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::MonolithicDeploymentDescription Type;
        MonolithicDeploymentDescription (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        name (Type const&);

        virtual void
        source_pre (Type const&);

        virtual void
        source_next (Type const&);

        virtual void
        source_post (Type const&);

        virtual void
        artifact_pre (Type const&);

        virtual void
        artifact_next (Type const&);

        virtual void
        artifact_post (Type const&);

        virtual void
        execParameter_pre (Type const&);

        virtual void
        execParameter_next (Type const&);

        virtual void
        execParameter_post (Type const&);

        virtual void
        deployRequirement_pre (Type const&);

        virtual void
        deployRequirement_next (Type const&);

        virtual void
        deployRequirement_post (Type const&);

        virtual void
        id (Type const&);

        protected:
        MonolithicDeploymentDescription ();
      };

      struct Config_Handlers_Export ResourceUsageKind : Traversal::ResourceUsageKind, 
      virtual ::XSCRT::Writer< char >
      {
        ResourceUsageKind (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        protected:
        ResourceUsageKind ();
      };

      struct Config_Handlers_Export InstanceResourceDeploymentDescription : Traversal::InstanceResourceDeploymentDescription, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::InstanceResourceDeploymentDescription Type;
        InstanceResourceDeploymentDescription (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        resourceUsage (Type const&);

        virtual void
        requirementName (Type const&);

        virtual void
        resourceName (Type const&);

        virtual void
        resourceValue (Type const&);

        protected:
        InstanceResourceDeploymentDescription ();
      };

      struct Config_Handlers_Export InstanceDeploymentDescription : Traversal::InstanceDeploymentDescription, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::InstanceDeploymentDescription Type;
        InstanceDeploymentDescription (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        name (Type const&);

        virtual void
        node (Type const&);

        virtual void
        source (Type const&);

        virtual void
        implementation (Type const&);

        virtual void
        configProperty_pre (Type const&);

        virtual void
        configProperty_next (Type const&);

        virtual void
        configProperty_post (Type const&);

        virtual void
        deployedResource_pre (Type const&);

        virtual void
        deployedResource_next (Type const&);

        virtual void
        deployedResource_post (Type const&);

        virtual void
        deployedSharedResource (Type const&);

        virtual void
        id (Type const&);

        protected:
        InstanceDeploymentDescription ();
      };

      struct Config_Handlers_Export CCMComponentPortKind : Traversal::CCMComponentPortKind, 
      virtual ::XSCRT::Writer< char >
      {
        CCMComponentPortKind (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        protected:
        CCMComponentPortKind ();
      };

      struct Config_Handlers_Export ComponentPortDescription : Traversal::ComponentPortDescription, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::ComponentPortDescription Type;
        ComponentPortDescription (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        name (Type const&);

        virtual void
        specificType (Type const&);

        virtual void
        supportedType (Type const&);

        virtual void
        provider (Type const&);

        virtual void
        exclusiveProvider (Type const&);

        virtual void
        exclusiveUser (Type const&);

        virtual void
        optional (Type const&);

        virtual void
        kind (Type const&);

        protected:
        ComponentPortDescription ();
      };

      struct Config_Handlers_Export ComponentPropertyDescription : Traversal::ComponentPropertyDescription, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::ComponentPropertyDescription Type;
        ComponentPropertyDescription (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        name (Type const&);

        virtual void
        type (Type const&);

        protected:
        ComponentPropertyDescription ();
      };

      struct Config_Handlers_Export ComponentExternalPortEndpoint : Traversal::ComponentExternalPortEndpoint, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::ComponentExternalPortEndpoint Type;
        ComponentExternalPortEndpoint (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        portName (Type const&);

        protected:
        ComponentExternalPortEndpoint ();
      };

      struct Config_Handlers_Export PlanSubcomponentPortEndpoint : Traversal::PlanSubcomponentPortEndpoint, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::PlanSubcomponentPortEndpoint Type;
        PlanSubcomponentPortEndpoint (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        portName (Type const&);

        virtual void
        provider (Type const&);

        virtual void
        kind (Type const&);

        virtual void
        instance (Type const&);

        protected:
        PlanSubcomponentPortEndpoint ();
      };

      struct Config_Handlers_Export ExternalReferenceEndpoint : Traversal::ExternalReferenceEndpoint, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::ExternalReferenceEndpoint Type;
        ExternalReferenceEndpoint (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        location (Type const&);

        protected:
        ExternalReferenceEndpoint ();
      };

      struct Config_Handlers_Export ConnectionResourceDeploymentDescription : Traversal::ConnectionResourceDeploymentDescription, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::ConnectionResourceDeploymentDescription Type;
        ConnectionResourceDeploymentDescription (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        targetName (Type const&);

        virtual void
        requirementName (Type const&);

        virtual void
        resourceName (Type const&);

        virtual void
        resourceValue (Type const&);

        protected:
        ConnectionResourceDeploymentDescription ();
      };

      struct Config_Handlers_Export PlanConnectionDescription : Traversal::PlanConnectionDescription, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::PlanConnectionDescription Type;
        PlanConnectionDescription (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        name (Type const&);

        virtual void
        source (Type const&);

        virtual void
        deployRequirement_pre (Type const&);

        virtual void
        deployRequirement_next (Type const&);

        virtual void
        deployRequirement_post (Type const&);

        virtual void
        externalEndpoint_pre (Type const&);

        virtual void
        externalEndpoint_next (Type const&);

        virtual void
        externalEndpoint_post (Type const&);

        virtual void
        internalEndpoint_pre (Type const&);

        virtual void
        internalEndpoint_next (Type const&);

        virtual void
        internalEndpoint_post (Type const&);

        virtual void
        externalReference_pre (Type const&);

        virtual void
        externalReference_next (Type const&);

        virtual void
        externalReference_post (Type const&);

        virtual void
        deployedResource_pre (Type const&);

        virtual void
        deployedResource_next (Type const&);

        virtual void
        deployedResource_post (Type const&);

        protected:
        PlanConnectionDescription ();
      };

      struct Config_Handlers_Export ImplementationDependency : Traversal::ImplementationDependency, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::ImplementationDependency Type;
        ImplementationDependency (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        requiredType (Type const&);

        protected:
        ImplementationDependency ();
      };

      struct Config_Handlers_Export Capability : Traversal::Capability, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::Capability Type;
        Capability (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        name (Type const&);

        virtual void
        resourceType (Type const&);

        virtual void
        property (Type const&);

        protected:
        Capability ();
      };

      struct Config_Handlers_Export ImplementationRequirement : Traversal::ImplementationRequirement, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::ImplementationRequirement Type;
        ImplementationRequirement (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        resourceUsage (Type const&);

        virtual void
        resourcePort (Type const&);

        virtual void
        componentPort (Type const&);

        virtual void
        resourceType (Type const&);

        virtual void
        name (Type const&);

        virtual void
        property (Type const&);

        protected:
        ImplementationRequirement ();
      };

      struct Config_Handlers_Export ComponentPackageReference : Traversal::ComponentPackageReference, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::ComponentPackageReference Type;
        ComponentPackageReference (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        requiredUUID (Type const&);

        virtual void
        requiredName (Type const&);

        virtual void
        requiredType (Type const&);

        protected:
        ComponentPackageReference ();
      };

      struct Config_Handlers_Export SubcomponentPortEndpoint : Traversal::SubcomponentPortEndpoint, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::SubcomponentPortEndpoint Type;
        SubcomponentPortEndpoint (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        portName (Type const&);

        virtual void
        instance (Type const&);

        protected:
        SubcomponentPortEndpoint ();
      };

      struct Config_Handlers_Export AssemblyConnectionDescription : Traversal::AssemblyConnectionDescription, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::AssemblyConnectionDescription Type;
        AssemblyConnectionDescription (::XSCRT::XML::Element< char >&);

        virtual void
        traverse (Type const&);

        virtual void
        name (Type const&);

        virtual void
        deployRequirement (Type const&);

        virtual void
        externalEndpoint_pre (Type const&);

        virtual void
        externalEndpoint_next (Type const&);

        virtual void
        externalEndpoint_post (Type const&);

        virtual void
        internalEndpoint_pre (Type const&);

        virtual void
        internalEndpoint_next (Type const&);

        virtual void
        internalEndpoint_post (Type const&);

        virtual void
        externalReference_pre (Type const&);

        virtual void
        externalReference_next (Type const&);

        virtual void
        externalReference_post (Type const&);

        protected:
        AssemblyConnectionDescription ();
      };
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
  }
}

#endif // BASIC_DEPLOYMENT_DATA_HPP
