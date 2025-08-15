#include "VitrumOverseer.h"
#include "VitrumMarble.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Engine/DamageEvents.h"

AVitrumOverseer::AVitrumOverseer()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->SetSimulatePhysics(true);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(CollisionSphere);

    AIControllerClass = AAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AVitrumOverseer::BeginPlay()
{
    Super::BeginPlay();

    // Setup BT
    AAIController* AICont = Cast<AAIController>(GetController());
    if (AICont && BehaviorTree)
    {
        AICont->RunBehaviorTree(BehaviorTree);
    }

    CollisionSphere->OnComponentHit.AddDynamic(this, &AVitrumOverseer::OnCollision);

    StartHunting();
}

void AVitrumOverseer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update target location in blackboard
    if (TargetMarble)
    {
        // Random burst
        if (FMath::RandBool() && 0.1f > FMath::FRand()) // 10% chance per tick
        {
            CollisionSphere->AddImpulse(GetActorForwardVector() * 500.0f); // Burst forward
        }

        // Evasion check: If player charging Spin Dash (assume blackboard key or distance)
        if (FVector::Dist(GetActorLocation(), TargetMarble->GetActorLocation()) < 100.0f && TargetMarble->GetCurrentCharge() > 0.5f)
        {
            // Dodge sideways
            FVector DodgeDir = FVector::CrossProduct(GetActorForwardVector(), FVector::UpVector);
            CollisionSphere->AddImpulse(DodgeDir * 1000.0f);
        }
    }
}

void AVitrumOverseer::StartHunting()
{
    TargetMarble = Cast<AVitrumMarble>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (TargetMarble) OnTargetAcquired();
}

void AVitrumOverseer::RamAttack()
{
    if (TargetMarble)
    {
        FVector Direction = (TargetMarble->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        CollisionSphere->AddImpulse(Direction * RamForce);
    }
}

void AVitrumOverseer::OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor == TargetMarble)
    {
        // Apply damage/knockback
        FDamageEvent DamageEvent;
        TargetMarble->TakeDamage(20.0f, DamageEvent, nullptr, this);
    }
    else if (OtherActor->ActorHasTag("Hazard"))
    {
        // Destroy self if rammed into hazard
        Destroy();
    }
}
