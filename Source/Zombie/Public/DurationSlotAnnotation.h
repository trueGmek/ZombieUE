#pragma once

#include "CoreMinimal.h"
#include "SmartObjectAnnotation.h"

#include "DurationSlotAnnotation.generated.h"

USTRUCT(BlueprintType)
struct FDurationSlotAnnotation : public FSmartObjectSlotAnnotation {
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smart Object", meta = (ClampMin = "0.0", UIMin = "0.0"))
  float duration;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smart Object", meta = (ClampMin = "0.0", UIMin = "0.0"))
  float derivation;
};
