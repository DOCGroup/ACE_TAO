// $Id$

#ifndef PACKAGE_VISITOR_H
#define PACKAGE_VISITOR_H

#include <memory>
#include <stack>
#include <map>
#include "PICML/PICML.h"

// Xerces includes
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

// Utility includes
#include "Utils/XercesString.h"
#include "Utils/Utils.h"
#include "Package/Package_Export.h"

namespace PICML
{
  using xercesc::DOMImplementation;
  using xercesc::DOMDocument;
  using xercesc::DOMElement;
  using xercesc::DOMWriter;
  using xercesc::XMLFormatTarget;
  using xercesc::LocalFileFormatTarget;

  using std::set;
  using std::vector;
  using std::string;
  using std::map;

  class Package_Export PackageVisitor: public Visitor
  {
    public:
    PackageVisitor (const std::string& outputPath);
    ~PackageVisitor();

    // Lord Of the Rings..
    virtual void Visit_RootFolder(const RootFolder&);

    // Predefined Types
    virtual void Visit_Boolean(const Boolean&);
    virtual void Visit_Byte(const Byte&);
    virtual void Visit_String(const String&);
    virtual void Visit_RealNumber(const RealNumber&);
    virtual void Visit_ShortInteger(const ShortInteger&);
    virtual void Visit_LongInteger(const LongInteger&);

    virtual void Visit_GenericValue(const GenericValue&){};
    virtual void Visit_TypeKind(const TypeKind&){};
    virtual void Visit_GenericObject(const GenericObject&){};
    virtual void Visit_PredefinedTypes(const PredefinedTypes&){};
    virtual void Visit_TypeEncoding(const TypeEncoding&){};
    virtual void Visit_GenericValueObject(const GenericValueObject&){};

    // ImplementationArtifact operations

    virtual void Visit_ImplementationArtifacts(const ImplementationArtifacts&);
    virtual void Visit_ArtifactContainer(const ArtifactContainer&);
    virtual void Visit_ImplementationArtifact(const ImplementationArtifact&);
    virtual void Visit_ArtifactDependsOn(const ArtifactDependsOn&);
    virtual void Visit_ArtifactDependency(const ArtifactDependency&);
    virtual void Visit_ImplementationArtifactReference(const ImplementationArtifactReference&);
    virtual void Visit_ArtifactExecParameter(const ArtifactExecParameter&);
    virtual void Visit_Property(const Property&);
    virtual void Visit_DataType(const DataType&);

    virtual void Visit_ArtifactDeployRequirement(const ArtifactDeployRequirement&);
    virtual void Visit_ArtifactInfoProperty(const ArtifactInfoProperty&);
    virtual void Visit_ImplementationRequirement(const ImplementationRequirement&);

    // TopLevelPackage operations

    virtual void Visit_TopLevelPackages(const TopLevelPackages&);
    virtual void Visit_TopLevelPackageContainer(const TopLevelPackageContainer&);
    virtual void Visit_TopLevelPackage(const TopLevelPackage&);
    virtual void Visit_package(const package&);
    virtual void Visit_PackageConfigurationReference(const PackageConfigurationReference&);

    // PackageConfiguration operations

    virtual void Visit_PackageConfigurations(const PackageConfigurations&);
    virtual void Visit_PackageConfigurationContainer(const PackageConfigurationContainer&);
    virtual void Visit_PackageConfiguration(const PackageConfiguration&);
    virtual void Visit_PackageConfConfigProperty(const PackageConfConfigProperty&);
    virtual void Visit_PackageConfReference(const PackageConfReference&);
    virtual void Visit_PackageConfSpecializedConfig(const PackageConfSpecializedConfig&);
    virtual void Visit_PackageConfSelectRequirement(const PackageConfSelectRequirement&);
    virtual void Visit_PackageConfBasePackage(const PackageConfBasePackage&);
    virtual void Visit_ComponentPackageReference(const ComponentPackageReference&);

    // ComponentPackage operations

    virtual void Visit_ComponentPackages(const ComponentPackages&);
    virtual void Visit_PackageContainer(const PackageContainer&);
    virtual void Visit_ComponentPackage(const ComponentPackage&);
    virtual void Visit_PackageInterface(const PackageInterface&);
    virtual void Visit_PackageConfigProperty(const PackageConfigProperty&);
    virtual void Visit_PackageInfoProperty(const PackageInfoProperty&);
    virtual void Visit_Implementation(const Implementation&);
    virtual void Visit_ComponentImplementationReference(const ComponentImplementationReference&);

    // ComponentInterface operations

    virtual void Visit_ComponentTypes(const ComponentTypes&);
    virtual void Visit_ComponentContainer(const ComponentContainer&);
    virtual void Visit_ComponentRef(const ComponentRef&);
    virtual void Visit_ComponentPropertyDescription(const ComponentPropertyDescription&);
    virtual void Visit_ComponentProperty(const ComponentProperty&);
    virtual void Visit_ComponentInfoProperty(const ComponentInfoProperty&);
    virtual void Visit_ComponentConfigProperty(const ComponentConfigProperty&);
    virtual void Visit_OutEventPort(const OutEventPort&);
    virtual void Visit_ProvidedRequestPort(const ProvidedRequestPort&);
    virtual void Visit_Component(const Component&);
    virtual void Visit_RequiredRequestPort(const RequiredRequestPort&);
    virtual void Visit_InEventPort(const InEventPort&);
    virtual void Visit_Supports(const Supports&);



    // ComponentImplementation operations

    virtual void Visit_ComponentImplementations(const ComponentImplementations&);
    virtual void Visit_ComponentImplementationContainer(const ComponentImplementationContainer&);
    virtual void Visit_MonolithicImplementation(const MonolithicImplementation&);
    virtual void Visit_MonolithExecParameter(const MonolithExecParameter&);

    virtual void Visit_Requirement(const Requirement&);
    virtual void Visit_SatisfierProperty(const SatisfierProperty&);
    virtual void Visit_ImplementationDependency(const ImplementationDependency&);
    virtual void Visit_Capability(const Capability&);
    virtual void Visit_AssemblyselectRequirement(const AssemblyselectRequirement&);
    virtual void Visit_AssemblyConfigProperty(const AssemblyConfigProperty&);
    virtual void Visit_PublishConnector(const PublishConnector&);
    virtual void Visit_publish(const publish&);
    virtual void Visit_deliverTo(const deliverTo&);
    virtual void Visit_AssemblyDeployRequirement(const AssemblyDeployRequirement&);
    virtual void Visit_ComponentAssembly(const ComponentAssembly&);
    virtual void Visit_emit(const emit&);
    virtual void Visit_invoke(const invoke&);

    virtual void Visit_InfoProperty(const InfoProperty&);
    virtual void Visit_MonolithprimaryArtifact(const MonolithprimaryArtifact&);
    virtual void Visit_MonolithDeployRequirement(const MonolithDeployRequirement&);
    virtual void Visit_ConfigProperty(const ConfigProperty&);
    virtual void Visit_ImplementationDependsOn(const ImplementationDependsOn&);
    virtual void Visit_Implements(const Implements&);
    virtual void Visit_ImplementationCapability(const ImplementationCapability&);
    // Component Attribute related operations
    virtual void Visit_ReadonlyAttribute(const ReadonlyAttribute&);
    virtual void Visit_AttributeValue(const AttributeValue&);
    virtual void Visit_AttributeDelegate(const AttributeDelegate&);
    virtual void Visit_AttributeMapping(const AttributeMapping&);
    virtual void Visit_AttributeMappingValue(const AttributeMappingValue&);
    virtual void Visit_AttributeMappingDelegate(const AttributeMappingDelegate&);

    // Critical Path related operations

    virtual void Visit_PathReference(const PathReference&);
    virtual void Visit_Path(const Path&);
    virtual void Visit_CriticalPath(const CriticalPath&);

    // Supported Types Specific operations
    virtual void Visit_Object(const Object&);
    virtual void Visit_Event(const Event&);

    private:


    void init();
    void initTarget (const string& fileName);
    void initDocument (const string& rootName);
    void initRootAttributes();
    void dumpDocument();

    void push();
    void pop();
    string ExtractName (Udm::Object obj);
    DOMElement* createSimpleContent (const string& name,
                                     const string& value);

    void GetReceptacleComponents (const RequiredRequestPort& receptacle,
                                  map<Component,string>& output);

    void GetFacetComponents (const ProvidedRequestPort& facet,
                             map<Component,string>& output);

    void GetEventSinkComponents (const InEventPort& consumer,
                                 map<Component,string>& output);

    void GetEventSourceComponents (const OutEventPort& publisher,
                                   map<Component,string>& output);

    void GetAttributeComponents (const AttributeMapping& mapping,
                                 set<pair<string, string> >& output);

    template <typename T, typename Del, typename DelRet, typename DelEndRet>
      void GetComponents (const T& port,
                          DelRet (T::*srcDel)() const,
                          DelRet (T::*dstDel) () const,
                          DelEndRet (Del::*srcDelEnd)() const,
                          DelEndRet (Del::*dstDelEnd)() const,
                          map<Component, string>& output,
                          set<T>& visited);

    void CreateAssemblies (const ComponentAssembly& assembly);

    void CreateConnections (const map<Component, string>& src,
                            const map<Component, string>& dst);

    void CreateConnection (const Component& srcComp,
                           const string& srcPortName,
                           const Component& dstComp,
                           const string& dstPortName);
    void CreateAssemblyInstances (set<Component>& comps);
    void CreateAssemblyConnections (vector<ComponentAssembly>& assemblies);
    void CreateAttributeMappings (vector<ComponentAssembly>& assemblies);
    void CreatePropertyElement (string name, const Property& property);
    void DumpStringProperty (const string& name, const string& pvalue);

    string CreatePath (const DisplayNode& node);
    string CreateRepositoryId (const Udm::Object& comp);
    void CreateComponentCanonicalIds();
    void CreateObjectCanonicalIds();
    void CreateEventCanonicalIds();
    void CollectSupportedTypes(const Component& comp);
    void CollectSupportedTypes (const Object& obj, set<string>& supportedTypes);
    void CollectSupportedTypes (const Event& event,
                                set<string>& supportedTypes);

    private:

    DOMImplementation*  impl_;
    DOMDocument*        doc_;
    DOMElement*         root_;
    DOMElement*         curr_;
    DOMWriter*          serializer_;
    XMLFormatTarget*    target_;
    string         outputPath_;
    stack<DOMElement*> curr_stack_;

    // Maintain associations between ComponentType and ComponentPackages
    map<string, string> interfaces_;

    // Maintain associations between PublishConnector and event publishers
    map<string, OutEventPort> publishers_;

    // Maintain associations between PublishConnector and event consumers
    multimap<string, InEventPort> consumers_;

    // Maintain association between pair<Component, Readonlyattribute> and
    // Property
    map<pair<string, string>, Property> attrValues_;

  };
}

#endif /* PACKAGE_VISITOR_H */
