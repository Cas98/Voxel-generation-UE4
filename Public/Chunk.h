// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Chunk.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOXELWORLDPLUGIN_API UChunk : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChunk();
	/*virtual ~UChunk();

	enum class VoxelType
	{
		Air,
		Full
	};
*/
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaticMesh")
//		UStaticMeshComponent* m_StaticMeshCube;
//private:
//	//const int m_ChunkSize;
//	VoxelType*** m_ChunkArray;
//
	
};
