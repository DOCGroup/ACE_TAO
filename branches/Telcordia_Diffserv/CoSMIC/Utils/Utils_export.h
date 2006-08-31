#ifndef UTILS_EXPORT_H
#	define UTILS_EXPORT_H
#	ifndef Utils_Export
#		if defined (UTILS_HAS_DLL) && (UTILS_HAS_DLL == 1)
#			if defined (UTILS_BUILD_DLL)
#				define Utils_Export __declspec(dllexport)
#			else
#				define Utils_Export __declspec(dllimport)
#			endif /* UTILS_BUILD_DLL */
#		else
#			define Utils_Export
#		endif /*UTILS_HAS_DLL*/
#	endif /* Utils_Export */ 
#endif /* UTILS_EXPORT_H */ 
