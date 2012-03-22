/*
 *  resource_manager.h
 *  ancopool
 *
 *  Created by Matthias Klein on 25.10.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#ifdef __APPLE__
#include <CoreFoundation/CFBundle.h>
#endif

#include <string>

class Resource_manager {
public:
	static Resource_manager& get();
	bool initialize();
	std::string get_resource(const std::string& resource_name) const;
	std::string get_resources_directory() const;
	
private:
	Resource_manager();
	Resource_manager(const Resource_manager&);
	
	std::string resources_directory;
	
#ifdef __APPLE__
	CFBundleRef main_bundle;
#endif
};

#endif // RESOURCE_MANAGER_H