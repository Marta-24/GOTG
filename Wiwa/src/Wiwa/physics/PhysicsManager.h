#pragma once

#include <wipch.h>
#include <Wiwa/core/Core.h>

#include <Wiwa/ecs/components/RigidBody.h>
#include <Wiwa/ecs/components/ColliderCube.h>
#include <Wiwa/ecs/components/ColliderCylinder.h>
#include <Wiwa/ecs/components/ColliderSphere.h>
#include "Wiwa/ecs/components/Transform3D.h"
#include <Wiwa/utilities/render/shaders/Shader.h>
#include <Wiwa/utilities/render/Uniforms.h>
#include <Wiwa/core/Resources.h>

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include <vector>
// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f)
class DebugDrawer;
class Camera;
namespace Wiwa {

	struct MyRigidBody {
		MyRigidBody(btRigidBody& body_, const size_t id_) : btBody(&body_), id(id_) {};

		btRigidBody* btBody;
		size_t id;
	};

	enum CollisionType {
		CT_ENTER,
		CT_LOOP,
		CT_LEAVE,
		CT_NONE
	};

	struct CollisionData {
		size_t entityA;
		size_t entityB;
		CollisionType collisionType;
	};

	class WI_API PhysicsManager
	{
	public:
		PhysicsManager();

		~PhysicsManager();

		bool InitWorld();

		bool StepSimulation();

		bool UpdateEngineToPhysics();

		bool UpdatePhysicsToEngine();

		bool CleanWorld();

		bool DeleteBody(MyRigidBody* body);

		bool AddBodySphere(size_t id, const Wiwa::ColliderSphere& sphere, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		bool AddBodyCube(size_t id, const Wiwa::ColliderCube& cube, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		bool AddBodyCylinder(size_t id, const Wiwa::ColliderCylinder& cylinder, Wiwa::Transform3D& transform, Wiwa::Rigidbody& rigid_body);

		void SetBodyMass(MyRigidBody* body, float mass);

		void SetBodyGravity(MyRigidBody* body, btVector3 gravity);

		void SetTrigger(MyRigidBody* body, bool isTrigger);

		void SetStatic(MyRigidBody* body, bool isStatic);

		MyRigidBody* FindByEntityId(size_t id);

		void UpdateCollisionType(size_t first, size_t second);

		inline glm::vec3 WiwaToGLM(Wiwa::Vector3f vector)
		{
			return glm::vec3(vector.x, vector.y, vector.z);
		}

		bool getInit();

		bool LogBodies();

		void DebugDrawWorld();
		
	private:
		bool m_Debug;
		bool m_HasBeenInit;
		btDiscreteDynamicsWorld* m_World;

		btDefaultCollisionConfiguration* m_Collision_conf;
		btCollisionDispatcher* m_Dispatcher;
		btBroadphaseInterface* m_Broad_phase;
		btSequentialImpulseConstraintSolver* m_Solver;

		//  btDefaultVehicleRaycaster* v_Vehicle_raycaster;
		DebugDrawer* m_Debug_draw;

		std::list<btCollisionShape*> m_Shapes;
		std::list<MyRigidBody*> m_Bodies;
		std::list<btDefaultMotionState*> m_Motions;
		std::list<btTypedConstraint*> m_Constraints;

		std::vector<CollisionData> m_CollisionList;
	};
}

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer()
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;

	ResourceId lineDisplayShaderId;
	Wiwa::Shader* lineDisplayShader;
	DefaultUnlitUniforms lineDisplayShaderUniforms;
};