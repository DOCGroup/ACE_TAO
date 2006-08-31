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
#include "SchemaLogger.h"
#include "SchemaVisitor.h"

NSBEGIN

	SchemaLogger *glob_log = new SchemaLogger (CString("DomainDescriptor.cdd"));

NSEND

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

void Component::invokeEx( Project& project, FCO& currentFCO, const std::set<FCO>& setSelectedFCOs, long lParam )
{
	// ======================
	// Insert application specific code here
	//AfxMessageBox("This BON2 Component does not support objectInvokeEx method!");

	Domain domain = Domain(currentFCO);

	using namespace NS__;
	//glob_log->timeStamp ();


	//*glob_log << "*******************************************************************\n";
	//*glob_log << "Component Domain Descriptor \n";
	//*glob_log << "*******************************************************************\n";
	//*glob_log << "\n";
	*glob_log << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
	*glob_log << "<Deployment:Domain\n";
	*glob_log << "        xmlns:Deployment=\"http://www.omg.org/Deployment\"\n";
	*glob_log << "        xmlns:xmi=\"http://www.omg.org/XMI\"\n";
	*glob_log << "        xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
	*glob_log << "        xsi:schemaLocation=\"http://www.omg.org/2001/Deployment Deployment.xsd\">\n";


	SchemaVisitor *sv=new SchemaVisitor;
	//project->getRootFolder()->accept(sv,false);


	std::set<Model> children = project->getRootFolder()->getChildModels();
	for ( std::set<Model>::iterator it = children.begin() ; it != children.end() ; it++ )
	{
		std::string otype = (*it)->getObjectMeta().name();
		if (otype == "Domain")
		{
			BON::Attribute domain_name = (*it)->getAttribute("label");

			BON::Attribute domain_uuid = (*it)->getAttribute("UUID");

			*glob_log << "<label>" << domain_name->getStringValue() << "</label>" << "\n";
			*glob_log << "<UUID>" << domain_uuid->getStringValue() << "</UUID>" << "\n";


	    	std::set<Model> model_children = (*it)->getChildModels ();
			for ( std::set<Model>::iterator child_it = model_children.begin() ; child_it != model_children.end() ; child_it++ )
			{
				if ((*child_it)->getName() == "bridge")
				{
					BON::Attribute bridge_name = (*child_it)->getAttribute("BridgeName");

					BON::Attribute bridge_label = (*child_it)->getAttribute("BridgeLabel");

		            *glob_log << "<bridge xmi:id=\"" << bridge_label->getStringValue() << "\">" << "\n";

					*glob_log << "        <name>" << bridge_name->getStringValue() << "</name>" << "\n";
					*glob_log << "        <label>" << bridge_label->getStringValue() << "</label>" << "\n";

					std::set<Reference> br_ch_refs = (*child_it)->getChildReferences ();
				    for ( std::set<Reference>::iterator br_ch_ref = br_ch_refs.begin (); br_ch_ref != br_ch_refs.end (); br_ch_ref++ )
					{
					    //*glob_log << "     Visited Child Reference with " << (*br_ch_ref)->getName () << "\n";
						std::string br_ch_ref_name = (*br_ch_ref)->getName ();

						if (br_ch_ref_name == "InterconnectReference")
						{
							BON::FCO br_ch_ic_referred = (*br_ch_ref)->getReferred();

							BON::Attribute br_ch_ic_referred_attr = (br_ch_ic_referred)->getAttribute("InterconnectLabel");
							*glob_log << "        <connect xmi:idref=\"" << br_ch_ic_referred_attr->getStringValue() << "\" />" << "\n";
						}

						if (br_ch_ref_name == "ResourceReference")
						{
							BON::FCO br_ch_re_referred = (*br_ch_ref)->getReferred();

							BON::Attribute br_ch_re_referred_name_attr = (br_ch_re_referred)->getAttribute("ResourceName");
							BON::Attribute br_ch_re_referred_type_attr = (br_ch_re_referred)->getAttribute("resourceType");

							*glob_log << "    <resource xmi:idref=\"" << br_ch_re_referred_name_attr->getStringValue() << "\">" << "\n";
							*glob_log << "            <name>" << br_ch_re_referred_name_attr->getStringValue() << "</name>" << "\n";
							*glob_log << "            <resourceType>" << br_ch_re_referred_type_attr->getStringValue() << "</resourceType>" << "\n";
							*glob_log << "    </resource>" << "\n";
						}
					}

					*glob_log << "</" << (*child_it)->getName() << ">" << "\n";
				}
				if ((*child_it)->getName() == "node")
				{
					BON::Attribute node_name = (*child_it)->getAttribute("NodeName");

					BON::Attribute node_label = (*child_it)->getAttribute("NodeLabel");

					*glob_log << "<node xmi:id=\"" << node_label->getStringValue() << "\">" << "\n";

					*glob_log << "        <name>" << node_name->getStringValue() << "</name>" << "\n";
					*glob_log << "        <label>" << node_label->getStringValue() << "</label>" << "\n";

					std::set<Reference> n_ch_refs = (*child_it)->getChildReferences ();
				    for ( std::set<Reference>::iterator n_ch_ref = n_ch_refs.begin (); n_ch_ref != n_ch_refs.end (); n_ch_ref++ )
					{
					   //*glob_log << "     Visited Child Reference with " << (*n_ch_ref)->getName () << "\n";

					   std::string n_ch_ref_name = (*n_ch_ref)->getName ();

						if (n_ch_ref_name == "InterconnectReference")
						{
							BON::FCO n_ch_ic_referred = (*n_ch_ref)->getReferred();

							BON::Attribute n_ch_ic_referred_attr = (n_ch_ic_referred)->getAttribute("InterconnectLabel");
							*glob_log << "        <connection xmi:idref=\"" << n_ch_ic_referred_attr->getStringValue() << "\" />" << "\n";
						}

						if (n_ch_ref_name == "ResourceReference")
						{
							BON::FCO n_ch_re_referred = (*n_ch_ref)->getReferred();

							BON::Attribute n_ch_re_referred_name_attr = (n_ch_re_referred)->getAttribute("ResourceName");
							BON::Attribute n_ch_re_referred_type_attr = (n_ch_re_referred)->getAttribute("resourceType");

							*glob_log << "        <resource xmi:idref=\"" << n_ch_re_referred_name_attr->getStringValue() << "\">" << "\n";
							*glob_log << "            <name>" << n_ch_re_referred_name_attr->getStringValue() << "</name>" << "\n";
							*glob_log << "            <resourceType>" << n_ch_re_referred_type_attr->getStringValue() << "</resourceType>" << "\n";
							*glob_log << "        </resource>" << "\n";
						}

						if (n_ch_ref_name == "SharedResourceReference")
						{
							BON::FCO n_ch_sr_referred = (*n_ch_ref)->getReferred();

							BON::Attribute n_ch_sr_referred_attr = (n_ch_sr_referred)->getAttribute("SharedresourceName");
							*glob_log << "        <sharedResource xmi:idref=\"" << n_ch_sr_referred_attr->getStringValue() << "\" />" << "\n";
						}
					}

					*glob_log << "</" << (*child_it)->getName() << ">" << "\n";
				}
				if ((*child_it)->getName() == "Interconnect")
				{
					BON::Attribute ic_name = (*child_it)->getAttribute("InterconnectName");

					BON::Attribute ic_label = (*child_it)->getAttribute("InterconnectLabel");

					*glob_log << "<interconnect xmi:id=\"" << ic_label->getStringValue() << "\">" << "\n";

					*glob_log << "        <name>" << ic_name->getStringValue() << "</name>" << "\n";
					*glob_log << "        <label>" << ic_label->getStringValue() << "</label>" << "\n";

					std::set<Reference> ic_ch_refs = (*child_it)->getChildReferences ();
				    for ( std::set<Reference>::iterator ic_ch_ref = ic_ch_refs.begin (); ic_ch_ref != ic_ch_refs.end (); ic_ch_ref++ )
					{
					   //*glob_log << "     Visited Child Reference with " << (*ic_ch_ref)->getName () << "\n";
					   std::string ic_ch_ref_name = (*ic_ch_ref)->getName ();

						if (ic_ch_ref_name == "NodeReference")
						{
							BON::FCO ic_ch_n_referred = (*ic_ch_ref)->getReferred();

							BON::Attribute ic_ch_n_referred_attr = (ic_ch_n_referred)->getAttribute("NodeLabel");
							*glob_log << "        <connect xmi:idref=\"" << ic_ch_n_referred_attr->getStringValue() << "\" />" << "\n";
						}

						if (ic_ch_ref_name == "ResourceReference")
						{
							BON::FCO ic_ch_re_referred = (*ic_ch_ref)->getReferred();

							BON::Attribute ic_ch_re_referred_name_attr = (ic_ch_re_referred)->getAttribute("ResourceName");
							BON::Attribute ic_ch_re_referred_type_attr = (ic_ch_re_referred)->getAttribute("resourceType");

							*glob_log << "        <resource xmi:idref=\"" << ic_ch_re_referred_name_attr->getStringValue() << "\">" << "\n";
							*glob_log << "            <name>" << ic_ch_re_referred_name_attr->getStringValue() << "</name>" << "\n";
							*glob_log << "            <resourceType>" << ic_ch_re_referred_type_attr->getStringValue() << "</resourceType>" << "\n";
							*glob_log << "        </resource>" << "\n";
						}

						if (ic_ch_ref_name == "BridgeReference")
						{
							BON::FCO ic_ch_br_referred = (*ic_ch_ref)->getReferred();

							BON::Attribute ic_ch_br_referred_attr = (ic_ch_br_referred)->getAttribute("BridgeLabel");
							*glob_log << "        <connection xmi:idref=\"" << ic_ch_br_referred_attr->getStringValue() << "\" />" << "\n";
						}
					}

					*glob_log << "</" << "interconnect" << ">" << "\n";
				}
				if ((*child_it)->getName() == "sharedResource")
				{
					BON::Attribute sr_name = (*child_it)->getAttribute("SharedresourceName");

					BON::Attribute sr_label = (*child_it)->getAttribute("SharedresourceType");

					*glob_log << "<sharedResource xmi:id=\"" << sr_name->getStringValue() << "\">" << "\n";

					*glob_log << "        <name>" << sr_name->getStringValue() << "</name>" << "\n";
					*glob_log << "        <resourceType>" << sr_label->getStringValue() << "</resourceType>" << "\n";

					std::set<Reference> sr_ch_refs = (*child_it)->getChildReferences ();
				    for ( std::set<Reference>::iterator sr_ch_ref = sr_ch_refs.begin (); sr_ch_ref != sr_ch_refs.end (); sr_ch_ref++ )
					{
					   //*glob_log << "     Visited Child Reference with " << (*sr_ch_ref)->getName () << "\n";

					   std::string sr_ch_ref_name = (*sr_ch_ref)->getName ();

						if (sr_ch_ref_name == "NodeReference")
						{
							BON::FCO sr_ch_n_referred = (*sr_ch_ref)->getReferred();

							BON::Attribute sr_ch_n_referred_attr = (sr_ch_n_referred)->getAttribute("NodeLabel");
							*glob_log << "        <node xmi:idref=\"" << sr_ch_n_referred_attr->getStringValue() << "\" />" << "\n";
						}

					}

					*glob_log << "</" << (*child_it)->getName() << ">" << "\n";
				}

            }
		}

		/*
		if (otype == "resource")
		{
	    	//*glob_log<< "Visited Again Resource with " << (*it)->getName() << "\n";
			BON::Attribute r_name = (*it)->getAttribute("ResourceName");

			BON::Attribute r_label = (*it)->getAttribute("resourceType");

			*glob_log << "<Resource xmi:id=\"" << r_name->getStringValue() << "\">" << "\n";

			*glob_log << "    <name>" << r_name->getStringValue() << "</name>" << "\n";
			*glob_log << "    <resourceType>" << r_label->getStringValue() << "</resourceType>" << "\n";

			std::set<Atom> model_children = (*it)->getChildAtoms ();
			for ( std::set<Atom>::iterator child_it = model_children.begin() ; child_it != model_children.end() ; child_it++ )
			{
                //*glob_log<< "      Visited Again Resource with " << (*child_it)->getName() << "\n";
			}

			*glob_log << "</" << "Resource" << ">" << "\n";
		}
		*/
	}

	/*

	for ( std::set<Model>::iterator iter = children.begin() ; iter != children.end() ; iter++ )
	{
		std::string otype = (*iter)->getObjectMeta().name();
		if (otype == "Domain")
		{
	    	*glob_log<< "Visited Again Domain with " << (*iter)->getName() << "\n";
			std::set<Model> model_childrens = (*iter)->getChildModels ();
			for ( std::set<Model>::iterator child_iter = model_childrens.begin() ; child_iter != model_childrens.end() ; child_iter++ )
			{
                *glob_log<< "     Visited Again Domain with " << (*child_iter)->getName() << "\n";
			}
		}
	}
	*/
	*glob_log << "</Deployment:Domain>" << "\n";
	AfxMessageBox("Parsed the target environment, created the domain descriptor");
	glob_log->finalize ();
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

IMPLEMENT_BONEXTENSION( Domain, "Component" );

FILE *DomainImpl::fpt;


}; // namespace BON
