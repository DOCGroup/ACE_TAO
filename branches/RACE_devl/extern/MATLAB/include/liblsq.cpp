//
// MATLAB Compiler: 4.7 (R2007b)
// Date: Wed Dec 05 14:13:02 2007
// Arguments: "-B" "macro_default" "-W" "cpplib:liblsq" "-T" "link:lib"
// "lsqlin_simple2.m" 
//

#include <stdio.h>
#define EXPORTING_liblsq 1
#include "liblsq.h"
#ifdef __cplusplus
extern "C" {
#endif

extern mclComponentData __MCC_liblsq_component_data;

#ifdef __cplusplus
}
#endif


static HMCRINSTANCE _mcr_inst = NULL;


#if defined( _MSC_VER) || defined(__BORLANDC__) || defined(__WATCOMC__) || defined(__LCC__)
#ifdef __LCC__
#undef EXTERN_C
#endif
#include <windows.h>

static char path_to_dll[_MAX_PATH];

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, void *pv)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        char szDllPath[_MAX_PATH];
        char szDir[_MAX_DIR];
        if (GetModuleFileName(hInstance, szDllPath, _MAX_PATH) > 0)
        {
             _splitpath(szDllPath, path_to_dll, szDir, NULL, NULL);
            strcat(path_to_dll, szDir);
        }
	else return FALSE;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
    }
    return TRUE;
}
#endif
#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultPrintHandler(const char *s)
{
  return mclWrite(1 /* stdout */, s, sizeof(char)*strlen(s));
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultErrorHandler(const char *s)
{
  int written = 0;
  size_t len = 0;
  len = strlen(s);
  written = mclWrite(2 /* stderr */, s, sizeof(char)*len);
  if (len > 0 && s[ len-1 ] != '\n')
    written += mclWrite(2 /* stderr */, "\n", sizeof(char));
  return written;
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_liblsq_C_API 
#define LIB_liblsq_C_API /* No special import/export declaration */
#endif

LIB_liblsq_C_API 
bool MW_CALL_CONV liblsqInitializeWithHandlers(
    mclOutputHandlerFcn error_handler,
    mclOutputHandlerFcn print_handler
)
{
  if (_mcr_inst != NULL)
    return true;
  if (!mclmcrInitialize())
    return false;
  if (!mclInitializeComponentInstance(&_mcr_inst, &__MCC_liblsq_component_data,
                                      true, NoObjectType, LibTarget,
                                      error_handler, print_handler))
    return false;
  return true;
}

LIB_liblsq_C_API 
bool MW_CALL_CONV liblsqInitialize(void)
{
  return liblsqInitializeWithHandlers(mclDefaultErrorHandler,
                                      mclDefaultPrintHandler);
}

LIB_liblsq_C_API 
void MW_CALL_CONV liblsqTerminate(void)
{
  if (_mcr_inst != NULL)
    mclTerminateInstance(&_mcr_inst);
}

LIB_liblsq_C_API 
void MW_CALL_CONV liblsqPrintStackTrace(void) 
{
  char** stackTrace;
  int stackDepth = mclGetStackTrace(_mcr_inst, &stackTrace);
  int i;
  for(i=0; i<stackDepth; i++)
  {
    mclWrite(2 /* stderr */, stackTrace[i], sizeof(char)*strlen(stackTrace[i]));
    mclWrite(2 /* stderr */, "\n", sizeof(char)*strlen("\n"));
  }
  mclFreeStackTrace(&stackTrace, stackDepth);
}


LIB_liblsq_C_API 
bool MW_CALL_CONV mlxLsqlin_simple2(int nlhs, mxArray *plhs[],
                                    int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "lsqlin_simple2", nlhs, plhs, nrhs, prhs);
}

LIB_liblsq_CPP_API 
void MW_CALL_CONV lsqlin_simple2(int nargout, mwArray& X
                                 , const mwArray& C, const mwArray& d
                                 , const mwArray& A, const mwArray& b)
{
  mclcppMlfFeval(_mcr_inst, "lsqlin_simple2",
                 nargout, 1, 4, &X, &C, &d, &A, &b);
}
