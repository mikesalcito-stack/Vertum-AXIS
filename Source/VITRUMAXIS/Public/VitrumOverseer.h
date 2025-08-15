#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"
#include "VitrumMarble.h"
#include "VitrumOverseer.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumOverseer : public APawn
{
    GENERATED_BODY()

public:
    AVitrumOverseer();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UPathFollowingComponent* PathFollowing;

    // Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float ChaseSpeed = 1200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float RamForce = 2000.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    AVitrumMarble* TargetMarble;

    UPROPERTY(EditAnywhere, Category = "AI")
    class UBehaviorTree* BehaviorTree;

    // Functions
    UFUNCTION(BlueprintCallable, Category = "AI")
    void StartHunting();

    UFUNCTION(BlueprintCallable, Category = "AI")
    void RamAttack();

    // Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnTargetAcquired();

    UFUNCTION()
    void OnCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
