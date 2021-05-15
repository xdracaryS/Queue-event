# Queue-event

I modify event class for m2-client. Logic same like m2-gamesrc. You can use in c++ or in py. You can see how can use. See you later... 


# Examples:

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
## PY

```
import queque
import dbg

def TestFunctionNoLoop():
	dbg.TraceError("Hello from TestFunctionNoLoop")


def TestFunctionLoop():
	dbg.TraceError("Hello from TestFunctionLoop")
	return 5# next event time to 5 second.
	#return 0# if return 0 event removing.


queque.quequeCreateEvent(EVENTNAME = "no_loop_event", function = TestFunctionNoLoop, eventTime = 2, Loop = False) # paramater.


queque.quequeCreateEvent("no_loop_event",TestFunctionNoLoop,2,False)
queque.quequeCreateEvent("loop_event",TestFunctionLoop,2,True)


queque.CancelEvent("loop_event")# remove event..
queque.CancelEvent(EVENTNAME = "loop_event") # paramater.




queque.SetEventTime(EVENTNAME = "no_loop_event", TIME = 10) # paramater.
queque.SetEventTime("no_loop_event", 10)# set event time..

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


