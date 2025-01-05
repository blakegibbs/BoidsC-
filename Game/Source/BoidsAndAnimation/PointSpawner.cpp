// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BillboardComponent.h"
#include "Boid.h"
#include "FlockManager.h"
#include "PointSpawner.h"

// Sets default values
APointSpawner::APointSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	spawnPointBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Spawn Point Billboard Component"));
	RootComponent = spawnPointBillboard;

	numBoidsToSpawn = 0;
}

// Called when the game starts or when spawned
void APointSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnBoids(numBoidsToSpawn);
}

void APointSpawner::SpawnBoids(int32 numBoids)
{
	if(assignedFlockManager == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No FlockManager found for Boid spawner: %s."), *GetName());
		return;
	}

	if(boidType == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BoidType not set for Spawner: %s"), *GetName());
		return;
	}

	FVector spawnLocation = this->GetActorLocation();
	FRotator spawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters boidSpawnParamaters;
	boidSpawnParamaters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	boidSpawnParamaters.Owner = assignedFlockManager;

	for (int32 i = 0; i < numBoids; i++)
	{
		spawnRotation = FMath::VRand().ToOrientationRotator();
		assignedFlockManager->AddBoidToFlock(GetWorld()->SpawnActor<ABoid>(boidType, spawnLocation, spawnRotation, boidSpawnParamaters));
	}
}