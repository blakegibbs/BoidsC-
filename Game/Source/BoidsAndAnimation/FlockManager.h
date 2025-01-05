// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlockManager.generated.h"

class ABoid;
class UBillboardComponent;

UCLASS()
class BOIDSANDANIMATION_API AFlockManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlockManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	TArray<ABoid*> boids;

	UPROPERTY(EditAnywhere, Category = "Boid Movement")
	float maxSpeed;
	UPROPERTY(EditAnywhere, Category = "Boid Movement")
	float minSpeed;
	UPROPERTY(EditAnywhere, Category = "Boid Steering")
	float alignmentStrength;
	UPROPERTY(EditAnywhere, Category = "Boid Steering")
	float separationStrength;
	UPROPERTY(EditAnywhere, Category = "Boid Steering")
	float cohesionStrength;
	UPROPERTY(EditAnywhere, Category = "Boid Steering")
	float avoidanceStrength;
	UPROPERTY(EditAnywhere, Category = "Boid Steering")
	float randomMoveStrength;

	UPROPERTY(EditAnywhere, Category = "Boid Target")
	AActor* target;
	UPROPERTY(EditAnywhere, Category = "Boid Target")
	float targetingStrength;
	UPROPERTY(EditAnywhere, Category = "Boid Target")
	float maxTargetDistance;

	UPROPERTY(EditAnywhere, Category = "Boid Perception", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float separationFOV;
	UPROPERTY(EditAnywhere, Category = "Boid Perception", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float alignmentFOV;
	UPROPERTY(EditAnywhere, Category = "Boid Perception", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float cohesionFOV;

	UPROPERTY(EditAnywhere, Category = "Boid Flock Distance")
	float maxFlockmateDistance;

	const float goldenRatio = (1.0f + FMath::Sqrt(5.0f)) / 2;

	UPROPERTY(EditAnywhere, Category = "Boid Avoidance", meta = (ClampMin = "0", ClampMax = "1000"))
	int32 numSensors;

	UPROPERTY(EditAnywhere, Category = "Boid Avoidance", meta = (ClampMin = "0"))
	float sensorRadius;

	TArray<FVector> avoidanceSensors;

	void BuildAvoidanceSensors();


public:
	void AddBoidToFlock(ABoid* boid);

	inline TArray<ABoid*> GetFlock() { return boids; };

	UFUNCTION(BlueprintCallable, Category = "Boid Movement")
	inline float GetMaxSpeed() { return maxSpeed; };
	UFUNCTION(BlueprintCallable, Category = "Boid Movement")
	inline float GetMinSpeed() { return minSpeed; };
	UFUNCTION(BlueprintCallable, Category = "Boid Movement")
	void SetMaxSpeed(float newMaxSpeed);
	UFUNCTION(BlueprintCallable, Category = "Boid Movement")
	void SetMinSpeed(float newMinSpeed);

	UFUNCTION(BlueprintCallable, Category = "Boid Steering")
	inline float GetAlignmentStrength() { return alignmentStrength; };
	UFUNCTION(BlueprintCallable, Category = "Boid Steering")
	inline float GetSeparationStrength() { return separationStrength; };
	UFUNCTION(BlueprintCallable, Category = "Boid Steering")
	inline float GetCohesionStrength() { return cohesionStrength; };
	UFUNCTION(BlueprintCallable, Category = "Boid Steering")
	inline float GetAvoidanceStrength() { return avoidanceStrength; };
	UFUNCTION(BlueprintCallable, Category = "Boid Steering")
	inline float GetRandomMoveStrength() { return randomMoveStrength; };
	UFUNCTION(BlueprintCallable, Category = "Boid Target")
	inline float GetTargetingStrength() { return targetingStrength; }
	UFUNCTION(BlueprintCallable, Category = "Boid Target")
	inline float GetMaxTargetDistance() { return maxTargetDistance; }
	UFUNCTION(BlueprintCallable, Category = "Boid Target")
	inline AActor* GetTarget() { return target; }
	UFUNCTION(BlueprintCallable, Category = "Boid Flock Distance")
	inline float GetMaxFlockmateDistance() { return maxFlockmateDistance; };
	UFUNCTION(BlueprintCallable, Category = "Boid Steering")
	void SetAlignmentStrength(float newAlignmentStrength);
	UFUNCTION(BlueprintCallable, Category = "Boid Steering")
	void SetSeparationStrength(float newSeparationStrength);
	UFUNCTION(BlueprintCallable, Category = "Boid Steering")
	void SetCohesionStrength(float newCohesionStrength);
	UFUNCTION(BlueprintCallable, Category = "Boid Steering")
	void SetAvoidanceStrength(float newAvoidanceStrength);
	UFUNCTION(BlueprintCallable, Category = "Boid Steering")
	void SetRandMoveStrength(float newRandMoveStrength);
	UFUNCTION(BlueprintCallable, Category = "Boid Flock Distance")
	void SetMaxFlockmateDistance(float newMaxFlockmateDistance);




	UFUNCTION(BlueprintCallable, Category = "Boid Perception")
	inline float GetSeparationFOV() { return separationFOV; };
	UFUNCTION(BlueprintCallable, Category = "Boid Perception")
	inline float GetAlignmentFOV() { return alignmentFOV; };
	UFUNCTION(BlueprintCallable, Category = "Boid Perception")
	inline float GetCohesionFOV() { return cohesionFOV; };

	inline float GetSensorRadius() { return sensorRadius; }
	inline TArray<FVector> GetAvoidanceSensors() { return avoidanceSensors; }
};
