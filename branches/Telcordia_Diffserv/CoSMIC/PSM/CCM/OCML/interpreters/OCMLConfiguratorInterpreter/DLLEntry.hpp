#ifndef DLL_ENTRY_HPP
#define DLL_ENTRY_HPP

#include "extern_declarations.hpp"

// The caller should free the result with free
typedef char* (*DLLFunctionPtr) (const char*, size_t);

extern "C" OCMLConfigurator_API char*
DLLFunction(const char* values, size_t values_size, const char* tree_file);

#endif // DLL_ENTRY_HPP
