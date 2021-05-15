# Queue-event

I modify event class for m2-client. Logic same like m2-gamesrc. You can use in c++ or in py. You can see how can use. See you later... 


# Examples:

## PY

```
import queque
import dbg


@ <CREATE EVENT PARAMETER>
#queque.quequeCreateEvent(event_name, function, when, loop, ...)

@ Without arg examples.

def TestFunctionNoLoop():
    dbg.TraceError("Hello from TestFunctionNoLoop")

def TestFunctionLoop():
    dbg.TraceError("Hello from TestFunctionLoop")
    return 5 # next event time to 5 second, if value is == 0 event will be remove. !IMPORTANT!

queque.quequeCreateEvent("not_loop_event",TestFunctionNoLoop,2,False)
queque.quequeCreateEvent("loop_event",TestFunctionLoop, 2, True)

------------------------

@ With arg examples.

def TestFunctionNoLoop(str):
    dbg.TraceError("Hello from TestFunctionNoLoop %s"%str)

def TestFunctionLoop(str, number, float_number):
    dbg.TraceError("Hello from TestFunctionLoop")
    return 5 # next event time to 5 second, if value is == 0 event will be remove. !IMPORTANT!

queque.quequeCreateEvent("not_loop_event",TestFunctionNoLoop,2,False, "TEST TEXT")
queque.quequeCreateEvent("loop_event",TestFunctionLoop, 2, True, "Hello World", 13, 13.34)


---------------------------------------------------------------------------------------------------


@ <REMOVE EVENT PARAMETER>
#queque.CancelEvent(event_name)

queque.CancelEvent("loop_event")

---------------------------------------------------------------------------------------------------

@ <SET EVENT TIME PARAMETER>
#queque.SetEventTime(event_name, when)

queque.SetEventTime("loop_event", 4)


---------------------------------------------------------------------------------------------------

# Callback python function from c++


New idea from @_HelloBrightness in m2dowloands.

@ Without Arg Example

def ClearData():
    pass

queque.AddEventCallback("OnClearData",ClearData)

@ With Arg Example

def ClearData(isRealClear):
    pass

queque.AddEventCallback("OnClearData",ClearData, True)

# We insert function(args) and name key. And we can callback any file like this.

queque.Invoke("OnClearData")

# You can remove callback function like this.

queque.RemoveEventCallBack("OnClearData")

```


## C++


```
EVENTINFO(event_info){
	int time_cycle;
};

EVENTFUNC(queque_event)
{
	event_info* info = dynamic_cast<event_info*>(event->info);
	if (info == NULL)
	{
		TraceError("<queque_event> <Factor> Null pointer");
		return 0;
	}
	CPythonNetworkStream::Instance().SendChatPacket("HELLO",1);
	return info->time_cycle;
}

int queque()
{
	bool loop = False;
	event_info* info = AllocEventInfo<event_info>();
	info->time_cycle = loop ? 10 : 0;
	LPEVENT event = event_create(queque_event, info, 10);

	event_reset_time(event, 20) // event time 10sec to 20sec.

	long event_time_before_start = event_time(event);
	long event_time_after_start = event_processing_time(event);
	long event_count = event_count()

	event_cancel(&event);//stop and remove..
}
```


# Event functions..

```
extern void		event_destroy();
extern int		event_process(int pulse);
extern int		event_count();
extern LPEVENT	event_create_ex(TEVENTFUNC func, event_info_data* info, long when);
extern void		event_cancel(LPEVENT* event);
extern long		event_processing_time(LPEVENT event);
extern long		event_time(LPEVENT event);
extern void		event_reset_time(LPEVENT event, long when);
extern void		intrusive_ptr_add_ref(EVENT* p);
extern void		intrusive_ptr_release(EVENT* p);
```

# VIDEO

[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/p2jmiIpt3lo/0.jpg)](https://www.youtube.com/watch?v=fUFaGf623dw)

[VIDEO LINK](https://www.youtube.com/watch?v=fUFaGf623dw)

# discord: dracaryS#9089
# skype: kuun_12


