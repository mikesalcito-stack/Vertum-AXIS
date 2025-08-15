#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "VitrumEnergyEmitter.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumEnergyEmitter : public AActor
{
    GENERATED_BODY()

public:
    AVitrumEnergyEmitter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* EmitterMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UNiagaraComponent* EnergyFlowEffect;

    // Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
    FVector FlowDirection = FVector(1.0f, 0.0f, 0.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
    float FlowSpeed = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
    bool bIsActive = true;

    // Functions
    UFUNCTION(BlueprintCallable, Category = "Energy")
    void Activate();

    UFUNCTION(BlueprintCallable, Category = "Energy")
    void Deactivate();

    UFUNCTION(BlueprintCallable, Category = "Energy")
    void UpdateFlow();
};
