// $Id$

#include "Video_Repository.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(server, Video_Repository, "$Id$")


const char* TAO_Video_Repository::MOVIE_INFO = "Movie_Info";

TAO_Video_Repository::TAO_Video_Repository (const char* file_name)
  : filename_ (file_name),
    movie_info_ (0),
    num_movies_ (0),
    lastchanged_ (0)
{
  cout<<"The movie file is "<<filename_<<endl;
}

TAO_Video_Repository::~TAO_Video_Repository (void)
{
  TAO_VR::Movie_Info::freebuf (this->movie_info_);
}

CORBA::Any*
TAO_Video_Repository::evalDP (const char* name,
                              CORBA::TypeCode_ptr returned_type,
                              const CORBA::Any& extra_info,
			      CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CosTradingDynamic::DPEvalFailure))
{
  ACE_DEBUG ((LM_DEBUG,"TAO_Video_Repository::evalDP:%s\n",name));
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
	  else cout<<"The movie_database.txt file does not exist"<<endl;
	}

      TAO_VR::Movie_Info* movie_info = 0;
      ACE_NEW_RETURN (movie_info,
		      TAO_VR::Movie_Info (this->num_movies_,
					  this->num_movies_,
					  this->movie_info_,
					  0),
		      0);

      (*return_value) <<= movie_info;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,"TAO_Video_Repository::evalDP::stat failed\n"));
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

  TAO_VR::Movie_Info::freebuf (this->movie_info_);

  this->movie_info_ = TAO_VR::Movie_Info::allocbuf (num_lines);

  if (this->movie_info_ != 0)
    {
      current = ACE_OS::strtok (current, "%");
      while (current != 0)
	{
	  TAO_VR::Movie& movie = this->movie_info_[i];

	  movie.name_ = (const char*) current;
	  movie.audio_filename_ = (const char*) ACE_OS::strtok (0, delim);
	  movie.video_filename_ = (const char*) ACE_OS::strtok (0, delim);
	  movie.description_ = (const char*) ACE_OS::strtok (0, delim);

	  ACE_DEBUG ((LM_DEBUG,
		      "Movie Name: %s\nFile Name: %s\nDescription: %s\n",
		      (const char *) movie.name_,
		      (const char *) movie.audio_filename_,
		      (const char *) movie.video_filename_,
		      (const char *) movie.description_));

	  // From the actual movie file, extract the techincal information.
	  TAO_Video_Repository::
	    obtain_movie_info (movie.audio_filename_, movie);

	  TAO_Video_Repository::
	    obtain_movie_info (movie.video_filename_, movie);


	  current = ACE_OS::strtok (0, delim);
	  i++;
	}
    }

  this->num_movies_ = num_lines;
}

void
TAO_Video_Repository::
export_properties (TAO_Property_Exporter& prop_exporter)
{
  CORBA::Any extra_info;

  extra_info <<= MOVIE_INFO;
  CosTradingDynamic::DynamicProp* dp_struct =
    this->construct_dynamic_prop (MOVIE_INFO,
                                  TAO_VR::_tc_Movie_Info,
                                  extra_info);

  prop_exporter.add_dynamic_property (MOVIE_INFO, dp_struct);
}

int
TAO_Video_Repository::
define_properties (CosTradingRepos::ServiceTypeRepository::PropStructSeq& prop_seq,
		   CORBA::ULong offset) const
{
  CORBA::ULong num_props = prop_seq.length ();

  if (num_props <= offset)
    prop_seq.length (offset + 1);

  prop_seq[offset].name = MOVIE_INFO;
  prop_seq[offset].value_type = CORBA::TypeCode::_duplicate (TAO_VR::_tc_Movie_Info);
  prop_seq[offset].mode = CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY;

  return 1;
}
