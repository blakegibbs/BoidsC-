// Fill out your copyright notice in the Description page of Project Settings.

#include "Boid.h"
#include "FlockManager.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "BoidsAndAnimation.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoidCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Boid Collision Component"));
	RootComponent = BoidCollision;
	BoidCollision->SetCollisionObjectType(ECC_Pawn);
	BoidCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoidCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();

	AFlockManager* boidOwner = Cast<AFlockManager>(this->GetOwner());
	if (boidOwner) 
	{
		flockManager = boidOwner;
		velocity = this->GetActorForwardVector();
		velocity.Normalize();
		velocity *= FMath::FRandRange(flockManager->GetMinSpeed(), flockManager->GetMaxSpeed());
	}
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Steering(DeltaTime);
}

void ABoid::Steering(float DeltaTime) 
{
	TArray<ABoid*> flockmates = flockManager->GetFlock();
	AActor* target = flockManager->GetTarget();

	FVector acceleration = FVector::ZeroVector;

	this->SetActorLocation(this->GetActorLocation() + (velocity * DeltaTime));
	this->SetActorRotation(velocity.ToOrientationQuat());

	acceleration += Seperation(flockmates);
	acceleration += Alignment(flockmates);
	acceleration += Cohesion(flockmates);
	acceleration += Targeting(target);

	if (IsHeadingForCollision()) 
	{
		acceleration += AvoidObstacle();
	}

	float randomMoveStrength = flockManager->GetRandomMoveStrength();
	FVector randomForce = FVector(FMath::FRandRange(-randomMoveStrength, randomMoveStrength), FMath::FRandRange(-randomMoveStrength, randomMoveStrength), FMath::FRandRange(-randomMoveStrength, randomMoveStrength));
	acceleration += randomForce; 

	velocity += acceleration * DeltaTime;
	velocity = velocity.GetClampedToSize(flockManager->GetMinSpeed(), flockManager->GetMaxSpeed());
}

FVector ABoid::Seperation(TArray<ABoid*> flock)
{
	FVector steeringForce = FVector::ZeroVector;
	int32 flockSize = 0;

	for (ABoid* boid : flock)
	{
		if (boid != nullptr && boid != this)
		{
			if (FVector::DotProduct(this->GetActorForwardVector(),
				(boid->GetActorLocation() - this->GetActorLocation()).GetSafeNormal()) <= flockManager->GetSeparationFOV())
			{
				continue;
			}

			FVector separationDirection = this->GetActorLocation() - boid->GetActorLocation();
			float distance = separationDirection.Size();

			separationDirection = separationDirection.GetSafeNormal();

			float weight = (distance > 0) ? 1.0f / distance : 1.0f;

			steeringForce += separationDirection * weight;
			flockSize++;
		}
	}

	if (flockSize > 0)
	{
		steeringForce /= flockSize;
		steeringForce.GetSafeNormal() -= this->velocity.GetSafeNormal();
		steeringForce *= flockManager->GetSeparationStrength();
	}
	else 
	{
		return FVector::ZeroVector;
	}

	return steeringForce;
}


FVector ABoid::Alignment(TArray<ABoid*> flock)
{
	FVector steeringForce = FVector::ZeroVector;
	int32 flockSize = 0;
	float distance = 0;

	for (ABoid* boid : flock) 
	{
		if (boid != nullptr && boid != this) 
		{
			if (FVector::DotProduct(this->GetActorForwardVector(), (boid->GetActorLocation() - this->GetActorLocation()).GetSafeNormal()) <= flockManager->GetAlignmentFOV())
			{
				continue;
			}

			distance = FVector::Dist(this->GetActorLocation(), boid->GetActorLocation());

			if (distance > flockManager->GetMaxFlockmateDistance()) 
			{
				return FVector::ZeroVector;
			}
			else 
			{
				steeringForce += boid->velocity.GetSafeNormal();
				flockSize++;
			}
		}
	}

	if (flockSize > 0)
	{
		steeringForce /= flockSize;
		steeringForce.GetSafeNormal() -= this->velocity.GetSafeNormal();
		steeringForce *= flockManager->GetAlignmentStrength();
		return steeringForce;
	}
	else
	{
		return FVector::ZeroVector;
	}
}

FVector ABoid::Cohesion(TArray<ABoid*> flock)
{
	FVector steeringForce = FVector::ZeroVector;
	FVector averagePosition = FVector::ZeroVector;
	int32 flockSize = 0;

	for (ABoid* boid : flock) 
	{
		if (boid != nullptr && boid != this) 
		{
			if (FVector::DotProduct(this->GetActorForwardVector(), (boid->GetActorLocation() - this->GetActorLocation()).GetSafeNormal()) <= flockManager->GetCohesionFOV())
			{
				continue;
			}
			averagePosition += boid->GetActorLocation();
			flockSize++;
		}
	}

	if (flockSize > 0)
	{
		averagePosition /= flockSize;
		steeringForce = averagePosition - this->GetActorLocation();
		steeringForce.GetSafeNormal() -= this->velocity.GetSafeNormal();
		steeringForce *= flockManager->GetCohesionStrength();
		return steeringForce;
	}
	else
	{
		return FVector::ZeroVector;
	}
}

FVector ABoid::Targeting(AActor* target)
{
	FVector steeringForce = FVector::ZeroVector;

	if (target != nullptr)
	{
		FVector targetLocation = target->GetActorLocation();
		FVector currentLocation = this->GetActorLocation();

		float distance = FVector::Dist(this->GetActorLocation(), target->GetActorLocation());

		if (distance <= flockManager->GetMaxTargetDistance())
		{
			FVector desiredVelocity = (targetLocation - currentLocation).GetSafeNormal() * flockManager->GetMaxSpeed();
			FVector currentVelocity = this->GetVelocity();

			steeringForce = desiredVelocity - currentVelocity;

			steeringForce *= flockManager->GetTargetingStrength();
		}
	}

	return steeringForce;
}


bool ABoid::IsHeadingForCollision()
{
	if (flockManager == nullptr) { return false; }

	if (flockManager->GetAvoidanceSensors().Num() > 0)
	{
		FQuat sensorRotation = FQuat::FindBetweenVectors(flockManager->GetAvoidanceSensors()[0], this->GetActorForwardVector());
		FVector newSensorDirection = FVector::ZeroVector;
		newSensorDirection = sensorRotation.RotateVector(flockManager->GetAvoidanceSensors()[0]);
		FCollisionQueryParams TraceParameters;
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, this->GetActorLocation(), this->GetActorLocation() + newSensorDirection * flockManager->GetSensorRadius(), COLLISION_AVOIDANCE, TraceParameters);

		return Hit.bBlockingHit;
	}

	return false;
}

FVector ABoid::AvoidObstacle()
{
	//check for valid flock manager
	if (flockManager == nullptr) { return FVector::ZeroVector; }

	FVector Steering = FVector::ZeroVector;
	FQuat SensorRotation = FQuat::FindBetweenVectors(flockManager->GetAvoidanceSensors()[0], this->GetActorForwardVector());
	FVector NewSensorDirection = FVector::ZeroVector;
	FCollisionQueryParams TraceParameters;
	FHitResult Hit;

	for (FVector AvoidanceSensor : flockManager->GetAvoidanceSensors())
	{
		//rotate avoidance sensor to align with boid orientation and trace for collision
		NewSensorDirection = SensorRotation.RotateVector(AvoidanceSensor);
		GetWorld()->LineTraceSingleByChannel(Hit, this->GetActorLocation(), this->GetActorLocation() + NewSensorDirection * flockManager->GetSensorRadius(), COLLISION_AVOIDANCE, TraceParameters);
		if (!Hit.bBlockingHit)
		{
			Steering = NewSensorDirection.GetSafeNormal() - this->velocity.GetSafeNormal();
			Steering *= flockManager->GetAvoidanceStrength();
			return Steering;
		}
	}

	return FVector::ZeroVector;
}


