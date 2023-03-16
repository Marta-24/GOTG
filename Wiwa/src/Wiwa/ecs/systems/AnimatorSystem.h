#pragma once
#include "System.h"
#include "../components/Transform3D.h"
#include "../components/AnimatorComponent.h"

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	class WI_API AnimatorSystem : public System {
	private:

	public:
		AnimatorSystem();
		~AnimatorSystem();

		void OnAwake() override;

		void OnInit() override;

		void OnUpdate() override;

		void OnDestroy() override;

		void OnEventFinish();
	};
}

REGISTER_SYSTEM(Wiwa::AnimatorSystem);