/**
 * @file		exception.hpp
 * @brief		COSSB exception
 * @author		Byunghun Hwang<bhhwang@nsynapse.com>
 * @date 		2015. 8. 23
 * @details	cossb exception header
 */


#ifndef _COSSB_EXCEPTION_HPP_
#define _COSSB_EXCEPTION_HPP_

#include <string>
#include <iostream>
#include <exception>
#include <stdexcept>
#include "logger.hpp"
#include "util/format.h"

using namespace std;


namespace cossb {
namespace exception {
class cossb_exception : public std::exception {
public:
	cossb_exception() { }
	~cossb_exception() throw() { }
	virtual const char* what() const throw() {
		return exception_str.c_str();
	}
protected:
	void set(const char* msg) { exception_str = msg; }
private:
	std::string exception_str;
};
} /* namespace exception */

namespace broker {
enum class excode : int {
	DRIVER_NOT_FOUND=0,
};

/**
 * @brief	broker exception
 */
class exception : public cossb::exception::cossb_exception {
public:
	exception(broker::excode code, const char* err_msg = nullptr) {
		switch(code)
		{
		case	excode::DRIVER_NOT_FOUND: err_msg?set(fmt::format("Driver cannot be found : {}",err_msg).c_str()):set("Driver cannot be found");  break;
		default:
			set("Unknown Exception");
		}
	}
};

} /* namespace broker */


namespace driver {

enum class excode : int {
	COMPONENT_LOAD_FAIL = 0,	//driver cannnot be loaded own component
	COMPONENT_UNLOAD_FAIL,
	COMPONENT_OPEN_ERROR,
};

/**
 * @brief	component driver exception
 */
class exception : public cossb::exception::cossb_exception {
public:
	exception(driver::excode code, const char* err_msg = nullptr) {
		switch(code) {
		case excode::COMPONENT_LOAD_FAIL: err_msg?set(fmt::format("Component load failed : {}",err_msg).c_str()):set("Component load failed"); break;
		case excode::COMPONENT_UNLOAD_FAIL: err_msg?set(fmt::format("Component unload failed : {}",err_msg).c_str()):set("Component unload fail"); break;
		case excode::COMPONENT_OPEN_ERROR: err_msg?set(fmt::format("Component open failed : {}",err_msg).c_str()):set("Component open failed"); break;
		default: set("Unknown exception");
		}
	}
};

} /* namespace driver */

namespace profile {
enum class excode : int {
	PROFILE_LOAD_FAIL = 0,
	PROFILE_MISSING,
	PROFILE_INVALID,
};

class exception : public cossb::exception::cossb_exception {
public:
	exception(profile::excode code, const char* err_msg = nullptr) {
			switch(code) {
			case excode::PROFILE_LOAD_FAIL: err_msg?set(fmt::format("Profile load failed : {}",err_msg).c_str()):set("Profile load fail"); break;
			case excode::PROFILE_MISSING: err_msg?set(fmt::format("Invalid missing : {}",err_msg).c_str()):set("Profile missing"); break;
			case excode::PROFILE_INVALID: err_msg?set(fmt::format("Invalid Profile : {}",err_msg).c_str()):set("Invalid profile"); break;
			default: set("Unknown exception");
			}
		}
};

}


} /* namespace cossb */


#endif /* _COSSB_EXCEPTION_HPP_ */
