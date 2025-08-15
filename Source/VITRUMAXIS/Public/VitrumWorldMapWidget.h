#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "VitrumLevelManager.h"
#include "VitrumWorldMapWidget.generated.h"

USTRUCT(BlueprintType)
struct FMapNodeData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Node")
    FName LevelID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Node")
    FVector2D Position;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Node")
    TArray<FName> ConnectedNodes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Node")
    bool bIsUnlocked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Node")
    bool bIsCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Node")
    int32 ArtisanShardsCollected;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Node")
    int32 TotalArtisanShards;

    FMapNodeData()
    {
        LevelID = NAME_None;
        Position = FVector2D::ZeroVector;
        bIsUnlocked = false;
        bIsCompleted = false;
        ArtisanShardsCollected = 0;
        TotalArtisanShards = 0;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelSelected, FName, LevelID);

UCLASS(BlueprintType, Blueprintable)
class VITRUMAXIS_API UVitrumWorldMapWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UVitrumWorldMapWidget(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
    // Map Components
    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* MapCanvas;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* WorldTitleText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* WorldDescriptionText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ProgressText;

    UPROPERTY(meta = (BindWidget))
    UButton* BackButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ZoomInButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ZoomOutButton;

    // Map Data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    TArray<FMapNodeData> MapNodes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    FName CurrentWorldID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    float MapZoom = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    FVector2D MapOffset = FVector2D::ZeroVector;

    // Visual Assets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UTexture2D* NodeUnlockedTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UTexture2D* NodeLockedTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UTexture2D* NodeCompletedTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UTexture2D* ConnectionLineTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UTexture2D* BackgroundTexture;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLevelSelected OnLevelSelected;

    // Public Functions
    UFUNCTION(BlueprintCallable, Category = "Map")
    void InitializeMap(int32 WorldIndex);

    UFUNCTION(BlueprintCallable, Category = "Map")
    void RefreshMap();

    UFUNCTION(BlueprintCallable, Category = "Map")
    void SelectLevel(FName LevelID);

    UFUNCTION(BlueprintCallable, Category = "Map")
    void ZoomIn();

    UFUNCTION(BlueprintCallable, Category = "Map")
    void ZoomOut();

    UFUNCTION(BlueprintCallable, Category = "Map")
    void ResetZoom();

    UFUNCTION(BlueprintCallable, Category = "Map")
    void UpdateMapView();

    UFUNCTION(BlueprintCallable, Category = "Map")
    FVector2D GetNodePosition(FName LevelID);

    UFUNCTION(BlueprintCallable, Category = "Map")
    bool IsNodeUnlocked(FName LevelID);

    UFUNCTION(BlueprintCallable, Category = "Map")
    bool IsNodeCompleted(FName LevelID);

protected:
    // Internal Functions
    void CreateMapNodes();
    void CreateConnectionLines();
    void UpdateNodeVisuals();
    void UpdateProgressDisplay();
    FVector2D WorldToScreenPosition(FVector2D WorldPosition);

    // Button Event Handlers
    UFUNCTION(BlueprintCallable, Category = "Events")
    void OnBackButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Events")
    void OnZoomInButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Events")
    void OnZoomOutButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "Events")
    void OnNodeButtonClicked();

private:
    // Internal State
    TMap<FName, UButton*> NodeButtons;
    TMap<FName, UImage*> NodeImages;
    TArray<UImage*> ConnectionLines;
    AVitrumLevelManager* LevelManager;
    FWorldData CurrentWorldData;
    bool bIsInitialized;
};
