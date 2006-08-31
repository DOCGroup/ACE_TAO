#ifndef METRIC_EMITTER_C
#define METRIC_EMITTER_C

#include <fstream>
#include <ctype.h>

#include "BGML/MetricEmitter.h"
#include "BGML/IDL_Util.h"
#include "BGML/Benchmark_Stream.h"
#include <algorithm>

template <class T>
MetricEmitter<T>::MetricEmitter (PICML::OperationRef &ref,
								 T& latency,
								 std::string& metric,
								 BGML_Data& state)
: operation_ref_ (ref),
  latency_ (latency),
  metric_ (metric),
  bgml_state_ (state)
{

}


template <class T>
MetricEmitter<T>::~MetricEmitter ()
{

}

template<class T>
void
MetricEmitter<T>::generate_header_file (std::string& class_name,
										std::string& component_name,
										std::string& operation_name,
										std::vector<string>& arg_list,
										bool is_main_task)
{
	std::string output_file = this->bgml_state_.output_path + "\\" + class_name + ".h";
	std::ofstream output_stream (output_file.c_str (), ios::out);
	BenchmarkStream bench_stream (component_name,
								  operation_name,
								  arg_list,
								  output_stream,
								  this->bgml_state_);
	bench_stream.generate_task_header (class_name, is_main_task);
}

template <class T>
void
MetricEmitter<T>::create_build_file (std::vector<std::string>& file_list,
									 std::string& project_name,
									 std::string& dependant_list)
{
	// Open the build stream
	std::string build_file = this->bgml_state_.output_path + "\\" + project_name + ".mpc";
	std::ofstream build_stream (build_file.c_str (), ios::out);

	// Create project
	build_stream << "project (";
	build_stream << project_name << ") : ciao_component_dnc {";
	build_stream << "\n";

	// Create include definition and libs+
	std::string home_include = "\"$(BGML_HOME)\"";
	build_stream << "  includes += "
				 << home_include.c_str ()
				 << " \n";
	build_stream << "  libs     += BGML_Base ";
	build_stream << dependant_list.c_str ();

	/// ---- DYNAMIC FLAGS NEED NOT BE GENERATED FOR NOW ---
	/*
	build_stream << "\n  dynamicflags = ";

	std::transform (project_name.begin(),
				    project_name.end(),
					ostream_iterator <char> (build_stream),
					toupper);
	// There has to a "_BUIL_DLL" convention added"
	build_stream << "_BUILD_DLL";
	build_stream << "\n";
	*/

	// Add the Benchmarking Source files
	build_stream << "\n";
	build_stream << "  Source_Files { \n";
	for (size_t i = 0; i < file_list.size (); i++)
	{
		build_stream << "    ";
		build_stream << file_list [i];
		build_stream << "\n";
	}

	build_stream << "  }\n";

	// Add NULL IDL file definition
	build_stream << "  IDL_Files { \n";
	build_stream << "  }\n";

	build_stream << "}\n";

	// Close the file
	build_stream.close ();
}

template <class T>
void
MetricEmitter<T>::generate_workload_files (std::string &component_name,
										   std::string &operation_name,
										   std::vector<std::string> &arg_list,
										   std::vector<std::string> &source_file_list,
										   bool is_main,
										   BGML_Task_Group_Data &data)
{
	std::string class_name = component_name + "_" + operation_name + "_Workload";

	// Add a barrier to the list of arguments
	std::vector<std::string> temp_list (arg_list);
	temp_list.push_back ("ACE_Barrier&");
	is_main = 0; /* Workload headers, not main benchmarking class */
	this->generate_header_file (class_name,
		                        component_name,
		                        operation_name,
		                        temp_list,
		                        is_main);

	std::string workload_cpp_file =
		this->bgml_state_.output_path + "\\" + component_name
		+ "_" + operation_name + "_Workload.cpp";
	source_file_list.push_back (operation_name + "_Workload.cpp");

	std::ofstream workload_stream (workload_cpp_file.c_str (), ios::out);
	BenchmarkStream workload_cpp_stream (component_name,
		                                 operation_name,
		                                 temp_list,
		                                 workload_stream,
		                                 this->bgml_state_);

	workload_cpp_stream.generate_workload_def (this->bgml_state_.benchmark_iterations,
											   data);
}

template <class T>
void
MetricEmitter<T>::generate_workload_tasks_that_differ (BGML_Task_Group_Data &task_group_data,
													   std::string &component_name,
													   std::vector<std::string> &source_file_list,
													   bool is_main)
{
	for (size_t j = 0; j < task_group_data.background_operations.size (); j++)
	{
		/// Obtain the Operation Reference Element
		PICML::TwowayOperation twoway_op =
			PICML::TwowayOperation::Cast (task_group_data.background_operations [j]);
		std::string& operation_name = IDL_Util::operation_name (twoway_op);
		std::vector<std::string>& arg_list = IDL_Util::argument_list (twoway_op);


		this->generate_workload_files (component_name,
									   operation_name,
			                           arg_list,
			                           source_file_list,
			                           is_main,
									   task_group_data);
	}
}


template <class T>
void
MetricEmitter<T>::generate_benchmark ()
{
	// First obtain the file_name, warmup and iterations
	this->bgml_state_.warmup_iterations = latency_.warmup();
	this->bgml_state_.file_name = latency_.fileName();
	this->bgml_state_.benchmark_iterations = latency_.iterations ();

	// Use the operation Name to get the name of the Component
	// This is necessary to get the signature of the remote operation

	// PART 1: Obtaing Component Name
	PICML::OperationBase operation_base = operation_ref_.ref ();
	PICML::TwowayOperation twoway_op = PICML::TwowayOperation::Cast (operation_base);

	// Check if there is a component associated with this operation, if so
	// use that component, else figure out which component I am associated
	// with
	std::string& component_name = IDL_Util::component_name (this->operation_ref_);
	std::string& operation_name = IDL_Util::operation_name (twoway_op);
	std::vector<std::string>& arg_list = IDL_Util::argument_list (twoway_op);

	//// Generate the Benchmarking Header
	std::string class_name = "Benchmark_" + component_name + "_" + operation_name;

	/* Flag to show that the header file is for the main benchmarking class */
	bool is_main = 1;
	this->generate_header_file (class_name,
								component_name,
								operation_name,
								arg_list,
								is_main);


	/// This vector maintains a list of all file names that have been
	/// generated till now:
	/// E.g.:
	///      -- Benchmark_get_message.h
	///      -- Benchmark_get_message.cpp
	std::vector<std::string> source_file_list;

	for (size_t i =0;
		 i < this->bgml_state_.task_group_data.size ();
		 i++)
	{
		is_main = 0; /* All files here are workload files */
		BGML_Task_Group_Data &task_group_data =
			this->bgml_state_.task_group_data [i];

		/// Check to see if there is any task that is running another
		/// Three cases arise in this situation:
		/// 1: size = 0: Meaning all background tasks run the same operation as the
		///              main task
		/// 2: size > 0 but size < number of tasks: In this case some tasks run
		///              different operation than main task
		/// 3: size == number of tasks: In this case all tasks are running different
		///              operations than the main benchmarking task
		if (! task_group_data.background_operations.size ())
		{
			/// Case 1 --- Generate the .h and .cpp file for this case
			this->generate_workload_files (component_name,
										   operation_name,
										   arg_list,
										   source_file_list,
										   is_main,
										   task_group_data);
		}
		else if (task_group_data.size  ==
			task_group_data.background_operations.size ())
		{
			/// Case 2 --- Generate .h and .cpp files for each task

			/// NOTE: Component name has to be the same!!
			this->generate_workload_tasks_that_differ (task_group_data,
													   component_name,
													   source_file_list,
													   is_main);
		}
		else
		{
			/// Case 3 --- There will be a file benchmarking the same operation as the
			/// background task and there will be files corresponding to other operations

			/// File corresponding to the main task
			this->generate_workload_files (component_name,
										   operation_name,
										   arg_list,
										   source_file_list,
										   is_main,
										   task_group_data);

			/// Files corresponding to different tasks
			this->generate_workload_tasks_that_differ (task_group_data,
													   component_name,
													   source_file_list,
													   is_main);

		}
	}

	///// Generate .cpp file for the task ///////////////////////////
	std::string cpp_file =
		this->bgml_state_.output_path + "\\" + "Benchmark_" +
		  component_name + "_" + operation_name + ".cpp";
	source_file_list.push_back ("Benchmark_" + operation_name + ".cpp");

	std::ofstream cpp_stream (cpp_file.c_str (), ios::out);

	this->bgml_state_.benchmark_rate = (this->bgml_state_.benchmark_rate <= 0) ? -1 : this->bgml_state_.benchmark_rate;
	this->bgml_state_.benchmark_priority = (this->bgml_state_.benchmark_priority < 0) ? -1 : this->bgml_state_.benchmark_priority;

	BenchmarkStream bench_cpp_stream (component_name,
									  operation_name,
									  arg_list,
									  cpp_stream,
									  this->bgml_state_);

	bench_cpp_stream.generate_task_def (this->metric_, this->bgml_state_);

	/* ------------ DO NOT NEED THIS FOR NOW ----------------------------
	// Create the mpc file for building the benchmarks
	//std::string project_name = "Benchmark_" + operation_name;
	//std::string dependant_libs = IDL_Util::dependant_idls (twoway_op);

	// The generated files are templates so we can directly include them rather
	// than create this as a separate library
	this->create_build_file (source_file_list, project_name, dependant_libs);
	*/
}

#endif // METRIC_EMITTER_C
