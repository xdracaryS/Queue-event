#include "StdAfx.h"
#include "EventModule.h"

QuequeManager::QuequeManager() { Destroy(); }
QuequeManager::~QuequeManager() { Destroy(); }

EVENTINFO(queque_event_info)
{
	PyObject* func;
	//char arg_types[10];
	PyObject* args;
	long time_cycle;
	queque_event_info()
	{
		func = NULL;
		args = NULL;
		time_cycle = 0;
		//memset(&arg_types, 0, sizeof(arg_types));
	}
	~queque_event_info()
	{
		Py_XDECREF(func);
		Py_XDECREF(args);
		func = NULL;
		args = NULL;
	}
};

EVENTFUNC(queque_event)
{
	queque_event_info* info = dynamic_cast<queque_event_info*>(event->info);
	if (info == NULL)
	{
		TraceError("<queque_event> <Factor> Null pointer");
		return 0;
	}	
	//PyObject* ptr = PyObject_CallFunctionObjArgs(info->func, info->args);
	//PyObject* ptr = PyObject_CallNoArgs(info->func);
	PyObject* ptr = PyObject_CallObject(info->func, info->args);
	//PyObject* ptr = PyObject_CallFunction(info->func, "");
	if (!info->time_cycle)
		return 0;
	long returnBack = PyLong_AsLong(ptr);
	return returnBack;
}

void QuequeManager::Destroy()
{
	for (auto iter = m_queque_.begin(); m_queque_.end() != iter; ++iter)
		event_cancel(&iter->second);
	m_queque_.clear();
}

LPEVENT QuequeManager::CheckEventName(const char* eventname)
{
	auto iter = m_queque_.find(eventname);
	if (iter == m_queque_.end())
		return NULL;
	return iter->second;
}

void QuequeManager::InsertEvent(const char* eventname, LPEVENT event_ptr)
{
	if (CheckEventName(eventname))
	{
		TraceError("<queque_event> %s this event already have.", eventname);
		return;
	}
	m_queque_.emplace(eventname, event_ptr);
}

void QuequeManager::RemoveEvent(const char* eventname)
{
	auto iter = m_queque_.find(eventname);
	if (iter == m_queque_.end())
		return;
	LPEVENT event_ptr = iter->second;
	m_queque_.erase(iter);
	event_cancel(&event_ptr);
	event_ptr = NULL;
}

PyObject* quequeCreateEvent(PyObject* poSelf, PyObject* poArgs)
{
	char* eventname;
	if (!PyTuple_GetString(poArgs, 0, &eventname))
		return Py_BuildException();

	PyObject* func = NULL;
	if (!PyTuple_GetObject(poArgs, 1, &func))
		return Py_BuildException();

	int when;
	if (!PyTuple_GetInteger(poArgs, 2, &when))
		return Py_BuildException();

	if (when <= 0)
		when = 1;

	bool loop;
	if (!PyTuple_GetBoolean(poArgs, 3, &loop))
		return Py_BuildException();

	queque_event_info* info = AllocEventInfo<queque_event_info>();
	std::swap(info->func, func);
	info->time_cycle = loop ? when : 0;

	//char* arg_types;
	//if (PyTuple_GetString(poArgs, 4, &arg_types))
	//{
	//	strcpy(info->arg_types, arg_types);
	//	//PyObject* poList = PyList_New(0);
	//	PyObject* poList = PyTuple_New(5);
	//	for (BYTE j = 0; j < 5; ++j)
	//	{
	//		PyObject* arg;
	//		if (PyTuple_GetObject(poArgs, 5 + j, &arg))
	//			//PyList_Append(poList, arg);
	//			PyTuple_SET_ITEM(poList,j,arg);
	//		else
	//		{
	//			if (j == 0)
	//				return Py_BuildNone();
	//		}
	//	}
	//	//info->args = Py_BuildValue(info->arg_types, poList);
	//	std::swap(info->args, poList);
	//}

	LPEVENT event_ptr = event_create(queque_event, info, when);
	QuequeManager::Instance().InsertEvent(eventname, event_ptr);
	return Py_BuildNone();
}

PyObject* quequeCancelEvent(PyObject* poSelf, PyObject* poArgs)
{
	char* eventname;
	if (!PyTuple_GetString(poArgs, 0, &eventname))
		return Py_BuildException();
	QuequeManager::Instance().RemoveEvent(eventname);
	return Py_BuildNone();
}

PyObject* quequeSetEventTime(PyObject* poSelf, PyObject* poArgs)
{
	char* eventname;
	if (!PyTuple_GetString(poArgs, 0, &eventname))
		return Py_BuildException();
	int eventtime;
	if (!PyTuple_GetInteger(poArgs, 1, &eventtime))
		return Py_BuildException();
	LPEVENT event_ptr = QuequeManager::Instance().CheckEventName(eventname);
	if (event_ptr != NULL)
		event_reset_time(event_ptr, eventtime);
	return Py_BuildNone();
}

void initQueque()
{
	static PyMethodDef s_methods[] =
	{
		{ "CreateEvent", quequeCreateEvent, METH_VARARGS },
		{ "CancelEvent", quequeCancelEvent, METH_VARARGS },
		{ "SetEventTime", quequeSetEventTime, METH_VARARGS },
		{ NULL, NULL, NULL },
	};
	PyObject* poModule = Py_InitModule("queque", s_methods);
}
