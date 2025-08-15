#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "VitrumBeamReceptor.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumBeamReceptor : public AActor
{
    GENERATED_BODY()

public:
    AVitrumBeamReceptor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* ReceptorMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UNiagaraComponent* ReceptorEffect;

    // Receptor Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Receptor")
    FLinearColor RequiredColor = FLinearColor::Red;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Receptor")
    bool bIsActivated = false;

    // Functions
    UFUNCTION(BlueprintCallable, Category = "Receptor")
    void ReceiveBeam(FLinearColor IncomingColor);

    UFUNCTION(BlueprintCallable, Category = "Receptor")
    void Deactivate();

    // Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnActivated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnDeactivated();

private:
    // Helper functions
    void UpdateVisuals();
};
