#pragma once

#include "event.h"

class QuequeManager : public CSingleton<QuequeManager>
{
public:
	QuequeManager();
	~QuequeManager();
	void Destroy();
	void InsertEvent(const char* eventname, LPEVENT event_ptr);
	void RemoveEvent(const char* eventname);
	LPEVENT CheckEventName(const char* eventname);

protected:
	std::map<std::string, LPEVENT> m_queque_;
};
