#pragma once

#include "CoreMinimal.h"

#include "PointOfInvestigation.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct ZOMBIE_API FPointOfInvestigation {
  GENERATED_BODY()
public:
  UPROPERTY(BlueprintReadOnly)
  FVector Location;

  UPROPERTY(BlueprintReadOnly)
  FVector Directon;

  UPROPERTY(BlueprintReadOnly)
  float TimeStamp;

  UPROPERTY(BlueprintReadOnly)
  float Strength;

  UPROPERTY(BlueprintReadWrite)
  bool bWasInspected;
};
