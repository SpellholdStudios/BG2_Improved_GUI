#ifndef ENGPRIESTBK_H
#define ENGPRIESTBK_H

#include "engine.h"

class CScreenPriestBook : public CEngine { //Size 1470h
//constructor: 0x784820
public:
	void SetLevel(int);
	void Update();
    void SummonPopup(int);
    BOOL CanCastPriestSpells(CCreatureObject *pSprite);

	virtual void v0() {}

	char ufe[0x346];
	int currLevel; //444h
    char u448[0x552-0x448];
    ResRef CurrentSpell; // 0x552
	//0x572 - BOOL showKnownSpellIcons (?)
};

extern void (CScreenPriestBook::*CScreenPriestBook_SetLevel)(int);
extern void (CScreenPriestBook::*CScreenPriestBook_Update)();

#endif //ENGPRIESTBK_H