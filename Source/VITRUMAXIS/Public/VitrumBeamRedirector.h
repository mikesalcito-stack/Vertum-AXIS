#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "VitrumBeamRedirector.generated.h"

UENUM(BlueprintType)
enum class ERedirectorType : uint8
{
    Mirror UMETA(DisplayName = "Mirror"),
    Prism UMETA(DisplayName = "Prism")
};

UCLASS()
class VITRUMAXIS_API AVitrumBeamRedirector : public AActor
{
    GENERATED_BODY()

public:
    AVitrumBeamRedirector();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* RedirectorMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UNiagaraComponent* RedirectorEffect;

    // Redirector Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Redirector")
    ERedirectorType RedirectorType = ERedirectorType::Mirror;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Redirector")
    FVector ReflectionNormal = FVector(0.0f, 0.0f, 1.0f);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Redirector")
    TArray<FLinearColor> IncomingColors;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Redirector")
    FLinearColor MixedColor;

    // Functions
    UFUNCTION(BlueprintCallable, Category = "Redirector")
    void RedirectBeam(FLinearColor IncomingColor, FVector IncomingDirection);

private:
    // Helper functions
    void UpdateVisuals();
};
