// $Id$

#include "Video_Repository.h"
#include "ace/Read_Buffer.h"

const char* TAO_Video_Repository::MOVIE_INFO = "Movie_Info";
const char* TAO_Video_Repository::MOVIE_NAMES = "Movie_Names";

TAO_Video_Repository::TAO_Video_Repository (const char* file_name)
  : filename_ (file_name),
    movie_info_ (0),
    movie_names_ (0),
    num_movies_ (0),
    lastchanged_ (0)
{
}

TAO_Video_Repository::~TAO_Video_Repository (void)
{
  TAO_VR::Movie_Names::freebuf (this->movie_names_);
  TAO_VR::Movie_Info::freebuf (this->movie_info_);
}

CORBA::Any*
TAO_Video_Repository::evalDP (const CORBA::Any& extra_info,
			      CORBA::TypeCode_ptr returned_type,
			      CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTradingDynamic::DPEvalFailure))
{
  CORBA::Any* return_value = 0;
  ACE_NEW_RETURN (return_value, CORBA::Any, 0);
  
  struct stat file_stat;
  if (ACE_OS::stat (this->filename_, &file_stat) == 0)
    {      
      if (this->lastchanged_ < file_stat.st_mtime)
	{
	  FILE* file = ACE_OS::fopen (this->filename_, "r");
	  
	  if (file != 0)
	    {
	      // Read the file into a buffer
	      ACE_Read_Buffer read_file (file, 1);
	      char* database = read_file.read (EOF, '\n', '%');	      

	      // Parse the file into a sequence and insert it into an
	      // Any (i.e., this->return_)
	      TAO_Video_Repository::parse_file (database,
						read_file.replaced ());
	      
	      ACE_Allocator* alloc = ACE_Allocator::instance ();
	      alloc->free (database);

	      this->lastchanged_ = file_stat.st_mtime;
	    }
	}

      char* prop_name = 0;
      extra_info >>= prop_name;

      String_var prop_name_var (prop_name);
      if (ACE_OS::strcmp (prop_name, MOVIE_NAMES) == 0)
	{
	  TAO_VR::Movie_Names* movie_names = 0;
	  ACE_NEW_RETURN (movie_names,
			  TAO_VR::Movie_Names (this->num_movies_,
					       this->num_movies_,
					       this->movie_names_,
					       CORBA::B_FALSE),
			  0);
	  (*return_value) <<= movie_names;
	}
      else if (ACE_OS::strcmp (prop_name, MOVIE_INFO) == 0)
	{
	  TAO_VR::Movie_Info* movie_info = 0;
	  ACE_NEW_RETURN (movie_info,
			  TAO_VR::Movie_Info (this->num_movies_,
					       this->num_movies_,
					       this->movie_info_,
					       CORBA::B_FALSE),
			  0);
	  
	  (*return_value) <<= movie_info;
	}
    }

  return return_value;
}

void
TAO_Video_Repository::parse_file (const char* database, int num_lines)
{
  const char* delim = "%";
  CORBA::ULong i = 0;
  CORBA::Any* return_value = 0;
  char* current = (char *) database;

  ACE_DEBUG ((LM_DEBUG, "Recomputing the movie stats.\n"));
  
  TAO_VR::Movie_Names::freebuf (this->movie_names_);
  TAO_VR::Movie_Info::freebuf (this->movie_info_);

  this->movie_names_ = TAO_VR::Movie_Names::allocbuf (num_lines);
  this->movie_info_ = TAO_VR::Movie_Info::allocbuf (num_lines);

  if (this->movie_names_ != 0 && this->movie_info_ != 0)
    {
      current = ACE_OS::strtok (current, "%");
      while (current != 0)
	{
	  TAO_VR::Movie& movie = this->movie_info_[i];
	  
	  this->movie_names_[i] = CORBA::string_dup (current);
	  movie.name_ = (const char*) current;
	  movie.filename_ = (const char*) ACE_OS::strtok (0, delim);
	  movie.description_ = (const char*) ACE_OS::strtok (0, delim);
	  
	  ACE_DEBUG ((LM_DEBUG,
		      "Movie Name: %s\nFile Name: %s\nDescription: %s\n",
		      (const char *) movie.name_,
		      (const char *) movie.filename_,
		      (const char *) movie.description_));
	  
	  // From the actual movie file, extract the techincal information.
	  TAO_Video_Repository::
	    obtain_movie_info (movie.filename_, movie);
           
	  current = ACE_OS::strtok (0, delim);
	  i++;
	}
    }

  this->num_movies_ = num_lines;
}

void
TAO_Video_Repository::
export_dynamic_properties (TAO_Property_Exporter& prop_exporter,
			   TAO_DP_Dispatcher& dp_dispatcher) const
{
  CORBA::Any extra_info;

  extra_info <<= MOVIE_INFO;
  CosTradingDynamic::DynamicProp* dp_struct = 
    dp_dispatcher.construct_dynamic_prop (MOVIE_INFO,
					  TAO_VR::_tc_Movie_Info,
					  extra_info);    

  dp_dispatcher.register_handler (MOVIE_INFO, (TAO_DP_Evaluation_Handler*) this);
  prop_exporter.add_dynamic_property (MOVIE_INFO, dp_struct);

  extra_info <<= MOVIE_NAMES;
  dp_struct = dp_dispatcher.construct_dynamic_prop (MOVIE_INFO,
						    TAO_VR::_tc_Movie_Names,
						    extra_info);    

  dp_dispatcher.register_handler (MOVIE_NAMES, (TAO_DP_Evaluation_Handler*) this);
  prop_exporter.add_dynamic_property (MOVIE_NAMES, dp_struct);
}

int
TAO_Video_Repository::
define_properties (CosTradingRepos::ServiceTypeRepository::PropStructSeq& prop_seq,
		   CORBA::ULong offset) const
{
  CORBA::ULong num_props = prop_seq.length ();

  if (num_props <= offset + 1)
    prop_seq.length (offset + 2);

  prop_seq[offset].name = MOVIE_INFO;
  prop_seq[offset].value_type = CORBA::TypeCode::_duplicate (TAO_VR::_tc_Movie_Info);
  prop_seq[offset].mode = CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;
  prop_seq[offset + 1].name = MOVIE_NAMES;
  prop_seq[offset + 1].value_type = CORBA::TypeCode::_duplicate (TAO_VR::_tc_Movie_Names);
  prop_seq[offset + 1].mode = CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;

  return 2;
}

