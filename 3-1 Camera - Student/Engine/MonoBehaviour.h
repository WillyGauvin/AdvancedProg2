#pragma once
#include "Component.h"

class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();
//STEP 12 - LOOK AT 
private:
	virtual void FinalUpdate() sealed { }
};

