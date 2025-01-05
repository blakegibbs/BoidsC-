// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointSpawner.generated.h"

class UBillboardComponent;
class ABoid;
class AFlockManager;

UCLASS()
class BOIDSANDANIMATION_API APointSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APointSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected :
	UPROPERTY(VisibleAnywhere, Category = "Boid Components")
	UBillboardComponent* spawnPointBillboard;

protected:
	UPROPERTY(EditAnywhere, Category = "Boid Spawn")
	int32 numBoidsToSpawn;

	UPROPERTY(EditAnywhere, Category = "Boid Spawn")
	TSubclassOf<ABoid> boidType;

	void SpawnBoids(int32 numBoids);

	UPROPERTY(EditAnywhere, Category = "Boid Spawn")
	AFlockManager* assignedFlockManager;
};
