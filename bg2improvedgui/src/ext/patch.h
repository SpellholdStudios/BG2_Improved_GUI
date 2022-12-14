#ifndef PATCH_H
#define PATCH_H

#include "stdafx.h"

#include <vector>

#include "infgame.h"

extern CRuleTable** ClassAbilityTable;

typedef CRuleTable* ClAbTable[1281]; //for KitExtend

class Data {
public:
	Data();
	Data(const Data& d);
	Data(DWORD address, SIZE_T size, unsigned char bytes[]);
	Data(DWORD address, SIZE_T size, unsigned char bytes[], unsigned char src[]);
	~Data();

	Data& operator=(const Data& d);

	DWORD GetAddress();
	SIZE_T GetSize();
	unsigned char* GetBytes();
	unsigned char* GetSrc();

protected:
	DWORD dwAddress;
	SIZE_T nSize;
	unsigned char* szBytes;
	unsigned char* szSrc;
};

class Patch {
public:
	Patch();
	Patch(const Patch& p);
	Patch(std::vector<Data> vData);
	Patch(std::vector<Data> vData, const char* sz);
	~Patch();

	Patch& operator=(const Patch& p);

	char* GetName();
	std::vector<Data> GetData();
protected:
	char* szName;
	std::vector<Data> vData;
};

bool CheckPatch(Data& d);
void ApplyPatch(Data& d);
void InitUserPatches(std::vector<Patch>* pvPatchList, std::vector<Data>* pvDataList);
void InitPatches();

#endif //PATCH_H