// cpp(meta datanetwork format) source file CrossLinks.cpp generated from diagram CrossLinks
// generated on Mon Nov 15 23:04:45 2004

#include "CrossLinks.h"
#include "UmlExt.h"

#include "UdmStatic.h"

// cross-package metainformation header file
#include "Cadena2PICML.h"

namespace CrossLinks {

	Uml::Diagram umldiagram;

	void Creates()
	{
	}
	void InitClassesAttributes()
	{
		
		
	}
	 void Initialize()
	{
		static bool first = true;
		if(!first) return;
		first = false;
		Uml::Initialize();

		Cadena2PICML::Initialize();

		ASSERT( umldiagram == Udm::null );
		UdmStatic::StaticDataNetwork * meta_dn = new UdmStatic::StaticDataNetwork(Uml::diagram);
		meta_dn->CreateNew("CrossLinks.mem", "", Uml::Diagram::meta, Udm::CHANGES_LOST_DEFAULT);
		umldiagram = Uml::Diagram::Cast(meta_dn->GetRootObject());
		umldiagram.name() ="CrossLinks";
		umldiagram.version() ="1.00";
		Creates();
		InitClassesAttributes();
		Uml::InitDiagram(umldiagram, "CrossLinks", "1.00");

		
	}
	void Initialize(const Uml::Diagram & dgr)
	{
		umldiagram = dgr;
		//composition child roles 
		// composition parentroles 
		// Association roles 
		
	}
	 Udm::UdmDiagram diagram = { &umldiagram, Initialize };
	static struct _regClass
	{
		_regClass()
		{
			Udm::MetaDepository::StoreDiagram("CrossLinks", diagram);
		};
		~_regClass()
		{
			Udm::MetaDepository::RemoveDiagram("CrossLinks");
		};
	} __regUnUsed;

}
// END CrossLinks.cpp
