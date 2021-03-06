/**
Copyright (c) 2015, Nsynapse Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @mainpage	COSSB(Component-based Open & Simple Service Broker)
 * @details
 */


/**
 * @file		engine.cpp
 * @brief		COSSB Engine
 * @author		Byunghun Hwang<bhhwang@nsynapse.com>
 * @date 		2015. 6. 9
 * @details	COSSB Engine main starter
 */

#include <iostream>
#include <csignal>
#include <cstdlib>
#include <popt.h>
#include <memory>
#include <dirent.h>
#include <util/validation.hpp>
#include <cossb.hpp>
#include <util/uuid.hpp>

using namespace std;
using namespace cossb;

void destroy() {
	cossb::core::cossb_destroy();
	_exit(EXIT_SUCCESS);
}

/**
 * @brief	SIGINT signal callback function
 * @details	Stop all services and destroy all instances
 */
void sigc_interrupt(int param) {
	destroy();
}

/**
 * @brief	Main routine
 * @param	command
 * @details	Start with default components
 */
int main(int argc, char* argv[])
{
	signal(SIGINT, sigc_interrupt);

	char* manifest_file = nullptr;
	char* util = nullptr;
	char* target = nullptr;
	struct poptOption optionTable[] =
	{
		{"run",			'r', POPT_ARG_NONE, 0, 'r', "Run Engine with manifest configuration file", "manifest.xml file"},
		{"run-config",	'c', POPT_ARG_STRING, (void*)manifest_file, 'c', "Run Engine with manifest configuration file", "XML manifest file"},
		{"version",		'v', POPT_ARG_NONE, 0, 'v', "Version", "version"},
		{"utility", 	'u', POPT_ARG_STRING, (void*)util, 'u', "Run target utility for COSSB", "target utility"},
		{"list", 		'l', POPT_ARG_STRING, (void*)target, 'l', "Show list", "[component|utiltiy]"},
		POPT_AUTOHELP
		POPT_TABLEEND
	};
	poptContext optionCon = poptGetContext(NULL, argc, (const char**)argv, optionTable, 0);
	poptSetOtherOptionHelp(optionCon, "<option>");

	if(argc<2)
	{
		std::cout << poptStrerror(POPT_ERROR_NOARG) << endl;
		_exit(EXIT_SUCCESS);
	}

	//only one opt
	int opt = poptGetNextOpt(optionCon);
	if(opt>=0)
	{
		switch(opt)
		{
		// install components
		case 'v':
		{
			std::cout << COSSB_NAME << COSSB_VERSION << " (Released " << __DATE__ << " " <<__TIME__ << ")" << std::endl;
			_exit(EXIT_SUCCESS);

		} break;

		//run with default configuration file(manifest.xml)
		case 'r':
		{
			if(!cossb::core::cossb_init("manifest.xml"))
				destroy();
			else
				cossb_log->log(log::loglevel::INFO, fmt::format("{}{} Now Starting....",COSSB_NAME, COSSB_VERSION).c_str());

			//start the cossb service
			cossb::core::cossb_start();
			pause();
		}
		break;

		//run with custom config file
		case 'c':
		{
			string manifest = (const char*)poptGetOptArg(optionCon);

			if(!manifest.empty())
			{
				if(!cossb::core::cossb_init(manifest.c_str()))
					destroy();
				else
					cossb_log->log(log::loglevel::INFO, fmt::format("{}{} Now Starting....",COSSB_NAME, COSSB_VERSION).c_str());

				//start the cossb service
				cossb::core::cossb_start();
				pause();
			}
		}
			break;

		//use COSSB utility
		case 'u':
		{
			cossb_log->log(log::loglevel::INFO, "Initializing....");
			if(!cossb::core::cossb_init("manifest.xml", false))
				destroy();
			else
				cossb_log->log(log::loglevel::INFO, fmt::format("{}{} Now Starting....",COSSB_NAME, COSSB_VERSION).c_str());

			string target = (const char*)poptGetOptArg(optionCon);

			interface::iutility* _utility = new util::utilloader(target.c_str());
			if(!_utility->execute(argc, argv))
				cossb_log->log(log::loglevel::ERROR, fmt::format("Cannot execute '{}' utility", target).c_str());
			delete _utility;

		}
		break;

		//show list of what
		case 'l':
		{
			if(!cossb::core::cossb_init("manifest.xml", false))
				destroy();
			else
			{
				string target = (const char*)poptGetOptArg(optionCon);
				std::transform(target.begin(), target.end(), target.begin(), ::tolower);

				const char* targets[] = {"component", "utility"};
				std::vector<string> ltarget(targets,targets+sizeof(targets)/sizeof(char*));

				std::vector<string>::iterator itr = std::find(ltarget.begin(), ltarget.end(), target);
				if(itr!=ltarget.end()) {
					int pos = itr-ltarget.begin();
					cossb_log->log(log::loglevel::INFO, fmt::format("Show all {}", target).c_str());
					struct dirent* ent;
					struct stat st;
					DIR* dir;

					switch(pos) {
					case 0:	//component
					{
						string path = cossb_config->get_path()["component"];
						if(!path.empty()) {
							dir = ::opendir(path.c_str());
							while((ent = readdir(dir))) {
								const string filename = ent->d_name;
								const string fullpath = path+filename;

								if(!stat(fullpath.c_str(), &st) && (st.st_mode&S_IFMT)==S_IFREG) {
									if(filename.substr(filename.find_last_of(".") + 1) == "comp") {
										cossb_log->log(log::loglevel::INFO, fmt::format("Component : {}",filename));
									}
								}
							}
							closedir(dir);
						}
					}
						break;

					case 1: //utility
					{
						string path = cossb_config->get_path()["utility"];
						if(!path.empty()) {
							dir = ::opendir(path.c_str());
							while((ent = readdir(dir))) {
								const string filename = ent->d_name;
								const string fullpath = path+filename;

								if(!stat(fullpath.c_str(), &st) && (st.st_mode&S_IFMT)==S_IFREG) {
									if(filename.substr(filename.find_last_of(".") + 1) == "util") {
										cossb_log->log(log::loglevel::INFO, fmt::format("Utility : {}",filename));
									}
								}
							}
							closedir(dir);
						}
					}
						break;
					}
				}
			}
		}
		break;

		}
	}

	if (opt<-1)
	{
		cout << poptBadOption(optionCon, POPT_BADOPTION_NOALIAS) << ":" << poptStrerror(opt) << endl;
		destroy();
	}

	poptFreeContext(optionCon);

	destroy();

	return 0;
}
