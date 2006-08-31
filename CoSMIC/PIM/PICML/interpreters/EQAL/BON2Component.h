//###############################################################################################################################################
//
//	Meta and Builder Object Network V2.0 for GME
//	BON2Component.h
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
	class EventChannelConfigurationImpl;
	DECLARE_BONEXTENSION( Model, EventChannelConfigurationImpl, EventChannelConfiguration );

  class RTEC_Proxy_SupplierImpl;
  DECLARE_BONEXTENSION( Model, RTEC_Proxy_SupplierImpl, RTEC_Proxy_Supplier );

  class RTEC_Proxy_SupplierImpl;
  DECLARE_BONEXTENSION( Model, RTEC_Proxy_ConsumerImpl, RTEC_Proxy_Consumer );

  class RTEC_Resource_FactoryImpl;
	DECLARE_BONEXTENSION( Atom, RTEC_Resource_FactoryImpl, RTEC_Resource_Factory );

	class RTEC_Resource_Factory_ReferenceImpl;
	DECLARE_BONEXTENSION( Reference, RTEC_Resource_Factory_ReferenceImpl, RTEC_Resource_Factory_Reference );

	class RT_Info_ParamImpl;
	DECLARE_BONEXTENSION( Atom, RT_Info_ParamImpl, RT_Info_Param );

	class RT_InfoImpl;
	DECLARE_BONEXTENSION( RT_Info_Param, RT_InfoImpl, RT_Info );

  class OutEventPortReferenceImpl;
  DECLARE_BONEXTENSION ( Reference, OutEventPortReferenceImpl, OutEventPortReference);

  class OutEventPortImpl;
  DECLARE_BONEXTENSION ( Atom, OutEventPortImpl, OutEventPort);

  class CCMComponentImpl;
  DECLARE_BONEXTENSION ( Model, CCMComponentImpl, CCMComponent);

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

class EventChannelConfigurationImpl
	: public ModelImpl
{

};

class RTEC_Proxy_SupplierImpl
	: public ModelImpl
{

};

class RTEC_Proxy_ConsumerImpl
	: public ModelImpl
{

};

class RTEC_Resource_FactoryImpl
	: public AtomImpl
{
public:
  void generate_SVC (const char * file_name);
};

class RTEC_Resource_Factory_ReferenceImpl
	: public ReferenceImpl
{

};

class RT_Info_ParamImpl
	: public AtomImpl
{

};

class Null_RT_InfoImpl
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


class OutEventPortReferenceImpl
	: public ReferenceImpl
{

};

class OutEventPortImpl
	: public AtomImpl
{

};

class CCMComponentImpl
	: public ModelImpl
{

};

class EventChannelConfigurationVisitor
	: public Visitor
{

  public:

		virtual void visitModelImpl (const Model& model);

  private:

    void visitEventChannelConfigurationImpl (const EventChannelConfiguration& ec_configuration);

    void visitRTEC_Proxy_SupplierImpl (const RTEC_Proxy_Supplier & rtec_proxy_supp);

    void visitRTEC_Proxy_ConsumerImpl (const RTEC_Proxy_Consumer & rtec_proxy_cons);
};

class RTEC_Proxy_SupplierVisitor
  : public Visitor
{
  public:

    virtual void visitAtomImpl (const Atom& atom);

  private:

    virtual void visitRT_InfoImpl (const RT_Info& rt_info);
};


class RTEC_Proxy_ConsumerVisitor
  : public Visitor
{
  public:

    virtual void visitAtomImpl (const Atom& atom);

  private:

    virtual void visitRT_InfoImpl (const RT_Info& rt_info);
};

}; // namespace BON

#endif // Bon2Component_H