#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "VitrumMovableBlock.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumMovableBlock : public AActor
{
	GENERATED_BODY()

public:
	AVitrumMovableBlock();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BlockMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* PhysicsConstraint;

	// Movement Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float PushForce = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Friction = 0.98f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bCanBePushed = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bGravityAffected = true;

	// State Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	FVector CurrentVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsFalling = false;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ApplyPushForce(FVector ForceDirection, float ForceMagnitude);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void UpdatePhysics(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetGravityEnabled(bool bEnabled);

	// Events
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnPushed();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnFallen();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnLanded();

	// Collision Events
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
