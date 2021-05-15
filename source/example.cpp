

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

