/*
 *  resource_manager.cpp
 *  ancopool
 *
 *  Created by Matthias Klein on 25.10.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 *  Mac code from http://experimentgarden.blogspot.com/2009/06/how-to-load-resource-from-your.html
 */

#include "resource_manager.h"

Resource_manager::Resource_manager() {
}

Resource_manager::Resource_manager(const Resource_manager&) {
}

Resource_manager& Resource_manager::get() {
	static Resource_manager instance;
	return instance;
}

std::string Resource_manager::get_resources_directory() const {
	return resources_directory;
}

#ifdef __APPLE__
/* Mac OS X specific code */

bool Resource_manager::initialize() {
	main_bundle = CFBundleGetMainBundle();
	
	if (!main_bundle)
		return false;

	char resources_directory_cstr[200];
	
	CFURLRef url = CFBundleCopyResourcesDirectoryURL(main_bundle);
	if (!CFURLGetFileSystemRepresentation(url, true, (Uint8*) resources_directory_cstr, 200)) {
		return false;
	}
	
	resources_directory = std::string(resources_directory_cstr);

	return true;
}

std::string Resource_manager::get_resource(const std::string& resource_name) const {
	CFURLRef resource_url = CFBundleCopyResourceURL(main_bundle, CFStringCreateWithCString(NULL, resource_name.c_str(), kCFStringEncodingASCII), NULL, NULL);
	
	if (!resource_url) {
		return std::string("");
	}
	
	char file_url[200];
	
	if (!CFURLGetFileSystemRepresentation(resource_url, true, (UInt8*) file_url, 200)) {
		return std::string("");
	}
	
	return std::string(file_url);
}

#else
/* Other platforms. */

bool Resource_manager::initialize() {
	resources_directory = std::string("./resources");
	
	return true;
}

std::string Resource_manager::get_resource(const std::string& resource_name) const {
	return resource_name;
}

#endif
