// -*- C++ -*-
// File: Library.cpp

// Author: Phil Mesnier

// This file contains the implementation of the classes responsible for
// generating specialized mpc files for individual libraries, as well as
// outputting usage metrics for the various object modules contained in the
// library.

#include "ace/OS_NS_dirent.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_ctype.h"
#include "ace/Log_Msg.h"
#include <cstring>

#include "Library.h"

MPC_Generator::MPC_Generator (ACE_CString const &libname)
  : libname_ (libname)
{
  mpcfilename_ = libname_ + "_subset.mpc";
}

void
MPC_Generator::write_file (ACE_CString const &file)
{
  mpcfile_ << "    " << file << ".cpp" << endl;
}

void
MPC_Generator::write_prolog (ACE_CString const &path)
{
  ACE_CString fname (path + "/" + mpcfilename_);
  ACE_DEBUG ((LM_DEBUG, "writing file %s\n",fname.c_str()));
  mpcfile_.open(fname.c_str());
  if (!mpcfile_)
    ACE_DEBUG ((LM_DEBUG,"mpc file open failed\n"));

  mpcfile_
    << "// Generated mpc file for producing a subset of the "
    << libname_ << " library " << endl << endl
    << "project(" << libname_ << "_subset)";

  this->write_baseprojects ();

  mpcfile_
    << " {" << endl
    << "  sharedname   = " << libname_ << "_subset" << endl
    << "  pch_header   = " << endl
    << "  pch_source   = " << endl;

  this->write_projectinfo ();

  mpcfile_ << endl
           << "  Source_Files {" << endl;
}

void
MPC_Generator::write_epilog ()
{
  mpcfile_ << "  }" << endl
           << "}" << endl;
  mpcfile_.close();
}

void
MPC_Generator::write_baseprojects()
{
  mpcfile_ << ": acedefaults, core";
}

void
MPC_Generator::write_projectinfo()
{
  mpcfile_ << "  libout       = $(ACE_ROOT)/lib" << endl
           << "  dynamicflags = ACE_BUILD_DLL ACE_OS_BUILD_DLL" << endl;
}

//-----------------------------------------------------------------------------
MPC_ACE_Dep_Lib::MPC_ACE_Dep_Lib (ACE_CString const &libname)
  : MPC_Generator(libname)
{}

void
MPC_ACE_Dep_Lib::write_baseprojects()
{
  mpcfile_ << ": acedefaults, aceversion";
}

void
MPC_ACE_Dep_Lib::write_projectinfo()
{
  mpcfile_ << "  libout       = $(ACE_ROOT)/lib" << endl
           << "  libs        += ACE_subset" << endl
           << "  after       += ACE_subset" << endl;
}

//-----------------------------------------------------------------------------
MPC_TAO_Lib::MPC_TAO_Lib (ACE_CString const &libname)
  : MPC_ACE_Dep_Lib(libname)
{}

void
MPC_TAO_Lib::write_baseprojects()
{
  MPC_ACE_Dep_Lib::write_baseprojects ();
  mpcfile_ << ", core, tao_output, taodefaults";
}

void
MPC_TAO_Lib::write_projectinfo()
{
  MPC_ACE_Dep_Lib::write_projectinfo();
  mpcfile_ << "  dynamicflags = TAO_BUILD_DLL" << endl;
}

//-----------------------------------------------------------------------------
MPC_TAO_Dep_Lib::MPC_TAO_Dep_Lib (ACE_CString const &libname)
  : MPC_TAO_Lib(libname)
{}

void
MPC_TAO_Dep_Lib::write_baseprojects()
{
  MPC_TAO_Lib::write_baseprojects ();
  mpcfile_ << ", taoidldefaults";
}

void
MPC_TAO_Dep_Lib::write_projectinfo()
{
  // Try our best to generate the dynamicflags
  ACE_CString dflags;
  for(size_t i = 0; i < this->libname_.length (); ++i) {
    dflags += static_cast<char>(ACE_OS::ace_toupper (this->libname_[i]));
  }
  dflags += "_BUILD_DLL";

  MPC_ACE_Dep_Lib::write_projectinfo();
  mpcfile_ << "  dynamicflags = " << dflags.c_str () << endl
           << "  libs        += TAO_subset" << endl
           << "  after       += TAO_subset" << endl
           << "  includes    += $(TAO_ROOT)/orbsvcs" << endl
           << "  idlflags    += -I$(TAO_ROOT)/orbsvcs" << endl;
}

//-----------------------------------------------------------------------------

Library::Library (ACE_TCHAR const *name)
  : name_ (name),
    exported_ (0)
{
  if (name_ == "ACE")
    mpcfile_ = std::make_unique<MPC_Generator> (name_);
  else if (name_.find ("ACE_") == 0)
    mpcfile_ = std::make_unique<MPC_ACE_Dep_Lib> (name_);
  else if (name_ == "TAO")
    mpcfile_ = std::make_unique<MPC_TAO_Lib> (name_);
  else
    mpcfile_ = std::make_unique<MPC_TAO_Dep_Lib> (name_);
}

void
Library::set_path (ACE_TCHAR const *p)
{
  char abspath[1000];
  std::memset (abspath, 0, 1000);
  ssize_t const abspathlen = ACE_OS::readlink(p,abspath,999);
  ACE_CString path (p);
  if (abspathlen > 0) {
    abspath[abspathlen] = '\0';
    path = abspath;
  }

  ACE_CString::size_type const pathsep = path.rfind('/');

  if (pathsep == ACE_CString::npos) {
    path_ = ".";
  } else {
    path_ = path.substr(0,pathsep);
  }
}

ACE_CString const &
Library::name () const
{
  return name_;
}

bool
Library::has_modules () const
{
  return num_modules_ > 0;
}

extern "C" {
static int
selector (dirent const *d)
{
  return std::strstr (d->d_name, ".o") != nullptr;
}

static int
comparator (dirent const **d1, dirent const **d2)
{
  return std::strcmp ((*d1)->d_name, (*d2)->d_name);
}

} /* extern "C" */

void
Library::load_modules ()
{
  ACE_CString const subdir = path_ + "/.shobj";

  struct dirent **dent {};
  num_modules_ = ACE_OS::scandir(ACE_TEXT_CHAR_TO_TCHAR (subdir.c_str()),
                                 &dent,selector,comparator);

  if (num_modules_ > 0) {
    modules_.resize (num_modules_);
    for (int i = 0; i < num_modules_; i++) {
      ACE_CString const ent_name (ACE_TEXT_ALWAYS_CHAR (dent[i]->d_name));
      modules_[i] = std::make_unique<Obj_Module> (ent_name);
      modules_[i]->add_source (ACE_CString(subdir + "/" + ent_name).c_str());
      ACE_OS::free(dent[i]);
    };
  }

  if (num_modules_ >= 0)
    ACE_OS::free(dent);
}

void
Library::resolve (Sig_List &undefs)
{
  if (num_modules_ < 1)
    return;

  for (Signature const *uname = undefs.first();
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
            for (Signature const *needed = modules_[i]->imports().first();
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
Library::write_export_list (bool show_ref_counts)
{
  if (num_modules_ < 1)
    return;

  ACE_CString const excludedfilename = path_ + "/excluded_modules";
  ACE_CString const rcpath = path_ + "/usage_metrics";

  ofstream exclusions (excludedfilename.c_str());
  if (!exclusions) {
    ACE_ERROR ((LM_ERROR, "%p\n", "open exclusions list"));
  }

  if (show_ref_counts) {
    ACE_DEBUG ((LM_DEBUG, "Making directory %s\n",rcpath.c_str()));
    if (ACE_OS::mkdir(ACE_TEXT_CHAR_TO_TCHAR (rcpath.c_str())) == -1 &&
        errno != EEXIST)
      ACE_ERROR ((LM_ERROR, "%p\n", "mkdir"));
  }

  ACE_DEBUG ((LM_DEBUG,"%s: %d out of %d modules required\n",
              name_.c_str(), num_extrefs_, num_modules_));

  mpcfile_->write_prolog(path_);

  for (int i = 0; i < num_modules_; i++)
    if (modules_[i]->extref()) {
      if (show_ref_counts) {
        ACE_CString const fname = rcpath + "/" +  modules_[i]->name();
        ofstream countfile (fname.c_str());
        countfile << "Exported symbols:" << endl;
        for (Signature const *sig = modules_[i]->exports().first();
             modules_[i]->exports().hasmore();
             sig = modules_[i]->exports().next())
          {
            countfile.width(5);
            countfile << sig->used_count() << " " << sig->name() << endl;
          }
        countfile << "\nImported symbols:" << endl;
        for (Signature const *n_sig = modules_[i]->imports().first();
             modules_[i]->imports().hasmore();
             n_sig = modules_[i]->imports().next())
          countfile << n_sig->name() << endl;
      }
      mpcfile_->write_file(modules_[i]->name().substring(0,modules_[i]->name().length()-2));
    } else {
      exclusions
        << modules_[i]->name().substring(0,modules_[i]->name().length()-2)
        << endl;
    }

  mpcfile_->write_epilog();
}
