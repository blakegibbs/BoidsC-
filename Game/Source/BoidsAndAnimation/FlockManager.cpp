// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockManager.h"
#include "Components/BillboardComponent.h"
#include "Boid.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFlockManager::AFlockManager()
{
	//disable ticking
	PrimaryActorTick.bCanEverTick = false;

	maxSpeed = 700.0f;
	minSpeed = 300.0f;
	alignmentStrength = 200.0f;
	separationStrength = 30.0f;
	cohesionStrength = 5.0f;
	avoidanceStrength = 10000.0f;
	randomMoveStrength = 1000.0f;
	maxFlockmateDistance = 500.0f;
	target = nullptr;
	targetingStrength = 1.0f;
	separationFOV = -1.0f;
	alignmentFOV = 0.5f;
	cohesionFOV = -0.5f;
	numSensors = 100;
	sensorRadius = 300.0f;
	BuildAvoidanceSensors();
}


// Called when the game starts or when spawned
void AFlockManager::BeginPlay()
{
	Super::BeginPlay();
}

void AFlockManager::AddBoidToFlock(ABoid* Boid)
{
	if (Boid)
	{
		//add new boid to flock
		boids.AddUnique(Boid);
	}
}


void AFlockManager::SetMaxSpeed(float NewMaxSpeed)
{
	if (NewMaxSpeed < 0)
	{
		//log warning to console
		UE_LOG(LogTemp, Warning, TEXT("Request to change Boid Max Speed to negative value ignored in FlockManager: %s."), *GetName());
		return;
	}

	//update max speed to new setting
	maxSpeed = NewMaxSpeed;

	//check if new Max is lower than Min, lower Min to new speed to avoid Min > Max error behavior
	if (maxSpeed < minSpeed)
	{
		minSpeed = maxSpeed;
	}
}

void AFlockManager::SetMinSpeed(float newMinSpeed)
{
	if (newMinSpeed < 0)
	{
		//log warning to console
		UE_LOG(LogTemp, Warning, TEXT("Request to change Boid Max Speed to negative value ignored in FlockManager: %s."), *GetName());
		return;
	}

	//update min speed to new setting
	minSpeed = newMinSpeed;

	//check if new Min is higher than Max, raise Max to new speed to avoid Min > Max error behavior
	if (minSpeed > maxSpeed)
	{
		maxSpeed = minSpeed;
	}
}

void AFlockManager::SetAlignmentStrength(float newAlignmentStrength)
{
	alignmentStrength = newAlignmentStrength;
}

void AFlockManager::SetSeparationStrength(float newSeparationStrength)
{
	separationStrength = newSeparationStrength;
}

void AFlockManager::SetCohesionStrength(float newCohesionStrength)
{
	cohesionStrength = newCohesionStrength;
}

void AFlockManager::SetAvoidanceStrength(float newAvoidanceStrength)
{
	avoidanceStrength = newAvoidanceStrength;
}

void AFlockManager::SetRandMoveStrength(float newRandMoveStrength)
{
	randomMoveStrength = newRandMoveStrength;
}

void AFlockManager::SetMaxFlockmateDistance(float newMaxFlockmateDistance)
{
	avoidanceStrength = newMaxFlockmateDistance;
}

void AFlockManager::BuildAvoidanceSensors()
{
	//empty sensor array
	avoidanceSensors.Empty();

	//theta angle of rotation on xy plane around z axis (yaw) around sphere
	float theta;
	//phi angle of rotation (~pitch) around sphere
	float phi;
	//assuming unit sphere (radius = 1) to simplify calculations
	//direction vector pointing from the center to point on sphere surface
	FVector sensorDirection;

	for (int32 i = 0; i < numSensors; ++i)
	{
		//calculate the spherical coordinates of the direction vectors endpoint
		theta = 2 * UKismetMathLibrary::GetPI() * goldenRatio * i;
		phi = FMath::Acos(1 - (2 * float(i) / numSensors));

		//convert point on sphere to cartesian coordinates xyz
		sensorDirection.X = FMath::Cos(theta) * FMath::Sin(phi);
		sensorDirection.Y = FMath::Sin(theta) * FMath::Sin(phi);
		sensorDirection.Z = FMath::Cos(phi);
		//add direction to list of sensors for avoidance
		avoidanceSensors.Emplace(sensorDirection);
	}
}

