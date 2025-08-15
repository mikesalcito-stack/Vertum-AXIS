#include "VitrumGearSystem.h"
#include "Components/StaticMeshComponent.h"

AVitrumGearSystem::AVitrumGearSystem()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create gears dynamically or in BP
}

void AVitrumGearSystem::BeginPlay()
{
    Super::BeginPlay();
}

void AVitrumGearSystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsRotating)
    {
        for (int i = 0; i < Gears.Num(); ++i)
        {
            float Direction = (i % 2 == 0) ? 1.0f : -1.0f;
            Gears[i]->AddLocalRotation(FRotator(0, RotationSpeed * DeltaTime * Direction, 0));
        }
    }
}

void AVitrumGearSystem::StartRotation()
{
    bIsRotating = true;
    OnRotationStarted();
}

void AVitrumGearSystem::StopRotation()
{
    bIsRotating = false;
    OnRotationStopped();
}

void AVitrumGearSystem::UpdateGearPositions(FRotator WorldRotation)
{
    // Realign gears based on rotation
    for (UStaticMeshComponent* Gear : Gears)
    {
        FVector NewPos = WorldRotation.RotateVector(Gear->GetRelativeLocation());
        Gear->SetRelativeLocation(NewPos);
    }
    // Check if gears form a continuous path (logic for puzzle solving)
}
