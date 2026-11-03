/*
 *
 * To populate the registry_db, construct a codeset registry text file based
 * on the OSF's Character and Code Set Registry. See DCE RFC 40.1 for details
 * on obtaining the full text for the current registry. Once you have composed
 * a text file containing all the desired codeset information, build and run
 * mkcsregdb. The source is in $ACE_ROOT/apps/mkcsregdb. It will generate a new
 * copy of this file, with the registry_db_ array properly initialized.
 */

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/Codeset_Registry.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

class csdb_generator : public ACE_Codeset_Registry
{
public:
  csdb_generator ();
  csdb_generator (csdb_generator const &) = delete;
  csdb_generator (csdb_generator &&) = delete;
  csdb_generator &operator= (csdb_generator const &) = delete;
  csdb_generator &operator= (csdb_generator &&) = delete;
  int read_from (const char *source);
  int init_output (const char *srcfile);
  int write_entry ();
  int fini_output (const char *target);
  int read_line ();
  void write_dummy_entry();
  void fail(const char *msg);
private:
  struct File_Closer
  {
    void operator() (FILE *file) const
    {
      if (file != nullptr)
        std::fclose (file);
    }
  };

  static const char * tags_[];
  static constexpr int max_line_ = 1024;
  int count_;
  bool in_entry_;
  bool have_description_;
  bool have_loc_name_;
  registry_entry entry_;
  int linecounter_;
  std::array<char, max_line_> source_line_;
  char *line_data_; // Non-owning pointer into source_line_ for the current parsed line.
  std::ifstream inf_;
  std::unique_ptr<FILE, File_Closer> outf_;
  std::string tempfilename_;
  std::string ace_src_;
  std::string description_;
  std::string loc_name_;
};

const char * csdb_generator::tags_[] = {
  "start",
  "end",
  "description ",
  "loc_name ",
  "rgy_value ",
  "char_values ",
  "max_bytes ",
  nullptr
};

csdb_generator::csdb_generator ()
  : count_ (0),
    in_entry_ (false),
    have_description_ (false),
    have_loc_name_ (false),
    linecounter_ (0),
    line_data_ (nullptr)
{
  const char *ace_root = ACE_OS::getenv ("ACE_ROOT");
  if (ace_root != nullptr)
    this->ace_src_ = ace_root;
}

void
csdb_generator::fail (const char *msg)
{
  ACE_ERROR ((LM_ERROR,"Error, line %d: %s\n",linecounter_,msg));
  this->outf_.reset ();
  ACE_OS::unlink (this->tempfilename_.c_str ());
  ACE_OS::exit (-1);
}

int
csdb_generator::read_line()
{
  this->inf_.getline (this->source_line_.data (), max_line_);
  this->line_data_ = nullptr;
  char *cpos = std::strchr (this->source_line_.data (), '#');
  if (cpos == nullptr)
    cpos = this->source_line_.data () + std::strlen (this->source_line_.data ());
  while (cpos > this->source_line_.data () && std::strchr (" \t", *(cpos - 1))) cpos--;
  *cpos = 0;
  if (cpos == this->source_line_.data ())
    return 0;
  for (int i = 0; tags_[i]; i++)
    {
      cpos = std::strstr (this->source_line_.data (), tags_[i]);
      if (cpos == nullptr) // not found
        continue;
      if (cpos > this->source_line_.data ()) // make sure it's first token
        {
          char *tpos = cpos-1;
          while (tpos > this->source_line_.data () && std::strchr (" \t", *tpos)) tpos--;
          if (tpos > this->source_line_.data ())
            continue;
        }
      if (i == 0 && in_entry_)
        fail ("\"start\" encountered twice before \"end\"");
      if (i > 0 && !this->in_entry_)
        {
          char emsg[100];
          std::snprintf (emsg, sizeof emsg, "\"%s\" encountered before \"start\"", tags_[i]);
          fail (emsg);
        }
      if (i > 1)
        {
          this->line_data_ = cpos + std::strlen (tags_[i]);
          while (*this->line_data_ && std::strchr (" \t", *this->line_data_))
            this->line_data_++;
        }
      return i+1;
    }
  return -1;
}

int
csdb_generator::read_from (const char *srcfile)
{
  this->inf_.open (srcfile);
  if (!this->inf_)
    {
      this->outf_.reset ();
      ACE_OS::unlink (this->tempfilename_.c_str ());
      return -1;
    }
  char *ptr = nullptr;
  while (this->inf_.good () && !this->inf_.eof ()) {
    linecounter_++;
    switch (read_line ()) {
    case -1: // bogus line
      fail ("unknown field tag");
      break;
    case 0: // comment or blank line
      break;
    case 1: // start
      this->description_.clear ();
      this->loc_name_.clear ();
      this->have_description_ = false;
      this->have_loc_name_ = false;
      entry_.desc_ = nullptr;
      entry_.loc_name_ = nullptr;
      entry_.codeset_id_ = 0;
      entry_.num_sets_ = 0;
      entry_.max_bytes_ = 0;
      this->in_entry_ = true;
      break;
    case 2: // end
      if (entry_.codeset_id_ == 0)
        fail ("entry missing rgy_value");
      if (entry_.num_sets_ == 0)
        fail ("entry does not include at least one char_value");
      if (entry_.max_bytes_ == 0)
        fail ("entry does not define max_bytes");
      // These registry fields borrow the strings only while write_entry() serializes them.
      entry_.desc_ = this->description_.c_str ();
      entry_.loc_name_ = this->loc_name_.c_str ();
      write_entry ();
      this->count_++;
      this->in_entry_ = false;
      break;
    case 3: // description
      if (this->have_description_)
        fail ("duplicate description");
      this->description_ = this->line_data_;
      this->have_description_ = true;
      break;
    case 4: // loc_name
      if (this->have_loc_name_)
        fail ("duplicate loc_name");
       this->loc_name_ = this->line_data_;
       this->have_loc_name_ = true;
       break;
    case 5: // rgy_value
      if (entry_.codeset_id_ != 0)
        fail ("duplicate rgy_value");
      entry_.codeset_id_ = std::strtoul (this->line_data_, &ptr, 16);
      if (*ptr != 0 || entry_.codeset_id_ == 0)
        {
          char emsg [100];
          std::snprintf (emsg, sizeof emsg, "invalid rgy_value, '%s'", this->line_data_);
          fail (emsg);
        }
      break;
    case 6: // char_values
      if (entry_.num_sets_ != 0)
        fail ("duplicate char_values");
      ptr = this->line_data_;
      do {
        if (*ptr == ':')
          ptr++;
        ACE_CDR::UShort tmp =
          static_cast<ACE_CDR::UShort> (std::strtoul (ptr, &ptr, 16));
        if (*ptr != 0 && *ptr != ':')
          {
            char emsg[100];
            std::snprintf (emsg, sizeof emsg, "invalid symbol \'%c\' in char_values", *ptr);
            fail (emsg);
          }
        if (entry_.num_sets_ < max_charsets_)
          entry_.char_sets_[entry_.num_sets_++] = tmp;
        else entry_.num_sets_++;
      } while (*ptr == ':');
      if (entry_.num_sets_ > max_charsets_)
        {
          char emsg[200];
          std::snprintf (emsg, sizeof emsg,
                         "max of %d char_values exceeded.\nIncrease ACE_Codeset_Registry::max_charsets_ "
                         "to at least %d and rebuild mkcsregdb",
                         max_charsets_, entry_.num_sets_);
          fail (emsg);
        }
      break;
    case 7: // max_bytes
      if (entry_.max_bytes_ != 0)
        fail ("duplicate max_bytes");
      entry_.max_bytes_ =
        static_cast<ACE_CDR::UShort> (std::strtol (this->line_data_, &ptr, 10));
      if (*ptr != 0)
        fail ("invalid max_bytes");
      break;
    }
  }
  return 0;
}

int
csdb_generator::init_output (const char *srcfile)
{
  ACE_stat buf;
  if (ACE_OS::stat (srcfile,&buf) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,"Unable to open %s\n",srcfile),-1);

  if (this->ace_src_.empty ())
    ACE_ERROR_RETURN ((LM_ERROR,"You must first set $ACE_ROOT\n"),-1);

  time_t now = ACE_OS::time();
  std::string temp_template = this->ace_src_ + ACE_DIRECTORY_SEPARATOR_STR + "csdbXXXXXX";
  std::vector<char> temp_template_buffer (temp_template.begin (), temp_template.end ());
  temp_template_buffer.push_back ('\0');
  ACE_HANDLE fd = ACE_OS::mkstemp (temp_template_buffer.data ());
  if (fd == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open output file, %p\n",
                       temp_template_buffer.data ()),
                      -1);
  this->tempfilename_ = temp_template_buffer.data ();
  this->outf_.reset (ACE_OS::fdopen (fd, "w"));
  if (!this->outf_)
    {
      ACE_OS::close (fd);
      ACE_OS::unlink (this->tempfilename_.c_str ());
      return -1;
    }

  std::fprintf (this->outf_.get (), "/*\n * Codeset registry DB, generated %s * source: %s\n",
                   ACE_OS::asctime (ACE_OS::localtime(&now)),
                   srcfile);
  std::fprintf (this->outf_.get (),
                " *\n * To populate the registry_db, construct a codeset registry text file "
                "based\n");
  std::fprintf (this->outf_.get (), " * on the OSF's Character and Code Set Registry. See DCE RFC 40.1 for details\n");
  std::fprintf (this->outf_.get (), " * on obtaining the full text for the current registry. Once you have composed\n");
  std::fprintf (this->outf_.get (), " * a text file containing all the desired codeset information, build and run\n");
  std::fprintf (this->outf_.get (),
                " * mkcsregdb. The source is in $ACE_ROOT/apps/mkcsregdb. It will generate a "
                "new\n");
  std::fprintf (this->outf_.get (), " * copy of this file, with the registry_db_ array properly initialized.\n */\n");
  std::fprintf (this->outf_.get (),
                "\n#include \"ace/Codeset_Registry.h\"\n\nACE_BEGIN_VERSIONED_NAMESPACE_DECL\n\n%s\n%s\n{\n",
                   "ACE_Codeset_Registry::registry_entry const",
                   "ACE_Codeset_Registry::registry_db_[] =");
  return 0;
}

int
csdb_generator::write_entry ()
{
  if (count_)
    std::fprintf (this->outf_.get (), ",\n");
  std::fprintf (this->outf_.get (), "  {\"%s\",\"%s\",0x%08x,%d,{",
                   entry_.desc_,
                   entry_.loc_name_,
                   entry_.codeset_id_,
                   entry_.num_sets_);
  for (ACE_CDR::UShort j = 0; j < entry_.num_sets_; j++)
    if (j+1 < entry_.num_sets_)
      std::fprintf (this->outf_.get (), "0x%04x,", entry_.char_sets_[j]);
    else
      std::fprintf (this->outf_.get (), "0x%04x", entry_.char_sets_[j]);
  std::fprintf (this->outf_.get (), "},%d}", entry_.max_bytes_);
  return 0;
}

void
csdb_generator::write_dummy_entry()
{
  entry_.desc_ = "No codesets defined";
  entry_.loc_name_ = "NONE";
  entry_.codeset_id_ = 0;
  entry_.num_sets_ = 1;
  entry_.char_sets_[0] = 0;
  entry_.max_bytes_ = 0;
  write_entry();
}

int
csdb_generator::fini_output (const char *tgt)
{
  std::string target = this->ace_src_ + ACE_DIRECTORY_SEPARATOR_STR + "ace"
    + ACE_DIRECTORY_SEPARATOR_STR + tgt;
  if (count_ == 0)
    write_dummy_entry();
  std::fprintf (this->outf_.get (),
                "\n};\n\nsize_t const ACE_Codeset_Registry::num_registry_entries_ = %d;\n\n"
                "ACE_END_VERSIONED_NAMESPACE_DECL\n",
                count_);
  this->outf_.reset ();
  ACE_stat buf;
  if (ACE_OS::stat (target.c_str (), &buf) == 0)
    {
      int index = 0;
      std::string fname;
      do
        {
          const std::string suffix = std::to_string (index++);
          fname = target + "." + std::string (suffix.size () < 3 ? 3 - suffix.size () : 0, '0') + suffix;
        }
      while (ACE_OS::stat (fname.c_str (), &buf) == 0);
      const std::string backup_suffix = fname.substr (target.size ());
      ACE_DEBUG ((LM_DEBUG, "Moving $ACE_ROOT/ace/%s to $ACE_ROOT/ace%s\n",
                  tgt, backup_suffix.c_str ()));
      if (ACE_OS::rename (target.c_str (), fname.c_str ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,"Could not create %s\n, output stored in %s",
                           fname.c_str (), this->tempfilename_.c_str ()), -1);
    }
  ACE_DEBUG ((LM_DEBUG,"writing $ACE_ROOT/ace/%s\n",tgt));
  if (ACE_OS::rename (this->tempfilename_.c_str (), target.c_str ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"Could not create %s\n, output stored in %s",
                       target.c_str (), this->tempfilename_.c_str ()), -1);
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  std::string srcname;
  if (argc > 1)
    srcname = argv[1];
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Usage: %s <source_file>\n"
                       "where source file is the full path to a code set registry text file.\n",
                       argv[0]),
                      -1);
  csdb_generator csdb;
  if (csdb.init_output (srcname.c_str ()) == -1)
    return 0;
  if (csdb.read_from (srcname.c_str ()) == 0)
    csdb.fini_output ("Codeset_Registry_db.cpp");
  return 0;
}
