#ifndef OCML_EXTERN_DECLARATIONS
#define OCML_EXTERN_DECLARATIONS

#include "windows.h"

#ifdef OCML_CONFIGURATOR_DLL_ENTRY_EXPORTS
#define OCMLConfigurator_API __declspec(dllexport)
#else
#define OCMLConfigurator_API __declspec(dllimport)
#endif

#endif // OCML_EXTERN_DECLARATIONS
