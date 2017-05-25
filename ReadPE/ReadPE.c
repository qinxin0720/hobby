#include <Windows.h>
#include <stdio.h>


int ShowFunctionName(LPCWSTR fileName);

int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		printf("\te.g. ReadPE c:\\zlib.dll\n\te.g. ReadPE c:\\zlib.dll d:\\ssleay32.dll");
	}
	else
	{
		for (int i=1; i<argc; i++)
		{
			printf ("This is the export function of %s\n", argv[i]);
			ShowFunctionName((LPCSTR)argv[i]);
		}
	}
	return 0;
}

int ShowFunctionName(LPCSTR fileName)
{
	int Num = 0;
	int i;

	DWORD *k, *l;

	char * functionName = NULL;
	char * functionAddress = NULL;

	PIMAGE_EXPORT_DIRECTORY pExport;
	HANDLE hfile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		printf("Warning, Create File Failed!\n");
		return -1;
	}
	HANDLE hFileMapping = CreateFileMapping(hfile, NULL, PAGE_READONLY, 0, 0, NULL);
	if ((hFileMapping == NULL) || (hFileMapping == INVALID_HANDLE_VALUE))
	{
		printf("Warning, Could not create file mapping object!\n");
		return -2;
	}
	LPBYTE lpBaseAddress = (LPBYTE)MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
	if (lpBaseAddress == NULL)
	{
		printf("Warning, Could not map view of file!\n");
		return -3;
	}
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(lpBaseAddress + pDosHeader->e_lfanew);
	pExport = (PIMAGE_EXPORT_DIRECTORY)(lpBaseAddress + pNtHeader->OptionalHeader.DataDirectory[0].VirtualAddress);
	PIMAGE_SECTION_HEADER pSecheader = (PIMAGE_SECTION_HEADER)(lpBaseAddress + pDosHeader->e_lfanew + (size_t)(sizeof(*pNtHeader)));

	int dwSectionCount = pNtHeader->FileHeader.NumberOfSections;
	if (pNtHeader->OptionalHeader.DataDirectory[0].VirtualAddress <= pNtHeader->OptionalHeader.SizeOfHeaders)
	{
		;
	}
	else
	{
		for (i = 0; i<dwSectionCount; i++)
		{
			if (((pNtHeader->OptionalHeader.DataDirectory[0].VirtualAddress >= pSecheader[i].VirtualAddress) &&
				(pNtHeader->OptionalHeader.DataDirectory[0].VirtualAddress < pSecheader[i].VirtualAddress + pSecheader[i].Misc.VirtualSize)))
			{
				pExport = (PIMAGE_EXPORT_DIRECTORY)(lpBaseAddress + pNtHeader->OptionalHeader.DataDirectory[0].VirtualAddress -
					pSecheader[i].VirtualAddress + pSecheader[i].PointerToRawData);
				break;
			}
		}
	}

	Num = pExport->NumberOfNames;
	printf("\tTotal number of functions is %d\n\n", Num);


	char address[32];
	for (int j = 0; j<Num; j++)
	{
		k = (DWORD *)(pExport->AddressOfNames + lpBaseAddress + 4 * j - pSecheader[i].VirtualAddress + pSecheader[i].PointerToRawData);
		functionName = (char *)(*k + lpBaseAddress - pSecheader[i].VirtualAddress + pSecheader[i].PointerToRawData);

		l = (DWORD *)(pExport->AddressOfFunctions + lpBaseAddress - pSecheader[i].VirtualAddress + pSecheader[i].PointerToRawData + 4 * j);
		functionAddress = (char *)*l;


		if (functionAddress == NULL)
		{
			printf("\t%s", "Export by serial number!");
		}
		else
		{
			sprintf_s (address, sizeof (address), "0x%p", functionAddress);
			printf("\t%s", address);
		}

		printf("\t%s\n", functionName);
	}
	UnmapViewOfFile(lpBaseAddress);
	CloseHandle(hFileMapping);
	CloseHandle(hfile);
	return 0;
}

