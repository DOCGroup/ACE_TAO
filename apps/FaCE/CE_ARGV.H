// -*- C++ -*-
//=============================================================================
/**
 *  @file    CE_ARGV.h
 *
 *  $Id$
 *
 *  @author Si Mong Park <spark@ociweb.com>
 */
//=============================================================================

#ifndef CE_ARGV_H
#define CE_ARGV_H

#include <windows.h>


/**
 * @class CE_ARGV
 *
 * @brief This class is to hash input parameters, argc and argv, for WinCE platform.
 *
 * Since WinCE only supports wchar_t as an input from OS, some implementation detail,
 * especially for CORBA spec, will not support wchar_t (wchar_t) type parameter.
 * Moreover, WinCE's input parameter type is totally different than any other OS;
 * all command line parameters will be stored in a single wide-character string with
 * each unit parameter divided by blank space, and it does not provide the name of
 * executable (generally known as argv[0]).
 * This class is to convert CE's command line parameters and simulate as in the same
 * manner as other general platforms, adding 'root' as a first argc, which is for the
 * name of executable in other OS.
 */
class CE_ARGV
{
public:
    /**
     * Ctor accepts CE command line as a paramter.
     */
    CE_ARGV(wchar_t* cmdLine);

    /**
     * Default Dtor that deletes any memory allocated for the converted string.
     */
    ~CE_ARGV(void);

    /**
     * Returns the number of command line paramters, same as argc on Unix.
     */
    int argc(void);

    /**
     * Returns the 'char**' that contains the converted command line parameters.
     */
    wchar_t** const argv(void);

private:
    /**
     * Copy Ctor is not allowed.
     */
    CE_ARGV(void);

    /**
     * Copy Ctor is not allowed.
     */
    CE_ARGV(CE_ARGV&);

    /**
     * Pointer of converted command line paramters.
     */
    wchar_t** ce_argv_;

    /**
     * Integer that is same as argc on other OS's.
     */
    int ce_argc_;
};


inline int CE_ARGV::argc()
{
    return ce_argc_;
}


inline wchar_t** const CE_ARGV::argv()
{
    return ce_argv_;
}

#endif  // CE_ARGV_H
