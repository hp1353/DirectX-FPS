#include "StdAfx.h"
#include "cObject.h"


cObject::cObject(void)
	: m_ulRefCount(1)
{
	g_pObjectManager->AddObject(this);
}


cObject::~cObject(void)
{
	assert(m_ulRefCount == 0 && "ÀÌ·±Áþ ÇÏÁö¸»ÀÚ..ÂÍ!~");
	g_pObjectManager->RemoveObject(this);
}

void cObject::AddRef()
{
	++m_ulRefCount;
}

void cObject::Release()
{
	--m_ulRefCount;
	if(m_ulRefCount == 0)
		delete this;
}
