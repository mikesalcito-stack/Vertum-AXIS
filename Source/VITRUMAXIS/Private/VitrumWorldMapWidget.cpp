#include "VitrumWorldMapWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "VitrumLevelManager.h"

UVitrumWorldMapWidget::UVitrumWorldMapWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Set default values
    CurrentWorldID = NAME_None;
    MapZoom = 1.0f;
    MapOffset = FVector2D::ZeroVector;
    LevelManager = nullptr;
    bIsInitialized = false;
}

void UVitrumWorldMapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button events
    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UVitrumWorldMapWidget::OnBackButtonClicked);
    }

    if (ZoomInButton)
    {
        ZoomInButton->OnClicked.AddDynamic(this, &UVitrumWorldMapWidget::OnZoomInButtonClicked);
    }

    if (ZoomOutButton)
    {
        ZoomOutButton->OnClicked.AddDynamic(this, &UVitrumWorldMapWidget::OnZoomOutButtonClicked);
    }

    // Find level manager
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVitrumLevelManager::StaticClass(), FoundActors);
    
    if (FoundActors.Num() > 0)
    {
        LevelManager = Cast<AVitrumLevelManager>(FoundActors[0]);
    }
}

void UVitrumWorldMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (bIsInitialized)
    {
        UpdateNodeVisuals();
        UpdateProgressDisplay();
    }
}

void UVitrumWorldMapWidget::InitializeMap(int32 WorldIndex)
{
    if (!LevelManager)
    {
        return;
    }

    CurrentWorldData = LevelManager->GetCurrentWorldData();
    CurrentWorldID = FName(*FString::Printf(TEXT("World%d"), WorldIndex));

    // Update world information
    if (WorldTitleText)
    {
        WorldTitleText->SetText(FText::FromString(CurrentWorldData.WorldName));
    }

    if (WorldDescriptionText)
    {
        WorldDescriptionText->SetText(FText::FromString(CurrentWorldData.WorldDescription));
    }

    // Create map nodes from level data
    MapNodes.Empty();
    for (const FLevelData& LevelData : CurrentWorldData.Levels)
    {
        FMapNodeData NodeData;
        NodeData.LevelID = LevelData.LevelID;
        NodeData.Position = LevelData.MapPosition;
        NodeData.ConnectedNodes = LevelData.ConnectedLevels;
        NodeData.bIsUnlocked = LevelData.bIsUnlocked;
        NodeData.bIsCompleted = LevelData.bIsCompleted;
        NodeData.ArtisanShardsCollected = LevelData.ArtisanShardsCollected;
        NodeData.TotalArtisanShards = LevelData.TotalArtisanShards;
        
        MapNodes.Add(NodeData);
    }

    CreateMapNodes();
    CreateConnectionLines();
    UpdateMapView();

    bIsInitialized = true;
}

void UVitrumWorldMapWidget::RefreshMap()
{
    if (!LevelManager || !bIsInitialized)
    {
        return;
    }

    // Update node data from level manager
    for (FMapNodeData& NodeData : MapNodes)
    {
        FLevelData LevelData = LevelManager->GetLevelData(NodeData.LevelID);
        NodeData.bIsUnlocked = LevelData.bIsUnlocked;
        NodeData.bIsCompleted = LevelData.bIsCompleted;
        NodeData.ArtisanShardsCollected = LevelData.ArtisanShardsCollected;
        NodeData.TotalArtisanShards = LevelData.TotalArtisanShards;
    }

    UpdateNodeVisuals();
    UpdateProgressDisplay();
}

void UVitrumWorldMapWidget::SelectLevel(FName LevelID)
{
    if (LevelManager && LevelManager->IsLevelUnlocked(LevelID))
    {
        OnLevelSelected.Broadcast(LevelID);
        LevelManager->LoadLevel(LevelID);
    }
}

void UVitrumWorldMapWidget::ZoomIn()
{
    MapZoom = FMath::Min(2.0f, MapZoom + 0.2f);
    UpdateMapView();
}

void UVitrumWorldMapWidget::ZoomOut()
{
    MapZoom = FMath::Max(0.5f, MapZoom - 0.2f);
    UpdateMapView();
}

void UVitrumWorldMapWidget::ResetZoom()
{
    MapZoom = 1.0f;
    MapOffset = FVector2D::ZeroVector;
    UpdateMapView();
}

void UVitrumWorldMapWidget::UpdateMapView()
{
    if (!MapCanvas)
    {
        return;
    }

    // Update node positions
    for (const FMapNodeData& NodeData : MapNodes)
    {
        if (UButton** ButtonPtr = NodeButtons.Find(NodeData.LevelID))
        {
            if (UButton* Button = *ButtonPtr)
            {
                if (UCanvasPanelSlot* ButtonSlot = Cast<UCanvasPanelSlot>(Button->Slot))
                {
                    FVector2D ScreenPosition = WorldToScreenPosition(NodeData.Position);
                    ButtonSlot->SetPosition(ScreenPosition);
                }
            }
        }
    }

    // Update connection line positions
    for (UImage* ConnectionLine : ConnectionLines)
    {
        if (UCanvasPanelSlot* LineSlot = Cast<UCanvasPanelSlot>(ConnectionLine->Slot))
        {
            // Update connection line position and rotation
            // This would be calculated based on the connected nodes
        }
    }
}

FVector2D UVitrumWorldMapWidget::GetNodePosition(FName LevelID)
{
    for (const FMapNodeData& NodeData : MapNodes)
    {
        if (NodeData.LevelID == LevelID)
        {
            return WorldToScreenPosition(NodeData.Position);
        }
    }
    return FVector2D::ZeroVector;
}

bool UVitrumWorldMapWidget::IsNodeUnlocked(FName LevelID)
{
    for (const FMapNodeData& NodeData : MapNodes)
    {
        if (NodeData.LevelID == LevelID)
        {
            return NodeData.bIsUnlocked;
        }
    }
    return false;
}

bool UVitrumWorldMapWidget::IsNodeCompleted(FName LevelID)
{
    for (const FMapNodeData& NodeData : MapNodes)
    {
        if (NodeData.LevelID == LevelID)
        {
            return NodeData.bIsCompleted;
        }
    }
    return false;
}

void UVitrumWorldMapWidget::CreateMapNodes()
{
    if (!MapCanvas)
    {
        return;
    }

    // Clear existing nodes
    for (auto& Pair : NodeButtons)
    {
        if (Pair.Value)
        {
            Pair.Value->RemoveFromParent();
        }
    }
    NodeButtons.Empty();
    NodeImages.Empty();

    // Create new nodes
    for (const FMapNodeData& NodeData : MapNodes)
    {
        // Create button for the node
        UButton* NodeButton = NewObject<UButton>(this);
        NodeButton->SetVisibility(ESlateVisibility::Visible);
        
        // Create image for the node
        UImage* NodeImage = NewObject<UImage>(this);
        NodeImage->SetVisibility(ESlateVisibility::Visible);
        
        // Set the appropriate texture based on node state
        if (NodeData.bIsCompleted && NodeCompletedTexture)
        {
            NodeImage->SetBrushFromTexture(NodeCompletedTexture);
        }
        else if (NodeData.bIsUnlocked && NodeUnlockedTexture)
        {
            NodeImage->SetBrushFromTexture(NodeUnlockedTexture);
        }
        else if (NodeLockedTexture)
        {
            NodeImage->SetBrushFromTexture(NodeLockedTexture);
        }

        // Add image to button
        NodeButton->AddChild(NodeImage);
        
        // Position the button
        FVector2D ScreenPosition = WorldToScreenPosition(NodeData.Position);
        UCanvasPanelSlot* NodeSlot = MapCanvas->AddChildToCanvas(NodeButton);
        NodeSlot->SetPosition(ScreenPosition);
        NodeSlot->SetSize(FVector2D(64.0f, 64.0f));
        NodeSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));

        // Bind click event
        NodeButton->OnClicked.AddDynamic(this, &UVitrumWorldMapWidget::OnNodeButtonClicked);
        
        // Store references
        NodeButtons.Add(NodeData.LevelID, NodeButton);
        NodeImages.Add(NodeData.LevelID, NodeImage);
    }
}

void UVitrumWorldMapWidget::CreateConnectionLines()
{
    if (!MapCanvas || !ConnectionLineTexture)
    {
        return;
    }

    // Clear existing connection lines
    for (UImage* ConnectionLine : ConnectionLines)
    {
        if (ConnectionLine)
        {
            ConnectionLine->RemoveFromParent();
        }
    }
    ConnectionLines.Empty();

    // Create connection lines between connected nodes
    for (const FMapNodeData& NodeData : MapNodes)
    {
        for (FName ConnectedLevelID : NodeData.ConnectedNodes)
        {
            // Find the connected node
            FVector2D StartPos = FVector2D::ZeroVector;
            FVector2D EndPos = FVector2D::ZeroVector;
            
            for (const FMapNodeData& ConnectedNode : MapNodes)
            {
                if (ConnectedNode.LevelID == ConnectedLevelID)
                {
                    StartPos = WorldToScreenPosition(NodeData.Position);
                    EndPos = WorldToScreenPosition(ConnectedNode.Position);
                    break;
                }
            }

            if (StartPos != FVector2D::ZeroVector && EndPos != FVector2D::ZeroVector)
            {
                // Create connection line
                UImage* ConnectionLine = NewObject<UImage>(this);
                ConnectionLine->SetBrushFromTexture(ConnectionLineTexture);
                ConnectionLine->SetVisibility(ESlateVisibility::Visible);

                // Calculate line position and rotation
                FVector2D LineCenter = (StartPos + EndPos) * 0.5f;
                FVector2D LineDirection = EndPos - StartPos;
                float LineLength = LineDirection.Size();
                float LineRotation = FMath::Atan2(LineDirection.Y, LineDirection.X) * 180.0f / PI;

                // Add to canvas
                UCanvasPanelSlot* ConnectionSlot = MapCanvas->AddChildToCanvas(ConnectionLine);
                ConnectionSlot->SetPosition(LineCenter);
                ConnectionSlot->SetSize(FVector2D(LineLength, 4.0f));
                ConnectionSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));

                // Set rotation
                ConnectionLine->SetRenderTransformAngle(LineRotation);

                ConnectionLines.Add(ConnectionLine);
            }
        }
    }
}

void UVitrumWorldMapWidget::UpdateNodeVisuals()
{
    for (const FMapNodeData& NodeData : MapNodes)
    {
        if (UImage** ImagePtr = NodeImages.Find(NodeData.LevelID))
        {
            if (UImage* Image = *ImagePtr)
            {
                // Update texture based on current state
                if (NodeData.bIsCompleted && NodeCompletedTexture)
                {
                    Image->SetBrushFromTexture(NodeCompletedTexture);
                }
                else if (NodeData.bIsUnlocked && NodeUnlockedTexture)
                {
                    Image->SetBrushFromTexture(NodeUnlockedTexture);
                }
                else if (NodeLockedTexture)
                {
                    Image->SetBrushFromTexture(NodeLockedTexture);
                }

                // Update button interactivity
                if (UButton** ButtonPtr = NodeButtons.Find(NodeData.LevelID))
                {
                    if (UButton* Button = *ButtonPtr)
                    {
                        Button->SetIsEnabled(NodeData.bIsUnlocked);
                    }
                }
            }
        }
    }
}

void UVitrumWorldMapWidget::UpdateProgressDisplay()
{
    if (!ProgressText)
    {
        return;
    }

    int32 CompletedLevels = 0;
    int32 TotalLevels = MapNodes.Num();
    int32 TotalShards = 0;
    int32 CollectedShards = 0;

    for (const FMapNodeData& NodeData : MapNodes)
    {
        if (NodeData.bIsCompleted)
        {
            CompletedLevels++;
        }
        TotalShards += NodeData.TotalArtisanShards;
        CollectedShards += NodeData.ArtisanShardsCollected;
    }

    FString ProgressString = FString::Printf(TEXT("Progress: %d/%d Levels | Shards: %d/%d"), 
        CompletedLevels, TotalLevels, CollectedShards, TotalShards);
    
    ProgressText->SetText(FText::FromString(ProgressString));
}

FVector2D UVitrumWorldMapWidget::WorldToScreenPosition(FVector2D WorldPosition)
{
    // Apply zoom and offset
    FVector2D ScreenPosition = (WorldPosition + MapOffset) * MapZoom;
    
    // Center the map
    if (MapCanvas)
    {
        FVector2D CanvasSize = MapCanvas->GetCachedGeometry().GetLocalSize();
        ScreenPosition += CanvasSize * 0.5f;
    }
    
    return ScreenPosition;
}

void UVitrumWorldMapWidget::OnBackButtonClicked()
{
    // Return to main menu or previous screen
    RemoveFromParent();
}

void UVitrumWorldMapWidget::OnZoomInButtonClicked()
{
    ZoomIn();
}

void UVitrumWorldMapWidget::OnZoomOutButtonClicked()
{
    ZoomOut();
}

void UVitrumWorldMapWidget::OnNodeButtonClicked()
{
    // This will need to be implemented to handle the specific button that was clicked
    // For now, we'll just call SelectLevel with a default value
    SelectLevel(NAME_None);
}
