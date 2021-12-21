#define WINVER 0x0502
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>



#define INFO_BUFFER_SIZE 32767
#define BUFSIZE MAX_PATH
#define MAX_KEY_LENGTH 255
using namespace std;

int main()
{
SECURITY_ATTRIBUTES attributes;
attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
attributes.bInheritHandle = TRUE;
attributes.lpSecurityDescriptor = NULL;
HANDLE input_pipe_read_end;
HANDLE input_pipe_write_end;
HANDLE output_pipe_read_end;
HANDLE output_pipe_write_end;
SetHandleInformation(input_pipe_write_end, HANDLE_FLAG_INHERIT, 0);
SetHandleInformation(output_pipe_write_end, HANDLE_FLAG_INHERIT, 0);
CreatePipe(&input_pipe_read_end, &input_pipe_write_end, &attributes, 0);
CreatePipe(&output_pipe_read_end, &output_pipe_write_end, &attributes, 0);
STARTUPINFO startup_info;
ZeroMemory(&startup_info, sizeof(startup_info));
startup_info.cb = sizeof(startup_info);
startup_info.hStdInput = input_pipe_read_end;
startup_info.hStdOutput = output_pipe_write_end;
startup_info.hStdError = output_pipe_write_end;
startup_info.dwFlags |= STARTF_USESTDHANDLES;
PROCESS_INFORMATION pi;
BOOL result = CreateProcess(NULL,"cmd.exe",NULL,NULL,TRUE,0,NULL,NULL,&startup_info,&pi);
DWORD bytes_read;
BOOL mainflag = true;
do{
BOOL flag = true;
do {
    char tmp[1];
    ReadFile(output_pipe_read_end, tmp, sizeof(tmp), &bytes_read, NULL);
    fwrite(tmp, bytes_read, 1, stdout);
        if (tmp[0] == '>') {
        flag=false;
        }
    } while (flag==true);
const char PLEASE[] = "please";

char* input = NULL;
char tmp[256];
while (!input) {
    fgets(tmp, sizeof(tmp), stdin);
    if (!strncmp(tmp, "thanks", 6)) {
        mainflag=false;
        UINT lpExitCode ;
        GetExitCodeProcess(&startup_info,LPDWORD(lpExitCode));
        TerminateProcess(&startup_info,lpExitCode);
        CloseHandle(input_pipe_read_end);
        CloseHandle(input_pipe_write_end);
        CloseHandle(output_pipe_read_end);
        CloseHandle(output_pipe_write_end);
        break;
    }
    else if (strncmp(tmp, "please", 6)) {
        fprintf(stderr, "Please ask politely!\n> ");
    }
    else {
        input = tmp + sizeof(PLEASE);
        WriteFile(input_pipe_write_end,input,strlen(input),NULL,NULL);
    }
}
}while (mainflag==true);
return 0;
}
