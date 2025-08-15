#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VitrumConductivityPuzzle.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumConductivityPuzzle : public AActor
{
    GENERATED_BODY()

public:
    AVitrumConductivityPuzzle();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Circuit")
    bool bIsComplete = false;

    UFUNCTION(BlueprintCallable, Category = "Circuit")
    void CheckCircuitCompletion(AVitrumMarble* Marble);
};
