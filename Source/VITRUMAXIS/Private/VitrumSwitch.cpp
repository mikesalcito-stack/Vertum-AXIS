#include "VitrumSwitch.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "VitrumMarble.h"
#include "VitrumMovableBlock.h"
#include "VitrumGearSystem.h"
#include "VitrumPiston.h"
#include "VITRUMAXIS.h"

AVitrumSwitch::AVitrumSwitch()
{
    PrimaryActorTick.bCanEverTick = true;

    SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
    RootComponent = SwitchMesh;

    InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
    InteractionBox->SetupAttachment(SwitchMesh);

    Tags.Add(TEXT("PuzzleElement"));
    Tags.Add(TEXT("Rotatable"));
}

void AVitrumSwitch::BeginPlay()
{
    Super::BeginPlay();
    UpdateVisuals();
}

void AVitrumSwitch::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (SwitchType == ESwitchType::PressurePlate)
    {
        // Check weight
        float TotalWeight = 0.0f;
        TArray<AActor*> OverlappingActors;
        InteractionBox->GetOverlappingActors(OverlappingActors);

        for (AActor* Actor : OverlappingActors)
        {
            if (AVitrumMarble* Marble = Cast<AVitrumMarble>(Actor))
            {
                float Weight = 1.0f;
                if (Marble->CurrentState == EVitrumState::Metal) Weight = 3.0f;
                else if (Marble->CurrentState == EVitrumState::Acid) Weight = 1.5f;
                TotalWeight += Weight;
            }
            else if (AVitrumMovableBlock* Block = Cast<AVitrumMovableBlock>(Actor))
            {
                TotalWeight += 2.0f; // Assume block weight
            }
        }

        bool bShouldActivate = TotalWeight >= RequiredWeight;
        if (bShouldActivate != bIsActivated)
        {
            bIsActivated = bShouldActivate;
            NotifyLinkedActors(bIsActivated);
            UpdateVisuals();
            if (bIsActivated) OnActivated(); else OnDeactivated();
        }
    }
}

void AVitrumSwitch::Activate()
{
    if (!bIsActivated)
    {
        bIsActivated = true;
        NotifyLinkedActors(true);
        UpdateVisuals();
        OnActivated();
    }
}

void AVitrumSwitch::Deactivate()
{
    if (bIsActivated)
    {
        bIsActivated = false;
        NotifyLinkedActors(false);
        UpdateVisuals();
        OnDeactivated();
    }
}

void AVitrumSwitch::Toggle()
{
    if (SwitchType == ESwitchType::Toggle)
    {
        bIsActivated = !bIsActivated;
        NotifyLinkedActors(bIsActivated);
        UpdateVisuals();
        if (bIsActivated) OnActivated(); else OnDeactivated();
    }
}

void AVitrumSwitch::UpdateVisuals()
{
    // Update mesh/material based on state (implement in BP)
}

void AVitrumSwitch::NotifyLinkedActors(bool bActive)
{
    for (AActor* Linked : LinkedActors)
    {
        if (AVitrumGearSystem* Gear = Cast<AVitrumGearSystem>(Linked))
        {
            if (bActive) Gear->StartRotation(); else Gear->StopRotation();
        }
        else if (AVitrumPiston* Piston = Cast<AVitrumPiston>(Linked))
        {
            if (bActive) Piston->Extend(); else Piston->Retract();
        }
        // Add more types as needed
    }
}
