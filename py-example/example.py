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
	dbg.TraceError("Hello from TestFunctionLoop %s %d %.5f"%(str, number, float_number))
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