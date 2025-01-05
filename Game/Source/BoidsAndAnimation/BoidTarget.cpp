#include "BoidTarget.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

ABoidTarget::ABoidTarget()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABoidTarget::BeginPlay()
{
    Super::BeginPlay();

    if (!PlayerController)
    {
        PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->bShowMouseCursor = true;
            PlayerController->SetInputMode(FInputModeGameAndUI());
        }
    }
}

void ABoidTarget::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdatePositionToMouse(DeltaTime);
}

void ABoidTarget::UpdatePositionToMouse(float DeltaTime)
{
    if (!PlayerController)
    {
        PlayerController = GetWorld()->GetFirstPlayerController();
        if (!PlayerController) return;
    }

    float MouseDeltaX, MouseDeltaY;
    PlayerController->GetInputMouseDelta(MouseDeltaX, MouseDeltaY);

    FVector CurrentPosition = GetActorLocation();

    FVector AdjustedPosition = CurrentPosition + FVector(0, MouseDeltaX * 10.f, MouseDeltaY * 10.f);

    SetActorLocation(AdjustedPosition);
}
