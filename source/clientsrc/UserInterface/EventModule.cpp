#include "StdAfx.h"
#include "EventModule.h"

QuequeManager::QuequeManager() { Destroy(); }
QuequeManager::~QuequeManager() { Destroy(); }

void QuequeManager::Destroy()
{
	for (auto iter = m_queque_.begin(); m_queque_.end() != iter; ++iter)
		event_cancel(&iter->second);
	m_queque_.clear();
	for (auto iter = m_callBack_.begin(); m_callBack_.end() != iter; ++iter)
	{
		Py_XDECREF(iter->second.first);
		Py_XDECREF(iter->second.second);
	}
	m_callBack_.clear();
}

EVENTINFO(queque_event_info)
{
	PyObject* func;
	PyObject* args;
	long time_cycle;
	queque_event_info()
	{
		func = NULL;
		args = NULL;
		time_cycle = 0;
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
	if (PyCallable_Check(info->func))
	{
		PyObject* ptr = PyObject_CallObject(info->func, info->args);
		if (!info->time_cycle) {
			Py_XDECREF(ptr);
			return 0;
		}
		long returnBack = PyLong_AsLong(ptr);
		Py_XDECREF(ptr);
		return returnBack;
	}
	return 0;
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
		event_cancel(&event_ptr);//leak fix.
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

bool QuequeManager::CheckCallBackEventName(const char* eventname)
{
	auto iter = m_callBack_.find(eventname);
	if (iter == m_callBack_.end())
		return false;
	return true;
}

void QuequeManager::InsertCallBackEvent(const char* eventname, std::pair<PyObject*, PyObject*>& event_ptr)
{
	if (CheckCallBackEventName(eventname))
	{
		TraceError("<callback_event> %s this call back event already have.", eventname);
		Py_XDECREF(event_ptr.first);
		Py_XDECREF(event_ptr.second);
		return;
	}
	m_callBack_.emplace(eventname, event_ptr);
}

void QuequeManager::RemoveCallBackEvent(const char* eventname)
{
	auto iter = m_callBack_.find(eventname);
	if (iter == m_callBack_.end())
		return;
	Py_XDECREF(iter->second.first);
	Py_XDECREF(iter->second.second);
	m_callBack_.erase(iter);
}

void QuequeManager::CallBackEvent(const char* eventname)
{
	auto iter = m_callBack_.find(eventname);
	if (iter == m_callBack_.end())
		return;
	auto event_ptr = iter->second;
	if (PyCallable_Check(event_ptr.first))
	{
		PyObject* ptr = PyObject_CallObject(event_ptr.first, event_ptr.second);
		Py_XDECREF(ptr);
	}
}

PyObject* quequeCreateEvent(PyObject* poSelf, PyObject* poArgs)
{
	char* eventname;
	if (!PyTuple_GetString(poArgs, 0, &eventname))
		return Py_BuildException();

	PyObject* func = NULL;
	if (!PyTuple_GetObject(poArgs, 1, &func))
		return Py_BuildException();

	if (!PyCallable_Check(func)) {
		TraceError("<queue_event> <Factor> event func is not callable!");
		return Py_BuildNone();
	}

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


	if (PyTuple_GetObject(poArgs, 5, &func))
	{
		BYTE tuple_len = 0;
		for (BYTE j = 0; j < 10; ++j)
		{
			if (PyTuple_GetObject(poArgs, 4 + j, &func))
				tuple_len += 1;
		}

		if(tuple_len == 0)
			return Py_BuildNone();

		PyObject* poList = PyTuple_New(tuple_len);
		for (BYTE j = 0; j < tuple_len+1; ++j)
		{
			char* str;
			int number;
			float float_number;
			if (PyTuple_GetString(poArgs, 4 + j, &str))
				PyTuple_SET_ITEM(poList,j, PyString_FromString(str));
			else if (PyTuple_GetFloat(poArgs, 4 + j, &float_number))
				PyTuple_SET_ITEM(poList, j, PyFloat_FromDouble(float_number));
			else if (PyTuple_GetInteger(poArgs, 4 + j, &number))
				PyTuple_SET_ITEM(poList, j, PyInt_FromLong(number));
		}
		std::swap(info->args, poList);
	}

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

PyObject* quequeAddEventCallback(PyObject* poSelf, PyObject* poArgs)
{
	char* eventname;
	if (!PyTuple_GetString(poArgs, 0, &eventname))
		return Py_BuildException();

	PyObject* func = NULL;
	if (!PyTuple_GetObject(poArgs, 1, &func))
		return Py_BuildException();

	if (!PyCallable_Check(func)) {
		TraceError("<callback_event> <Factor> event func is not callable!");
		return Py_BuildNone();
	}

	std::pair<PyObject*, PyObject*> callback;
	std::swap(callback.first, func);
	
	if (PyTuple_GetObject(poArgs, 2, &func))
	{
		BYTE tuple_len = 0;
		for (BYTE j = 0; j < 10; ++j)
		{
			if (PyTuple_GetObject(poArgs, 2 + j, &func))
				tuple_len += 1;
		}

		if (tuple_len == 0) {
			return Py_BuildNone();
		}

		PyObject* poList = PyTuple_New(tuple_len);
		for (BYTE j = 0; j < tuple_len + 1; ++j)
		{
			char* str;
			int number;
			float float_number;
			if (PyTuple_GetString(poArgs, 2 + j, &str))
				PyTuple_SET_ITEM(poList, j, PyString_FromString(str));
			else if (PyTuple_GetFloat(poArgs, 2 + j, &float_number))
				PyTuple_SET_ITEM(poList, j, PyFloat_FromDouble(float_number));
			else if (PyTuple_GetInteger(poArgs, 2 + j, &number))
				PyTuple_SET_ITEM(poList, j, PyInt_FromLong(number));
		}
		std::swap(callback.second, poList);
	}

	QuequeManager::Instance().InsertCallBackEvent(eventname, callback);
	return Py_BuildNone();
}

PyObject* quequeRemoveEventCallBack(PyObject* poSelf, PyObject* poArgs)
{
	char* eventname;
	if (!PyTuple_GetString(poArgs, 0, &eventname))
		return Py_BuildException();
	QuequeManager::Instance().RemoveCallBackEvent(eventname);
	return Py_BuildNone();
}

PyObject* quequeInvoke(PyObject* poSelf, PyObject* poArgs)
{
	char* eventname;
	if (!PyTuple_GetString(poArgs, 0, &eventname))
		return Py_BuildException();
	QuequeManager::Instance().CallBackEvent(eventname);
	return Py_BuildNone();
}

void initQueque()
{
	static PyMethodDef s_methods[] =
	{
		{ "CreateEvent", quequeCreateEvent, METH_VARARGS },
		{ "CancelEvent", quequeCancelEvent, METH_VARARGS },
		{ "SetEventTime", quequeSetEventTime, METH_VARARGS },


		{ "Invoke", quequeInvoke, METH_VARARGS },
		{ "AddEventCallback", quequeAddEventCallback, METH_VARARGS },
		{ "RemoveEventCallBack", quequeRemoveEventCallBack, METH_VARARGS },
		{ NULL, NULL, NULL },
	};
	PyObject* poModule = Py_InitModule("queque", s_methods);
}
