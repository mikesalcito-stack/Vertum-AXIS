#include "VitrumStateStation.h"
#include "VitrumMarble.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

AVitrumStateStation::AVitrumStateStation()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create station mesh
	StationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StationMesh"));
	StationMesh->SetupAttachment(RootComponent);
	StationMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create interaction box
	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetupAttachment(RootComponent);
	InteractionBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	InteractionBox->SetCollisionProfileName(TEXT("OverlapAll"));
	InteractionBox->SetGenerateOverlapEvents(true);

	// Create station effect
	StationEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("StationEffect"));
	StationEffect->SetupAttachment(RootComponent);

	// Create processing effect
	ProcessingEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProcessingEffect"));
	ProcessingEffect->SetupAttachment(RootComponent);

	// Set default values
	bIsActive = true;
	bIsProcessing = false;
	ProcessingTimer = 0.0f;
	CooldownTimer = 0.0f;
	ProcessingMarble = nullptr;

	// Add tags for world manager
	Tags.Add(TEXT("StateStation"));
	Tags.Add(TEXT("Rotatable"));

	if (StationType == EStationType::RepairChannel)
	{
		MoltenGlassEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MoltenGlassEffect"));
		MoltenGlassEffect->SetupAttachment(RootComponent);
	}
}

void AVitrumStateStation::BeginPlay()
{
	Super::BeginPlay();

	// Set up overlap events
	InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &AVitrumStateStation::OnBeginOverlap);
	InteractionBox->OnComponentEndOverlap.AddDynamic(this, &AVitrumStateStation::OnEndOverlap);

	// Set target state based on station type
	switch (StationType)
	{
	case EStationType::PlatingLine:
		TargetState = EVitrumState::Metal;
		break;
	case EStationType::AcidVat:
		TargetState = EVitrumState::Acid;
		break;
	case EStationType::RepairChannel:
		TargetState = EVitrumState::Glass;
		break;
	}

	// Initialize visual state
	UpdateVisuals(0.0f);
}

void AVitrumStateStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateProcessing(DeltaTime);
	UpdateVisuals(DeltaTime);
}

void AVitrumStateStation::ActivateStation()
{
	if (!bIsActive)
	{
		bIsActive = true;
		UpdateVisuals(0.0f);
		PlayActivationSound();
		OnStationActivated();
	}
}

void AVitrumStateStation::DeactivateStation()
{
	if (bIsActive)
	{
		bIsActive = false;
		UpdateVisuals(0.0f);
		OnStationDeactivated();
	}
}

void AVitrumStateStation::ProcessMarble(AVitrumMarble* Marble)
{
	if (CanProcessMarble(Marble))
	{
		bIsProcessing = true;
		ProcessingTimer = 0.0f;
		ProcessingMarble = Marble;

		PlayProcessingSound();
		OnProcessingStarted(Marble);
	}
}

bool AVitrumStateStation::CanProcessMarble(AVitrumMarble* Marble)
{
	if (!Marble || !bIsActive || bIsProcessing || CooldownTimer > 0.0f)
	{
		return false;
	}

	// Check if marble is in the correct state for processing
	switch (StationType)
	{
	case EStationType::PlatingLine:
		// Can only plate glass marbles
		return Marble->CurrentState == EVitrumState::Glass;
	case EStationType::AcidVat:
		// Can only acid-coat glass or metal marbles
		return Marble->CurrentState == EVitrumState::Glass || Marble->CurrentState == EVitrumState::Metal;
	case EStationType::RepairChannel:
		// Can repair any state back to glass
		return Marble->CurrentState != EVitrumState::Glass || Marble->Integrity < 100.0f;
	}

	return false;
}

void AVitrumStateStation::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AVitrumMarble* Marble = Cast<AVitrumMarble>(OtherActor))
	{
		if (!OverlappingMarbles.Contains(Marble))
		{
			OverlappingMarbles.Add(Marble);
		}

		// Start processing if possible
		if (CanProcessMarble(Marble))
		{
			ProcessMarble(Marble);
		}
	}
}

void AVitrumStateStation::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AVitrumMarble* Marble = Cast<AVitrumMarble>(OtherActor))
	{
		OverlappingMarbles.Remove(Marble);

		// Stop processing if this was the processing marble
		if (ProcessingMarble == Marble)
		{
			bIsProcessing = false;
			ProcessingMarble = nullptr;
		}
	}
}

void AVitrumStateStation::UpdateProcessing(float DeltaTime)
{
	if (bIsProcessing && ProcessingMarble)
	{
		ProcessingTimer += DeltaTime;

		// Check if processing is complete
		if (ProcessingTimer >= ProcessingTime)
		{
			CompleteProcessing();
		}
	}

	// Update cooldown
	if (CooldownTimer > 0.0f)
	{
		CooldownTimer -= DeltaTime;
	}
}

void AVitrumStateStation::UpdateVisuals(float DeltaTime)
{
	// Update station effect based on state
	if (StationEffect)
	{
		StationEffect->SetFloatParameter(TEXT("Active"), bIsActive ? 1.0f : 0.0f);
		StationEffect->SetFloatParameter(TEXT("Processing"), bIsProcessing ? 1.0f : 0.0f);
		StationEffect->SetFloatParameter(TEXT("Cooldown"), CooldownTimer > 0.0f ? 1.0f : 0.0f);

		// Set effect type based on station type
		switch (StationType)
		{
		case EStationType::PlatingLine:
			StationEffect->SetFloatParameter(TEXT("StationType"), 0.0f);
			break;
		case EStationType::AcidVat:
			StationEffect->SetFloatParameter(TEXT("StationType"), 1.0f);
			break;
		case EStationType::RepairChannel:
			StationEffect->SetFloatParameter(TEXT("StationType"), 2.0f);
			break;
		}
	}

	// Update processing effect
	if (ProcessingEffect)
	{
		if (bIsProcessing)
		{
			ProcessingEffect->SetFloatParameter(TEXT("Active"), 1.0f);
			ProcessingEffect->SetFloatParameter(TEXT("Progress"), ProcessingTimer / ProcessingTime);
		}
		else
		{
			ProcessingEffect->SetFloatParameter(TEXT("Active"), 0.0f);
		}
	}

	if (StationType == EStationType::RepairChannel && ProcessingMarble)
	{
		ProcessingMarble->RepairIntegrity();
		// Activate particles
		if (MoltenGlassEffect) MoltenGlassEffect->Activate();
	}
}

void AVitrumStateStation::CompleteProcessing()
{
	if (ProcessingMarble)
	{
		// Apply state change
		ProcessingMarble->ChangeState(TargetState);

		// Special handling for repair channel
		if (StationType == EStationType::RepairChannel)
		{
			ProcessingMarble->RepairIntegrity();
			// Activate particles
			if (MoltenGlassEffect) MoltenGlassEffect->Activate();
		}

		// Set up state duration timer for temporary states
		if (TargetState != EVitrumState::Glass && StateDuration > 0.0f)
		{
			// Set up timer to revert to glass state
			FTimerHandle StateTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				StateTimerHandle,
				FTimerDelegate::CreateLambda([this]()
				{
					if (ProcessingMarble && ProcessingMarble->IsValidLowLevel())
					{
						ProcessingMarble->ChangeState(EVitrumState::Glass);
					}
				}),
				StateDuration,
				false
			);
		}

		PlayCompletionSound();
		OnProcessingCompleted(ProcessingMarble);
	}

	// Reset processing state
	bIsProcessing = false;
	ProcessingMarble = nullptr;
	CooldownTimer = CooldownTime;

	FTimerHandle ParticleTimer;
	GetWorldTimerManager().SetTimer(ParticleTimer, [this]() {
		if (MoltenGlassEffect) MoltenGlassEffect->Deactivate();
	}, 2.0f, false);
}

void AVitrumStateStation::PlayActivationSound()
{
	if (ActivationSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ActivationSound, GetActorLocation());
	}
}

void AVitrumStateStation::PlayProcessingSound()
{
	if (ProcessingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ProcessingSound, GetActorLocation());
	}
}

void AVitrumStateStation::PlayCompletionSound()
{
	if (CompletionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CompletionSound, GetActorLocation());
	}
}
