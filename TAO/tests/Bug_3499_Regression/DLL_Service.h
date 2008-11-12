// $Id$

#ifndef _DLL_SERVICE_H_
#define _DLL_SERVICE_H_

#include "DLL_Service_export.h"

class ACE_DLL_SERVICE_Export ACE_DLL_Service
{
public:
  ACE_DLL_Service (void);

  virtual ~ACE_DLL_Service (void);

  virtual int init (int , ACE_TCHAR * []);

  virtual int fini (void);

  /// This is only here for destruction purposes
  virtual void destroy (void);
};

#define ACE_DLL_SERVICE_DECL(export_macro, symbol) \
  extern "C" export_macro ACE_DLL_Service * symbol (void)

#define ACE_DLL_SERVICE_IMPL(classname, symbol) \
  ACE_DLL_Service * symbol (void) \
  { \
    classname * service = 0; \
    ACE_NEW_RETURN (service, classname (), 0); \
    return service; \
  }

#endif
