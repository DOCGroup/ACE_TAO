-------------------------------------------------------------
Startup procedure for running the implicit System Trace Test
with Tornado/VxWorks.
The pathes and IP/port addresses are examples
-------------------------------------------------------------

(1) VxWorks Image linked statically with ACE, TAO, DIOP and SystemTrace Objects
In Tornado project in Properties/Macros/EXTRA_MODULES:
e:\ACE\ACE_wrappers\ace\libACE.so e:\ACE\ACE_wrappers\ace\libTAO.so K:\Siemens\NodeB\appl\testAppl\OCI\SystemTrace\SystemTraceMan.o e:\ACE\ACE_wrappers\tao\tao\libTAO_DIOP.so

(2) System Trace Server
# K:\Siemens\NodeB\appl\testAppl\OCI\SystemTrace\SysTraceServer\SystemTraceServer.exe
# Runs with NT in DOS-Box.
# Call: 
SystemTraceServer port
# Default Port:  2001

(3) Implicit Trace Test: client-server
# Load Server and Client on PPC-Target:
cd "K:\Siemens\NodeB\appl\testAppl\OCI\SystemTrace_Neu\test\ImplPerform"
ld < server
ld < client
niter=100
SIZE_BLOCK = 256
# start TraceConfigurator server (IIOP) and Test-Server (DIOP)
spa testServer, "iiop://192.68.154.2:21000", "diop://192.68.154.2:22001"
#-> generates NB_TraceConfig.ior
# Now the Trace Configuration Client should be started on NT
#-> reads NB_TraceConfig.ior
#-> Command: Set Destination address 193.98.154.35
#-> Command: Implicit Trace Point, ...
# start Test-Client (sends some CORBA/DIOP calls)
spa testClient
# -> generates test.ior
