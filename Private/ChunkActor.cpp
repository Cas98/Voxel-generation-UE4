// Fill out your copyright notice in the Description page of Project Settings.

#include "ChunkActor.h"
#include "Engine/Engine.h"
#include "SimplexNoiseLibrary.h"
#include "MarchingCubes.h"
#include "DrawDebugHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

// Sets default values
AChunkActor::AChunkActor()
{
	//Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialize procedural mesh
	m_ChunkMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	m_ChunkMesh->bUseAsyncCooking = true;

	//init material
	ConstructorHelpers::FObjectFinder<UMaterial> materialOb(TEXT("Material'/VoxelWorldPlugin/TerrainMaterial.TerrainMaterial'"));
	m_Material = materialOb.Object;
}

AChunkActor::~AChunkActor()
{
	
}

// Called when the game starts or when spawned
void AChunkActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AChunkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//create procedural mesh
	if (m_GenerateMesh)
	{
		GenerateChunkMesh();
		m_GenerateMesh = false;
	}


	if (MeshUpdateTimer <= 0.0f)
	{
		UpdateMesh();
	}
	else
	{
		MeshUpdateTimer -= DeltaTime;
	}
}

void AChunkActor::UpdateMesh()
{

}

void AChunkActor::GenerateChunkMesh()
{
	m_ChunkMesh->CreateMeshSection(0, m_Vertices, m_Indices, m_Normals, TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	//empty buffers
	m_Vertices.Empty();
	m_Indices.Empty();
	m_Normals.Empty();
	//set material
	m_ChunkMesh->SetMaterial(0, Cast<UMaterialInterface>(m_Material));
}



void AChunkActor::CreateVoxelsMultiThreaded(const FVector& chunkPos)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("SpawnChunk"));
	//Call CreateVoxels on worker thread
	(new FAutoDeleteAsyncTask<ChunkCalculationAsyncTask>(this, chunkPos))->StartBackgroundTask();
}

void AChunkActor::CreateVoxels(const FVector& chunkPos)
{
	//set position
	m_Position = chunkPos;

	//Allocate chunk voxel memory
	m_ChunkArray = new float**[m_ChunkSizeExtended];

	for (int i = 0; i < m_ChunkSizeExtended; i++)
	{
		m_ChunkArray[i] = new float*[m_ChunkSizeExtended];

		for (int j = 0; j < m_ChunkSizeExtended; j++)
		{

			m_ChunkArray[i][j] = new float[m_ChunkSizeExtended];
		}
	}


	// assign values to chunk
	for (int x = 0; x < m_ChunkSizeExtended; x++)
	{
		for (int y = 0; y < m_ChunkSizeExtended; y++)
		{
			for (int z = 0; z < m_ChunkSizeExtended; z++)
			{

				m_ChunkArray[x][y][z] = 0.0f;

			}
		}
	}

	//create voxel array
	for (int x = 0; x < m_ChunkSizeExtended; x++)
	{
		for (int y = 0; y < m_ChunkSizeExtended; y++)
		{
			//Terrain height
			ApplyTerrainHeight(x, y);
			for (int z = 0; z < m_ChunkSizeExtended; z++)
			{
				if (m_ChunkArray[x][y][z] > m_Isolevel)
				{
					//cave generation
					Apply3DSimplexNoise(x, y, z);
				}
			}
		}
	}

	//smooth voxel terrain using marching cubes
	MarchingCubes temp{ m_ChunkArray, m_Indices, m_Vertices, m_Normals, m_ChunkSize, m_Isolevel, m_VoxelSize };
	
	//if set to true procedural mesh will be created in Tick function
	m_GenerateMesh = true;
}

void AChunkActor::ApplyTerrainHeight(int indexX, int indexY)
{
	//Get input coordinates for the noise based of voxel the position
	int noiseIndexX = int(m_Position.X / m_VoxelSize) + indexX;
	int noiseIndexY = int(m_Position.Y / m_VoxelSize) + indexY;
	int layerIndex = int(m_BasegroundLevel / m_VoxelSize) - int(m_Position.Z / (m_VoxelSize * m_ChunkSize));

	float noiseValue{ 0 };

	//Octaves and frequency
	for (int i{ 0 }; i < m_Octaves; i++)
	{
		noiseValue += (USimplexNoiseLibrary::SimplexNoiseInRange2D((m_Frequency  * (i + 1)) * noiseIndexX, (m_Frequency  * (i + 1)) * noiseIndexY, 0.5f, 1.0f)) / m_Octaves;
	}

	//Terracing
	if (m_Terraces != 0)
	{
		noiseValue = (round(noiseValue * m_Terraces) / m_Terraces);
	}

	//redistribution
	noiseValue = pow(noiseValue, m_Redistribiution);

	for (int i{ 0 }; i < m_ChunkSizeExtended; ++i)
	{
		m_ChunkArray[indexX][indexY][i] += ((noiseValue * layerIndex)) - ((noiseValue / m_ChunkSize) * (i - 1));
	}
}

void AChunkActor::Apply3DSimplexNoise(int indexX, int indexY, int indexZ)
{
	FVector input;
	input.X = int(m_Position.X / m_VoxelSize) + indexX;
	input.Y = int(m_Position.Y / m_VoxelSize) + indexY;
	input.Z = int(m_Position.Z / m_VoxelSize) + indexZ;

	float noiseValue = USimplexNoiseLibrary::SimplexNoiseInRange3D(input.X * m_CaveFrequency, input.Y * m_CaveFrequency, input.Z * m_CaveFrequency, 0.0f, 1.0f);

	if ((m_ChunkArray[indexX][indexY][indexZ] + noiseValue) < m_Isolevel)
	{
		m_ChunkArray[indexX][indexY][indexZ] += noiseValue;
	}
}

FVector AChunkActor::GetPosition() const
{
	return m_Position;
}

void AChunkActor::SetVoxelSize(float voxelSize)
{
	m_VoxelSize = voxelSize;
}

void AChunkActor::SetChunkSize(int chunkSize)
{
	m_ChunkSize = chunkSize;
	m_ChunkSizeExtended = m_ChunkSize + 1;
}


void AChunkActor::SetFrequency(float frequency)
{
	m_Frequency = frequency;
}

void AChunkActor::SetOctaves(int octaves)
{
	m_Octaves = octaves;
}

void AChunkActor::SetRedistribution(float redistribution)
{
	m_Redistribiution = redistribution;
}

void AChunkActor::SetTerraces(float terraces)
{
	m_Terraces = terraces;
}

void AChunkActor::SetCaveFrequency(float caveFrequency)
{
	m_CaveFrequency = caveFrequency;
}

void AChunkActor::SetIsoLevel(float isoLevel)
{
	m_Isolevel = isoLevel;
}

void AChunkActor::SetMaterial(UMaterial* material)
{
	m_Material = material;
}

