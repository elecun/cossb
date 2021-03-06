/*
 * message.cpp
 *
 *  Created on: 2015. 10. 12.
 *      Author: hwang
 */

#include "message.hpp"
#include "interface/icomponent.hpp"

namespace cossb {
namespace message {

message::message(interface::icomponent* component, msg_type type, msg_target target)
{
	frame.type = type;
	frame.from = component->get_name();
	frame.pub_topic = component->get_profile()->get(profile::section::info, "publish").asString("undefined");
	frame.target = target;
}

} /* namespace message */
} /* namespace cossb */
