#pragma once

#include "cpch.h"
#include "cdefs.h"

class iEvent {
public:
	iEvent() {}
	virtual ~iEvent() {}
		
protected:
	bool handled = false;
};
