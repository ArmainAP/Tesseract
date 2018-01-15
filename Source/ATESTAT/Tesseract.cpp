// Fill out your copyright notice in the Description page of Project Settings.

#include "Tesseract.h"
#include "ATESTATGameModeBase.h"

//In aceasta functie initializam obiectul 4D
ATesseract::ATesseract()
{
	//Cream un component de tip "ProceduralMesh"
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Tesseract"));
	RootComponent = ProceduralMesh;

	//Cautam in fisierele aplicatiei textura care urmeaza sa fie aplicata pe obiectul 4D
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/Tesseract_Mat"));
	if (FoundMaterial.Succeeded())
		ProceduralMesh->SetMaterial(0, FoundMaterial.Object);

	//Construim obiectul 
	Construct();
}

void ATesseract::BeginPlay()
{
	Super::BeginPlay();

	//Adaugam o referinta catre obiectul nostru in clasa de legatura
	Cast<AATESTATGameModeBase>(GetWorld()->GetAuthGameMode())->Tesseract = this;
}

//Generam procedural obiectul 4D
void ATesseract::Construct()
{
	//Initializam punctele tesseract-ului
	{
		Box4DVerts[0] = FVector4(-Size.X, Size.Y, Size.Z, -Size.W);
		Box4DVerts[1] = FVector4(Size.X, Size.Y, Size.Z, -Size.W);
		Box4DVerts[2] = FVector4(Size.X, -Size.Y, Size.Z, -Size.W);
		Box4DVerts[3] = FVector4(-Size.X, -Size.Y, Size.Z, -Size.W);

		Box4DVerts[4] = FVector4(-Size.X, Size.Y, -Size.Z, -Size.W);
		Box4DVerts[5] = FVector4(Size.X, Size.Y, -Size.Z, -Size.W);
		Box4DVerts[6] = FVector4(Size.X, -Size.Y, -Size.Z, -Size.W);
		Box4DVerts[7] = FVector4(-Size.X, -Size.Y, -Size.Z, -Size.W);

		Box4DVerts[8] = FVector4(-Size.X, Size.Y, Size.Z, Size.W);
		Box4DVerts[9] = FVector4(Size.X, Size.Y, Size.Z, Size.W);
		Box4DVerts[10] = FVector4(Size.X, -Size.Y, Size.Z, Size.W);
		Box4DVerts[11] = FVector4(-Size.X, -Size.Y, Size.Z, Size.W);

		Box4DVerts[12] = FVector4(-Size.X, Size.Y, -Size.Z, Size.W);
		Box4DVerts[13] = FVector4(Size.X, Size.Y, -Size.Z, Size.W);
		Box4DVerts[14] = FVector4(Size.X, -Size.Y, -Size.Z, Size.W);
		Box4DVerts[15] = FVector4(-Size.X, -Size.Y, -Size.Z, Size.W);
	}

	//Initializam culorile tessaractului
	{
		BoxColor.Add(FColor(0, 0, 0, 255));
		BoxColor.Add(FColor(255, 255, 255, 255));
		BoxColor.Add(FColor(255, 0, 0, 255));
		BoxColor.Add(FColor(0, 255, 0, 255));
		BoxColor.Add(FColor(0, 0, 255, 255));
		BoxColor.Add(FColor(255, 255, 0, 255));
		BoxColor.Add(FColor(255, 0, 255, 255));
		BoxColor.Add(FColor(0, 255, 255, 255));
	}

	//Resetam variabilele TArray si alocam memorie
	{
		Triangles.Reset();

		// 6 fete x 4 puncte per fata x 8 cutii = 192

		Vertices4D.Reset();
		Vertices4D.AddUninitialized(192);

		Normals.Reset();
		Normals.AddUninitialized(192);

		Tangents.Reset();
		Tangents.AddUninitialized(192);

		UVs.Reset();
		UVs.AddUninitialized(192);

		VertexColors.Reset();
		VertexColors.AddUninitialized(192);
	}

	//Initializam fiecare cutie 3D care realizeaza cutia 4D
	{
		//0-23
		{
			Vertices4D[0] = Box4DVerts[0];
			Vertices4D[1] = Box4DVerts[1];
			Vertices4D[2] = Box4DVerts[2];
			Vertices4D[3] = Box4DVerts[3];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 3, 2, 1, 0);
			Normals[0] = Normals[1] = Normals[2] = Normals[3] = FVector(0, 0, 1);
			Tangents[0] = Tangents[1] = Tangents[2] = Tangents[3] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[4] = Box4DVerts[4];
			Vertices4D[5] = Box4DVerts[0];
			Vertices4D[6] = Box4DVerts[3];
			Vertices4D[7] = Box4DVerts[7];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 7, 6, 5, 4);
			Normals[4] = Normals[5] = Normals[6] = Normals[7] = FVector(-1, 0, 0);
			Tangents[4] = Tangents[5] = Tangents[6] = Tangents[7] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[8] = Box4DVerts[5];
			Vertices4D[9] = Box4DVerts[1];
			Vertices4D[10] = Box4DVerts[0];
			Vertices4D[11] = Box4DVerts[4];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 11, 10, 9, 8);
			Normals[8] = Normals[9] = Normals[10] = Normals[11] = FVector(0, 1, 0);
			Tangents[8] = Tangents[9] = Tangents[10] = Tangents[11] = FProcMeshTangent(-1.f, 0.f, 0.f);

			Vertices4D[12] = Box4DVerts[6];
			Vertices4D[13] = Box4DVerts[2];
			Vertices4D[14] = Box4DVerts[1];
			Vertices4D[15] = Box4DVerts[5];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 15, 14, 13, 12);
			Normals[12] = Normals[13] = Normals[14] = Normals[15] = FVector(1, 0, 0);
			Tangents[12] = Tangents[13] = Tangents[14] = Tangents[15] = FProcMeshTangent(0.f, 1.f, 0.f);

			Vertices4D[16] = Box4DVerts[7];
			Vertices4D[17] = Box4DVerts[3];
			Vertices4D[18] = Box4DVerts[2];
			Vertices4D[19] = Box4DVerts[6];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 19, 18, 17, 16);
			Normals[16] = Normals[17] = Normals[18] = Normals[19] = FVector(0, -1, 0);
			Tangents[16] = Tangents[17] = Tangents[18] = Tangents[19] = FProcMeshTangent(1.f, 0.f, 0.f);

			Vertices4D[20] = Box4DVerts[7];
			Vertices4D[21] = Box4DVerts[6];
			Vertices4D[22] = Box4DVerts[5];
			Vertices4D[23] = Box4DVerts[4];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 23, 22, 21, 20);
			Normals[20] = Normals[21] = Normals[22] = Normals[23] = FVector(0, 0, -1);
			Tangents[20] = Tangents[21] = Tangents[22] = Tangents[23] = FProcMeshTangent(0.f, 1.f, 0.f);

			UVs[0] = UVs[4] = UVs[8] = UVs[12] = UVs[16] = UVs[20] = FVector2D(0.f, 0.f);
			UVs[1] = UVs[5] = UVs[9] = UVs[13] = UVs[17] = UVs[21] = FVector2D(0.f, 1.f);
			UVs[2] = UVs[6] = UVs[10] = UVs[14] = UVs[18] = UVs[22] = FVector2D(1.f, 1.f);
			UVs[3] = UVs[7] = UVs[11] = UVs[15] = UVs[19] = UVs[23] = FVector2D(1.f, 0.f);

			VertexColors[0] = VertexColors[1] = VertexColors[2] = VertexColors[3] = VertexColors[4] = VertexColors[5] =
			VertexColors[6] = VertexColors[7] = VertexColors[8] = VertexColors[9] = VertexColors[10] = VertexColors[11] =
			VertexColors[12] = VertexColors[13] = VertexColors[14] = VertexColors[15] = VertexColors[16] = VertexColors[17] =
			VertexColors[18] = VertexColors[19] = VertexColors[20] = VertexColors[21] = VertexColors[22] = VertexColors[23] = 
			BoxColor[0];
		}

		//24-47
		{
			Vertices4D[24] = Box4DVerts[8];
			Vertices4D[25] = Box4DVerts[9];
			Vertices4D[26] = Box4DVerts[10];
			Vertices4D[27] = Box4DVerts[11];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 24, 25, 26, 27);
			Normals[24] = Normals[25] = Normals[26] = Normals[27] = FVector(0, 0, 1);
			Tangents[24] = Tangents[25] = Tangents[26] = Tangents[27] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[28] = Box4DVerts[12];
			Vertices4D[29] = Box4DVerts[8];
			Vertices4D[30] = Box4DVerts[11];
			Vertices4D[31] = Box4DVerts[15];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 28, 29, 30, 31);
			Normals[28] = Normals[29] = Normals[30] = Normals[31] = FVector(-1, 0, 0);
			Tangents[28] = Tangents[29] = Tangents[30] = Tangents[31] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[32] = Box4DVerts[13];
			Vertices4D[33] = Box4DVerts[9];
			Vertices4D[34] = Box4DVerts[8];
			Vertices4D[35] = Box4DVerts[12];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 32, 33, 34, 35);
			Normals[32] = Normals[33] = Normals[34] = Normals[35] = FVector(0, 1, 0);
			Tangents[32] = Tangents[33] = Tangents[34] = Tangents[35] = FProcMeshTangent(-1.f, 0.f, 0.f);

			Vertices4D[36] = Box4DVerts[14];
			Vertices4D[37] = Box4DVerts[10];
			Vertices4D[38] = Box4DVerts[9];
			Vertices4D[39] = Box4DVerts[13];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 36, 37, 38, 39);
			Normals[36] = Normals[37] = Normals[38] = Normals[39] = FVector(1, 0, 0);
			Tangents[36] = Tangents[37] = Tangents[38] = Tangents[39] = FProcMeshTangent(0.f, 1.f, 0.f);

			Vertices4D[40] = Box4DVerts[15];
			Vertices4D[41] = Box4DVerts[11];
			Vertices4D[42] = Box4DVerts[10];
			Vertices4D[43] = Box4DVerts[14];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 40, 41, 42, 43);
			Normals[40] = Normals[41] = Normals[42] = Normals[43] = FVector(0, -1, 0);
			Tangents[40] = Tangents[41] = Tangents[42] = Tangents[43] = FProcMeshTangent(1.f, 0.f, 0.f);

			Vertices4D[44] = Box4DVerts[15];
			Vertices4D[45] = Box4DVerts[14];
			Vertices4D[46] = Box4DVerts[13];
			Vertices4D[47] = Box4DVerts[12];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 44, 45, 46, 47);
			Normals[44] = Normals[45] = Normals[46] = Normals[47] = FVector(0, 0, -1);
			Tangents[44] = Tangents[45] = Tangents[46] = Tangents[47] = FProcMeshTangent(0.f, 1.f, 0.f);

			UVs[24] = UVs[28] = UVs[32] = UVs[36] = UVs[40] = UVs[44] = FVector2D(0.f, 0.f);
			UVs[25] = UVs[29] = UVs[33] = UVs[37] = UVs[41] = UVs[45] = FVector2D(0.f, 1.f);
			UVs[26] = UVs[30] = UVs[34] = UVs[38] = UVs[42] = UVs[46] = FVector2D(1.f, 1.f);
			UVs[27] = UVs[31] = UVs[35] = UVs[39] = UVs[43] = UVs[47] = FVector2D(1.f, 0.f);

			VertexColors[24] = VertexColors[25] = VertexColors[26] = VertexColors[27] = VertexColors[28] = VertexColors[29] =
			VertexColors[30] = VertexColors[31] = VertexColors[32] = VertexColors[33] = VertexColors[34] = VertexColors[35] =
			VertexColors[36] = VertexColors[37] = VertexColors[38] = VertexColors[39] = VertexColors[40] = VertexColors[41] =
			VertexColors[42] = VertexColors[43] = VertexColors[44] = VertexColors[45] = VertexColors[46] = VertexColors[47] =
			BoxColor[1];
		}

		//48-71
		{
			Vertices4D[48] = Box4DVerts[0];
			Vertices4D[49] = Box4DVerts[1];
			Vertices4D[50] = Box4DVerts[2];
			Vertices4D[51] = Box4DVerts[3];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 48, 49, 50, 51);
			Normals[48] = Normals[49] = Normals[50] = Normals[51] = FVector(0, 0, 1);
			Tangents[48] = Tangents[49] = Tangents[50] = Tangents[51] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[52] = Box4DVerts[8];
			Vertices4D[53] = Box4DVerts[0];
			Vertices4D[54] = Box4DVerts[3];
			Vertices4D[55] = Box4DVerts[11];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 52, 53, 54, 55);
			Normals[52] = Normals[53] = Normals[54] = Normals[55] = FVector(-1, 0, 0);
			Tangents[52] = Tangents[53] = Tangents[54] = Tangents[55] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[56] = Box4DVerts[9];
			Vertices4D[57] = Box4DVerts[1];
			Vertices4D[58] = Box4DVerts[0];
			Vertices4D[59] = Box4DVerts[8];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 56, 57, 58, 59);
			Normals[56] = Normals[57] = Normals[58] = Normals[59] = FVector(0, 1, 0);
			Tangents[56] = Tangents[57] = Tangents[58] = Tangents[59] = FProcMeshTangent(-1.f, 0.f, 0.f);

			Vertices4D[60] = Box4DVerts[10];
			Vertices4D[61] = Box4DVerts[2];
			Vertices4D[62] = Box4DVerts[1];
			Vertices4D[63] = Box4DVerts[9];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 60, 61, 62, 63);
			Normals[60] = Normals[61] = Normals[62] = Normals[63] = FVector(1, 0, 0);
			Tangents[60] = Tangents[61] = Tangents[62] = Tangents[63] = FProcMeshTangent(0.f, 1.f, 0.f);

			Vertices4D[64] = Box4DVerts[11];
			Vertices4D[65] = Box4DVerts[3];
			Vertices4D[66] = Box4DVerts[2];
			Vertices4D[67] = Box4DVerts[10];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 64, 65, 66, 67);
			Normals[64] = Normals[65] = Normals[66] = Normals[67] = FVector(0, -1, 0);
			Tangents[64] = Tangents[65] = Tangents[66] = Tangents[67] = FProcMeshTangent(1.f, 0.f, 0.f);

			Vertices4D[68] = Box4DVerts[11];
			Vertices4D[69] = Box4DVerts[10];
			Vertices4D[70] = Box4DVerts[9];
			Vertices4D[71] = Box4DVerts[8];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 68, 69, 70, 71);
			Normals[68] = Normals[69] = Normals[70] = Normals[71] = FVector(0, 0, -1);
			Tangents[68] = Tangents[69] = Tangents[70] = Tangents[71] = FProcMeshTangent(0.f, 1.f, 0.f);

			UVs[48] = UVs[52] = UVs[56] = UVs[60] = UVs[64] = UVs[68] = FVector2D(0.f, 0.f);
			UVs[49] = UVs[53] = UVs[57] = UVs[61] = UVs[65] = UVs[69] = FVector2D(0.f, 1.f);
			UVs[50] = UVs[54] = UVs[58] = UVs[62] = UVs[66] = UVs[70] = FVector2D(1.f, 1.f);
			UVs[51] = UVs[55] = UVs[59] = UVs[63] = UVs[67] = UVs[71] = FVector2D(1.f, 0.f);

			VertexColors[48] = VertexColors[49] = VertexColors[50] = VertexColors[51] = VertexColors[52] = VertexColors[53] =
			VertexColors[54] = VertexColors[55] = VertexColors[56] = VertexColors[57] = VertexColors[58] = VertexColors[59] =
			VertexColors[60] = VertexColors[61] = VertexColors[62] = VertexColors[63] = VertexColors[64] = VertexColors[65] =
			VertexColors[66] = VertexColors[67] = VertexColors[68] = VertexColors[69] = VertexColors[70] = VertexColors[71] = 
			BoxColor[2];
		}

		//72-95
		{
			Vertices4D[72] = Box4DVerts[12];
			Vertices4D[73] = Box4DVerts[13];
			Vertices4D[74] = Box4DVerts[14];
			Vertices4D[75] = Box4DVerts[15];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 72, 73, 74, 75);
			Normals[72] = Normals[73] = Normals[74] = Normals[75] = FVector(0, 0, 1);
			Tangents[72] = Tangents[73] = Tangents[74] = Tangents[75] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[76] = Box4DVerts[4];
			Vertices4D[77] = Box4DVerts[12];
			Vertices4D[78] = Box4DVerts[15];
			Vertices4D[79] = Box4DVerts[7];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 76, 77, 78, 79);
			Normals[76] = Normals[77] = Normals[78] = Normals[79] = FVector(-1, 0, 0);
			Tangents[76] = Tangents[77] = Tangents[78] = Tangents[79] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[80] = Box4DVerts[5];
			Vertices4D[81] = Box4DVerts[13];
			Vertices4D[82] = Box4DVerts[12];
			Vertices4D[83] = Box4DVerts[4];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 80, 81, 82, 83);
			Normals[80] = Normals[81] = Normals[82] = Normals[83] = FVector(0, 1, 0);
			Tangents[80] = Tangents[81] = Tangents[82] = Tangents[83] = FProcMeshTangent(-1.f, 0.f, 0.f);

			Vertices4D[84] = Box4DVerts[6];
			Vertices4D[85] = Box4DVerts[14];
			Vertices4D[86] = Box4DVerts[13];
			Vertices4D[87] = Box4DVerts[5];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 84, 85, 86, 87);
			Normals[84] = Normals[85] = Normals[86] = Normals[87] = FVector(1, 0, 0);
			Tangents[84] = Tangents[85] = Tangents[86] = Tangents[87] = FProcMeshTangent(0.f, 1.f, 0.f);

			Vertices4D[88] = Box4DVerts[7];
			Vertices4D[89] = Box4DVerts[15];
			Vertices4D[90] = Box4DVerts[14];
			Vertices4D[91] = Box4DVerts[6];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 88, 89, 90, 91);
			Normals[88] = Normals[89] = Normals[90] = Normals[91] = FVector(0, -1, 0);
			Tangents[88] = Tangents[89] = Tangents[90] = Tangents[91] = FProcMeshTangent(1.f, 0.f, 0.f);

			Vertices4D[92] = Box4DVerts[7];
			Vertices4D[93] = Box4DVerts[6];
			Vertices4D[94] = Box4DVerts[5];
			Vertices4D[95] = Box4DVerts[4];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 92, 93, 94, 95);
			Normals[92] = Normals[93] = Normals[94] = Normals[95] = FVector(0, 0, -1);
			Tangents[92] = Tangents[93] = Tangents[94] = Tangents[95] = FProcMeshTangent(0.f, 1.f, 0.f);

			UVs[72] = UVs[76] = UVs[80] = UVs[84] = UVs[88] = UVs[92] = FVector2D(0.f, 0.f);
			UVs[73] = UVs[77] = UVs[81] = UVs[85] = UVs[89] = UVs[93] = FVector2D(0.f, 1.f);
			UVs[74] = UVs[78] = UVs[82] = UVs[86] = UVs[90] = UVs[94] = FVector2D(1.f, 1.f);
			UVs[75] = UVs[79] = UVs[83] = UVs[87] = UVs[91] = UVs[95] = FVector2D(1.f, 0.f);

			VertexColors[72] = VertexColors[73] = VertexColors[74] = VertexColors[75] = VertexColors[76] = VertexColors[77] =
			VertexColors[78] = VertexColors[79] = VertexColors[80] = VertexColors[81] = VertexColors[82] = VertexColors[83] =
			VertexColors[84] = VertexColors[85] = VertexColors[86] = VertexColors[87] = VertexColors[88] = VertexColors[89] =
			VertexColors[90] = VertexColors[91] = VertexColors[92] = VertexColors[93] = VertexColors[94] = VertexColors[95] = 
			BoxColor[3];
		}

		//96-119
		{
			Vertices4D[96] = Box4DVerts[0];
			Vertices4D[97] = Box4DVerts[8];
			Vertices4D[98] = Box4DVerts[11];
			Vertices4D[99] = Box4DVerts[3];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 96, 97, 98, 99);
			Normals[96] = Normals[97] = Normals[98] = Normals[99] = FVector(0, 0, 1);
			Tangents[96] = Tangents[97] = Tangents[98] = Tangents[99] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[100] = Box4DVerts[4];
			Vertices4D[101] = Box4DVerts[0];
			Vertices4D[102] = Box4DVerts[3];
			Vertices4D[103] = Box4DVerts[7];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 100, 101, 102, 103);
			Normals[100] = Normals[101] = Normals[102] = Normals[103] = FVector(-1, 0, 0);
			Tangents[100] = Tangents[101] = Tangents[102] = Tangents[103] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[104] = Box4DVerts[12];
			Vertices4D[105] = Box4DVerts[8];
			Vertices4D[106] = Box4DVerts[0];
			Vertices4D[107] = Box4DVerts[4];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 104, 105, 106, 107);
			Normals[104] = Normals[103] = Normals[104] = Normals[105] = FVector(0, 1, 0);
			Tangents[104] = Tangents[103] = Tangents[104] = Tangents[105] = FProcMeshTangent(-1.f, 0.f, 0.f);

			Vertices4D[108] = Box4DVerts[15];
			Vertices4D[109] = Box4DVerts[11];
			Vertices4D[110] = Box4DVerts[8];
			Vertices4D[111] = Box4DVerts[12];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 108, 109, 110, 111);
			Normals[108] = Normals[109] = Normals[110] = Normals[111] = FVector(1, 0, 0);
			Tangents[108] = Tangents[109] = Tangents[110] = Tangents[111] = FProcMeshTangent(0.f, 1.f, 0.f);

			Vertices4D[112] = Box4DVerts[7];
			Vertices4D[113] = Box4DVerts[3];
			Vertices4D[114] = Box4DVerts[11];
			Vertices4D[115] = Box4DVerts[15];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 112, 113, 114, 115);
			Normals[112] = Normals[113] = Normals[114] = Normals[115] = FVector(0, -1, 0);
			Tangents[112] = Tangents[113] = Tangents[114] = Tangents[115] = FProcMeshTangent(1.f, 0.f, 0.f);

			Vertices4D[116] = Box4DVerts[7];
			Vertices4D[117] = Box4DVerts[15];
			Vertices4D[118] = Box4DVerts[12];
			Vertices4D[119] = Box4DVerts[4];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 116, 117, 118, 119);
			Normals[116] = Normals[117] = Normals[118] = Normals[119] = FVector(0, 0, -1);
			Tangents[116] = Tangents[117] = Tangents[118] = Tangents[119] = FProcMeshTangent(0.f, 1.f, 0.f);

			UVs[96] = UVs[100] = UVs[104] = UVs[108] = UVs[112] = UVs[116] = FVector2D(0.f, 0.f);
			UVs[97] = UVs[101] = UVs[105] = UVs[109] = UVs[113] = UVs[117] = FVector2D(0.f, 1.f);
			UVs[98] = UVs[102] = UVs[106] = UVs[110] = UVs[114] = UVs[118] = FVector2D(1.f, 1.f);
			UVs[99] = UVs[103] = UVs[107] = UVs[111] = UVs[115] = UVs[119] = FVector2D(1.f, 0.f);

			VertexColors[96] = VertexColors[97] = VertexColors[98] = VertexColors[99] = VertexColors[100] = VertexColors[101] =
			VertexColors[102] = VertexColors[103] = VertexColors[104] = VertexColors[105] = VertexColors[106] = VertexColors[107] =
			VertexColors[108] = VertexColors[109] = VertexColors[110] = VertexColors[111] = VertexColors[112] = VertexColors[113] =
			VertexColors[114] = VertexColors[115] = VertexColors[116] = VertexColors[117] = VertexColors[118] = VertexColors[119] = 
			BoxColor[4];
		}

		//120-143
		{
			Vertices4D[120] = Box4DVerts[0];
			Vertices4D[121] = Box4DVerts[1];
			Vertices4D[122] = Box4DVerts[9];
			Vertices4D[123] = Box4DVerts[8];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 120, 121, 122, 123);
			Normals[120] = Normals[121] = Normals[122] = Normals[123] = FVector(0, 0, 1);
			Tangents[120] = Tangents[121] = Tangents[122] = Tangents[123] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[124] = Box4DVerts[4];
			Vertices4D[125] = Box4DVerts[0];
			Vertices4D[126] = Box4DVerts[8];
			Vertices4D[127] = Box4DVerts[12];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 124, 125, 126, 127);
			Normals[124] = Normals[125] = Normals[126] = Normals[127] = FVector(-1, 0, 0);
			Tangents[124] = Tangents[125] = Tangents[126] = Tangents[127] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[128] = Box4DVerts[5];
			Vertices4D[129] = Box4DVerts[1];
			Vertices4D[130] = Box4DVerts[0];
			Vertices4D[131] = Box4DVerts[4];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 128, 129, 130, 131);
			Normals[128] = Normals[129] = Normals[130] = Normals[131] = FVector(0, 1, 0);
			Tangents[128] = Tangents[129] = Tangents[130] = Tangents[131] = FProcMeshTangent(-1.f, 0.f, 0.f);

			Vertices4D[132] = Box4DVerts[13];
			Vertices4D[133] = Box4DVerts[9];
			Vertices4D[134] = Box4DVerts[1];
			Vertices4D[135] = Box4DVerts[5];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 132, 133, 134, 135);
			Normals[132] = Normals[133] = Normals[134] = Normals[135] = FVector(1, 0, 0);
			Tangents[132] = Tangents[133] = Tangents[134] = Tangents[135] = FProcMeshTangent(0.f, 1.f, 0.f);

			Vertices4D[136] = Box4DVerts[12];
			Vertices4D[137] = Box4DVerts[8];
			Vertices4D[138] = Box4DVerts[9];
			Vertices4D[139] = Box4DVerts[13];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 136, 137, 138, 139);
			Normals[136] = Normals[137] = Normals[138] = Normals[139] = FVector(0, -1, 0);
			Tangents[136] = Tangents[137] = Tangents[138] = Tangents[139] = FProcMeshTangent(1.f, 0.f, 0.f);

			Vertices4D[140] = Box4DVerts[12];
			Vertices4D[141] = Box4DVerts[13];
			Vertices4D[142] = Box4DVerts[5];
			Vertices4D[143] = Box4DVerts[4];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 140, 141, 142, 143);
			Normals[140] = Normals[141] = Normals[142] = Normals[143] = FVector(0, 0, -1);
			Tangents[140] = Tangents[141] = Tangents[142] = Tangents[143] = FProcMeshTangent(0.f, 1.f, 0.f);

			UVs[120] = UVs[124] = UVs[128] = UVs[132] = UVs[136] = UVs[140] = FVector2D(0.f, 0.f);
			UVs[121] = UVs[125] = UVs[129] = UVs[133] = UVs[137] = UVs[141] = FVector2D(0.f, 1.f);
			UVs[122] = UVs[126] = UVs[130] = UVs[134] = UVs[138] = UVs[142] = FVector2D(1.f, 1.f);
			UVs[123] = UVs[127] = UVs[131] = UVs[135] = UVs[139] = UVs[143] = FVector2D(1.f, 0.f);

			VertexColors[120] = VertexColors[121] = VertexColors[122] = VertexColors[123] = VertexColors[124] = VertexColors[125] =
			VertexColors[126] = VertexColors[127] = VertexColors[128] = VertexColors[129] = VertexColors[130] = VertexColors[131] =
			VertexColors[132] = VertexColors[133] = VertexColors[134] = VertexColors[135] = VertexColors[136] = VertexColors[137] =
			VertexColors[138] = VertexColors[139] = VertexColors[140] = VertexColors[141] = VertexColors[142] = VertexColors[143] = 
			BoxColor[5];
		}

		//144-167
		{
			Vertices4D[144] = Box4DVerts[9];
			Vertices4D[145] = Box4DVerts[1];
			Vertices4D[146] = Box4DVerts[2];
			Vertices4D[147] = Box4DVerts[10];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 144, 145, 146, 147);
			Normals[144] = Normals[145] = Normals[146] = Normals[147] = FVector(0, 0, 1);
			Tangents[144] = Tangents[145] = Tangents[146] = Tangents[147] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[148] = Box4DVerts[13];
			Vertices4D[149] = Box4DVerts[9];
			Vertices4D[150] = Box4DVerts[10];
			Vertices4D[151] = Box4DVerts[14];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 148, 149, 150, 151);
			Normals[148] = Normals[149] = Normals[150] = Normals[151] = FVector(-1, 0, 0);
			Tangents[148] = Tangents[149] = Tangents[150] = Tangents[151] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[152] = Box4DVerts[5];
			Vertices4D[153] = Box4DVerts[1];
			Vertices4D[154] = Box4DVerts[9];
			Vertices4D[155] = Box4DVerts[13];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 152, 153, 154, 155);
			Normals[152] = Normals[153] = Normals[154] = Normals[155] = FVector(0, 1, 0);
			Tangents[152] = Tangents[153] = Tangents[154] = Tangents[155] = FProcMeshTangent(-1.f, 0.f, 0.f);

			Vertices4D[156] = Box4DVerts[6];
			Vertices4D[157] = Box4DVerts[2];
			Vertices4D[158] = Box4DVerts[1];
			Vertices4D[159] = Box4DVerts[5];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 156, 157, 158, 159);
			Normals[156] = Normals[157] = Normals[158] = Normals[159] = FVector(1, 0, 0);
			Tangents[156] = Tangents[157] = Tangents[158] = Tangents[159] = FProcMeshTangent(0.f, 1.f, 0.f);

			Vertices4D[160] = Box4DVerts[14];
			Vertices4D[161] = Box4DVerts[10];
			Vertices4D[162] = Box4DVerts[2];
			Vertices4D[163] = Box4DVerts[6];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 160, 161, 162, 163);
			Normals[160] = Normals[161] = Normals[162] = Normals[163] = FVector(0, -1, 0);
			Tangents[160] = Tangents[161] = Tangents[162] = Tangents[163] = FProcMeshTangent(1.f, 0.f, 0.f);

			Vertices4D[164] = Box4DVerts[14];
			Vertices4D[165] = Box4DVerts[6];
			Vertices4D[166] = Box4DVerts[5];
			Vertices4D[167] = Box4DVerts[13];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 164, 165, 166, 167);
			Normals[164] = Normals[165] = Normals[166] = Normals[167] = FVector(0, 0, -1);
			Tangents[164] = Tangents[165] = Tangents[166] = Tangents[167] = FProcMeshTangent(0.f, 1.f, 0.f);

			UVs[144] = UVs[148] = UVs[152] = UVs[156] = UVs[160] = UVs[164] = FVector2D(0.f, 0.f);
			UVs[145] = UVs[149] = UVs[153] = UVs[157] = UVs[161] = UVs[165] = FVector2D(0.f, 1.f);
			UVs[146] = UVs[150] = UVs[154] = UVs[158] = UVs[162] = UVs[166] = FVector2D(1.f, 1.f);
			UVs[147] = UVs[151] = UVs[155] = UVs[157] = UVs[163] = UVs[167] = FVector2D(1.f, 0.f);

			VertexColors[144] = VertexColors[145] = VertexColors[146] = VertexColors[147] = VertexColors[148] = VertexColors[149] =
			VertexColors[150] = VertexColors[151] = VertexColors[152] = VertexColors[153] = VertexColors[154] = VertexColors[155] =
			VertexColors[156] = VertexColors[157] = VertexColors[158] = VertexColors[159] = VertexColors[160] = VertexColors[161] =
			VertexColors[162] = VertexColors[163] = VertexColors[164] = VertexColors[165] = VertexColors[166] = VertexColors[167] = 
			BoxColor[6];
		}

		//168-191
		{
			Vertices4D[168] = Box4DVerts[11];
			Vertices4D[169] = Box4DVerts[10];
			Vertices4D[170] = Box4DVerts[2];
			Vertices4D[171] = Box4DVerts[3];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 168, 169, 170, 171);
			Normals[168] = Normals[169] = Normals[170] = Normals[171] = FVector(0, 0, 1);
			Tangents[168] = Tangents[169] = Tangents[170] = Tangents[171] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[172] = Box4DVerts[15];
			Vertices4D[173] = Box4DVerts[11];
			Vertices4D[174] = Box4DVerts[3];
			Vertices4D[175] = Box4DVerts[7];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 172, 173, 174, 175);
			Normals[172] = Normals[173] = Normals[174] = Normals[175] = FVector(-1, 0, 0);
			Tangents[172] = Tangents[173] = Tangents[174] = Tangents[175] = FProcMeshTangent(0.f, -1.f, 0.f);

			Vertices4D[176] = Box4DVerts[14];
			Vertices4D[177] = Box4DVerts[10];
			Vertices4D[178] = Box4DVerts[11];
			Vertices4D[179] = Box4DVerts[15];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 176, 177, 178, 179);
			Normals[176] = Normals[177] = Normals[178] = Normals[179] = FVector(0, 1, 0);
			Tangents[176] = Tangents[177] = Tangents[178] = Tangents[179] = FProcMeshTangent(-1.f, 0.f, 0.f);

			Vertices4D[180] = Box4DVerts[6];
			Vertices4D[181] = Box4DVerts[2];
			Vertices4D[182] = Box4DVerts[10];
			Vertices4D[183] = Box4DVerts[14];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 180, 181, 182, 183);
			Normals[180] = Normals[181] = Normals[182] = Normals[183] = FVector(1, 0, 0);
			Tangents[180] = Tangents[181] = Tangents[182] = Tangents[183] = FProcMeshTangent(0.f, 1.f, 0.f);

			Vertices4D[184] = Box4DVerts[7];
			Vertices4D[185] = Box4DVerts[3];
			Vertices4D[186] = Box4DVerts[2];
			Vertices4D[187] = Box4DVerts[6];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 184, 185, 186, 187);
			Normals[184] = Normals[185] = Normals[186] = Normals[187] = FVector(0, -1, 0);
			Tangents[184] = Tangents[185] = Tangents[186] = Tangents[187] = FProcMeshTangent(1.f, 0.f, 0.f);

			Vertices4D[188] = Box4DVerts[7];
			Vertices4D[189] = Box4DVerts[6];
			Vertices4D[190] = Box4DVerts[14];
			Vertices4D[191] = Box4DVerts[15];
			UKismetProceduralMeshLibrary::ConvertQuadToTriangles(Triangles, 188, 189, 190, 191);
			Normals[188] = Normals[189] = Normals[190] = Normals[191] = FVector(0, 0, -1);
			Tangents[188] = Tangents[189] = Tangents[190] = Tangents[191] = FProcMeshTangent(0.f, 1.f, 0.f);

			UVs[168] = UVs[172] = UVs[176] = UVs[180] = UVs[184] = UVs[188] = FVector2D(0.f, 0.f);
			UVs[169] = UVs[173] = UVs[177] = UVs[181] = UVs[185] = UVs[189] = FVector2D(0.f, 1.f);
			UVs[170] = UVs[174] = UVs[178] = UVs[182] = UVs[186] = UVs[190] = FVector2D(1.f, 1.f);
			UVs[171] = UVs[175] = UVs[179] = UVs[183] = UVs[187] = UVs[191] = FVector2D(1.f, 0.f);

			VertexColors[168] = VertexColors[169] = VertexColors[170] = VertexColors[171] = VertexColors[172] = VertexColors[173] =
			VertexColors[174] = VertexColors[175] = VertexColors[176] = VertexColors[177] = VertexColors[178] = VertexColors[179] =
			VertexColors[180] = VertexColors[181] = VertexColors[182] = VertexColors[183] = VertexColors[184] = VertexColors[185] =
			VertexColors[186] = VertexColors[187] = VertexColors[188] = VertexColors[189] = VertexColors[190] = VertexColors[191] =
			BoxColor[7];
		}
	}

	//Cream o sectiune procedurala
	ProceduralMesh->CreateMeshSection(0, Vertices3D, Triangles, Normals, UVs, VertexColors, Tangents, false);
	//Proiectam obiectul 4D in spatiu 3D
	Project();
}

void ATesseract::Project()
{
	//Stergem si readaugam nodurile punctelor 3D
	Vertices3D.Empty();
	for (int i = 0; i < 192; i++)
		if (StereographicProjection)
			//Proiectam punctele din spatiu 4D in spatiu 3D folosind proiectie sterografica
			Vertices3D.Add(FVector(Vertices4D[i].X * LightOnW / (LightOnW - Vertices4D[i].W), Vertices4D[i].Y * LightOnW / (LightOnW - Vertices4D[i].W), Vertices4D[i].Z * LightOnW / (LightOnW - Vertices4D[i].W)) / 2);
		else
			//Proiectam punctele din spatiu 4D in spatiu 3D ignorand axa W
			Vertices3D.Add(FVector(Vertices4D[i].X, Vertices4D[i].Y, Vertices4D[i].Z));

	//Actualizam sectiunea procedurala
	ProceduralMesh->UpdateMeshSection(0, Vertices3D, Normals, UVs, VertexColors, Tangents);
}

//Aceasta functie actualizeaza culorile obiectului 4D
void ATesseract::UpdateColors()
{
	for (int i = 0; i < 24; i++)
		VertexColors[i] = BoxColor[0];

	for (int i = 24; i < 48; i++)
		VertexColors[i] = BoxColor[1];

	for (int i = 48; i < 72; i++)
		VertexColors[i] = BoxColor[2];

	for (int i = 72; i < 96; i++)
		VertexColors[i] = BoxColor[3];

	for (int i = 96; i < 120; i++)
		VertexColors[i] = BoxColor[4];

	for (int i = 120; i < 144; i++)
		VertexColors[i] = BoxColor[5];

	for (int i = 144; i < 168; i++)
		VertexColors[i] = BoxColor[6];

	for (int i = 168; i < 192; i++)
		VertexColors[i] = BoxColor[7];

	//Actualizam sectiunea procedurala
	ProceduralMesh->UpdateMeshSection(0, Vertices3D, Normals, UVs, VertexColors, Tangents);
}

void ATesseract::AddRotation(float Angle, EPlaneEnum PlaneToRotate)
{
	float x, y, z, w;

	//Transformam din grade in radiani
	Angle = Angle * (3.14159265358979323846 / 180);

	//Rotim nodurile 4D in functie de planul de rotatie dorit
	switch (PlaneToRotate)
	{
	case EPlaneEnum::R_XY:
	{
		for (int i = 0; i < 192; i++)
		{
			x = FMath::Cos(Angle) * Vertices4D[i].X - FMath::Sin(Angle) * Vertices4D[i].Y;
			y = FMath::Sin(Angle) * Vertices4D[i].X + FMath::Cos(Angle) * Vertices4D[i].Y;
			Vertices4D[i] = FVector4(x, y, Vertices4D[i].Z, Vertices4D[i].W);
		}
		break;
	}

	case EPlaneEnum::R_XZ:
	{
		for (int i = 0; i < 192; i++)
		{
			x = FMath::Cos(Angle) * Vertices4D[i].X - FMath::Sin(Angle) * Vertices4D[i].Z;
			z = FMath::Sin(Angle) * Vertices4D[i].X + FMath::Cos(Angle) * Vertices4D[i].Z;
			Vertices4D[i] = FVector4(x, Vertices4D[i].Y, z, Vertices4D[i].W);
		}
		break;
	}

	case EPlaneEnum::R_YZ:
	{
		for (int i = 0; i < 192; i++)
		{
			y = FMath::Cos(Angle) * Vertices4D[i].Y - FMath::Sin(Angle) * Vertices4D[i].Z;
			z = FMath::Sin(Angle) * Vertices4D[i].Y + FMath::Cos(Angle) * Vertices4D[i].Z;
			Vertices4D[i] = FVector4(Vertices4D[i].X, y, z, Vertices4D[i].W);
		}
		break;
	}

	case EPlaneEnum::R_XW:
	{
		for (int i = 0; i < 192; i++)
		{
			x = FMath::Cos(Angle) * Vertices4D[i].X - FMath::Sin(Angle) * Vertices4D[i].W;
			w = FMath::Sin(Angle) * Vertices4D[i].X + FMath::Cos(Angle) * Vertices4D[i].W;
			Vertices4D[i] = FVector4(x, Vertices4D[i].Y, Vertices4D[i].Z, w);
		}
		break;
	}

	case EPlaneEnum::R_YW:
	{
		for (int i = 0; i < 192; i++)
		{
			y = FMath::Cos(Angle) * Vertices4D[i].Y - FMath::Sin(Angle) * Vertices4D[i].W;
			w = FMath::Sin(Angle) * Vertices4D[i].Y + FMath::Cos(Angle) * Vertices4D[i].W;
			Vertices4D[i] = FVector4(Vertices4D[i].X, y, Vertices4D[i].Z, w);
		}
		break;
	}

	case EPlaneEnum::R_ZW:
	{
		for (int i = 0; i < 192; i++)
		{
			z = FMath::Cos(Angle) * Vertices4D[i].Z - FMath::Sin(Angle) * Vertices4D[i].W;
			w = FMath::Sin(Angle) * Vertices4D[i].Z + FMath::Cos(Angle) * Vertices4D[i].W;
			Vertices4D[i] = FVector4(Vertices4D[i].X, Vertices4D[i].Y, z, w);
		}
		break;
	}

	case EPlaneEnum::R_XYZW:
	{
		for (int i = 0; i < 192; i++)
		{
			x = FMath::Cos(Angle) * Vertices4D[i].X - FMath::Sin(Angle) * Vertices4D[i].Y;
			y = FMath::Sin(Angle) * Vertices4D[i].X + FMath::Cos(Angle) * Vertices4D[i].Y;
			z = FMath::Cos(Angle) * Vertices4D[i].Z - FMath::Sin(Angle) * Vertices4D[i].W;
			w = FMath::Sin(Angle) * Vertices4D[i].Z + FMath::Cos(Angle) * Vertices4D[i].W;
			Vertices4D[i] = FVector4(x, y, z, w);
		}
		break;
	}

	case EPlaneEnum::R_XZYW:
	{
		for (int i = 0; i < 192; i++)
		{
			x = FMath::Cos(Angle) * Vertices4D[i].X - FMath::Sin(Angle) * Vertices4D[i].Z;
			z = FMath::Sin(Angle) * Vertices4D[i].X + FMath::Cos(Angle) * Vertices4D[i].Z;
			y = FMath::Cos(Angle) * Vertices4D[i].Y - FMath::Sin(Angle) * Vertices4D[i].W;
			w = FMath::Sin(Angle) * Vertices4D[i].Y + FMath::Cos(Angle) * Vertices4D[i].W;
			Vertices4D[i] = FVector4(x, y, z, w);
		}
		break;
	}

	case EPlaneEnum::R_YZXW:
	{
		for (int i = 0; i < 192; i++)
		{
			y = FMath::Cos(Angle) * Vertices4D[i].Y - FMath::Sin(Angle) * Vertices4D[i].Z;
			z = FMath::Sin(Angle) * Vertices4D[i].Y + FMath::Cos(Angle) * Vertices4D[i].Z;
			x = FMath::Cos(Angle) * Vertices4D[i].X - FMath::Sin(Angle) * Vertices4D[i].W;
			w = FMath::Sin(Angle) * Vertices4D[i].X + FMath::Cos(Angle) * Vertices4D[i].W;
			Vertices4D[i] = FVector4(x, y, z, w);
		}
		break;
	}
	}

	//Proiectam obiectul 4D in spatiu 3D
	Project();
}
