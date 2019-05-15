// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkActor.h"
#include "WorldGenerator.generated.h"

UCLASS()
class VOXELWORLDPLUGIN_API AWorldGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//chunk data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoxelWorld")
	int m_ChunkSize = 64;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoxelWorld")
	float m_VoxelSize = 512.0f;

	//render data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoxelWorld")
		int m_ChunkDistance = 12;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoxelWorld")
		int m_ChunkDepthDistance = 2;

	//Terain data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoxelWorld")
		float m_Frequency = 0.005f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoxelWorld")
		int m_Octaves = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoxelWorld")
		float m_Redistribiution = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoxelWorld")
		int m_Terraces = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoxelWorld")
		float m_CaveFrequency = 0.04f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoxelWorld")
		float m_Isolevel = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VoxelWorld")
		UMaterial* m_Material = nullptr;
	UFUNCTION(BlueprintCallable, Category = "VoxelWorld")
		void SetSeed(int seed);

private:
	//chunk data
	TArray<AChunkActor*> m_Chunks;
	TArray<FVector> m_NewChunks;
	int m_ChunkSizeExtended = m_ChunkSize + 1;
	FVector m_LastPlayerPos = FVector(0.0f,0.0f,0.0f);

	//functions
	void UpdateChunks(const FVector& playerPos);
	void GenerateChunks(const TArray<FVector>& newChunksToGenerate);
	
};
