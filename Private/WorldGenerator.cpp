// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGenerator.h"
#include <algorithm>
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Engine.h"
#include "SimplexNoiseLibrary.h"

// Sets default values
AWorldGenerator::AWorldGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWorldGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWorldGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Update chunks
	if (GetWorld()->GetFirstPlayerController()->GetPawn() != nullptr)
	{
		m_LastPlayerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	}
	UpdateChunks(m_LastPlayerPos);

	//spawn one chunk each frame
	if (m_NewChunks.Num() > 0)
	{
		FActorSpawnParameters SpawnInfo;

		auto pos = m_NewChunks[m_NewChunks.Num() - 1];

		auto chunk = GetWorld()->SpawnActor<AChunkActor>(pos, FRotator(0, 0, 0), SpawnInfo);
		m_Chunks.Add(chunk);
		chunk->CreateVoxelsMultiThreaded(pos);
		chunk->SetVoxelSize(m_VoxelSize);
		chunk->SetChunkSize(m_ChunkSize);
		chunk->SetFrequency(m_Frequency);
		chunk->SetOctaves(m_Octaves);
		chunk->SetRedistribution(m_Redistribiution);
		chunk->SetTerraces(m_Terraces);
		chunk->SetCaveFrequency(m_CaveFrequency);
		chunk->SetIsoLevel(m_Isolevel);
		if (m_Material != nullptr)
		{
			chunk->SetMaterial(m_Material);
		}
		m_NewChunks.Pop();
	}
}

void AWorldGenerator::UpdateChunks(const FVector& playerPos)
{
	//DeleteChunks out of range
	//************************
	for (int i{ 0 }; i < m_Chunks.Num(); ++i)
	{
		if (abs(m_Chunks[i]->GetPosition().X - playerPos.X) > (m_VoxelSize * m_ChunkSizeExtended * m_ChunkDistance )
			|| abs(m_Chunks[i]->GetPosition().Y - playerPos.Y) > (m_VoxelSize * m_ChunkSizeExtended * m_ChunkDistance))
		{
			auto chunk = m_Chunks[i];
			m_Chunks.Remove(chunk);
			chunk->Destroy();
		}
	}

	//check if chunks need to be generated
	//************************************
	FVector corner = playerPos;
	corner.X = ((int(m_VoxelSize) * m_ChunkSize) * (int(playerPos.X) / (int(m_VoxelSize) * m_ChunkSize)));
	corner.Y = ((int(m_VoxelSize) * m_ChunkSize) * (int(playerPos.Y) / (int(m_VoxelSize) * m_ChunkSize)));
	corner.Z = 0.0f;

	//check if chunk is already generated if not add to list to generate chunks
	for (int x{ 0 }; x < m_ChunkDistance; ++x)
	{
		for (int y{ 0 }; y < m_ChunkDistance; ++y)
		{
			for (int z{ 0 }; z < m_ChunkDepthDistance; ++z)
			{
				//get new chunk location
				FVector chunkPos{ 0.0f,0.0f,0.0f };
				chunkPos.X = corner.X + (x * m_VoxelSize * m_ChunkSize) - (m_VoxelSize * m_ChunkSize * (m_ChunkDistance / 2));
				chunkPos.Y = corner.Y + (y * m_VoxelSize * m_ChunkSize) - (m_VoxelSize * m_ChunkSize * (m_ChunkDistance / 2));
				chunkPos.Z = corner.Z - ((z * m_VoxelSize * m_ChunkSize) + (m_VoxelSize * m_ChunkSize));

				//check if chunk is already generated
				bool isInArray = false;

				for (int i{ 0 }; i < m_Chunks.Num(); ++i)
				{
					if ((abs(m_Chunks[i]->GetPosition().X - chunkPos.X)) < 0.1f
						&& (abs(m_Chunks[i]->GetPosition().Y - chunkPos.Y)) < 0.1f
						&& (abs(m_Chunks[i]->GetPosition().Z - chunkPos.Z)) < 0.1f)
					{
						isInArray = true;
						break;
					}
				}

				if (!isInArray)
				{
					for (int i{ 0 }; i < m_NewChunks.Num(); ++i)
					{
						if ((abs(m_NewChunks[i].X - chunkPos.X)) < 0.1f
							&& (abs(m_NewChunks[i].Y - chunkPos.Y)) < 0.1f
							&& (abs(m_NewChunks[i].Z - chunkPos.Z)) < 0.1f)
						{
							isInArray = true;
							break;
						}
					}
				}

				//if chunk not generated add 
				if (!isInArray)
				{
					m_NewChunks.Add(chunkPos);
				}
			}
		}
	}
}

void AWorldGenerator::GenerateChunks(const TArray<FVector>& newChunksToGenerate)
{
	for (int i{ 0 }; i < newChunksToGenerate.Num(); ++i)
	{
		FActorSpawnParameters SpawnInfo;

		auto pos = newChunksToGenerate[i];
		
		auto chunk = GetWorld()->SpawnActor<AChunkActor>(pos, FRotator(0, 0, 0), SpawnInfo);
		m_Chunks.Add(chunk);
		chunk->SetVoxelSize(m_VoxelSize);
		chunk->SetChunkSize(m_ChunkSize);
		chunk->SetFrequency(m_Frequency);
		chunk->SetOctaves(m_Octaves);
		chunk->SetRedistribution(m_Redistribiution);
		chunk->SetCaveFrequency(m_CaveFrequency);
		chunk->SetIsoLevel(m_Isolevel);
		chunk->CreateVoxelsMultiThreaded(pos);
	}
}

void AWorldGenerator::SetSeed(int seed)
{
	USimplexNoiseLibrary::setNoiseSeed(seed);
}



