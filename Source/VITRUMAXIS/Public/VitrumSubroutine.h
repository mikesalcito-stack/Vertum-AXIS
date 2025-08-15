#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VitrumSubroutine.generated.h"

UCLASS(Abstract)
class VITRUMAXIS_API AVitrumSubroutine : public AActor
{
    GENERATED_BODY()

public:
    AVitrumSubroutine();

    UFUNCTION(BlueprintCallable, Category = "Subroutine")
    virtual void Use(AVitrumMarble* User);
};
