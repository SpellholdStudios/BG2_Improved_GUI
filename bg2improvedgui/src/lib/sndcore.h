#ifndef SNDCORE_H
#define SNDCORE_H

#include "stdafx.h"
#include "rescore.h"

//Sound channels
#define CSOUNDCHANNEL_NARRATION		0
#define CSOUNDCHANNEL_AREAAMBIENT	1
#define CSOUNDCHANNEL_ACTIONS		2
#define CSOUNDCHANNEL_SWINGS		3
#define CSOUNDCHANNEL_CASTING		4
#define CSOUNDCHANNEL_GUI			5
#define CSOUNDCHANNEL_DIALOG		6
#define CSOUNDCHANNEL_PARTYSLOT0	7
#define CSOUNDCHANNEL_PARTYSLOT1	8
#define CSOUNDCHANNEL_PARTYSLOT2	9
#define CSOUNDCHANNEL_PARTYSLOT3	10
#define CSOUNDCHANNEL_PARTYSLOT4	11
#define CSOUNDCHANNEL_PARTYSLOT5	12
#define CSOUNDCHANNEL_MONSTER		13
#define CSOUNDCHANNEL_HITS			14
#define CSOUNDCHANNEL_MISSILE		15
#define CSOUNDCHANNEL_AMBIENTL		16
#define CSOUNDCHANNEL_AMBIENTN		17
#define CSOUNDCHANNEL_WALKINGC		18
#define CSOUNDCHANNEL_WALKINGM		19
#define CSOUNDCHANNEL_ARMOR			20

#define CSOUNDMIXER_MAX_SONGS		100

typedef IECPtrList CSongPosList; //AB9984

class CArea;

class CSoundChannel { //32h
//Constructor: 0x9DF8AC
public:
	//AB9978
	int* vtable;
	
	//0: all SFX, no additional behaviour
	//1: nil, use CVoice sounds list, always highest priority, always highest channel
	//2: nil, plays one sound at a time, no interrupt
	//3: all speech, interruptible
	int type; //4h

	int nVolumeCurrent; //8h (0-100)
	int nVolumeInternal; //ch, SNDCHANN.2DA (0-100)
	int nCutsceneVolumeReductionPercent; //10h
	CObList Sounds; //14h, contains CSounds
	bool bCutsceneMode; //30h, hard-coded volume reduction at 25%
	char u31; //pad
};

struct SongPosition { //Size Ch
	int nSong;
	int nPart;
	int nCursorPos;
};

struct ResWavContainer { //Size 10h
	BOOL bLoaded; //0h
	ResWav* pResWav; //4h
	ResRef soundName; //8h
};

class CSound : CObject { //Size 6Ah
//Constructor: 0x9DCF80
public:
	//AB9960
    CSound();
    virtual ~CSound();
    int SetFireForget(BOOL bNewFireForget);
    int SetChannel(int nNewChannel, CArea* pArea);
    BOOL Play(BOOL bReplay);
    BOOL PlayAtCoord(int nNewXCoordinate, int nNewYCoordinate, int nNewZCoordinate, BOOL bReplay);
    BOOL Stop();
    BOOL IsSoundPlaying(bool bInSoundUpdate);
    BOOL SetVolume(int nNewVolume);
    BOOL ResetVolume();

	ResWavContainer wav; //4h
	BOOL bPositionedSound; //14h, uses 24h
	int nBufferSize; //18h
	int nFrequency; //1ch (1/500 of Hz), bSound_44KhzMixer uses freq without /500
	int nSRCurveRadius; //20h
	int nPositionalVolumePercent; //24h
	int xPos; //28h
	int yPos; //2ch
	int zPos; //30h
	int nPan; //34h, (1/5000 of dB)
	int nVolume; //38h (in %)
	BOOL bFrequencySet; // 3ch
	int nChannelIdx; //40h, channel to use
	int nPriority; //44h, lower number, higher priority when all buffers full (nTotalChannels[21] - nChannelIdx - 1 ) * 100 + custom
	BOOL bLoop; //48h
	int nFrequencyShift; //4ch (in Hz)
	int nVolumeShift; //50h (in %)
	BOOL b3DPositionning; //54h
	int* m_pSoundBuffer; //58h, IDirectSoundBuffer*
	BOOL bFireForget; //5ch, bAllowHighestPriority, bugged so does not do anything
	CArea* pArea; //60h

	//bit 0: Use cutscene volume reduction (reduces sound volume 25%)
	//bit 1: Use custom SR curve radius
	//bit 2: Use random frequency variation
	//bit 3: Use random volume variation
	//bit 4: Do not use environmental audio
	unsigned int dwWfxFlags; //64h

	bool bSoundIsntDucked; //68h, bCutsceneMode
	char u69; //pad
};


struct CSoundMixer { //Size 28DAh
//Constructor: 0x9DFADC
	BOOL InitSonglist(int nSongs, char** pSongFileArray);
    BOOL IsChannelUsed(int channel);
    void UpdateSoundList();
    void AddToLoopingList(CSound* pSoundPtr);

	BOOL bInitDsound; //0h
	int* lpDirectSound; //4h, LPDIRECTSOUND
	int* lpDirectSoundBuffer; //8h, LPDIRECTSOUNDBUFFER
#ifdef _DEBUG
	_CCriticalSection csSos; //ch, for music driver
#else
	CCriticalSection csSos;
#endif
	int u2c[12]; //unused?
	BOOL bMixerInitialized; //5ch
	int u60;
	
	//locks for access to arrays and lists below
	BOOL bInLoopingUpdate;  //64h bLockSoundChannelArray    CSoundMixer::RemoveFromLoopingList()
	BOOL bInPositionUpdate; //68h bLockLoopSoundQueue       CSoundMixer::UpdateSoundPositions()
	BOOL bInSoundUpdate;    //6ch bLockPlaylist             CSoundMixer::UpdateSoundList()
	BOOL bInQueueUpdate;    //70h bLockAreaLoopSoundQueue   CSoundMixer::UpdateQueue()
	CObArray SoundChannels; //74h
	CObList AllLoopingQueue;    //88h, looping CSounds on all areas
	CObList PlayingNow;         //a4h, currently playing CVoices, max count is CSoundMixer->nMaxSoundBuffers
	CObList Waiting;            //c0h, 1) CSounds from PlayingNow when it's overlowed,
                                //     2) looped CSounds if not current area

	HWND hWnd; //dch
	int ue0[4]; //unused, has get and set functions
	UINT uDeviceID; //f0h
	int nSoundChannels; //f4h
	int nMaxSoundBuffers; //f8h, set to 8
	int ufc; //1024, nMaxSoundChannels?
	
	//IDirectSound3dListener position
	int xPos; //100h, for positional sound
	int yPos; //104h, for positional sound
	int zPos; //108h, not used

	CArea* pArea; //10ch
	int nDeviceVolume; //110h, auxGetVolume() / 655
	int u114;
	int u118; //unused, has get and set functions

	CDWordArray u11c; //unused
	IECString sSongPath; //130h
	BOOL bSosDriverLoaded; //134h
	int nSongCurrent; //138h
	int nNumSongs; //13ch
	int nSongPrev; //140h
	struct CSoundProperties { //Size 24h
	//Constructor: 0x9F9537
		int* lpDirectSoundBuffer; //0h
		int* pIKsPropertySet; //4h

		//bit0 = DSPROPSETID_EAX_ReverbProperties supports KSPROPERTY_SUPPORT_SET
		//bit1 = DSPROPSETID_EAXBUFFER_ReverbProperties supports KSPROPERTY_SUPPORT_SET
		//bit2 = DSPROPSETID_VoiceManager supports KSPROPERTY_SUPPORT_SET
		unsigned int dwFlagsPropertySetSupportsSetFunction; //8h

		int uc;

		//PropertyData for DSPROPSETID_EAX_ReverbProperties
		int nEnvironment; //10h
		int nDamping; //14h
		int nDecayTime; //18h
		int nVolume; //1ch

		int nReverbLevel; //20h
	} m_CSoundProperties; //144h
	int* u168; //LPDIRECTSOUND3DLISTENER
	unsigned int dwFlagsPropertySetSupportsSetFunction; //16c, copy of CSoundProperties
	struct CA3d { //Size 2ch
	//Constructor: 0x9F9D68
		int* u0; //IA3d4*
		int* u4; //IA3dGeom*
		int* u8; //IA3dListener*
		IECPtrList uc; //AB9DF0, CA3dPlaylistElement (0x0 pIA3dSource, 0x4 pCSound, 0x8 int)
		CSoundMixer* pSoundMixer; //28h
	} m_A3d; //170h
	int u19c;
	int nSRCurveRadius; //1a0h
	bool bUseAureal3d; //1a4h
	char sqrtArray[10000]; //1a5h, contains sqrt(idx) used for spectral recording curve
	char u28b5; //pad
	int nCutsceneSounds; //28b6h
	CSongPosList m_SavedSongs; //28bah
	int nSavedMusicVolume; //28d6h, stores music volume when a cutscene sounds is played, restored when finished
};

extern BOOL (CSoundMixer::*CSoundMixer_InitSonglist)(int, char**);

class CVoice : CObject { //Size 16h, tobex new size 30h
//Constructor: 0x9DF55E
public:
	//AB996C

	CSound* pSound; //4h
	int* m_pSoundBuffer; //8h, IDirectSoundBuffer, used when pSound == null
	int nChannel; //ch
	int nPriority; //u44 of CSound
	bool bDuckedOthers; //14h
	char u15; //pad

    // tobex new
    char   bPauseMode;           // 16h
    char   bTimeStopMode;        // 17h
    char   bScreenMode;          // 18h
    char   pad1;                 // 19h
    char   pad2;                 // 1ah
    char   pad3;                 // 1bh
    DWORD  DSBufPosition;        // 1ch, DirectSoundBuffer_GetCurrentPosition()
    int    nVolume;              // 20h    
    ResRef soundName;            // 24h
    CArea *pArea;                // 2ch
};

#endif //SNDCORE_H