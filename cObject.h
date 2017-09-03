#pragma once
class cObject
{
protected:
	ULONG m_ulRefCount;

public:
	cObject(void);
	virtual ~cObject(void);

	virtual void AddRef();
	virtual void Release();
};

