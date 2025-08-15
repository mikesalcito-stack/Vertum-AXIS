#include "VitrumBreachBomb.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/DestructibleActor.h"

AVitrumBreachBomb::AVitrumBreachBomb()
{
    // Constructor
}

void AVitrumBreachBomb::Use(AVitrumMarble* User)
{
    if (User)
    {
        // Place bomb at user's location or aimed location
        SetActorLocation(User->GetActorLocation() + User->GetActorForwardVector() * 50.0f);

        // Explode after delay or immediately
        UGameplayStatics::ApplyRadialDamage(GetWorld(), 100.0f, GetActorLocation(), ExplosionRadius, nullptr, {}, this, nullptr);

        // Find and destroy nearby destructibles
        TArray<FHitResult> HitResults;
        FCollisionShape Sphere = FCollisionShape::MakeSphere(ExplosionRadius);
        GetWorld()->SweepMultiByChannel(HitResults, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_Visibility, Sphere);

        for (FHitResult Hit : HitResults)
        {
            if (ADestructibleActor* Destructible = Cast<ADestructibleActor>(Hit.GetActor()))
            {
                if (Destructible->ActorHasTag(TEXT("CrackedWall")))
                {
                    Destructible->ApplyRadiusDamage(100.0f, Hit, ExplosionRadius, 100.0f, false);
                }
            }
        }

        Destroy();
    }
}
