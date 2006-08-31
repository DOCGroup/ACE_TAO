#include <algorithm>
#include <functional>
#include "Package/PackageVisitor.h"
#include "UmlExt.h"

using Utils::XStr;
using Utils::CreateUuid;

namespace PICML
{
  using xercesc::LocalFileFormatTarget;
  using xercesc::DOMImplementationRegistry;
  using xercesc::DOMImplementationLS;
  using xercesc::DOMException;
  using xercesc::XMLUni;
  using xercesc::XMLException;
  using xercesc::DOMText;
  using std::set;
  using std::vector;
  using std::string;
  using std::map;

  PackageVisitor::PackageVisitor (const string& outputPath)
    : impl_ (0), doc_ (0), root_ (0), curr_ (0), serializer_ (0), target_ (0),
      outputPath_ (outputPath)
  {
    this->init();
  }

  PackageVisitor::~PackageVisitor ()
  {
    if (this->doc_)
      this->doc_->release();
    delete this->target_;
  }

  void PackageVisitor::init()
  {
    this->impl_ = DOMImplementationRegistry::getDOMImplementation(XStr("LS"));
    this->serializer_ = ((DOMImplementationLS*)impl_)->createDOMWriter();

    // Set features if the serializer supports the feature/mode
    if (this->serializer_->canSetFeature
        (XMLUni::fgDOMWRTDiscardDefaultContent, true))
      this->serializer_->setFeature (XMLUni::fgDOMWRTDiscardDefaultContent,
                                     true);

    if (this->serializer_->canSetFeature (XMLUni::fgDOMWRTFormatPrettyPrint,
                                          true))
      this->serializer_->setFeature (XMLUni::fgDOMWRTFormatPrettyPrint, true);

    if (this->serializer_->canSetFeature (XMLUni::fgDOMWRTBOM, false))
      this->serializer_->setFeature (XMLUni::fgDOMWRTBOM, false);
  }

  void PackageVisitor::initTarget (const string& fileName)
  {
    if (this->target_)
      delete this->target_;
    this->target_ = new LocalFileFormatTarget (fileName.c_str());
  }

  void PackageVisitor::initDocument (const string& rootName)
  {
    if (this->doc_)
      this->doc_->release();
    // Create the document
    this->doc_ =
      this->impl_->createDocument (XStr ("http://www.omg.org/Deployment"),
                                   XStr (rootName.c_str()),
                                   0);
  }

  void PackageVisitor::initRootAttributes()
  {
    this->doc_->setEncoding (XStr("UTF-8"));
    this->doc_->setVersion (XStr("1.0"));
    this->root_ = this->doc_->getDocumentElement();
    this->root_->setAttributeNS (XStr ("http://www.w3.org/2000/xmlns/"),
                                 XStr ("xmlns:Deployment"),
                                 XStr ("http://www.omg.org/Deployment"));
    this->root_->setAttributeNS (XStr ("http://www.w3.org/2000/xmlns/"),
                                 XStr ("xmlns:xsi"),
                                 XStr ("http://www.w3.org/2001/XMLSchema-instance"));
    this->root_->setAttributeNS (XStr ("http://www.w3.org/2000/xmlns/"),
                                 XStr ("xmlns:xmi"),
                                 XStr ("http://www.omg.org/XMI"));
    this->root_->setAttribute (XStr ("xsi:schemaLocation"),
                               XStr ("http://www.omg.org/Deployment Deployment.xsd"));
    this->curr_ = this->root_;
  }

  void PackageVisitor::dumpDocument()
  {
    this->serializer_->writeNode (this->target_, *this->doc_);
  }

  void PackageVisitor::push()
  {
    this->curr_stack_.push (this->curr_);
  }

  void PackageVisitor::pop()
  {
    if (!this->curr_stack_.empty())
      {
        this->curr_ = this->curr_stack_.top();
        this->curr_stack_.pop();
      }
    else
      {
        throw udm_exception ("Mismatched push/pop from stack!");
      }
  }

  DOMElement* PackageVisitor::createSimpleContent (const string& name,
                                                   const string& value)
  {
    DOMElement* pName = this->doc_->createElement (XStr (name.c_str()));
    DOMText* pValue = this->doc_->createTextNode (XStr (value.c_str()));
    pName->appendChild (pValue);
    return pName;
  }

  void PackageVisitor::Visit_RootFolder(const RootFolder& rf)
  {
    set<RootFolder> rfolders = rf.RootFolder_kind_children();
    for (set<RootFolder>::iterator iter = rfolders.begin();
         iter != rfolders.end();
         ++iter)
      {
        RootFolder rfolder = *iter;
        rfolder.Accept (*this);
      }

    set<ComponentPackages>
      pkgs = rf.ComponentPackages_kind_children();
    for (set<ComponentPackages>::iterator iter = pkgs.begin();
         iter != pkgs.end();
         ++iter)
      {
        ComponentPackages folder = *iter;
        folder.Accept (*this);
      }
    set<ComponentImplementations>
      impls = rf.ComponentImplementations_kind_children();
    for (set<ComponentImplementations>::iterator iter = impls.begin();
         iter != impls.end();
         ++iter)
      {
        ComponentImplementations folder = *iter;
        folder.Accept (*this);
      }
    set<ComponentTypes>
      comps = rf.ComponentTypes_kind_children();
    for (set<ComponentTypes>::iterator iter = comps.begin();
         iter != comps.end();
         ++iter)
      {
        ComponentTypes folder = *iter;
        folder.Accept (*this);
      }
    set<ImplementationArtifacts>
      artifacts = rf.ImplementationArtifacts_kind_children();
    for (set<ImplementationArtifacts>::iterator iter = artifacts.begin();
         iter != artifacts.end();
         ++iter)
      {
        ImplementationArtifacts folder = *iter;
        folder.Accept (*this);
      }
    set<TopLevelPackages>
      tlpkgs = rf.TopLevelPackages_kind_children();
    for (set<TopLevelPackages>::iterator iter = tlpkgs.begin();
         iter != tlpkgs.end();
         ++iter)
      {
        TopLevelPackages folder = *iter;
        folder.Accept (*this);
      }
    set<PackageConfigurations>
      pkgConfs = rf.PackageConfigurations_kind_children();
    for (set<PackageConfigurations>::iterator iter = pkgConfs.begin();
         iter != pkgConfs.end();
         ++iter)
      {
        PackageConfigurations folder = *iter;
        folder.Accept (*this);
      }
  }

  // Implementation Artifact operations

  void PackageVisitor::Visit_ImplementationArtifacts(const ImplementationArtifacts& ia)
  {
    set<ArtifactContainer>
      afs = ia.ArtifactContainer_kind_children();
    for (set<ArtifactContainer>::iterator iter = afs.begin();
         iter != afs.end();
         ++iter)
      {
        ArtifactContainer ac = *iter;
        ac.Accept (*this);
      }
  }

  void PackageVisitor::Visit_ArtifactContainer(const ArtifactContainer& ac)
  {
    const set<ImplementationArtifact>
      ias = ac.ImplementationArtifact_kind_children();
    for (set<ImplementationArtifact>::const_iterator iter = ias.begin();
         iter != ias.end();
         ++iter)
      {
        ImplementationArtifact ia = *iter;
        ia.Accept (*this);
      }
  }

  void PackageVisitor::Visit_ImplementationArtifact(const ImplementationArtifact& ia)
  {
    this->push();
    string name = this->outputPath_ + "\\";
    name += ia.name();
    name += ".iad";
    this->initTarget (name);
    this->initDocument ("Deployment:implementationArtifactDescription");
    this->initRootAttributes();

    string label = ia.label();
    if (!label.empty())
      this->curr_->appendChild (this->createSimpleContent ("label",
                                                           label));
    string uuid (ia.UUID());
    if (uuid.empty())
      ia.UUID() = uuid = ::Utils::CreateUuid();
    this->curr_->appendChild (this->createSimpleContent ("UUID", uuid));

    string location = ia.location();
    // MAJO: Location field should not be empty or not present.  This
    // should be enforced in the model.
    this->curr_->appendChild (this->createSimpleContent ("location",
                                                         location));

    const set<ArtifactDependsOn> dps = ia.dstArtifactDependsOn();
    for (set<ArtifactDependsOn>::const_iterator iter = dps.begin();
         iter != dps.end();
         ++iter)
      {
        ArtifactDependsOn ad = *iter;
        ad.Accept (*this);
      }

    const set<ArtifactDependency> adps = ia.dstArtifactDependency();
    for (set<ArtifactDependency>::const_iterator iter = adps.begin();
         iter != adps.end();
         ++iter)
      {
        ArtifactDependency ad = *iter;
        ad.Accept (*this);
      }

    const set<ArtifactExecParameter> exec = ia.dstArtifactExecParameter();
    for (set<ArtifactExecParameter>::const_iterator it = exec.begin();
         it != exec.end();
         ++it)
      {
        ArtifactExecParameter aep = *it;
        aep.Accept (*this);
      }

    // Dump out an ImplementationArtifactDescription file
    this->dumpDocument();
    this->pop();
  }

  void PackageVisitor::Visit_ArtifactDependsOn(const ArtifactDependsOn& ado)
  {
    ImplementationArtifactReference ref = ado.dstArtifactDependsOn_end();
    ref.Accept (*this);
  }

  void PackageVisitor::Visit_ArtifactDependency(const ArtifactDependency& ado)
  {
    this->push();
    ImplementationArtifact ia = ado.dstArtifactDependency_end();
    DOMElement* ele = this->doc_->createElement (XStr ("dependsOn"));
    ele->appendChild (this->createSimpleContent ("name",
                                                 ia.getPath (".", false, true, "name", true)));
    string iaName (ia.name());
    iaName += ".iad";
    DOMElement*
      iaEle = this->doc_->createElement (XStr ("referencedArtifact"));
    iaEle->setAttribute (XStr ("href"), XStr (iaName));
    ele->appendChild (iaEle);
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PackageVisitor::Visit_ImplementationArtifactReference(const ImplementationArtifactReference& iar)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("dependsOn"));
    const ImplementationArtifact ref = iar.ref();
    ele->appendChild (this->createSimpleContent ("name",
                                                 ref.getPath (".", false, true, "name", true)));
    string refName (ref.name());
    refName += ".iad";
    DOMElement*
      refEle = this->doc_->createElement (XStr ("referencedArtifact"));
    refEle->setAttribute (XStr ("href"), XStr (refName));
    ele->appendChild (refEle);
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PackageVisitor::Visit_ArtifactExecParameter(const ArtifactExecParameter& param)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("execParameter"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    Property ref = param.dstArtifactExecParameter_end();
    ref.Accept (*this);
    this->pop();
  }

  void PackageVisitor::Visit_Property(const Property& property)
  {
    this->CreatePropertyElement (property.name(), property);
  }

  void PackageVisitor::CreatePropertyElement (string name, const Property& property)
  {
    this->push();
    this->curr_->appendChild (this->createSimpleContent ("name", name));
    // Property's value
    DOMElement* value = this->doc_->createElement (XStr ("value"));
    this->curr_->appendChild (value);
    this->curr_ = value;
    // Property's type
    DataType type = property.DataType_child();
    type.Accept (*this);
    // Property's type's value
    DOMElement* val = this->doc_->createElement (XStr ("value"));
    this->curr_->appendChild (val);
    this->curr_ = val;
    PredefinedType ref = type.ref();
    string refName = ref.name();
    if (refName == "Boolean")
      {
        this->curr_->appendChild (this->createSimpleContent ("boolean",
                                                             property.DataValue()));
      }
    else if (refName == "Byte")
      {
        this->curr_->appendChild (this->createSimpleContent ("octet",
                                                             property.DataValue()));
      }
    else if (refName == "String")
      {
        this->curr_->appendChild (this->createSimpleContent ("string",
                                                             property.DataValue()));
      }
    else if (refName == "RealNumber")
      {
        this->curr_->appendChild (this->createSimpleContent ("double",
                                                             property.DataValue()));
      }
    else if (refName == "ShortInteger")
      {
        this->curr_->appendChild (this->createSimpleContent ("short",
                                                             property.DataValue()));
      }
    else if (refName == "LongInteger")
      {
        this->curr_->appendChild (this->createSimpleContent ("long",
                                                             property.DataValue()));
      }
    this->pop();
  }

  void PackageVisitor::Visit_DataType(const DataType& type)
  {
    PredefinedType ref = type.ref();
    string kindName = ref.name();
    if (kindName == "Boolean")
      {
        Boolean boolv = PICML::Boolean::Cast (ref);
        boolv.Accept (*this);
      }
    else if (kindName == "Byte")
      {
        Byte byte = PICML::Byte::Cast (ref);
        byte.Accept (*this);
      }
    else if (kindName == "String")
      {
        String str = PICML::String::Cast (ref);
        str.Accept (*this);
      }
    else if (kindName == "RealNumber")
      {
        RealNumber real = PICML::RealNumber::Cast (ref);
        real.Accept (*this);
      }
    else if (kindName == "ShortInteger")
      {
        ShortInteger shortv = PICML::ShortInteger::Cast (ref);
        shortv.Accept (*this);
      }
    else if (kindName == "LongInteger")
      {
        LongInteger lint = PICML::LongInteger::Cast (ref);
        lint.Accept (*this);
      }
  }

  // Predefined Types
  void PackageVisitor::Visit_Boolean(const Boolean& boolean)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_boolean"));
    this->pop();

  }

  void PackageVisitor::Visit_Byte(const Byte& byte)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_octet"));
    this->pop();
  }

  void PackageVisitor::Visit_String(const String& str)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_string"));
    this->pop();
  }

  void PackageVisitor::Visit_RealNumber(const RealNumber& real)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_double"));
    this->pop();
  }

  void PackageVisitor::Visit_ShortInteger(const ShortInteger&)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_short"));
    this->pop();
  }

  void PackageVisitor::Visit_LongInteger(const LongInteger&)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_long"));
    this->pop();
  }

  void PackageVisitor::Visit_ArtifactDeployRequirement(const ArtifactDeployRequirement&)
  {}

  void PackageVisitor::Visit_ArtifactInfoProperty(const ArtifactInfoProperty&)
  {}

  void PackageVisitor::Visit_ImplementationRequirement(const ImplementationRequirement&)
  {}

  void PackageVisitor::Visit_TopLevelPackages(const TopLevelPackages& tp)
  {
    set<TopLevelPackageContainer>
      tpcs = tp.TopLevelPackageContainer_kind_children();
    for (set<TopLevelPackageContainer>::iterator iter = tpcs.begin();
         iter != tpcs.end();
         ++iter)
      {
        TopLevelPackageContainer tpc = *iter;
        tpc.Accept (*this);
      }
  }


  void PackageVisitor::Visit_TopLevelPackageContainer(const TopLevelPackageContainer& tpc)
  {
    set<TopLevelPackage> tps = tpc.TopLevelPackage_kind_children();
    for (set<TopLevelPackage>::iterator iter = tps.begin();
         iter != tps.end();
         ++iter)
      {
        TopLevelPackage tp = *iter;
        tp.Accept (*this);
      }
  }

  void PackageVisitor::Visit_TopLevelPackage(const TopLevelPackage& tp)
  {
    this->push();
    string name = this->outputPath_;
    name += "\\";
    name += tp.name();
    name += ".tpd";
    this->initTarget (name);
    this->initDocument ("Deployment:topLevelPackageDescription");
    this->initRootAttributes();

    package pkg = tp.dstpackage();
    pkg.Accept (*this);
    this->dumpDocument();
    this->pop();
  }

  void PackageVisitor::Visit_package(const package& pkg)
  {
    PackageConfigurationReference pcr = pkg.dstpackage_end();
    pcr.Accept (*this);
  }

  void PackageVisitor::Visit_PackageConfigurationReference(const PackageConfigurationReference& pcr)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("basePackage"));
    const PackageConfiguration pc = pcr.ref();
    string refName (pc.name());
    refName += ".pcd";
    ele->setAttribute (XStr ("href"), XStr (refName));
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PackageVisitor::Visit_PackageConfigurations(const PackageConfigurations& pcs)
  {
    set<PackageConfigurationContainer>
      pccs = pcs.PackageConfigurationContainer_kind_children();
    for (set<PackageConfigurationContainer>::iterator iter = pccs.begin();
         iter != pccs.end();
         ++iter)
      {
        PackageConfigurationContainer pcc = *iter;
        pcc.Accept (*this);
      }
  }

  void PackageVisitor::Visit_PackageConfigurationContainer(const PackageConfigurationContainer& pcc)
  {
    set<PackageConfiguration>
      pcs = pcc.PackageConfiguration_kind_children();
    for (set<PackageConfiguration>::iterator iter = pcs.begin();
         iter != pcs.end();
         ++iter)
      {
        PackageConfiguration pc = *iter;
        pc.Accept (*this);
      }
  }

  void PackageVisitor::Visit_PackageConfiguration(const PackageConfiguration& pc)
  {
    this->push();
    string name = this->outputPath_ + "\\";
    name += pc.name();
    name += ".pcd";
    this->initTarget (name);
    this->initDocument ("Deployment:packageConfiguration");
    this->initRootAttributes();

    string uuid (pc.UUID());
    if (uuid.empty())
      pc.UUID() = uuid = ::Utils::CreateUuid();
    this->curr_->appendChild (this->createSimpleContent ("UUID", uuid));

    PackageConfBasePackage bp = pc.dstPackageConfBasePackage();
    if (bp != Udm::null)
      bp.Accept (*this);
    else
      {
        PackageConfReference pcr = pc.dstPackageConfReference();
        if (pcr != Udm::null)
          pcr.Accept (*this);
        else
          {
            PackageConfSpecializedConfig
              pcsc = pc.dstPackageConfSpecializedConfig();
            if (pcsc != Udm::null)
              pcsc.Accept (*this);
          }
      }
    this->dumpDocument();
    this->pop();
  }

  void PackageVisitor::Visit_PackageConfBasePackage(const PackageConfBasePackage& pcbp)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("basePackage"));
    const ComponentPackage pkg = pcbp.dstPackageConfBasePackage_end();
    string pkgName (pkg.name());
    pkgName += ".cpd";
    ele->setAttribute (XStr ("href"), XStr (pkgName));
    this->curr_->appendChild (ele);
    this->pop();
  }


  void PackageVisitor::Visit_PackageConfReference(const PackageConfReference& pkgconfref)
  {
    ComponentPackageReference pkgref = pkgconfref.dstPackageConfReference_end();
    pkgref.Accept (*this);
  }

  void PackageVisitor::Visit_ComponentPackageReference(const ComponentPackageReference& pkgref)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("basePackage"));
    ComponentPackage pkg = pkgref.ref();
    string pkgName (pkg.name());
    pkgName += ".cpd";
    ele->setAttribute (XStr ("href"), XStr (pkgName));
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PackageVisitor::Visit_PackageConfConfigProperty(const PackageConfConfigProperty&)
  {}

  void PackageVisitor::Visit_PackageConfSpecializedConfig(const PackageConfSpecializedConfig&)
  {}

  void PackageVisitor::Visit_PackageConfSelectRequirement(const PackageConfSelectRequirement&)
  {}


  void PackageVisitor::Visit_ComponentPackages(const ComponentPackages& cps)
  {
    set<PackageContainer> pcs = cps.PackageContainer_kind_children();
    for (set<PackageContainer>::iterator iter = pcs.begin();
         iter != pcs.end();
         ++iter)
      {
        PackageContainer pc = *iter;
        pc.Accept (*this);
      }
  }

  void PackageVisitor::Visit_PackageContainer(const PackageContainer& pc)
  {
    set<ComponentPackage> cps = pc.ComponentPackage_kind_children();
    for (set<ComponentPackage>::iterator iter = cps.begin();
         iter != cps.end();
         ++iter)
      {
        ComponentPackage cp = *iter;
        cp.Accept (*this);
      }
  }

  void PackageVisitor::Visit_ComponentPackage(const ComponentPackage& cp)
  {
    this->push();
    string name = this->outputPath_ + "\\";
    name += cp.name();
    name += ".cpd";
    this->initTarget (name);
    this->initDocument ("Deployment:componentPackageDescription");
    this->initRootAttributes();

    string label = cp.label();
    if (!label.empty())
      this->curr_->appendChild (this->createSimpleContent ("label",
                                                           label));
    string uuid (cp.UUID());
    if (uuid.empty())
      cp.UUID() = uuid = ::Utils::CreateUuid();
    this->curr_->appendChild (this->createSimpleContent ("UUID", uuid));

    PackageInterface pi = cp.dstPackageInterface();
    if (pi != Udm::null)
      {
        const ComponentRef cref = pi.dstPackageInterface_end();
        const Component comp  = cref.ref();
        string refName (comp.name());
        this->interfaces_[refName] = cp.name();
        pi.Accept (*this);
      }

    set<Implementation> impls = cp.dstImplementation();
    for (set<Implementation>::const_iterator it = impls.begin();
         it != impls.end();
         ++it)
      {
        Implementation impl = *it;
        impl.Accept (*this);
      }
    // Dump out an ComponentPackageDescription file
    this->dumpDocument();
    this->pop();
  }

  void PackageVisitor::Visit_PackageInterface(const PackageInterface& pi)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("realizes"));
    const ComponentRef cref = pi.dstPackageInterface_end();
    const Component comp  = cref.ref();
    string refName (comp.name());
    refName += ".ccd";
    ele->setAttribute (XStr ("href"), XStr (refName));
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PackageVisitor::Visit_Implementation(const Implementation& impl)
  {
    ComponentImplementationReference cir = impl.dstImplementation_end();
    cir.Accept (*this);
  }

  void PackageVisitor::Visit_ComponentImplementationReference(const ComponentImplementationReference& cir)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("implementation"));
    const ComponentImplementation ref = cir.ref();
    ele->appendChild (this->createSimpleContent ("name",
                                                 ref.getPath (".", false, true, "name", true)));
    string refName (ref.name());
    refName += ".cid";
    DOMElement*
      refEle = this->doc_->createElement (XStr ("referencedImplementation"));
    refEle->setAttribute (XStr ("href"), XStr (refName));
    ele->appendChild (refEle);
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PackageVisitor::Visit_PackageConfigProperty(const PackageConfigProperty&)
  {}

  void PackageVisitor::Visit_PackageInfoProperty(const PackageInfoProperty&)
  {}

  void PackageVisitor::Visit_ComponentTypes(const ComponentTypes& cts)
  {
    set<ComponentContainer> ccs = cts.ComponentContainer_kind_children();
    for (set<ComponentContainer>::iterator iter = ccs.begin();
         iter != ccs.end();
         ++iter)
      {
        ComponentContainer cc = *iter;
        cc.Accept (*this);
      }
  }

  void PackageVisitor::Visit_ComponentContainer(const ComponentContainer& cc)
  {
    set<ComponentRef> cts = cc.ComponentRef_kind_children();
    for (set<ComponentRef>::iterator iter = cts.begin();
         iter != cts.end();
         ++iter)
      {
        ComponentRef ct = *iter;
        ct.Accept (*this);
      }
  }

  void PackageVisitor::Visit_ComponentRef(const ComponentRef& ct)
  {
	this->push();
	Component comp = ct.ref();
    string name = this->outputPath_ + "\\";
    name += comp.name();
    name += ".ccd";
    this->initTarget (name);
    this->initDocument ("Deployment:componentInterfaceDescription");
    this->initRootAttributes();
	comp.Accept (*this);
   
	const set<ComponentConfigProperty>
      ccps = ct.dstComponentConfigProperty();
    for (set<ComponentConfigProperty>::const_iterator it1 = ccps.begin();
         it1 != ccps.end();
         ++it1)
      {
        ComponentConfigProperty ccp = *it1;
        ccp.Accept (*this);
      }
	const set<ComponentInfoProperty>
      cips = ct.dstComponentInfoProperty();
    for (set<ComponentInfoProperty>::const_iterator it2 = cips.begin();
         it2 != cips.end();
         ++it2)
      {
        ComponentInfoProperty cip = *it2;
        cip.Accept (*this);
      }
	// Dump out an ComponentInterfaceDescription file
    this->dumpDocument();
    this->pop();
    
  }

  void PackageVisitor::CreateComponentCanonicalIds()
  {
    DOMElement* type = 0;
    type = this->createSimpleContent ("supportedType",
                                      "IDL:omg.org/Components/Navigation:1.0");
    this->curr_->appendChild (type);

    type = this->createSimpleContent ("supportedType",
                                      "IDL:omg.org/Components/Receptacles:1.0");
    this->curr_->appendChild (type);

    type = this->createSimpleContent ("supportedType",
                                      "IDL:omg.org/Components/Events:1.0");
    this->curr_->appendChild (type);

    type = this->createSimpleContent ("supportedType",
                                      "IDL:omg.org/Components/CCMObject:1.0");
    this->curr_->appendChild (type);

    this->CreateObjectCanonicalIds();
  }

  void PackageVisitor::CreateObjectCanonicalIds()
  {
    DOMElement* type = 0;
    type = this->createSimpleContent ("supportedType",
                                      "IDL:omg.org/CORBA/Object:1.0");
    this->curr_->appendChild (type);
  }

  void PackageVisitor::CreateEventCanonicalIds()
  {
    DOMElement* type = 0;
    type = this->createSimpleContent ("supportedType",
                                      "IDL:omg.org/Components/EventConsumerBase:1.0");
    this->curr_->appendChild (type);
    this->CreateObjectCanonicalIds();
  }

  string PackageVisitor::CreateRepositoryId (const Udm::Object& obj)
  {
    string id;
    Udm::Object parent;
    string prefix;
    string version;
    if (Udm::IsDerivedFrom (obj.type(), Component::meta))
      {
        const Component& comp = Component::Cast(obj);
        id = comp.name();
        version = comp.VersionTag();
        parent = comp.parent();
      }
    else if (Udm::IsDerivedFrom (obj.type(), Object::meta))
      {
        const Object& object = Object::Cast (obj);
        id = object.name();
        prefix = object.PrefixTag();
        version = object.VersionTag();
        parent = object.parent();
      }
    else if (Udm::IsDerivedFrom (obj.type(), Event::meta))
      {
        const Event& event = Event::Cast (obj);
        id = event.name();
        // Add Consumer to the name of the event as required by the spec
        id += "Consumer";
        prefix = event.PrefixTag();
        version = event.VersionTag();
        parent = event.parent();
      }
    else
      throw udm_exception ("Invalid object passed to CreateRepositoryId()");

    if (version.empty())
      version = "1.0";
    while (parent != Udm::null)
      {
        if (Udm::IsDerivedFrom (parent.type(), File::meta))
          {
            File file = File::Cast (parent);
            if (prefix.empty())
              prefix = file.PrefixTag();
            id.insert (0, prefix);
            id.insert (0, "IDL:");
            id.append (":");
            id.append (version);
            return id;
          }
        else if (Udm::IsDerivedFrom (parent.type(), Package::meta))
          {
            Package package = Package::Cast (parent);
            if (prefix.empty())
              prefix = package.PrefixTag();
            string pkgName = package.name();
            pkgName.append ("/");
            id.insert (0, pkgName);
            parent = package.parent();
          }
        else
          throw udm_exception (string ("Invalid parent for object"
                                            + id));
     }
    return id;
  }

  void PackageVisitor::CollectSupportedTypes(const Component& comp)
  {
    // If the user specifies a SpecifyIdTag, we should honor it.
    string specificType = comp.SpecifyIdTag();
    if (specificType.empty())
      specificType = this->CreateRepositoryId (comp);
    // Add our specific type as a supported type
    this->curr_->appendChild (this->createSimpleContent ("supportedType",
                                                             specificType));
    // Add all the parent component types
    if (comp.isSubtype())
      {
        // Cast away constness
        Component ncomp = comp;
        Component parent = ncomp.Archetype();
        this->CollectSupportedTypes (parent);
      }
    else
      {
        // Add all our supported interfaces
        set<Supports> supports = comp.Supports_kind_children();
        for (set<Supports>::iterator iter = supports.begin();
             iter != supports.end();
             ++iter)
          {
            Supports supported = *iter;
            Object interFace = supported.ref();
            interFace.Accept (*this);
          }
      }
  }

  void PackageVisitor::CollectSupportedTypes (const Object& obj,
                                              set<string>& supportedTypes)
  {
    set<Inherits> inherited = obj.Inherits_kind_children();
    for (set<Inherits>::iterator iter = inherited.begin();
         iter != inherited.end();
         ++iter)
      {
        Inherits inheritedInterface = *iter;
        Object interFace = Object::Cast (inheritedInterface.ref());
        this->CollectSupportedTypes (interFace, supportedTypes);
      }
    // If the user specifies a SpecifyIdTag, we should honor it.
    string specificType = obj.SpecifyIdTag();
    if (specificType.empty())
      specificType = this->CreateRepositoryId (obj);
    supportedTypes.insert (specificType);
  }


  void PackageVisitor::Visit_Object (const Object& obj)
  {
    set<string> supportedTypes;
    this->CollectSupportedTypes (obj, supportedTypes);

    for (set<string>::iterator iter = supportedTypes.begin();
         iter != supportedTypes.end();
         ++iter)
      {
        // Add our specific type as a supported type
        this->curr_->appendChild (this->createSimpleContent ("supportedType",
                                                             *iter));
      }
  }

  void PackageVisitor::CollectSupportedTypes (const Event& event,
                                              set<string>& supportedTypes)
  {
    set<Inherits> inherited = event.Inherits_kind_children();
    for (set<Inherits>::iterator iter = inherited.begin();
         iter != inherited.end();
         ++iter)
      {
        Inherits inheritedEvent = *iter;
        Event baseEvent = Event::Cast (inheritedEvent.ref());
        this->CollectSupportedTypes (baseEvent, supportedTypes);
      }
    // If the user specifies a SpecifyIdTag, we should honor it.
    string specificType = event.SpecifyIdTag();
    if (specificType.empty())
      specificType = this->CreateRepositoryId (event);
    supportedTypes.insert (specificType);
  }

  void PackageVisitor::Visit_Event (const Event& event)
  {
    set<string> supportedTypes;
    this->CollectSupportedTypes (event, supportedTypes);

    for (set<string>::iterator iter = supportedTypes.begin();
         iter != supportedTypes.end();
         ++iter)
      {
        // Add our specific type as a supported type
        this->curr_->appendChild (this->createSimpleContent ("supportedType",
                                                             *iter));
      }
  }

  void PackageVisitor::Visit_Component(const Component& comp)
  {
    this->push();
	/*
    string name = this->outputPath_ + "\\";
    name += comp.name();
    name += ".ccd";
    this->initTarget (name);
    this->initDocument ("Deployment:componentInterfaceDescription");
    this->initRootAttributes();
*/
    string label = comp.label();
    if (!label.empty())
      this->curr_->appendChild (this->createSimpleContent ("label",
                                                           label));
    string uuid (comp.UUID());
    if (uuid.empty())
      comp.UUID() = uuid = ::Utils::CreateUuid();
    this->curr_->appendChild (this->createSimpleContent ("UUID", uuid));

    // If the user specifies a SpecifyIdTag, we should honor it.
    string specificType = comp.SpecifyIdTag();
    if (specificType.empty())
      specificType = this->CreateRepositoryId (comp);
    this->curr_->appendChild (this->createSimpleContent ("specificType",
                                                         specificType));

    // Add all inherited types
    this->CollectSupportedTypes (comp);

    // Add all the standard supported types
    this->CreateComponentCanonicalIds();

    // Add the idlFile we were defined in
    Udm::Object parent = comp.parent();
    while (parent != Udm::null)
      {
        if (Udm::IsDerivedFrom (parent.type(), File::meta))
          {
            File file = File::Cast (parent);
            string idlFile = file.name();
            idlFile += ".idl";
            this->curr_->appendChild (this->createSimpleContent ("idlFile",
                                                                 idlFile));
            break;
          }
        else if (Udm::IsDerivedFrom (parent.type(), Package::meta))
          {
            Package package = Package::Cast (parent);
            parent = package.parent();
          }
        else
          throw udm_exception ("Invalid hierarchy in interface definition!");
      }

    const set<OutEventPort> oeps = comp.OutEventPort_kind_children();
    for (set<OutEventPort>::const_iterator it1 = oeps.begin();
         it1 != oeps.end();
         ++it1)
      {
        OutEventPort oep = *it1;
        oep.Accept (*this);
      }

    const set<InEventPort> ieps = comp.InEventPort_kind_children();
    for (set<InEventPort>::const_iterator it2 = ieps.begin();
         it2 != ieps.end();
         ++it2)
      {
        InEventPort iep = *it2;
        iep.Accept (*this);
      }

    const set<ProvidedRequestPort>
      facets = comp.ProvidedRequestPort_kind_children();
    for (set<ProvidedRequestPort>::const_iterator it3 = facets.begin();
         it3 != facets.end();
         ++it3)
      {
        ProvidedRequestPort facet = *it3;
        facet.Accept (*this);
      }

    const set<RequiredRequestPort>
      receps = comp.RequiredRequestPort_kind_children();
    for (set<RequiredRequestPort>::const_iterator it4 = receps.begin();
         it4 != receps.end();
         ++it4)
      {
        RequiredRequestPort recep = *it4;
        recep.Accept (*this);
      }

	// Dump out an ComponentInterfaceDescription file
  //  this->dumpDocument();
    this->pop();
  }

  void PackageVisitor::Visit_OutEventPort(const OutEventPort& oep)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("port"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;

    this->curr_->appendChild (this->createSimpleContent ("name", oep.name()));

    Event referred = Event::Cast (oep.ref());
    // If the user specifies a SpecifyIdTag, we should honor it.
    string specificType = referred.SpecifyIdTag();
    if (specificType.empty())
      specificType = this->CreateRepositoryId (referred);
    this->curr_->appendChild (this->createSimpleContent ("specificType",
                                                         specificType));
    // Generate all the supported types
    referred.Accept (*this);

    // Add all the standard supported types
    this->CreateEventCanonicalIds();

    this->curr_->appendChild (this->createSimpleContent ("provider", "true"));
    this->curr_->appendChild (this->createSimpleContent ("exclusiveProvider",
                                                 oep.exclusiveProvider() ? "true" : "false"));
    this->curr_->appendChild (this->createSimpleContent ("exclusiveUser",
                                                 oep.exclusiveUser() ? "true" : "false"));
    this->curr_->appendChild (this->createSimpleContent ("optional",
                                                 oep.optional() ? "true" : "false"));

    // Check whether it is a RT_Event_Channel out port
    string out_port_type = oep.out_event_port_type ();
    if (oep.exclusiveProvider())
      {
        if (out_port_type == "DirectConnect")
          this->curr_->appendChild (this->createSimpleContent ("kind", "EventEmitter"));
        else // must be RT Event Channel
          this->curr_->appendChild (this->createSimpleContent ("kind", "rtecEventEmitter"));
      }
    else
      {
        if (out_port_type == "DirectConnect")
          this->curr_->appendChild (this->createSimpleContent ("kind", "EventPublisher"));
        else // must be RT Event Channel
          this->curr_->appendChild (this->createSimpleContent ("kind", "rtecEventPublisher"));
      }
    this->pop();
  }

  void PackageVisitor::Visit_InEventPort(const InEventPort& iep)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("port"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;

    this->curr_->appendChild (this->createSimpleContent ("name", iep.name()));

    Event referred = Event::Cast (iep.ref());
    // If the user specifies a SpecifyIdTag, we should honor it.
    string specificType = referred.SpecifyIdTag();
    if (specificType.empty())
      specificType = this->CreateRepositoryId (referred);
    this->curr_->appendChild (this->createSimpleContent ("specificType",
                                                         specificType));
    // Generate all the supported types
    referred.Accept (*this);

    // Add all the standard supported types
    this->CreateEventCanonicalIds();

    this->curr_->appendChild (this->createSimpleContent ("provider", "false"));
    this->curr_->appendChild (this->createSimpleContent ("exclusiveProvider",
                                                 iep.exclusiveProvider() ? "true" : "false"));
    this->curr_->appendChild (this->createSimpleContent ("exclusiveUser",
                                                 iep.exclusiveUser() ? "true" : "false"));
    this->curr_->appendChild (this->createSimpleContent ("optional",
                                                 iep.optional() ? "true" : "false"));

    this->curr_->appendChild (this->createSimpleContent ("kind", "EventConsumer"));
    this->pop();
  }

  void PackageVisitor::Visit_ProvidedRequestPort(const ProvidedRequestPort& facet)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("port"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    this->curr_->appendChild (this->createSimpleContent ("name", facet.name()));

    Object referred = Object::Cast (facet.ref());
    // If the user specifies a SpecifyIdTag, we should honor it.
    string specificType = referred.SpecifyIdTag();
    if (specificType.empty())
      specificType = this->CreateRepositoryId (referred);
    this->curr_->appendChild (this->createSimpleContent ("specificType",
                                                         specificType));
    // Generate all the supported types
    referred.Accept (*this);

    // Add all the standard supported types
    this->CreateObjectCanonicalIds();

    this->curr_->appendChild (this->createSimpleContent ("provider", "true"));
    this->curr_->appendChild (this->createSimpleContent ("exclusiveProvider",
                                                 facet.exclusiveProvider() ? "true" : "false"));
    this->curr_->appendChild (this->createSimpleContent ("exclusiveUser",
                                                 facet.exclusiveUser() ? "true" : "false"));
    this->curr_->appendChild (this->createSimpleContent ("optional",
                                                 facet.optional() ? "true" : "false"));
    this->curr_->appendChild (this->createSimpleContent ("kind", "Facet"));
    this->pop();
  }


  void PackageVisitor::Visit_RequiredRequestPort(const RequiredRequestPort& recep)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("port"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;

    this->curr_->appendChild (this->createSimpleContent ("name", recep.name()));

    Object referred = Object::Cast(recep.ref());
    // If the user specifies a SpecifyIdTag, we should honor it.
    string specificType = referred.SpecifyIdTag();
    if (specificType.empty())
      specificType = this->CreateRepositoryId (referred);
    this->curr_->appendChild (this->createSimpleContent ("specificType",
                                                         specificType));
    // Generate all the supported types
    referred.Accept (*this);

    // Add all the standard supported types
    this->CreateObjectCanonicalIds();

    this->curr_->appendChild (this->createSimpleContent ("provider", "false"));
    this->curr_->appendChild (this->createSimpleContent ("exclusiveProvider",
                                                         recep.exclusiveProvider() ? "true" : "false"));
    this->curr_->appendChild (this->createSimpleContent ("exclusiveUser",
                                                         recep.exclusiveUser() ? "true" : "false"));
    this->curr_->appendChild (this->createSimpleContent ("optional",
                                                         recep.optional() ? "true" : "false"));
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         recep.multiple_connections() ? "MultiplexReceptacle" : "SimplexReceptacle"));
    this->pop();
  }

  void PackageVisitor::Visit_ComponentPropertyDescription(const ComponentPropertyDescription&)
  {}

  void PackageVisitor::Visit_ComponentProperty(const ComponentProperty&)
  {}

  void PackageVisitor::Visit_ComponentInfoProperty(const ComponentInfoProperty& cip)
  {
	this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("infoProperty"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    Property ref = cip.dstComponentInfoProperty_end();
    ref.Accept (*this);
    this->pop();
	
  }

  void PackageVisitor::Visit_ComponentConfigProperty(const ComponentConfigProperty& ccp)
  {
	this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("configProperty"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    Property ref = ccp.dstComponentConfigProperty_end();
    ref.Accept (*this);
    this->pop();
	
  }

  void PackageVisitor::Visit_Supports(const Supports&)
  {}

  void PackageVisitor::Visit_ComponentImplementations(const ComponentImplementations& cimpls)
  {
    set<ComponentImplementationContainer>
      cics = cimpls.ComponentImplementationContainer_kind_children();
    for (set<ComponentImplementationContainer>::iterator
           iter = cics.begin();
         iter != cics.end();
         ++iter)
      {
        ComponentImplementationContainer cc = *iter;
        cc.Accept (*this);
      }
  }

  void PackageVisitor::Visit_ComponentImplementationContainer(const ComponentImplementationContainer& cic)
  {
    set<MonolithicImplementation>
      mimpls = cic.MonolithicImplementation_kind_children();
    for (set<MonolithicImplementation>::iterator iter = mimpls.begin();
         iter != mimpls.end();
         ++iter)
      {
        MonolithicImplementation mimpl = *iter;
        mimpl.Accept (*this);
      }

    set<ComponentAssembly> asms = cic.ComponentAssembly_kind_children();
    for (set<ComponentAssembly>::iterator it = asms.begin();
         it != asms.end();
         ++it)
      {
        ComponentAssembly assembly = *it;
        assembly.Accept (*this);
      }
  }

  void PackageVisitor::Visit_MonolithicImplementation(const MonolithicImplementation& mimpl)
  {
    this->push();
    string name = this->outputPath_ + "\\";
    name += mimpl.name();
    name += ".cid";
    this->initTarget (name);
    this->initDocument ("Deployment:componentImplementationDescription");
    this->initRootAttributes();

    string label = mimpl.label();
    if (!label.empty())
      this->curr_->appendChild (this->createSimpleContent ("label",
                                                           label));
    string uuid = mimpl.UUID();
    if (uuid.empty())
      mimpl.UUID() = uuid = ::Utils::CreateUuid();
    this->curr_->appendChild (this->createSimpleContent ("UUID", uuid));

    Implements iface = mimpl.dstImplements();
    iface.Accept (*this);

    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("monolithicImpl"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    const set<MonolithprimaryArtifact>
      mpas = mimpl.dstMonolithprimaryArtifact();
    for (set<MonolithprimaryArtifact>::const_iterator it = mpas.begin();
         it != mpas.end();
         ++it)
      {
        MonolithprimaryArtifact mpa = *it;
        mpa.Accept (*this);
      }
    this->pop();

    const set<ConfigProperty>
      cps = mimpl.dstConfigProperty();
    for (set<ConfigProperty>::const_iterator it2 = cps.begin();
         it2 != cps.end();
         ++it2)
      {
        ConfigProperty cp = *it2;
        cp.Accept (*this);
      }

    // Dump out an ComponentImplementationDescription file
    this->dumpDocument();
    this->pop();
  }

  void PackageVisitor::Visit_Implements(const Implements& impl)
  {
    this->push();
    const ComponentRef iface = impl.dstImplements_end();
    const Component ref = iface.ref();
    string refName (ref.name());
    refName += ".ccd";
    DOMElement*
      refEle = this->doc_->createElement (XStr ("implements"));
    refEle->setAttribute (XStr ("href"), XStr (refName));
    this->curr_->appendChild (refEle);
    this->pop();
  }

  void PackageVisitor::Visit_MonolithprimaryArtifact(const MonolithprimaryArtifact& mpa)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("primaryArtifact"));
    const ImplementationArtifactReference iaref = mpa.dstMonolithprimaryArtifact_end();
    const ImplementationArtifact ref = iaref.ref();
    ele->appendChild (this->createSimpleContent ("name",
                                                 ref.getPath (".", false, true, "name", true)));
    string refName = ref.name();
    refName += ".iad";
    DOMElement*
      refEle = this->doc_->createElement (XStr ("referencedArtifact"));
    refEle->setAttribute (XStr ("href"), XStr (refName));
    ele->appendChild (refEle);
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PackageVisitor::Visit_ConfigProperty(const ConfigProperty& cp)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("configProperty"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    Property ref = cp.dstConfigProperty_end();
    ref.Accept (*this);
    this->pop();
  }

  void PackageVisitor::Visit_AssemblyConfigProperty(const AssemblyConfigProperty& acp)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("configProperty"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    Property ref = acp.dstAssemblyConfigProperty_end();
    ref.Accept (*this);
    this->pop();
  }


  void PackageVisitor::Visit_ComponentAssembly(const ComponentAssembly& assembly)
  {
    this->push();
    string name = this->outputPath_ + "\\";
    name += assembly.name();
    name += ".cid";
    this->initTarget (name);
    this->initDocument ("Deployment:componentImplementationDescription");
    this->initRootAttributes();

    string label = assembly.label();
    if (!label.empty())
      this->curr_->appendChild (this->createSimpleContent ("label",
                                                           label));

    string uuid = assembly.UUID();
    if (uuid.empty())
      assembly.UUID() = uuid = ::Utils::CreateUuid();
    // Make a copy as opposed to casting away constness
    ComponentAssembly casm = assembly;
    // Make sure that every instance has a UUID
    if (assembly.isInstance())
      {
        ComponentAssembly typeParent = casm.Archetype();
        string parentUuid (typeParent.UUID());
        if (uuid == parentUuid)
          casm.UUID() = uuid = ::Utils::CreateUuid();
      }
    this->curr_->appendChild (this->createSimpleContent ("UUID", uuid));

    this->CreateAssemblies (assembly);

	set<ConfigProperty> cps = assembly.dstConfigProperty ();
	// Handle configProperties on the assembly
	for (set<ConfigProperty>::const_iterator it2 = cps.begin();
         it2 != cps.end();
         ++it2)
      {
        ConfigProperty cp = *it2;
        cp.Accept (*this);
      }
	// @@Note:  The Critical path MUST be generated immediately AFTER any configproperties.
    CriticalPath cpath = assembly.dstCriticalPath();
    if (cpath != Udm::null)
      cpath.Accept (*this);

	// Handle infoProperties on the assembly
	set< InfoProperty > ips = assembly.dstInfoProperty ();
	for (set<InfoProperty>::const_iterator it2 = ips.begin();
         it2 != ips.end();
         ++it2)
      {
        InfoProperty ip = *it2;
        ip.Accept (*this);
      }

    // Dump out an ComponentImplementationDescription file
    this->dumpDocument();
    this->pop();
  }

  void PackageVisitor::Visit_CriticalPath (const CriticalPath& cpath)
  {
    PathReference pref = cpath.dstCriticalPath_end();
    pref.Accept (*this);
  }


  void PackageVisitor::Visit_PathReference (const PathReference& pref)
  {
    Path criticalpath = pref.ref();
    criticalpath.Accept (*this);
  }

  void PackageVisitor::Visit_Path (const Path& path)
  {
    // Dump the top-level pointer to the critical path
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("configProperty"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    string pname = "edu.vanderbilt.dre.CIAO.RACE.criticalPath";
    this->DumpStringProperty (pname,
                              path.getPath (".", false, true, "name",
                                            true));
    this->pop();

    // Dump all the properties that are associated with the Path.
    set<PathProperty> properties = path.dstPathProperty();
    for (set<PathProperty>::iterator iter = properties.begin();
         iter != properties.end();
         ++iter)
      {
        this->push();
        DOMElement* ele = this->doc_->createElement (XStr ("configProperty"));
        this->curr_->appendChild (ele);
        this->curr_ = ele;
        PathProperty pprop = *iter;
        Property prop = pprop.dstPathProperty_end();
        string pname = path.getPath (".", false, true, "name", true);
        pname += "/";
        pname += prop.name();
        this->CreatePropertyElement (pname, prop);
        this->pop();
      }

    // Find the source node of the path.  Need to fix the metamodel so that
    // we don't need to do an O(n) search to find the source node.
    set<DisplayNode> nodes = path.DisplayNode_kind_children();
    for (set<DisplayNode>::iterator iter = nodes.begin();
         iter != nodes.end();
         ++iter)
      {
        DisplayNode node = *iter;
        SrcEdge edge = node.srcSrcEdge();
        if (edge == Udm::null)
          {
            // Dump the value of the criticalPath
            this->push();
            DOMElement* ele =
              this->doc_->createElement (XStr ("configProperty"));
            this->curr_->appendChild (ele);
            this->curr_ = ele;
            string pvalue = this->CreatePath (node);
            this->DumpStringProperty (path.getPath(".", false, true, "name",
                                                  true), pvalue);
            this->pop();
            break;
          }
      }
  }

  string PackageVisitor::CreatePath (const DisplayNode& node)
  {
    // Handle the source node
    string nodename = node.name();
    string path ("");
    size_t offset = nodename.find ('/');
    if (offset == string::npos)
      throw udm_exception (string ("Invalid node name" + nodename));
    // Append component's uuid
    path.append (nodename, 0, offset);
    path += ',';
    // Append source port name
    path.append (nodename, offset + 1, nodename.size() - offset - 1);
    SrcEdge source = node.srcSrcEdge();
    if (source != Udm::null)
      throw  udm_exception
        (string ("Invalid detection of source port" + nodename));
    path += ',';
    // Append destination port name to be the same as source port name
    path.append (nodename, offset + 1, nodename.size() - offset - 1);

    // Get the connection to the intermediate nodes
    DstEdge dst = node.dstDstEdge();
    while (dst != Udm::null)
      {
        path += ';';
        // Get the intermediate connection node
        Edge dstEdge = dst.dstDstEdge_end();
        SrcEdge dstNodeEdge = dstEdge.dstSrcEdge();
        if (dstNodeEdge == Udm::null)
          throw udm_exception (string ("Connection from"
                                            + this->ExtractName (dstEdge)
                                            + " is null!"));
        // Get the display node at the end of the connection node
        Udm::Object vertex = dstNodeEdge.dstSrcEdge_end();
        if (!Udm::IsDerivedFrom (vertex.type(), DisplayNode::meta))
          throw udm_exception (string ("Invalid object inheritance in " +
                                            this->ExtractName (vertex)));

        DisplayNode dstNode = DisplayNode::Cast (vertex);
        string dstNodeName = dstNode.name();
        offset = dstNodeName.find ('/');
        if (offset == string::npos)
          throw udm_exception (string ("Invalid node name"
                                            + dstNodeName));
        // Append Component's uuid
        path.append (dstNodeName, 0, offset);
        path += ',';
        // Append source port name
        path.append (dstNodeName, offset + 1,
                     dstNodeName.size() - offset - 1);
        // Get the connection to the intermediate connection node
        DstEdge nodeEdge = dstNode.dstDstEdge();
        if (nodeEdge == Udm::null)
          {
            // We have reached the end of the path.  Just append the
            // source port name as the destination port name.
            path += ',';
            // Append destination port name
            path.append (dstNodeName, offset + 1,
                         dstNodeName.size() - offset - 1);
            return path;
          }
        else
          {
            // Get the intermediate connection node
            dstEdge = nodeEdge.dstDstEdge_end();
            dstNodeEdge = dstEdge.dstSrcEdge();
            if (dstNodeEdge == Udm::null)
              throw udm_exception (string ("Connection from"
                                                + this->ExtractName (dstEdge)
                                                + " is null!"));
            // Get the display node at the end of the connection node
            vertex = dstNodeEdge.dstSrcEdge_end();
            if (!Udm::IsDerivedFrom (vertex.type(), DisplayNode::meta))
              throw udm_exception (string ("Invalid object inheritance in "
                                                 + this->ExtractName (vertex)));
            DisplayNode dnode = DisplayNode::Cast (vertex);
            string dnodename = dnode.name();
            offset = dnodename.find ('/');
            if (offset == string::npos)
              throw udm_exception (string ("Invalid node name"
                                                + dstNodeName));
            path += ',';
            // Append destination port name
            path.append (dnodename, offset + 1,
                         dnodename.size() - offset - 1);
            // Setup the loop for the next iteration
            dst = dnode.dstDstEdge();
          }
      }
    return path;
  }


  void PackageVisitor::DumpStringProperty (const string& name,
                                           const string& pvalue)
  {
    this->push();

    // Property's name
    this->curr_->appendChild (this->createSimpleContent ("name", name));
    // Property's value
    DOMElement* value = this->doc_->createElement (XStr ("value"));
    this->curr_->appendChild (value);
    this->curr_ = value;

    // Property's type
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind", "tk_string"));
    this->pop();

    // Property's type's value
    this->push();
    DOMElement* val = this->doc_->createElement (XStr ("value"));
    this->curr_->appendChild (val);
    this->curr_ = val;
    this->curr_->appendChild (this->createSimpleContent ("string", pvalue));
    this->pop();

    this->pop();
  }


  void PackageVisitor::CreateAssemblies (const ComponentAssembly& assembly)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("assemblyImpl"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;

    // Collect all the Components of this assembly into a set.
    set<Component> comps = assembly.Component_kind_children();

    // Add all the shared Components of this assembly into the set.  A
    // shared Component is implemented as a reference to a Component.  So
    // just traverse the reference and add it to the set.
    set<ComponentRef> scomps = assembly.ComponentRef_kind_children();
    for (set<ComponentRef>::const_iterator
           iter = scomps.begin();
         iter != scomps.end();
         ++iter)
      {
        const ComponentRef compRef = *iter;
        comps.insert (compRef.ref());
      }

    // Collect all the immediate ComponentAssembly children of this assembly
    set<ComponentAssembly>
      subasms = assembly.ComponentAssembly_kind_children();

    // Add all the shared ComponentAssemblies of the current assembly.
    // Like shared components, shared assemblies are also implemented as
    // references.  So just traverse the references, and add them to the set.
    set<ComponentAssemblyReference>
      sasms = assembly.ComponentAssemblyReference_kind_children();
    for (set<ComponentAssemblyReference>::const_iterator
           iter = sasms.begin();
         iter != sasms.end();
         ++iter)
      {
        const ComponentAssemblyReference asmRef = *iter;
        subasms.insert (asmRef.ref());
      }

    // Maintain a list of all ComponentAssemblies in this assembly
    vector<ComponentAssembly> assemblies;

    // Put ourselves in the global list first.
    assemblies.push_back (assembly);

    // Do a Depth-First search and collect all the ComponentAssembly,
    // Component children of this assembly, and add them to the
    // assembly-specific list.
    while (!subasms.empty())
      {
        ComponentAssembly rassembly = *subasms.begin();
        // Put the first assembly from the current list to the
        // assembly-specific list.
        assemblies.push_back (rassembly);

        // Remove this assembly so that we don't process it again.
        subasms.erase (rassembly);

        // Get the components of the current assembly
        set<Component> rcomps = rassembly.Component_kind_children();

        // Get the shared components of the current assembly
        scomps = rassembly.ComponentRef_kind_children();
        for (set<ComponentRef>::const_iterator
               iter = scomps.begin();
               iter != scomps.end();
             ++iter)
          {
            const ComponentRef compRef = *iter;
            rcomps.insert (compRef.ref());
          }
        // Insert both into the component list.
        comps.insert (rcomps.begin(), rcomps.end());

        // Get the subassemblies of the first assembly.
        set<ComponentAssembly>
          rasms = rassembly.ComponentAssembly_kind_children();

        // Add all the shared ComponentAssemblies of the current assembly.
        // Like shared components, shared assemblies are also implemented
        // as references.  So just traverse the references, and add them to
        // the set.
        set<ComponentAssemblyReference>
          sasms = rassembly.ComponentAssemblyReference_kind_children();
        for (set<ComponentAssemblyReference>::const_iterator
               iter = sasms.begin();
             iter != sasms.end();
             ++iter)
          {
            const ComponentAssemblyReference asmRef = *iter;
            rasms.insert (asmRef.ref());
          }

        // Insert them to the current list.
        subasms.insert (rasms.begin(), rasms.end());
      }

    // Create the appropriate component attribute value mappings
    this->CreateAttributeMappings (assemblies);
    this->CreateAssemblyInstances (comps);
    this->CreateAssemblyConnections (assemblies);
    this->pop();
  }


  void PackageVisitor::CreateAttributeMappings (vector<ComponentAssembly>& assemblies)
  {
    for (vector<ComponentAssembly>::iterator iter = assemblies.begin();
         iter != assemblies.end();
         ++iter)
      {
        ComponentAssembly assembly = *iter;
        string assemblyName = this->ExtractName (assembly);
        const set<AttributeMapping>
          mappings = assembly.AttributeMapping_kind_children();
        for (set<AttributeMapping>::const_iterator iter = mappings.begin();
             iter != mappings.end();
             ++iter)
          {
            AttributeMapping mapping = *iter;
            mapping.Accept (*this);
          }
      }
  }

  void PackageVisitor::Visit_AttributeMapping(const AttributeMapping& mapping)
  {
    string mappingName = this->ExtractName (mapping);
    AttributeMappingValue value = mapping.dstAttributeMappingValue();
    if (value != Udm::null)
      {
        Property prop = value.dstAttributeMappingValue_end();
        set<pair<string, string> > compAttrs;
        this->GetAttributeComponents (mapping, compAttrs);
        for (set<pair<string, string> >::const_iterator
               iter = compAttrs.begin();
             iter != compAttrs.end();
             ++iter)
          {
            // Get the component, attribute pair
            pair<string, string> compAttr = *iter;
            // Set the name of the associated property to the attribute name
            // prop.name() = compAttr.second;
            // If this component's attribute hasn't been assigned a value,
            // i.e., a value hasn't been propagated from a higher-level assembly,
            // set it to the current value.
            if (this->attrValues_.find (compAttr) == this->attrValues_.end())
              this->attrValues_[compAttr] = prop;
          }
      }
  }

  void PackageVisitor::GetAttributeComponents (const AttributeMapping& mapping,
                                               set<pair<string, string> >& output)
  {
    string mappingName = this->ExtractName (mapping);
    set<AttributeDelegate> delegates = mapping.dstAttributeDelegate();
    if (delegates.empty())
      {
        set<AttributeMappingDelegate>
          mapDelegates = mapping.dstAttributeMappingDelegate();
        if (mapDelegates.empty())
          {
            string mapPath = mapping.getPath (".", false, true, "name", true);

            throw udm_exception (string ("AttributeMapping " +
                                              mapPath +
                                              " is not connected to any attributes or delegated to another AttributeMapping"));
          }
        else
          {
            for (set<AttributeMappingDelegate>::const_iterator
                   iter = mapDelegates.begin();
                 iter != mapDelegates.end();
                 ++iter)
              {
                AttributeMappingDelegate mapDelegate = *iter;
                AttributeMapping
                  delegate = mapDelegate.dstAttributeMappingDelegate_end();
                string delegateName = this->ExtractName (delegate);
                this->GetAttributeComponents (delegate, output);
              }
          }
      }
    else
      {
        for (set<AttributeDelegate>::const_iterator
               iter = delegates.begin();
             iter != delegates.end();
             ++iter)
          {
            AttributeDelegate delegate = *iter;
            ReadonlyAttribute attr = delegate.dstAttributeDelegate_end();
            string attrName = this->ExtractName (attr);
            Component parent = attr.Component_parent();
            string parentName = this->ExtractName (parent);
            string compName = parent.getPath (".", false, true, "name", true);
            output.insert (make_pair (compName, attr.name()));
          }
      }
  }

  void PackageVisitor::CreateAssemblyInstances (set<Component>& comps)
  {
    for (set<Component>::iterator iter = comps.begin();
         iter != comps.end();
         ++iter)
      {
        Component comp = *iter;
        DOMElement* instance = this->doc_->createElement (XStr ("instance"));
        this->curr_->appendChild (instance);
        this->push();
        this->curr_ = instance;
        Component typeParent;
        string uniqueName = comp.UUID();
        // If the component's UUID is empty, then simply assign one.
        if (uniqueName.empty())
          comp.UUID() = uniqueName =  ::Utils::CreateUuid();
        // Make sure that every instance has a uniue UUID.
        if (comp.isInstance())
          {
            typeParent = comp.Archetype();
            string parentName (typeParent.UUID());
            if (uniqueName == parentName)
              comp.UUID() = uniqueName = ::Utils::CreateUuid();
          }
        uniqueName = string ("_") + uniqueName;
        instance->setAttribute (XStr ("id"), XStr (uniqueName));
        instance->appendChild (this->createSimpleContent ("name",
                                                          comp.getPath (".", false, true, "name", true)));

        if (comp.isInstance())
          {
            typeParent = comp.Archetype();
            while (typeParent.isInstance())
              typeParent = typeParent.Archetype();
          }
        string interfaceName = typeParent.name();
        string refName = this->interfaces_[interfaceName];
        refName += ".cpd";
        DOMElement* refEle = this->doc_->createElement (XStr ("basePackage"));
        refEle->setAttribute (XStr ("href"), XStr (refName));
        instance->appendChild (refEle);
        set<AssemblyConfigProperty> cps = comp.dstAssemblyConfigProperty();
        for (set<AssemblyConfigProperty>::const_iterator it2 = cps.begin();
             it2 != cps.end();
             ++it2)
          {
            AssemblyConfigProperty cp = *it2;
            cp.Accept (*this);
          }
        set<ReadonlyAttribute> attrs = comp.ReadonlyAttribute_children();
        for (set<ReadonlyAttribute>::const_iterator iter = attrs.begin();
             iter != attrs.end();
             ++iter)
          {
            ReadonlyAttribute attr = *iter;
            attr.Accept (*this);
          }
        for (map<pair<string, string>, Property>::const_iterator iter = this->attrValues_.begin();
             iter != this->attrValues_.end();
             ++iter)
          {
            pair<pair<string, string>, Property>
              attrVal = *iter;
            pair<string, string> compAttr = attrVal.first;
            if (compAttr.first == uniqueName)
              {
                this->push();
                DOMElement*
                  ele = this->doc_->createElement (XStr ("configProperty"));
                this->curr_->appendChild (ele);
                this->curr_ = ele;
                Property val = attrVal.second;
                this->CreatePropertyElement (compAttr.second, val);
                this->pop();
              }
          }
        this->pop();
      }
  }

  void PackageVisitor::Visit_ReadonlyAttribute(const ReadonlyAttribute& attr)
  {
    AttributeValue attValue = attr.dstAttributeValue();
    if (attValue != Udm::null)
      attValue.Accept (*this);
  }

  void PackageVisitor::Visit_AttributeValue(const AttributeValue& value)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("configProperty"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    Property ref = value.dstAttributeValue_end();
    ReadonlyAttribute attr = value.srcAttributeValue_end();
    ref.name() = attr.name();
    ref.Accept (*this);
    this->pop();
  }

  void PackageVisitor::Visit_AttributeDelegate(const AttributeDelegate&){}
  void PackageVisitor::Visit_AttributeMappingValue(const AttributeMappingValue&){}
  void PackageVisitor::Visit_AttributeMappingDelegate(const AttributeMappingDelegate&){}

  void PackageVisitor::CreateAssemblyConnections (vector<ComponentAssembly>& assemblies)
  {
    for (vector<ComponentAssembly>::iterator iter = assemblies.begin();
         iter != assemblies.end();
         ++iter)
      {
        ComponentAssembly subasm = *iter;
        const set<invoke> invokes = subasm.invoke_kind_children();
        for (set<invoke>::const_iterator iter = invokes.begin();
             iter != invokes.end();
             ++iter)
          {
            invoke iv = *iter;
            iv.Accept (*this);
          }
        const set<emit> emits = subasm.emit_kind_children();
        for (set<emit>::const_iterator iter = emits.begin();
             iter != emits.end();
             ++iter)
          {
            emit ev = *iter;
            ev.Accept (*this);
          }
        const set<publish> publishers = subasm.publish_kind_children();
        for (set<publish>::const_iterator iter = publishers.begin();
             iter != publishers.end();
             ++iter)
          {
            publish ev = *iter;
            ev.Accept (*this);
          }
        const set<deliverTo> deliverTos = subasm.deliverTo_kind_children();
        for (set<deliverTo>::const_iterator iter = deliverTos.begin();
             iter != deliverTos.end();
             ++iter)
          {
            deliverTo dv = *iter;
            dv.Accept (*this);
          }
        const set<PublishConnector>
          connectors = subasm.PublishConnector_kind_children();
        for (set<PublishConnector>::const_iterator iter = connectors.begin();
             iter != connectors.end();
             ++iter)
          {
            PublishConnector conn = *iter;
            conn.Accept (*this);
          }
        this->publishers_.clear();
        this->consumers_.clear();
      }


  }

  template <typename T, typename Del, typename DelRet, typename DelEndRet>
  void PackageVisitor::GetComponents (const T& port,
                                      DelRet (T::*srcDel)() const,
                                      DelRet (T::*dstDel) () const,
                                      DelEndRet (Del::*srcDelEnd)() const,
                                      DelEndRet (Del::*dstDelEnd)() const,
                                      map<Component, string>& output,
                                      set<T>& visited)
  {
    visited.insert (port);
    Udm::Object par = port.parent();
    string recepName = port.name();
    string parentName = this->ExtractName (par);
    if (Udm::IsDerivedFrom (par.type(), ComponentAssembly::meta))
      {
        set<Del> delegates = (port.*dstDel)();
        for (set<Del>::const_iterator iter = delegates.begin();
             iter != delegates.end();
             ++iter)
          {
            Del delegate = *iter;
            T srcPort = (delegate.*dstDelEnd)();
            string srcPortName = this->ExtractName(srcPort);
            if (find (visited.begin(),
                           visited.end(),
                           srcPort) == visited.end())
              this->GetComponents(srcPort, srcDel, dstDel,
                                  srcDelEnd, dstDelEnd, output, visited);
          }
        delegates = (port.*srcDel)();
        for (set<Del>::const_iterator iter = delegates.begin();
             iter != delegates.end();
             ++iter)
          {
            Del delegate = *iter;
            T dstPort = (delegate.*srcDelEnd)();
            string dstPortName = this->ExtractName(dstPort);
            if (find (visited.begin(),
                           visited.end(),
                           dstPort) == visited.end())
              this->GetComponents(dstPort, srcDel, dstDel,
                                  srcDelEnd, dstDelEnd, output, visited);
          }
      }
    else if (Udm::IsDerivedFrom (par.type(), Component::meta))
      {
        Component recep_comp = Component::Cast (par);
        output.insert (make_pair (recep_comp, port.name()));
      }
    visited.erase (port);
    return;
  }



  void PackageVisitor::GetReceptacleComponents (const RequiredRequestPort& receptacle,
                                                map<Component,string>& output)
  {
    set<RequiredRequestPort> visited;
    this->GetComponents (receptacle,
                         &RequiredRequestPort::srcReceptacleDelegate,
                         &RequiredRequestPort::dstReceptacleDelegate,
                         &ReceptacleDelegate::srcReceptacleDelegate_end,
                         &ReceptacleDelegate::dstReceptacleDelegate_end,
                         output,
                         visited);
  }

  void PackageVisitor::GetFacetComponents (const ProvidedRequestPort& facet,
                                           map<Component,string>& output)
  {
    set<ProvidedRequestPort> visited;
    this->GetComponents (facet,
                         &ProvidedRequestPort::srcFacetDelegate,
                         &ProvidedRequestPort::dstFacetDelegate,
                         &FacetDelegate::srcFacetDelegate_end,
                         &FacetDelegate::dstFacetDelegate_end,
                         output,
                         visited);
  }

  void PackageVisitor::GetEventSinkComponents (const InEventPort& consumer,
                                               map<Component,string>& output)
  {
    set<InEventPort> visited;
    this->GetComponents (consumer,
                         &InEventPort::srcEventSinkDelegate,
                         &InEventPort::dstEventSinkDelegate,
                         &EventSinkDelegate::srcEventSinkDelegate_end,
                         &EventSinkDelegate::dstEventSinkDelegate_end,
                         output,
                         visited);
  }

  void PackageVisitor::GetEventSourceComponents (const OutEventPort& publisher,
                                                 map<Component,string>& output)
  {
    set<OutEventPort> visited;
    this->GetComponents (publisher,
                         &OutEventPort::srcEventSourceDelegate,
                         &OutEventPort::dstEventSourceDelegate,
                         &EventSourceDelegate::srcEventSourceDelegate_end,
                         &EventSourceDelegate::dstEventSourceDelegate_end,
                         output,
                         visited);
  }

  void PackageVisitor::CreateConnections (const map<Component, string>& src,
                                          const map<Component, string>& dst)
  {
    for (map<Component,string>::const_iterator iter = src.begin();
         iter != src.end();
         ++iter)
      {
        Component srcComp = iter->first;
        string srcPortName = iter->second;
        for (map<Component, string>::const_iterator iter = dst.begin();
             iter != dst.end();
             ++iter)
          {
            Component dstComp = iter->first;
            string dstPortName = iter->second;
            this->CreateConnection (srcComp, srcPortName, dstComp, dstPortName);
          }
      }
  }

  void PackageVisitor::CreateConnection (const Component& srcComp,
                                         const string& srcPortName,
                                         const Component& dstComp,
                                         const string& dstPortName)
  {
    // Create a connection
    DOMElement* ele = this->doc_->createElement (XStr ("connection"));
    this->curr_->appendChild (ele);

    string connection = srcPortName + "_" + dstPortName;
    ele->appendChild (this->createSimpleContent ("name", connection));

    // Source endPoint
    DOMElement* endPoint
      = this->doc_->createElement (XStr ("internalEndpoint"));
    endPoint->appendChild (this->createSimpleContent ("portName",
                                                      srcPortName));
    // Source instance
    string uuid (srcComp.UUID());
    uuid = string ("_") + uuid;
    endPoint->appendChild (this->createSimpleContent ("instance", uuid));
    ele->appendChild (endPoint);

    // Destination endPoint
    endPoint = this->doc_->createElement (XStr ("internalEndpoint"));
    endPoint->appendChild (this->createSimpleContent ("portName",
                                                      dstPortName));
    // Destination instance
    uuid = dstComp.UUID();
    uuid = string ("_") + uuid;
    endPoint->appendChild (this->createSimpleContent ("instance", uuid));
    ele->appendChild (endPoint);
  }

  void PackageVisitor::Visit_invoke(const invoke& iv)
  {
    // Get the receptacle end
    RequiredRequestPort receptacle = iv.srcinvoke_end();

    // Get the facet end
    ProvidedRequestPort facet = iv.dstinvoke_end();

    map<Component,string> receptacles;
    map<Component,string> facets;
    this->GetReceptacleComponents (receptacle, receptacles);
    this->GetFacetComponents (facet, facets);
    this->CreateConnections (receptacles, facets);
  }

  void PackageVisitor::Visit_emit(const emit& ev)
  {
    // Get the emitter end
    OutEventPort emitter = ev.srcemit_end();

    // Get the consumer end
    InEventPort consumer = ev.dstemit_end();

    map<Component,string> emitters;
    map<Component,string> consumers;
    this->GetEventSourceComponents (emitter, emitters);
    this->GetEventSinkComponents (consumer, consumers);
    this->CreateConnections (emitters, consumers);
  }

  void PackageVisitor::Visit_publish(const publish& ev)
  {
    // Get the publisher end
    const OutEventPort publisher = ev.srcpublish_end();

    // Get the connector end
    const PublishConnector connector = ev.dstpublish_end();

    // Create an entry in the publishers_ map
    this->publishers_[string (connector.name())] = publisher;
  }

  void PackageVisitor::Visit_deliverTo(const deliverTo& dv)
  {
    // Get the connector end
    const  PublishConnector connector = dv.srcdeliverTo_end();

    // Get the consumer end
    const InEventPort consumer = dv.dstdeliverTo_end();

    // Create an entry in the consumers_ map
    this->consumers_.insert (make_pair (connector.name(), consumer));
  }

  void PackageVisitor::Visit_PublishConnector(const PublishConnector& pubctor)
  {
    string ctor = pubctor.name();

    // Get Publisher
    OutEventPort publisher = this->publishers_[ctor];
    map<Component,string> publishers;
    this->GetEventSourceComponents (publisher, publishers);

    for (map<Component,string>::const_iterator
           iter = publishers.begin();
         iter != publishers.end();
         ++iter)
      {
        Component srcComp = iter->first;
        string srcPortName = iter->second;

        for (multimap<string, InEventPort>::const_iterator
               iter = this->consumers_.lower_bound (ctor);
             iter != this->consumers_.upper_bound (ctor);
             ++iter)
          {
            // Get Consumer
            InEventPort consumer = iter->second;
            map<Component,string> consumers;
            this->GetEventSinkComponents (consumer, consumers);
            for (map<Component,string>::const_iterator
                   iter = consumers.begin();
                 iter != consumers.end();
                 ++iter)
              {
                Component dstComp = iter->first;
                string dstPortName = iter->second;
                this->CreateConnection (srcComp, srcPortName, dstComp,
                                        dstPortName);
              }
          }
      }
  }

  void PackageVisitor::Visit_MonolithExecParameter(const MonolithExecParameter&)
  {}

  void PackageVisitor::Visit_Requirement(const Requirement&)
  {}

  void PackageVisitor::Visit_SatisfierProperty(const SatisfierProperty&)
  {}

  void PackageVisitor::Visit_ImplementationDependency(const ImplementationDependency&)
  {}

  void PackageVisitor::Visit_Capability(const Capability&)
  {}

  void PackageVisitor::Visit_AssemblyselectRequirement(const AssemblyselectRequirement&)
  {}


  void PackageVisitor::Visit_AssemblyDeployRequirement(const AssemblyDeployRequirement&)
  {}


  void PackageVisitor::Visit_InfoProperty(const InfoProperty&)
  {}


  void PackageVisitor::Visit_MonolithDeployRequirement(const MonolithDeployRequirement&)
  {}


  void PackageVisitor::Visit_ImplementationDependsOn(const ImplementationDependsOn&)
  {}


  void PackageVisitor::Visit_ImplementationCapability(const ImplementationCapability&)
  {}

  string PackageVisitor::ExtractName(Udm::Object ob)
  {
    Uml::Class cls= ob.type();
    set<Uml::Attribute> attrs=cls.attributes();

    // Adding parent attributes
    set<Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
    attrs.insert(aattrs.begin(),aattrs.end());

    for(set<Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++)
      {
        if(string(ai->type())=="String")
          {
            string str=ai->name();
            if(str=="name")
              {
                string value=ob.getStringAttr(*ai);
                if(value.empty())value="<empty string>";
                return value;
              }
          }
      }
    return string("<no name specified>");
  }
}
