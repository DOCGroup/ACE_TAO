#include <windows.h>
#include <algorithm>
#include <functional>
#include "PathVisitor.h"
#include "UmlExt.h"

using xercesc::LocalFileFormatTarget;
using xercesc::DOMImplementationRegistry;
using xercesc::DOMImplementationLS;
using xercesc::DOMException;
using xercesc::XMLUni;
using xercesc::XMLException;
using xercesc::DOMText;

#include <fstream>

#include <boost/graph/topological_sort.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/pending/integer_range.hpp>
#include <boost/pending/indirect_cmp.hpp>

std::ofstream outf;
using Utils::XStr;

namespace PICML
{
  template < typename TimeMap > class dfs_time_visitor: public boost::default_dfs_visitor {
    typedef typename boost::property_traits < TimeMap >::value_type T;
  public:
    dfs_time_visitor(TimeMap dmap, TimeMap fmap, T & t , PathVisitor* visitor)
  :  m_dtimemap(dmap), m_ftimemap(fmap), m_time(t), visitor_(visitor) {
    }
    template < typename Vertex, typename Graph >
      void discover_vertex(Vertex u, const Graph & g) const
    {
      put(m_dtimemap, u, m_time++);
            //TODO ::

      // already got all the paths , ignore
      if (visitor_->got_all_path_)
        return;

      if (visitor_->searching_end_tree_ == 0)
      {
        // here remove the vertex from the path ...
        visitor_->current_path_.push_back (u);
      }
      // if vertex == dest vertex .. save path
      if (u == visitor_->end_port_)
      {
        // save the path
        visitor_->all_the_paths_.push_back (visitor_->current_path_);

        // now search the end tree ...
        visitor_->searching_end_tree_ = 1;
      }
    }
    template < typename Vertex, typename Graph >
      void finish_vertex(Vertex u, const Graph & g) const
    {
      put(m_ftimemap, u, m_time++);
            //TODO ::

      // already got all the paths , ignore
      if (visitor_->got_all_path_)
        return;
      if (u == visitor_->start_port_)
        visitor_->got_all_path_ = 1;

      if (u == visitor_->end_port_)
        visitor_->searching_end_tree_ = 0;

      if (visitor_->searching_end_tree_ == 0)
      {
        // here remove the vertex from the path ...
        visitor_->current_path_.pop_back ();
      }
    }
    //template < typename Vertex, typename Graph >
//    void tree_edge (e, g)
    template <class Edge, class Graph>
      void tree_edge(Edge u, const Graph& g)
    {
      Edge_Numbers new_edge;
      new_edge.source_ = u.m_source;
      new_edge.target_ = u.m_target;
      visitor_->dfs_edges.push_back (new_edge);
      //TODO ::
      // add the end point to the .. path ..
    }
    template <class Edge, class Graph>
    void forward_or_cross_edge(Edge u, const Graph& g)
    {
      Edge_Numbers new_edge;
      new_edge.source_ = u.m_source;
      new_edge.target_ = u.m_target;
      visitor_->forward_edges_.push_back (new_edge);
      //TODO ::

      // already got all the paths , ignore
      if (visitor_->got_all_path_)
        return;

      // if this is within the tree rooted at
      //the end-point then ignore
      if (visitor_->searching_end_tree_ == 1)
      {
        return;
      }

      // check the end-point
      // if it belongs to any node already discovered in the
      // previous paths except the current path .. then save path

      if (u.m_target == visitor_->end_port_)
      {
        PathVisitor::A_Path new_path = visitor_->current_path_;
        new_path.push_back (u.m_target);
        visitor_->all_the_paths_.push_back (new_path);
      }
      else
        visitor_->search_node_in_previous_paths (u.m_target);
    }

    TimeMap m_dtimemap;
    TimeMap m_ftimemap;
    T & m_time;
    PathVisitor* visitor_;
  };

  /*Component_Port_Vertex create_a_component (Port a_port , string par_name)
  {
    Component_Port_Vertex vertex;
    vertex.vertex_name_ = par_name;
    vertex.vertex_name_ = "_";
    vertex.vertex_name_ = the_port_.name ();
    vertex.the_port_ = a_port;
    vertex.the_component_ = a_port.parent ();
  }*/

  Component_Port_Vertex::Component_Port_Vertex (Port a_port , string par_name)
    : the_component_(a_port.parent ()),
      the_port_(a_port)
  {
    is_drawn = 0;

    this->vertex_name_ = the_component_.getPath (".", false, true, "name", true);
    //this->vertex_name_ +=
    //this->vertex_name_ = par_name;

    this->vertex_name_ += "/";
    this->vertex_name_ += the_port_.name ();

    // here save the vertex name ...
    this->display_label_ = this->vertex_name_;

    // now add the port type to the vertex_name ...
    if (Udm::IsDerivedFrom (a_port.type () , InEventPort::meta))
      this->vertex_name_ += "InEventPort";
    else if (Udm::IsDerivedFrom (a_port.type () , OutEventPort::meta))
      this->vertex_name_ += "OutEventPort";
    else if (Udm::IsDerivedFrom (a_port.type () , RequiredRequestPort::meta))
      this->vertex_name_ += "RequiredRequestPort";
    else if (Udm::IsDerivedFrom (a_port.type () , ProvidedRequestPort::meta))
      this->vertex_name_ += "ProvidedRequestPort";
  }


  PathVisitor::PathVisitor (Udm::Object port)
    : impl_ (0), doc_ (0), root_ (0), curr_ (0), serializer_ (0), target_ (0),
      the_graph_ (100), vertex_count_ (0)
  {
    //Port startPort = Port::Cast (port);
    Port startPort;
    try
    {
     // OutEventPort startPort = OutEventPort::Cast (port);
      startPort = Port::Cast (port);
    }
    catch (udm_exception)
    {
      throw;
    }
    Udm::Object parent = startPort.parent ();
    Component parent_cmp = Component::Cast (parent);

  }

  PathVisitor::PathVisitor (set<Udm::Object>& selections)
    : impl_ (0),
      doc_ (0),
      root_ (0),
      curr_ (0),
      serializer_ (0),
      target_ (0),
      the_graph_ (100),
      vertex_count_ (0),
      searching_end_tree_ (0),
      got_all_path_ (0)
  {
    set<Udm::Object>::iterator iter;
    iter = selections.begin ();
    //  to counter GME's strange behavior
   // iter++;
    Port startPort;
    // TODO :: here save the StartPort as one variable in the
    // class , same for the Destination port
    try
    {
     // OutEventPort startPort = OutEventPort::Cast (port);
      startPort = Port::Cast (*iter++);
    }
    catch (udm_exception)
    {
      throw;
    }

    // now store the end port
    Port end_port;
    try
    {
     // OutEventPort startPort = OutEventPort::Cast (port);
      end_port = Port::Cast (*iter);
    }
    catch (udm_exception)
    {
      throw;
    }

/// verify the port sequence with the user ...
    // form the message here ,
    std::string msg = "You have chosen startPort = ";
    msg += startPort.name ();
    msg += " and endPort = ";
    msg += end_port.name ();
    //msg += " Please confirm";

    int result = //AfxMessageBox (msg.c_str (), MB_YESNO);
      MessageBox (0, msg.c_str () , "Port Order" , MB_YESNO | MB_ICONQUESTION);

    if (result == IDNO)
    {
      // the order is reversed , swap here
      Port temp;
      temp = startPort;
      startPort = end_port;
      end_port = temp;
    }


    //Component_Port_Vertex vertex (startPort, this->extract_uuid(startPort.parent ()));
    Component_Port_Vertex vertex (startPort, startPort.getPath (".", false, true, "name", true));

    graph_vertex_indices_[vertex_count_]
        = vertex;
    graph_vertex_[vertex.vertex_name_]
        = vertex_count_;

    // store the start port
    start_port_ = vertex_count_++;

    //Component_Port_Vertex end_vertex (end_port,this->extract_uuid (end_port.parent ()));
    Component_Port_Vertex end_vertex (end_port, end_port.getPath (".", false, true, "name", true));

    graph_vertex_indices_[vertex_count_]
        = end_vertex;
    graph_vertex_[end_vertex.vertex_name_]
        = vertex_count_;

    // store the end port
    end_port_ = vertex_count_++;
  }

  PathVisitor::~PathVisitor ()
  {
    /*
    if (this->doc_)
      this->doc_->release();
    delete this->target_;
    */
  }

  void PathVisitor::init()
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

  void PathVisitor::initTarget (const std::string& fileName)
  {
    if (this->target_)
      delete this->target_;
    this->target_ = new LocalFileFormatTarget (fileName.c_str());
  }

  void PathVisitor::initDocument (const std::string& rootName)
  {
    if (this->doc_)
      this->doc_->release();
    // Create the document
    this->doc_ =
      this->impl_->createDocument (XStr ("http://www.omg.org/Deployment"),
                                   XStr (rootName.c_str()),
                                   0);
  }

  void PathVisitor::initRootAttributes()
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

  void PathVisitor::dumpDocument()
  {
    this->serializer_->writeNode (this->target_, *this->doc_);
  }

  void PathVisitor::push()
  {
    //this->curr_stack_.push (this->curr_);
  }

  void PathVisitor::pop()
  {
  }

  DOMElement* PathVisitor::createSimpleContent (const std::string& name,
                                                   const std::string& value)
  {
    DOMElement* pName = this->doc_->createElement (XStr (name.c_str()));
    DOMText* pValue = this->doc_->createTextNode (XStr (value.c_str()));
    pName->appendChild (pValue);
    return pName;
  }

  void PathVisitor::Visit_RootFolder(const RootFolder& rf)
  {
    outf.open ("Test.txt");

    std::string name = rf.name ();

    outf << name << std::endl;

    outf.close ();


    std::set<ComponentImplementations>
      impls = rf.ComponentImplementations_kind_children();
    for (std::set<ComponentImplementations>::iterator iter = impls.begin();
         iter != impls.end();
         ++iter)
      {
        ComponentImplementations folder = *iter;
        folder.Accept (*this);
      }

      std::vector < size_type > discover_order(num_vertices (the_graph_));
      CalculatePath (discover_order);

      PathDiagrams a_path_diagram
        = PathDiagrams::Create (rf);

      a_path_diagram.name () = "PathDiagram";

      // Create All_paths model here and place all the path
      // within this
      Paths all_paths = Paths::Create (a_path_diagram);

      // Form the path name here it will be of
      // startport_endport
      // add the start port name
      std::string path_name = graph_vertex_indices_[this->start_port_].the_port_.name ();
      path_name += std::string ("_");
      path_name += graph_vertex_indices_[this->end_port_].the_port_.name ();
      all_paths.name () = path_name;

      int counter = 0;
      for (PathList::iterator iter = this->all_the_paths_.begin ();
           iter != this->all_the_paths_.end ();
           iter++, counter++)
      {
        the_current_path_flow_ = Path::Create (all_paths);

        // form the path name
        char path_name [100];
        sprintf (path_name ,"%s%d", "Path_" , counter);

        the_current_path_flow_.name () = path_name;

        A_Path a_path = *iter;
        draw_path (&a_path);
      }
  }

  // Implementation Artifact operations

  void PathVisitor::Visit_ImplementationArtifacts(const ImplementationArtifacts& ia)
  {
    std::set<ArtifactContainer>
      afs = ia.ArtifactContainer_kind_children();
    for (std::set<ArtifactContainer>::iterator iter = afs.begin();
         iter != afs.end();
         ++iter)
      {
        ArtifactContainer ac = *iter;
        ac.Accept (*this);
      }
  }

  void PathVisitor::Visit_ArtifactContainer(const ArtifactContainer& ac)
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

  void PathVisitor::Visit_ImplementationArtifact(const ImplementationArtifact& ia)
  {
    this->push();
    std::string name = "\\";
    name += ia.name();
    name += ".iad";
    this->initTarget (name);
    this->initDocument ("Deployment:ImplementationArtifactDescription");
    this->initRootAttributes();

    std::string label = ia.label();
    if (!label.empty())
      this->curr_->appendChild (this->createSimpleContent ("label",
                                                           label));
    std::string uuid = ia.UUID();
    if (uuid.empty())
      uuid = ia.getPath ("_", false, true, "name", true);
    this->curr_->appendChild (this->createSimpleContent ("UUID", uuid));

    std::string location = ia.location();
    if (!location.empty())
      this->curr_->appendChild (this->createSimpleContent ("location",
                                                           location));

    const std::set<ArtifactDependsOn> dps = ia.dstArtifactDependsOn();
    for (std::set<ArtifactDependsOn>::const_iterator iter = dps.begin();
         iter != dps.end();
         ++iter)
      {
        ArtifactDependsOn ad = *iter;
        ad.Accept (*this);
      }

    const std::set<ArtifactDependency> adps = ia.dstArtifactDependency();
    for (std::set<ArtifactDependency>::const_iterator iter = adps.begin();
         iter != adps.end();
         ++iter)
      {
        ArtifactDependency ad = *iter;
        ad.Accept (*this);
      }

    const std::set<ArtifactExecParameter> exec = ia.dstArtifactExecParameter();
    for (std::set<ArtifactExecParameter>::const_iterator it = exec.begin();
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

  void PathVisitor::Visit_ArtifactDependsOn(const ArtifactDependsOn& ado)
  {
    ImplementationArtifactReference ref = ado.dstArtifactDependsOn_end();
    ref.Accept (*this);
  }

  void PathVisitor::Visit_ArtifactDependency(const ArtifactDependency& ado)
  {
    this->push();
    ImplementationArtifact ia = ado.dstArtifactDependency_end();
    DOMElement* ele = this->doc_->createElement (XStr ("dependsOn"));
    ele->appendChild (this->createSimpleContent ("name", ia.name()));
    std::string iaName (ia.name());
    iaName += ".iad";
    DOMElement*
      iaEle = this->doc_->createElement (XStr ("referencedArtifact"));
    iaEle->setAttribute (XStr ("href"), XStr (iaName));
    ele->appendChild (iaEle);
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PathVisitor::Visit_ImplementationArtifactReference(const ImplementationArtifactReference& iar)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("dependsOn"));
    ele->appendChild (this->createSimpleContent ("name", iar.name()));
    const ImplementationArtifact ref = iar.ref();
    std::string refName (ref.name());
    refName += ".iad";
    DOMElement*
      refEle = this->doc_->createElement (XStr ("referencedArtifact"));
    refEle->setAttribute (XStr ("href"), XStr (refName));
    ele->appendChild (refEle);
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PathVisitor::Visit_ArtifactExecParameter(const ArtifactExecParameter& param)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("execParameter"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    Property ref = param.dstArtifactExecParameter_end();
    ref.Accept (*this);
    this->pop();
  }

  void PathVisitor::Visit_Property(const Property& property)
  {
  this->CreatePropertyElement (property.name(), property);
  }

  void PathVisitor::CreatePropertyElement (std::string name, const Property& property)
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

  void PathVisitor::Visit_DataType(const DataType& type)
  {
    PredefinedType ref = type.ref();
    std::string kindName = ref.name();
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
  void PathVisitor::Visit_Boolean(const Boolean& boolean)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_boolean"));
    this->pop();

  }

  void PathVisitor::Visit_Byte(const Byte& byte)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_octet"));
    this->pop();
  }

  void PathVisitor::Visit_String(const String& str)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_string"));
    this->pop();
  }

  void PathVisitor::Visit_RealNumber(const RealNumber& real)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_double"));
    this->pop();
  }

  void PathVisitor::Visit_ShortInteger(const ShortInteger&)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_short"));
    this->pop();
  }

  void PathVisitor::Visit_LongInteger(const LongInteger&)
  {
    this->push();
    DOMElement* type = this->doc_->createElement (XStr ("type"));
    this->curr_->appendChild (type);
    this->curr_ = type;
    this->curr_->appendChild (this->createSimpleContent ("kind",
                                                         "tk_long"));
    this->pop();
  }

  void PathVisitor::Visit_ArtifactDeployRequirement(const ArtifactDeployRequirement&)
  {}

  void PathVisitor::Visit_ArtifactInfoProperty(const ArtifactInfoProperty&)
  {}

  void PathVisitor::Visit_ImplementationRequirement(const ImplementationRequirement&)
  {}

  void PathVisitor::Visit_TopLevelPackages(const TopLevelPackages& tp)
  {
    std::set<TopLevelPackageContainer>
      tpcs = tp.TopLevelPackageContainer_kind_children();
    for (std::set<TopLevelPackageContainer>::iterator iter = tpcs.begin();
         iter != tpcs.end();
         ++iter)
      {
        TopLevelPackageContainer tpc = *iter;
        tpc.Accept (*this);
      }
  }


  void PathVisitor::Visit_TopLevelPackageContainer(const TopLevelPackageContainer& tpc)
  {
    std::set<TopLevelPackage> tps = tpc.TopLevelPackage_kind_children();
    for (std::set<TopLevelPackage>::iterator iter = tps.begin();
         iter != tps.end();
         ++iter)
      {
        TopLevelPackage tp = *iter;
        tp.Accept (*this);
      }
  }

  void PathVisitor::Visit_TopLevelPackage(const TopLevelPackage& tp)
  {
    this->push();
    std::string name = "\\package.tpd";
    this->initTarget (name);
    this->initDocument ("Deployment:TopLevelPackageDescription");
    this->initRootAttributes();

    package pkg = tp.dstpackage();
    pkg.Accept (*this);
    this->dumpDocument();
    this->pop();
  }

  void PathVisitor::Visit_package(const package& pkg)
  {
    PackageConfigurationReference pcr = pkg.dstpackage_end();
    pcr.Accept (*this);
  }

  void PathVisitor::Visit_PackageConfigurationReference(const PackageConfigurationReference& pcr)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("package"));
    const PackageConfiguration pc = pcr.ref();
    std::string refName (pc.name());
    refName += ".pcd";
    ele->setAttribute (XStr ("href"), XStr (refName));
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PathVisitor::Visit_PackageConfigurations(const PackageConfigurations& pcs)
  {
    std::set<PackageConfigurationContainer>
      pccs = pcs.PackageConfigurationContainer_kind_children();
    for (std::set<PackageConfigurationContainer>::iterator iter = pccs.begin();
         iter != pccs.end();
         ++iter)
      {
        PackageConfigurationContainer pcc = *iter;
        pcc.Accept (*this);
      }
  }

  void PathVisitor::Visit_PackageConfigurationContainer(const PackageConfigurationContainer& pcc)
  {
    std::set<PackageConfiguration>
      pcs = pcc.PackageConfiguration_kind_children();
    for (std::set<PackageConfiguration>::iterator iter = pcs.begin();
         iter != pcs.end();
         ++iter)
      {
        PackageConfiguration pc = *iter;
        pc.Accept (*this);
      }
  }

  void PathVisitor::Visit_PackageConfiguration(const PackageConfiguration& pc)
  {
    this->push();
    std::string name = "\\";
    name += pc.name();
    name += ".pcd";
    this->initTarget (name);
    this->initDocument ("Deployment:PackageConfiguration");
    this->initRootAttributes();

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

  void PathVisitor::Visit_PackageConfBasePackage(const PackageConfBasePackage& pcbp)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("basePackage"));
    const ComponentPackage pkg = pcbp.dstPackageConfBasePackage_end();
    std::string pkgName (pkg.name());
    pkgName += ".cpd";
    ele->setAttribute (XStr ("href"), XStr (pkgName));
    this->curr_->appendChild (ele);
    this->pop();
  }


  void PathVisitor::Visit_PackageConfReference(const PackageConfReference& pkgconfref)
  {
    ComponentPackageReference pkgref = pkgconfref.dstPackageConfReference_end();
    pkgref.Accept (*this);
  }

  void PathVisitor::Visit_ComponentPackageReference(const ComponentPackageReference& pkgref)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("basePackage"));
    ComponentPackage pkg = pkgref.ref();
    std::string pkgName (pkg.name());
    pkgName += ".cpd";
    ele->setAttribute (XStr ("href"), XStr (pkgName));
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PathVisitor::Visit_PackageConfConfigProperty(const PackageConfConfigProperty&)
  {}

  void PathVisitor::Visit_PackageConfSpecializedConfig(const PackageConfSpecializedConfig&)
  {}

  void PathVisitor::Visit_PackageConfSelectRequirement(const PackageConfSelectRequirement&)
  {}


  void PathVisitor::Visit_ComponentPackages(const ComponentPackages& cps)
  {
    std::set<PackageContainer> pcs = cps.PackageContainer_kind_children();
    for (std::set<PackageContainer>::iterator iter = pcs.begin();
         iter != pcs.end();
         ++iter)
      {
        PackageContainer pc = *iter;
        pc.Accept (*this);
      }
  }

  void PathVisitor::Visit_PackageContainer(const PackageContainer& pc)
  {
    std::set<ComponentPackage> cps = pc.ComponentPackage_kind_children();
    for (std::set<ComponentPackage>::iterator iter = cps.begin();
         iter != cps.end();
         ++iter)
      {
        ComponentPackage cp = *iter;
        cp.Accept (*this);
      }
  }

  void PathVisitor::Visit_ComponentPackage(const ComponentPackage& cp)
  {
    this->push();
    std::string name = "\\";
    name += cp.name();
    name += ".cpd";
    this->initTarget (name);
    this->initDocument ("Deployment:ComponentPackageDescription");
    this->initRootAttributes();

    std::string label = cp.label();
    if (!label.empty())
      this->curr_->appendChild (this->createSimpleContent ("label",
                                                           label));
    std::string uuid = cp.UUID();
    if (uuid.empty())
      uuid = cp.getPath ("_", false, true, "name", true);
    this->curr_->appendChild (this->createSimpleContent ("UUID", uuid));

    PackageInterface pi = cp.dstPackageInterface();
    if (pi != Udm::null)
      {
        const ComponentRef cref = pi.dstPackageInterface_end();
        const Component comp  = cref.ref();
        std::string refName (comp.name());
        this->interfaces_[refName] = cp.name();
        pi.Accept (*this);
      }

    std::set<Implementation> impls = cp.dstImplementation();
    for (std::set<Implementation>::const_iterator it = impls.begin();
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

  void PathVisitor::Visit_PackageInterface(const PackageInterface& pi)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("realizes"));
    const ComponentRef cref = pi.dstPackageInterface_end();
    const Component comp  = cref.ref();
    std::string refName (comp.name());
    refName += ".ccd";
    ele->setAttribute (XStr ("href"), XStr (refName));
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PathVisitor::Visit_Implementation(const Implementation& impl)
  {
    ComponentImplementationReference cir = impl.dstImplementation_end();
    cir.Accept (*this);
  }

  void PathVisitor::Visit_ComponentImplementationReference(const ComponentImplementationReference& cir)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("implementation"));
    ele->appendChild (this->createSimpleContent ("name", cir.name()));
    const ComponentImplementation ref = cir.ref();
    std::string refName (ref.name());
    refName += ".cid";
    DOMElement*
      refEle = this->doc_->createElement (XStr ("referencedImplementation"));
    refEle->setAttribute (XStr ("href"), XStr (refName));
    ele->appendChild (refEle);
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PathVisitor::Visit_PackageConfigProperty(const PackageConfigProperty&)
  {}

  void PathVisitor::Visit_PackageInfoProperty(const PackageInfoProperty&)
  {}

  void PathVisitor::Visit_ComponentTypes(const ComponentTypes& cts)
  {
    std::set<ComponentContainer> ccs = cts.ComponentContainer_kind_children();
    for (std::set<ComponentContainer>::iterator iter = ccs.begin();
         iter != ccs.end();
         ++iter)
      {
        ComponentContainer cc = *iter;
        cc.Accept (*this);
      }
  }

  void PathVisitor::Visit_ComponentContainer(const ComponentContainer& cc)
  {
    std::set<ComponentRef> cts = cc.ComponentRef_kind_children();
    for (std::set<ComponentRef>::iterator iter = cts.begin();
         iter != cts.end();
         ++iter)
      {
        ComponentRef ct = *iter;
        ct.Accept (*this);
      }
  }

  void PathVisitor::Visit_ComponentRef(const ComponentRef& ct)
  {
    Component comp = ct.ref();
    comp.Accept (*this);
  }

  void PathVisitor::Visit_Component(const Component& comp)
  {
    this->push();
    std::string name = "\\";
    name += comp.name();
    name += ".ccd";
    this->initTarget (name);
    this->initDocument ("Deployment:ComponentInterfaceDescription");
    this->initRootAttributes();

    std::string label = comp.label();
    if (!label.empty())
      this->curr_->appendChild (this->createSimpleContent ("label",
                                                           label));
    std::string uuid = comp.UUID();
    if (uuid.empty())
      uuid = comp.getPath ("_", false, true, "name", true);
    this->curr_->appendChild (this->createSimpleContent ("UUID", uuid));

    const std::set<OutEventPort> oeps = comp.OutEventPort_kind_children();
    for (std::set<OutEventPort>::const_iterator it1 = oeps.begin();
         it1 != oeps.end();
         ++it1)
      {
        OutEventPort oep = *it1;
        oep.Accept (*this);
      }

    const std::set<InEventPort> ieps = comp.InEventPort_kind_children();
    for (std::set<InEventPort>::const_iterator it2 = ieps.begin();
         it2 != ieps.end();
         ++it2)
      {
        InEventPort iep = *it2;
        iep.Accept (*this);
      }

    const std::set<ProvidedRequestPort>
      facets = comp.ProvidedRequestPort_kind_children();
    for (std::set<ProvidedRequestPort>::const_iterator it3 = facets.begin();
         it3 != facets.end();
         ++it3)
      {
        ProvidedRequestPort facet = *it3;
        facet.Accept (*this);
      }

    const std::set<RequiredRequestPort>
      receps = comp.RequiredRequestPort_kind_children();
    for (std::set<RequiredRequestPort>::const_iterator it4 = receps.begin();
         it4 != receps.end();
         ++it4)
      {
        RequiredRequestPort recep = *it4;
        recep.Accept (*this);
      }

    // Dump out an ComponentInterfaceDescription file
    this->dumpDocument();
    this->pop();
  }

  void PathVisitor::Visit_OutEventPort(const OutEventPort& oep)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("port"));
    ele->appendChild (this->createSimpleContent ("name", oep.name()));
    ele->appendChild (this->createSimpleContent ("exclusiveProvider",
                                                 oep.exclusiveProvider() ? "true" : "false"));
    ele->appendChild (this->createSimpleContent ("exclusiveUser",
                                                 oep.exclusiveUser() ? "true" : "false"));
    ele->appendChild (this->createSimpleContent ("optional",
                                                 oep.optional() ? "true" : "false"));
    ele->appendChild (this->createSimpleContent ("provider", "true"));

    // Check whether it is a RT_Event_Channel out port
    std::string out_port_type = oep.out_event_port_type ();
    if (oep.exclusiveProvider())
    {
      if (out_port_type == "DirectConnect")
        ele->appendChild (this->createSimpleContent ("kind", "EventEmitter"));
      else // must be RT Event Channel
        ele->appendChild (this->createSimpleContent ("kind", "rtecEventEmitter"));
    }
    else
    {
      if (out_port_type == "DirectConnect")
        ele->appendChild (this->createSimpleContent ("kind", "EventPublisher"));
      else // must be RT Event Channel
        ele->appendChild (this->createSimpleContent ("kind", "rtecEventPublisher"));
    }

    this->curr_->appendChild (ele);
    this->pop();
  }

  void PathVisitor::Visit_InEventPort(const InEventPort& iep)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("port"));
    ele->appendChild (this->createSimpleContent ("name", iep.name()));
    ele->appendChild (this->createSimpleContent ("exclusiveProvider",
                                                 iep.exclusiveProvider() ? "true" : "false"));
    ele->appendChild (this->createSimpleContent ("exclusiveUser",
                                                 iep.exclusiveUser() ? "true" : "false"));
    ele->appendChild (this->createSimpleContent ("optional",
                                                 iep.optional() ? "true" : "false"));
    ele->appendChild (this->createSimpleContent ("provider", "false"));
    ele->appendChild (this->createSimpleContent ("kind", "EventConsumer"));
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PathVisitor::Visit_ProvidedRequestPort(const ProvidedRequestPort& facet)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("port"));
    ele->appendChild (this->createSimpleContent ("name", facet.name()));
    ele->appendChild (this->createSimpleContent ("exclusiveProvider",
                                                 facet.exclusiveProvider() ? "true" : "false"));
    ele->appendChild (this->createSimpleContent ("exclusiveUser",
                                                 facet.exclusiveUser() ? "true" : "false"));
    ele->appendChild (this->createSimpleContent ("optional",
                                                 facet.optional() ? "true" : "false"));
    ele->appendChild (this->createSimpleContent ("provider", "true"));
    ele->appendChild (this->createSimpleContent ("kind", "Facet"));
    this->curr_->appendChild (ele);
    this->pop();
  }


  void PathVisitor::Visit_RequiredRequestPort(const RequiredRequestPort& recep)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("port"));
    ele->appendChild (this->createSimpleContent ("name", recep.name()));
    ele->appendChild (this->createSimpleContent ("exclusiveProvider",
                                                 recep.exclusiveProvider() ? "true" : "false"));
    ele->appendChild (this->createSimpleContent ("exclusiveUser",
                                                 recep.exclusiveUser() ? "true" : "false"));
    ele->appendChild (this->createSimpleContent ("optional",
                                                 recep.optional() ? "true" : "false"));
    ele->appendChild (this->createSimpleContent ("provider", "false"));
    ele->appendChild (this->createSimpleContent ("kind",
                                                 recep.multiple_connections() ? "MultiplexReceptacle" : "SimplexReceptacle"));
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PathVisitor::Visit_ComponentPropertyDescription(const ComponentPropertyDescription&)
  {}

  void PathVisitor::Visit_ComponentProperty(const ComponentProperty&)
  {}

  void PathVisitor::Visit_ComponentInfoProperty(const ComponentInfoProperty&)
  {}

  void PathVisitor::Visit_ComponentConfigProperty(const ComponentConfigProperty&)
  {}

  void PathVisitor::Visit_Supports(const Supports&)
  {}

  void PathVisitor::Visit_Object(const Object&)
  {}

  void PathVisitor::Visit_ComponentImplementations(const ComponentImplementations& cimpls)
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

  void PathVisitor::Visit_ComponentImplementationContainer(const ComponentImplementationContainer& cic)
  {
    std::set<MonolithicImplementation>
      mimpls = cic.MonolithicImplementation_kind_children();
  //  I dont want to visit Monolithic implementation ....
    for (std::set<MonolithicImplementation>::iterator iter = mimpls.begin();
         iter != mimpls.end();
         ++iter)
      {
        MonolithicImplementation mimpl = *iter;
        mimpl.Accept (*this);
      }


    std::set<ComponentAssembly> asms = cic.ComponentAssembly_kind_children();
    for (std::set<ComponentAssembly>::iterator it = asms.begin();
         it != asms.end();
         ++it)
      {
        ComponentAssembly assembly = *it;
        assembly.Accept (*this);
      }
  }

  void PathVisitor::Visit_MonolithicImplementation(const MonolithicImplementation& mimpl)
  {
    std::string name = mimpl.name();

    /*
    this->push();
    std::string name = this->outputPath_ + "\\";
    name += mimpl.name();
    name += ".cid";
    this->initTarget (name);
    this->initDocument ("Deployment:ComponentImplementationDescription");
    this->initRootAttributes();

    std::string label = mimpl.label();
    if (!label.empty())
      this->curr_->appendChild (this->createSimpleContent ("label",
                                                           label));
    std::string uuid = mimpl.UUID();
    if (uuid.empty())
      uuid = mimpl.getPath ("_", false, true, "name", true);
    this->curr_->appendChild (this->createSimpleContent ("UUID", uuid));

    Implements iface = mimpl.dstImplements();
    iface.Accept (*this);

    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("monolithicImpl"));
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
    this->pop();

    const std::set<ConfigProperty>
      cps = mimpl.dstConfigProperty();
    for (std::set<ConfigProperty>::const_iterator it2 = cps.begin();
         it2 != cps.end();
         ++it2)
      {
        ConfigProperty cp = *it2;
        cp.Accept (*this);
      }

    // Dump out an ComponentImplementationDescription file
    this->dumpDocument();
    this->pop();
  */
  }

  void PathVisitor::Visit_Implements(const Implements& impl)
  {
    this->push();
    const ComponentRef iface = impl.dstImplements_end();
    const Component ref = iface.ref();
    std::string refName (ref.name());
    refName += ".ccd";
    DOMElement*
      refEle = this->doc_->createElement (XStr ("implements"));
    refEle->setAttribute (XStr ("href"), XStr (refName));
    this->curr_->appendChild (refEle);
    this->pop();
  }

  void PathVisitor::Visit_MonolithprimaryArtifact(const MonolithprimaryArtifact& mpa)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("primaryArtifact"));
    const ImplementationArtifactReference iaref = mpa.dstMonolithprimaryArtifact_end();
    const ImplementationArtifact ref = iaref.ref();
    ele->appendChild (this->createSimpleContent ("name", ref.name()));
    std::string refName (ref.name());
    refName += ".iad";
    DOMElement*
      refEle = this->doc_->createElement (XStr ("referencedArtifact"));
    refEle->setAttribute (XStr ("href"), XStr (refName));
    ele->appendChild (refEle);
    this->curr_->appendChild (ele);
    this->pop();
  }

  void PathVisitor::Visit_ConfigProperty(const ConfigProperty& cp)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("configProperty"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    Property ref = cp.dstConfigProperty_end();
    ref.Accept (*this);
    this->pop();
  }

  void PathVisitor::Visit_AssemblyConfigProperty(const AssemblyConfigProperty& acp)
  {
    this->push();
    DOMElement* ele = this->doc_->createElement (XStr ("configProperty"));
    this->curr_->appendChild (ele);
    this->curr_ = ele;
    Property ref = acp.dstAssemblyConfigProperty_end();
    ref.Accept (*this);
    this->pop();
  }


  void PathVisitor::Visit_ComponentAssembly(const ComponentAssembly& assembly)
  {
    std::string name = assembly.name ();

  // get the underlying compoenents ...
  std::set<Component> components =
    assembly.Component_kind_children ();

    // Collect all the Components of this assembly into a set.
    //std::set<Component> comps = assembly.Component_kind_children();
    comps = assembly.Component_kind_children();

    // Collect all the immediate ComponentAssembly children of this assembly
    std::vector<ComponentAssembly>
      subasms = assembly.ComponentAssembly_kind_children();

    // Maintain a list of all ComponentAssemblies in this assembly
    std::vector<ComponentAssembly> assemblies;

    // Put ourselves in the global list first.
    assemblies.push_back (assembly);

    // Do a Depth-First search and collect all the ComponentAssembly,
    // Component children of this assembly, and add them to the
    // assembly-specific list.
    while (!subasms.empty())
      {
        ComponentAssembly rassembly = subasms.back();
        std::string rname = rassembly.name ();
        // Put the first assembly from the current list to the
        // assembly-specific list.
        assemblies.push_back (rassembly);

        subasms.pop_back();

        // Get the components of the current assembly, and insert them into
        // the component list
        std::set<Component> rcomps = rassembly.Component_kind_children();
        comps.insert (rcomps.begin(), rcomps.end());

        // Get the subassemblies of the first assembly.
        std::vector<ComponentAssembly>
          rasms = rassembly.ComponentAssembly_kind_children();

        // Insert them to the current list.
        std::copy (rasms.begin(), rasms.end(), std::back_inserter (subasms));
      }

    // Create the appropriate component attribute value mappings

  // Get the Component Ports .....
      int number =0;
      std::set<Component>::iterator iter;
      for (iter = comps.begin ();
            iter != comps.end ();
            iter++)
      {
        //Component* comp_ptr = ;
        std::string comp_name = iter->name ();
        std::set<InEventPort> in_event_ports
          = iter->InEventPort_kind_children ();

        std::set<OutEventPort> out_event_ports
          = iter->OutEventPort_kind_children ();

        std::set<RequiredRequestPort> reqd_request_port
          = iter->RequiredRequestPort_kind_children ();

        number++;
        // for all in-event ports do this
        std::set<InEventPort>::iterator in_event_port_iter;
        for (in_event_port_iter = in_event_ports.begin ();
              in_event_port_iter != in_event_ports.end ();
              in_event_port_iter++)
        {
          // now iterate for the out -event port
          std::set<OutEventPort>::iterator out_event_port_iter;
          for (out_event_port_iter = out_event_ports.begin ();
                out_event_port_iter != out_event_ports.end ();
              out_event_port_iter++)
          {
             std::string start_vertex = iter->name ();
            start_vertex += + "_";
            start_vertex += in_event_port_iter->name ();

            std::string end_vertex;
            end_vertex += iter->name ();
            end_vertex += "_";
            end_vertex += out_event_port_iter->name ();

            std::string connection
                = start_vertex + "-" + end_vertex;

            //Component_Port_Vertex start (*in_event_port_iter, extract_uuid (*iter));

            Component_Port_Vertex start (*in_event_port_iter,
              in_event_port_iter->getPath (".", false, true, "name", true));
            Component_Port_Vertex end (*out_event_port_iter,
              out_event_port_iter->getPath (".", false, true, "name", true));

            add_the_edges (start , end);
          }

          // Here we are connecting the Required Request port
          std::set<RequiredRequestPort>::iterator reqd_request_port_iter;
          for (reqd_request_port_iter = reqd_request_port.begin ();
                reqd_request_port_iter != reqd_request_port.end ();
              reqd_request_port_iter++)
          {
             std::string start_vertex = iter->name ();
            start_vertex += + "_";
            start_vertex += in_event_port_iter->name ();

            std::string end_vertex;
            end_vertex += iter->name ();
            end_vertex += "_";
            end_vertex += reqd_request_port_iter->name ();

            std::string connection
                = start_vertex + "-" + end_vertex;

            //Component_Port_Vertex start (*in_event_port_iter,extract_uuid (*iter));

            //Component_Port_Vertex end (*reqd_request_port_iter, extract_uuid (*iter));

            Component_Port_Vertex start (*in_event_port_iter,
              in_event_port_iter->getPath (".", false, true, "name", true));

            Component_Port_Vertex end (*reqd_request_port_iter,
              reqd_request_port_iter->getPath (".", false, true, "name", true));

            add_the_edges (start , end);
          }

        } //  for in-event Ports .....


        std::set<ProvidedRequestPort> prov_request_port
          = iter->ProvidedRequestPort_kind_children ();
        // for all provided-request ports do this
        std::set<ProvidedRequestPort>::iterator prov_request_port_iter;
        for (prov_request_port_iter = prov_request_port.begin ();
              prov_request_port_iter != prov_request_port.end ();
              prov_request_port_iter++)
        {
          // now iterate for the out -event port
          std::set<OutEventPort>::iterator out_event_port_iter;
          for (out_event_port_iter = out_event_ports.begin ();
                out_event_port_iter != out_event_ports.end ();
              out_event_port_iter++)
          {
             std::string start_vertex = iter->name ();
            start_vertex += + "_";
            start_vertex += prov_request_port_iter->name ();

            std::string end_vertex;
            end_vertex += iter->name ();
            end_vertex += "_";
            end_vertex += out_event_port_iter->name ();

            std::string connection
                = start_vertex + "-" + end_vertex;
/*
            Component_Port_Vertex start (*prov_request_port_iter, extract_uuid (*iter));

            Component_Port_Vertex end (*out_event_port_iter, extract_uuid (*iter));
*/
            Component_Port_Vertex start (*prov_request_port_iter,
              prov_request_port_iter->getPath (".", false, true, "name", true));

            Component_Port_Vertex end (*out_event_port_iter,
              out_event_port_iter->getPath (".", false, true, "name", true));

            add_the_edges (start , end);
          }

          // Here we are connecting the Required Request port
          std::set<RequiredRequestPort>::iterator reqd_request_port_iter;
          for (reqd_request_port_iter = reqd_request_port.begin ();
                reqd_request_port_iter != reqd_request_port.end ();
              reqd_request_port_iter++)
          {
             std::string start_vertex = iter->name ();
            start_vertex += + "_";
            start_vertex += prov_request_port_iter->name ();

            std::string end_vertex;
            end_vertex += iter->name ();
            end_vertex += "_";
            end_vertex += reqd_request_port_iter->name ();

            std::string connection
                = start_vertex + "-" + end_vertex;

/*            Component_Port_Vertex start (*prov_request_port_iter, extract_uuid (*iter));

            Component_Port_Vertex end (*reqd_request_port_iter, extract_uuid (*iter));
*/
            Component_Port_Vertex start (*prov_request_port_iter,
              prov_request_port_iter->getPath (".", false, true, "name", true));

            Component_Port_Vertex end (*reqd_request_port_iter,
              reqd_request_port_iter->getPath (".", false, true, "name", true));

            add_the_edges (start , end);
          }

        } // Provided Request Port ....


      } // for all Components ......

  // ....Component Ports ..................

    this->CreateAttributeMappings (assemblies);
    //this->CreateAssemblyInstances (comps);
    this->CreateAssemblyConnections (assemblies);
    this->pop();

    // Dump out an ComponentImplementationDescription file
  //  this->dumpDocument();
    this->pop();

  }

  void PathVisitor::add_the_edges (Component_Port_Vertex start_vertex,
                                   Component_Port_Vertex end_vertex)
  {

    /// find the nodes and add the edges ...
    int start_index;
    std::map<std::string,int>::iterator iter
      = graph_vertex_.find (start_vertex.vertex_name_);
    // get the start vertwx of the edge

    if (iter == graph_vertex_.end ())
    {
      // The vertex is found new ...
      start_index = vertex_count_;
      graph_vertex_indices_[vertex_count_]
        = start_vertex;
      graph_vertex_[start_vertex.vertex_name_]
        = vertex_count_;
      vertex_count_++;
    }
    else
    {
      // This vertex was already found
      start_index = iter->second;
    }

    // get the end vertex of the edge
    int end_index ;
    iter = graph_vertex_.find (end_vertex.vertex_name_);
    if (iter == graph_vertex_.end ())
    {
      // The vertex is found new ...
      end_index = vertex_count_;
      graph_vertex_indices_[vertex_count_]
      = end_vertex;
      graph_vertex_[end_vertex.vertex_name_]
        = vertex_count_;
      vertex_count_++;
    }
    else
    {
      // This vertex was already found
      end_index = iter->second;
    }

    Edge_Numbers new_edge;
    new_edge.source_ = start_index;
    new_edge.target_ = end_index;

    all_edges.push_back (new_edge);

  }

  void PathVisitor::CreateAttributeMappings (std::vector<ComponentAssembly>& assemblies)
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

  void PathVisitor::Visit_AttributeMapping(const AttributeMapping& mapping)
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

  void PathVisitor::GetAttributeComponents (const AttributeMapping& mapping,
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
            std::string mapPath = mapping.getPath ("_", false, true, "name", true);

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
            std::string compName = parent.getPath ("_", false, true, "name", true);
            output.insert (make_pair (compName, attr.name()));
          }
      }
  }

  void PathVisitor::CreateAssemblyInstances (std::set<Component>& comps)
  {
    for (std::set<Component>::iterator iter = comps.begin();
         iter != comps.end();
         ++iter)
      {
        Component comp = *iter;
        //DOMElement* instance = this->doc_->createElement (XStr ("instance"));
        //this->curr_->appendChild (instance);
        //this->push();
        //this->curr_ = instance;
        std::string uniqueName = comp.getPath ("_", false, true, "name", true);
        //instance->setAttribute (XStr ("xmi:id"), XStr (uniqueName));
        //instance->appendChild (this->createSimpleContent ("name",
//                                                          uniqueName));
        Component typeParent;
        if (comp.isInstance())
          {
           typeParent = comp.Archetype();
            while (typeParent.isInstance())
              typeParent = typeParent.Archetype();
          }
        std::string interfaceName = typeParent.name();
        //std::string refName = this->interfaces_[interfaceName];
        //refName += ".cpd";
        //DOMElement* refEle = this->doc_->createElement (XStr ("package"));
        //refEle->setAttribute (XStr ("href"), XStr (refName));
        //instance->appendChild (refEle);
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
        for (std::map<std::pair<std::string, std::string>, Property>::const_iterator iter = this->attrValues_.begin();
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
        this->CreatePropertyElement (compAttr.second, val);
                this->pop();
              }
          }
        this->pop();
      }
  }

  void PathVisitor::Visit_ReadonlyAttribute(const ReadonlyAttribute& attr)
  {
    AttributeValue attValue = attr.dstAttributeValue();
    if (attValue != Udm::null)
      attValue.Accept (*this);
  }

  void PathVisitor::Visit_AttributeValue(const AttributeValue& value)
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

  void PathVisitor::Visit_AttributeDelegate(const AttributeDelegate&){}
  void PathVisitor::Visit_AttributeMappingValue(const AttributeMappingValue&){}
  void PathVisitor::Visit_AttributeMappingDelegate(const AttributeMappingDelegate&){}

  void PathVisitor::CreateAssemblyConnections (std::vector<ComponentAssembly>& assemblies)
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
        for (std::set<PublishConnector>::const_iterator iter = connectors.begin();
             iter != connectors.end();
             ++iter)
          {
            PublishConnector conn = *iter;
            conn.Accept (*this);
          }
        this->publishers_.clear();
        this->consumers_.clear();
      }

    typedef std::vector< Vertex > container;
    container c;

    outf.open ("topo.txt");
    for ( container::reverse_iterator ii=sorted_vertices.rbegin();
          ii!=sorted_vertices.rend();
          ++ii)

    outf << graph_vertex_indices_[*ii].vertex_name_ << " " << endl;
    outf << endl;
    outf.close ();

  }

  template <typename T, typename Del, typename DelRet, typename DelEndRet>
  void PathVisitor::GetComponents (const T& port,
                                      DelRet (T::*srcDel)() const,
                                      DelRet (T::*dstDel) () const,
                                      DelEndRet (Del::*srcDelEnd)() const,
                                      DelEndRet (Del::*dstDelEnd)() const,
                                      Component_Port& output,
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
              this->GetComponents(srcPort, srcDel, dstDel,
                                  srcDelEnd, dstDelEnd, output, visited);
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
              this->GetComponents(dstPort, srcDel, dstDel,
                                  srcDelEnd, dstDelEnd, output, visited);
          }
      }
    else if (Udm::IsDerivedFrom (par.type(), Component::meta))
      {
        Component recep_comp = Component::Cast (par);
        output.insert (make_pair (recep_comp, port));
      }
    visited.erase (port);
    return;
  }



  void PathVisitor::GetReceptacleComponents (const RequiredRequestPort& receptacle,
                                                Component_Port& output)
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

  void PathVisitor::GetFacetComponents (const ProvidedRequestPort& facet,
                                           Component_Port& output)
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

  void PathVisitor::GetEventSinkComponents (const InEventPort& consumer,
                                               Component_Port& output)
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

    void PathVisitor::GetEventSourceComponents (const OutEventPort& publisher,
                                                   Component_Port& output)
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

  void PathVisitor::CreateConnections (const Component_Port& src,
                                          const Component_Port& dst)
  {
    for (Component_Port::const_iterator iter = src.begin();
         iter != src.end();
         ++iter)
      {
        Component srcComp = iter->first;
        Port srcPort = iter->second;
        for (Component_Port::const_iterator iter = dst.begin();
             iter != dst.end();
             ++iter)
          {
            Component dstComp = iter->first;
            Port dstPort = iter->second;
            this->CreateConnection (srcComp, srcPort, dstComp, dstPort);
          }
      }
  }

  void PathVisitor::CreateConnection (const Component& srcComp,
                                      const Port& srcPort,
                                      const Component& dstComp,
                                      const Port& dstPort)
  {

    Component_Port_Vertex start (srcPort);

    int start_index;
    std::map<std::string,int>::iterator iter
      = graph_vertex_.find (start.vertex_name_);
    // get the start vertwx of the edge
    if (iter == graph_vertex_.end ())
    {
      start_index = vertex_count_;
      graph_vertex_indices_[vertex_count_]
        = start;
      graph_vertex_[start.vertex_name_]
        = vertex_count_;
      vertex_count_++;
    }
    else
    {
      start_index = iter->second;
    }

    Component_Port_Vertex end (dstPort) ;

    // get the end vertex of the edge
    int end_index ;
    iter = graph_vertex_.find (end.vertex_name_);
    if (iter == graph_vertex_.end ())
    {

      //end.vertex_name_ = end_vertex;
      //end.the_component_ = dstComp;

      end_index = vertex_count_;
      graph_vertex_indices_[vertex_count_]
        = end;
      graph_vertex_[end.vertex_name_]
        = vertex_count_;
      vertex_count_++;

    }
    else
    {
      end_index = iter->second;
    }

    Edge_Numbers new_edge;
    new_edge.source_ = start_index;
    new_edge.target_ = end_index;

    all_edges.push_back (new_edge);

  }

  void PathVisitor::Visit_invoke(const invoke& iv)
  {
    // Get the receptacle end
    RequiredRequestPort receptacle = iv.srcinvoke_end();

    // Get the facet end
    ProvidedRequestPort facet = iv.dstinvoke_end();

    Component_Port receptacles;
    Component_Port facets;
    this->GetReceptacleComponents (receptacle, receptacles);
    this->GetFacetComponents (facet, facets);
    this->CreateConnections (receptacles, facets);
  }

  //void PathVisitor::Visit_invoke(const invoke& iv)
  //{
  //  // Get the receptacle end
  //  RequiredRequestPort receptacle = iv.srcinvoke_end();

  //  // Get the facet end
  //  ProvidedRequestPort facet = iv.dstinvoke_end();

  //  Udm::Object parent = receptacle.parent();
  //  Component source_component = Component::Cast (parent);
  //  parent = facet.parent ();
  //  Component target_component = Component::Cast (parent);

  //  std::string start_vertex = source_component.name ();
  //  start_vertex += + "_";
   // start_vertex += receptacle.name ();

   // std::string end_vertex;
   // end_vertex += target_component.name ();
   // end_vertex += "_";
   // end_vertex += facet.name ();

   // std::string connection
    //    = start_vertex + "-" + end_vertex;

  //  Component_Port_Vertex start;
  //  start.vertex_name_ = start_vertex;
  //  start.the_component_ = source_component;
  //  start.the_port_ = receptacle;

  //  Component_Port_Vertex end;
  //  end.vertex_name_ = end_vertex;
  //  end.the_component_ = target_component;
  //  end.the_port_ = facet;

  //  add_the_edges (start , end);
  //}
/*
  void PathVisitor::Visit_emit(const emit& ev)
  {
    // Get the emitter end
    OutEventPort emitter = ev.srcemit_end();

    // Get the consumer end
    InEventPort consumer = ev.dstemit_end();

    std::map<Component,std::string> emitters;
    std::map<Component,std::string> consumers;
    this->GetEventSourceComponents (emitter, emitters);
    this->GetEventSinkComponents (consumer, consumers);
    this->CreateConnections (emitters, consumers);
  }
*/
  void PathVisitor::Visit_emit(const emit& ev)
  {
    // Get the emitter end
    OutEventPort emitter = ev.srcemit_end();

    // Get the consumer end
    InEventPort consumer = ev.dstemit_end();

    Udm::Object parent = emitter.parent();
    Component source_component = Component::Cast (parent);
    parent = consumer.parent ();
    Component target_component = Component::Cast (parent);

    std::string start_vertex = source_component.name ();
    start_vertex += + "_";
    start_vertex += emitter.name ();

    std::string end_vertex;
    end_vertex += target_component.name ();
    end_vertex += "_";
    end_vertex += consumer.name ();

    std::string connection
        = start_vertex + "-" + end_vertex;

    /*
    Component_Port_Vertex start (emitter , source_component.UUID ());

    Component_Port_Vertex end (consumer , target_component.UUID ());
*/
    Component_Port_Vertex start (emitter ,
      emitter.getPath (".", false, true, "name", true));

    Component_Port_Vertex end (consumer ,
      consumer.getPath (".", false, true, "name", true));

    add_the_edges (start , end);
  }

  void PathVisitor::Visit_publish(const publish& ev)
  {
    // Get the publisher end
    const OutEventPort publisher = ev.srcpublish_end();

    // Get the connector end
    const PublishConnector connector = ev.dstpublish_end();

    // Create an entry in the publishers_ map
    this->publishers_[std::string (connector.name())] = publisher;
  }

  void PathVisitor::Visit_deliverTo(const deliverTo& dv)
  {
    // Get the connector end
    const  PublishConnector connector = dv.srcdeliverTo_end();

    // Get the consumer end
    const InEventPort consumer = dv.dstdeliverTo_end();

    // Create an entry in the consumers_ map
    this->consumers_.insert (make_pair (connector.name(), consumer));
  }

  void PathVisitor::Visit_PublishConnector(const PublishConnector& pubctor)
  {
    std::string ctor = pubctor.name();

    // Get Publisher
    OutEventPort publisher = this->publishers_[ctor];
    Component_Port publishers;
    this->GetEventSourceComponents (publisher, publishers);

    for (Component_Port::const_iterator
           iter = publishers.begin();
         iter != publishers.end();
         ++iter)
      {
        Component srcComp = iter->first;
        Port srcPort = iter->second;

        for (std::multimap<std::string, InEventPort>::const_iterator
               iter = this->consumers_.lower_bound (ctor);
             iter != this->consumers_.upper_bound (ctor);
             ++iter)
          {
            // Get Consumer
            InEventPort consumer = iter->second;
            Component_Port consumers;
            this->GetEventSinkComponents (consumer, consumers);
            for (Component_Port::const_iterator
                   iter = consumers.begin();
                 iter != consumers.end();
                 ++iter)
              {
                Component dstComp = iter->first;
                Port dstPort = iter->second;
                this->CreateConnection (srcComp, srcPort, dstComp,
                                        dstPort);
              }
          }
      }
  }

  void PathVisitor::Visit_MonolithExecParameter(const MonolithExecParameter&)
  {}

  void PathVisitor::Visit_Requirement(const Requirement&)
  {}

  void PathVisitor::Visit_SatisfierProperty(const SatisfierProperty&)
  {}

  void PathVisitor::Visit_ImplementationDependency(const ImplementationDependency&)
  {}

  void PathVisitor::Visit_Capability(const Capability&)
  {}

  void PathVisitor::Visit_AssemblyselectRequirement(const AssemblyselectRequirement&)
  {}


  void PathVisitor::Visit_AssemblyDeployRequirement(const AssemblyDeployRequirement&)
  {}


  void PathVisitor::Visit_InfoProperty(const InfoProperty&)
  {}


  void PathVisitor::Visit_MonolithDeployRequirement(const MonolithDeployRequirement&)
  {}


  void PathVisitor::Visit_ImplementationDependsOn(const ImplementationDependsOn&)
  {}


  void PathVisitor::Visit_ImplementationCapability(const ImplementationCapability&)
  {}

  std::string PathVisitor::ExtractName(Udm::Object ob)
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

  void PathVisitor::Visit_PathDiagrams(const PathDiagrams& path)
  {
    std::vector < size_type > discover_order(num_vertices (the_graph_));
    CalculatePath (discover_order);
    Path the_path = Path::Create (path);
    the_path.name () = "Path";

    int k;
    int size = discover_order.size ();
    for (k = 0; k < size; ++k)
    {
      //outf << graph_vertex_indices_[discover[k]].vertex_name_ << " ";

      //ComponentRef cmp_ref = ComponentRef::Create (the_path);
      //cmp_ref.ref () =
        //    graph_vertex_indices_[discover_order[k]].the_component_;
      DisplayNode node = DisplayNode::Create (the_path);
      node.name () = graph_vertex_indices_[discover_order[k]].vertex_name_;
      int x_co_ordinate = k*100;
      int y_co_ordinate = 100;
      char buf [100];
      memset (buf , 0 , 100);
      sprintf (buf , "%s:(%d,%d)" , "PathAspect" , x_co_ordinate , y_co_ordinate);
      //str_
      node.position () = buf;
      graph_vertex_indices_[discover_order[k]].node_ = node;
      // Now create the connections ....
      // find the port .....
    }

    // for the component .. find the port ...
    for (std::vector<Edge_Numbers>::size_type i=0;
      i < dfs_edges.size ();
      i++)
    {
      Edge_Numbers edge = dfs_edges[i];

      Edge e = Edge::Create (the_path);
      DstEdge dst_edge = DstEdge::Create (the_path);
      SrcEdge src_edge = SrcEdge::Create (the_path);
      src_edge.srcSrcEdge_end () = e;
      //src_edge.dstSrcEdge_end () = graph_vertex_indices_[edge.target_].the_port_;
      //dst_edge.srcDstEdge_end () = graph_vertex_indices_[edge.source_].the_port_;

      src_edge.dstSrcEdge_end () = graph_vertex_indices_[edge.target_].node_;
      dst_edge.srcDstEdge_end () = graph_vertex_indices_[edge.source_].node_;

      dst_edge.dstDstEdge_end () = e;
    }
  }

  void PathVisitor::CalculatePath (std::vector < size_type >& discover)
  {
  // add the edges to the graph
    std::list <Edge_Numbers>::iterator iter;

    the_graph_.clear ();
    outf.open ("edges.txt");

    for (iter=all_edges.begin ();iter != all_edges.end ();iter++)
    {
      boost::add_edge(iter->source_,
                      iter->target_,
                      the_graph_
                      );

      outf << "source " << iter->source_ << "target " << iter->target_ << endl;
    }

    outf.close ();

    outf.open ("nodes.txt");
    for (std::map<std::string, int>::iterator iter = graph_vertex_.begin ();
      iter != graph_vertex_.end ();iter++)
    {
      outf << "vertex name " << iter->first
        << "vertex id " << iter->second << endl;
    }

    outf.close ();

    dfs_edges.clear ();
    forward_edges_.clear ();
    // here calculate the paths for the corresponding
    // graph ..
  // discover time and finish time properties
    std::vector < size_type > dtime(num_vertices(the_graph_));
    std::vector < size_type > ftime(num_vertices(the_graph_));
    size_type t = 0;
    dfs_time_visitor < size_type * >vis(&dtime[0], &ftime[0], t , this);

    depth_first_search (the_graph_, visitor(vis));

    outf.open ("paths.txt");
    ostream_iterator<int> out (outf , " ");
    for (PathList::iterator iter = this->all_the_paths_.begin ();
         iter != this->all_the_paths_.end ();
         iter++)
    {
      copy (iter->begin (), iter->end (),out);
      outf << "\n" << "Next path " << endl;
    }

    outf.close ();

    typedef size_type* Iiter;

    std::vector < size_type > ordered_vertices (dtime.size ());
    // use std::sort to order the vertices by their discover time
    //std::vector < size_type > discover_order(10);
    boost::integer_range < size_type > r(0, dtime.size ());
    std::copy(r.begin(), r.end(), ordered_vertices.begin());
    std::sort(ordered_vertices.begin(), ordered_vertices.end(),
      boost::indirect_cmp < Iiter, std::less < size_type > >(&dtime[0]));
    std::cout << "order of discovery: ";

    discover = ordered_vertices;

    outf.open ("flow.txt");
    outf << "The first vertice is " << *vertices(the_graph_).first << std::endl;
    int k;
    int size = discover.size ();
    for (k = 0; k < 10; ++k)
      outf << graph_vertex_indices_[discover[k]].vertex_name_ << " " << discover [k] << " ";

    outf.close ();
  }
  void PathVisitor::add_tree_edge (int source , int target)
  {

  }

  void PathVisitor::search_node_in_previous_paths (int node)
  {
    PathList new_paths;
    for (PathList::iterator path_list_iter = all_the_paths_.begin ();
          path_list_iter != all_the_paths_.end ();
          path_list_iter++)
    {
      for (A_Path::iterator path_iter = path_list_iter->begin ();
        path_iter != path_list_iter->end ();
        path_iter++)
      {
        if (node == *path_iter)
        {
          A_Path new_path = current_path_;
          new_path.insert (new_path.end (),
                            path_iter,
                            path_list_iter->end ()
                           );
//          all_the_paths_.push_back (new_path);
          new_paths.push_back (new_path);
          break;
        }

      }// for (A_Path
    }// for PathList
    if (new_paths.size () > 0)
    {
      //here take only unique values of new_path
      //std::sort (new_paths.begin (), new_paths.end ());
      PathList::iterator iter =
        std::unique (new_paths.begin (), new_paths.end ());
      new_paths.erase (iter , new_paths.end ());
      all_the_paths_.insert (all_the_paths_.end (),
                              new_paths.begin (),
                              new_paths.end ()
                             );
    }
  }
  void PathVisitor::draw_path (A_Path* a_path)
  {
    DisplayNode previous_node;
    int k = 0;
    for (A_Path::iterator iter = a_path->begin ();
          iter != a_path->end ();
          iter++,k++)
    {
      DisplayNode node = DisplayNode::Create (the_current_path_flow_);
      std::string name = graph_vertex_indices_[*iter].vertex_name_;
      node.name () = graph_vertex_indices_[*iter].display_label_;

      int x_co_ordinate = 300;
      int y_co_ordinate = (k + 1)*150;
      char buf [100];
      memset (buf , 0 , 100);
      sprintf (buf , "(%d,%d)" , x_co_ordinate , y_co_ordinate);
      //str_
      node.position () = buf;


      if (iter != a_path->begin ())
      {
        Edge e = Edge::Create (the_current_path_flow_);

        y_co_ordinate = static_cast <int> ((k + 0.5) * 150);
        memset (buf , 0 , 100);
        sprintf (buf , "(%d,%d)" , x_co_ordinate , y_co_ordinate);

        e.position () = buf;
        DstEdge dst_edge = DstEdge::Create (the_current_path_flow_);
        SrcEdge src_edge = SrcEdge::Create (the_current_path_flow_);
        src_edge.srcSrcEdge_end () = e;

        src_edge.dstSrcEdge_end () = node;
        dst_edge.srcDstEdge_end () = previous_node;

        dst_edge.dstDstEdge_end () = e;
      } // if
      previous_node = node;
    } // for loop
  }// draw_path

  std::string PathVisitor::extract_uuid (Udm::Object obj)
  {
    if (Udm::IsDerivedFrom (obj.type (), ComponentAssembly::meta))
    {
      ComponentAssembly assembly = ComponentAssembly::Cast (obj);
      return assembly.UUID ();
    }
    else
    {
      Component component = Component::Cast (obj);
      return component.UUID ();
    }
  } // extract_uuid ...

} // PICML namespace
