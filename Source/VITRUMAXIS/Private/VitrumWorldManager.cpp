#include "VitrumWorldManager.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "VitrumGameMode.h"
#include "VitrumMarble.h"

AVitrumWorldManager::AVitrumWorldManager()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create root component
    WorldRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WorldRoot"));
    RootComponent = WorldRoot;

    // Set default values
    bIsRotating = false;
    CurrentRotation = EWorldRotation::North;
    TargetRotation = EWorldRotation::North;
    RotationTimer = 0.0f;
    RotationStart = FRotator::ZeroRotator;
    EndRotation = FRotator::ZeroRotator;
    GameMode = nullptr;
}

void AVitrumWorldManager::BeginPlay()
{
    Super::BeginPlay();

    // Get reference to game mode
    GameMode = Cast<AVitrumGameMode>(GetWorld()->GetAuthGameMode());

    // Set initial rotation
    SetWorldRotation(EWorldRotation::North);
}

void AVitrumWorldManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateRotation(DeltaTime);
}

void AVitrumWorldManager::RotateWorldLeft()
{
    if (!bIsRotating)
    {
        EWorldRotation NewRotation;
        
        switch (CurrentRotation)
        {
            case EWorldRotation::North:
                NewRotation = EWorldRotation::West;
                break;
            case EWorldRotation::West:
                NewRotation = EWorldRotation::South;
                break;
            case EWorldRotation::South:
                NewRotation = EWorldRotation::East;
                break;
            case EWorldRotation::East:
                NewRotation = EWorldRotation::North;
                break;
        }
        
        StartRotation(NewRotation);
    }
}

void AVitrumWorldManager::RotateWorldRight()
{
    if (!bIsRotating)
    {
        EWorldRotation NewRotation;
        
        switch (CurrentRotation)
        {
            case EWorldRotation::North:
                NewRotation = EWorldRotation::East;
                break;
            case EWorldRotation::East:
                NewRotation = EWorldRotation::South;
                break;
            case EWorldRotation::South:
                NewRotation = EWorldRotation::West;
                break;
            case EWorldRotation::West:
                NewRotation = EWorldRotation::North;
                break;
        }
        
        StartRotation(NewRotation);
    }
}

void AVitrumWorldManager::SetWorldRotation(EWorldRotation NewRotation)
{
    if (!bIsRotating && CurrentRotation != NewRotation)
    {
        StartRotation(NewRotation);
    }
}

void AVitrumWorldManager::RegisterRotatableActor(AActor* Actor)
{
    if (Actor && !RotatableActors.Contains(Actor))
    {
        RotatableActors.Add(Actor);
    }
}

void AVitrumWorldManager::UnregisterRotatableActor(AActor* Actor)
{
    if (Actor)
    {
        RotatableActors.Remove(Actor);
    }
}

void AVitrumWorldManager::ApplyRotationToActor(AActor* Actor, EWorldRotation Rotation)
{
    if (Actor)
    {
        FRotator NewRotation = GetRotationForDirection(Rotation);
        Actor->SetActorRotation(NewRotation);
    }
}

void AVitrumWorldManager::UpdateRotation(float DeltaTime)
{
    if (bIsRotating)
    {
        RotationTimer += DeltaTime;
        float Alpha = FMath::Clamp(RotationTimer / RotationDuration, 0.0f, 1.0f);
        
        // Use smooth interpolation
        float SmoothAlpha = FMath::SmoothStep(0.0f, 1.0f, Alpha);
        FRotator CurrentRot = FMath::Lerp(RotationStart, EndRotation, SmoothAlpha);
        
        // Apply rotation to world root
        WorldRoot->SetWorldRotation(CurrentRot);
        
        // Apply rotation to all registered actors
        ApplyRotationToAllActors(CurrentRotation);
        
        // Check if rotation is complete
        if (Alpha >= 1.0f)
        {
            CompleteRotation();
        }
    }
}

void AVitrumWorldManager::StartRotation(EWorldRotation NewTarget)
{
    if (!bIsRotating)
    {
        bIsRotating = true;
        TargetRotation = NewTarget;
        RotationTimer = 0.0f;
        
        // Store start and end rotations
        RotationStart = GetRotationForDirection(CurrentRotation);
        EndRotation = GetRotationForDirection(TargetRotation);
        
        // Broadcast rotation started event
        OnRotationChanged.Broadcast(TargetRotation);
        
        // Update game mode if available
        if (GameMode)
        {
            GameMode->SetGameState(EGameState::Solve);
        }
    }
}

void AVitrumWorldManager::CompleteRotation()
{
    bIsRotating = false;
    CurrentRotation = TargetRotation;
    
    // Ensure final rotation is exact
    WorldRoot->SetWorldRotation(GetRotationForDirection(CurrentRotation));
    
    // Broadcast rotation complete event
    OnRotationComplete.Broadcast(CurrentRotation);
    
    // Update game mode if available
    if (GameMode)
    {
        GameMode->SetGameState(EGameState::Flow);
    }
}

FRotator AVitrumWorldManager::GetRotationForDirection(EWorldRotation Direction)
{
    switch (Direction)
    {
        case EWorldRotation::North:
            return FRotator(0.0f, 0.0f, 0.0f);
        case EWorldRotation::East:
            return FRotator(0.0f, 90.0f, 0.0f);
        case EWorldRotation::South:
            return FRotator(0.0f, 180.0f, 0.0f);
        case EWorldRotation::West:
            return FRotator(0.0f, 270.0f, 0.0f);
        default:
            return FRotator::ZeroRotator;
    }
}

void AVitrumWorldManager::ApplyRotationToAllActors(EWorldRotation Rotation)
{
    for (AActor* Actor : RotatableActors)
    {
        if (Actor && IsValid(Actor))
        {
            ApplyRotationToActor(Actor, Rotation);
        }
    }
}
