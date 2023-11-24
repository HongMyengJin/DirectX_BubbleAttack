#pragma once

//SceneComponent Type
enum class SceneComponentType
{
	Stage1ComponentType,
	SceneComponentTypeEnd
};

enum class SceneType
{
	Stage1Type,
	MirrorStageType,
	SceneTypeEnd
};

enum class MaterialType
{
	MaterialAlbedoMap = 0x01,
	MaterialSpecularMap = 0x02,
	MaterialNormalMap = 0x04,
	MaterialMetallicMap = 0x08,
	MaterialEmissionMap = 0x10,
	MaterialDetailAlbedoMap = 0x20,
	MaterialDetailNormalMap = 0x40,
	MaterialMapEnd
};

enum class ComponentType
{
	ComponentTransform,
	ComponentShader,
	ComponentMaterial,
	ComponentMesh,
	ComponentTypeEnd
};

enum class ResourceTextureType
{
	ResourceTexture2D = 1,
	ResourceTexture2D_ARRAY,
	ResourceTexture2DARRAY,
	ResourceTextureCUBE,
	ResourceTexture2DBUFFER,
	ResourceTextureTypeEnd
};
