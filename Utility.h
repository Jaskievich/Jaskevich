#ifndef UTILITY_H
#define UTILITY_H

#include <windows.h>

inline FILETIME GetFileTime(LONGLONG m_stamp)
{
    ULARGE_INTEGER  st;
    FILETIME ft;
    st.QuadPart = m_stamp;
    ft.dwLowDateTime  = st.LowPart;
    ft.dwHighDateTime = st.HighPart;
    return ft;
}

inline SYSTEMTIME _GetLocalTime(FILETIME ft)
{
    FILETIME lft;
    FileTimeToLocalFileTime(&ft, &lft);
    SYSTEMTIME st;
    FileTimeToSystemTime(&lft, &st);
    return st;
}



#endif // UTILITY_H
