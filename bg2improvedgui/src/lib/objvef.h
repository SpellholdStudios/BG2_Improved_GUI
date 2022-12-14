#ifndef OBJVEF_H
#define OBJVEF_H

#include "stdafx.h"
#include "rescore.h"
#include "scrcore.h"
#include "vidcore.h"
#include "sndcore.h"
#include "pathfind.h"
#include "objcore.h"

typedef IECPtrList CVefComponentList; //AAD00C

class CVisualEffectVidCell : public CGameObject { //Size 3A0h
//Constructor: 0x64C5FE
public:
	void AddToArea(CArea& area, POINT& pt, int zPos, char cVertListType, int nStartDuration);
	
	//AACED0

	VvcFileData vvc; //42h
	CVidCell* anim1; //226h, related to nameAnim1
	CVidCell* anim2; //22ah, related to nameAnim2 (N.B. anim1 loaded here in constructor instead)
	CVidPalette u22e; //size 0x24
	CVidBitmap bitmap; //252h, object of nameBitmap
	BOOL m_bDurationElapsed; //308h
	int m_nOrientation; //30ch
	TerrainTable u310; //050505ff, 05050505, 05ff0505, 0505ff05
	CSound u320; //320h, size 0x6a, for currentSnd
	CVidCell* anim3; //38ah, related to nameAnim3
	char m_nPurgeType; //38eh, 1 = name as ResRef in CMessage, 2 = nameanim1 as ResRef in CMessage
	char u38f; //played middle sound? no sound?
	int u390;

	//these three actually unused?
	int u394; //gets VvcArray.0xa (dw)
	int u398; //gets VvcArray.0xe (dw), copied to CVisualEffect 2a2h
	int u39c; //gets VvcArray.0x8 (w), compared with pGame->4dc4
};

extern void (CVisualEffectVidCell::*CVisualEffectVidCell_AddToArea)(CArea&, POINT&, int, char, int);

struct ResVvcContainer { //Size 10h
	ResVvcContainer(ResRef r);
	ResVvcContainer& Construct(ResRef r) { return *this; } //dummy

	~ResVvcContainer();

	CVisualEffectVidCell& Unmarshal();

	BOOL m_bLoaded;
	ResVid* m_pRes;
	ResRef m_rName;
};

extern ResVvcContainer& (ResVvcContainer::*ResVvcContainer_Construct_1)(ResRef);
extern CVisualEffectVidCell& (ResVvcContainer::*ResVvcContainer_Unmarshal)();

class CVisualEffect : public CGameObject { //Size 2A6h
public:
	void* operator new(size_t size);
	void operator delete(void* mem);

	CVisualEffect(); //65467D
	CVisualEffect& Construct() { return *this; } //dummy

	void AddToArea(CArea& area, POINT& pt, int zPos, char cVertListType);

	//AACFA8

	//struct _u42 { //Size 1C0h
	int nTimedEventOffset;      //42h
	int nTimedEventCount;       //46h
	int nTriggeredEventOffset;  //4ah
	int nTriggeredEventCount;   //4eh
	int nTargetType;        //52h, has owner (or 6e has flags?)
	int m_nSpeed;           //56h, Arg8 (zPos)
	int m_nDuration;        //5ah, purge if m_nAIUpdates > this, unless duration <= 0
	POINT ptStartDeltaV;    //5eh
	long nDeltaVDec;        //66h, zPos?
	long nHeight;           //6ah

	//bit0 = do not purge if no more vid cells/vef components
	//bit1 = update zPos
	//bit2 = attached to an owner (cre or projectileBAM)
    //bit3 = if target type is Self          (from parent effect) *TobEx v29 NEW*
    //bit4 = if target type is Preset Target (from parent effect) *TobEx v29 NEW*
	DWORD m_dwFlags;        //6eh

	int u72;
	int u76;
	int u7a;
	short u7e;
	char u80;
	char u81;
	int u82;
	int u86;
	int u8a;
	int u8e;
	short u92;
	int u94;
	int u98;
	int u9c;
	long long ua0;
	short ua8;
	int uaa[0x56];

	CEnumList m_VidCells; //202h, contains CVisualEffectVidCell enums
	CVefComponentList m_VefComponents; //21eh, VefFileComponent1 and VefFileComponent2
	CVefComponentList m_VefComponents2; //23ah, unused
	POINT ptSource; //256h, directly from constructor
	Enum eOwner; //25eh
	POINT ptPixels; //262h, set when AddToArea(), adds 27eh, y is 4/3 normal (in pixels, not x.y pos, 1024 pixels per pos)
	POINT ptPrev; //26ah (copied from 6h and ah during AIUpdate)
	int m_nAIUpdates; //272h
	int u276;
	int orientation; //27ah, range: 0x0-0x10
	POINT ptToAdd; //27eh, how much to move in next tick, y is 4/3 normal (in pixel, not x.y pos)
	int u286[4]; //set of constants
		
	//this is used to DisplayAllBams()
	//when one Vvc in VefVidCellList terminates due to end of duration, a new one is created based on this
	CKeyTableEntryList* u296;
	POSITION* u29a; //gets the m_pNodeHead of u296
	BOOL u29e; //bHasPosition (1 if u29a has a position)

	int u2a2;
};

extern CVisualEffect& (CVisualEffect::*CVisualEffect_Construct)();
extern void (CVisualEffect::*CVisualEffect_AddToArea)(CArea&, POINT&, int, char);

#endif //OBJVEF_H