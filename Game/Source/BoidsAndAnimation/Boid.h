// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

class USphereComponent;
class AFlockManager;

UCLASS()
class BOIDSANDANIMATION_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();
	//called every frame
	virtual void Tick(float DeltaTime) override;

	inline AFlockManager* GetFlockManager() { return flockManager; }
	inline FVector GetBoidVelocity() { return velocity; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Boid Components")
	USphereComponent* BoidCollision;

	AFlockManager* flockManager;
	FVector velocity;

	FVector Seperation(TArray<ABoid*> flock);
	FVector Alignment(TArray<ABoid*> flock);
	FVector Cohesion(TArray<ABoid*> flock);
	FVector Targeting(AActor* target);

	void Steering(float DeltaTime);

	bool IsHeadingForCollision();
	FVector AvoidObstacle();
};
