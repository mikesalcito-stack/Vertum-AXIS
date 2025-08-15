#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "VitrumPiston.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumPiston : public AActor
{
    GENERATED_BODY()

public:
    AVitrumPiston();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piston")
    float ExtendLength = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piston")
    float ExtendSpeed = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Piston")
    bool bIsExtended = false;

    UFUNCTION(BlueprintCallable, Category = "Piston")
    void Extend();

    UFUNCTION(BlueprintCallable, Category = "Piston")
    void Retract();
};
