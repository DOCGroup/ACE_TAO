#ifndef SVC_CONF_EXPORTER_EXTERN_DECLARATIONS
#define SVC_CONF_EXPORTER_EXTERN_DECLARATIONS

#include "windows.h"

#ifdef SVC_CONF_EXPORTER_DLL_ENTRY_EXPORTS
#define Svc_Conf_Exporter_API __declspec(dllexport)
#else
#define Svc_Conf_Exporter_API __declspec(dllimport)
#endif

#endif // SVC_CONF_EXPORTER_EXTERN_DECLARATIONS
