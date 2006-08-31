
#ifndef RTConfig_VISITOR_H
#define RTConfig_VISITOR_H

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
#include "PICML/XercesString.h"
#include "PICML/Utils.h"
#include "RTConfig/RTConfig_Export.h"

namespace PICML
{
  using xercesc::DOMImplementation;
  using xercesc::DOMDocument;
  using xercesc::DOMElement;
  using xercesc::DOMWriter;
  using xercesc::XMLFormatTarget;
  using xercesc::LocalFileFormatTarget;

  class RTConfig_Export RTConfigVisitor: public Visitor
  {
  public:
    RTConfigVisitor (const std::string& outputPath);
    ~RTConfigVisitor();

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

    //RTConfig-related elements

    virtual void Visit_RTConfiguration (const RTConfiguration& qc);
    virtual void Visit_RTModel (const RTModel& qm);

    virtual void Visit_ResourceContainer (const ResourceContainer& rc);
    virtual void Visit_resource (const resource& r);
    virtual void Visit_EnvironmentConf (const EnvironmentConf& ec);

    virtual void Visit_BandedConnection (const BandedConnection& bc);
    virtual void Visit_BandedConnectionRef (const BandedConnectionRef& bcr);
    virtual void Visit_ConnectionBand (const ConnectionBand& cb);

    virtual void Visit_ThreadPool (const ThreadPool& tp);
    virtual void Visit_ThreadPoolRef (const ThreadPoolRef& tpr);
    virtual void Visit_SimpleThreadPool (const SimpleThreadPool& stp);
    virtual void Visit_ThreadPoolWithLanes (const ThreadPoolWithLanes& tpwl);
    virtual void Visit_ThreadPoolLane (const ThreadPoolLane& tpl);

    virtual void Visit_PriorityModelPolicy (const PriorityModelPolicy& pmp);
    virtual void Visit_PolicySet (const PolicySet& ps);
    virtual void Visit_PolicySetRef (const PolicySetRef& psr);

  private:

    DOMImplementation*      impl_;
    DOMDocument*            doc_;
    DOMElement*             root_;
    DOMElement*             curr_;
    DOMElement*             resources_;
    DOMWriter*              serializer_;
    XMLFormatTarget*        target_;
    std::string             outputPath_;
    std::stack<DOMElement*> curr_stack_;

    void init();
    void initTarget (const std::string& fileName);
    void initDocument (const std::string& rootName);
    void initRootAttributes();
    void dumpDocument();

    void push();
    void pop();
    std::string ExtractName (Udm::Object obj);
    DOMElement* createSimpleContent (const std::string& name,
                                     const std::string& value);


    //maps to maintain the elements that were added already
    //on a per-file basis. Needed for consistency
    std::map<PICML::SimpleThreadPool, bool> simpletp_;
    std::map<PICML::ThreadPoolWithLanes, bool> tpWithLanes_;
    std::map<PICML::BandedConnection, bool> connections_;

  };
}

#endif /* RTConfig_VISITOR_H */
