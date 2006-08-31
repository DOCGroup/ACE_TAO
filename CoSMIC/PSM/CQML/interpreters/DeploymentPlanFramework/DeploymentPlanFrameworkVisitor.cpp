#include <algorithm>
#include <functional>
#include <sstream>
#include <fstream>
#include "DeploymentPlanFramework/DeploymentPlanFrameworkVisitor.h"
#include "UmlExt.h"

using xercesc::LocalFileFormatTarget;
using xercesc::DOMImplementationRegistry;
using xercesc::DOMImplementationLS;
using xercesc::DOMException;
using xercesc::XMLUni;
using xercesc::XMLException;
using xercesc::DOMText;

#include "Utils/Utils.h"
using Utils::XStr;
using Utils::CreateUuid;

namespace CQML
{
  DeploymentPlanFrameworkVisitor::DeploymentPlanFrameworkVisitor (const std::string& outputPath)
    : impl_ (0), doc_ (0), root_ (0), curr_ (0), serializer_ (0), target_ (0),
      outputPath_ (outputPath)
  {
    this->init();
    this->injectors_.clear ();
  }

  DeploymentPlanFrameworkVisitor::~DeploymentPlanFrameworkVisitor ()
  {
    if (this->doc_)
      {
        this->doc_->release();
      }

    delete this->target_;
  }

  void DeploymentPlanFrameworkVisitor::init()
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

  void DeploymentPlanFrameworkVisitor::initTarget (const std::string& fileName)
  {
    if (this->target_)
      {
        delete this->target_;
      }

    this->target_ = new LocalFileFormatTarget (fileName.c_str());
  }

  void DeploymentPlanFrameworkVisitor::initNodeRefName (const std::string& nodeRefName)
  {
    this->node_ref_name_ = nodeRefName;
  }

  std::string DeploymentPlanFrameworkVisitor::retNodeRefName ()
  {
    return this->node_ref_name_;
  }

  void DeploymentPlanFrameworkVisitor::initcgName (const std::string& cgName)
  {
    this->cg_name_ = cgName;
  }

  std::string DeploymentPlanFrameworkVisitor::retcgName ()
  {
    return this->cg_name_;
  }

  void DeploymentPlanFrameworkVisitor::initDocument (const std::string& rootName)
  {
    if (this->doc_)
      {
        this->doc_->release();
      }
    // Create the document
    this->doc_ =
      this->impl_->createDocument (XStr ("http://www.omg.org/Deployment"),
                                   XStr (rootName.c_str()),
                                   0);
  }

  void DeploymentPlanFrameworkVisitor::initRootAttributes()
  {
    this->doc_->setEncoding (XStr("UTF-8"));
    this->doc_->setVersion (XStr("1.0"));
    this->root_ = this->doc_->getDocumentElement();
    this->root_->setAttributeNS (XStr ("http://www.w3.org/2000/xmlns/"),
                                 XStr ("xmlns:Deployment"),
                                 XStr ("http://www.omg.org/Deployment"));
    this->root_->setAttributeNS (XStr ("http://www.w3.org/2000/xmlns/"),
                                 XStr ("xmlns:xsi"),
                                 XStr 
                                 ("http://www.w3.org/2001/XMLSchema-instance"));
    this->root_->setAttributeNS (XStr ("http://www.w3.org/2000/xmlns/"),
                                 XStr ("xmlns:xmi"),
                                 XStr ("http://www.omg.org/XMI"));
    this->root_->setAttribute (XStr ("xsi:schemaLocation"),
                               XStr 
                              ("http://www.omg.org/Deployment Deployment.xsd"));
    this->curr_ = this->root_;
  }

  void DeploymentPlanFrameworkVisitor::dumpDocument()
  {
    this->serializer_->writeNode (this->target_, *this->doc_);
  }

  void DeploymentPlanFrameworkVisitor::push()
  {
    this->curr_stack_.push (this->curr_);
  }

  void DeploymentPlanFrameworkVisitor::pop()
  {
    if (!this->curr_stack_.empty())
      {
        this->curr_ = this->curr_stack_.top();
        this->curr_stack_.pop();
      }
    else
      {
        throw(std::exception());
      }
  }

  DOMElement* DeploymentPlanFrameworkVisitor::createSimpleContent (const std::string& name,
                                                                   const std::string& value)
  {
    DOMElement* pName = this->doc_->createElement (XStr (name.c_str()));
    DOMText* pValue = this->doc_->createTextNode (XStr (value.c_str()));
    pName->appendChild (pValue);
    return pName;
  }

  void DeploymentPlanFrameworkVisitor::Visit_RootFolder(const RootFolder& rf)
  {
    this->root_folder_ = rf;
    std::set<DeploymentPlans> folders = rf.DeploymentPlans_kind_children();
    for (std::set<DeploymentPlans>::iterator iter = folders.begin();
         iter != folders.end();
         ++iter)
      {
        DeploymentPlans folder = *iter;
        folder.Accept (*this);
      }
  }

  // Predefined Types
  void DeploymentPlanFrameworkVisitor::Visit_LongInteger(const LongInteger&)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_long"));
    this->pop();
  }

  void DeploymentPlanFrameworkVisitor::Visit_RealNumber(const RealNumber& real)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_double"));
    this->pop();
  }

  void DeploymentPlanFrameworkVisitor::Visit_Boolean(const Boolean&)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_boolean"));
    this->pop();
  }

  void DeploymentPlanFrameworkVisitor::Visit_ShortInteger(const ShortInteger&)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_short"));
    this->pop();
  }

  // Implementation Artifact operations

  void DeploymentPlanFrameworkVisitor::Visit_ImplementationArtifacts (
                        const ImplementationArtifacts& ia)
  {
    std::set<ArtifactContainer> afs = ia.ArtifactContainer_kind_children();
    for (std::set<ArtifactContainer>::iterator iter = afs.begin();
         iter != afs.end();
         ++iter)
      {
        ArtifactContainer ac = *iter;
        ac.Accept (*this);
      }
  }

  void DeploymentPlanFrameworkVisitor::Visit_ArtifactContainer(const ArtifactContainer& ac)
  {
    const std::set<ImplementationArtifact>
      ias = ac.ImplementationArtifact_kind_children();
    for (std::set<ImplementationArtifact>::const_iterator iter = ias.begin();
         iter != ias.end();
         ++iter)
      {
        ImplementationArtifact ia = *iter;
        ia.Accept (*this);
      }
  }

  void DeploymentPlanFrameworkVisitor::generate_infoproperties (const DeploymentPlan &plan)
    {
      DOMElement* ele = this->doc_->createElement (XStr ("infoProperty"));
      this->curr_->appendChild (ele);
      this->push();
      this->curr_ = ele;

      std::set <Property> info_properties = plan.Property_kind_children ();
      for (std::set <Property>::const_iterator iter = info_properties.begin ();
           iter != info_properties.end ();
           ++iter)
        {
          Property property = *iter;
          property.Accept (*this);
        }
      this->pop ();
    }

  void DeploymentPlanFrameworkVisitor::Visit_ImplementationArtifact (
                   const ImplementationArtifact& ia)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("artifact"));

    std::string artifactName = ia.getPath (".",false,true,"name",true);
    std::string uniqueName = ia.UUID();
    if (uniqueName.empty())
      {
        ia.UUID() = uniqueName = ::Utils::CreateUuid();
      }
    uniqueName = std::string ("_") + uniqueName;

    ele->setAttribute (XStr ("id"), XStr (uniqueName));
    ele->appendChild (this->createSimpleContent ("name", artifactName));
    //ele->appendChild (this->createSimpleContent ("node", "<!-- empty-->"));
    ele->appendChild (this->doc_->createElement (XStr ("source")));
    ele->appendChild (this->doc_->createElement (XStr ("node")));
    std::string location = ia.location();
    if (!location.empty())
      {
        ele->appendChild (this->createSimpleContent ("location",
                                                     location));
      }
    else
      {
        ele->appendChild (this->doc_->createElement (XStr ("location")));
      }
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    const std::set<ArtifactExecParameter> exec = ia.dstArtifactExecParameter();
    for (std::set<ArtifactExecParameter>::const_iterator it = exec.begin();
         it != exec.end();
         ++it)
      {
        ArtifactExecParameter aep = *it;
        aep.Accept (*this);
      }
    this->pop();
  }

  void DeploymentPlanFrameworkVisitor::Visit_ArtifactDependsOn(const ArtifactDependsOn& ado)
  {
    ImplementationArtifactReference ref = ado.dstArtifactDependsOn_end();
    ref.Accept (*this);
  }

  void DeploymentPlanFrameworkVisitor::Visit_ImplementationArtifactReference (
                   const ImplementationArtifactReference& iar)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("dependsOn"));
    ele->appendChild (this->createSimpleContent ("name", iar.name()));
    const ImplementationArtifact ref = iar.ref();
    std::string refName (ref.name());
    refName += ".iad";
    DOMElement* refEle = this->doc_->createElement (XStr ("referencedArtifact"));
    refEle->setAttribute (XStr ("href"), XStr (refName));
    ele->appendChild (refEle);
    this->curr_->appendChild (ele);
    this->pop();
  }

  void DeploymentPlanFrameworkVisitor::Visit_ArtifactExecParameter (
                    const ArtifactExecParameter& param)
  {
    this->push();
    DOMElement* value = this->doc_->createElement (XStr ("execParameter"));
    this->curr_->appendChild (value);
    this->curr_ = value;
    Property ref = param.dstArtifactExecParameter_end();
    ref.Accept (*this);
    this->pop();
  }

  void DeploymentPlanFrameworkVisitor::Visit_Property(const Property& property)
  {
    this->CreatePropertyElement (property.name(), property);
  }

  void DeploymentPlanFrameworkVisitor::CreatePropertyElement (std::string name, 
                                               const Property& property)
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
    std::string refName = ref.name();
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

  void DeploymentPlanFrameworkVisitor::Visit_DataType (const DataType& type)
  {
    PredefinedType ref = type.ref();
    std::string kindName = ref.name();
    if (kindName == "Boolean")
      {
        Boolean boolv = CQML::Boolean::Cast (ref);
        boolv.Accept (*this);
      }
    else if (kindName == "Byte")
      {
        Byte byte = CQML::Byte::Cast (ref);
        byte.Accept (*this);
      }
    else if (kindName == "String")
      {
        String str = CQML::String::Cast (ref);
        str.Accept (*this);
      }
    else if (kindName == "RealNumber")
      {
        RealNumber real = CQML::RealNumber::Cast (ref);
        real.Accept (*this);
      }
    else if (kindName == "ShortInteger")
      {
        ShortInteger shortv = CQML::ShortInteger::Cast (ref);
        shortv.Accept (*this);
      }
    else if (kindName == "LongInteger")
      {
        LongInteger lint = CQML::LongInteger::Cast (ref);
        lint.Accept (*this);
      }
  }

  void DeploymentPlanFrameworkVisitor::Visit_String(const String& str)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_string"));
    this->pop();
  }

  void DeploymentPlanFrameworkVisitor::Visit_Byte(const Byte& byte)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_octet"));
    this->pop();
  }

  void DeploymentPlanFrameworkVisitor::Visit_ComponentImplementations (
                   const ComponentImplementations& cimpls)
  {
    std::set<ComponentImplementationContainer>
      cics = cimpls.ComponentImplementationContainer_kind_children();
    for (std::set<ComponentImplementationContainer>::iterator
           iter = cics.begin();
         iter != cics.end();
         ++iter)
      {
        ComponentImplementationContainer cc = *iter;
        cc.Accept (*this);
      }
  }

  void DeploymentPlanFrameworkVisitor::Visit_ComponentImplementationContainer (
                    const ComponentImplementationContainer& cic)
  {
    std::set<MonolithicImplementation>
      mimpls = cic.MonolithicImplementation_kind_children();
    for (std::set<MonolithicImplementation>::iterator iter = mimpls.begin();
         iter != mimpls.end();
         ++iter)
      {
        MonolithicImplementation mimpl = *iter;
        mimpl.Accept (*this);
      }
  }

  void DeploymentPlanFrameworkVisitor::Visit_MonolithicImplementation
                   (const MonolithicImplementation& mimpl)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("implementation"));

    Implements iface = mimpl.dstImplements();
    const ComponentRef iface_ref = iface.dstImplements_end();
    const Component comp_ref = iface_ref.ref();
    std::string refName (comp_ref.name());

    // std::string impl_name = mimpl.name ();
    this->monoimpls_.insert (make_pair (refName, mimpl));

    std::string implName = mimpl.getPath (".",false,true,"name",true);
    std::string uniqueName = mimpl.UUID();
    if (uniqueName.empty())
      {
        mimpl.UUID() = uniqueName = ::Utils::CreateUuid();
      }
    uniqueName = std::string ("_") + uniqueName;

    ele->setAttribute (XStr ("id"), XStr (uniqueName));
    ele->appendChild (this->createSimpleContent ("name", implName));
    ele->appendChild (this->doc_->createElement (XStr ("source")));
    this->curr_->appendChild (ele);
    this->curr_ = ele;

    const std::set<MonolithprimaryArtifact>
      mpas = mimpl.dstMonolithprimaryArtifact();
    for (std::set<MonolithprimaryArtifact>::const_iterator it = mpas.begin();
         it != mpas.end();
         ++it)
      {
        MonolithprimaryArtifact mpa = *it;
        mpa.Accept (*this);
      }

    /*const std::set<ConfigProperty>
    cps = mimpl.dstConfigProperty();
    for (std::set<ConfigProperty>::const_iterator it2 = cps.begin();
    it2 != cps.end();
    ++it2)
    {
    ConfigProperty cp = *it2;
    cp.Accept (*this);
    }*/

	  const std::set<MonolithExecParameter>
      mexecs = mimpl.dstMonolithExecParameter();
    for (std::set<MonolithExecParameter>::const_iterator it2 = mexecs.begin();
         it2 != mexecs.end();++it2)
      {
        MonolithExecParameter mexec = *it2;
        mexec.Accept (*this);
      }

    this->pop();
  }

  void DeploymentPlanFrameworkVisitor::Visit_MonolithExecParameter(const MonolithExecParameter& mexec)
  {
	  this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("execParameter"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    Property ref = mexec.dstMonolithExecParameter_end();
    ref.Accept (*this);
    this->pop();
  }

  void DeploymentPlanFrameworkVisitor::Visit_Implements(const Implements& impl)
  {
    this->push();
    const ComponentRef iface = impl.dstImplements_end();
    const Component ref = iface.ref();
    std::string refName (ref.name());
    refName += ".ccd";
    DOMElement* refEle = this->doc_->createElement (XStr ("implements"));
    refEle->setAttribute (XStr ("href"), XStr (refName));
    this->curr_->appendChild (refEle);
    this->pop();
  }

  void DeploymentPlanFrameworkVisitor::Visit_MonolithprimaryArtifact
        (const MonolithprimaryArtifact& mpa)
  {
    this->push();
    const ImplementationArtifactReference iaref = 
      mpa.dstMonolithprimaryArtifact_end();
    const ImplementationArtifact ref = iaref.ref();

    // std::string uniqueName = ref.getPath ("_",false,true,"name",true);
    std::string uniqueName = ref.UUID();
    if (uniqueName.empty())
      {
        ref.UUID() = uniqueName = ::Utils::CreateUuid();
      }
    uniqueName = std::string ("_") + uniqueName;

    this->curr_->appendChild 
        (this->createSimpleContent ("artifact", uniqueName));
    this->pop();
  }

  void DeploymentPlanFrameworkVisitor::Visit_ConfigProperty(const ConfigProperty& cp)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("configProperty"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    Property ref = cp.dstConfigProperty_end();
    ref.Accept (*this);
    this->pop();
  }

  void DeploymentPlanFrameworkVisitor::Visit_AssemblyConfigProperty
          (const AssemblyConfigProperty& acp)
  {
    this->push();
    DOMElement* value = this->doc_->createElement (XStr ("configProperty"));
    this->curr_->appendChild (value);
    this->curr_ = value;
    Property ref = acp.dstAssemblyConfigProperty_end();
    ref.Accept (*this);
    this->pop();
  }

  template <typename T, typename Del, typename DelRet, typename DelEndRet>
  void DeploymentPlanFrameworkVisitor::GetComponents (const T& port,
                                      DelRet (T::*srcDel)() const,
                                      DelRet (T::*dstDel) () const,
                                      DelEndRet (Del::*srcDelEnd)() const,
                                      DelEndRet (Del::*dstDelEnd)() const,
                                      std::map<Component, std::string>& output,
                                      std::set<T>& visited)
  {
    visited.insert (port);
    Udm::Object par = port.parent();
    std::string recepName = port.name();
    std::string parentName = this->ExtractName (par);
    if (Udm::IsDerivedFrom (par.type(), ComponentAssembly::meta))
      {
        std::set<Del> delegates = (port.*dstDel)();
        for (std::set<Del>::const_iterator iter = delegates.begin();
             iter != delegates.end();
             ++iter)
          {
            Del delegate = *iter;
            T srcPort = (delegate.*dstDelEnd)();
            std::string srcPortName = this->ExtractName(srcPort);
            if (std::find (visited.begin(),
                           visited.end(),
                           srcPort) == visited.end())
              {
                this->GetComponents(srcPort, srcDel, dstDel,
                                    srcDelEnd, dstDelEnd, output, visited);
              }
          }
        delegates = (port.*srcDel)();
        for (std::set<Del>::const_iterator iter = delegates.begin();
             iter != delegates.end();
             ++iter)
          {
            Del delegate = *iter;
            T dstPort = (delegate.*srcDelEnd)();
            std::string dstPortName = this->ExtractName(dstPort);
            if (std::find (visited.begin(),
                           visited.end(),
                           dstPort) == visited.end())
              {
                this->GetComponents(dstPort, srcDel, dstDel,
                                    srcDelEnd, dstDelEnd, output, visited);
              }
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

  void DeploymentPlanFrameworkVisitor::GetReceptacleComponents 
      (const RequiredRequestPort& receptacle,
       std::map<Component,std::string>& output)
  {
    std::set<RequiredRequestPort> visited;
    this->GetComponents (receptacle,
                        &RequiredRequestPort::srcReceptacleDelegate,
                        &RequiredRequestPort::dstReceptacleDelegate,
                        &ReceptacleDelegate::srcReceptacleDelegate_end,
                        &ReceptacleDelegate::dstReceptacleDelegate_end,
                        output,
                        visited);
  }

  void DeploymentPlanFrameworkVisitor::GetFacetComponents (const ProvidedRequestPort& facet,
                                            std::map<Component,
                                            std::string>& output)
  {
    std::set<ProvidedRequestPort> visited;
    this->GetComponents (facet,
                         &ProvidedRequestPort::srcFacetDelegate,
                         &ProvidedRequestPort::dstFacetDelegate,
                         &FacetDelegate::srcFacetDelegate_end,
                         &FacetDelegate::dstFacetDelegate_end,
                         output,
                         visited);
  }

  void DeploymentPlanFrameworkVisitor::GetEventSinkComponents (const InEventPort& consumer,
                                                std::map<Component,
                                                std::string>& output)
  {
    std::set<InEventPort> visited;
    this->GetComponents (consumer,
                         &InEventPort::srcEventSinkDelegate,
                         &InEventPort::dstEventSinkDelegate,
                         &EventSinkDelegate::srcEventSinkDelegate_end,
                         &EventSinkDelegate::dstEventSinkDelegate_end,
                         output,
                         visited);
  }

    void DeploymentPlanFrameworkVisitor::GetEventSourceComponents 
       (const OutEventPort& publisher,
        std::map<Component,std::string>& output)
  {
    std::set<OutEventPort> visited;
    this->GetComponents (publisher,
                         &OutEventPort::srcEventSourceDelegate,
                         &OutEventPort::dstEventSourceDelegate,
                         &EventSourceDelegate::srcEventSourceDelegate_end,
                         &EventSourceDelegate::dstEventSourceDelegate_end,
                         output,
                         visited);
  }

  void DeploymentPlanFrameworkVisitor::CreateConnections (const std::map<Component, 
    std::string>& src, const std::map<Component, std::string>& dst,
    const std::string& source_kind, const std::string& dest_kind)
  {
    for (std::map<Component,std::string>::const_iterator iter = src.begin();
         iter != src.end();
         ++iter)
      {
        Component srcComp = iter->first;
        std::string srcPortName = iter->second;
        for (std::map<Component, 
             std::string>::const_iterator iter = dst.begin();
             iter != dst.end();
             ++iter)
          {
            Component dstComp = iter->first;
            std::string dstPortName = iter->second;
            this->CreateConnection (srcComp, srcPortName, dstComp, 
                    dstPortName, source_kind, dest_kind);
          }
      }
  }

  void DeploymentPlanFrameworkVisitor::CreateConnection (const Component& srcComp,
     const std::string& srcPortName, const Component& dstComp,
     const std::string& dstPortName, const std::string& source_kind,
		 const std::string& dest_kind)
  {
    CQML::InstanceConnection source_connection;
    CQML::InstanceConnection dest_connection;

    std::string source_comp_instance_path = 
        srcComp.getPath (".",false,true,"name",true);
    std::string source_comp_instance = srcComp.UUID();

    if (source_comp_instance.empty())
      {
        srcComp.UUID() = source_comp_instance = ::Utils::CreateUuid();
      }
    source_comp_instance = std::string ("_") + source_comp_instance;

    std::string dest_comp_instance_path = 
         dstComp.getPath (".",false,true,"name",true);
    std::string dest_comp_instance = dstComp.UUID();

    if (dest_comp_instance.empty())
      {
        dstComp.UUID() = dest_comp_instance = ::Utils::CreateUuid();
      }
    dest_comp_instance = std::string ("_") + dest_comp_instance;

    if (this->selected_instances_.find (source_comp_instance_path)
		    != this->selected_instances_.end ())
      {
        if (this->selected_instances_.find (dest_comp_instance_path)
			      != this->selected_instances_.end ())
          {
            // Create a connection
            DOMElement* ele = this->doc_->createElement (XStr ("connection"));

            this->curr_->appendChild (ele);

            std::string connection = srcPortName + "_" + dstPortName + "_" + source_comp_instance + "_" + dest_comp_instance;;
            ele->appendChild (this->createSimpleContent ("name", connection));

            // Source endPoint
            DOMElement* endPoint
               = this->doc_->createElement (XStr ("internalEndpoint"));
            endPoint->appendChild (this->createSimpleContent ("portName",
                                   srcPortName));
            endPoint->appendChild (this->createSimpleContent ("kind",
                                   dest_kind));
		    
            // std::string source_comp_instance = 
            //     srcComp.getPath ("_",false,true,"name",true);
            std::string source_instance_id = source_comp_instance_path;
            endPoint->appendChild (this->createSimpleContent ("instance", 
                                   source_instance_id));
            ele->appendChild (endPoint);

            source_connection.instance_name = source_comp_instance_path;
            source_connection.port_name = srcPortName;
            source_connection.port_kind = dest_kind;

            // Destination endPoint
            endPoint = this->doc_->createElement (XStr ("internalEndpoint"));
            endPoint->appendChild (this->createSimpleContent ("portName",
                                   dstPortName));
            endPoint->appendChild (this->createSimpleContent ("kind",
                                   source_kind));
		    
            // std::string dest_comp_instance = 
            //      dstComp.getPath ("_",false,true,"name",true);
            std::string dest_instance_id = dest_comp_instance_path;
            endPoint->appendChild (this->createSimpleContent ("instance", 
                                   dest_instance_id));
            ele->appendChild (endPoint);

            dest_connection.instance_name = dest_comp_instance_path;
            dest_connection.port_name = dstPortName;
            dest_connection.port_kind = source_kind;

            std::pair <CQML::InstanceConnection, CQML::InstanceConnection > 
              connection_pair (make_pair (source_connection, dest_connection));

            std::string connection_instance = source_comp_instance_path + "_";
            connection_instance = connection_instance + dest_comp_instance_path;

            this->connections_. insert (make_pair (connection_instance, connection_pair));

            // this->connections_.insert (make_pair (source_connection, dest_connection));
          }
      }
  }

  std::string DeploymentPlanFrameworkVisitor::ExtractName(Udm::Object ob)
  {
    Uml::Class cls= ob.type();
    set<Uml::Attribute> attrs=cls.attributes();

    // Adding parent attributes
    set<Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
    attrs.insert(aattrs.begin(),aattrs.end());

    for(set<Uml::Attribute>::iterator ai = attrs.begin();
        ai != attrs.end(); ai++)
      {
        if(string(ai->type())=="String")
          {
            string str=ai->name();
            if(str=="name")
              {
                string value=ob.getStringAttr(*ai);
                if(value.empty())
                  {
                    value="<empty string>";
                  }
                return value;
              }
          }
      }
    return string("<no name specified>");
  }

  void DeploymentPlanFrameworkVisitor::Visit_invoke(const invoke& iv)
  {

    // Get the receptacle end
    RequiredRequestPort receptacle = iv.srcinvoke_end();

    // Get the facet end
    ProvidedRequestPort facet = iv.dstinvoke_end();

    std::map<Component,std::string> receptacles;
    std::map<Component,std::string> facets;
    std::string source_kind = "Facet";
    std::string dest_kind = "SimplexReceptacle";
    this->GetReceptacleComponents (receptacle, receptacles);
    this->GetFacetComponents (facet, facets);
    this->CreateConnections (receptacles, facets, source_kind, dest_kind);
  }

  void DeploymentPlanFrameworkVisitor::Visit_emit(const emit& ev)
  {
    // Get the emitter end
    OutEventPort emitter = ev.srcemit_end();

    // Get the consumer end
    InEventPort consumer = ev.dstemit_end();

    std::map<Component,std::string> emitters;
    std::map<Component,std::string> consumers;
    std::string source_kind = "EventConsumer";
    std::string dest_kind = "EventEmitter";
    this->GetEventSourceComponents (emitter, emitters);
    this->GetEventSinkComponents (consumer, consumers);
    this->CreateConnections (emitters, consumers, source_kind, dest_kind);
  }

  void DeploymentPlanFrameworkVisitor::Visit_publish(const publish& ev)
  {
    // Get the publisher end
    const OutEventPort publisher = ev.srcpublish_end();

    // Get the connector end
    const PublishConnector connector = ev.dstpublish_end();

    // Create an entry in the publishers_ map
    this->publishers_[std::string (connector.name())] = publisher;
  }

  void DeploymentPlanFrameworkVisitor::Visit_deliverTo(const deliverTo& dv)
  {
    // Get the connector end
    const  PublishConnector connector = dv.srcdeliverTo_end();

    // Get the consumer end
    const InEventPort consumer = dv.dstdeliverTo_end();

    // Create an entry in the consumers_ map
    this->consumers_.insert (make_pair (std::string (connector.name()),
                                        consumer));
  }

  void DeploymentPlanFrameworkVisitor::Visit_PublishConnector(const PublishConnector& pubctor)
  {
    std::string ctor = pubctor.name();

    // Get Publisher
    OutEventPort publisher = this->publishers_[ctor];
    std::map<Component,std::string> publishers;
    this->GetEventSourceComponents (publisher, publishers);

    for (std::map<Component,std::string>::const_iterator
           iter = publishers.begin();
         iter != publishers.end();
         ++iter)
      {
        Component srcComp = iter->first;
        std::string srcPortName = iter->second;

        for (std::multimap<std::string, InEventPort>::const_iterator
             iter = this->consumers_.lower_bound (ctor);
             iter != this->consumers_.upper_bound (ctor);
             ++iter)
          {
            // Get Consumer
            InEventPort consumer = iter->second;
            std::map<Component,std::string> consumers;
            this->GetEventSinkComponents (consumer, consumers);
            for (std::map<Component,std::string>::const_iterator
                 iter = consumers.begin();
                 iter != consumers.end();
                 ++iter)
              {
                Component dstComp = iter->first;
                std::string dstPortName = iter->second;
                std::string source_kind = "EventConsumer";
                std::string dest_kind = "EventPublisher";
                this->CreateConnection (srcComp, srcPortName, dstComp,
                                        dstPortName, source_kind, dest_kind);
              }
          }
      }
  }

  void DeploymentPlanFrameworkVisitor::Visit_ArtifactDeployRequirement
        (const ArtifactDeployRequirement&)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_ArtifactInfoProperty(const ArtifactInfoProperty&)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_ImplementationRequirement
        (const ImplementationRequirement&)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_TopLevelPackages(const TopLevelPackages& tp)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_TopLevelPackageContainer(const TopLevelPackageContainer& tpc)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_TopLevelPackage(const TopLevelPackage& tp)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_package(const package& pkg)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_PackageConfigurationReference
           (const PackageConfigurationReference& pcr)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_PackageConfigurations
        (const PackageConfigurations& pcs)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_PackageConfigurationContainer
            (const PackageConfigurationContainer& pcc)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_PackageConfiguration
          (const PackageConfiguration& pc)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_PackageConfConfigProperty
           (const PackageConfConfigProperty&)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_PackageConfReference(const PackageConfReference&)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_PackageConfSpecializedConfig
           (const PackageConfSpecializedConfig&)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_PackageConfSelectRequirement
          (const PackageConfSelectRequirement&)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_PackageConfBasePackage
          (const PackageConfBasePackage&)
  {}

  void DeploymentPlanFrameworkVisitor::Visit_ComponentPackageReference
          (const ComponentPackageReference&)
  {}

  void DeploymentPlanFrameworkVisitor::add_injector (const std::string& plan_name, Injector* injector)
  {
    this->injectors_.insert (make_pair (plan_name, injector));
  }

  void DeploymentPlanFrameworkVisitor::Visit_DeploymentPlans(const DeploymentPlans& dps)
  {
    std::set<DeploymentPlan> plans = dps.DeploymentPlan_kind_children();

    for (std::set<DeploymentPlan>::iterator iter = plans.begin();
         iter != plans.end();
         ++iter)
      {
        this->clear_private_variables ();

        DeploymentPlan plan = *iter;
        std::string plan_name = plan.name ();
		    plan.Accept (*this);

		    Injector* injector;
		    std::map<std::string, CQML::Component> monolith_instances;
        std::map<std::string, CQML::Component> assembly_instances;
        std::map<std::string, std::string> node_mappings;
        std::map<std::string, std::string> node_initial_mappings;
        std::set<std::string> selected_failover_units;
        std::map<std::string, std::string> deployed_instances;

		    this->instantiate_deployment_plan_descriptor (plan);
		    this->create_label_and_uuid (plan);
		    this->generate_implementation_descriptions ();
        deployed_instances = this->deployed_instances_;
    		
		    if (this->injectors_.find (plan_name) != this->injectors_.end ())
		      {
		        injector = this->injectors_[plan_name];

	          monolith_instances = injector->add_monolith_instances (plan_name);
            this->instance_monolith_components_.insert 
              (monolith_instances.begin (), monolith_instances.end ());
            assembly_instances = injector->add_assembly_instances (plan_name);
            this->instance_assembly_components_.insert 
               (assembly_instances.begin (), assembly_instances.end ());
            node_mappings = injector->assign_node_mappings (plan_name, deployed_instances);
            for (std::map<std::string, std::string>::const_iterator node_mapping_iter =
                 node_mappings.begin (); node_mapping_iter != node_mappings.end ();
                 ++node_mapping_iter)
              {
                std::string comp_instance_name = node_mapping_iter->first;
                std::string node_name = node_mapping_iter->second;
                this->update_component_instance (comp_instance_name, node_name);
              }
		      }

		    this->generate_instance_deployment_descriptions ();
		    this->generate_assembly_instance_deployment_descriptions ();
		    this->generate_parent_connections ();
		    this->generate_child_connections ();

        Injector::ConnectionMap replica_connections;

        if (this->injectors_.find (plan_name) != this->injectors_.end ())
          {
            injector = this->injectors_[plan_name];
            replica_connections = injector->add_connections (plan_name, this->connections_);
          }

        for (Injector::ConnectionMap::const_iterator itr = replica_connections.begin ();
             itr != replica_connections.end ();
             ++itr)
          {
            const std::string &source_comp_name = itr->second.first.instance_name;
            const std::string &source_port_name = itr->second.first.port_name;
            const std::string &source_port_kind = itr->second.first.port_kind;
            std::string source_instance_id = std::string ("_") + source_comp_name;

            const std::string &dest_comp_name = itr->second.second.instance_name;
            const std::string &dest_port_name = itr->second.second.port_name;
            const std::string &dest_port_kind = itr->second.second.port_kind;
            std::string dest_instance_id = std::string ("_") + dest_comp_name;

            DOMElement* ele = this->doc_->createElement (XStr ("connection"));
            this->curr_->appendChild (ele);

            std::string connection = source_port_name + "_" + dest_port_name;
            ele->appendChild (this->createSimpleContent ("name", connection));

            // Source endPoint
            DOMElement* endPoint
               = this->doc_->createElement (XStr ("internalEndpoint"));
            endPoint->appendChild (this->createSimpleContent ("portName",
                                   source_port_name));
            endPoint->appendChild (this->createSimpleContent ("kind",
                                   source_port_kind));
            endPoint->appendChild (this->createSimpleContent ("instance", 
                                   source_instance_id));
            ele->appendChild (endPoint);

            // Destination endPoint
            endPoint = this->doc_->createElement (XStr ("internalEndpoint"));
            endPoint->appendChild (this->createSimpleContent ("portName",
                                   dest_port_name));
            endPoint->appendChild (this->createSimpleContent ("kind",
                                   dest_port_kind));   
            endPoint->appendChild (this->createSimpleContent ("instance", 
                                   dest_instance_id));
            ele->appendChild (endPoint);
          }

		    this->generate_artifact_descriptions ();
        this->generate_infoproperties(plan);
		    this->finalize_deployment_plan_descriptor ();

        this->clear_private_variables ();
      }
  }

  void DeploymentPlanFrameworkVisitor::clear_private_variables (void)
  {
    this->selected_instances_.clear ();
	  this->path_parents_.clear ();
	  this->containing_assemblies_.clear ();
	  this->assembly_components_.clear ();
    this->monoimpls_.clear ();
	  this->deployed_instances_.clear ();
	  this->monolith_components_.clear ();
	  this->final_assembly_components_.clear ();
	  this->publishers_.clear ();
	  this->consumers_.clear ();
	  this->interfaces_.clear ();
	  this->attrValues_.clear ();
    this->instance_monolith_components_.clear ();
    this->instance_assembly_components_.clear ();
    this->connections_.clear ();
  }

  void DeploymentPlanFrameworkVisitor::Visit_DeploymentPlan(const DeploymentPlan& dp)
  {
    const std::set<CollocationGroup> dps = dp.CollocationGroup_children();

    for (std::set<CollocationGroup>::const_iterator iter = dps.begin();
         iter != dps.end();
         ++iter)
      {
        std::string nodeRefName;
        std::string cgName;
        CollocationGroup cg = *iter;
        cgName = cg.name();
        this->initcgName (cgName);

        const std::set<InstanceMapping> cg_ins_maps = cg.dstInstanceMapping ();

        for (std::set<InstanceMapping>::const_iterator
             cg_ins_map_iter = cg_ins_maps.begin();
             cg_ins_map_iter != cg_ins_maps.end ();
             ++cg_ins_map_iter)
          {
            InstanceMapping cg_ins = *cg_ins_map_iter;
            NodeReference node_ref = cg_ins.dstInstanceMapping_end();
            const Node ref = node_ref.ref();
            nodeRefName = ref.name();
            this->initNodeRefName (nodeRefName);
          }

        std::set<CollocationGroup_Members_Base> comp_types = cg.members ();
        for (std::set<CollocationGroup_Members_Base>::const_iterator
             comp_type_iter = comp_types.begin();
             comp_type_iter != comp_types.end (); ++comp_type_iter)
          {
            CollocationGroup_Members_Base comp_type = *comp_type_iter;
            if (Udm::IsDerivedFrom (comp_type.type(), ComponentRef::meta))
              {
                ComponentRef component_ref = ComponentRef::Cast (comp_type);
				        std::string comp_ref_name = component_ref.name ();
                Component comp = component_ref.ref();
				        this->monolith_components_.insert (comp);
				        std::string comp_name = comp.name ();
				        std::string component_name = comp.getPath (".",false,true,"name",true);
				        this->instance_monolith_components_.insert (make_pair (component_name, comp));
				        update_component_parents (comp);
				        update_component_instance (component_name, nodeRefName);
              }
            else if (Udm::IsDerivedFrom
                     (comp_type.type(), ComponentAssemblyReference::meta))
              {
                ComponentAssemblyReference comp_assembly_ref =
                    ComponentAssemblyReference::Cast (comp_type);
				        std::string comp_assembly_ref_name = comp_assembly_ref.name ();
                ComponentAssembly comp_assembly = comp_assembly_ref.ref ();
		            comp_assembly.Accept (*this);

				        for (std::set<Component>::iterator iter = 
                     this->assembly_components_.begin();
					           iter != this->assembly_components_.end();
					           ++iter)
				          {
					          Component comp = *iter;
					          Component typeParent;
					          std::string comp_in_assembly_name = comp.name ();
					          std::string component_ref_name = comp_assembly_ref_name + comp_in_assembly_name;
					          std::string assembly_component_name = comp.getPath (".",false,true,"name",true);
					          this->instance_assembly_components_.insert (make_pair (assembly_component_name, comp));
					          update_component_instance (assembly_component_name, nodeRefName);
					          this->final_assembly_components_.insert (comp);
				          }
		            this->containing_assemblies_.insert(comp_assembly);
				        update_component_assembly_parents (comp_assembly);
              }
			      else if (Udm::IsDerivedFrom (comp_type.type(), SharedComponentReference::meta))
			        {
			          SharedComponentReference shared_component_ref = 
                  SharedComponentReference::Cast (comp_type);
				        std::string shared_comp_ref_name = 
                  shared_component_ref.name ();
                ComponentRef shared_comp = shared_component_ref.ref();
				        Component referred_comp = shared_comp.ref();
				        this->monolith_components_.insert (referred_comp);
                std::string referred_component_name = referred_comp.name ();
				        std::string shared_component_name = 
                  referred_comp.getPath (".",false,true,"name",true);
				        this->instance_monolith_components_.insert 
                  (make_pair (shared_component_name, referred_comp));
				        update_shared_component_parents (shared_comp);
				        update_component_instance (shared_component_name, nodeRefName);
			        }
          }
      }
  }

  void DeploymentPlanFrameworkVisitor::instantiate_deployment_plan_descriptor (DeploymentPlan& dp)
  {
	  this->push();
    std::string name = this->outputPath_ + "\\";
    name += dp.name();
    name += ".cdp";
    this->initTarget (name);
    this->initDocument ("Deployment:deploymentPlan");
    this->initRootAttributes();
  }

  void DeploymentPlanFrameworkVisitor::create_label_and_uuid (DeploymentPlan& dp)
  {
    std::string label = dp.label();
    if (!label.empty())
      {
        this->curr_->appendChild (this->createSimpleContent ("label",
                                                           label));
      }

    std::string uuid = dp.UUID();
    if (uuid.empty())
      {
        dp.UUID() = uuid = ::Utils::CreateUuid();
      }
    else
      {
        // Make a copy as opposed to casting away constness
        DeploymentPlan new_dp = dp;
        // Make sure that every instance has a UUID
        if (dp.isInstance())
          {
            DeploymentPlan typeParent = new_dp.Archetype();
            std::string parentUuid (typeParent.UUID());
            if (uuid == parentUuid)
              {
                new_dp.UUID() = uuid = ::Utils::CreateUuid();
              }
          }
      }

    this->curr_->appendChild (this->createSimpleContent ("UUID", uuid));
  }

  void DeploymentPlanFrameworkVisitor::finalize_deployment_plan_descriptor (void)
  {
	  this->dumpDocument();
    this->pop();
  }

  void DeploymentPlanFrameworkVisitor::generate_parent_connections (void)
  {
	  for (std::set<CQML::ComponentAssembly>::const_iterator assembly_iter =
        this->path_parents_.begin();
        assembly_iter != this->path_parents_.end (); ++assembly_iter)
      {
        ComponentAssembly component_assembly = *assembly_iter;
        update_parent_connections (component_assembly);
      }
  }

  void DeploymentPlanFrameworkVisitor::generate_child_connections (void)
  {
	  for (std::set<CQML::ComponentAssembly>::const_iterator assembly_iter =
        this->containing_assemblies_.begin();
        assembly_iter != this->containing_assemblies_.end (); ++assembly_iter)
      {
        ComponentAssembly component_assembly = *assembly_iter;
        update_connections (component_assembly);
      }
  }

  void DeploymentPlanFrameworkVisitor::generate_implementation_descriptions (void)
  {
	  std::set<ComponentImplementations>
    folders = this->root_folder_.ComponentImplementations_kind_children();
    for (std::set<ComponentImplementations>::iterator iter = folders.begin();
        iter != folders.end();
        ++iter)
      {
        ComponentImplementations folder = *iter;
        folder.Accept (*this);
	    }
  }

  void DeploymentPlanFrameworkVisitor::generate_artifact_descriptions (void)
  {
	  std::set<ImplementationArtifacts>
    folders = this->root_folder_.ImplementationArtifacts_kind_children();
    for (std::set<ImplementationArtifacts>::iterator iter = folders.begin();
        iter != folders.end();
        ++iter)
      {
        ImplementationArtifacts folder = *iter;
        folder.Accept (*this);
      }
  }

  void DeploymentPlanFrameworkVisitor::generate_instance_deployment_descriptions (void)
  {
	  for (std::map<std::string, CQML::Component>::const_iterator instance_iterator =
	       this->instance_monolith_components_.begin ();
		     instance_iterator != this->instance_monolith_components_.end ();
	       ++instance_iterator)
	    {
	      Component instance_component = instance_iterator->second;
		    std::string instance_component_name = instance_iterator->first;
		    this->push ();
		    update_monolith_impl (instance_component);
		    create_component_instance (instance_component, instance_component_name);
		    create_component_config_properties (this->mimpl_);
		    create_component_readonly_attributes (instance_component);
		    this->pop ();
	    }
  }

  void DeploymentPlanFrameworkVisitor::generate_assembly_instance_deployment_descriptions (void)
  {
	  for (std::map<std::string,CQML::Component>::const_iterator assembly_comp_iter =
		     this->instance_assembly_components_.begin ();
		     assembly_comp_iter != this->instance_assembly_components_.end ();
		     ++assembly_comp_iter)
	    {
		    Component assembly_component = assembly_comp_iter->second;
		    std::string assembly_component_name = assembly_comp_iter->first;
		    this->push ();
		    update_monolith_impl (assembly_component);
		    create_component_instance (assembly_component, assembly_component_name);
		    create_component_readonly_attributes (assembly_component);
		    create_assembly_config_properties (assembly_component);
		    create_assembly_attribute_properties (assembly_component);
		    this->pop ();
	    }
  }

  void DeploymentPlanFrameworkVisitor::update_monolith_impl (Component& comp)
  {
	  Component typeParent;
	  std::string component_name = comp.name ();

    if (this->monoimpls_.find (component_name)
        != this->monoimpls_.end ())
      {
	      this->mimpl_ = this->monoimpls_[component_name];
	    }
    else
      {
		    if (comp.isInstance())
	        {
			      typeParent = comp.Archetype();
			      while (typeParent.isInstance())
				      typeParent = typeParent.Archetype();
		      }
		    std::string refName = typeParent.name();
		    if (this->monoimpls_.find (refName)
			      != this->monoimpls_.end ())
		      {
			      this->mimpl_ = this->monoimpls_[refName];
		      }
      }
  }

  void DeploymentPlanFrameworkVisitor::update_component_assembly_parents (ComponentAssembly& comp_assembly)
  {
	  ComponentAssembly comp_assembly_parent;
    // containing_assemblies.insert(comp_assembly);
    if (comp_assembly.isInstance())
      {
        //comp_assembly_parent = comp_assembly.Archetype();
        comp_assembly_parent =
            comp_assembly.ComponentAssembly_parent();
		    this->path_parents_.insert (comp_assembly_parent);
        while (comp_assembly_parent.isInstance())
		      {
            comp_assembly_parent =
            comp_assembly_parent.ComponentAssembly_parent();
		        this->path_parents_.insert (comp_assembly_parent);
		      }
       }

    // containing_assemblies.insert
    //     (comp_assembly.ComponentAssembly_parent());
    // containing_assemblies.insert(comp_assembly_parent);
    // comp_assembly.Accept (*this);
  }
  
  void DeploymentPlanFrameworkVisitor::update_component_parents (Component& comp)
  {
	  ComponentAssembly component_assembly_parent;
	  component_assembly_parent = comp.ComponentAssembly_parent();
    
    if (component_assembly_parent.isInstance())
      {
		    this->path_parents_.insert (component_assembly_parent);
		    component_assembly_parent =
			    component_assembly_parent.ComponentAssembly_parent();
		    while (component_assembly_parent.isInstance())
		      {
			      this->path_parents_.insert (component_assembly_parent);
			      component_assembly_parent =
			        component_assembly_parent.ComponentAssembly_parent();
		      }
      }
    this->path_parents_.insert(component_assembly_parent);
  }

  void DeploymentPlanFrameworkVisitor::create_assembly_config_properties (Component& comp)
  {
	  std::set<AssemblyConfigProperty> cps = comp.dstAssemblyConfigProperty();
	  for (std::set<AssemblyConfigProperty>::const_iterator it2 = cps.begin();
			   it2 != cps.end();
			   ++it2)
	    {
		    AssemblyConfigProperty cp = *it2;
		    cp.Accept (*this);
	    }
  }

  void DeploymentPlanFrameworkVisitor::create_assembly_attribute_properties (Component& comp)
  {
    std::string uniqueName = comp.UUID();
    if (uniqueName.empty())
      {
        comp.UUID() = uniqueName = ::Utils::CreateUuid();
      }
	  uniqueName = std::string ("_") + uniqueName;

	  for (std::map<std::pair<std::string, std::string>, Property>::
		     const_iterator iter = this->attrValues_.begin();
		     iter != this->attrValues_.end();
		     ++iter)
	    {
        std::pair<std::pair<std::string, std::string>, Property>
			     attrVal = *iter;
	      std::pair<std::string, std::string> compAttr 
          = attrVal.first;
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
  }

  void DeploymentPlanFrameworkVisitor::create_component_config_properties (MonolithicImplementation& mimpl)
  {
	  const std::set<ConfigProperty> imcps = 
      mimpl.dstConfigProperty();
    for (std::set<ConfigProperty>::const_iterator it2 = 
         imcps.begin();
         it2 != imcps.end();
         ++it2)
      {
        ConfigProperty imcp = *it2;
        imcp.Accept (*this);
      }
  }

  void DeploymentPlanFrameworkVisitor::create_component_readonly_attributes (Component& comp)
  {
	  std::set<ReadonlyAttribute> attrs = 
      comp.ReadonlyAttribute_children();
    for (std::set<ReadonlyAttribute>::const_iterator 
         iter = attrs.begin();
         iter != attrs.end();
         ++iter)
      {
        ReadonlyAttribute attr = *iter;
        attr.Accept (*this);
      }
  }

  void DeploymentPlanFrameworkVisitor::update_shared_component_parents (ComponentRef& comp_ref)
  {
	  ComponentAssembly component_assembly_parent;
	  component_assembly_parent = comp_ref.ComponentAssembly_parent();
    
    if (component_assembly_parent.isInstance())
      {
		    this->path_parents_.insert (component_assembly_parent);
		    component_assembly_parent =
			    component_assembly_parent.ComponentAssembly_parent();
		    while (component_assembly_parent.isInstance())
		      {
			      this->path_parents_.insert (component_assembly_parent);
			      component_assembly_parent =
			        component_assembly_parent.ComponentAssembly_parent();
		      }
      }
    this->path_parents_.insert(component_assembly_parent);
  }

  void DeploymentPlanFrameworkVisitor::update_component_instance (std::string& comp_instance_name, std::string& nodeRefName)
  {
    if (this->deployed_instances_.find (comp_instance_name) != this->deployed_instances_.end ())
      {
        this->deployed_instances_[comp_instance_name] = nodeRefName;
      }
    else
      {
        this->deployed_instances_.insert (make_pair (comp_instance_name, nodeRefName));
      }
  }

  void DeploymentPlanFrameworkVisitor::create_component_instance (Component& comp, std::string& instance_name)
  {
	  DOMElement* ele = this->doc_->createElement (XStr ("instance"));

    /*std::string instanceName = 
        comp.getPath (".",false,true,"name",true);
	  std::string nodeRefName = this->deployed_instances_[instance_name];
	  std::string uniqueName = comp.UUID();
    if (uniqueName.empty())
      {
        comp.UUID() = uniqueName = ::Utils::CreateUuid();
      }
	  uniqueName = std::string ("_") + uniqueName;*/

	  // this->selected_instances_.insert (uniqueName);
	  this->selected_instances_.insert (instance_name);
    std::string nodeRefName = this->deployed_instances_[instance_name];

    std::string id_name = std::string ("_") + instance_name;
    
    // ele->setAttribute (XStr ("id"), XStr (uniqueName));
	  ele->setAttribute (XStr ("id"), XStr (id_name));

    /*ele->appendChild
        (this->createSimpleContent ("name", instanceName));*/
	  ele->appendChild
        (this->createSimpleContent ("name", instance_name));

    ele->appendChild
        (this->createSimpleContent ("node", nodeRefName));

    ele->appendChild (this->doc_->createElement (XStr ("source")));

    this->curr_->appendChild (ele);
    this->curr_ = ele;

	  std::string mimpl_name = this->mimpl_.UUID();
    if (mimpl_name.empty())
      {
        this->mimpl_.UUID() = mimpl_name = ::Utils::CreateUuid();
      }
	  mimpl_name = std::string ("_") + mimpl_name;
    this->curr_->appendChild
        (this->createSimpleContent ("implementation", mimpl_name));
  }

  void DeploymentPlanFrameworkVisitor::update_parent_connections
         (const ComponentAssembly& assembly)
  {
    const std::set<invoke> invokes = assembly.invoke_kind_children();
    for (std::set<invoke>::const_iterator iter = invokes.begin();
            iter != invokes.end();
            ++iter)
      {
        invoke iv = *iter;
        iv.Accept (*this);
      }

    const std::set<emit> emits = assembly.emit_kind_children();
    for (std::set<emit>::const_iterator iter = emits.begin();
         iter != emits.end();
         ++iter)
      {
        emit ev = *iter;
        ev.Accept (*this);
      }

    const std::set<publish> publishers = assembly.publish_kind_children();
    for (std::set<publish>::const_iterator iter = publishers.begin();
         iter != publishers.end();
         ++iter)
      {
        publish ev = *iter;
        ev.Accept (*this);
      }

    const std::set<deliverTo> deliverTos = assembly.deliverTo_kind_children();
    for (std::set<deliverTo>::const_iterator iter = deliverTos.begin();
         iter != deliverTos.end();
         ++iter)
      {
        deliverTo dv = *iter;
        dv.Accept (*this);
      }

    const std::set<PublishConnector>
        connectors = assembly.PublishConnector_kind_children();
    for (std::set<PublishConnector>::const_iterator iter =
         connectors.begin();
         iter != connectors.end();
         ++iter)
      {
        PublishConnector conn = *iter;
        conn.Accept (*this);
      }

    this->publishers_.erase (this->publishers_.begin(),
                             this->publishers_.end());

    this->consumers_.erase (this->consumers_.begin(),
                            this->consumers_.end());
  }

  void DeploymentPlanFrameworkVisitor::update_connections(const ComponentAssembly& assembly)
  {
    // Collect all the Components of this assembly into a set.
    std::set<Component> comps = assembly.Component_kind_children();

    // Add all the shared Components of this assembly into the set.  A
    // shared Component is implemented as a reference to a Component.  So
    // just traverse the reference and add it to the set.
    std::set<ComponentRef> scomps = assembly.ComponentRef_kind_children();
    for (std::set<ComponentRef>::const_iterator
           iter = scomps.begin();
         iter != scomps.end();
         ++iter)
      {
        const ComponentRef compRef = *iter;
        comps.insert (compRef.ref());
      }

    // Collect all the immediate ComponentAssembly children of this assembly
    std::set<ComponentAssembly>
      subasms = assembly.ComponentAssembly_kind_children();

    // Add all the shared ComponentAssemblies of the current assembly.
    // Like shared components, shared assemblies are also implemented as
    // references.  So just traverse the references, and add them to the set.
    std::set<ComponentAssemblyReference>
      sasms = assembly.ComponentAssemblyReference_kind_children();
    for (std::set<ComponentAssemblyReference>::const_iterator
           iter = sasms.begin();
         iter != sasms.end();
         ++iter)
      {
        const ComponentAssemblyReference asmRef = *iter;
        subasms.insert (asmRef.ref());
      }

    // Maintain a list of all ComponentAssemblies in this assembly
    std::vector<ComponentAssembly> assemblies;

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

        subasms.erase(rassembly);

        // Get the components of the current assembly, and insert them into
        // the component list
        std::set<Component> rcomps = rassembly.Component_kind_children();

	      // Get the shared components of the current assembly
        scomps = rassembly.ComponentRef_kind_children();
        for (std::set<ComponentRef>::const_iterator
               iter = scomps.begin();
               iter != scomps.end();
             ++iter)
          {
            const ComponentRef compRef = *iter;
            rcomps.insert (compRef.ref());
          }

        comps.insert (rcomps.begin(), rcomps.end());

        // Get the subassemblies of the first assembly.
        std::set<ComponentAssembly>
          rasms = rassembly.ComponentAssembly_kind_children();

	      // Add all the shared ComponentAssemblies of the current assembly.
        // Like shared components, shared assemblies are also implemented
        // as references.  So just traverse the references, and add them to
        // the set.
        std::set<ComponentAssemblyReference>
          sasms = rassembly.ComponentAssemblyReference_kind_children();
        for (std::set<ComponentAssemblyReference>::const_iterator
               iter = sasms.begin();
             iter != sasms.end();
             ++iter)
          {
            const ComponentAssemblyReference asmRef = *iter;
            rasms.insert (asmRef.ref());
          }

        // Insert them to the current list.
        // std::copy (rasms.begin(), rasms.end(), std::back_inserter (subasms));
	      subasms.insert (rasms.begin(), rasms.end());
      }

    for (std::vector<ComponentAssembly>::iterator iter = assemblies.begin();
         iter != assemblies.end();
         ++iter)
      {
        ComponentAssembly subasm = *iter;

        const std::set<invoke> invokes = subasm.invoke_kind_children();
        for (std::set<invoke>::const_iterator iter = invokes.begin();
             iter != invokes.end();
             ++iter)
          {
            invoke iv = *iter;
            iv.Accept (*this);
          }

        const std::set<emit> emits = subasm.emit_kind_children();
        for (std::set<emit>::const_iterator iter = emits.begin();
             iter != emits.end();
             ++iter)
          {
            emit ev = *iter;
            ev.Accept (*this);
          }

        const std::set<publish> publishers = subasm.publish_kind_children();
        for (std::set<publish>::const_iterator iter = publishers.begin();
             iter != publishers.end();
             ++iter)
          {
            publish ev = *iter;
            ev.Accept (*this);
          }

        const std::set<deliverTo> deliverTos = subasm.deliverTo_kind_children();
        for (std::set<deliverTo>::const_iterator iter = deliverTos.begin();
             iter != deliverTos.end();
             ++iter)
          {
            deliverTo dv = *iter;
            dv.Accept (*this);
          }

        const std::set<PublishConnector>
          connectors = subasm.PublishConnector_kind_children();
        for (std::set<PublishConnector>::const_iterator iter =
             connectors.begin();
             iter != connectors.end();
             ++iter)
          {
            PublishConnector conn = *iter;
            conn.Accept (*this);
          }

        this->publishers_.erase (this->publishers_.begin(),
                                 this->publishers_.end());

        this->consumers_.erase (this->consumers_.begin(),
                                this->consumers_.end());
      }
  }

  void DeploymentPlanFrameworkVisitor::Visit_ComponentAssembly
           (const ComponentAssembly& assembly)
  {
    std::string node_reference_name = this->retNodeRefName ();
    std::string cg_name = this->retcgName ();

    // Collect all the Components of this assembly into a set.
    std::set<Component> comps = assembly.Component_kind_children();

    // Add all the shared Components of this assembly into the set.  A
    // shared Component is implemented as a reference to a Component.  So
    // just traverse the reference and add it to the set.
    std::set<ComponentRef> scomps = assembly.ComponentRef_kind_children();
    for (std::set<ComponentRef>::const_iterator
           iter = scomps.begin();
         iter != scomps.end();
         ++iter)
      {
        const ComponentRef compRef = *iter;
        comps.insert (compRef.ref());
      }

    // Collect all the immediate ComponentAssembly children of this assembly
    std::set<ComponentAssembly>
      subasms = assembly.ComponentAssembly_kind_children();

    // Add all the shared ComponentAssemblies of the current assembly.
    // Like shared components, shared assemblies are also implemented as
    // references.  So just traverse the references, and add them to the set.
    std::set<ComponentAssemblyReference>
      sasms = assembly.ComponentAssemblyReference_kind_children();
    for (std::set<ComponentAssemblyReference>::const_iterator
           iter = sasms.begin();
         iter != sasms.end();
         ++iter)
      {
        const ComponentAssemblyReference asmRef = *iter;
        subasms.insert (asmRef.ref());
      }

    // Maintain a list of all ComponentAssemblies in this assembly
    std::vector<ComponentAssembly> assemblies;

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

        subasms.erase(rassembly);

        // Get the components of the current assembly, and insert them into
        // the component list
        std::set<Component> rcomps = rassembly.Component_kind_children();

	      // Get the shared components of the current assembly
        scomps = rassembly.ComponentRef_kind_children();
        for (std::set<ComponentRef>::const_iterator
               iter = scomps.begin();
               iter != scomps.end();
             ++iter)
          {
            const ComponentRef compRef = *iter;
            rcomps.insert (compRef.ref());
          }

        comps.insert (rcomps.begin(), rcomps.end());

        // Get the subassemblies of the first assembly.
        std::set<ComponentAssembly>
          rasms = rassembly.ComponentAssembly_kind_children();

	      // Add all the shared ComponentAssemblies of the current assembly.
        // Like shared components, shared assemblies are also implemented
        // as references.  So just traverse the references, and add them to
        // the set.
        std::set<ComponentAssemblyReference>
          sasms = rassembly.ComponentAssemblyReference_kind_children();
        for (std::set<ComponentAssemblyReference>::const_iterator
               iter = sasms.begin();
             iter != sasms.end();
             ++iter)
          {
            const ComponentAssemblyReference asmRef = *iter;
            rasms.insert (asmRef.ref());
          }

        // Insert them to the current list.
        // std::copy (rasms.begin(), rasms.end(), std::back_inserter (subasms));
	      subasms.insert (rasms.begin(), rasms.end());
      }

    // Create the appropriate component attribute value mappings
    this->CreateAttributeMappings (assemblies);
	  this->assembly_components_ = comps;
  }

  void DeploymentPlanFrameworkVisitor::CreateAttributeMappings (
        std::vector<ComponentAssembly>& assemblies)
  {
    for (std::vector<ComponentAssembly>::iterator iter = assemblies.begin();
         iter != assemblies.end();
         ++iter)
      {
        ComponentAssembly assembly = *iter;
        std::string assemblyName = this->ExtractName (assembly);
        const std::set<AttributeMapping>
          mappings = assembly.AttributeMapping_kind_children();
        for (std::set<AttributeMapping>::const_iterator iter = mappings.begin();
             iter != mappings.end();
             ++iter)
          {
            AttributeMapping mapping = *iter;
            mapping.Accept (*this);
          }
      }
  }

  void DeploymentPlanFrameworkVisitor::Visit_AttributeMapping(const AttributeMapping& mapping)
  {
    std::string mappingName = this->ExtractName (mapping);
    AttributeMappingValue value = mapping.dstAttributeMappingValue();
    if (value != Udm::null)
      {
        Property prop = value.dstAttributeMappingValue_end();
        std::set<std::pair<std::string, std::string> > compAttrs;
        this->GetAttributeComponents (mapping, compAttrs);
        for (std::set<std::pair<std::string, std::string> >::const_iterator
               iter = compAttrs.begin();
             iter != compAttrs.end();
             ++iter)
          {
            // Get the component, attribute pair
            pair<std::string, std::string> compAttr = *iter;
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

  void DeploymentPlanFrameworkVisitor::GetAttributeComponents (const AttributeMapping& mapping,
                      std::set<std::pair<std::string, std::string> >& output)
  {
    std::string mappingName = this->ExtractName (mapping);
    std::set<AttributeDelegate> delegates = mapping.dstAttributeDelegate();
    if (delegates.empty())
      {
        std::set<AttributeMappingDelegate>
          mapDelegates = mapping.dstAttributeMappingDelegate();
        if (mapDelegates.empty())
          {
            std::string mapPath = 
                mapping.getPath ("_", false, true,"name",true);

            throw udm_exception (std::string ("AttributeMapping " +
                                              mapPath +
               " is not connected to any attributes or delegated to another AttributeMapping"));
          }
        else
          {
            for (std::set<AttributeMappingDelegate>::const_iterator
                   iter = mapDelegates.begin();
                 iter != mapDelegates.end();
                 ++iter)
              {
                AttributeMappingDelegate mapDelegate = *iter;
                AttributeMapping
                  delegate = mapDelegate.dstAttributeMappingDelegate_end();
                std::string delegateName = this->ExtractName (delegate);
                this->GetAttributeComponents (delegate, output);
              }
          }
      }
    else
      {
        for (std::set<AttributeDelegate>::const_iterator
               iter = delegates.begin();
             iter != delegates.end();
             ++iter)
          {
            AttributeDelegate delegate = *iter;
            ReadonlyAttribute attr = delegate.dstAttributeDelegate_end();
            std::string attrName = this->ExtractName (attr);
            Component parent = attr.Component_parent();
            std::string parentName = this->ExtractName (parent);

            // std::string compName = 
            //      parent.getPath ("_", false, true,"name",true);
	          std::string compName = parent.UUID();
            if (compName.empty())
              {
                parent.UUID() = compName = ::Utils::CreateUuid();
              }
	          compName = std::string ("_") + compName;
            output.insert (make_pair (compName, attr.name()));
          }
      }
  }

  void DeploymentPlanFrameworkVisitor::CreateAssemblyInstances (std::set<Component>& comps)
  {
    for (std::set<Component>::iterator iter = comps.begin();
         iter != comps.end();
         ++iter)
      {
        Component comp = *iter;
        DOMElement* instance = this->doc_->createElement (XStr ("instance"));
        this->curr_->appendChild (instance);
        this->push();
        this->curr_ = instance;

        // std::string uniqueName = comp.getPath ("_",false,true,"name",true);
	      std::string uniqueName = comp.UUID();
        if (uniqueName.empty())
          {
            comp.UUID() = uniqueName = ::Utils::CreateUuid();
          }
	      uniqueName = std::string ("_") + uniqueName;

        instance->setAttribute (XStr ("xmi:id"), XStr (uniqueName));
        instance->appendChild (this->createSimpleContent ("name",
                                                          uniqueName));
        Component typeParent;
        if (comp.isInstance())
          {
           typeParent = comp.Archetype();
            while (typeParent.isInstance())
              typeParent = typeParent.Archetype();
          }
        std::string interfaceName = typeParent.name();
        std::string refName = this->interfaces_[interfaceName];
        refName += ".cpd";
        DOMElement* refEle = this->doc_->createElement (XStr ("package"));
        refEle->setAttribute (XStr ("href"), XStr (refName));
        instance->appendChild (refEle);
        std::set<AssemblyConfigProperty> cps = comp.dstAssemblyConfigProperty();
        for (std::set<AssemblyConfigProperty>::const_iterator it2 = cps.begin();
             it2 != cps.end();
             ++it2)
          {
            AssemblyConfigProperty cp = *it2;
            cp.Accept (*this);
          }
        std::set<ReadonlyAttribute> attrs = comp.ReadonlyAttribute_children();
        for (std::set<ReadonlyAttribute>::const_iterator iter = attrs.begin();
             iter != attrs.end();
             ++iter)
          {
            ReadonlyAttribute attr = *iter;
            attr.Accept (*this);
          }
        for (std::map<std::pair<std::string, std::string>, 
             Property>::const_iterator iter = this->attrValues_.begin();
             iter != this->attrValues_.end();
             ++iter)
          {
            std::pair<std::pair<std::string, std::string>, Property>
              attrVal = *iter;
            std::pair<std::string, std::string> compAttr = attrVal.first;
            if (compAttr.first == uniqueName)
              {
                this->push();
                DOMElement*
                  ele = this->doc_->createElement (XStr ("configProperty"));
                this->curr_->appendChild (ele);
				        this->curr_ = ele;
                Property val = attrVal.second;
                //val.Accept (*this);
	              this->CreatePropertyElement (compAttr.second, val);
		            this->pop();
              }
          }
        this->pop();
      }
  }

  void DeploymentPlanFrameworkVisitor::Visit_ReadonlyAttribute(const ReadonlyAttribute& attr)
  {
    AttributeValue attValue = attr.dstAttributeValue();
    if (attValue != Udm::null)
      attValue.Accept (*this);
  }

  void DeploymentPlanFrameworkVisitor::Visit_AttributeValue(const AttributeValue& value)
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

  void DeploymentPlanFrameworkVisitor::Visit_AttributeDelegate(const AttributeDelegate&){}

  void DeploymentPlanFrameworkVisitor::Visit_AttributeMappingValue
         (const AttributeMappingValue&){}

  void DeploymentPlanFrameworkVisitor::Visit_AttributeMappingDelegate
           (const AttributeMappingDelegate&){}

  void DeploymentPlanFrameworkVisitor::CreateAssemblyConnections 
          (std::vector<ComponentAssembly>& assemblies)
  {
    for (std::vector<ComponentAssembly>::iterator iter = assemblies.begin();
         iter != assemblies.end();
         ++iter)
      {
        ComponentAssembly subasm = *iter;
        const std::set<invoke> invokes = subasm.invoke_kind_children();
        for (std::set<invoke>::const_iterator iter = invokes.begin();
             iter != invokes.end();
             ++iter)
          {
            invoke iv = *iter;
            iv.Accept (*this);
          }
        const std::set<emit> emits = subasm.emit_kind_children();
        for (std::set<emit>::const_iterator iter = emits.begin();
             iter != emits.end();
             ++iter)
          {
            emit ev = *iter;
            ev.Accept (*this);
          }
        const std::set<publish> publishers = subasm.publish_kind_children();
        for (std::set<publish>::const_iterator iter = publishers.begin();
             iter != publishers.end();
             ++iter)
          {
            publish ev = *iter;
            ev.Accept (*this);
          }
        const std::set<deliverTo> deliverTos = subasm.deliverTo_kind_children();
        for (std::set<deliverTo>::const_iterator iter = deliverTos.begin();
             iter != deliverTos.end();
             ++iter)
          {
            deliverTo dv = *iter;
            dv.Accept (*this);
          }
        const std::set<PublishConnector>
          connectors = subasm.PublishConnector_kind_children();
        for (std::set<PublishConnector>::const_iterator iter = 
             connectors.begin();
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

  void DeploymentPlanFrameworkVisitor::Visit_InstanceMapping(const InstanceMapping& ins_map)
  {
    NodeReference node_ref = ins_map.dstInstanceMapping_end();
    node_ref.Accept (*this);
  }

  void DeploymentPlanFrameworkVisitor::Visit_NodeReference(const NodeReference& node_ref)
  {
  }
}