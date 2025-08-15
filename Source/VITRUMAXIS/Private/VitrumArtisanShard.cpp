#include "VitrumArtisanShard.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "VitrumMarble.h"
#include "VitrumLevelManager.h"

AVitrumArtisanShard::AVitrumArtisanShard()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create components
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->SetSphereRadius(50.0f);
    CollisionSphere->SetCollisionProfileName(TEXT("OverlapAll"));

    ShardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShardMesh"));
    ShardMesh->SetupAttachment(RootComponent);
    ShardMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    IdleParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticles"));
    IdleParticles->SetupAttachment(RootComponent);

    CollectionParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CollectionParticles"));
    CollectionParticles->SetupAttachment(RootComponent);
    CollectionParticles->SetAutoActivate(false);

    // Set default values
    ShardIndex = 0;
    ShardType = EShardType::Standard;
    ShardState = EShardState::Idle;
    ShardName = TEXT("Artisan Shard");
    ShardDescription = TEXT("A fragment of pure energy, pulsing with ancient power.");
    ShardValue = 1;

    // Visual properties
    RotationSpeed = 90.0f;
    BobSpeed = 2.0f;
    BobHeight = 20.0f;
    ShardColor = FLinearColor(0.2f, 0.8f, 1.0f, 1.0f);
    PulseSpeed = 3.0f;
    PulseIntensity = 0.3f;

    // Audio properties
    IdleSoundVolume = 0.3f;
    CollectionSoundVolume = 0.8f;

    // Initialize internal state
    InitialZ = 0.0f;
    CurrentRotation = 0.0f;
    CurrentBob = 0.0f;
    CurrentPulse = 0.0f;
    IdleAudioComponent = nullptr;
    ShardMaterial = nullptr;
    bIsInitialized = false;
}

void AVitrumArtisanShard::BeginPlay()
{
    Super::BeginPlay();

    // Store initial Z position
    InitialZ = GetActorLocation().Z;

    // Set up collision events
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AVitrumArtisanShard::OnBeginOverlap);

    // Create dynamic material
    if (ShardMesh && ShardMesh->GetMaterial(0))
    {
        ShardMaterial = UMaterialInstanceDynamic::Create(ShardMesh->GetMaterial(0), this);
        ShardMesh->SetMaterial(0, ShardMaterial);
    }

    // Set up audio
    if (IdleSound)
    {
        IdleAudioComponent = UGameplayStatics::SpawnSoundAttached(IdleSound, RootComponent);
        if (IdleAudioComponent)
        {
            IdleAudioComponent->SetVolumeMultiplier(IdleSoundVolume);
            IdleAudioComponent->bAutoActivate = true;
        }
    }

    // Initialize shard properties based on type
    InitializeShardProperties();

    bIsInitialized = true;
}

void AVitrumArtisanShard::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsInitialized && ShardState == EShardState::Idle)
    {
        UpdateVisuals(DeltaTime);
        UpdateAudio();
    }
}

void AVitrumArtisanShard::CollectShard()
{
    if (ShardState != EShardState::Idle)
    {
        return;
    }

    ShardState = EShardState::Collected;

    // Play collection effects
    PlayCollectionEffects();

    // Stop idle effects
    if (IdleParticles)
    {
        IdleParticles->Deactivate();
    }

    if (IdleAudioComponent)
    {
        IdleAudioComponent->Stop();
    }

    // Hide the shard mesh
    if (ShardMesh)
    {
        ShardMesh->SetVisibility(false);
    }

    // Notify level manager
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVitrumLevelManager::StaticClass(), FoundActors);
    
    if (FoundActors.Num() > 0)
    {
        AVitrumLevelManager* LevelManager = Cast<AVitrumLevelManager>(FoundActors[0]);
        if (LevelManager)
        {
            LevelManager->CollectArtisanShard(ShardIndex);
        }
    }

    // Broadcast event
    OnShardCollected.Broadcast(ShardIndex);

    // Destroy after a delay to allow effects to play
    SetLifeSpan(2.0f);
}

void AVitrumArtisanShard::ConsumeShard()
{
    if (ShardState == EShardState::Collected)
    {
        ShardState = EShardState::Consumed;
    }
}

bool AVitrumArtisanShard::IsCollected() const
{
    return ShardState == EShardState::Collected;
}

bool AVitrumArtisanShard::IsConsumed() const
{
    return ShardState == EShardState::Consumed;
}

void AVitrumArtisanShard::ResetShard()
{
    ShardState = EShardState::Idle;
    
    if (ShardMesh)
    {
        ShardMesh->SetVisibility(true);
    }

    if (IdleParticles)
    {
        IdleParticles->Activate();
    }

    if (IdleAudioComponent)
    {
        IdleAudioComponent->Play();
    }
}

FString AVitrumArtisanShard::GetShardName() const
{
    return ShardName;
}

FString AVitrumArtisanShard::GetShardDescription() const
{
    return ShardDescription;
}

int32 AVitrumArtisanShard::GetShardValue() const
{
    return ShardValue;
}

TArray<FName> AVitrumArtisanShard::GetUnlockedAbilities() const
{
    return UnlockedAbilities;
}

void AVitrumArtisanShard::UpdateVisuals(float DeltaTime)
{
    // Update rotation
    CurrentRotation += RotationSpeed * DeltaTime;
    if (CurrentRotation >= 360.0f)
    {
        CurrentRotation -= 360.0f;
    }

    // Update bob
    CurrentBob += BobSpeed * DeltaTime;
    if (CurrentBob >= 360.0f)
    {
        CurrentBob -= 360.0f;
    }

    // Update pulse
    CurrentPulse += PulseSpeed * DeltaTime;
    if (CurrentPulse >= 360.0f)
    {
        CurrentPulse -= 360.0f;
    }

    // Apply visual effects
    if (ShardMesh)
    {
        // Set rotation
        FRotator NewRotation = GetActorRotation();
        NewRotation.Yaw = CurrentRotation;
        ShardMesh->SetRelativeRotation(NewRotation);

        // Set bob position
        FVector NewLocation = GetActorLocation();
        float BobOffset = FMath::Sin(FMath::DegreesToRadians(CurrentBob)) * BobHeight;
        NewLocation.Z = InitialZ + BobOffset;
        SetActorLocation(NewLocation);
    }

    // Update material properties
    UpdateMaterialProperties();
}

void AVitrumArtisanShard::UpdateAudio()
{
    if (IdleAudioComponent && IdleAudioComponent->IsPlaying())
    {
        // Adjust volume based on distance to player
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVitrumMarble::StaticClass(), FoundActors);
        
        if (FoundActors.Num() > 0)
        {
            AVitrumMarble* Player = Cast<AVitrumMarble>(FoundActors[0]);
            if (Player)
            {
                float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
                float MaxDistance = 1000.0f;
                float VolumeMultiplier = FMath::Max(0.0f, 1.0f - (Distance / MaxDistance));
                IdleAudioComponent->SetVolumeMultiplier(IdleSoundVolume * VolumeMultiplier);
            }
        }
    }
}

void AVitrumArtisanShard::PlayCollectionEffects()
{
    // Play collection particles
    if (CollectionParticles)
    {
        CollectionParticles->Activate();
    }

    // Play collection sound
    if (CollectionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, CollectionSound, GetActorLocation(), GetActorRotation(), CollectionSoundVolume);
    }

    // Add screen shake or other effects here
}

void AVitrumArtisanShard::UpdateMaterialProperties()
{
    if (ShardMaterial)
    {
        // Set base color
        ShardMaterial->SetVectorParameter(TEXT("BaseColor"), ShardColor);

        // Set pulse effect
        float PulseValue = 1.0f + FMath::Sin(FMath::DegreesToRadians(CurrentPulse)) * PulseIntensity;
        ShardMaterial->SetScalarParameter(TEXT("PulseIntensity"), PulseValue);

        // Set emission intensity based on shard type
        float EmissionIntensity = 1.0f;
        switch (ShardType)
        {
        case EShardType::Standard:
            EmissionIntensity = 1.0f;
            break;
        case EShardType::Rare:
            EmissionIntensity = 1.5f;
            break;
        case EShardType::Legendary:
            EmissionIntensity = 2.0f;
            break;
        case EShardType::Master:
            EmissionIntensity = 3.0f;
            break;
        }
        ShardMaterial->SetScalarParameter(TEXT("EmissionIntensity"), EmissionIntensity);
    }
}

void AVitrumArtisanShard::StartIdleSound()
{
    if (IdleAudioComponent && !IdleAudioComponent->IsPlaying())
    {
        IdleAudioComponent->Play();
    }
}

void AVitrumArtisanShard::StopIdleSound()
{
    if (IdleAudioComponent && IdleAudioComponent->IsPlaying())
    {
        IdleAudioComponent->Stop();
    }
}

void AVitrumArtisanShard::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the overlapping actor is the player
    AVitrumMarble* Player = Cast<AVitrumMarble>(OtherActor);
    if (Player && ShardState == EShardState::Idle)
    {
        CollectShard();
    }
}

void AVitrumArtisanShard::InitializeShardProperties()
{
    // Set properties based on shard type
    switch (ShardType)
    {
    case EShardType::Standard:
        ShardName = TEXT("Standard Artisan Shard");
        ShardDescription = TEXT("A basic fragment of pure energy.");
        ShardValue = 1;
        ShardColor = FLinearColor(0.2f, 0.8f, 1.0f, 1.0f);
        break;

    case EShardType::Rare:
        ShardName = TEXT("Rare Artisan Shard");
        ShardDescription = TEXT("A rare fragment pulsing with enhanced power.");
        ShardValue = 3;
        ShardColor = FLinearColor(0.8f, 0.2f, 1.0f, 1.0f);
        PulseIntensity = 0.5f;
        break;

    case EShardType::Legendary:
        ShardName = TEXT("Legendary Artisan Shard");
        ShardDescription = TEXT("A legendary fragment of immense power.");
        ShardValue = 5;
        ShardColor = FLinearColor(1.0f, 0.8f, 0.2f, 1.0f);
        PulseIntensity = 0.7f;
        RotationSpeed = 120.0f;
        break;

    case EShardType::Master:
        ShardName = TEXT("Master Artisan Shard");
        ShardDescription = TEXT("The ultimate fragment of pure mastery.");
        ShardValue = 10;
        ShardColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
        PulseIntensity = 1.0f;
        RotationSpeed = 150.0f;
        BobSpeed = 3.0f;
        break;
    }

    // Set up unlocked abilities based on shard type
    UnlockedAbilities.Empty();
    switch (ShardType)
    {
    case EShardType::Standard:
        // No special abilities
        break;
    case EShardType::Rare:
        UnlockedAbilities.Add(FName(TEXT("EnhancedDash")));
        break;
    case EShardType::Legendary:
        UnlockedAbilities.Add(FName(TEXT("EnhancedDash")));
        UnlockedAbilities.Add(FName(TEXT("EnhancedTractionField")));
        break;
    case EShardType::Master:
        UnlockedAbilities.Add(FName(TEXT("EnhancedDash")));
        UnlockedAbilities.Add(FName(TEXT("EnhancedTractionField")));
        UnlockedAbilities.Add(FName(TEXT("TimeSlow")));
        break;
    }
}
