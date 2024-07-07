#include "BitMapLoadingStep.h"

#include "Area.h"
#include "GroundTruthLoadingStep.h"
#include "QueriesLoadingStep.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Canvas.h"
#include "LandscapeProxy.h"

#include "Engine/Texture2D.h" // TODO: MAYBE REMOVE THIS???.
#include "Serialization/BulkData.h" // TODO: MAYBE REMOVE THIS???.

UBitMapLoadingStep::UBitMapLoadingStep()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBitMapLoadingStep::BeginPlay()
{
	Super::BeginPlay();
}

void UBitMapLoadingStep::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBitMapLoadingStep::LoadDataFromFiles()
{
	UE_LOG(LogTemp, Warning, TEXT("Loading bitmap from file."));
	// Reads bitmap result, from a previous generated result (load a png)
}

void UBitMapLoadingStep::LaunchLoadThread()
{
	UE_LOG(LogTemp, Warning, TEXT("Generating bitmap."));

	const auto& GroundTruthComponent {GetOwner()->GetComponentByClass(UGroundTruthLoadingStep::StaticClass())};
	const auto& GroundTruth {Cast<UGroundTruthLoadingStep>(GroundTruthComponent)->GetGroundTruth()};

	const auto& QueriesComponent {GetOwner()->GetComponentByClass(UQueriesLoadingStep::StaticClass())};
	const auto& QueriesInfo {Cast<UQueriesLoadingStep>(QueriesComponent)->GetQueriesInfo()};

	// Using CanvasRenderTarget2D: 
	/*
	// First call CreateCanvasRenderTarget2D
	// Bind this render target to custom function we use to draw to the canvas.
	// The canvas render target. 
	// Set is as material on the mesh component (or the terrain actor). 
	*/

	// Using KismetRenderingLibrary::BeginDrawCanvasToRenderTarget. 
	float MAP_WIDTH {2048.0f};
	float MAP_HEIGHT {2048.0f};

	up_RenderTarget->InitCustomFormat(MAP_WIDTH, MAP_HEIGHT, PF_B8G8R8A8, true);
	UCanvas* Canvas;
	FVector2D CanvasSize {MAP_WIDTH, MAP_HEIGHT};
	FDrawToRenderTargetContext Context;

	FVector Location {GetOwner()->GetActorLocation()};

	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, up_RenderTarget, Canvas, CanvasSize, Context);

	// Draw to canvas. 
	FVector2D LineLocStart {FVector2D(0.0f, 0.0f)};
	FVector2D LineLocEnd {LineLocStart + FVector2D(MAP_WIDTH*4/4, MAP_HEIGHT*4/4)};
	Canvas->K2_DrawLine(LineLocStart, LineLocEnd, 1.0f, FLinearColor::Red);

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);


	
	// Modyfying directly the texture, bit by bit: 
	/*
	uint32 MAP_WIDTH {100U};
	uint32 MAP_HEIGHT {100U};
	UTexture2D* CustomTexture = UTexture2D::CreateTransient(MAP_WIDTH, MAP_HEIGHT);
	FTexture2DMipMap* MipMap = &(CustomTexture->PlatformData->Mips[0]);
	FByteBulkData* ImageData = &(MipMap->BulkData);
	uint8* RawImageData = (uint8*)ImageData->Lock(LOCK_READ_WRITE);
	int ArraySize = 4 * MAP_WIDTH * MAP_HEIGHT;
	for (auto i = 0; i < ArraySize; i += 4)
	{
		RawImageData[i] = 255;
		RawImageData[i + 1] = 0;
		RawImageData[i + 2] = 255;
		RawImageData[i + 3] = 0;
	}
	ImageData->Unlock();
	CustomTexture->UpdateResource();
	UMaterialInstanceDynamic* DynamicMaterial = MeshComponent->CreateDynamicMaterialInstance(0, MeshComponent->GetMaterial(0));
	DynamicMaterial->SetTextureParameterValue("Texture", CustomTexture);
	MeshComponent->SetMaterial(0, DynamicMaterial);
	*/

	// Set material to quad. 
	const auto& QuadMeshComponent {Cast<UMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()))};
	UMaterialInstanceDynamic* QuadDynamicMaterial = QuadMeshComponent->CreateDynamicMaterialInstance(0, QuadMeshComponent->GetMaterial(0));
	QuadDynamicMaterial->SetTextureParameterValue("Texture", up_RenderTarget);
	QuadMeshComponent->SetMaterial(0, QuadDynamicMaterial);

	// Set material to terrain. // TODO: Move to its own component. // TODO: NOT GET THE TERRAIN ACTOR DIRECTLY? 
	//up_TerrainActor->SetLandscapeMaterialTextureParameterValue("Texture", up_RenderTarget);
	m_LoadingFinished = true;
}

void UBitMapLoadingStep::StopLoadThread()
{
	// Kills process that is generating bitmap. 
}