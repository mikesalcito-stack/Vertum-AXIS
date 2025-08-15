#include "VitrumHazard.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "VitrumMarble.h"

AVitrumHazard::AVitrumHazard()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create root component
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootScene;

    // Create hazard mesh
    HazardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HazardMesh"));
    HazardMesh->SetupAttachment(RootScene);
    HazardMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Create trigger sphere
    TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
    TriggerSphere->SetupAttachment(RootScene);
    TriggerSphere->SetSphereRadius(100.0f);
    TriggerSphere->SetCollisionProfileName(TEXT("OverlapAll"));

    // Set default values
    HazardType = EHazardType::AcidPool;
    HazardState = EHazardState::Inactive;
    DamageTimer = 0.0f;
    PulseTimer = 0.0f;
    CurrentPulseIntensity = 0.0f;
    bIsPulsing = false;
    InitialLocation = FVector::ZeroVector;
    TargetPlayer = nullptr;
    HazardMaterial = nullptr;
}

void AVitrumHazard::BeginPlay()
{
    Super::BeginPlay();

    // Set up mesh based on hazard type
    if (HazardMesh)
    {
        switch (HazardType)
        {
            case EHazardType::AcidPool:
            {
                // Use cylinder for acid pool
                static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("/Engine/BasicShapes/Cylinder"));
                if (CylinderMeshAsset.Succeeded())
                {
                    HazardMesh->SetStaticMesh(CylinderMeshAsset.Object);
                }
                HazardMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 0.1f));
                break;
            }
            case EHazardType::Stalker:
            {
                // Use sphere for stalker
                static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
                if (SphereMeshAsset.Succeeded())
                {
                    HazardMesh->SetStaticMesh(SphereMeshAsset.Object);
                }
                HazardMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
                break;
            }
            case EHazardType::NullZone:
            {
                // Use box for null zone
                static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
                if (BoxMeshAsset.Succeeded())
                {
                    HazardMesh->SetStaticMesh(BoxMeshAsset.Object);
                }
                HazardMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 0.1f));
                break;
            }
        }

        // Create dynamic material
        if (UMaterialInterface* BaseMaterial = HazardMesh->GetMaterial(0))
        {
            HazardMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
            HazardMesh->SetMaterial(0, HazardMaterial);
        }
    }

    // Set up collision events
    TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AVitrumHazard::OnBeginOverlap);
    TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &AVitrumHazard::OnEndOverlap);

    // Store initial location
    InitialLocation = GetActorLocation();

    // Set initial visual state
    UpdateMaterialColor(GetColorForHazardType());
}

void AVitrumHazard::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateVisuals(DeltaTime);
    UpdateMovement(DeltaTime);
    UpdatePulse(DeltaTime);
}

void AVitrumHazard::ActivateHazard()
{
    if (HazardState == EHazardState::Inactive)
    {
        HazardState = EHazardState::Active;
        PlayAmbientEffects();
    }
}

void AVitrumHazard::DeactivateHazard()
{
    if (HazardState == EHazardState::Active)
    {
        HazardState = EHazardState::Inactive;
        TargetPlayer = nullptr;
    }
}

void AVitrumHazard::TriggerHazard()
{
    if (HazardState == EHazardState::Active)
    {
        HazardState = EHazardState::Triggered;
        PlayTriggerEffects();
        OnHazardTriggered.Broadcast(this);
    }
}

void AVitrumHazard::UpdateVisuals(float DeltaTime)
{
    FLinearColor TargetColor = GetColorForHazardType();
    UpdateMaterialColor(TargetColor);
}

void AVitrumHazard::UpdateMovement(float DeltaTime)
{
    if (HazardType == EHazardType::Stalker && HazardState == EHazardState::Active)
    {
        MoveTowardsPlayer(DeltaTime);
    }
}

void AVitrumHazard::UpdatePulse(float DeltaTime)
{
    if (HazardState == EHazardState::Active)
    {
        PulseTimer += DeltaTime * PulseSpeed;
        CurrentPulseIntensity = FMath::Sin(PulseTimer) * PulseIntensity;
        bIsPulsing = true;

        // Apply pulse to material
        if (HazardMaterial)
        {
            HazardMaterial->SetScalarParameterValue("PulseIntensity", CurrentPulseIntensity);
        }
    }
    else
    {
        bIsPulsing = false;
        CurrentPulseIntensity = 0.0f;

        if (HazardMaterial)
        {
            HazardMaterial->SetScalarParameterValue("PulseIntensity", 0.0f);
        }
    }
}

void AVitrumHazard::PlayTriggerEffects()
{
    if (TriggerSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, TriggerSound, GetActorLocation());
    }

    if (TriggerParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TriggerParticles, GetActorLocation(), GetActorRotation());
    }
}

void AVitrumHazard::PlayDamageEffects()
{
    if (DamageSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetActorLocation());
    }

    if (DamageParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamageParticles, GetActorLocation(), GetActorRotation());
    }
}

void AVitrumHazard::PlayAmbientEffects()
{
    if (AmbientParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AmbientParticles, GetActorLocation(), GetActorRotation());
    }
}

void AVitrumHazard::UpdateMaterialColor(FLinearColor Color)
{
    if (HazardMaterial)
    {
        HazardMaterial->SetVectorParameterValue("BaseColor", Color);
    }
}

void AVitrumHazard::MoveTowardsPlayer(float DeltaTime)
{
    if (!TargetPlayer)
    {
        // Find player
        TargetPlayer = Cast<AVitrumMarble>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
        if (!TargetPlayer)
        {
            return;
        }
    }

    FVector PlayerLocation = TargetPlayer->GetActorLocation();
    FVector HazardLocation = GetActorLocation();
    float DistanceToPlayer = FVector::Dist(PlayerLocation, HazardLocation);

    // Check if player is in detection range
    if (DistanceToPlayer <= DetectionRadius)
    {
        FVector Direction = (PlayerLocation - HazardLocation).GetSafeNormal();
        
        // Move towards player
        FVector NewLocation = HazardLocation + Direction * MovementSpeed * DeltaTime;
        SetActorLocation(NewLocation);

        // Check if close enough to damage
        if (DistanceToPlayer <= ChaseRadius)
        {
            DamageTimer += DeltaTime;
            if (DamageTimer >= DamageInterval)
            {
                DamageTimer = 0.0f;
                
                // Deal damage to player
                if (TargetPlayer)
                {
                    TargetPlayer->TakeDamage(DamageAmount);
                    PlayDamageEffects();
                }
            }
        }
    }
}

FLinearColor AVitrumHazard::GetColorForHazardType()
{
    switch (HazardType)
    {
        case EHazardType::AcidPool:
            return AcidColor;
        case EHazardType::Stalker:
            return StalkerColor;
        case EHazardType::NullZone:
            return NullZoneColor;
        default:
            return FLinearColor::White;
    }
}

void AVitrumHazard::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AVitrumMarble* Marble = Cast<AVitrumMarble>(OtherActor))
    {
        if (HazardState == EHazardState::Active)
        {
            // Deal immediate damage
            Marble->TakeDamage(DamageAmount);
            PlayDamageEffects();

            // For lethal hazards, trigger immediately
            if (bIsLethal)
            {
                TriggerHazard();
            }
        }
    }
}

void AVitrumHazard::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Optional: Handle when player leaves the hazard area
}
