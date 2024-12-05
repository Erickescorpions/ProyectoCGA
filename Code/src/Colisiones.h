/*
 * Colisiones.h
 *
 *  Created on: Dec 18, 2019
 *      Author: rey
 */

#ifndef COLISIONES_H_
#define COLISIONES_H_

#include <map>
#include "Headers/AbstractModel.h"

void addOrUpdateColliders(
		std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > &colliders,
		std::string name, AbstractModel::OBB collider, glm::mat4 transform);
void addOrUpdateColliders(
		std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > &colliders,
		std::string name);
void addOrUpdateColliders(
		std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > &colliders,
		std::string name, AbstractModel::SBB collider, glm::mat4 transform);
void addOrUpdateColliders(
		std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > &colliders,
		std::string name);
void addOrUpdateCollisionDetection(std::map<std::string, bool> &collisionDetector,
		std::string name, bool isCollision);

void addOrUpdateCollisionDetection2(std::map<std::string, std::string> &collisionDetector,
		std::string collider1, std::string collider2); 

bool raySphereIntersect(glm::vec3 orig, glm::vec3 dest, glm::vec3 dir, AbstractModel::SBB sbb, float &t);
bool testSphereSphereIntersection(AbstractModel::SBB sbb1, AbstractModel::SBB sbb2);
bool testSphereOBox(AbstractModel::SBB sbb, AbstractModel::OBB obb);
bool testOBBOBB(AbstractModel::OBB a, AbstractModel::OBB b);

#endif /* COLISIONES_H_ */
