#include "engpriestbk.h"

#include "stdafx.h"

void (CScreenPriestBook::*CScreenPriestBook_SetLevel)(int) =
	SetFP(static_cast<void (CScreenPriestBook::*)(int)>	(&CScreenPriestBook::SetLevel),	0x7868D9);
void (CScreenPriestBook::*CScreenPriestBook_Update)() =
	SetFP(static_cast<void (CScreenPriestBook::*)()>	(&CScreenPriestBook::Update),	0x7870AC);

void CScreenPriestBook::SetLevel(int nLevel)	{ return (this->*CScreenPriestBook_SetLevel)(nLevel); }
void CScreenPriestBook::Update()				{ return (this->*CScreenPriestBook_Update)(); }
_n void CScreenPriestBook::SummonPopup(int) { _bgmain(0x78785B) }
_n BOOL CScreenPriestBook::CanCastPriestSpells(CCreatureObject *pSprite) { _bgmain(0x7887CE) }