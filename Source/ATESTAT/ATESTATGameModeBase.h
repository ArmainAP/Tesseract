#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ATESTATGameModeBase.generated.h"

//Aceasta clasa creaza legatura dintre UI si obiectul 4D.

class ATesseract;
class UUI;
UCLASS()
class ATESTAT_API AATESTATGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UUI* UI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classes")
		TSubclassOf<UUI> WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATesseract* Tesseract;
};
