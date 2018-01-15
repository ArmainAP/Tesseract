#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI.generated.h"

	//Aceasta clasa contine cateva functii pentru UI

class AATESTATGameModeBase;
UCLASS()
class ATESTAT_API UUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void Create(float x, float y, float z, float w);

	UFUNCTION(BlueprintPure)
		FColor LiniarToColor(float R, float G, float B, float A);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AATESTATGameModeBase* GameMode;
};
