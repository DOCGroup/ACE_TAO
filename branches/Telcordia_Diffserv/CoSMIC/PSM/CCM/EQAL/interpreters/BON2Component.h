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

#include <iostream>
#include <fstream>

namespace BON
{

	class Component_AssemblyImpl;
	DECLARE_BONEXTENSION( Model, Component_AssemblyImpl, Component_Assembly );

	class CCMComponentImpl;
	DECLARE_BONEXTENSION( Model, CCMComponentImpl, CCMComponent );

	class Event_SourceImpl;
	DECLARE_BONEXTENSION( Atom, Event_SourceImpl, Event_Source );

	class Event_SinkImpl;
	DECLARE_BONEXTENSION( Atom, Event_SinkImpl, Event_Sink );

	class NS_Resource_FactoryImpl;
	DECLARE_BONEXTENSION( Atom, NS_Resource_FactoryImpl, NS_Resource_Factory );

	class NS_Resource_Factory_ReferenceImpl;
	DECLARE_BONEXTENSION( Reference, NS_Resource_Factory_ReferenceImpl, NS_Resource_Factory_Reference );

	class RTEC_Resource_FactoryImpl;
	DECLARE_BONEXTENSION( Atom, RTEC_Resource_FactoryImpl, RTEC_Resource_Factory );

	class RTEC_Resource_Factory_ReferenceImpl;
	DECLARE_BONEXTENSION( Reference, RTEC_Resource_Factory_ReferenceImpl, RTEC_Resource_Factory_Reference );

	class NS_QoSImpl;
	DECLARE_BONEXTENSION( Atom, NS_QoSImpl, NS_QoS );

	class RT_Info_ParamImpl;
	DECLARE_BONEXTENSION( Atom, RT_Info_ParamImpl, RT_Info_Param );

	class RTEC_Filter_GroupImpl;
	DECLARE_BONEXTENSION( Atom, RTEC_Filter_GroupImpl, RTEC_Filter_Group );

	class RT_InfoImpl;
	DECLARE_BONEXTENSION( RT_Info_Param, RT_InfoImpl, RT_Info );

	class Null_RT_InfoImpl;
	DECLARE_BONEXTENSION( RT_Info_Param, Null_RT_InfoImpl, Null_RT_Info );

	class DisjunctionGroupImpl;
	DECLARE_BONEXTENSION( RTEC_Filter_Group, DisjunctionGroupImpl, DisjunctionGroup );

	class ConjunctionGroupImpl;
	DECLARE_BONEXTENSION( RTEC_Filter_Group, ConjunctionGroupImpl, ConjunctionGroup );

	class LogicalANDGroupImpl;
	DECLARE_BONEXTENSION( RTEC_Filter_Group, LogicalANDGroupImpl, LogicalANDGroup );

	class eventtypeImpl;
	DECLARE_BONEXTENSION( Atom, eventtypeImpl, eventtype );

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

class Component_AssemblyImpl
	: public ModelImpl
{

};

class CCMComponentImpl
	: public ModelImpl
{

};

class Event_SourceImpl
	: public AtomImpl
{

};

class Event_SinkImpl
	: public AtomImpl
{

};

class RTEC_Resource_FactoryImpl
	: public AtomImpl
{
public:
  void generate_SVC (const char * file_name);
};

class NS_Resource_FactoryImpl
	: public AtomImpl
{
public:
  void generate_SVC (const char * file_name);
};

class RTEC_Resource_Factory_ReferenceImpl
	: public ReferenceImpl
{

};

class NS_Resource_Factory_ReferenceImpl
	: public ReferenceImpl
{

};

class NS_QoSImpl
	: public AtomImpl
{
public:
  void generate_CPF (const char * file_name, const char * component_name, const char * port_name);
};

class RT_Info_ParamImpl
	: public AtomImpl
{

};

class RTEC_Filter_GroupImpl
	: public AtomImpl
{

};

class RT_InfoImpl
	: public RT_Info_ParamImpl
{
public:
  void generate_CPF (const char * file_name, const char * component_name, const char * port_name);
};

class Null_RT_InfoImpl
	: public RT_Info_ParamImpl
{

};

class DisjunctionGroupImpl
	: public RTEC_Filter_GroupImpl
{

};

class ConjunctionGroupImpl
	: public RTEC_Filter_GroupImpl
{

};

class LogicalANDGroupImpl
	: public RTEC_Filter_GroupImpl
{

};

class eventtypeImpl
	: public AtomImpl
{

};

class AssemblyVisitor
	: public Visitor
{

  public:

		virtual void visitAtomImpl (const Atom& atom);

		virtual void visitModelImpl (const Model& model);

  private:

    void visitComponent_AssemblyImpl (const Component_Assembly& assembly);

    void visitEvent_SourceImpl (const Event_Source& event_source);

    void visitEvent_SinkImpl (const Event_Sink& event_sink);

};

class ECDVisitor
	: public Visitor
{

  public:

    ECDVisitor (const char * component_name, const char * sink_name);

    ~ECDVisitor ();

		virtual void visitAtomImpl (const Atom& atom);

  private:

    void visitDisjunctionGroupImpl (const DisjunctionGroup& disjunction);

    void visitConjunctionGroupImpl (const ConjunctionGroup& conjunction);
  
    void visitLogicalANDGroupImpl (const LogicalANDGroup& logicaland);

    void visitNull_RT_InfoImpl (const Null_RT_Info& null_rt_info);

    void visitRT_InfoImpl (const RT_Info& rt_info);

    std::ofstream file_out_;

    int indent_;

    std::string component_name_;

    std::string sink_name_;

};

}; // namespace BON

#endif // Bon2Component_H