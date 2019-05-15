// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "ChunkActor.generated.h"

UCLASS()
class VOXELWORLDPLUGIN_API AChunkActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkActor();
	virtual ~AChunkActor();
	
	enum class ChunkLOD
	{
		LOD0,
		LOD1,
		LOD2,
		LOD3
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//initialisation for chunk array
	void CreateVoxels(const FVector& chunkPos);
	//Call CreateVoxels function on worker thread
	void CreateVoxelsMultiThreaded(const FVector& chunkPos);
	
	FVector GetPosition() const;

	//set parameters
	void SetVoxelSize(float voxelSize);
	void SetChunkSize(int chunkSize);
	void SetFrequency(float frequency);
	void SetOctaves(int octaves);
	void SetRedistribution(float redistribution);
	void SetTerraces(float terraces);
	void SetCaveFrequency(float caveFrequency);
	void SetIsoLevel(float isoLevel);
	void SetMaterial(UMaterial* material);

private:
	//chunk data
	float*** m_ChunkArray;
	float m_VoxelSize;
	int m_ChunkSize;
	int m_ChunkSizeExtended;
	FVector m_Position;

	//Terrain generation data
	float m_BasegroundLevel = 0.0f;
	float m_Frequency;
	int m_Octaves;
	float m_Redistribiution;
	float m_CaveFrequency;
	int m_Terraces;

	//Marching cubes data
	float m_Isolevel;

	//procedural mesh data
	UProceduralMeshComponent* m_ChunkMesh;
	TArray<FVector> m_Vertices;
	TArray<int32> m_Indices;
	TArray<FVector> m_Normals;

	//mesh generation
	bool m_GenerateMesh = false; //Set to true after multithreaded CreateVoxels function is done
	
	//Terrain generation functions
	void Apply3DSimplexNoise(int indexX, int indexY, int indexZ);
	void ApplyTerrainHeight(int indexX, int indexY);
	
	//procedural mesh function
	void GenerateChunkMesh();
	void UpdateMesh();
	float MeshUpdateTimer = 10.0f;
	
	//material
	UMaterial* m_Material;

};


//*********************
//MultiThreading
//*********************
class ChunkCalculationAsyncTask : public FNonAbandonableTask
{
private:
	AChunkActor* m_Object;
	FVector m_ChunkPos;

public:
	/*Default constructor*/
	ChunkCalculationAsyncTask(AChunkActor* object, const FVector& chunkPos)
	{
		this->m_Object = object;
		this->m_ChunkPos = chunkPos;
	}

	/*This function is needed from the API of the engine.*/
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(ChunkCalculationAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	/*This function is executed when we tell our task to execute*/
	void DoWork()
	{
		m_Object->CreateVoxels(m_ChunkPos);
	}
};