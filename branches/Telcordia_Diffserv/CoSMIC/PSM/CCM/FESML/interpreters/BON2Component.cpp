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
#include <strstream> 

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

std::string indent = "  "; // two blank spaces for indentation purpose.

void Component::invokeEx( Project& project, FCO& currentFCO, const std::set<FCO>& setSelectedFCOs, long lParam )
{
	// ======================
	// Insert application specific code here

  Federated_EC root = Federated_EC (currentFCO);

	if (!root) {
		AfxMessageBox ("Interpretation must start from a model element!");
		return;
	}

	std::string filename;
 	
  //All atoms in the diagram, saved as the same order as in the diagram
	std::set<BON::Atom> atoms;

  //All models in the diagram, saved as the same order as in the diagram
  std::set<BON::Model> models; 

	filename = root->getName ();
	filename = filename + ".xml";
	ofstream outf;
  
  outf.open (filename.c_str ());

	outf << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl << endl;
  outf << "<!DOCTYPE componentassembly SYSTEM \"http://www.dre.vanderbilt.edu/CIAO/DTD/componentassembly.dtd\">" << endl << endl;

  outf << "<!-- This is part of of the component assembly descriptor file to specify " << endl;
  outf << "     federated event service. -->" << endl << endl;

  outf << "<componentassembly id=\"distributed assembly\">" << endl;
  outf << indent << "<partitioning>" << endl;

	root->traverse (atoms, models);
	root->check (atoms, models);
	root->writeFile (atoms, models, outf);

	AfxMessageBox (CString("The XML file ") +filename.c_str () +
		CString (" has created sucessfully!"));

	return;
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


//###############################################################################################################################################
//
// 	C L A S S : BON::Federated_ECImpl
//
//###############################################################################################################################################

IMPLEMENT_BONEXTENSION(Federated_EC, "Federated_EC");

void Federated_ECImpl::initialize ()
{

}

void Federated_ECImpl::traverse (std::set<BON::Atom> &atoms, std::set<BON::Model> &models)
{
  std::set<BON::Atom> atomsInModel;

  models = getChildModels (); // These models are "sites".

  // Iterate over each site, put all the atoms in each site into the atoms set <atoms>.
  for (std::set<BON::Model>::iterator it = models.begin (); it != models.end (); it ++)
		{
      atomsInModel = (*it)->getChildAtoms ();
      for (std::set<BON::Atom>::iterator it = atomsInModel.begin (); it != atomsInModel.end (); it ++)
        {
          atoms.insert (*it);
        }
    }
}

void Federated_ECImpl::check (std::set<BON::Atom> &atoms, std::set<BON::Model> &models)
{

}

void Federated_ECImpl::writeFile (std::set<BON::Atom> &atoms, std::set<BON::Model> &models, ofstream &outf)
{
  
  // each host has a strstream corresponding to it, which outputs
  // XML elments <hostcollocation>.
  std::strstream* host_str = new std::strstream[models.size ()]; 

  // Iterate over each "site" to create the beginning part of the <hostcollocation> element,
  // which contains the name of each site as the "id" attribute of <hostcollocation> element.
  int siteID = 0;
  {
  for (std::set<BON::Model>::iterator it = models.begin (); it != models.end (); it ++, siteID ++)
    {
      std::string site_name = (*it)->getName ().c_str ();

      host_str[siteID] << "\t<hostcollocation id=\"" << site_name << "\">" << endl;
      host_str[siteID] << "\t\t<extension class=\"CORBA_Gateway\" origin=\"CIAO\">" << endl;
      host_str[siteID] << "\t\t\t\"" << endl;
    }
  }

 	std::string atomName;

  // Iterate over each "CORBA_Gateway" to create the XML extension.
  for (std::set<Atom>::iterator it = atoms.begin (); it != atoms.end (); it ++)
    {
      atomName = (*it)->getAtomMeta().name ();

      if (atomName == "CORBA_Gateway")
        {
          // Get the parent Site Node reference and site name.
          BON::Model parentSite = (*it)->getParentModel ();
          std::string localSiteName = parentSite->getName ().c_str ();
          
          
          // Get the Source (Remote) event channel name
          std::multiset<BON::ConnectionEnd> srcs = 
            BON::ConnectionEnd (*it) -> getConnEnds (MON::Connection (), "src");

          if (int(srcs.size ()) != 1)
            {
              AfxMessageBox("Error: Each CORBA_Gateway must have exactly one remote event channel!");
              break;
            }

          std::multiset<BON::ConnectionEnd>::iterator src_it = srcs.begin (); 
          std::string srcEcName = BON::FCO (*src_it)->getName().c_str ();

          // Get the remote Site name
          std::string remoteSiteName = BON::FCO (*src_it)->getParentModel()->getName().c_str();

          // Get the Destination (local) event channel name
          std::multiset<BON::ConnectionEnd> dsts = 
            BON::ConnectionEnd (*it) -> getConnEnds (MON::Connection (), "dst");

          if (int(dsts.size ()) != 1)
            {
              AfxMessageBox("Error: Each CORBA_Gateway must have exactly one local event channel!");
              break;
            }

          std::multiset<BON::ConnectionEnd>::iterator dst_it = dsts.begin (); 
          std::string dstEcName = BON::FCO (*dst_it)->getName ();


          // Identify the index of the site in the models set by
          // checking the name of the site.
          int site_index = 0;
          {
          for (std::set<BON::Model>::iterator it = models.begin (); it != models.end (); it ++)
            {
              if (strcmp ( localSiteName.c_str(), (*it)->getName().c_str() ) == 0)
                break;
              else
                site_index++;
            }
          }
          // Out put the extension part, a string, of the <extension> element.
          host_str[site_index] << "\t\t\tsource_ec=" << remoteSiteName << "_" << srcEcName << " "
                               << "dest_ec=" << localSiteName << "_" << dstEcName << endl;

      } // end of <if atomName == "CORBA_Gateway">
    } // end of for iteration over atoms set

  // Out put the end part of the <hostcollocation> element.
  siteID = 0;
  {
  for (std::set<BON::Model>::iterator it = models.begin (); it != models.end (); it ++, siteID ++)
    {
      std::string site_name = (*it)->getName ().c_str ();

      host_str[siteID] << "\t\t\t\"" << endl;
      host_str[siteID] << "\t\t</extension>"<< endl;
      host_str[siteID] << "\t\t<destination>" << site_name << "</destination>" << endl;
      host_str[siteID] << "\t</hostcollocation>" << endl << endl;
  
      outf << host_str[siteID].str ();
    }
  }

  outf << indent << "</partitioning>" << endl;
  outf << "</componentassembly>" << endl;

  outf.close ();
}

}; // namespace BON

