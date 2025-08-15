#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Game-specific enums
UENUM(BlueprintType)
enum class EVitrumState : uint8
{
	Glass UMETA(DisplayName = "Glass"),
	Molten UMETA(DisplayName = "Molten"),
	Solid UMETA(DisplayName = "Solid"),
	Plated UMETA(DisplayName = "Plated"),
	Damaged UMETA(DisplayName = "Damaged"),
	Metal UMETA(DisplayName = "Metal"),
	Acid UMETA(DisplayName = "Acid")
};

UENUM(BlueprintType)
enum class EStationType : uint8
{
	PlatingLine UMETA(DisplayName = "Plating Line"),
	AcidVat UMETA(DisplayName = "Acid Vat"),
	RepairChannel UMETA(DisplayName = "Repair Channel"),
	QualityControl UMETA(DisplayName = "Quality Control"),
	Packaging UMETA(DisplayName = "Packaging")
};

class FVITRUMAXISModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
