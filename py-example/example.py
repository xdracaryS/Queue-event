
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


