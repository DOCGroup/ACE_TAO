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
#include "Events.h"
#include "fstream.h"
#include <ComponentConfig.h>

namespace BON
{
  class Federated_ECImpl;
  DECLARE_BONEXTENSION(Model, Federated_ECImpl, Federated_EC);


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

//###############################################################################################################################################
//
// 	C L A S S : BON::Federated_ECImpl
//
//###############################################################################################################################################

class Federated_ECImpl 
	: public ModelImpl
{

	private:

	public:
		//initialize table
		void initialize ();

		//Traverse the diagram and fill in the set atoms, set models
    void traverse (std::set<BON::Atom> &atoms, std::set<BON::Model> &models);

		//Check the diagram and give some warning informations, if necessary.
		void check (std::set <BON::Atom> &atoms, std::set<BON::Model> &models);

		//Generate XML file
		void writeFile (std::set <BON::Atom> &atoms, std::set<BON::Model> &models, ofstream &ftp);
};



}; // namespace BON

#endif // Bon2Component_H