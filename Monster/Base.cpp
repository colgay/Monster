#include "Base.h"

edict_t* EHANDLE::Get(void)
{
	if (m_pent)
	{
		if (m_pent->serialnumber == m_serialnumber)
			return m_pent;
		else
			return NULL;
	}
	return NULL;
};

edict_t* EHANDLE::Set(edict_t* pent)
{
	m_pent = pent;
	if (pent)
		m_serialnumber = m_pent->serialnumber;
	return pent;
};


EHANDLE :: operator edict_t* ()
{
	return Get();
};


edict_t* EHANDLE :: operator = (edict_t* pEntity)
{
	if (pEntity)
	{
		m_pent = pEntity;
		if (m_pent)
			m_serialnumber = m_pent->serialnumber;
	}
	else
	{
		m_pent = NULL;
		m_serialnumber = 0;
	}
	return pEntity;
}


edict_t* EHANDLE :: operator -> ()
{
	return Get();
}
