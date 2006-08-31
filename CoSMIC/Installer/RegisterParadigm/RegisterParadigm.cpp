// $Id$
// RegisterParadigm.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>
#include <msiquery.h>
#include <objbase.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <string>
#include <vector>

#include "mgautil.h"

#define PARADIGMCOST 30000


BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call,
                      LPVOID lpReserved)
{
  return TRUE;
}

const TCHAR* LastError()
{
  static TCHAR lpMsgBuf[65536];
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ::GetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                lpMsgBuf, sizeof(lpMsgBuf), NULL);
  return lpMsgBuf;
}


void InitProgressBar(MSIHANDLE hInstall, int nSteps, int nStepSize,
                     const char* szActionName,
                     const char* szDescription,
                     const char* szTemplate)
{
  MSIHANDLE nTotalCost = nSteps * nStepSize;
  PMSIHANDLE hRec = MsiCreateRecord(3);

  MsiRecordSetString(hRec, 1, szActionName);	// action name
  MsiRecordSetString(hRec, 2, szDescription);	// description
  MsiRecordSetString(hRec, 3, szTemplate);	// template for ACTIONDATA
  int nResult = MsiProcessMessage(hInstall,
                                  INSTALLMESSAGE_ACTIONSTART,
                                  hRec);

  MsiRecordSetInteger(hRec, 1, 0);	    // reset PB, set the total ticks
  MsiRecordSetInteger(hRec, 2, nTotalCost); // est. total ticks
  MsiRecordSetInteger(hRec, 3, 0);          // forward direction
  nResult = MsiProcessMessage(hInstall,
                              INSTALLMESSAGE_PROGRESS,
                              hRec);

  MsiRecordSetInteger(hRec, 1, 1);         // progress setup info
  MsiRecordSetInteger(hRec, 2, nStepSize); // step size
  MsiRecordSetInteger(hRec, 3, 1);         // increment by the prev parameter
  nResult = MsiProcessMessage(hInstall,
                              INSTALLMESSAGE_PROGRESS,
                              hRec);
}


void SendMsgToProgressBar(MSIHANDLE hInstall,
                          const char* szMessage)
{
  MSIHANDLE hProgressRec = MsiCreateRecord(3);
  MSIHANDLE hRec = MsiCreateRecord(1);

  MsiRecordSetInteger(hProgressRec, 1, 2);	// increment the PB
  MsiRecordSetInteger(hProgressRec, 2, 1);	// ignored
  MsiRecordSetInteger(hProgressRec, 3, 0);	// unused

  MsiRecordSetString(hRec, 1, szMessage); // set the progress bar message

  int nResult = MsiProcessMessage(hInstall,
                                  INSTALLMESSAGE_ACTIONDATA,
                                  hRec);
  nResult = MsiProcessMessage(hInstall,
                              INSTALLMESSAGE_PROGRESS,
                              hProgressRec);
}


void SendErrorMsg(MSIHANDLE hInstall, const char* svErrorMessage, int nFatal)
{
  int hRec = MsiCreateRecord(1);
  MsiRecordSetString(hRec, 0, "Error occured: [1]");
  MsiRecordSetString(hRec, 1, svErrorMessage);
  MsiProcessMessage(hInstall,
                    INSTALLMESSAGE_ERROR,
                    hRec);
}

char* GetRegistryValue (HKEY key, const char* keyName, const char* name)
{
  HKEY hKeyLocal;

  // Open the registry key key
  if (RegOpenKeyEx (key, keyName, 0, KEY_READ, &hKeyLocal) != ERROR_SUCCESS)
    return 0;

  // Get the maximum length of a value inside this key
  unsigned long len;
  if (RegQueryInfoKey (hKeyLocal, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                       NULL, &len, NULL, NULL) != ERROR_SUCCESS)
    return 0;
  len++;
  unsigned char* buf = new unsigned char[len];

  // Query the registry value of name
  if (RegQueryValueEx(hKeyLocal, name, NULL, NULL, buf, &len) != ERROR_SUCCESS)
    return 0;

  if (RegCloseKey (hKeyLocal) != ERROR_SUCCESS)
    return 0;

  return (char*)buf;
}


bool RegisterParadigm (const std::string& paradigm)
{
  //Initialize the OLE libraries
  HRESULT hr = ::CoInitialize(NULL);
  if (FAILED(hr))
    return false;

  CLSID clsid;

  hr = ::CLSIDFromProgID(OLESTR("Mga.MgaRegistrar"), &clsid);
  if (FAILED(hr))
    return false;

  IMgaRegistrar *reg;
  hr = ::CoCreateInstance(clsid, NULL, CLSCTX_ALL, __uuidof(IMgaRegistrar),
                          reinterpret_cast<void**>(&reg));
  if (FAILED(hr))
    return false;

  UINT cp = GetACP ();
  int len = ::MultiByteToWideChar (cp, 0, paradigm.c_str(), -1, 0, 0);
  wchar_t *wstr = new wchar_t[len];
  ::MultiByteToWideChar (cp, 0, paradigm.c_str(), -1, wstr, len);
  BSTR connstr = ::SysAllocString(wstr);
  BSTR parname = NULL;

  hr = reg->RegisterParadigmFromData(connstr, &parname,
                                     REGACCESS_SYSTEM);
  if (FAILED(hr))
    return false;

  ::SysFreeString(connstr);
  ::SysFreeString(parname);

  reg->Release();

  ::CoUninitialize();
  return true;
}



UINT __stdcall RegisterParadigms(MSIHANDLE hInstall)
{
  // installable paradigm number
  int nParadigmNum = 1;

  // Initialize progress bar
  InitProgressBar(hInstall, nParadigmNum, PARADIGMCOST,
                  "Paradigm Install",
                  "Registering Paradigms into GME.", "Registering [1]");
  char* value = GetRegistryValue (HKEY_LOCAL_MACHINE,
                                  "SOFTWARE\\ISIS\\CoSMIC",
                                  "TargetDir");
  if (value == 0)
    {
      SendErrorMsg (hInstall, "Unable to access Registry Key "
                    "HKEY_LOCAL_MACHINE\\SOFTWARE\\ISIS\\CoSMIC\\TargetDir", 1);
      return 0;
    }

  std::vector<std::string> paradigms;
  paradigms.push_back ("PICML.xmp");

  for (std::vector<std::string>::const_iterator iter = paradigms.begin();
       iter != paradigms.end();
       ++iter)
    {
      // Register the PICML paradigm
      std::string svParadigmName = *iter;
      SendMsgToProgressBar(hInstall, svParadigmName.c_str());
      std::string targetParadigm ("XML=");
      targetParadigm += std::string(value) + "\\paradigms\\" + svParadigmName;
      if (!RegisterParadigm (targetParadigm))
      {
        std::string errorMsg ("Unable to register Paradigm " + targetParadigm);
        errorMsg += ". Please check if you have a valid GME installation. \n";
        errorMsg += LastError();
        SendErrorMsg (hInstall, errorMsg.c_str(), 1);
        return 0;
      }
    }
  // Don't change this return value or BAD THINGS[TM] will happen.
  return 1;
}

bool UnRegisterParadigm (const std::string& paradigmName)
{
  //Initialize the OLE libraries
  HRESULT hr = ::CoInitialize(NULL);
  if (FAILED(hr))
    return false;

  CLSID clsid;

  hr = ::CLSIDFromProgID(OLESTR("Mga.MgaRegistrar"), &clsid);
  if (FAILED(hr))
    return false;

  IMgaRegistrar *reg;
  hr = ::CoCreateInstance(clsid, NULL, CLSCTX_ALL, __uuidof(IMgaRegistrar),
                          reinterpret_cast<void**>(&reg));
  if (FAILED(hr))
    return false;

  UINT cp = GetACP ();
  int len = ::MultiByteToWideChar (cp, 0, paradigmName.c_str(), -1, 0, 0);
  wchar_t *wstr = new wchar_t[len];
  ::MultiByteToWideChar (cp, 0, paradigmName.c_str(), -1, wstr, len);
  BSTR name = ::SysAllocString(wstr);

  hr = reg->UnregisterParadigm(name, REGACCESS_BOTH);

  if (FAILED(hr))
    return false;

  ::SysFreeString(name);

  reg->Release();

  ::CoUninitialize();

  return true;
}

UINT __stdcall UnRegisterParadigms (MSIHANDLE hInstall)
{
  // installable paradigm number
  int nParadigmNum = 1;

  // Initialize progress bar
  InitProgressBar(hInstall, nParadigmNum, PARADIGMCOST,
                  "Paradigm Uninstall",
                  "UnRegistering Paradigms from GME.", "UnRegistering [1]");

  std::vector<std::string> paradigms;
  paradigms.push_back ("PICML");

  for (std::vector<std::string>::const_iterator iter = paradigms.begin();
       iter != paradigms.end();
       ++iter)
    {
      // UnRegister the PICML paradigm
      std::string svParadigmName = *iter;
      SendMsgToProgressBar(hInstall, svParadigmName.c_str());
      if (!UnRegisterParadigm (svParadigmName))
        {
          std::string errorMsg ("Unable to unregister Paradigm " +
                                svParadigmName);
          errorMsg += LastError();
          SendErrorMsg (hInstall, errorMsg.c_str(), 1);
          return 0;
        }
    }
  // Don't change this return value or BAD THINGS[TM] will happen.
  return 1;
}
