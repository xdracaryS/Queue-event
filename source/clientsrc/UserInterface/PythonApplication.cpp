
# Add

#ifdef ENABLE_EVENT
#include "event.h"
#endif


# Search

	OnUIUpdate();

# Add after

#ifdef ENABLE_EVENT
	event_process(time(0));
#endif


# Search

	WebBrowser_Destroy();

# Add after

#ifdef ENABLE_EVENT
	m_pyQueque.Destroy();
#endif