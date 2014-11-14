$Id$

This test uses a TimeTrigger-Service-Connector.
The connector self registrates the service-reference with the install_service_component_reference method.
If this has taken place a user component can after calling resolve_service_reference method,
make use of this TimeTrigger Service Connector.
The deployment plan, only has an instance of the service-connector and a
localityContraint 'SameProcess' for the user-component and the Timetrigger-Sservice-Connector.


Compare this test with the TTSCTest, in this test the service-connector is registrated via the deploymentplan
with the extra ConfigProperties edu.vanderbilt.dre.CIAO.ServiceReference with value TimeService and
edu.vanderbilt.dre.CIAO.ServiceReference.TimeService with value portname.






