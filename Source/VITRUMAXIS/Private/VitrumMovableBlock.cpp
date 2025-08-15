#include "VitrumMovableBlock.h"
#include "VitrumMarble.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

AVitrumMovableBlock::AVitrumMovableBlock()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create block mesh
    BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
    RootComponent = BlockMesh;
    BlockMesh->SetSimulatePhysics(true);
    BlockMesh->SetEnableGravity(bGravityAffected);
    BlockMesh->SetLinearDamping(0.1f);
    BlockMesh->SetAngularDamping(0.1f);

    // Create physics constraint (optional, to constrain movement)
    PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
    PhysicsConstraint->SetupAttachment(BlockMesh);

    // Set default values
    CurrentVelocity = FVector::ZeroVector;
    bIsFalling = false;

    // Add tags for world manager
    Tags.Add(TEXT("MovableBlock"));
    Tags.Add(TEXT("Rotatable"));
    Tags.Add(TEXT("PuzzleElement"));
}

void AVitrumMovableBlock::BeginPlay()
{
    Super::BeginPlay();

    // Set up hit events
    BlockMesh->OnComponentHit.AddDynamic(this, &AVitrumMovableBlock::OnHit);
}

void AVitrumMovableBlock::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdatePhysics(DeltaTime);
}

void AVitrumMovableBlock::ApplyPushForce(FVector ForceDirection, float ForceMagnitude)
{
    if (bCanBePushed)
    {
        FVector Force = ForceDirection.GetSafeNormal() * ForceMagnitude * PushForce;
        BlockMesh->AddForce(Force);
        OnPushed();
    }
}

void AVitrumMovableBlock::UpdatePhysics(float DeltaTime)
{
    // Get current velocity
    CurrentVelocity = BlockMesh->GetPhysicsLinearVelocity();

    // Apply friction
    CurrentVelocity *= FMath::Pow(Friction, DeltaTime);
    BlockMesh->SetPhysicsLinearVelocity(CurrentVelocity);

    // Check if falling
    bool bWasFalling = bIsFalling;
    bIsFalling = CurrentVelocity.Z < -1.0f; // Threshold for falling

    if (bIsFalling && !bWasFalling)
    {
        OnFallen();
    }
}

void AVitrumMovableBlock::SetGravityEnabled(bool bEnabled)
{
    bGravityAffected = bEnabled;
    BlockMesh->SetEnableGravity(bEnabled);
}

void AVitrumMovableBlock::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (bIsFalling && Hit.ImpactNormal.Z > 0.5f) // Hit floor
    {
        bIsFalling = false;
        OnLanded();
    }

    // Check if hit by marble for pushing
    if (AVitrumMarble* Marble = Cast<AVitrumMarble>(OtherActor))
    {
        FVector PushDirection = (GetActorLocation() - Marble->GetActorLocation()).GetSafeNormal();
        float PushMagnitude = Marble->GetCurrentVelocity().Size() / Marble->MaxVelocity;
        ApplyPushForce(PushDirection, PushMagnitude);
    }
}
