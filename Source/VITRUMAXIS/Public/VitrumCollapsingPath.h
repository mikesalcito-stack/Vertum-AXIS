#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "VitrumCollapsingPath.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumCollapsingPath : public AActor
{
    GENERATED_BODY()

public:
    AVitrumCollapsingPath();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* PathMesh;

    // Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collapse")
    float CollapseDelay = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collapse")
    float CollapseDuration = 5.0f;

    // Functions
    UFUNCTION(BlueprintCallable, Category = "Collapse")
    void StartCollapse();

    // Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnCollapseStarted();
};
