#include "StdAfx.h"
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

	void InsertCallBackEvent(const char* eventname, std::pair<PyObject*, PyObject*>& event_ptr);
	void RemoveCallBackEvent(const char* eventname);
	bool CheckCallBackEventName(const char* eventname);
	void CallBackEvent(const char* eventname);

protected:
	std::map<std::string, LPEVENT> m_queque_;
	std::map<std::string, std::pair<PyObject*, PyObject*>> m_callBack_;
};
