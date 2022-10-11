#pragma once

#include "Panel.h"

class ShadingView;

class ScenePanel : public Panel
{
public:
	ScenePanel();
	virtual ~ScenePanel();

	void Draw() override;

private:
	std::vector<ShadingView*> m_Shadings;
};
