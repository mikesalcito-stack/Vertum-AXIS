#include "VitrumMarble.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "VitrumGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "InputActionValue.h"

AVitrumMarble::AVitrumMarble()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create root component
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->SetSphereRadius(50.0f);
    CollisionSphere->SetCollisionProfileName(TEXT("Pawn"));
    CollisionSphere->SetSimulatePhysics(true);
    CollisionSphere->SetEnableGravity(true);
    CollisionSphere->SetLinearDamping(0.1f);
    CollisionSphere->SetAngularDamping(0.1f);

    // Create mesh component
    MarbleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MarbleMesh"));
    MarbleMesh->SetupAttachment(CollisionSphere);
    MarbleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MarbleMesh->SetRelativeLocation(FVector::ZeroVector);

    // Create spring arm for camera
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(CollisionSphere);
    SpringArm->TargetArmLength = 800.0f;
    SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
    SpringArm->bDoCollisionTest = false;
    SpringArm->bEnableCameraLag = true;
    SpringArm->CameraLagSpeed = 3.0f;

    // Create camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    // Create audio components
    RollAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("RollAudio"));
    RollAudioComponent->SetupAttachment(CollisionSphere);
    RollAudioComponent->bAutoActivate = false;

    EffectsAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EffectsAudio"));
    EffectsAudioComponent->SetupAttachment(CollisionSphere);
    EffectsAudioComponent->bAutoActivate = false;

    // Set default values
    CurrentState = EVitrumState::Solid;
    Integrity = MaxIntegrity;
    CurrentEnergy = MaxEnergy;
    bIsDashing = false;
    bTractionFieldActive = false;
    DashTimer = 0.0f;
    RollSoundTimer = 0.0f;
    bIsGrounded = false;
    bWasGrounded = false;
    LastCheckpoint = FVector::ZeroVector;
}

void AVitrumMarble::BeginPlay()
{
    Super::BeginPlay();

    // Set initial checkpoint
    LastCheckpoint = GetActorLocation();

    // Set up physics properties
    if (CollisionSphere)
    {
        CollisionSphere->SetMassOverrideInKg(NAME_None, Mass);
        CollisionSphere->SetPhysicsLinearVelocity(FVector::ZeroVector);
        CollisionSphere->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
    }

    // Set up mesh
    if (MarbleMesh)
    {
        // Load default sphere mesh if not set
        static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
        if (SphereMeshAsset.Succeeded())
        {
            MarbleMesh->SetStaticMesh(SphereMeshAsset.Object);
        }
        MarbleMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
    }
}

void AVitrumMarble::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdatePhysics(DeltaTime);
    UpdateEnergy(DeltaTime);
    UpdateAudio();
    UpdateVisualEffects();

    // Update grounded state
    bWasGrounded = bIsGrounded;
    bIsGrounded = false;

    // Simple ground check
    FVector Start = GetActorLocation();
    FVector End = Start - FVector(0, 0, 60.0f);
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
    {
        bIsGrounded = true;
    }
}

void AVitrumMarble::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind movement inputs
    PlayerInputComponent->BindAxis("MoveForward", this, &AVitrumMarble::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AVitrumMarble::MoveRight);

    // Bind action inputs
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AVitrumMarble::Jump);
    PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AVitrumMarble::Dash);
    PlayerInputComponent->BindAction("TractionField", IE_Pressed, this, &AVitrumMarble::ActivateTractionField);
    PlayerInputComponent->BindAction("TractionField", IE_Released, this, &AVitrumMarble::DeactivateTractionField);
}

void AVitrumMarble::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        FVector Direction = Camera->GetForwardVector();
        Direction.Z = 0.0f;
        Direction.Normalize();
        MovementInput.X = Value;
        Move(Direction * Value);
    }
    else
    {
        MovementInput.X = 0.0f;
    }
}

void AVitrumMarble::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        FVector Direction = Camera->GetRightVector();
        Direction.Z = 0.0f;
        Direction.Normalize();
        MovementInput.Y = Value;
        Move(Direction * Value);
    }
    else
    {
        MovementInput.Y = 0.0f;
    }
}

void AVitrumMarble::MoveFromInputAction(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (MovementVector.Y != 0.0f)
    {
        FVector Direction = Camera->GetForwardVector();
        Direction.Z = 0.0f;
        Direction.Normalize();
        Move(Direction * MovementVector.Y);
    }

    if (MovementVector.X != 0.0f)
    {
        FVector Direction = Camera->GetRightVector();
        Direction.Z = 0.0f;
        Direction.Normalize();
        Move(Direction * MovementVector.X);
    }
}

void AVitrumMarble::Move(const FVector& Direction)
{
    if (!bIsDashing)
    {
        float ForceMultiplier = bIsGrounded ? 1.0f : AirControl;
        ApplyMovementForce(Direction, MovementForce * ForceMultiplier);
    }
}

void AVitrumMarble::Jump()
{
    if (bIsGrounded && !bJumpPressed)
    {
        bJumpPressed = true;
        FVector JumpDirection = FVector(0, 0, 1);
        ApplyMovementForce(JumpDirection, JumpForce);

        if (JumpSound)
        {
            EffectsAudioComponent->SetSound(JumpSound);
            EffectsAudioComponent->Play();
        }
    }
}

void AVitrumMarble::Dash()
{
    if (!bIsDashing && CurrentEnergy >= 25.0f && !bDashPressed)
    {
        bDashPressed = true;
        bIsDashing = true;
        DashTimer = DashDuration;
        CurrentEnergy -= 25.0f;

        // Apply dash force in movement direction
        FVector DashDirection = MovementInput;
        if (DashDirection.IsNearlyZero())
        {
            DashDirection = Camera->GetForwardVector();
            DashDirection.Z = 0.0f;
        }
        DashDirection.Normalize();
        ApplyMovementForce(DashDirection, DashForce);

        if (DashSound)
        {
            EffectsAudioComponent->SetSound(DashSound);
            EffectsAudioComponent->Play();
        }

        if (DashParticles)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DashParticles, GetActorLocation(), GetActorRotation());
        }
    }
}

void AVitrumMarble::ActivateTractionField()
{
    if (!bTractionFieldActive && CurrentEnergy >= 10.0f && !bTractionFieldPressed)
    {
        bTractionFieldPressed = true;
        bTractionFieldActive = true;
    }
}

void AVitrumMarble::DeactivateTractionField()
{
    bTractionFieldPressed = false;
    bTractionFieldActive = false;
}

float AVitrumMarble::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Integrity = FMath::Max(0.0f, Integrity - Damage);

    // Update state based on integrity
    if (Integrity <= 0.0f)
    {
        CurrentState = EVitrumState::Damaged;
        Shatter();
    }
    else if (Integrity <= CriticalThreshold)
    {
        CurrentState = EVitrumState::Damaged;
    }
    else if (Integrity <= DamageThreshold)
    {
        CurrentState = EVitrumState::Damaged;
    }
    else
    {
        CurrentState = EVitrumState::Solid;
    }

    // Play damage sound and effects
    if (DamageSound)
    {
        EffectsAudioComponent->SetSound(DamageSound);
        EffectsAudioComponent->Play();
    }

    if (DamageParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamageParticles, GetActorLocation(), GetActorRotation());
    }

    OnStateChanged.Broadcast(CurrentState);
    return Damage;
}

void AVitrumMarble::Heal(float HealAmount)
{
    Integrity = FMath::Min(MaxIntegrity, Integrity + HealAmount);

    // Update state
    if (Integrity > DamageThreshold)
    {
        CurrentState = EVitrumState::Solid;
    }
    else if (Integrity > CriticalThreshold)
    {
        CurrentState = EVitrumState::Damaged;
    }

    if (HealSound)
    {
        EffectsAudioComponent->SetSound(HealSound);
        EffectsAudioComponent->Play();
    }

    if (HealParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HealParticles, GetActorLocation(), GetActorRotation());
    }

    OnStateChanged.Broadcast(CurrentState);
}

void AVitrumMarble::Shatter()
{
    if (ShatterSound)
    {
        EffectsAudioComponent->SetSound(ShatterSound);
        EffectsAudioComponent->Play();
    }

    if (ShatterParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShatterParticles, GetActorLocation(), GetActorRotation());
    }

    // Respawn after a short delay
    FTimerHandle RespawnTimer;
    GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AVitrumMarble::Respawn, 1.0f, false);
}

void AVitrumMarble::Respawn()
{
    SetActorLocation(LastCheckpoint);
    SetActorRotation(FRotator::ZeroRotator);
    
    if (CollisionSphere)
    {
        CollisionSphere->SetPhysicsLinearVelocity(FVector::ZeroVector);
        CollisionSphere->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
    }

    // Reset state
    Integrity = MaxIntegrity;
    CurrentState = EVitrumState::Solid;
    bIsDashing = false;
    bTractionFieldActive = false;
    DashTimer = 0.0f;

    OnStateChanged.Broadcast(CurrentState);
}

void AVitrumMarble::SetCheckpoint(FVector Location)
{
    LastCheckpoint = Location;
}

void AVitrumMarble::UpdatePhysics(float DeltaTime)
{
    if (CollisionSphere)
    {
        // Update dash timer
        if (bIsDashing)
        {
            DashTimer -= DeltaTime;
            if (DashTimer <= 0.0f)
            {
                bIsDashing = false;
                bDashPressed = false;
            }
        }

        // Apply traction field
        if (bTractionFieldActive && CurrentEnergy > 0.0f)
        {
            FVector UpDirection = FVector(0, 0, 1);
            ApplyMovementForce(UpDirection, TractionFieldForce);
            CurrentEnergy -= 15.0f * DeltaTime; // Drain energy while active
        }

        // Clamp velocity
        ClampVelocity();

        // Store last velocity for audio
        LastVelocity = CollisionSphere->GetPhysicsLinearVelocity();
    }
}

void AVitrumMarble::UpdateEnergy(float DeltaTime)
{
    if (!bTractionFieldActive)
    {
        CurrentEnergy = FMath::Min(MaxEnergy, CurrentEnergy + EnergyRegenRate * DeltaTime);
    }
}

void AVitrumMarble::UpdateAudio()
{
    if (CollisionSphere)
    {
        FVector CurrentVelocity = CollisionSphere->GetPhysicsLinearVelocity();
        float Speed = CurrentVelocity.Size();

        // Play roll sound based on speed
        if (Speed > 100.0f && bIsGrounded)
        {
            PlayRollSound();
        }
        else
        {
            StopRollSound();
        }
    }
}

void AVitrumMarble::UpdateVisualEffects()
{
    // Update mesh appearance based on state
    if (MarbleMesh)
    {
        FLinearColor Color = FLinearColor::White;
        
        switch (CurrentState)
        {
            case EVitrumState::Glass:
                Color = FLinearColor(0.8f, 0.9f, 1.0f, 0.8f); // Light blue, semi-transparent
                break;
            case EVitrumState::Molten:
                Color = FLinearColor(1.0f, 0.3f, 0.0f, 1.0f); // Orange-red
                break;
            case EVitrumState::Solid:
                Color = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f); // Dark gray
                break;
            case EVitrumState::Plated:
                Color = FLinearColor(0.7f, 0.7f, 0.9f, 1.0f); // Light metallic
                break;
            case EVitrumState::Damaged:
                Color = FLinearColor(0.8f, 0.4f, 0.0f, 1.0f); // Orange
                break;
            case EVitrumState::Metal:
                Color = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f); // Metallic gray
                break;
            case EVitrumState::Acid:
                Color = FLinearColor(0.0f, 0.8f, 0.0f, 1.0f); // Green
                break;
        }

        // Apply material color (this would need a dynamic material instance in practice)
        // For now, we'll just update the mesh visibility
        MarbleMesh->SetVisibility(CurrentState != EVitrumState::Damaged);
    }
}

void AVitrumMarble::ApplyMovementForce(const FVector& Direction, float Force)
{
    if (CollisionSphere)
    {
        CollisionSphere->AddForce(Direction * Force);
    }
}

void AVitrumMarble::ClampVelocity()
{
    if (CollisionSphere)
    {
        FVector CurrentVelocity = CollisionSphere->GetPhysicsLinearVelocity();
        if (CurrentVelocity.Size() > MaxVelocity)
        {
            CurrentVelocity = CurrentVelocity.GetSafeNormal() * MaxVelocity;
            CollisionSphere->SetPhysicsLinearVelocity(CurrentVelocity);
        }
    }
}

void AVitrumMarble::PlayRollSound()
{
    if (RollSound && RollAudioComponent)
    {
        if (!RollAudioComponent->IsPlaying())
        {
            RollAudioComponent->SetSound(RollSound);
            RollAudioComponent->Play();
        }

        // Adjust volume based on speed
        FVector CurrentVelocity = CollisionSphere->GetPhysicsLinearVelocity();
        float Speed = CurrentVelocity.Size();
        float Volume = FMath::Clamp(Speed / 1000.0f, 0.1f, 1.0f);
        RollAudioComponent->SetVolumeMultiplier(Volume);
    }
}

void AVitrumMarble::StopRollSound()
{
    if (RollAudioComponent && RollAudioComponent->IsPlaying())
    {
        RollAudioComponent->Stop();
    }
}

FVector AVitrumMarble::GetCurrentVelocity() const
{
    if (CollisionSphere)
    {
        return CollisionSphere->GetPhysicsLinearVelocity();
    }
    return FVector::ZeroVector;
}
