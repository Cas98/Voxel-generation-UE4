// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkActor.h"


/**
 * 
 */
class VOXELWORLDPLUGIN_API MarchingCubes
{
public:
	MarchingCubes(float*** chunkArray, TArray<int>& indices, TArray<FVector>& vertices, TArray<FVector>& normals, const int chunkSize,const float isolevel, const float voxelSize);
	~MarchingCubes();

private:
	void ConstructMesh(float*** chunkArray, TArray<int>& indices, TArray<FVector>& vertices, TArray<FVector>& normals, const int chunkSize, const float isolevel, const float voxelSize);
	float GetDensity(const int cornerIndex, const int voxelIndexX, const int voxelIndexY, const int voxelIndexZ, float*** chunkArray);
	FVector VertexInterp(const FVector &vertex1, const FVector &vertex2, const float val1, const float val2, const float isolevel);
	
};


//Table
class Table {

public :
	static TArray<int> m_Edges;

	static TArray<TArray<int>> m_Triangles;

};