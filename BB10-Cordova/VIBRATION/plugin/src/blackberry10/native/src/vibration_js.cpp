/*
 * Copyright 2013-2014  Blackberry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>
#include "../public/tokenizer.h"
#include "vibration_js.hpp"
#include "vibration_ndk.hpp"

using namespace std;

/**
 * Default constructor.
 */
VibrationJS::VibrationJS(const std::string& id) :
		m_id(id) {
	m_pVibrationController = new webworks::VibrationNDK(this);
}

/**
 * VibrationJS destructor.
 */
VibrationJS::~VibrationJS() {
	if (m_pVibrationController)
		delete m_pVibrationController;
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList() {
	static char name[] = "VibrationJS";
	return name;
}

/**
 * This method is used by JNext to instantiate the VibrationJS object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id) {
	if (className == "VibrationJS") {
		return new VibrationJS(id);
	}

	return NULL;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool VibrationJS::CanDelete() {
	return true;
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string VibrationJS::InvokeMethod(const string& command) {

	size_t commandIndex = command.find_first_of(" ");
		std::string strCommand = command.substr(0, commandIndex);
		size_t callbackIndex = command.find_first_of(" ", commandIndex + 1);
		std::string callbackId = command.substr(commandIndex + 1, callbackIndex - commandIndex - 1);
		std::string arg = command.substr(callbackIndex + 1, command.length());
	// command appears with parameters following after a space


	if (strCommand == "vibration_request") {
		m_pVibrationController->vibrationRequest(callbackId, arg);
	}

	strCommand.append(";");
		strCommand.append(command);
		return strCommand;
}

// Notifies JavaScript of an event
void VibrationJS::NotifyEvent(const std::string& event) {
	std::string eventString = m_id + " ";
	eventString.append(event);
	SendPluginEvent(eventString.c_str(), m_pContext);
}
