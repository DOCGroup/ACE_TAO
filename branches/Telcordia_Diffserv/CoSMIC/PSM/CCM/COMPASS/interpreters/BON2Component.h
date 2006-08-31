//###############################################################################################################################################
//
//	Meta and Builder Object Network V2.0 for GME
//	BON2Component.h
//
//###############################################################################################################################################

/*
  Copyright (c) Vanderbilt University, 2000-2003
  ALL RIGHTS RESERVED

  Vanderbilt University disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall Vanderbilt University be liable for
  any special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#ifndef BON2Component_h
#define BON2Component_h

#include "BON.h"
#include "BONImpl.h"
#include "Extensions.h"
#include <ComponentConfig.h>
#include <stack>

// Xerces includes
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

// Utility includes
#include "XercesString.h"

namespace BON
{

  //###############################################################################################################################################
  //
  // 	C L A S S : BON::Component
  //
  //###############################################################################################################################################

  class Component
  {
    //==============================================================
    // IMPLEMENTOR SPECIFIC PART
    // Insert application specific members and method deifinitions here

    //==============================================================
    // BON2 SPECIFIC PART
    // Do not modify anything below

    // Member variables
  public :
    Project 	m_project;
    bool		m_bIsInteractive;

  public:
    Component();
    ~Component();

  public:
    void initialize( Project& project );
    void finalize( Project& project );
    void invoke( Project& project, const std::set<FCO>& setModels, long lParam );
    void invokeEx( Project& project, FCO& currentFCO, const std::set<FCO>& setSelectedFCOs, long lParam );
    void objectInvokeEx( Project& project, Object& currentObject, const std::set<Object>& setSelectedObjects, long lParam );
    Util::Variant getParameter( const std::string& strName );
    void setParameter( const std::string& strName, const Util::Variant& varValue );

#ifdef GME_ADDON
    void globalEventPerformed( globalevent_enum event );
    void objectEventPerformed( Object& object, unsigned long event, VARIANT v );
#endif
  };

}; // namespace BON

namespace Compass {

  class TopLevelPackageImpl
    : public BON::ModelImpl
  {
  public:
    virtual void accept (BON::Visitor* pVisitor, bool bChildrenFirst = false);
  };

  class ImplementationArtifactImpl
    : public BON::ModelImpl
  {
  public:
    virtual void accept (BON::Visitor* pVisitor, bool bChildrenFirst = false);
  };

  class NamedImplementationArtifactImpl
    : public BON::ModelImpl
  {
  public:
    virtual void accept (BON::Visitor* pVisitor, bool bChildrenFirst = false);
  };

  class RequirementImpl
    : public BON::ModelImpl
  {
  public:
    virtual void accept (BON::Visitor* pVisitor, bool bChildrenFirst = false);
  };

  class ComponentInterfaceImpl
    : public BON::ModelImpl
  {
  public:
    virtual void accept (BON::Visitor* pVisitor, bool bChildrenFirst = false);
  };

  class ComponentPackageImpl
    : public BON::ModelImpl
  {
  public:
    virtual void accept (BON::Visitor* pVisitor, bool bChildrenFirst = false);
  };

  class PackagedComponentImplementationImpl
    : public BON::ModelImpl
  {
  public:
    virtual void accept (BON::Visitor* pVisitor, bool bChildrenFirst = false);
  };

  class ComponentImplementationImpl
    : public BON::ModelImpl
  {
  public:
    virtual void accept (BON::Visitor* pVisitor, bool bChildrenFirst = false);
  };

  class CapabilityImpl
    : public BON::ModelImpl
  {
  public:
    virtual void accept (BON::Visitor* pVisitor, bool bChildrenFirst = false);
  };

  class ComponentAssemblyImpl
    : public BON::ModelImpl
  {
  public:
    virtual void accept (BON::Visitor* pVisitor, bool bChildrenFirst = false);
  };

  class MonolithicImplementationImpl
    : public BON::ModelImpl
  {
  public:
    virtual void accept (BON::Visitor* pVisitor, bool bChildrenFirst = false);
  };

  class SubComponentInstantiationImpl
    : public BON::ModelImpl
  {
  public:
    virtual void accept (BON::Visitor* pVisitor, bool bChildrenFirst = false);
  };

  class AssemblyConnectionImpl
    : public BON::ModelImpl
  {
  public:
    virtual void accept (BON::Visitor* pVisitor, bool bChildrenFirst = false);
  };

  class SubComponentPortEndPointImpl
    : public BON::ModelImpl
  {
  public:
    virtual void accept (BON::Visitor* pVisitor, bool bChildrenFirst = false);
  };


  class SchemaVisitor
    : public BON::Visitor
  {
  public:
    SchemaVisitor(const std::string& path, const std::string& rootName);
    ~SchemaVisitor();

    void init();
    void initRootAttributes();
    void printDocument();
    void push();
    void pop();
    DOMElement* createSimpleContent (const std::string& name,
                                     const std::string& value);

    void visitAtomImpl (const BON::Atom& atom);
    void visitModelImpl (const BON::Model& model);
    void visitReferenceImpl (const BON::Reference& reference);

  private:

    DOMImplementation*  impl_;
    DOMDocument*        doc_;
    DOMElement*         root_;
    DOMElement*         curr_;
    DOMWriter*          serializer_;
    XMLFormatTarget*    target_;
    std::string         rootName_;
    std::stack<DOMElement*> curr_stack_;
  };

  DECLARE_BONEXTENSION (BON::Model, TopLevelPackageImpl, TopLevelPackage);
  DECLARE_BONEXTENSION (BON::Model, ImplementationArtifactImpl,
                        ImplementationArtifact);
  DECLARE_BONEXTENSION (BON::Model, NamedImplementationArtifactImpl,
                        NamedImplementationArtifact);
  DECLARE_BONEXTENSION (BON::Model, RequirementImpl, Requirement);
  DECLARE_BONEXTENSION (BON::Model, ComponentInterfaceImpl,
                        ComponentInterface);
  DECLARE_BONEXTENSION (BON::Model, ComponentPackageImpl, ComponentPackage);
  DECLARE_BONEXTENSION (BON::Model, PackagedComponentImplementationImpl,
                        PackagedComponentImplementation);
  DECLARE_BONEXTENSION (BON::Model, ComponentImplementationImpl,
                        ComponentImplementation);
  DECLARE_BONEXTENSION (BON::Model, CapabilityImpl, Capability);
  DECLARE_BONEXTENSION (BON::Model, ComponentAssemblyImpl, ComponentAssembly);
  DECLARE_BONEXTENSION (BON::Model, MonolithicImplementationImpl,
                        MonolithicImplementation);
  DECLARE_BONEXTENSION (BON::Model, SubComponentInstantiationImpl,
                        SubComponentInstantiation);
  DECLARE_BONEXTENSION (BON::Model, AssemblyConnectionImpl,
                        AssemblyConnection);
  DECLARE_BONEXTENSION (BON::Model, SubComponentPortEndPointImpl,
                        SubComponentPortEndPoint);

  bool getPath (const std::string& description, std::string& path);
};

#endif // Bon2Component_H
