#pragma once

#include "Libs.h"

#include "Reader.hpp"

class AccessControlSystem
{
	bool is_running_;
	Reader reader_;

	set<string> access_granted_ids_ = {
		"69B90152",     // NFC tag
		"A001"          // HCE Service of an Android device
	};

	bool IsIdValid(const string& id) const;

public:
	AccessControlSystem();

	bool IsRunning() const;
	void MainLoop();
};