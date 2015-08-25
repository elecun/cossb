

#include <base/configreader.hpp>
#include "instance.hpp"
#include "manager.hpp"
#include "broker.hpp"
#include "logger.hpp"
#include <string>
#include <iostream>

using namespace std;

namespace cossb {
namespace core {

bool cossb_init(const char* manifest)
{

	//1. load manifest file
	if(!cossb_config->load(manifest))
		return false;

	//2. create(setup) instances according to the manifest
	if(!cossb_system_manager->setup(cossb_config))
		return false;

	return true;
}

void cossb_destroy()
{
	cossb_stop();
	cossb_log->log(log::loglevel::INFO, "Now terminating...");
	cossb_system_manager->destroy();
	cossb_config->destroy();
}

bool cossb_sync()
{
	return true;
}


bool cossb_start()
{
	cossb_log->log(log::loglevel::INFO, "Now all components is running...");
	if(cossb_component_manager->run())
		return true;

	return false;
}

bool cossb_stop()
{
	cossb_log->log(log::loglevel::INFO, "Now all components is stopping...");
	if(cossb_component_manager->stop())
		return true;

	return false;
}



} /* namespace core */
} /* namespace cossb */
