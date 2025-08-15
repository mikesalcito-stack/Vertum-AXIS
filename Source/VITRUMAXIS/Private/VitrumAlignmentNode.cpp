#include "VitrumAlignmentNode.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "VitrumMarble.h"

AVitrumAlignmentNode::AVitrumAlignmentNode()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create root component
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->SetSphereRadius(50.0f);
    CollisionSphere->SetCollisionProfileName(TEXT("OverlapAll"));

    // Create node mesh
    NodeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NodeMesh"));
    NodeMesh->SetupAttachment(CollisionSphere);
    NodeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Create symbol mesh
    SymbolMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SymbolMesh"));
    SymbolMesh->SetupAttachment(NodeMesh);
    SymbolMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SymbolMesh->SetRelativeLocation(FVector(0, 0, 60.0f));

    // Set default values
    NodeSymbol = ENodeSymbol::Circle;
    NodeState = ENodeState::Inactive;
    bIsFirstInPair = false;
    PairedNode = nullptr;
    PulseTimer = 0.0f;
    CurrentPulseIntensity = 0.0f;
    bIsPulsing = false;
    NodeMaterial = nullptr;
    SymbolMaterial = nullptr;
}

void AVitrumAlignmentNode::BeginPlay()
{
    Super::BeginPlay();

    // Set up meshes
    if (NodeMesh)
    {
        // Load default sphere mesh if not set
        static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
        if (SphereMeshAsset.Succeeded())
        {
            NodeMesh->SetStaticMesh(SphereMeshAsset.Object);
        }
        NodeMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

        // Create dynamic material
        if (UMaterialInterface* BaseMaterial = NodeMesh->GetMaterial(0))
        {
            NodeMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
            NodeMesh->SetMaterial(0, NodeMaterial);
        }
    }

    if (SymbolMesh)
    {
        // Load default cylinder mesh for symbol
        static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("/Engine/BasicShapes/Cylinder"));
        if (CylinderMeshAsset.Succeeded())
        {
            SymbolMesh->SetStaticMesh(CylinderMeshAsset.Object);
        }
        SymbolMesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.1f));

        // Create dynamic material for symbol
        if (UMaterialInterface* BaseMaterial = SymbolMesh->GetMaterial(0))
        {
            SymbolMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
            SymbolMesh->SetMaterial(0, SymbolMaterial);
        }
    }

    // Set up collision events
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AVitrumAlignmentNode::OnBeginOverlap);
    CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AVitrumAlignmentNode::OnEndOverlap);

    // Set initial visual state
    UpdateMaterialColor(InactiveColor);
}

void AVitrumAlignmentNode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateVisuals(DeltaTime);
    UpdatePulse(DeltaTime);
}

void AVitrumAlignmentNode::ActivateNode()
{
    if (CanActivate())
    {
        NodeState = ENodeState::Active;
        PlayActivationEffects();
        OnNodeActivated.Broadcast(this, NodeSymbol);

        // Check if we can link with paired node
        if (PairedNode && PairedNode->NodeState == ENodeState::Active)
        {
            LinkNode(PairedNode);
        }
    }
}

void AVitrumAlignmentNode::LinkNode(AVitrumAlignmentNode* OtherNode)
{
    if (OtherNode && NodeState == ENodeState::Active && OtherNode->NodeState == ENodeState::Active)
    {
        NodeState = ENodeState::Linked;
        OtherNode->NodeState = ENodeState::Linked;

        PlayLinkEffects();
        OtherNode->PlayLinkEffects();

        OnNodeLinked.Broadcast(this, OtherNode);
    }
}

void AVitrumAlignmentNode::CompleteNode()
{
    if (NodeState == ENodeState::Linked)
    {
        NodeState = ENodeState::Completed;
        PlayCompletionEffects();
        OnNodeCompleted.Broadcast(this);
    }
}

void AVitrumAlignmentNode::ResetNode()
{
    NodeState = ENodeState::Inactive;
    UpdateMaterialColor(InactiveColor);
}

bool AVitrumAlignmentNode::CanActivate() const
{
    return NodeState == ENodeState::Inactive;
}

void AVitrumAlignmentNode::UpdateVisuals(float DeltaTime)
{
    FLinearColor TargetColor = InactiveColor;

    switch (NodeState)
    {
        case ENodeState::Inactive:
            TargetColor = InactiveColor;
            break;
        case ENodeState::Active:
            TargetColor = ActiveColor;
            break;
        case ENodeState::Linked:
            TargetColor = LinkedColor;
            break;
        case ENodeState::Completed:
            TargetColor = CompletedColor;
            break;
    }

    UpdateMaterialColor(TargetColor);
}

void AVitrumAlignmentNode::UpdatePulse(float DeltaTime)
{
    if (NodeState == ENodeState::Active)
    {
        PulseTimer += DeltaTime * PulseSpeed;
        CurrentPulseIntensity = FMath::Sin(PulseTimer) * PulseIntensity;
        bIsPulsing = true;

        // Apply pulse to material
        if (NodeMaterial)
        {
            NodeMaterial->SetScalarParameterValue("PulseIntensity", CurrentPulseIntensity);
        }
    }
    else
    {
        bIsPulsing = false;
        CurrentPulseIntensity = 0.0f;

        if (NodeMaterial)
        {
            NodeMaterial->SetScalarParameterValue("PulseIntensity", 0.0f);
        }
    }
}

void AVitrumAlignmentNode::PlayActivationEffects()
{
    if (ActivationSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ActivationSound, GetActorLocation());
    }

    if (ActivationParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivationParticles, GetActorLocation(), GetActorRotation());
    }
}

void AVitrumAlignmentNode::PlayLinkEffects()
{
    if (LinkSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, LinkSound, GetActorLocation());
    }

    if (LinkParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LinkParticles, GetActorLocation(), GetActorRotation());
    }
}

void AVitrumAlignmentNode::PlayCompletionEffects()
{
    if (CompletionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, CompletionSound, GetActorLocation());
    }

    if (CompletionParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CompletionParticles, GetActorLocation(), GetActorRotation());
    }
}

void AVitrumAlignmentNode::UpdateMaterialColor(FLinearColor Color)
{
    if (NodeMaterial)
    {
        NodeMaterial->SetVectorParameterValue("BaseColor", Color);
    }

    if (SymbolMaterial)
    {
        SymbolMaterial->SetVectorParameterValue("BaseColor", Color);
    }
}

void AVitrumAlignmentNode::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AVitrumMarble* Marble = Cast<AVitrumMarble>(OtherActor))
    {
        ActivateNode();
    }
}

void AVitrumAlignmentNode::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Optional: Handle when player leaves the node area
}
