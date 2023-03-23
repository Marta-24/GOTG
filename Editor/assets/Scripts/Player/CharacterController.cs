using System;
using Wiwa;


namespace Game
{
    [Component]
    public struct CharacterController
    {
        public float Velocity;
        public float GamepadDeadZone;
        public float RotationSpeed;

        public float WalkTreshold;
    }
    class CharacterControllerSystem : Behaviour
    {
        private ComponentIterator characterControllerIt;
        private ComponentIterator transformIt;
        private ComponentIterator rigidBodyIt;
        void Awake()
        {
            characterControllerIt = GetComponentIterator<CharacterController>();
            transformIt = GetComponentIterator<Transform3D>();
            rigidBodyIt = GetComponentIterator<CollisionBody>();
            Animator.PlayAnimationName("walk", m_EntityId);
        }

        void Update()
        {
            ref Transform3D transform = ref GetComponentByIterator<Transform3D>(transformIt);
            ref CharacterController controller = ref GetComponentByIterator<CharacterController>(characterControllerIt);
            ref CollisionBody rb = ref GetComponentByIterator<CollisionBody>(rigidBodyIt);

            Vector3 input = GetInput(ref controller);
            Vector3 velocity = input * controller.Velocity * Time.DeltaTimeMS();

            PhysicsManager.SetLinearVelocity(m_EntityId, velocity);

            if(input != Vector3Values.zero)
                SetPlayerRotation(ref transform.LocalRotation, input, controller.RotationSpeed);

            UpdateAnimation(input, controller);
        }

        Vector3 GetInput(ref CharacterController controller)
        {
            Vector3 input = Vector3Values.zero;

            //     z
            //     |
            //     |
            //-----|----- -x
            //     |
            //     |



            if (Input.IsKeyDown(KeyCode.A))
                input.x += 1;
            if (Input.IsKeyDown(KeyCode.D))
                input.x -= 1;
            if (Input.IsKeyDown(KeyCode.W))
                input.z += 1;
            if (Input.IsKeyDown(KeyCode.S))
                input.z -= 1;

            //This axis need to be inverted
            input.x -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.LeftX, controller.GamepadDeadZone);
            input.z -= Input.GetRawAxis(Gamepad.GamePad1, GamepadAxis.LeftY, controller.GamepadDeadZone);

            return input;
        }
        void SetPlayerRotation(ref Vector3 currentRotation, Vector3 input, float rotationSpeed)
        {
            float angle = Mathf.Atan2(input.x, input.z) * Mathf.Rad2Deg;
            currentRotation.y = Mathf.LerpAngle(currentRotation.y, angle, rotationSpeed);
        }
        void UpdateAnimation(Vector3 input, CharacterController controller)
        {
            //if (input == Vector3Values.zero)
            //{
            //    Animator.PlayAnimationName("idle", m_EntityId);
            //    Console.WriteLine("Player Idle");
            //    return;
            //}

            //float mag = input.magnitude;
            //Mathf.Clamp01(mag);

            //if (mag <= controller.WalkTreshold)
            //{
            //    Animator.PlayAnimationName("walk", m_EntityId);
            //    Console.WriteLine("Player Walk");
            //    return;
            //}
            //Animator.PlayAnimationName("run", m_EntityId);
            //Console.WriteLine("Player Run");
            
        }
    }
}