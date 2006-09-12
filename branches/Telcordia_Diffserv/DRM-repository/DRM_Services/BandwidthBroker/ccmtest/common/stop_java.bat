@echo off
rem # ====================================================================
rem #
rem # OpenCCM: The Open CORBA Component Model Platform
rem # Copyright (C) 2000-2003 INRIA - USTL - LIFL - GOAL
rem # Contact: openccm-team@objectweb.org
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
rem #                 Christophe Demarey
rem #                 Briclet Frédéric.
rem #
rem # ====================================================================
rem #
rem # Stopping a demonstration.
rem #
rem # ====================================================================

call tear_down %1%

rem # ============================
rem # Stop Java Component Servers.
rem # ============================
call jcs_stop ComponentClient
call jcs_stop ComponentBWServer
call jcs_stop ComponentFPServer


rem # ============================
rem # Stop Java Component Servers.
rem # ============================
call node_stop MainNode

rem # ============================
rem # Stop Java Component Servers.
rem # ============================
call factory_stop DefaultFactory

rem # ============================
rem # Stop Java DCI.
call dci_stop DefaultDCI

rem # ==================================================
rem # Stop the COACH Tracing Server (if any is running)
call coach_tracing_stop

rem # ============================
rem # Stop the Name Service.
rem # ============================
rem call ns_stop

rem # =======================================
rem # Stop the Transaction Service if needed.
rem # =======================================

rem # Obtain the IOR of the started Transaction Service.
call ots_ior > %OpenCCM_CONFIG_DIR%\stop_java.tmp
for /f %%i in (%OpenCCM_CONFIG_DIR%\stop_java.tmp) do set OTSIOR=%%i
del %OpenCCM_CONFIG_DIR%\stop_java.tmp
if defined OTSIOR (call ots_stop)

rem # =======================================
rem # Stop the Trading Service if needed.
rem # =======================================

rem # Obtain the IOR of the started Trading Service.
call trader_ior > %OpenCCM_CONFIG_DIR%\stop_java.tmp
for /f %%i in (%OpenCCM_CONFIG_DIR%\stop_java.tmp) do set TRADERIOR=%%i
del %OpenCCM_CONFIG_DIR%\stop_java.tmp
if defined TRADERIOR (call trader_stop)

rem # ==============================
rem # Deinstall the OpenCCM Platform.
rem # ==============================
call ccm_deinstall 
