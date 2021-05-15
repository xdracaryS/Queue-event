#pragma once
#include "intrusive_ptr.h"

struct event_info_data
{
	event_info_data() {}
	virtual ~event_info_data() {}
};

typedef struct event EVENT;
typedef eastl::intrusive_ptr<EVENT> LPEVENT;
typedef long (*TEVENTFUNC) (LPEVENT event, long processing_time);

#define EVENTFUNC(name)	long (name) (LPEVENT event, long processing_time)
#define EVENTINFO(name) struct name : public event_info_data

struct TQueueElement;

struct event
{
	event() : func(NULL), info(NULL), q_el(NULL), ref_count(0) {}
	~event() {
		if (info != NULL)
		{
			delete info;
		}
	}
	TEVENTFUNC			func;
	event_info_data* info;
	TQueueElement* q_el;
	char				is_force_to_end;
	char				is_processing;
	size_t ref_count;
};

template<class T>
T* AllocEventInfo(){return new T;}

extern void		event_destroy();
extern int		event_process(int pulse);
extern int		event_count();
#define event_create(func, info, when) event_create_ex(func, info, when)
extern LPEVENT	event_create_ex(TEVENTFUNC func, event_info_data* info, long when);
extern void		event_cancel(LPEVENT* event);
extern long		event_processing_time(LPEVENT event);
extern long		event_time(LPEVENT event);
extern void		event_reset_time(LPEVENT event, long when);
extern void		intrusive_ptr_add_ref(EVENT* p);
extern void		intrusive_ptr_release(EVENT* p);

