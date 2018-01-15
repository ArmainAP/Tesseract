#include "ATESTATGameModeBase.h"
#include "UI.h"

void AATESTATGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//Afisam cursor-ul la initializarea aplicatiei.
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	//Daca clasa pentru UI este valida, il cream, il facem vizibil si ii adaugam o referinta catre aceasta clasa.
	if (WidgetClass)
	{
		UI = CreateWidget<UUI>(GetWorld()->GetFirstPlayerController(), WidgetClass);
		if (UI)
		{
			UI->AddToViewport();
			UI->GameMode = this;
		}
	}
}