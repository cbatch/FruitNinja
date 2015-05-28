#define _USE_MATH_DEFINES
#include <cmath>

#include "GameEntity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/detail/func_vector_relational.hpp>
#include "World.h"

using namespace glm;
using namespace std;

GameEntity::GameEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, bool collision_response) : mesh(mesh), rotations(glm::vec3(0.f, 0.f, 0.f)),
collision_response(collision_response)
{
    setup_entity_box();
    setPosition(position + vec3(0.f, bounding_box.half_height, 0.f));
    list = SET_DRAW(list);
    list = SET_OCTTREE(list);
};


vec3 GameEntity::turnAngle(vec3 cartesian)
{
    vec3 rot_angles(0, 0, 0);

    if (cartesian.x < 0)
        rot_angles.y = -1.0f * atan(cartesian.z / cartesian.x);
    else
        rot_angles.y = atan(cartesian.z / -cartesian.x) + M_PI;

    rot_angles.y -= M_PI / 2.f;

    return rot_angles;
}

float GameEntity::getScale()
{
    return scale;
}

void GameEntity::setScale(float entScale)
{
    scale = entScale;

    bounding_box.half_width *= scale;
    bounding_box.half_height *= scale;
    bounding_box.half_depth *= scale;
    bounding_box.center.y = bounding_box.half_height;
}

glm::vec3 GameEntity::getRotations()
{
    return rotations;
}

void GameEntity::setRotations(glm::vec3 rots)
{
    rotations = rots;
}

glm::vec3 GameEntity::getPosition()
{
    return bounding_box.center;
}

void GameEntity::setPosition(glm::vec3 position)
{
    bounding_box.center = position;
}

void GameEntity::setup_entity_box()
{
    vector<Mesh*> meshes = mesh->getMeshes();
    vec3 lower_bound(FLT_MAX, FLT_MAX, FLT_MAX);
    vec3 upper_bound(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (int i = 0; i < meshes.size(); i++)
    {
        pair<vec3, vec3> bounds = meshes.at(i)->get_lower_and_upper_bounds();
        vec3 m_lower_bound(bounds.first);
        vec3 m_upper_bound(bounds.second);

        vec3 less = lessThan(m_lower_bound, lower_bound);
        vec3 greater = greaterThan(m_upper_bound, upper_bound);
        lower_bound.x = less.x ? m_lower_bound.x : lower_bound.x;
        lower_bound.y = less.y ? m_lower_bound.y : lower_bound.y;
        lower_bound.z = less.z ? m_lower_bound.z : lower_bound.z;
        upper_bound.x = greater.x ? m_upper_bound.x : upper_bound.x;
        upper_bound.y = greater.y ? m_upper_bound.y : upper_bound.y;
        upper_bound.z = greater.z ? m_upper_bound.z : upper_bound.z;
    }

    bounding_box.half_width = glm::distance(lower_bound.x, upper_bound.x) / 2.f;
    bounding_box.half_height = glm::distance(lower_bound.y, upper_bound.y) / 2.f;
    bounding_box.half_depth = glm::distance(lower_bound.z, upper_bound.z) / 2.f;
}

void GameEntity::setup_entity_box(std::shared_ptr<MeshSet> mesh)
{
    vector<Mesh*> meshes = mesh->getMeshes();
    vec3 lower_bound(FLT_MAX, FLT_MAX, FLT_MAX);
    vec3 upper_bound(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (int i = 0; i < meshes.size(); i++)
    {
        pair<vec3, vec3> bounds = meshes.at(i)->get_lower_and_upper_bounds();
        vec3 m_lower_bound(bounds.first);
        vec3 m_upper_bound(bounds.second);

        vec3 less = lessThan(m_lower_bound, lower_bound);
        vec3 greater = greaterThan(m_upper_bound, upper_bound);
        lower_bound.x = less.x ? m_lower_bound.x : lower_bound.x;
        lower_bound.y = less.y ? m_lower_bound.y : lower_bound.y;
        lower_bound.z = less.z ? m_lower_bound.z : lower_bound.z;
        upper_bound.x = greater.x ? m_upper_bound.x : upper_bound.x;
        upper_bound.y = greater.y ? m_upper_bound.y : upper_bound.y;
        upper_bound.z = greater.z ? m_upper_bound.z : upper_bound.z;
    }
    bounding_box.half_width = glm::distance(lower_bound.x, upper_bound.x) / 2.f;
    bounding_box.half_height = glm::distance(lower_bound.y, upper_bound.y) / 2.f;
    bounding_box.half_depth = glm::distance(lower_bound.z, upper_bound.z) / 2.f;
    bounding_box.center.y = bounding_box.half_height;
}

void GameEntity::collision(std::shared_ptr<GameEntity> entity)
{
    //cout << "generic object collided!" << endl;
}

glm::mat4 GameEntity::getModelMat()
{
    mat4 model_rot_x = rotate(mat4(1.0f), rotations.x, vec3(1.f, 0.f, 0.f));
    mat4 model_rot_y = rotate(mat4(1.0f), rotations.y, vec3(0.f, 1.f, 0.f));
    mat4 model_rot_z = rotate(mat4(1.0f), rotations.z, vec3(0.f, 0.f, 1.f));

    modelMat = getAlignedModelMat() * model_rot_z * model_rot_x * model_rot_y;
    return modelMat;
}

glm::mat4 GameEntity::getAlignedModelMat()
{
    mat4 model_trans = translate(mat4(1.0f), bounding_box.center - vec3(0.f, bounding_box.half_height, 0.f));
    mat4 model_scale = glm::scale(mat4(1.0f), vec3(scale, scale, scale));

    alignedModelMat = model_trans * model_scale;

    return alignedModelMat;
}

void GameEntity::update()
{
}