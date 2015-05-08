#include "World.h"
#include "DeferredRenderer.h"

using namespace glm;

DeferredRenderer::DeferredRenderer(std::string vertShader, std::string fragShader, GBuffer* gbuffer)
	: Shader(vertShader, fragShader), gbuffer(gbuffer), stencilShader("StencilVert.glsl", "StencilFrag.glsl")
{
	std::shared_ptr<MeshSet> mesh(new MeshSet(assetPath + "UnitSphere.obj"));
	lights.push_back(std::make_shared<Light>(vec3(30.0, 16.0, 31.5), vec3(1.0, 1.0, 1.0), 500.0f, mesh));
	lights.push_back(std::make_shared<Light>(vec3(0.0, 16.0, 31.5), vec3(1.0, 1.0, 1.0), 500.0f, mesh));
	lights.push_back(std::make_shared<Light>(vec3(30.0, 16.0, 0.0), vec3(1.0, 1.0, 1.0), 500.0f, mesh));
	lights.push_back(std::make_shared<Light>(vec3(-30.0, 2.0, 20), vec3(1.0, 1.0, 1.0), 500.0f, mesh));
	lights.push_back(std::make_shared<Light>(vec3(-50.0, 10.0, 0.0), vec3(1.0, 1.0, 1.0), 500.0f, mesh));

	glBindAttribLocation(getProgramID(), 0, "aPosition");
}

//failed attempt
DeferredRenderer::~DeferredRenderer()
{
	
}

void DeferredRenderer::pointLightPass(std::shared_ptr<Camera> camera, std::shared_ptr<Light> light)
{
	gbuffer->BindForLightPass();

	glUseProgram(getProgramID());

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glBindVertexArray(light->VAO());

	glUniform1i(getUniformHandle("posMap"), GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glUniform1i(getUniformHandle("colMap"), GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	glUniform1i(getUniformHandle("norMap"), GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);

	vec3 eye = camera->cameraPosition;
	vec3 light_pos = light->pos;
	glUniform3f(getUniformHandle("uEye"), eye.x, eye.y, eye.z);
	glUniform3f(getUniformHandle("uLightPos"), light_pos.x, light_pos.y, light_pos.z);
	glUniform2f(getUniformHandle("uSize"), (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	glUniformMatrix4fv(getUniformHandle("uModelMatrix"), 1, GL_FALSE, value_ptr(light->transform()));
	glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, glm::value_ptr(
		mat4(perspective((float)radians(45.0), screen_width / screen_height, 0.1f, 800.f))));


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light->IND());
	check_gl_error("rend before");
	glDrawElements(GL_TRIANGLES, light->indices_size(), GL_UNSIGNED_INT, 0);
	check_gl_error("rend after");


	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);

	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}


void DeferredRenderer::draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents)
{
	glEnable(GL_STENCIL_TEST);
	for (int i = 0; i < lights.size(); i++) {
		stencilShader.stencilPass(camera, gbuffer, lights[i]);
		pointLightPass(camera, lights[i]);
	}
	
	glDisable(GL_STENCIL_TEST);


}
void DeferredRenderer::draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity)
{
	std::cout << "I won't draw!";
}