#include "framework.h"
#include "CCsAutoLock.h"

//////////////////////////////////////////////////////////////////////////////
// CMyCriticalSection

/// @brief Constructor
/// @param
/// @return
/// @note
CMyCriticalSection::CMyCriticalSection(void)
{
    InitializeCriticalSection(&m_CS);
}

/// @brief Destructor
/// @param
/// @return
/// @note
CMyCriticalSection::~CMyCriticalSection(void)
{
    DeleteCriticalSection(&m_CS);
}

/// @brief Lock the Critical Section
/// @param
/// @return
/// @note
inline void CMyCriticalSection::Lock(void)
{
    EnterCriticalSection(&m_CS);
}

/// @brief Unlock the critical section
/// @param
/// @return
/// @note
inline void CMyCriticalSection::Unlock(void)
{
    LeaveCriticalSection(&m_CS);
}

/// @brief Try to enter critical section
/// @param
/// @return
/// @note
inline BOOL CMyCriticalSection::Try(void)
{
    return TryEnterCriticalSection(&m_CS);
}

//////////////////////////////////////////////////////////////////////////////
// CCsAutoLock

/// @brief Constructor
/// @param [in]  CS       CMyCriticalSection to enter
/// @param [in]  bForce   TRUE  : the entry is forced
/// @param                FALSE : it will try to enter
/// @param                The state can be inquired with the \c CCsAutoLock::IsLocked function.
/// @return
/// @note
CCsAutoLock::CCsAutoLock(const CMyCriticalSection &CS, BOOL bForce)
    : m_CS(*const_cast<CMyCriticalSection *>(&CS))
    , m_bLocked(FALSE)
{
    if (bForce == TRUE) {
        m_CS.Lock();
        m_bLocked = TRUE;
    }
    else {
        m_bLocked = m_CS.Try();
    }
}

/// @brief Constructor
/// @param [in]  CS       CMyCriticalSection to enter
/// @param [in]  bForce   TRUE  : the entry is forced
/// @param                FALSE : it will try to enter
/// @param                The state can be inquired with the \c CCsAutoLock::IsLocked function.
/// @return
/// @note
CCsAutoLock::CCsAutoLock(const CMyCriticalSection *pCS, BOOL bForce)
    : m_CS(*const_cast<CMyCriticalSection *>(pCS))
    , m_bLocked(FALSE)
{
    if (bForce == TRUE) {
        m_CS.Lock();
        m_bLocked = TRUE;
    }
    else {
        m_bLocked = m_CS.Try();
    }
}

/// @brief Destructor
/// @param
/// @return
/// @note
CCsAutoLock::~CCsAutoLock()
{
    if (m_bLocked == TRUE) {
        m_CS.Unlock();
    }
}

/// @brief Auto Lock
/// @param
/// @return
/// @note
void CCsAutoLock::AutoLock(void)
{
    if (m_bLocked == FALSE) {
        m_CS.Lock();
        m_bLocked = TRUE;
    }
}

/// @brief Lock release
/// @param
/// @return
/// @note
void CCsAutoLock::Release(void)
{
    if (m_bLocked == TRUE) {
        m_CS.Unlock();
        m_bLocked = FALSE;
    }
}

/// @brief Inquire if the CS can be locked by the calling thread or not
/// @param
/// @return TRUE  : the CS is locked by the calling thread or if it would be possible to enter.
/// @return FALSE : the CS is locked by  another thread.
/// @note
inline BOOL CCsAutoLock::IsLockable(void)
{
    BOOL bIsLockable;

    bIsLockable = m_CS.Try();

    if (bIsLockable == TRUE) {
        m_CS.Unlock();
        m_bLocked = FALSE;
    }

    return bIsLockable;
}

/// @brief Inquire if this Autlock is locked by any thread
/// @param
/// @return TRUE  : this Autolock is locked by any thread.
/// @return FALSE : this Autolock is free.
/// @note
inline BOOL CCsAutoLock::IsLocked(void)
{
    return m_bLocked;
}
