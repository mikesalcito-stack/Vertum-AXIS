#pragma once

#include "CoreMinimal.h"
#include "VitrumSubroutine.h"
#include "VitrumBreachBomb.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumBreachBomb : public AVitrumSubroutine
{
    GENERATED_BODY()

public:
    AVitrumBreachBomb();

    virtual void Use(AVitrumMarble* User) override;

    // Bomb properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
    float ExplosionRadius = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
    float ExplosionForce = 10000.0f;
};
