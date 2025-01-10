// KillFileOwner.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <Windows.h>
#include <RestartManager.h>
#include <stdio.h>

#pragma comment(lib,"Rstrtmgr.lib")

BOOL GetFileOwner(LPCWSTR pathName) {
    WCHAR sessionKey[CCH_RM_SESSION_KEY + 1] = { 0 };
    DWORD hSession = 0;
    if (RmStartSession(&hSession, 0, sessionKey) == ERROR_SUCCESS) {
        if (RmRegisterResources(hSession, 1, &pathName, 0, nullptr, 0, nullptr) == ERROR_SUCCESS) {
            UINT nProcInfoNeeded = 0;
            UINT nProcInfo = 0;
            DWORD reason = 0;

            do
            {
                DWORD ret = RmGetList(hSession, &nProcInfoNeeded, &nProcInfo, nullptr, &reason);

                if (ret != ERROR_MORE_DATA || !nProcInfoNeeded) {
                    RmEndSession(hSession);
                    return FALSE;
                }

                PRM_PROCESS_INFO pRPI = (PRM_PROCESS_INFO)malloc(sizeof(RM_PROCESS_INFO) * nProcInfoNeeded);
                if (!pRPI) {
                    RmEndSession(hSession);
                    return FALSE;
                }

                nProcInfo = nProcInfoNeeded;
                ret = RmGetList(hSession, &nProcInfoNeeded, &nProcInfo, pRPI, &reason);
                if (ret == ERROR_MORE_DATA) {
                    continue;
                }
                if (ret != ERROR_SUCCESS || !nProcInfoNeeded) {
                    free(pRPI);
                    RmEndSession(hSession);
                }

                for (int i = 0; i < nProcInfo; i++) {
                    printf("process id: %d, process name: %ws\n", pRPI[i].Process.dwProcessId, pRPI[i].strAppName);
                }
                RmShutdown(hSession, RmForceShutdown, nullptr);
            } while (FALSE);
            
            
        }

        RmEndSession(hSession);
        return TRUE;
    }
    DWORD error = ::GetLastError();
    printf("error: %d\n", error);
    return FALSE;
}

int main(){
    WCHAR file[] = L"D:\\Dev\\cpp\\KillFileOwner\\Debug\\hello.txt";
    GetFileOwner(file);
    return 0;
}

