#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "Tesseract.generated.h"

	//Aceasta este o enumeratie pentru planul de rotatie dorit.
UENUM(BlueprintType)
enum class EPlaneEnum : uint8
{
	R_XY	UMETA(DisplayName = "XY"),
	R_XZ	UMETA(DisplayName = "XZ"),
	R_YZ	UMETA(DisplayName = "YZ"),
	R_XW	UMETA(DisplayName = "XW"),
	R_YW	UMETA(DisplayName = "YW"),
	R_ZW	UMETA(DisplayName = "ZW"),
	R_XYZW	UMETA(DisplayName = "XYZW"),
	R_XZYW	UMETA(DisplayName = "XZYW"),
	R_YZXW	UMETA(DisplayName = "YZXW")
};

//Aceasta clasa realizeaza constructia, proiectia si rotatiile obiectului nostru 4D
UCLASS()
class ATESTAT_API ATesseract : public AActor
{
	GENERATED_BODY()

public:
	ATesseract();

	UFUNCTION(BlueprintCallable)
		void Construct();

	UFUNCTION(BlueprintCallable)
		void Project();

	UFUNCTION(BlueprintCallable)
		void UpdateColors();

	UFUNCTION(BlueprintCallable)
		void AddRotation(float Angle, EPlaneEnum PlaneToRotate);

protected:
	virtual void BeginPlay() override;

public:
	UProceduralMeshComponent* ProceduralMesh;

	FVector4 Size = FVector4(50.0f, 50.0f, 50.0f, 50.0f);
	FVector4 Box4DVerts[16];
	TArray<FVector4> Vertices4D;
	TArray<FVector> Vertices3D;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FColor> VertexColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FColor> BoxColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool StereographicProjection = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LightOnW = Size.W * 2;
};