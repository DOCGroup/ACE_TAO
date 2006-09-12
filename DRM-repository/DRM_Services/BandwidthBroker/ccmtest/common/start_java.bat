@echo off
rem # ====================================================================
rem #
rem # OpenCCM: The Open CORBA Component Model Platform
rem # Copyright (C) 2000-2004 INRIA - USTL - LIFL - GOAL
rem # Contact: openccm@objectweb.org
rem #
rem # This library is free software; you can redistribute it and/or
rem # modify it under the terms of the GNU Lesser General Public
rem # License as published by the Free Software Foundation; either
rem # version 2.1 of the License, or any later version.
rem #
rem # This library is distributed in the hope that it will be useful,
rem # but WITHOUT ANY WARRANTY; without even the implied warranty of
rem # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
rem # Lesser General Public License for more details.
rem #
rem # You should have received a copy of the GNU Lesser General Public
rem # License along with this library; if not, write to the Free Software
rem # Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
rem # USA
rem #
rem # Initial developer(s): Philippe Merle.
rem # Contributor(s): Marc Navarro (mnavarro@wins.uva.nl),
rem #                 Christophe Demarey, Jerome Offroy,
rem #                 Briclet Frédéric, Jerome Moroy
rem #
rem # ====================================================================
rem #
rem # Starting a demonstration.
rem #
rem # %1: demonstration name.
rem #
rem # ====================================================================

@setlocal

rem # =====================
rem # Compute Command Line.
rem # =====================

set /a nb_args=0
for %%i in (%*) do set /a nb_args=nb_args+1

set demo_name=%1
shift
set /a nb=1
set with_ots="false"
set with_trace="flase"
set with_tracing="false"
set xml="true"
set with_trader="false"
set run_id=cif
set services_args=
set services_list=
set assembly_zip=archives/%demo_name%.aar

:initial
if "%1" == "-h" goto help
if "%1" == "--ots" goto ots
if "%1" == "--trace" goto trace
if "%1" == "--tracing" goto tracing
if "%1" == "--monolithic" goto mono
if "%1" == "--xml" goto xml
if "%1" == "--trader" goto trader
if not "%1" == "" goto help
if %nb% == %nb_args% (goto end_command_line) else (goto arguments)

:ots
set with_ots="true"
set services_list=%services_list% Transactional
set services_args=%services_args% --ots
shift
set /a nb=nb+1
goto initial

:trace
set with_trace="true"
set services_list=%services_list% Trace
set services_args=%services_args% --trace
shift
set /a nb=nb+1
goto initial

:tracing
set with_tracing="true"
set services_list=%services_list% Tracing
set services_args=%services_args% --tracing
shift
set /a nb=nb+1
goto initial

:mono
set run_id=monolithic
shift
set /a nb=nb+1
goto initial

:xml
set xml="true"
shift
set /a nb=nb+1
goto initial

:trader
set with_trader="true"
shift
set /a nb=nb+1
goto initial

:arguments
shift
set /a nb=nb+1
goto initial

:help
echo usage: start_java ^<demo_name^> [--ots] [--trace] [--tracing] [--monolithic ^| --xml ]
goto :eof

:end_command_line

rem # ==========================
rem # Ant compilation if needed.
rem # ==========================

if not exist generated\idl\%demo_name%.idl (call build.bat)
if not exist archives\%demo_name%.jar (call build.bat)

rem # =============================
rem # Install the OpenCCM Platform.
rem # =============================

call ccm_install

rem # =============================
rem # Start the Name Service.
rem # =============================

if exist %OpenCCM_CONFIG_DIR%\NameService.IOR del %OpenCCM_CONFIG_DIR%\NameService.IOR


rem copy %NS_IOR% %OpenCCM_CONFIG_DIR%\NameService.IOR
rem echo The CosNaming Service is set.
rem call ns_set %NS_IOR%
call ns_start

rem # ==============================
rem # Start the Transaction Service.
rem # ==============================

if %with_ots% == "true" (call ots_start)

rem # ==============================
rem # Start the Trading Service.
rem # ==============================

if %with_trader% == "true" (call trader_start)

rem # ==============================
rem # Start the Coach Tracing Service.
rem # ==============================

if %with_tracing% == "true" (call coach_tracing_start)

rem # ===========================================
rem # Obtain the IOR of the started Name Service.
rem # ===========================================

call ns_ior > %OpenCCM_CONFIG_DIR%\%demo_name%.tmp
for /f %%i in (%OpenCCM_CONFIG_DIR%\%demo_name%.tmp) do set ior_NS=%%i
echo %ior_NS%
del %OpenCCM_CONFIG_DIR%\%demo_name%.tmp

rem # ==================================================
rem # Obtain the IOR of the started Transaction Service.
rem # ==================================================

if %with_ots% == "false" goto :jcs
call ots_ior > %OpenCCM_CONFIG_DIR%\%demo_name%.tmp
for /f %%i in (%OpenCCM_CONFIG_DIR%\%demo_name%.tmp) do set ior_OTS=%%i
del %OpenCCM_CONFIG_DIR%\%demo_name%.tmp

rem # =============================
rem # Start Java Component Servers.
rem # =============================

:jcs
call jcs_start %services_args% ComponentClient
:wait1
if not exist %OpenCCM_CONFIG_DIR%\ComponentServers\ComponentClient.IOR goto :wait1

if not exist %OpenCCM_CONFIG_DIR%\ComponentServers\ComponentClient.output goto :wait1

if not exist %OpenCCM_CONFIG_DIR%\ComponentServers\ComponentClient.output.tmp goto :wait1

if not exist %OpenCCM_CONFIG_DIR%\ComponentServers\ComponentClient.PID goto :wait1



call jcs_start %services_args% ComponentBWServer
:wait2 if not exist %OpenCCM_CONFIG_DIR%\ComponentServers\ComponentBWServer.IOR goto :wait2

if not exist %OpenCCM_CONFIG_DIR%\ComponentServers\ComponentBWServer.output goto :wait2

if not exist %OpenCCM_CONFIG_DIR%\ComponentServers\ComponentBWServer.output.tmp goto :wait2

if not exist %OpenCCM_CONFIG_DIR%\ComponentServers\ComponentBWServer.PID goto :wait2
call jcs_start %services_args% ComponentFPServer
:wait3
if not exist %OpenCCM_CONFIG_DIR%\ComponentServers\ComponentFPServer.IOR goto :wait3

if not exist %OpenCCM_CONFIG_DIR%\ComponentServers\ComponentFPServer.output goto :wait3

if not exist %OpenCCM_CONFIG_DIR%\ComponentServers\ComponentFPServer.output.tmp goto :wait3

if not exist %OpenCCM_CONFIG_DIR%\ComponentServers\ComponentFPServer.PID goto :wait3


rem # =============================
rem # Start Java Node.
rem # =============================

call dci_start %services_args% DefaultDCI

rem # =============================
rem # Start Java AssemblyFactory.
rem # =============================

call factory_start %services_args% DefaultFactory

rem # =============================
rem # Start Java Node.
rem # =============================

call node_start %services_args% MainNode

rem # ==================
rem # Run demonstration.
rem # ==================

:run

echo Starting demonstration %demo_name% with [%services_list%] deployment
if %xml% == "false" goto java
echo calling ccm_deploy %assembly_zip%
call ccm_deploy %services_args% -F DefaultFactory %assembly_zip%
goto end

:java
set jvm_args=-Dior_ns=%ior_NS%

:ots_service
if %with_ots% == "false" goto trace_service
set jvm_args=%jvm_args% -Dior_ots=%ior_OTS% set run_id=%run_id%_ots

:trace_service
if %with_trace% == "false" goto tracing_service
set run_id=%run_id%_trace

:tracing_service
if %with_tracing% == "false" goto deploy
set run_id=%run_id%_tracing

:deploy set jvm_args=%jvm_args%
call launcher exec "%jvm_args%" "--runid %run_id% %demo_name%.xml"

:end
echo The demonstration %demo_name% is ready to be used ...

:browser
call ccm_browser
:myend
