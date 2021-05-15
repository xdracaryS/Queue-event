#include "StdAfx.h"

#include "event.h"
#include "event_queue.h"
#include <algorithm>

CEventQueue::CEventQueue(){}

CEventQueue::~CEventQueue()
{
	Destroy();
}

void CEventQueue::Destroy()
{
	while (!m_pq_queue.empty())
	{
		TQueueElement* pElem = m_pq_queue.top();
		m_pq_queue.pop();

		Delete(pElem);
	}
}

TQueueElement* CEventQueue::Enqueue(LPEVENT pvData, int duration, int pulse)
{
	TQueueElement* pElem = new TQueueElement;
	//pElem->pvData = pvData;
	std::swap(pElem->pvData, pvData);
	pElem->iStartTime = pulse;
	pElem->iKey = duration + pulse;
	pElem->bCancel = FALSE;
	m_pq_queue.push(pElem);
	return pElem;
}

TQueueElement* CEventQueue::Dequeue()
{
	if (m_pq_queue.empty())
		return NULL;

	TQueueElement* pElem = m_pq_queue.top();
	m_pq_queue.pop();
	return pElem;
}

void CEventQueue::Delete(TQueueElement* pElem)
{
	delete pElem;
}

int CEventQueue::GetTopKey()
{
	if (m_pq_queue.empty())
		return INT_MAX;

	return m_pq_queue.top()->iKey;
}

int CEventQueue::Size()
{
	return m_pq_queue.size();
}
