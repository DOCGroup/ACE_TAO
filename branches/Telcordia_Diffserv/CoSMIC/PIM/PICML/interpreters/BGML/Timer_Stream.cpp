#include "BGML/Timer_Stream.h"
#include <ctype.h>

void
Timer_Stream::incr_indent (unsigned short flag)
{
	++indent_level_;
}

// Indentation
void
Timer_Stream::decr_indent (unsigned short flag)
{
	if (this->indent_level_ > 0)
    {
		--this->indent_level_;
    }
}

void
Timer_Stream::reset (void)
{
	this->indent_level_ = 0;
}

// Indented print.
void
Timer_Stream::indent (void)
{
	// Based on the current indentation level, leave appropriate number of blank
	// spaces in the output.
	if (this->indent_level_ > 0)
    {
		for (int i = 0; i < this->indent_level_; ++i)
        {
			this->strm_ << "  ";
        }
    }
}

void
Timer_Stream::nl (void)
{
	this->strm_ << std::endl;
}

Timer_Stream &
Timer_Stream::operator<< (const std::string &s)
{
	this->strm_ << s.c_str ();
	return *this;
}

Timer_Stream &
Timer_Stream::operator<< (const char *str)
{
	this->strm_ << str;
	return *this;
}

//////////////////////////////////////////////////////

Timer_Stream::Timer_Stream (std::ostream& fstream)
: strm_(fstream), indent_level_ (0)
{

}

Timer_Stream::~Timer_Stream ()
{

}

void
Timer_Stream::write_includes (void)
{
	this->strm_ << "//-- c++ --";
	this->nl ();
	this->nl ();

	this->strm_ << "// This is the static header class that is included that has all the timing information";
	this->strm_ <<"#include \"ace/High_Res_Timer.h\"";
	this->nl ();

	this->strm_ << "#include \"ace/Sched_Params.h\"";
	this->nl ();

	this->strm_ << "#include \"ace/Stats.h\"";
	this->nl ();

	this->strm_ << "#include \"ace/Sample_History.h\"";
	this->nl ();
	this->nl ();

	this->strm_ << "// Fill out the number of iterations....\n";
	this->strm_ << "int niterations = ...";
	this->nl ();

	this->strm_ << "ACE_Sample_History history (niterations);\n";
	this->strm_ << "int timer_count;\n";

	this->strm_ << "ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();\n";
	this->strm_ << "ACE_hrtime_t start_timer_probe, finish_timer_probe, test_start, test_end;\n";
	this->nl ();
}

void
Timer_Stream::write_start_time_probe (void)
{
	this->nl ();
	this->strm_ << "void start_time_probe ()\n";
	this->strm_ << "{\n";
	this->incr_indent ();
	this->indent ();
	this->strm_ << "if (!timer_count)\n";
	this->incr_indent ();
	this->indent ();
	this->strm_ << "test_start = ACE_OS::gethrtime ();\n";
	this->decr_indent ();
	this->indent ();
    this->strm_ << "start_timer_probe = ACE_OS::gethrtime ();\n";
	this->decr_indent ();
	this->strm_ << "}\n";
}

void
Timer_Stream::write_stop_time_probe ()
{
	this->nl ();
	this->strm_ << "void stop_time_probe ()\n";
	this->strm_ << "{\n";

	this->incr_indent ();
	this->indent ();

	this->strm_ << "finish_timer_probe = ACE_OS::gethrtime ();\n";
	this->indent ();

	this->strm_ << "history.sample (finish_timer_probe - start_timer_probe);\n";
	this->indent ();

	this->strm_ << "ACE_DEBUG ((LM_DEBUG, \"iteration %d \\n\", ++ timer_count));\n";
	this->indent ();

	this->strm_ << "if (timer_count == niterations)\n";
	this->indent ();
	this->strm_ << "{\n";

	this->incr_indent ();
	this->indent ();
	this->strm_ << "test_end = ACE_OS::gethrtime ();\n";
	this->indent ();
	this->strm_ << "ACE_DEBUG ((LM_DEBUG, \"test finished \"));\n";
	this->indent ();
	this->strm_ << "ACE_Basic_Stats stats;\n";
	this->indent ();
	this->strm_ << "history.collect_basic_stats (stats);\n";
	this->indent ();
	this->strm_ << "stats.dump_results (\"Total\", gsf);\n";
	this->indent ();
	this->strm_ << "ACE_Throughput_Stats::dump_throughput (\"Total\", gsf,";
	this->strm_ << "test_end - test_start, stats.samples_count ());\n";
	this->decr_indent ();
	this->indent ();
	this->strm_ << "}\n";
	this->strm_ << "}";
}
