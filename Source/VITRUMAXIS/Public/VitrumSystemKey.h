#pragma once

#include "CoreMinimal.h"
#include "VitrumSubroutine.h"
#include "VitrumSystemKey.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumSystemKey : public AVitrumSubroutine
{
    GENERATED_BODY()

public:
    AVitrumSystemKey();

    virtual void Use(AVitrumMarble* User) override;

    // Key properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
    FName KeyID;
};
