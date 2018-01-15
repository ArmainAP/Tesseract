#include "UI.h"
#include "ATESTATGameModeBase.h"
#include "Tesseract.h"

//Aceasta functie creaza cutia 4D cu marimile alese de utilizator
void UUI::Create(float x, float y, float z, float w)
{
	GameMode->Tesseract->Size = FVector4(x, y, z, w) / 2;
	GameMode->Tesseract->LightOnW = w;
	GameMode->Tesseract->Construct();
}

//Acesta functie realizeaza conversia culorilor din RGB liniar catre RGB
FColor UUI::LiniarToColor(float R, float G, float B, float A)
{
	return FColor(R * 255, G * 255, B * 255, A * 255);
}
