#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "VitrumBeamEmitter.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumBeamEmitter : public AActor
{
    GENERATED_BODY()

public:
    AVitrumBeamEmitter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* EmitterMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UNiagaraComponent* BeamEffect;

    // Beam Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam")
    FVector BeamDirection = FVector(1.0f, 0.0f, 0.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam")
    float BeamLength = 10000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam")
    FLinearColor BeamColor = FLinearColor::Red;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beam")
    bool bIsActive = true;

    // Functions
    UFUNCTION(BlueprintCallable, Category = "Beam")
    void ActivateEmitter();

    UFUNCTION(BlueprintCallable, Category = "Beam")
    void DeactivateEmitter();

    UFUNCTION(BlueprintCallable, Category = "Beam")
    void UpdateBeam();

    // Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnBeamActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnBeamDeactivated();

private:
    // Helper functions
    void TraceBeam();
};
