#pragma once
#include "Model.h"
#include "TextureLoader.h"
class CEmptyLoader : public CModel
{
public:
	virtual void InitModel(string file_name) override;

};
