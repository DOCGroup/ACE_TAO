#ifndef MOBIES_CROSSLINKS_H
#define MOBIES_CROSSLINKS_H
// header file CrossLinks.h generated from diagram CrossLinks
// generated on Mon Nov 15 23:04:45 2004

#ifndef MOBIES_UDMBASE_H
#include "UdmBase.h"
#endif

namespace PICML
{ 
	 class  PublishConnector;
	 class  invoke;
	 class  emit;
	 class  ComponentAssembly;
	 class  Property;
	 class  OutEventPort;
	 class  ProvidedRequestPort;
	 class  RequiredRequestPort;
	 class  InEventPort;
	 class  Port;
	 class  Component;
};
namespace PICMLCadena_Xchange_embedded_EIProperties
{ 
	 class  RootContainer;
};
namespace cadena_scenario
{ 
	 class  ESSConnection;
	 class  FRConnection;
	 class  EventSink;
	 class  EventSource;
	 class  Receptacle;
	 class  Facet;
	 class  ComponentPort;
	 class  Property;
	 class  ComponentInstance;
	 class  Scenario;
};


namespace CrossLinks {


	  void Initialize();

	  void Initialize(const Uml::Diagram &dgr);
	extern  Udm::UdmDiagram diagram;

}

#endif //MOBIES_CROSSLINKS_H
