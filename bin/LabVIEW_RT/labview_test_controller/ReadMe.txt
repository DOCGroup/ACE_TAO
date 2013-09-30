$Id$

========================================================================
    DYNAMIC LINK LIBRARY : labview_test_controller Project Overview
========================================================================

The labview_test_controller DLL is needed to control execution and reporting
for ACE+TAO tests on a LabVIEW RT/Pharlap ETS target. The LabVIEW RT
environment can not execute programs - LabVIEW RT itself is the one program
that runs on the Pharlap ETS base, so no other processes can be started.
This DLL is built on the host VC7.1 Windows machine and copied to the
target. The target's \ni-rt.ini file must have the full path of the
labview_test_controller.dll on the target added to the [STARTUP] section,
EarlyStartupLibraries key. If there are already entries in that key,
append labview_test_controller.dll's path with a semi-colon separator.
For example, this is what mine ended up as:
[STARTUP]
EarlyStartupLibraries=C:\ni-rt\system\lvuste.dll;C:\ni-rt\system\tsengine.dll;C:\ni-rt\system\nisl_emb.dll;C:\ni-rt\labview_test_controller.dll;
MainExe=/ni-rt/system/emblview.exe

After setting up the ni-rt.ini file and the DLL, reboot the target to
load the controller DLL.

Keep in mind that the current working directory while tests are running will
be, at least on mine, \ni-rt\system. The test logs end up in \ni-rt\system\log.

The labview_test_controller spawns a thread when the DLL is loaded at
system start. The child thread starts listening on TCP port 8888. For each
new connection to port 8888 a new thread is spawned to run the session.
The test host issues text commands to the controller via this TCP connection.
The responses are text as well. Thus, it's possible to test it using plain
telnet. There's also a "test" project in this VC7.1 solution. The test
project is a simple console app that calls the main thread's entrypoint
in labview_test_controller, facilitating its testing on the host Windows
machine where there's decent debugging capability. Debugging on the LabVIEW
target is by printf only.

The client side of the TCP connection that drives this test controller is
in the ACE_wrappers/bin/PerlACE/ProcessLVRT.pm. The TestTarget_LVRT.pm is
also involved in testing on the LabVIEW RT target.

The remainder of this is what Visual Studio .NET generated for this project.
========================================================================
AppWizard has created this labview_test_controller DLL for you.
This file contains a summary of what you will find in each of the files that
make up your labview_test_controller application.


labview_test_controller.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

labview_test_controller.cpp
    This is the main DLL source file.

	When created, this DLL does not export any symbols. As a result, it
	will not produce a .lib file when it is built. If you wish this project
	to be a project dependency of some other project, you will either need to
	add code to export some symbols from the DLL so that an export library
	will be produced, or you can set the Ignore Input Library property to Yes
	on the General propert page of the Linker folder in the project's Property
	Pages dialog box.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named labview_test_controller.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
