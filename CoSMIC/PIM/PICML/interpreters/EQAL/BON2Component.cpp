//###############################################################################################################################################
//
//	Meta and Builder Object Network V2.0 for GME
//	BON2Component.cpp
//
//###############################################################################################################################################

/*
	Copyright (c) Vanderbilt University, 2000-2004
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

namespace
{
  std::string outputPath; // global variable for outputPath.

  void getPath( const std::string& message, std::string& path )
  {
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
    folder_browsinfo.lpszTitle = message.c_str();
    // Use new GUI style and allow edit plus file view
    folder_browsinfo.ulFlags = BIF_BROWSEINCLUDEFILES | BIF_RETURNONLYFSDIRS;
    // No callback function
    folder_browsinfo.lpfn = NULL;
    // No parameter passing into the dialog
    folder_browsinfo.lParam = 0;

    LPITEMIDLIST folder_pidl;
    folder_pidl = SHBrowseForFolder(&folder_browsinfo);

    if (folder_pidl == NULL)
      {
        return;
      }
    else
      {
        TCHAR FolderNameBuffer[MAX_PATH];

        // Convert the selection into a path
        if (SHGetPathFromIDList (folder_pidl, FolderNameBuffer))
          {
            path = FolderNameBuffer;
          }

        // Free the ItemIDList object returned from the call to
        // SHBrowseForFolder using Gawp utility function
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
          {
            imalloc->Free ( folder_pidl );
            imalloc->Release ( );
          }
      }
  }
}

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
	if ( m_project ) {
		m_project->finalizeObjects();
		finalize( m_project );
		m_project = NULL;
	}
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

void Component::invokeEx( Project& project, FCO& currentFCO, const std::set<FCO>& setSelectedFCOs, long lParam )
{

  std::string message = "Please specify the Output Directory";
  getPath (message, outputPath);

  std::set<Object> selected = project->findByKind ("EventChannelConfiguration");

  for (std::set<Object>::const_iterator it = selected.begin ();
       it != selected.end ();
       ++it)
    {
      EventChannelConfiguration ec_configuration (*it);
      
      if (!ec_configuration)
        {
          AfxMessageBox ("Interpretation must start from an EventChannelConfiguration model!");
          continue;
        }
        
      EventChannelConfigurationVisitor visitor; 

      visitor.visitModelImpl (ec_configuration);
    }

  AfxMessageBox("Event channel configuration interpretation completed!");

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

IMPLEMENT_BONEXTENSION( EventChannelConfiguration, "EventChannelConfiguration" );

IMPLEMENT_BONEXTENSION( RTEC_Proxy_Supplier, "RTEC_Proxy_Supplier" );

IMPLEMENT_BONEXTENSION( RTEC_Proxy_Consumer, "RTEC_Proxy_Consumer" );

IMPLEMENT_BONEXTENSION( RTEC_Resource_Factory, "RTEC_Resource_Factory" );

IMPLEMENT_BONEXTENSION( RTEC_Resource_Factory_Reference, "RTEC_Resource_Factory_Reference" );

IMPLEMENT_BONEXTENSION( RT_Info_Param, "" );

IMPLEMENT_BONEXTENSION( RT_Info, "RT_Info" );

IMPLEMENT_BONEXTENSION( OutEventPort, "OutEventPort" );

IMPLEMENT_BONEXTENSION( OutEventPortReference, "OutEventPortReferenceImpl" );

IMPLEMENT_BONEXTENSION( CCMComponent, "CCMComponentImpl" );

//==================================================================
// EventChannelConfiguration visitor class implementation
//==================================================================

void EventChannelConfigurationVisitor::visitModelImpl (const Model& model)
{
	std::set<Model> child_models = model->getChildModels ();
	for (std::set<Model>::iterator model_iter = child_models.begin ();
       model_iter != child_models.end ();
       model_iter++)
    {
      this->visitRTEC_Proxy_SupplierImpl (RTEC_Proxy_Supplier (*model_iter));
      this->visitRTEC_Proxy_ConsumerImpl (RTEC_Proxy_Consumer (*model_iter));
    }
}

void EventChannelConfigurationVisitor::visitRTEC_Proxy_SupplierImpl (const RTEC_Proxy_Supplier& rtec_proxy_supp)
{
  if (!rtec_proxy_supp) return;

	std::set<Atom> child_atoms = rtec_proxy_supp->getChildAtoms ();
	for (std::set<Atom>::iterator atom_iter = child_atoms.begin ();
       atom_iter != child_atoms.end ();
       atom_iter++)
    {
       RTEC_Proxy_SupplierVisitor visitor; //any constructor parameter?
       visitor.visitAtomImpl (FCO (*atom_iter));
    }
}

void EventChannelConfigurationVisitor::visitRTEC_Proxy_ConsumerImpl (const RTEC_Proxy_Consumer& rtec_proxy_cons)
{
  if (!rtec_proxy_cons) return;

	std::set<Atom> child_atoms = rtec_proxy_cons->getChildAtoms ();
	for (std::set<Atom>::iterator atom_iter = child_atoms.begin ();
       atom_iter != child_atoms.end ();
       atom_iter++)
    {
       RTEC_Proxy_ConsumerVisitor visitor; //any constructor parameter?
       visitor.visitAtomImpl (FCO (*atom_iter));
    }
}

//==================================================================
// RTEC_Proxy_Supplier visitor class implementation
//==================================================================

// This operation will generate the CPF file to describe the
// QoS information on the consumer side.
void RTEC_Proxy_SupplierVisitor::visitAtomImpl (const Atom& atom)
{
  this->visitRT_InfoImpl (RT_Info (atom));
}

void RTEC_Proxy_SupplierVisitor::visitRT_InfoImpl (const RT_Info& rt_info)
{
  // @TODO: Add implementation later.
}

//==================================================================
// RTEC_Proxy_Consumer visitor class implementation
//==================================================================

void RTEC_Proxy_ConsumerVisitor::visitAtomImpl (const Atom& atom)
{
  // This operation will generate the CPF file to describe the 
  // QoS information on the supplier side.
  // Also the service configuration file of the event channel
  // will be generated.
  this->visitRT_InfoImpl (RT_Info (atom));
}


void RTEC_Proxy_ConsumerVisitor::visitRT_InfoImpl (const RT_Info & rt_info)
{
  if (!rt_info) return;

  // Get the source event port reference
  std::multiset<ConnectionEnd> source_port_list = rt_info->getInConnEnds ("RTEC_Connection");

	std::multiset<ConnectionEnd>::iterator source_port_iter = source_port_list.begin ();

  ReferencePort source_port_ref = ReferencePort (*source_port_iter);
  if (!source_port_ref) return;

  FCO source_port = source_port_ref->getFCO ();
  std::string name = source_port->getName ();
  if (!source_port) return;

  // Get the CCMComponent 
  FCO component (source_port->getParent ());
  if (!component) return; //ERROR
  std::string component_name = component->getName ();

  std::string entry_point (rt_info->getAttribute("entry_point")->getStringValue());
  std::string source_port_name (source_port->getName ());

  std::string cpf_name =  outputPath + "\\" + component_name;
  cpf_name += "-" + source_port_name + "-SupplierQoS-" + entry_point;

  // Write CPF file to describe the Event Supplier's QoS
  rt_info->generate_CPF (cpf_name.c_str (), component_name.c_str (), source_port_name.c_str ());

  // Write svc.conf
  std::multiset<ConnectionEnd> rtec_factory_list = rt_info->getOutConnEnds ("Use_RT_Info");

  // One event channel has only one RTEC_Resource_Factory.
	for (std::multiset<ConnectionEnd>::iterator rtec_factory_iter = rtec_factory_list.begin ();
       rtec_factory_iter != rtec_factory_list.end ();
       rtec_factory_iter++)
    {

      RTEC_Resource_Factory_Reference rtec_factory_ref = FCO (*rtec_factory_iter);
      if (!rtec_factory_ref) return;
      
      RTEC_Resource_Factory rtec_factory = rtec_factory_ref->getReferred ();
      if (!rtec_factory) return;

      rtec_factory->generate_SVC (rtec_factory->getName ().c_str ());
    }
}

//==================================================================
// RTEC_Resource_FactoryImp class implementation
//==================================================================

void RTEC_Resource_FactoryImpl::generate_SVC (const char * file_name)
{

  std::ofstream svc_file;
  std::string svc_name =  outputPath + "\\" + file_name;
  svc_name += ".conf";
  svc_file.open (svc_name.c_str ());

  svc_file << "static EC_Factory \"";

  std::set<Attribute> rtec_factory_attributes = this->getAttributes ();
	for (std::set<Attribute>::iterator attribute_iter = rtec_factory_attributes.begin ();
       attribute_iter != rtec_factory_attributes.end ();
       attribute_iter++)
    {
       std::string default_value ((*attribute_iter)->getAttributeMeta ().defaultValue ());
       if ((*attribute_iter)->getStringValue () != default_value)
         svc_file << " -" << (*attribute_iter)->getAttributeMeta ().name () << " " << (*attribute_iter)->getStringValue ();
    }

  svc_file << "\"\n";
  svc_file.close ();

}

//==================================================================
// RT_InfoImpl class implementation
//==================================================================

void RT_InfoImpl::generate_CPF (const char * file_name, const char * component_name, const char * port_name)
{

  std::ofstream cpf_file;
  std::string cpf_name (file_name);
  cpf_name += ".cpf";
  cpf_file.open (cpf_name.c_str ());

  cpf_file << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
           << "<!DOCTYPE properties SYSTEM \"properties.dtd\">\n"
           << "<properties>\n"
           << "  <struct name=\"" << file_name << "\" type=\"ACEXML_RT_Info\">\n"
           << "    <description>TAO Real-time Scheduler info for " << component_name << "::" << port_name << "</description>\n"
           << "    <simple name=\"port\" type=\"string\">\n"
           << "      <value>" << port_name << "</value>\n"
           << "    </simple>\n";

  std::set<Attribute> rt_info_attributes = this->getAttributes ();
	for (std::set<Attribute>::iterator attribute_iter = rt_info_attributes.begin ();
       attribute_iter != rt_info_attributes.end ();
       attribute_iter++)
    {
       cpf_file << "    <simple name=\"" << (*attribute_iter)->getAttributeMeta ().name () << "\" type=\"string\">\n"
                << "      <value>" << (*attribute_iter)->getStringValue () << "</value>\n"
                << "    </simple>\n";
    }
  cpf_file << "  </struct>\n"
           << "</properties>\n";

  cpf_file.close ();

}

}; // namespace BON

