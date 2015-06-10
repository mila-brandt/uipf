#include <iostream>
#include <string>
#include <QApplication>
#include "framework/ModuleManager.hpp"
#include "framework/Configuration.hpp"
#include "framework/Utils.hpp"

// foor Boost:
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace uipf;


// argument is a configFile
int main(int argc, char** argv){

	if (argc < 2) {
		std::cerr << "Usage Error, at least one argument is required!\n\n";
		std::cerr << "Usage: " << argv[0] << " -c <configFile>"<< std::endl;
		return 1;
	}

	// used by the module manager to access Qt components, TODO maybe move to module manager?
	QApplication app (argc,argv);

	ModuleManager mm;
	Configuration conf;

	if (string(argv[1]).compare("-c") == 0){
	// run a processing chain from a config file.
	// ./uipf -c example.yam

		// loads the configFile and create a Configuration
		string configFileName = argv[2];
		conf.load(configFileName);


	} else{
	// run a single module.
	// ./uipf <moduleName> ...options...

		//~ try {

			// Declare three groups of options.
				// Declare a group of options that will be allowed only on command line
				po::options_description generic("Generic options");
				generic.add_options()
					("version,v", "print version string")
					("help", "produce help message")
					;

				// Declare a group of options that will be
				// allowed both on command line and in
				// config file
				po::options_description config("Configuration");
				config.add_options()
					("input,i", po::value< vector<string> >()->composing(), "defines an input, can be used multiple times \n format: inputName:fileName \n inputName is optional if there is only one input")
					("output,o",	po::value< vector<string> >()->composing(), "defines an output, can be used multiple times \n format: outputName:fileName \n outputName is optional if there is only one output \n output itself is optional, if there is only one input and one output, \n the output filename will be choosen from the input name in this case.")
					("param,p",	po::value< vector<string> >()->composing(),	"defines a parameter, format:  name:valu")
					;

				// Hidden options, will be allowed both on command line and
				// in config file, but will not be shown to the user.
				po::options_description hidden("Hidden options");
				hidden.add_options()
					("modulename,m", po::value< string>(), "defines the name of the module")
					;

			// These two lines say that all positional options should be translated into "-modulename" options.
			po::positional_options_description p;
			p.add("modulename", -1);

			// Declare an options description instance which will include all the options
			po::options_description all("Allowed options");
			all.add(generic).add(config).add(hidden);

			// Declare an options description instance which will be shown to the user
			po::options_description visible("Allowed options");
			visible.add(generic).add(config);


			po::variables_map vm;
			store(parse_command_line(argc, argv, all), vm);
			po::notify(vm);

			if (vm.count("version")) {
				cout << "Version: 1.0" << "\n";
				return 0;
			}

			if (vm.count("help")) {
				//~ cout << visible;
				cout << all;
				return 0;
			}

			string modName = vm["modulename"].as<string>();
			MetaData md = mm.getModuleMetaData(modName);

			ProcessingStep processModule;
			processModule.name = "processModule";
			processModule.module = modName;

			if (vm.count("input")) {
				vector<string> inputs = vm["input"].as< vector<string> >();
				// this step is repeated as often, as the number of load modules is created
				for (unsigned int i=0; i<inputs.size(); i++){
					ProcessingStep loadModule;
					loadModule.name = "loadModule" + to_string(i);
					loadModule.module = "loadImage";

					string source = utils::secondPart(inputs[i]);

					loadModule.params.insert (pair<string,string>("filename",source) );

					conf.addProcessingStep(loadModule);

					// the input params of the module are set
					map<string, DataDescription> in = md.getInputs();
					string name;
					// if there is only one input, the name is optional on the command line, will be taken from metadata
					if (in.size() == 1) {
						auto it = in.cbegin();
						name = it->first;
					} else {
						name = utils::firstPart(inputs[i]);
					}
					pair<string,string> loadWithValue(loadModule.name, "image");

					processModule.inputs.insert(pair<string, pair<string, string> >(name, loadWithValue));
				}

				// if only one input, create an output automatically, if none is given explicitly
				if (inputs.size() == 1 && vm.count("output") == 0){
					map<string, DataDescription> out = md.getOutputs();
					// if only one output exists
					if(out.size() ==1){

						auto it = out.cbegin();
						string outName = it->first;

						pair<string, string> storeSource(processModule.name,outName);

						ProcessingStep storeModule;
						storeModule.name = "storeModule";
						storeModule.module = "storeImage";

						// where does the image come from
						storeModule.inputs.insert (pair<string, pair<string, string> >("image", storeSource));

						// where should it be stored
						string newName = utils::rename(utils::secondPart(inputs[0]));
						storeModule.params.insert (pair<string,string>("filename", newName));

						conf.addProcessingStep(storeModule);
					}
				}

			}


			if (vm.count("output")) {

				map<string, DataDescription> out = md.getOutputs();

				vector<string> outputs = vm["output"].as< vector<string> >();
				// this step is repeated as often, as the number of store modules is created
				for (unsigned int i=0; i<outputs.size(); i++){

					// if there is only one output, the name is optional on the command line, will be taken from metadata
					string outName;
					if (out.size() == 1) {
						auto it = out.cbegin();
						outName = it->first;
					} else {
						outName = utils::firstPart(outputs[i]);
					}

					pair<string, string> storeSource(processModule.name, outName);

					ProcessingStep storeModule;
					storeModule.name = "storeModule" + to_string(i);
					storeModule.module = "storeImage";

					// where does the image come from
					storeModule.inputs.insert (pair<string, pair<string, string> >("image",storeSource) );
					// where should it be stored
					string storeName = utils::secondPart(outputs[i]);
					storeModule.params.insert (pair<string,string>("filename",storeName) );

					conf.addProcessingStep(storeModule);

				}
			}


			if (vm.count("param")) {

				vector<string> params = vm["param"].as< vector<string> >();
				// this step is repeated as often, as the number of params is inserted
				for (unsigned int i=0; i<params.size(); i++){

					processModule.params.insert (pair<string,string>(utils::firstPart(params[i]), utils::secondPart(params[i])));

				}
			}

			conf.addProcessingStep(processModule);

		//~ }
		//~ catch(exception& e) {
			//~ cout << e.what() << "\n";
			//~ return 1;
		//~ }

	}

	// only for debug, print the loaded config
	conf.store("test.yaml");

	// validate configuration and show errors
	vector<string> errors = conf.validate(mm.getAllModuleMetaData());
	if (!errors.empty()) {
		cout << endl << "There are configuration errors!" << endl << endl;
		for(unsigned int i = 0; i < errors.size(); ++i) {
			cout << errors[i] << endl;
		}
		return 1;
	}

	// run the current configuration
	mm.run(conf);

	return 0;
}
