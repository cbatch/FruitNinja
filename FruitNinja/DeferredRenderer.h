#pragma once

#include "Shader.h"		
#include "StencilShader.h"
#include "DirLightShader.h"
#include "ShadowMapShader.h"

class DeferredRenderer : public Shader
{
public:
	DeferredRenderer(std::string vertShader, std::string fragShader, GBuffer* gbuffer);
	~DeferredRenderer();
	virtual void draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents, std::vector<Light*> lights);
	void draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity) override;
private:
	void pointLightPass(std::shared_ptr<Camera> camera, Light* light);
	GBuffer* gbuffer;
	StencilShader stencilShader;
	DirLightShader dirLightShader;
	ShadowMapShader shadowMapShader;
};