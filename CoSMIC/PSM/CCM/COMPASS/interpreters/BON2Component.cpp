//###############################################################################################################################################
//
//	Meta and Builder Object Network V2.0 for GME
//	BON2Component.cpp
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

#include "BON2Component.h"
#include <memory>

namespace BON
{

  //###############################################################################################################################################
  //
  // 	C L A S S : BON::Component
  //
  //###############################################################################################################################################

  Component::Component()
    : m_bIsInteractive( false )
  {
  }

  Component::~Component()
  {
    finalize( m_project );
    m_project = NULL;
  }

  // ====================================================
  // This method is called after all the generic initialization is done
  // This should be empty unless application-specific initialization is needed

  void Component::initialize( Project& project )
  {
    // ======================
    // Insert application specific code here
  }

  // ====================================================
  // This method is called before the whole BON2 project released and disposed
  // This should be empty unless application-specific finalization is needed

  void Component::finalize( Project& project )
  {
    // ======================
    // Insert application specific code here
  }

  // ====================================================
  // This is the obsolete component interface
  // This present implementation either tries to call InvokeEx, or does nothing except of a notification

  void Component::invoke( Project& project, const std::set<FCO>& setModels, long lParam )
  {
#ifdef SUPPORT_OLD_INVOKE
    Object focus;
    invokeEx( project, focus, setModels, lParam );
#else
    if ( m_bIsInteractive )
      AfxMessageBox("This BON2 Component does not support the obsolete invoke mechanism!");
#endif
  }

  // ====================================================
  // This is the main component method for Interpereters and Plugins.
  // May also be used in case of invokeable Add-Ons

  void Component::invokeEx( Project& project, FCO& currentFCO,
                            const std::set<FCO>& setSelectedFCOs, long lParam )
  {
    try
      {
        XMLPlatformUtils::Initialize();
      }
    catch (const XMLException& toCatch)
      {
        AfxMessageBox ("Error during Xerces-c Initialization.\n"
                       "  Exception message:" + XStr (toCatch.getMessage()));
        return;
      }

    if (!currentFCO && setSelectedFCOs.empty())
      {
        AfxMessageBox ("Interpretation must start from a TopLevelPackage, "
                       "or an ImplementationArtifact, or a "
                       "ComponentInterface, or a ComponentPackage, "
                       "ComponentImplementation !");
        return;
      }

    std::string outputPath;
    std::string message = "Please specify the Output Directory";
    Compass::getPath (message, outputPath);

    std::set<FCO> mySet (setSelectedFCOs);
    if (currentFCO)
      mySet.insert (currentFCO);

    std::string messageText = "The following files: \n";

    for (std::set<FCO>::iterator iter = mySet.begin();
         iter != mySet.end();
         ++iter)
      {
        std::string fileBase;
        std::string fileExt;
        if ((*iter)->getFCOMeta().name() == "TopLevelPackage")
          {
            fileBase = "package";
            fileExt = ".pcd";
          }
        else if ((*iter)->getFCOMeta().name() == "ImplementationArtifact")
          fileExt = ".iad";
        else if ((*iter)->getFCOMeta().name() == "ComponentInterface")
          fileExt = ".ccd";
        else if ((*iter)->getFCOMeta().name() == "ComponentPackage")
          fileExt = ".cpd";
        else if ((*iter)->getFCOMeta().name() == "ComponentImplementation")
          fileExt = ".cid";
        else
          {
            AfxMessageBox ("Interpretation must start from a TopLevelPackage, "
                           "or an ImplementationArtifact, or a "
                           "ComponentInterface, or a ComponentPackage, "
                           "ComponentImplementation !");
            return;
          }

        FCO root = (*iter);
        // Set the name of the root element
        std::string rootPrefix ("Deployment:");
        std::string rootSuffix ("Description");
        std::string rootName (root->getFCOMeta().name());
        if (fileBase.length() == 0)
          fileBase = root->getName();
        rootName = rootPrefix + rootName + rootSuffix;
        std::string fileName = fileBase + fileExt;
        fileName = outputPath + "\\" + fileName;

        Compass::SchemaVisitor* visitor = new Compass::SchemaVisitor(fileName,
                                                                     rootName);
        std::auto_ptr<Compass::SchemaVisitor> cleanup_visitor (visitor);
        if ((*iter)->getFCOMeta().name() == "TopLevelPackage")
          {
            Compass::TopLevelPackage start
              = Compass::TopLevelPackage ((*iter));
            start->accept(visitor);
          }
        else if ((*iter)->getFCOMeta().name() == "ImplementationArtifact")
          {
            Compass::ImplementationArtifact start
              = Compass::ImplementationArtifact ((*iter));
            start->accept(visitor);
          }
        else if ((*iter)->getFCOMeta().name() == "ComponentInterface")
          {
            Compass::ComponentInterface start
              = Compass::ComponentInterface ((*iter));
            start->accept(visitor);
          }
        else if ((*iter)->getFCOMeta().name() == "ComponentPackage")
          {
            Compass::ComponentPackage start
              = Compass::ComponentPackage ((*iter));
            start->accept(visitor);
          }
        else if ((*iter)->getFCOMeta().name() == "ComponentImplementation")
          {
            Compass::ComponentImplementation start
              = Compass::ComponentImplementation ((*iter));
            start->accept(visitor);
          }
        visitor->printDocument();
        messageText += fileName + "\n";
      }
    try
      {
        XMLPlatformUtils::Terminate();
      }
    catch (const XMLException& toCatch)
      {
        AfxMessageBox ("Error during Xerces-c Finalization.\n"
                       "  Exception message:" + XStr (toCatch.getMessage()));
        return;
      }
    AfxMessageBox (messageText.c_str() + CString(" were successfully generated ! "));    return;
  }

  // ====================================================
  // GME currently does not use this function
  // You only need to implement it if other invokation mechanisms are used

  void Component::objectInvokeEx( Project& project, Object& currentObject, const std::set<Object>& setSelectedObjects, long lParam )
  {
    if ( m_bIsInteractive )
      AfxMessageBox("This BON2 Component does not support objectInvokeEx method!");
  }

  // ====================================================
  // Implement application specific parameter-mechanism in these functions

  Util::Variant Component::getParameter( const std::string& strName )
  {
    // ======================
    // Insert application specific code here

    return Util::Variant();
  }

  void Component::setParameter( const std::string& strName, const Util::Variant& varValue )
  {
    // ======================
    // Insert application specific code here
  }

#ifdef GME_ADDON

  // ====================================================
  // If the component is an Add-On, then this method is called for every Global Event

  void Component::globalEventPerformed( globalevent_enum event )
  {
    // ======================
    // Insert application specific code here
  }

  // ====================================================
  // If the component is an Add-On, then this method is called for every Object Event

  void Component::objectEventPerformed( Object& object, unsigned long event, VARIANT v )
  {
    // ======================
    // Insert application specific code here
  }

#endif // GME_ADDON

  IMPLEMENT_BONEXTENSION (Compass::TopLevelPackage, "TopLevelPackage");
  IMPLEMENT_BONEXTENSION (Compass::ImplementationArtifact,
                          "ImplementationArtifact");
  IMPLEMENT_BONEXTENSION (Compass::NamedImplementationArtifact,
                          "NamedImplementationArtifact");
  IMPLEMENT_BONEXTENSION (Compass::Requirement, "Requirement");
  IMPLEMENT_BONEXTENSION (Compass::ComponentInterface, "ComponentInterface");
  IMPLEMENT_BONEXTENSION (Compass::ComponentPackage, "ComponentPackage");
  IMPLEMENT_BONEXTENSION (Compass::PackagedComponentImplementation,
                          "PackagedComponentImplementation");
  IMPLEMENT_BONEXTENSION (Compass::ComponentImplementation,
                          "ComponentImplementation");
  IMPLEMENT_BONEXTENSION (Compass::Capability, "Capability");
  IMPLEMENT_BONEXTENSION (Compass::ComponentAssembly, "ComponentAssembly");
  IMPLEMENT_BONEXTENSION (Compass::MonolithicImplementation,
                          "MonolithicImplementation");
  IMPLEMENT_BONEXTENSION (Compass::SubComponentInstantiation,
                          "SubComponentInstantiation");
  IMPLEMENT_BONEXTENSION (Compass::AssemblyConnection, "AssemblyConnection");
  IMPLEMENT_BONEXTENSION (Compass::SubComponentPortEndPoint,
                          "SubComponentPortEndPoint");

}; // namespace BON

namespace Compass {

  void TopLevelPackageImpl::accept (BON::Visitor* pVisitor,
                                    bool bChildrenFirst)
  {
    SchemaVisitor* sVisitor = dynamic_cast<SchemaVisitor*> (pVisitor);
    if (!sVisitor)
      {
        AfxMessageBox ("Oops..We have a corrupt visitor.");
        throw(std::exception());
      }
    sVisitor->push();
    BON::Model model( this );
    if (!bChildrenFirst )
      pVisitor->visitModel( model );

    std::set<BON::FCO> children = model->getChildFCOs();
    for (std::set<BON::FCO>::iterator it = children.begin();
         it != children.end();
         it++)
      (*it)->accept(pVisitor, bChildrenFirst);
    if (bChildrenFirst)
      pVisitor->visitModel(model);
    sVisitor->pop();

  }

  void ImplementationArtifactImpl::accept (BON::Visitor* pVisitor, bool bChildrenFirst)
  {
    SchemaVisitor* sVisitor = dynamic_cast<SchemaVisitor*> (pVisitor);
    if (!sVisitor)
      {
        AfxMessageBox ("Oops..We have a corrupt visitor.");
        throw(std::exception());
      }
    sVisitor->push();
    BON::Model model( this );
    if (!bChildrenFirst )
      pVisitor->visitModel( model );

    std::set<BON::FCO> children = model->getChildFCOs();
    for (std::set<BON::FCO>::iterator it = children.begin();
         it != children.end();
         it++)
      (*it)->accept(pVisitor, bChildrenFirst);
    if (bChildrenFirst)
      pVisitor->visitModel(model);
    sVisitor->pop();
  }

  void NamedImplementationArtifactImpl::accept (BON::Visitor* pVisitor,
                                                bool bChildrenFirst)
  {
    SchemaVisitor* sVisitor = dynamic_cast<SchemaVisitor*> (pVisitor);
    if (!sVisitor)
      {
        AfxMessageBox ("Oops..We have a corrupt visitor.");
        throw(std::exception());
      }
    sVisitor->push();
    BON::Model model( this );
    if (!bChildrenFirst )
      pVisitor->visitModel( model );

    std::set<BON::FCO> children = model->getChildFCOs();
    for (std::set<BON::FCO>::iterator it = children.begin();
         it != children.end();
         it++)
      (*it)->accept(pVisitor, bChildrenFirst);
    if (bChildrenFirst)
      pVisitor->visitModel(model);
    sVisitor->pop();
  }

  void RequirementImpl::accept (BON::Visitor* pVisitor,
                                bool bChildrenFirst)
  {
    SchemaVisitor* sVisitor = dynamic_cast<SchemaVisitor*> (pVisitor);
    if (!sVisitor)
      {
        AfxMessageBox ("Oops..We have a corrupt visitor.");
        throw(std::exception());
      }
    sVisitor->push();
    BON::Model model( this );
    if (!bChildrenFirst )
      pVisitor->visitModel( model );

    std::set<BON::FCO> children = model->getChildFCOs();
    for (std::set<BON::FCO>::iterator it = children.begin();
         it != children.end();
         it++)
      (*it)->accept(pVisitor, bChildrenFirst);
    if (bChildrenFirst)
      pVisitor->visitModel(model);
    sVisitor->pop();
  }

  void ComponentInterfaceImpl::accept (BON::Visitor* pVisitor,
                                       bool bChildrenFirst)
  {
    SchemaVisitor* sVisitor = dynamic_cast<SchemaVisitor*> (pVisitor);
    if (!sVisitor)
      {
        AfxMessageBox ("Oops..We have a corrupt visitor.");
        throw(std::exception());
      }
    sVisitor->push();
    BON::Model model( this );
    if (!bChildrenFirst )
      pVisitor->visitModel( model );

    std::set<BON::FCO> children = model->getChildFCOs();
    for (std::set<BON::FCO>::iterator it = children.begin();
         it != children.end();
         it++)
      (*it)->accept(pVisitor, bChildrenFirst);
    if (bChildrenFirst)
      pVisitor->visitModel(model);
    sVisitor->pop();
  }

  void ComponentPackageImpl::accept (BON::Visitor* pVisitor,
                                     bool bChildrenFirst)
  {
    SchemaVisitor* sVisitor = dynamic_cast<SchemaVisitor*> (pVisitor);
    if (!sVisitor)
      {
        AfxMessageBox ("Oops..We have a corrupt visitor.");
        throw(std::exception());
      }
    sVisitor->push();
    BON::Model model( this );
    if (!bChildrenFirst )
      pVisitor->visitModel( model );

    std::set<BON::FCO> children = model->getChildFCOs();
    for (std::set<BON::FCO>::iterator it = children.begin();
         it != children.end();
         it++)
      (*it)->accept(pVisitor, bChildrenFirst);
    if (bChildrenFirst)
      pVisitor->visitModel(model);
    sVisitor->pop();
  }

  void PackagedComponentImplementationImpl::accept (BON::Visitor* pVisitor,
                                                    bool bChildrenFirst)
  {
    SchemaVisitor* sVisitor = dynamic_cast<SchemaVisitor*> (pVisitor);
    if (!sVisitor)
      {
        AfxMessageBox ("Oops..We have a corrupt visitor.");
        throw(std::exception());
      }
    sVisitor->push();
    BON::Model model( this );
    if (!bChildrenFirst )
      pVisitor->visitModel( model );

    std::set<BON::FCO> children = model->getChildFCOs();
    for (std::set<BON::FCO>::iterator it = children.begin();
         it != children.end();
         it++)
      (*it)->accept(pVisitor, bChildrenFirst);
    if (bChildrenFirst)
      pVisitor->visitModel(model);
    sVisitor->pop();
  }

  void ComponentImplementationImpl::accept (BON::Visitor* pVisitor,
                                            bool bChildrenFirst)
  {
    SchemaVisitor* sVisitor = dynamic_cast<SchemaVisitor*> (pVisitor);
    if (!sVisitor)
      {
        AfxMessageBox ("Oops..We have a corrupt visitor.");
        throw(std::exception());
      }
    sVisitor->push();
    BON::Model model( this );
    if (!bChildrenFirst )
      pVisitor->visitModel( model );

    std::set<BON::FCO> children = model->getChildFCOs();
    for (std::set<BON::FCO>::iterator it = children.begin();
         it != children.end();
         it++)
      (*it)->accept(pVisitor, bChildrenFirst);
    if (bChildrenFirst)
      pVisitor->visitModel(model);
    sVisitor->pop();
  }

  void CapabilityImpl::accept (BON::Visitor* pVisitor,
                               bool bChildrenFirst)
  {
    SchemaVisitor* sVisitor = dynamic_cast<SchemaVisitor*> (pVisitor);
    if (!sVisitor)
      {
        AfxMessageBox ("Oops..We have a corrupt visitor.");
        throw(std::exception());
      }
    sVisitor->push();
    BON::Model model( this );
    if (!bChildrenFirst )
      pVisitor->visitModel( model );

    std::set<BON::FCO> children = model->getChildFCOs();
    for (std::set<BON::FCO>::iterator it = children.begin();
         it != children.end();
         it++)
      (*it)->accept(pVisitor, bChildrenFirst);
    if (bChildrenFirst)
      pVisitor->visitModel(model);
    sVisitor->pop();
  }


  void ComponentAssemblyImpl::accept (BON::Visitor* pVisitor,
                                      bool bChildrenFirst)
  {
    SchemaVisitor* sVisitor = dynamic_cast<SchemaVisitor*> (pVisitor);
    if (!sVisitor)
      {
        AfxMessageBox ("Oops..We have a corrupt visitor.");
        throw(std::exception());
      }
    sVisitor->push();
    BON::Model model( this );
    if (!bChildrenFirst )
      pVisitor->visitModel( model );

    std::set<BON::FCO> children1 = model->getChildFCOsAs("instance");
    for (std::set<BON::FCO>::iterator it = children1.begin();
         it != children1.end();
         it++)
      (*it)->accept(pVisitor, bChildrenFirst);

    std::set<BON::FCO> children2 = model->getChildFCOsAs("connection");
    for (std::set<BON::FCO>::iterator iter = children2.begin();
         iter != children2.end();
         iter++)
      (*iter)->accept(pVisitor, bChildrenFirst);

    if (bChildrenFirst)
      pVisitor->visitModel(model);
    sVisitor->pop();
  }

  void MonolithicImplementationImpl::accept (BON::Visitor* pVisitor,
                                             bool bChildrenFirst)
  {
    SchemaVisitor* sVisitor = dynamic_cast<SchemaVisitor*> (pVisitor);
    if (!sVisitor)
      {
        AfxMessageBox ("Oops..We have a corrupt visitor.");
        throw(std::exception());
      }
    sVisitor->push();
    BON::Model model( this );
    if (!bChildrenFirst )
      pVisitor->visitModel( model );

    std::set<BON::FCO> children = model->getChildFCOs();
    for (std::set<BON::FCO>::iterator it = children.begin();
         it != children.end();
         it++)
      (*it)->accept(pVisitor, bChildrenFirst);
    if (bChildrenFirst)
      pVisitor->visitModel(model);
    sVisitor->pop();
  }

  void SubComponentInstantiationImpl::accept (BON::Visitor* pVisitor,
                                              bool bChildrenFirst)
  {
    SchemaVisitor* sVisitor = dynamic_cast<SchemaVisitor*> (pVisitor);
    if (!sVisitor)
      {
        AfxMessageBox ("Oops..We have a corrupt visitor.");
        throw(std::exception());
      }
    sVisitor->push();
    BON::Model model( this );
    if (!bChildrenFirst )
      pVisitor->visitModel( model );

    std::set<BON::FCO> children = model->getChildFCOs();
    for (std::set<BON::FCO>::iterator it = children.begin();
         it != children.end();
         it++)
      (*it)->accept(pVisitor, bChildrenFirst);
    if (bChildrenFirst)
      pVisitor->visitModel(model);
    sVisitor->pop();
  }

  void AssemblyConnectionImpl::accept (BON::Visitor* pVisitor,
                                       bool bChildrenFirst)
  {
    SchemaVisitor* sVisitor = dynamic_cast<SchemaVisitor*> (pVisitor);
    if (!sVisitor)
      {
        AfxMessageBox ("Oops..We have a corrupt visitor.");
        throw(std::exception());
      }
    sVisitor->push();
    BON::Model model( this );
    if (!bChildrenFirst )
      pVisitor->visitModel( model );

    std::set<BON::FCO> children = model->getChildFCOs();
    for (std::set<BON::FCO>::iterator it = children.begin();
         it != children.end();
         it++)
      (*it)->accept(pVisitor, bChildrenFirst);
    if (bChildrenFirst)
      pVisitor->visitModel(model);
    sVisitor->pop();
  }

  void SubComponentPortEndPointImpl::accept (BON::Visitor* pVisitor,
                                             bool bChildrenFirst)
  {
    SchemaVisitor* sVisitor = dynamic_cast<SchemaVisitor*> (pVisitor);
    if (!sVisitor)
      {
        AfxMessageBox ("Oops..We have a corrupt visitor.");
        throw(std::exception());
      }
    sVisitor->push();
    BON::Model model( this );
    if (!bChildrenFirst )
      pVisitor->visitModel( model );

    std::set<BON::FCO> children = model->getChildFCOs();
    for (std::set<BON::FCO>::iterator it = children.begin();
         it != children.end();
         it++)
      (*it)->accept(pVisitor, bChildrenFirst);
    if (bChildrenFirst)
      pVisitor->visitModel(model);
    sVisitor->pop();
  }

  // This method prompts a dialog to allow the user to specify a folder
  bool getPath (const std::string& description, std::string& path)
  {
    // Initalize the com library
    //WINOLEAPI com_lib_return = OleInitialize(NULL);

    // Dialog instruction
    char display_buffer[MAX_PATH];
    BROWSEINFO folder_browsinfo;
    memset (&folder_browsinfo, 0, sizeof (folder_browsinfo));

    // Set GME as the owner of the dialog
    folder_browsinfo.hwndOwner = GetForegroundWindow();
    // Start the brows from desktop
    folder_browsinfo.pidlRoot = NULL;
    // Pointer to the folder name display buffer
    folder_browsinfo.pszDisplayName = &display_buffer[0];
    // Diaglog instruction string
    folder_browsinfo.lpszTitle = description.c_str();
    // Use new GUI style and allow edit plus file view
    folder_browsinfo.ulFlags = BIF_BROWSEINCLUDEFILES | BIF_RETURNONLYFSDIRS;
    // No callback function
    folder_browsinfo.lpfn = NULL;
    // No parameter passing into the dialog
    folder_browsinfo.lParam = 0;

    LPITEMIDLIST folder_pidl;
    folder_pidl = SHBrowseForFolder(&folder_browsinfo);

    if(folder_pidl == NULL)
      return false;
    else
      {
        TCHAR FolderNameBuffer[MAX_PATH];

        // Convert the selection into a path
        if (SHGetPathFromIDList (folder_pidl, FolderNameBuffer))
          path = FolderNameBuffer;

        // Free the ItemIDList object returned from the call to
        // SHBrowseForFolder using Gawp utility function
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
          {
            imalloc->Free ( folder_pidl );
            imalloc->Release ( );
          }
      }
    return true;
  }

  SchemaVisitor::SchemaVisitor (const std::string& path,
                                const std::string& rootName)
    : impl_ (0), doc_ (0), root_ (0), curr_ (0), serializer_ (0), target_ (0),
      rootName_ (rootName)
  {
    this->target_ = new LocalFileFormatTarget (path.c_str());
    this->init();
  }

  SchemaVisitor::~SchemaVisitor ()
  {
    if (this->doc_)
      this->doc_->release();
    delete this->target_;
  }

  void SchemaVisitor::init()
  {
    try
      {
        this->impl_ = DOMImplementationRegistry::getDOMImplementation(XStr("LS"));
        this->serializer_ = ((DOMImplementationLS*)impl_)->createDOMWriter();

        // set feature if the serializer supports the feature/mode
        if (this->serializer_->canSetFeature
            (XMLUni::fgDOMWRTDiscardDefaultContent, true))
          this->serializer_->setFeature (XMLUni::fgDOMWRTDiscardDefaultContent,
                                         true);

        if (this->serializer_->canSetFeature (XMLUni::fgDOMWRTFormatPrettyPrint,
                                              true))
          this->serializer_->setFeature (XMLUni::fgDOMWRTFormatPrettyPrint, true);

        if (this->serializer_->canSetFeature (XMLUni::fgDOMWRTBOM, false))
          this->serializer_->setFeature (XMLUni::fgDOMWRTBOM, false);

        // Create the document
        this->doc_ =
          this->impl_->createDocument (XStr ("http://www.omg.org/Deployment"),
                                       XStr (this->rootName_.c_str()),
                                       0);

      }
    catch (const DOMException& toCatch)
      {
        AfxMessageBox("Caught DOMException :" + XStr (toCatch.msg));
        return;
      }
    this->initRootAttributes();
  }

  void SchemaVisitor::initRootAttributes()
  {
    try
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
    catch (const DOMException& toCatch)
      {
        AfxMessageBox("Caught DOMException :" + XStr (toCatch.msg));
        return;
      }
  }

  void SchemaVisitor::printDocument()
  {
    try
      {
        this->serializer_->writeNode (this->target_, *this->doc_);
      }
    catch (const DOMException& toCatch)
      {
        AfxMessageBox("Caught DOMException :" + XStr (toCatch.msg));
        return;
      }
    catch (const XMLException& xmlex)
      {
        AfxMessageBox("Caught XMLException :" + XStr (xmlex.getMessage()));
        return;
      }
  }

  void SchemaVisitor::push()
  {
    this->curr_stack_.push (this->curr_);
  }

  void SchemaVisitor::pop()
  {
    if (!this->curr_stack_.empty())
      {
        this->curr_ = this->curr_stack_.top();
        this->curr_stack_.pop();
      }
    else
      {
        AfxMessageBox("Oops..Mismatch in push()/pop()");
        throw(std::exception());
      }
  }

  DOMElement* SchemaVisitor::createSimpleContent (const std::string& name,
                                                  const std::string& value)
  {
    DOMElement* pName = this->doc_->createElement (XStr (name.c_str()));
    DOMText* pValue = this->doc_->createTextNode (XStr (value.c_str()));
    pName->appendChild (pValue);
    return pName;
  }

  void SchemaVisitor::visitAtomImpl (const BON::Atom& atom)
  {
    std::string derivedType = atom->getFCOMeta().name();
    if (derivedType == "Property")
      {
        std::string role = atom->getRole().name();
        DOMElement* pRole = this->doc_->createElement (XStr (role.c_str()));
        std::string value = atom->getAttribute ("value")->getStringValue();
        pRole->appendChild (this->createSimpleContent ("name",
                                                       atom->getName()));
        pRole->appendChild (this->createSimpleContent ("value", value));
        this->curr_->appendChild (pRole);
      }
    else if (derivedType == "ImplementationDependency")
      {
        std::string role = atom->getRole().name();
        DOMElement* pRole = this->doc_->createElement (XStr (role.c_str()));
        std::string value = atom->getAttribute ("requiredType")->getStringValue();
        pRole->appendChild (this->createSimpleContent ("requiredType",
                                                       value));
        this->curr_->appendChild (pRole);
      }
    else if (derivedType == "SatisfierProperty")
      {
        std::string role = atom->getRole().name();
        DOMElement* pRole = this->doc_->createElement (XStr (role.c_str()));
        std::string kind
          = atom->getAttribute ("PropertyKind")->getStringValue();
        std::string value = atom->getAttribute ("value")->getStringValue();
        pRole->appendChild (this->createSimpleContent ("name",
                                                       atom->getName()));
        pRole->appendChild (this->createSimpleContent ("kind", kind));
        pRole->appendChild (this->createSimpleContent ("value", value));
        this->curr_->appendChild (pRole);
      }
    else if (derivedType == "ComponentProperty")
      {
        std::string role = atom->getRole().name();
        DOMElement* pRole = this->doc_->createElement (XStr (role.c_str()));
        std::string value = atom->getAttribute ("value")->getStringValue();
        pRole->appendChild (this->createSimpleContent ("name",
                                                       atom->getName()));
        pRole->appendChild (this->createSimpleContent ("type", value));
        this->curr_->appendChild (pRole);
      }
    else if (derivedType == "ComponentPort")
      {
        std::string role = atom->getRole().name();
        DOMElement* pRole = this->doc_->createElement (XStr (role.c_str()));
        std::string exclusiveProvider
          = atom->getAttribute ("exclusiveProvider")->getStringValue();
        std::string exclusiveUser
          = atom->getAttribute ("exclusiveUser")->getStringValue();
        std::string kind = atom->getAttribute ("kind")->getStringValue();
        std::string optional
          = atom->getAttribute ("optional")->getStringValue();
        std::string provider
          = atom->getAttribute ("provider")->getStringValue();
        std::string supportedType =
          atom->getAttribute ("supportedType")->getStringValue();
        std::string specificType =
          atom->getAttribute ("specificType")->getStringValue();
        pRole->appendChild (this->createSimpleContent ("name",
                                                       atom->getName()));
        pRole->appendChild (this->createSimpleContent ("exclusiveProvider",
                                                       exclusiveProvider));
        pRole->appendChild (this->createSimpleContent ("exclusiveUser",
                                                       exclusiveUser));
        pRole->appendChild (this->createSimpleContent ("optional",optional));
        pRole->appendChild (this->createSimpleContent ("provider",provider));
        pRole->appendChild (this->createSimpleContent ("supportedType",
                                                       supportedType));
        pRole->appendChild (this->createSimpleContent ("specificType",
                                                       specificType));
        pRole->appendChild (this->createSimpleContent ("kind", kind));
        this->curr_->appendChild (pRole);
      }
  }

  void SchemaVisitor::visitModelImpl (const BON::Model& model)
  {
    std::string derivedType = model->getFCOMeta().name();
    if (derivedType == "TopLevelPackage")
      {
      }
    else if (derivedType == "ImplementationArtifact")
      {
        std::string label = model->getAttribute ("label")->getStringValue();
        this->curr_->appendChild (this->createSimpleContent ("label", label));
        std::string UUID = model->getAttribute ("UUID")->getStringValue();
        this->curr_->appendChild (this->createSimpleContent ("UUID", UUID));
        std::string location
          = model->getAttribute ("location")->getStringValue();
        this->curr_->appendChild (this->createSimpleContent ("location",
                                                             location));
      }
    else if (derivedType == "NamedImplementationArtifact"
             || derivedType == "PackagedComponentImplementation")
      {
        std::string role = model->getRole().name();
        DOMElement* pRole = this->doc_->createElement (XStr (role.c_str()));
        pRole->appendChild (this->createSimpleContent ("name",
                                                       model->getName()));
        this->curr_->appendChild (pRole);
        this->curr_ = pRole;
      }
    else if (derivedType == "Requirement"
             || derivedType == "Capability")
      {
        std::string role = model->getRole().name();
        DOMElement* pRole = this->doc_->createElement (XStr (role.c_str()));
        pRole->appendChild (this->createSimpleContent ("resourceType",
                                                       model->getName()));
        this->curr_->appendChild (pRole);
        this->curr_ = pRole;
      }
    else if (derivedType == "ComponentInterface")
      {
        std::string label = model->getAttribute ("label")->getStringValue();
        this->curr_->appendChild (this->createSimpleContent ("label", label));
        std::string UUID = model->getAttribute ("UUID")->getStringValue();
        this->curr_->appendChild (this->createSimpleContent ("UUID", UUID));
        std::string specificType
          = model->getAttribute ("specificType")->getStringValue();
        this->curr_->appendChild (this->createSimpleContent ("specificType",
                                                             specificType));
        std::string supportedType
          = model->getAttribute ("supportedType")->getStringValue();
        this->curr_->appendChild (this->createSimpleContent ("supportedType",
                                                             supportedType));
      }
    else if (derivedType == "ComponentPackage"
             || derivedType == "ComponentImplementation")
      {
        std::string label = model->getAttribute ("label")->getStringValue();
        this->curr_->appendChild (this->createSimpleContent ("label", label));
        std::string UUID = model->getAttribute ("UUID")->getStringValue();
        this->curr_->appendChild (this->createSimpleContent ("UUID", UUID));
      }
    else if (derivedType == "ComponentAssembly"
             || derivedType == "MonolithicImplementation")
      {
        std::string role = model->getRole().name();
        DOMElement* pRole = this->doc_->createElement (XStr (role.c_str()));
        this->curr_->appendChild (pRole);
        this->curr_ = pRole;
      }
    else if (derivedType == "AssemblyConnection")
      {
        std::string role = model->getRole().name();
        DOMElement* pRole = this->doc_->createElement (XStr (role.c_str()));
        pRole->appendChild (this->createSimpleContent ("name",
                                                       model->getName()));
        this->curr_->appendChild (pRole);
        this->curr_ = pRole;
      }
    else if (derivedType == "SubComponentInstantiation")
      {
        std::string role = model->getRole().name();
        DOMElement* pRole = this->doc_->createElement (XStr (role.c_str()));
        std::string id = model->getAttribute ("id")->getStringValue();
        pRole->setAttribute (XStr ("xmi:id"),
                             XStr (id.c_str()));
        pRole->appendChild (this->createSimpleContent ("name",
                                                       model->getName()));
        this->curr_->appendChild (pRole);
        this->curr_ = pRole;
      }
    else if (derivedType == "SubComponentPortEndPoint")
      {
        std::string role = model->getRole().name();
        DOMElement* pRole = this->doc_->createElement (XStr (role.c_str()));
        std::string portName
          = model->getAttribute ("portName")->getStringValue();
        pRole->appendChild (this->createSimpleContent ("portName",
                                                       portName));
        this->curr_->appendChild (pRole);
        this->curr_ = pRole;
      }
  }

  void SchemaVisitor::visitReferenceImpl (const BON::Reference& reference)
  {
    std::string suffix;
    std::string derivedType = reference->getFCOMeta().name();
    if (derivedType == "ComponentPackageReference")
      suffix = ".cpd";
    else if (derivedType == "ImplementationArtifactReference")
      suffix = ".iad";
    else if (derivedType == "ComponentInterfaceReference")
      suffix = ".ccd";
    else if (derivedType == "ComponentImplementationReference")
      suffix = ".cid";
    else if (derivedType == "SubComponentInstantiationReference")
      {
        std::string roleName = reference->getRole().name();
        std::string referencedElem
          = reference->getReferred()->getAttribute ("id")->getStringValue();
        DOMElement* ref = this->doc_->createElement (XStr (roleName.c_str()));
        ref->setAttribute (XStr ("xmi:idref"),
                           XStr (referencedElem.c_str()));
        this->curr_->appendChild (ref);
        return;
      }
    std::string roleName = reference->getRole().name();
    std::string referencedElem = reference->getReferred()->getName();
    referencedElem += suffix;
    DOMElement* ref = this->doc_->createElement (XStr (roleName.c_str()));
    ref->setAttribute (XStr ("href"), XStr (referencedElem.c_str()));
    this->curr_->appendChild (ref);
  }
};
