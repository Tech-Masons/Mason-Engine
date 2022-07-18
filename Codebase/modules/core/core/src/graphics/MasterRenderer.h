#pragma once
#include "cpch.h"
#include "cdefs.h"
#include "structures/Vertices.h"
#include "interfaces/iRenderer.h"

namespace Graphics
{
	enum class API;
	class iSurface;
}

#include <map>
struct ModelData
{	
};

class MasterRenderer
{
public:
	MasterRenderer();
	~MasterRenderer();

	void Initilize(uint api, iGameWindow* pGameWindow);

	void Shutdown();

	void Render();

	void Update(double deltaTime);

	void UpdateViewMatrix(matrix4f _view);

	iRenderer* GetRenderer(RenderType type);

	static MasterRenderer* Create(uint api, iGameWindow* pGameWindow);
private:
	std::map<RenderType, iRenderer*> renderers;
	iGameWindow* pGameWindow;
	matrix4f viewMatrix;
};