#include "pch.h"
#include "Material.h"
#include "Engine.h"

void Material::PushData()//STEP 12 - LOOK AT
{
	// CBV
	//CALL CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushData(..) passing in &_params, sizeof(_params)
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushData(&_params, sizeof(_params));

	// SRV 
	for (size_t i = 0; i < _textures.size(); i++)
	{
		if (_textures[i] == nullptr)
			continue;
		//DECLARE a variable called reg of type SRV_REGISTER and ASSIGN it to the return value of SRV_REGISTER() passing in static_cast<int8>(SRV_REGISTER::t0) + i to SRV_REGISTER()
		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		//CALL GEngine->GetTableDescHeap()->SetSRV() passing in _textures[i]->GetCpuHandle(), reg
		GEngine->GetTableDescHeap()->SetSRV(_textures[i]->GetCpuHandle(), reg);
	}

	//CALL Update() on  _shader
	_shader->Update();
}
