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
	SceneTypeEnd
};
enum class VertexType
{
	VertexPosition = 1,
	VertexColor,
	VertexNormal,
	VertexTangent,
	VertexTextureCoord0,
	VertexTextureCoord1,
	VertexTypeEnd
};

enum class MaterialType
{
	MaterialAlbedoMap = 1,
	MaterialSpecularMap,
	MaterialNormalMap,
	MaterialMetallicMap,
	MaterialEmissionMap,
	MaterialDetailAlbedoMap,
	MaterialDetailNormalMap,
	MaterialMapEnd
};

enum class ComponentType
{
	ComponentShader,
	ComponentTransform,
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
