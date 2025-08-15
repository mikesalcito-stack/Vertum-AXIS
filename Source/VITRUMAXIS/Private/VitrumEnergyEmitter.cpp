#include "VitrumEnergyEmitter.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "VitrumEnergyRedirector.h"
#include "VitrumEnergyReceptor.h"

AVitrumEnergyEmitter::AVitrumEnergyEmitter()
{
    PrimaryActorTick.bCanEverTick = true;

    EmitterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EmitterMesh"));
    RootComponent = EmitterMesh;

    EnergyFlowEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EnergyFlowEffect"));
    EnergyFlowEffect->SetupAttachment(EmitterMesh);

    Tags.Add(TEXT("PuzzleElement"));
    Tags.Add(TEXT("Rotatable"));
}

void AVitrumEnergyEmitter::BeginPlay()
{
    Super::BeginPlay();
    UpdateFlow();
}

void AVitrumEnergyEmitter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsActive) UpdateFlow();
}

void AVitrumEnergyEmitter::Activate()
{
    bIsActive = true;
}

void AVitrumEnergyEmitter::Deactivate()
{
    bIsActive = false;
    if (EnergyFlowEffect) EnergyFlowEffect->Deactivate();
}

void AVitrumEnergyEmitter::UpdateFlow()
{
    if (!bIsActive) return;

    FVector Start = GetActorLocation();
    FVector End = Start + FlowDirection * 10000.0f;

    FHitResult Hit;
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
    {
        if (AVitrumEnergyRedirector* Redirector = Cast<AVitrumEnergyRedirector>(Hit.GetActor()))
        {
            Redirector->RedirectFlow(Hit.ImpactNormal);
        }
        else if (AVitrumEnergyReceptor* Receptor = Cast<AVitrumEnergyReceptor>(Hit.GetActor()))
        {
            Receptor->ReceiveFlow();
        }

        // Update Niagara endpoint for fluid sim
        EnergyFlowEffect->SetVectorParameter(TEXT("EndPoint"), Hit.ImpactPoint);
    }
    else
    {
        EnergyFlowEffect->SetVectorParameter(TEXT("EndPoint"), End);
    }

    EnergyFlowEffect->SetNiagaraVariableVec3("FlowDirection", FlowDirection);
    EnergyFlowEffect->SetNiagaraVariableFloat("FlowSpeed", FlowSpeed);

    EnergyFlowEffect->SetFloatParameter(TEXT("FlowSpeed"), FlowSpeed);
}
