/**
 * @file		config.hpp
 * @brief		COSSB configuration Management
 * @author		Byunghun Hwang<bhhwang@nsynapse.com>
 * @date 		2015. 6. 21
 * @details	Load configuration file
 */

#ifndef _COSSB_CONFIG_HPP_
#define _COSSB_CONFIG_HPP_

#include "arch/singleton.hpp"
#include <tinyxml2.h>
#include <list>
#include <string>
#include <map>

using namespace tinyxml2;
using namespace std;

namespace cossb {
namespace base {

enum class deptype : unsigned int { LIBRARY=1, COMPONENT=2 };

/**
 * @brief	system configuration class
 */
class config : public arch::singleton<config> {
public:

	config();
	virtual ~config();

	/**
	 * @brief	load configuration file(manifest file)
	 */
	bool load(const char* manifest_conf);

	/**
	 * @brief	update configuration
	 */
	bool update(config* conf);

	/**
	 * @brief	getting manifest information
	 */
	list<pair<string, string>> get_dependency() { return _dependency; };
	list<string> get_repository() { return _repository; }
	map<string, string> get_path() { return _path; }
	map<string, string> get_product() { return _product; }

private:
	void parse_dependency();
	void parse_path();
	void parse_repository();
	void parse_product();


private:
	tinyxml2::XMLDocument* _doc;
	list<pair<string, string>> _dependency;
	list<string> _repository;
	map<string, string> _path;
	map<string, string> _product;



};

} /* namespace base */
} /* namespace cossb */

#endif /* _COSSB_CONFIG_HPP_ */
