// $Id$

#ifndef DOMAIN_VISITOR_H
#define DOMAIN_VISITOR_H

#include <memory>
#include <stack>

// Xerces includes
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

// Utility includes
#include "PICML/PICML.h"
#include "Utils/Utils.h"
#include "Utils/XercesString.h"
#include "Target/Domain_Export.h"

namespace PICML
{
  using xercesc::DOMImplementation;
  using xercesc::DOMDocument;
  using xercesc::DOMElement;
  using xercesc::DOMWriter;
  using xercesc::XMLFormatTarget;
  using xercesc::LocalFileFormatTarget;

  class Domain_Export DomainVisitor: public Visitor
  {
  public:
    DomainVisitor (const std::string& outputPath);
    ~DomainVisitor();

    void init();
    void initTarget (const std::string& fileName);
    void initDocument (const std::string& rootName);
    void initRootAttributes();
    void dumpDocument();

    void push();
    void pop();

    DOMElement* createSimpleContent (const std::string& name,
                                     const std::string& value);

    virtual void Visit_RootFolder(const RootFolder&);
    virtual void Visit_Targets(const Targets& targets);
    virtual void Visit_Domain(const Domain& domain);
    virtual void Visit_Node(const Node& node);
    virtual void Visit_Interconnect(const Interconnect& ic);
    virtual void Visit_Bridge(const Bridge& br);
    virtual void Visit_SharedResource(const SharedResource& sr);
    virtual void Visit_InstanceMapping(const InstanceMapping& ins_map);
    virtual void Visit_Resource(const Resource& res);
    virtual void Visit_SatisfierProperty(const SatisfierProperty& property);
    virtual void Visit_DataType(const DataType& type);
    virtual void Visit_Boolean(const Boolean& boolean);
    virtual void Visit_Byte(const Byte& byte);
    virtual void Visit_String(const String& str);
    virtual void Visit_RealNumber(const RealNumber& real);
    virtual void Visit_ShortInteger(const ShortInteger&);
    virtual void Visit_LongInteger(const LongInteger&);

  private:

    DOMImplementation*  impl_;
    DOMDocument*        doc_;
    DOMElement*         root_;
    DOMElement*         curr_;
    DOMWriter*          serializer_;
    XMLFormatTarget*    target_;
    std::string         outputPath_;
    std::stack<DOMElement*> curr_stack_;
  };
}

#endif /* DOMAIN_VISITOR_H */
