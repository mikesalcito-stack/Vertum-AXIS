#include "VitrumCollapsingPath.h"
#include "Components/DestructibleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShake.h"

AVitrumCollapsingPath::AVitrumCollapsingPath()
{
    PrimaryActorTick.bCanEverTick = true;

    PathMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("PathMesh"));
    RootComponent = PathMesh;

    // Add Niagara component

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UNiagaraComponent* CollapseEffect;

    // In constructor
    CollapseEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CollapseEffect"));
    CollapseEffect->SetupAttachment(PathMesh);
}

void AVitrumCollapsingPath::BeginPlay()
{
    Super::BeginPlay();
}

void AVitrumCollapsingPath::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AVitrumCollapsingPath::StartCollapse()
{
    // Delay then apply damage for destruction
    FTimerHandle Handle;
    GetWorldTimerManager().SetTimer(Handle, [this]() {
        PathMesh->ApplyDamage(1000.0f, GetActorLocation(), FVector::DownVector, 1000.0f);
        OnCollapseStarted();
        // Camera shake
        UGameplayStatics::PlayWorldCameraShake(GetWorld(), UCameraShake::StaticClass(), GetActorLocation(), 0.0f, 1000.0f);
    }, CollapseDelay, false);

    // Full collapse after duration
    GetWorldTimerManager().SetTimer(Handle, [this]() {
        Destroy();
    }, CollapseDelay + CollapseDuration, false);
}
