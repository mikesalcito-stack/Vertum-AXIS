#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"
#include "VitrumMarble.h"
#include "VITRUMAXIS.h"
#include "VitrumStateStation.generated.h"

UCLASS()
class VITRUMAXIS_API AVitrumStateStation : public AActor
{
	GENERATED_BODY()

public:
	AVitrumStateStation();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StationMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* InteractionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* StationEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* ProcessingEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* MoltenGlassEffect; // Specific for RepairChannel

	// Station Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Station")
	EStationType StationType = EStationType::PlatingLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Station")
	bool bIsActive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Station")
	float ProcessingTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Station")
	float CooldownTime = 3.0f;

	// State Change Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EVitrumState TargetState = EVitrumState::Glass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float StateDuration = 30.0f;

	// Audio
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* ActivationSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* ProcessingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* CompletionSound;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Station")
	void ActivateStation();

	UFUNCTION(BlueprintCallable, Category = "Station")
	void DeactivateStation();

	UFUNCTION(BlueprintCallable, Category = "Station")
	void ProcessMarble(AVitrumMarble* Marble);

	UFUNCTION(BlueprintCallable, Category = "Station")
	bool CanProcessMarble(AVitrumMarble* Marble);

	// Events
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnStationActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnStationDeactivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnProcessingStarted(AVitrumMarble* Marble);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnProcessingCompleted(AVitrumMarble* Marble);

	// Overlap Events
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	// Internal state
	bool bIsProcessing = false;
	float ProcessingTimer = 0.0f;
	float CooldownTimer = 0.0f;
	AVitrumMarble* ProcessingMarble = nullptr;

	// Overlapping marbles
	TArray<AVitrumMarble*> OverlappingMarbles;

	// Helper functions
	void UpdateProcessing(float DeltaTime);
	void UpdateVisuals(float DeltaTime);
	void PlayActivationSound();
	void PlayProcessingSound();
	void PlayCompletionSound();
	void CompleteProcessing();
};
