/**
 * @file		cossb.hpp
 * @brief		COSSB Header
 * @author		Byunghun Hwang<bhhwang@nsynapse.com>
 * @date 		2015. 6. 9
 * @details
 */

#ifndef _COSSB_HEADER_HPP_
#define _COSSB_HEADER_HPP_

#if defined(__unix__) || defined(__gnu_linux__) || defined(linux) || defined(__linux__)


#include "version.hpp"
#include "errorcode.hpp"
#include "typedef.hpp"
#include "manager.hpp"
#include "broker.hpp"
#include "config.hpp"
#include "instance.hpp"
#include "logger.hpp"

#include "util/pid.h"
#endif

#endif
