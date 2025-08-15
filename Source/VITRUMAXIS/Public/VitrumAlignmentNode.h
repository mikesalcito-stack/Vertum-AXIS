#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "VitrumAlignmentNode.generated.h"

UENUM(BlueprintType)
enum class ENodeSymbol : uint8
{
    Circle UMETA(DisplayName = "Circle"),
    Square UMETA(DisplayName = "Square"),
    Triangle UMETA(DisplayName = "Triangle"),
    Diamond UMETA(DisplayName = "Diamond"),
    Star UMETA(DisplayName = "Star"),
    Cross UMETA(DisplayName = "Cross"),
    Hexagon UMETA(DisplayName = "Hexagon"),
    Octagon UMETA(DisplayName = "Octagon")
};

UENUM(BlueprintType)
enum class ENodeState : uint8
{
    Inactive UMETA(DisplayName = "Inactive"),
    Active UMETA(DisplayName = "Active"),
    Linked UMETA(DisplayName = "Linked"),
    Completed UMETA(DisplayName = "Completed")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNodeActivated, AVitrumAlignmentNode*, Node, ENodeSymbol, Symbol);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNodeLinked, AVitrumAlignmentNode*, Node1, AVitrumAlignmentNode*, Node2);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNodeCompleted, AVitrumAlignmentNode*, Node);

UCLASS(BlueprintType, Blueprintable)
class VITRUMAXIS_API AVitrumAlignmentNode : public AActor
{
    GENERATED_BODY()

public:
    AVitrumAlignmentNode();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    // Core Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* NodeMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* SymbolMesh;

    // Node Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    ENodeSymbol NodeSymbol;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    ENodeState NodeState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    bool bIsFirstInPair;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    AVitrumAlignmentNode* PairedNode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    float ActivationRadius = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    float PulseSpeed = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
    float PulseIntensity = 0.3f;

    // Visual Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FLinearColor InactiveColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FLinearColor ActiveColor = FLinearColor(0.8f, 0.8f, 0.2f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FLinearColor LinkedColor = FLinearColor(0.2f, 0.8f, 0.2f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FLinearColor CompletedColor = FLinearColor(0.8f, 0.8f, 0.8f, 1.0f);

    // Audio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* ActivationSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* LinkSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* CompletionSound;

    // Visual Effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* ActivationParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* LinkParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UParticleSystem* CompletionParticles;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnNodeActivated OnNodeActivated;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnNodeLinked OnNodeLinked;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnNodeCompleted OnNodeCompleted;

    // Public Functions
    UFUNCTION(BlueprintCallable, Category = "Node")
    void ActivateNode();

    UFUNCTION(BlueprintCallable, Category = "Node")
    void LinkNode(AVitrumAlignmentNode* OtherNode);

    UFUNCTION(BlueprintCallable, Category = "Node")
    void CompleteNode();

    UFUNCTION(BlueprintCallable, Category = "Node")
    void ResetNode();

    UFUNCTION(BlueprintPure, Category = "Node")
    bool CanActivate() const;

    UFUNCTION(BlueprintPure, Category = "Node")
    bool IsLinked() const { return NodeState == ENodeState::Linked || NodeState == ENodeState::Completed; }

    UFUNCTION(BlueprintPure, Category = "Node")
    bool IsCompleted() const { return NodeState == ENodeState::Completed; }

protected:
    // Internal Functions
    void UpdateVisuals(float DeltaTime);
    void UpdatePulse(float DeltaTime);
    void PlayActivationEffects();
    void PlayLinkEffects();
    void PlayCompletionEffects();
    void UpdateMaterialColor(FLinearColor Color);

private:
    // Internal State
    UPROPERTY()
    UMaterialInstanceDynamic* NodeMaterial;

    UPROPERTY()
    UMaterialInstanceDynamic* SymbolMaterial;

    float PulseTimer;
    float CurrentPulseIntensity;
    bool bIsPulsing;
};
