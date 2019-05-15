// Fill out your copyright notice in the Description page of Project Settings.

#include "Chunk.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UChunk::UChunk()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	 

	////Allocate chunk voxel memory
	//m_ChunkArray = new VoxelType**[5];

	//for (int i = 0; i < 5; i++)
	//{
	//	m_ChunkArray[i] = new VoxelType*[5];

	//	for (int j = 0; j < 5; j++)
	//	{

	//		m_ChunkArray[i][j] = new VoxelType[5];
	//	}
	//}
	//

	//// assign values to allocated memory
	//for (int i = 0; i < 5; i++)
	//{
	//	for (int j = 0; j < 5; j++)
	//	{
	//		for (int k = 0; k < 5; k++)
	//		{
	//			m_ChunkArray[i][j][k] = VoxelType::Full;
	//		}
	//	}
	//}
}
//
//UChunk::~UChunk()
//{
//	/*delete m_ChunkArray;
//	m_ChunkArray = 0;*/
//}



// Called when the game starts
void UChunk::BeginPlay()
{
	/*Super::BeginPlay();

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int k = 0; k < 5; k++)
			{
				if (m_ChunkArray[i][j][k] == VoxelType::Full)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("FullBlock"));
			}
		}
	}*/
}


// Called every frame
void UChunk::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

