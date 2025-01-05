#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidTarget.generated.h"

UCLASS()
class BOIDSANDANIMATION_API ABoidTarget : public AActor
{
    GENERATED_BODY()

public:
    ABoidTarget();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    void UpdatePositionToMouse(float DeltaTime);

private:
    APlayerController* PlayerController = nullptr;
};