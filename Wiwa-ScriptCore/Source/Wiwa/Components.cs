﻿namespace Wiwa
{
    using ResourceId = System.UInt64;
    #region Transform3D
    [Component]
    public struct Transform3D
    {
        public Vector3 Position;
        public Vector3 Rotation;
        public Vector3 Scale;

        public Transform3D(Vector3 pos, Vector3 rot, Vector3 scl)
        {
            Position = pos;
            Rotation = rot;
            Scale = scl;
        }
    }
    #endregion
    #region Mesh
    [Component]
    public struct Mesh
    {
        public ResourceId MeshId;
        public ResourceId MaterialId;

        public Mesh(ResourceId meshId, ResourceId matId)
        {
            MeshId = meshId;
            MaterialId = matId;
        }
    }
    #endregion

}
