/**
 * @file		iprofile.hpp
 * @brief		Component Profile interface class
 * @author		Byunghun Hwang<bhhwang@nsynapse.com>
 * @date 		2015. 6. 21
 * @details	Read component profile interface
 */


#ifndef _COSSB_IPROFILE_HPP_
#define _COSSB_IPROFILE_HPP_

#include <string>
#include <vector>
#include <map>
#include <boost/lexical_cast.hpp>
#include <cstring>
#include "../util/format.h"
#include "../util/uuid.hpp"
#include <ext/json.hpp>
#include <exception.hpp>

using namespace std;

namespace cossb {
namespace service {

/**
 * @brief	service method type
 */
enum class methodtype : int {
	PUBLISH = 1,
	SUBSCRIBE,
	UNDEFINED,
};

/**
 * @brief	service method
 */
typedef struct _service_method {
public:
	_service_method():method(methodtype::UNDEFINED) {}
	_service_method(const char* type) {
		string strType = type;
		std::transform(strType.begin(), strType.end(), strType.begin(), ::tolower);

		if(strType.compare("publish")==0)
			method = methodtype::PUBLISH;
		else if(strType.compare("subscribe")==0)
			method = methodtype::SUBSCRIBE;
		else
			method = methodtype::UNDEFINED;
	}
	const char* str() {
		string mt = "Undefined";
		switch(method) {
		case methodtype::PUBLISH: mt = "Publish"; break;
		case methodtype::SUBSCRIBE: mt = "Subscribe"; break;
		case methodtype::UNDEFINED: mt = "Undefined"; break;
		}
		return mt.c_str();
	}
	_service_method& operator= (_service_method const& m) { this->method = m.method; return *this; }
private:
	methodtype method = methodtype::UNDEFINED;
} service_method;

/**
 * @brief	service description
 */
typedef struct _service_desc {
	string name;				//service name
	service_method method;
	string topic;				//service topic
	const char* show() {
		return fmt::format("Name : {}\nMethod : {}\nTopic : {}", name, method.str(), topic).c_str();
	}
} service_desc;

/**
 * @brief	profile service section container
 */
typedef vector<service::service_desc> service_desc_container;

} /* namespace service */

namespace interface { class iprofile; }

namespace profile {

/**
 * @brief	device description
 */

typedef struct _device_desc {
	using json = nlohmann::json;

	util::uuid unique_id;
	string devicename;
	string component;
	int 	component_version = 0;
	string mac;
	string manufacturer;
	string author;
	string protocol;
	int port = 0;

	bool operator< (const _device_desc& other) const {
		return (this->unique_id < other.unique_id);
	}

	/**
	 * @brief	return as string (dump)
	 */
	const char* dump() {
		return this->profile.dump().c_str();
	}

	/**
	 * @brief	construct
	 */
	_device_desc(const char* data, int size) {

		try {

		profile = json::parse(data);

		if(profile.find("uuid")!=profile.end())			this->unique_id = util::uuid(profile["uuid"].get<string>());
		if(profile.find("devicename")!=profile.end())	this->devicename = profile["devicename"].get<string>();
		if(profile.find("mac")!=profile.end())			this->mac = profile["mac"].get<string>();
		if(profile.find("component")!=profile.end())		this->component = profile["component"].get<string>();
		if(profile.find("manufacturer")!=profile.end())	this->manufacturer = profile["manufacturer"].get<string>();
		if(profile.find("author")!=profile.end())			this->author = profile["author"].get<string>();
		if(profile.find("protocol")!=profile.end())		this->protocol = profile["protocol"].get<string>();
		if(profile.find("manufacturer")!=profile.end())	this->manufacturer = profile["manufacturer"].get<string>();
		if(profile.find("port")!=profile.end()) 			this->port = profile["port"].get<int>();
		if(profile.find("component_version")!=profile.end()) this->component_version = profile["component_version"].get<int>();

		}
		catch(std::exception& e) {
			throw profile::exception(cossb::profile::excode::PROFILE_INVALID, e.what());
		}
	}

private:
	json profile;

} device_desc;

/**
 * @brief	profile information section container
 */
typedef map<string, string> profile_info_container;

enum class section : unsigned int {
	info = 0,	//component information
	property, 	//properties
	resource,	//related resource
	service,	//service supporting
};

class type_value
{
public:
	type_value():value("") { }
	virtual ~type_value() { }

	friend class interface::iprofile;

	template<typename T>
	inline T as(T default_value) {
		try {
			T val = boost::lexical_cast<T>(value);
			return val;
		} catch( boost::bad_lexical_cast const& ) {
			return default_value;
		}
	}

	int asInt(int default_value) { return as(default_value); }
	unsigned int asUInt(unsigned int default_value) { return as(default_value); }
	unsigned long asUlong(unsigned long default_value) { return as(default_value); }
	double asDouble(double default_value) { return as(default_value); }
	float asFloat(float default_value) { return as(default_value); }
	bool asBool(bool default_value) { return as(default_value); }
	string asString(string default_value) { return as(default_value); }
	unsigned char asUChar(unsigned char default_value) { return as(default_value); }
	char asChar(char default_value) { return as(default_value); }

private:
	std::string value;

};
}

namespace driver { class component_driver; }

namespace interface {
class iprofile {

	friend class driver::component_driver;

public:
	iprofile() {
		_service_desc_container = new service::service_desc_container;
	}
	virtual ~iprofile() {
		delete _service_desc_container;
	}

	/**
	 * @brief	get profile
	 */
	virtual profile::type_value get(profile::section section, const char* element) = 0;

	/**
	 * @brief	update profile value
	 */

	virtual bool update(profile::section section, const char* element, const char* value) = 0;

	/**
	 * @brief	save profile
	 */
	virtual bool save() = 0;

	/**
	 * @brief	get multiple service descriptions
	 */
	service::service_desc_container* get_service_descs() const { return _service_desc_container; }

private:
	/**
	 * @brief	load profile
	 */
	virtual bool load(const char* filepath) = 0;

protected:
	/**
	 * @brief
	 */
	void set(profile::type_value& profile, string value) { profile.value = value; }

protected:

	/**
	 * @brief	service description container
	 */
	service::service_desc_container* _service_desc_container = nullptr;

};

} /* namespace interface */
} /* namespace cossb */

#endif
