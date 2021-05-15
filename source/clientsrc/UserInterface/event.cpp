#include "StdAfx.h"
#include "event.h"
#include "event_queue.h"
#include "PythonApplication.h"

static CEventQueue cxx_q;

LPEVENT event_create_ex(TEVENTFUNC func, event_info_data* info, long when)
{
	LPEVENT new_event = NULL;

	if (when < 1)
		when = 1;

	new_event = new event;
	assert(NULL != new_event);

	new_event->func = func;
	new_event->info = info;
	new_event->q_el = cxx_q.Enqueue(new_event, when, time(0));
	new_event->is_processing = FALSE;
	new_event->is_force_to_end = FALSE;

	return (new_event);
}

void event_cancel(LPEVENT* ppevent)
{
	LPEVENT event;

	if (!ppevent)
	{
		TraceError("null pointer");
		return;
	}

	if (!(event = *ppevent))
		return;

	if (event->is_processing)
	{
		event->is_force_to_end = TRUE;

		if (event->q_el)
			event->q_el->bCancel = TRUE;

		*ppevent = NULL;
		return;
	}

	if (!event->q_el)
	{
		*ppevent = NULL;
		return;
	}

	if (event->q_el->bCancel)
	{
		*ppevent = NULL;
		return;
	}

	event->q_el->bCancel = TRUE;

	*ppevent = NULL;
}

void event_reset_time(LPEVENT event, long when)
{
	if (!event->is_processing)
	{
		if (event->q_el)
			event->q_el->bCancel = TRUE;

		event->q_el = cxx_q.Enqueue(event, when, time(0));
	}
}

int event_process(int pulse)
{
	long	new_time;
	int		num_events = 0;

	while (pulse >= cxx_q.GetTopKey())
	{
		TQueueElement* pElem = cxx_q.Dequeue();

		if(!pElem)
			continue;

		if (pElem->bCancel)
		{
			cxx_q.Delete(pElem);
			continue;
		}

		new_time = pElem->iKey;

		LPEVENT the_event = pElem->pvData;

		if(!the_event)
			continue;

		long processing_time = event_processing_time(the_event);
		cxx_q.Delete(pElem);
		the_event->is_processing = TRUE;
		if (!the_event->info)
		{
			the_event->q_el = NULL;
		}
		else
		{
			new_time = (the_event->func) (get_pointer(the_event), processing_time);
			if (new_time <= 0 || the_event->is_force_to_end)
				the_event->q_el = NULL;
			else
			{
				the_event->q_el = cxx_q.Enqueue(the_event, new_time, pulse);
				the_event->is_processing = FALSE;
			}
		}
		++num_events;
	}
	return num_events;
}

long event_processing_time(LPEVENT event)
{
	long start_time;

	if (!event->q_el)
		return 0;

	start_time = event->q_el->iStartTime;
	return (time(0) - start_time);
}

long event_time(LPEVENT event)
{
	long when;

	if (!event->q_el)
		return 0;

	when = event->q_el->iKey;
	return (when - time(0));
}

void event_destroy(void)
{
	TQueueElement* pElem;

	while ((pElem = cxx_q.Dequeue()))
	{
		LPEVENT the_event = (LPEVENT)pElem->pvData;

		if (!pElem->bCancel)
		{
		}

		cxx_q.Delete(pElem);
	}
}

int event_count()
{
	return cxx_q.Size();
}

void intrusive_ptr_add_ref(EVENT* p)
{
	++(p->ref_count);
}

void intrusive_ptr_release(EVENT* p)
{
	if (--(p->ref_count) == 0)
	{
		delete p;
	}
}


