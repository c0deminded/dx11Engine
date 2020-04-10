#ifndef _KATAVICTIM_H_
#define _KATAVICTIM_H_

#include "Gameobject.h"

class KataVictim : public Gameobject {
public :
	KataVictim();
	~KataVictim();

	void Load() override;
	void  Unload() override;
	bool Init(HWND, D3DClass*) override;
	bool  Update() override;
};

#endif