#pragma once

//////////////////////////////////////////////////////////////////////////////
// CMyCriticalSection

class CMyCriticalSection
{
public:
	CMyCriticalSection(void);
	virtual ~CMyCriticalSection(void);

	virtual void Lock(void);
	virtual void Unlock(void);
	virtual BOOL Try(void);

private:
	CRITICAL_SECTION m_CS;
};

//////////////////////////////////////////////////////////////////////////////
// CCsAutoLock

class CCsAutoLock
{
public:
	CCsAutoLock(const CMyCriticalSection &CS, BOOL bForce = TRUE);
	CCsAutoLock(const CMyCriticalSection *pCS, BOOL bForce = TRUE);
	virtual ~CCsAutoLock(void);

	void AutoLock(void);
	void Release(void);

	BOOL IsLocked(void);
	BOOL IsLockable(void);

private:
	void operator = (CCsAutoLock &);

private:
	CMyCriticalSection &m_CS;		// Referenz to the CS to be used
	BOOL               m_bLocked;	// Flag if this instance of the AutoLock class locked the CS
};
