// PA2CSTransWrap.cpp : Defines the entry point for the console application.
//


// test.h is created with the udm application from the test paradigm
#include "PICML.h"
#include "cadena_scenario.h"
#include "UdmGme.h"
#include "UdmDom.h"
#include "UdmCORBA.h"
#include "UdmCORBA_Logger.h"
#include "UdmUtil.h"

//My headers - Gabriele
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <strstream>
#include <direct.h>
#include <shlwapi.h>

namespace
{
	class MutexAutoRelease //For exception safety
	{
	public:
		MutexAutoRelease(const HANDLE hMutex) : _hMutex(hMutex) {}
		~MutexAutoRelease() {ReleaseMutex(_hMutex);}
	protected:
		HANDLE _hMutex;
	};
}; //unnamed namespace

Logger logger = Logger("PA2CS_TranslatorWrapper");

class myTranslator 
	: public UdmCORBA::Translator 
{
public:
	myTranslator(const Udm::UdmDiagram &from_metainfo, const char *from_name, const char *from_version, const Uml::Class &from_rootclass, 
				 const Udm::UdmDiagram &to_metainfo, const char *to_name, const char *to_version, const Uml::Class &to_rootclass, 
				 int argc, char **argv, Logger *logger
				 ) : Translator(from_metainfo, from_name, from_version, from_rootclass, 
								to_metainfo, to_name, to_version, to_rootclass, 
								argc, argv, logger) 
				{};


	// this function is called when a datanetwork arrives
	void notify(const char *name, const char *version, const char *remarks, const unsigned long numOfDocs) 
	{
		try 
		{
			cout << "PA2CS_TransWrap: Document received" << endl;

			//We place a mutex here so that there is synchronized access to the PICML_LastExported.mga file by the export and import algorithms
			HANDLE hMutex = CreateMutex(NULL,FALSE,"PICML-Cadena integration mutex");
			WaitForSingleObject(hMutex, INFINITE);
			MutexAutoRelease mar(hMutex);  //For exception safety


			const char * picml2cadena_path_ptr = getenv("PICML2CADENA_PATH");
			if (picml2cadena_path_ptr == NULL) throw udm_exception("PICML2CADENA_PATH enviroment variable not found! Can't run PA2CS/CS2PA transformations.");

			string picml2cadena_path = picml2cadena_path_ptr;
			if (picml2cadena_path.find_last_of('\\') != picml2cadena_path.size()-1) picml2cadena_path += '\\';


			const char transformation_config[]	= "PICML2Cadena_Configuration.mga";
			const char cadenascenario_file[]	= "PA2CSTransWrap_CadenaScenario.xml";
			const char picml_file[]				= "PICML_LastExported.mga";
			const char dummy_embedded_file[]	= "PA2CSTransWrap_DummyEmbedded.xml";
			const char debug_flag[]				= "-dv";
			const char space		= ' ';
			const char quotes		= '"';

			char cwd_buf[MAX_PATH]; _getcwd(cwd_buf, MAX_PATH);
			string cwd = cwd_buf; if (cwd.find_last_of('\\') != cwd.size()-1) cwd += '\\';


			string transformation_fullpath = picml2cadena_path + "Transformations\\"+transformation_config;
			if (FALSE==PathFileExists(transformation_fullpath.c_str())) throw udm_exception(string()+"Configuration file for the transformation \""+transformation_fullpath+"\" not found! Can't run the transformation.");
			const char * great_path_ptr = getenv("GREAT_PATH");
			if (great_path_ptr==NULL) throw udm_exception("GREAT_PATH enviroment variable not found! Please install GReAT.");

			string great_path = great_path_ptr;
			if (great_path.find_last_of('\\') != great_path.size()-1) great_path += '\\';

			string gre_fullpath = great_path + "Bin\\GRE.exe";
			if (FALSE==PathFileExists(gre_fullpath.c_str())) throw udm_exception(string()+'"'+gre_fullpath+"\" not found! GRengine is needed to run the transformation. Please reinstall GReAT.");


			string cadenascenario_schema = picml2cadena_path + "Transformations\\Udm\\cadena_scenario.xsd";
			if (FALSE==PathFileExists(cadenascenario_schema.c_str())) throw udm_exception(string()+"Schema file for cadena_scenario \""+cadenascenario_schema+"\" not found! Can't run the transformation.");


			UdmGme::GmeDataNetwork disk_gmedn(PICML::diagram); //(const Udm::UdmDiagram &metainfo, Udm::UdmProject* project = NULL);
			disk_gmedn.CreateNew("PICML_LastExported.mga", "PICML", PICML::RootFolder::meta); //(const string &systemname, const string &metalocator, const Uml::Class &rootclass, enum Udm::BackendSemantics sem = Udm::CHANGES_PERSIST_ALWAYS);
			UdmUtil::CopyObjectHierarchy(fromDN->GetRootObject().__impl(), disk_gmedn.GetRootObject().__impl(), &disk_gmedn, UdmUtil::copy_assoc_map()); //(Udm::ObjectImpl* p_srcRoot, Udm::ObjectImpl* p_dstRoot, Udm::DataNetwork* p_dstBackend, copy_assoc_map &cam);
			disk_gmedn.CloseWithUpdate();


			string greinvocationcommand;
			greinvocationcommand = gre_fullpath+ space +quotes+transformation_fullpath+quotes;
			greinvocationcommand += string()+space+quotes+"PICML_File="+cwd+picml_file+quotes;
			greinvocationcommand += string()+space+quotes+"CadenaScenario_File="+cwd+cadenascenario_file+quotes;
			greinvocationcommand += string()+space+quotes+"EmbeddedDummy_File="+cwd+dummy_embedded_file+quotes;
			greinvocationcommand += string()+space+debug_flag;


			cout << "PA2CS_TransWrap: " << "Starting GRE with commandline:"<< endl << greinvocationcommand << endl;

			system(greinvocationcommand.c_str());

			//Let's fetch the transformed file and publish it on the backplane
			UdmDom::DomDataNetwork disk_ddn(cadena_scenario::diagram);
			disk_ddn.OpenExisting("PA2CSTransWrap_CadenaScenario.xml", ""); //systemname, metalocator (empty: using the one written in the file)
			
			UdmUtil::CopyObjectHierarchy(disk_ddn.GetRootObject().__impl(), toDN->GetRootObject().__impl(), toDN, UdmUtil::copy_assoc_map());

			disk_ddn.CloseNoUpdate();

			
			// publish the datanetwork which is in the toDN Translator variable
			publish(name, version, (string(remarks)+" --> PA2CS-translated").c_str());
		}
		catch (exception &e)
		{
			cout << "CS2PA_TransWrap: " << e.what() << endl; //Doing it outside the tr.run() does not seem to output the string into the backplane console
			throw;
		}
	}
};

int main(int argc, char* argv[]) 
{

	//Looping let's check that it really started
	//MessageBox(NULL,"Translator starting now", "Starting now", MB_OK);
	//for (int i =0;;) { i++;}

	try {
	// instantiate a translator, the test::diagram variable is defined in the
	// {paradigm_name}.h (here, test.h), Root::meta is the rootobject’s meta
	// defined in the {paradigm_name}.h (here, test.h)

		myTranslator tr(PICML::diagram, "PICML", "1.0", PICML::RootFolder::meta,
						cadena_scenario::diagram, "cadena_scenario", "1.0", cadena_scenario::Scenarios::meta,
						argc, argv, &logger);
	
	// run the translator, this is an infinite loop to check the arrived
	// documents and call the notify method
	tr.run();

	cout << "PA2CS_TransWrap: exiting" << endl;
	}
	catch (exception &e) 
	{
		logger.log(1, "Error", e.what());
	}
	catch (...) 
	{
		cerr << "CS2PA_TransWrap: " << "Fatal std::exception " << endl;
		logger.log(1, "Error", "Fatal std::exception");
	}
	return 0;
}
