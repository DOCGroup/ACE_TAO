// -*- C++ -*-
// $Id$

// File: Library.cpp

// Author: Phil Mesnier

// This file contains the implementation of the classes responsible for
// generating specialized makefiles for individual libraries, as well as
// outputting usage metrics for the various object modules contained in the
// library.


#include <ace/OS.h>
#include <ace/OS_Dirent.h>
#include <ace/Log_Msg.h>
#include <ace/streams.h>

#include "Library.h"

ACE_RCSID(src, Library, "$Id$")

Makefile_Generator::Makefile_Generator (const ACE_CString& libname)
  : makefile_(),
    libname_(libname),
    makefilename_()
{
  makefilename_ = "Makefile." + libname_ + "_subset";
}

Makefile_Generator::~Makefile_Generator ()
{
}

void 
Makefile_Generator::write_file (const ACE_CString& file)
{
  makefile_ << " \\\n\t" << file << flush;
}

void 
Makefile_Generator::write_prolog (const ACE_CString& path)
{
  ACE_CString fname (path + "/" + makefilename_);
  ACE_DEBUG ((LM_DEBUG, "writing file %s\n",fname.c_str()));
  makefile_.open(fname.c_str());
  if (!makefile_)
    ACE_DEBUG ((LM_DEBUG,"makefile open failed\n"));

  makefile_ 
    << "#--------------------------------------------------------------------"
    << endl;
  makefile_ 
    << "# Generated makefile for producing a subset of the " 
    << libname_ << " library " << endl;
  makefile_
    << "#--------------------------------------------------------------------"
    << endl;

  makefile_ << "\nMAKEFILE = " << makefilename_ << endl;
  makefile_ << "LIB      = lib" << libname_ << "_subset.a" << endl;
  makefile_ << "SHLIB    = lib" << libname_ << "_subset.$(SOEXT)" << endl;
  makefile_ << "\nFILES  =" << flush;
}

void 
Makefile_Generator::write_epilog ()
{
  makefile_ << "\n" << endl;
  this->write_libdeps();
  makefile_ 
    << "#--------------------------------------------------------" << endl;
  makefile_ 
    << "#             Include macros and targets" << endl;
  makefile_
    << "#--------------------------------------------------------" << endl;
  makefile_
    << "include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU" << endl;

  this->write_initial_rules();
  
  makefile_
    << "\nLSRC  = $(addsuffix .cpp,$(FILES))\n" << endl;

  makefile_ 
    << "include $(ACE_ROOT)/include/makeinclude/macros.GNU" << endl;
  makefile_ 
    << "include $(ACE_ROOT)/include/makeinclude/rules.common.GNU" << endl;
  makefile_ 
    << "include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU" << endl;
  makefile_ 
    << "include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU" << endl;
  makefile_ 
    << "include $(ACE_ROOT)/include/makeinclude/rules.local.GNU" << endl;

  this->write_final_rules();  

  makefile_ << "\n" << endl;

  makefile_ 
    << "#-----------------------------------------------------------" << endl;
  makefile_ 
    << "#       Dependencies" << endl;
  makefile_ 
    << "#-----------------------------------------------------------" << endl;
  makefile_ 
    << "# DO NOT DELETE THIS LINE -- g++dep uses it." << endl;
  makefile_ 
    << "# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY." << endl;
  makefile_ 
    << "# IF YOU PUT ANYTHING HERE IT WILL GO AWAY" << endl;

  makefile_.close();
}

void 
Makefile_Generator::write_libdeps()
{
  // nothing to do
}

void 
Makefile_Generator::write_initial_rules()
{
  // nothing to do
}

void 
Makefile_Generator::write_final_rules()
{
  // nothing to do
}

//-----------------------------------------------------------------------------
Make_ACE_Dep_Lib::Make_ACE_Dep_Lib (const ACE_CString& libname)
  : Makefile_Generator(libname)
{}

void 
Make_ACE_Dep_Lib::write_libdeps()
{
  makefile_ << "ACE_SHLIBS = -lACE_subset" << endl;
}

//-----------------------------------------------------------------------------
Make_TAO_Lib::Make_TAO_Lib (const ACE_CString& libname)
  : Make_ACE_Dep_Lib(libname)
{}

void
Make_TAO_Lib::write_libdeps()
{
  makefile_ << "ifndef TAO_ROOT" << endl;
  makefile_ << "TAO_ROOT = $(ACE_ROOT)/TAO" << endl;
  makefile_ << "endif" << endl;

  makefile_ << "ACE_SHLIBS = -lACE_subset" << endl;
}

void 
Make_TAO_Lib::write_initial_rules()
{
    makefile_ << "include $(TAO_ROOT)/rules.tao.GNU" << endl;
}

void 
Make_TAO_Lib::write_final_rules()
{
  makefile_ << "include $(TAO_ROOT)/taoconfig.mk" << endl;
}

//-----------------------------------------------------------------------------
Make_TAO_Dep_Lib::Make_TAO_Dep_Lib (const ACE_CString& libname)
  : Make_TAO_Lib(libname)
{}

void
Make_TAO_Dep_Lib::write_libdeps()
{
  makefile_ << "ifndef TAO_ROOT" << endl;
  makefile_ << "TAO_ROOT = $(ACE_ROOT)/TAO" << endl;
  makefile_ << "endif" << endl;

  makefile_ << "ACE_SHLIBS = -lTAO_subset -lACE_subset" << endl;
}

//-----------------------------------------------------------------------------

Library::Library (const ACE_TCHAR *name)
  : name_(name),
    path_(),
    num_modules_(0),
    num_exports_(0),
    num_extrefs_(0),
    modules_(0),
    exported_(0),
    makefile_(0)
{
  if (name_ == "ACE")
    makefile_ = new Makefile_Generator(name_);
  else if (name_.find ("ACE_") == 0)
    makefile_ = new Make_ACE_Dep_Lib (name_);
  else if (name_ == "TAO")
    makefile_ = new Make_TAO_Lib (name_);
  else
    makefile_ = new Make_TAO_Dep_Lib (name_);
}

Library::~Library ()
{
  delete makefile_;
  int i;
  for (i = 0; i < num_modules_; delete modules_[i++]);
  delete [] modules_;
}

void
Library::set_path (const ACE_TCHAR *p)
{
  char abspath[1000];
  memset (abspath,0,1000);
  int abspathlen = readlink(p,abspath,999);
  ACE_CString path (p);
  if (abspathlen > 0) {
    abspath[abspathlen] = 0;
    path = abspath;
  } 

  int pathsep = path.rfind('/');

  if (pathsep == ACE_CString::npos) {
    path_ = ".";
  } else {
    path_ = path.substr(0,pathsep);
  }
}

const ACE_CString &
Library::name () const
{
  return name_;
}

int
Library::has_modules () const
{
  return num_modules_ > 0;
}

static int
selector (const dirent *d)
{
  return ACE_OS_String::strstr (d->d_name, ".o") != 0;
}

static int
comparator (const dirent **d1, const dirent **d2)
{
  return ACE_OS_String::strcmp ((*d1)->d_name, (*d2)->d_name);
}

void
Library::load_modules ()
{
  ACE_CString subdir = path_ + "/.shobj";

  struct dirent **dent;
  num_modules_ = ACE_OS_Dirent::scandir(subdir.c_str(),
                                        &dent,selector,comparator);

  if (num_modules_ > 0) {
    modules_ = new Obj_Module * [num_modules_];
    for (int i = 0; i < num_modules_; i++) {
      modules_[i] = new Obj_Module(dent[i]->d_name);
      modules_[i]->add_source (ACE_CString(subdir + "/" + dent[i]->d_name).c_str());
      ACE_OS_Memory::free(dent[i]);
    };
  }

  if (num_modules_ > -1)
    ACE_OS_Memory::free(dent);
}

void
Library::resolve (Sig_List &undefs)
{
  if (num_modules_ < 1)
    return;

  for (const Signature *uname = undefs.first(); 
       undefs.hasmore(); 
       uname = undefs.next()) {
    if (exported_.index_of(uname) != -1) {
      undefs.remove_current();
    } 
    else
      for (int i = 0; i < num_modules_; i++)
        if (modules_[i]->extref() == 0 && 
            modules_[i]->exports().index_of(uname) != -1) 
          {
            undefs.remove_current();
            exported_.add (modules_[i]->exports());
            for (const Signature *needed = modules_[i]->imports().first();
                 modules_[i]->imports().hasmore();
                 needed = modules_[i]->imports().next()) 
              if (exported_.index_of(needed) == -1)
                undefs.add (needed->name());
            modules_[i]->add_extref();
            num_extrefs_++;
            break;
          }
  }
}

void 
Library::write_export_list (int show_ref_counts)
{
  if (num_modules_ < 1) 
    return;

  ACE_CString excludedfilename = path_ + "/excluded_modules";
  ACE_CString rcpath = path_ + "/usage_metrics";

  ofstream exclusions (excludedfilename.c_str());
  if (!exclusions) {
    ACE_ERROR ((LM_ERROR, "%p\n", "open exclusions list"));
  }

  if (show_ref_counts) {
    ACE_DEBUG ((LM_DEBUG, "Making directory %s\n",rcpath.c_str()));
    if (ACE_OS::mkdir(rcpath.c_str()) == -1)
      ACE_ERROR ((LM_ERROR, "%p\n", "mkdir"));
  }

  ACE_DEBUG ((LM_DEBUG,"%s: %d out of %d modules required\n", 
              name_.c_str(), num_extrefs_, num_modules_));

  makefile_->write_prolog(path_);

  for (int i = 0; i < num_modules_ ; i++)
    if (modules_[i]->extref()) {
      if (show_ref_counts) {
        ACE_CString fname = rcpath + "/" +  modules_[i]->name();
        ofstream countfile (fname.c_str());
        countfile << "Exported symbols:" << endl;
        for (const Signature *sig = modules_[i]->exports().first();
             modules_[i]->exports().hasmore();
             sig = modules_[i]->exports().next())
          {
            countfile.width(5);
            countfile << sig->used_count() << " " << sig->name() << endl;
          }
        countfile << "\nImported symbols:" << endl;
        for (const Signature *sig = modules_[i]->imports().first();
             modules_[i]->imports().hasmore();
             sig = modules_[i]->imports().next())
          countfile << sig->name() << endl;
      }
      makefile_->write_file(modules_[i]->name().substring(0,modules_[i]->name().length()-2));
    } else {
      //      const char * modname = modules_[i]->name().c_str();
      exclusions 
        << modules_[i]->name().substring(0,modules_[i]->name().length()-2) 
        << endl;
    }  

  makefile_->write_epilog();
}
