
#include "FlareColorPanel.h"
#include "../../Flare.h"
#include "../../Game/FlareGame.h"
#include "../../Player/FlareMenuPawn.h"
#include "../../Player/FlareMenuManager.h"
#include "../../Player/FlarePlayerController.h"
#include "../../Data/FlareCustomizationCatalog.h"


#define LOCTEXT_NAMESPACE "FlareColorPanel"


/*----------------------------------------------------
	Construct
----------------------------------------------------*/

void SFlareColorPanel::Construct(const FArguments& InArgs)
{
	// Data
	MenuManager = InArgs._MenuManager;
	AFlareGame* Game = Cast<AFlareGame>(MenuManager->GetWorld()->GetAuthGameMode());
	UFlareCustomizationCatalog* CustomizationCatalog = Game->GetCustomizationCatalog();
	UFlareSpacecraftComponentsCatalog* ShipPartsCatalog = Game->GetShipPartsCatalog();
	
	// Layout
	ChildSlot
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SAssignNew(PickerList, SHorizontalBox)
		
		// Pattern picker
		+ SHorizontalBox::Slot().AutoWidth()
		.VAlign(VAlign_Top)
		[
			SAssignNew(PatternPicker, SFlareDropList<int32>)
			.OnItemPicked(this, &SFlareColorPanel::OnPatternPicked)
		]
		
		// Base paint picker
		+ SHorizontalBox::Slot().AutoWidth()
		.VAlign(VAlign_Top)
		[
			SAssignNew(BasePaintColorPicker, SFlareDropList<FLinearColor>)
			.OnItemPicked(this, &SFlareColorPanel::OnBasePaintColorPickedByIndex)
			.OnColorPicked(this, &SFlareColorPanel::OnBasePaintColorPicked)
			.LineSize(3)
			.ItemWidth(1)
			.ItemHeight(1)
			.ShowColorWheel(true)
		]
		
		// Paint picker
		+ SHorizontalBox::Slot().AutoWidth()
		.VAlign(VAlign_Top)
		[
			SAssignNew(PaintColorPicker, SFlareDropList<FLinearColor>)
			.OnItemPicked(this, &SFlareColorPanel::OnPaintColorPickedByIndex)
			.OnColorPicked(this, &SFlareColorPanel::OnPaintColorPicked)
			.LineSize(3)
			.ItemWidth(1)
			.ItemHeight(1)
			.ShowColorWheel(true)
		]
		
		// Overlay picker
		+ SHorizontalBox::Slot().AutoWidth()
		.VAlign(VAlign_Top)
		[
			SAssignNew(OverlayColorPicker, SFlareDropList<FLinearColor>)
			.OnItemPicked(this, &SFlareColorPanel::OnOverlayColorPickedByIndex)
			.OnColorPicked(this, &SFlareColorPanel::OnOverlayColorPicked)
			.LineSize(3)
			.ItemWidth(1)
			.ItemHeight(1)
			.ShowColorWheel(true)
		]
		
		// Light picker
		+ SHorizontalBox::Slot().AutoWidth()
		.VAlign(VAlign_Top)
		[
			SAssignNew(LightColorPicker, SFlareDropList<FLinearColor>)
			.OnItemPicked(this, &SFlareColorPanel::OnLightColorPickedByIndex)
			.OnColorPicked(this, &SFlareColorPanel::OnLightColorPicked)
			.LineSize(3)
			.ItemWidth(1)
			.ItemHeight(1)
			.ShowColorWheel(true)
		]
	];

	// Fill patterns
	for (int i = 0; i < CustomizationCatalog->GetPatternCount(); i++)
	{
		PatternPicker->AddItem(SNew(SImage).Image(CustomizationCatalog->GetPatternBrush(i)));
	}

	// Fill colors
	for (int i = 0; i < CustomizationCatalog->GetColorCount(); i++)
	{
		BasePaintColorPicker->AddItem(SNew(SColorBlock).Color(CustomizationCatalog->GetColorByIndex(i)));
		PaintColorPicker->AddItem(SNew(SColorBlock).Color(CustomizationCatalog->GetColorByIndex(i)));
		OverlayColorPicker->AddItem(SNew(SColorBlock).Color(CustomizationCatalog->GetColorByIndex(i)));
		LightColorPicker->AddItem(SNew(SColorBlock).Color(CustomizationCatalog->GetColorByIndex(i)));
	}

	// Setup initial data
	BasePaintColorPicker->SetSelectedIndex(0);
	PaintColorPicker->SetSelectedIndex(0);
	OverlayColorPicker->SetSelectedIndex(0);
	LightColorPicker->SetSelectedIndex(0);
	PatternPicker->SetSelectedIndex(0);

	BasePaintColorPicker->SetColor(FLinearColor(0,0,0));
	PaintColorPicker->SetColor(FLinearColor(0,0,0));
	OverlayColorPicker->SetColor(FLinearColor(0,0,0));
	LightColorPicker->SetColor(FLinearColor(0,0,0));
}


/*----------------------------------------------------
	Interaction
----------------------------------------------------*/

void SFlareColorPanel::Setup(FFlarePlayerSave& PlayerData)
{
	AFlareGame* Game = Cast<AFlareGame>(MenuManager->GetWorld()->GetAuthGameMode());
	UFlareCompany* Company = Game->GetGameWorld()->FindCompany(PlayerData.CompanyIdentifier);
	if (Company)
	{
		UFlareCustomizationCatalog* CustomizationCatalog = Game->GetCustomizationCatalog();

		BasePaintColorPicker->SetSelectedIndex(CustomizationCatalog->FindColor(Company->GetBasePaintColor()));
		PaintColorPicker->SetSelectedIndex(CustomizationCatalog->FindColor(Company->GetPaintColor()));
		OverlayColorPicker->SetSelectedIndex(CustomizationCatalog->FindColor(Company->GetOverlayColor()));
		LightColorPicker->SetSelectedIndex(CustomizationCatalog->FindColor(Company->GetLightColor()));
		PatternPicker->SetSelectedIndex(Company->GetPatternIndex());

		BasePaintColorPicker->SetColor(Company->GetBasePaintColor());
		PaintColorPicker->SetColor(Company->GetPaintColor());
		OverlayColorPicker->SetColor(Company->GetOverlayColor());
		LightColorPicker->SetColor(Company->GetLightColor());
	}
}

void SFlareColorPanel::SetupDefault()
{
	AFlareGame* Game = Cast<AFlareGame>(MenuManager->GetWorld()->GetAuthGameMode());
	UFlareCustomizationCatalog* CustomizationCatalog = Game->GetCustomizationCatalog();

	FLinearColor CustomizationBasePaintColor = FLinearColor(0.147932, 0.167765, 0.185); //3
	FLinearColor CustomizationPaintColor = FLinearColor(0.952941, 0.043137, 0.007843); //7
	FLinearColor CustomizationOverlayColor = FLinearColor(0.894444, 0.892754, 1.0); //4
	FLinearColor CustomizationLightColor = FLinearColor(0.0, 0.3, 1.0); //13
	int CustomizationPatternIndex = 0;

	BasePaintColorPicker->SetSelectedIndex(CustomizationCatalog->FindColor(CustomizationBasePaintColor));
	PaintColorPicker->SetSelectedIndex(CustomizationCatalog->FindColor(CustomizationPaintColor));
	OverlayColorPicker->SetSelectedIndex(CustomizationCatalog->FindColor(CustomizationOverlayColor));
	LightColorPicker->SetSelectedIndex(CustomizationCatalog->FindColor(CustomizationLightColor));
	PatternPicker->SetSelectedIndex(CustomizationPatternIndex);

	BasePaintColorPicker->SetColor(CustomizationBasePaintColor);
	PaintColorPicker->SetColor(CustomizationPaintColor);
	OverlayColorPicker->SetColor(CustomizationOverlayColor);
	LightColorPicker->SetColor(CustomizationLightColor);
}

void SFlareColorPanel::OnBasePaintColorPickedByIndex(int32 Index)
{
	AFlarePlayerController* PC = MenuManager->GetPC();
	if (PC)
	{
		AFlareGame* Game = Cast<AFlareGame>(MenuManager->GetWorld()->GetAuthGameMode());
		UFlareCustomizationCatalog* CustomizationCatalog = Game->GetCustomizationCatalog();
		const FFlareCompanyDescription* CurrentCompanyData = MenuManager->GetPC()->GetCompanyDescription();

		FLOGV("SFlareColorPanel::OnBasePaintColorPicked %d", Index);
		PC->SetBasePaintColor(CustomizationCatalog->GetColorByIndex(Index));
		PC->GetMenuPawn()->UpdateCustomization();
		BasePaintColorPicker->SetColor(CurrentCompanyData->CustomizationBasePaintColor);
	}
}

void SFlareColorPanel::OnPaintColorPickedByIndex(int32 Index)
{
	AFlarePlayerController* PC = MenuManager->GetPC();
	if (PC)
	{
		AFlareGame* Game = Cast<AFlareGame>(MenuManager->GetWorld()->GetAuthGameMode());
		UFlareCustomizationCatalog* CustomizationCatalog = Game->GetCustomizationCatalog();
		const FFlareCompanyDescription* CurrentCompanyData = MenuManager->GetPC()->GetCompanyDescription();

		FLOGV("SFlareColorPanel::OnPaintColorPicked %d", Index);
		PC->SetPaintColor(CustomizationCatalog->GetColorByIndex(Index));
		PC->GetMenuPawn()->UpdateCustomization();
		PaintColorPicker->SetColor(CurrentCompanyData->CustomizationPaintColor);
	}
}

void SFlareColorPanel::OnOverlayColorPickedByIndex(int32 Index)
{
	AFlarePlayerController* PC = MenuManager->GetPC();
	if (PC)
	{
		AFlareGame* Game = Cast<AFlareGame>(MenuManager->GetWorld()->GetAuthGameMode());
		UFlareCustomizationCatalog* CustomizationCatalog = Game->GetCustomizationCatalog();
		const FFlareCompanyDescription* CurrentCompanyData = MenuManager->GetPC()->GetCompanyDescription();

		FLOGV("SFlareColorPanel::OnOverlayColorPicked %d", Index);
		PC->SetOverlayColor(CustomizationCatalog->GetColorByIndex(Index));
		PC->GetMenuPawn()->UpdateCustomization();
		OverlayColorPicker->SetColor(CurrentCompanyData->CustomizationOverlayColor);
	}
}

void SFlareColorPanel::OnLightColorPickedByIndex(int32 Index)
{
	AFlarePlayerController* PC = MenuManager->GetPC();
	if (PC)
	{
		AFlareGame* Game = Cast<AFlareGame>(MenuManager->GetWorld()->GetAuthGameMode());
		UFlareCustomizationCatalog* CustomizationCatalog = Game->GetCustomizationCatalog();
		const FFlareCompanyDescription* CurrentCompanyData = MenuManager->GetPC()->GetCompanyDescription();

		FLOGV("SFlareColorPanel::OnLightColorPicked %d", Index);
		PC->SetLightColor(CustomizationCatalog->GetColorByIndex(Index));
		PC->GetMenuPawn()->UpdateCustomization();
		LightColorPicker->SetColor(CurrentCompanyData->CustomizationLightColor);
	}
}

void SFlareColorPanel::OnBasePaintColorPicked(FLinearColor Color)
{
	AFlarePlayerController* PC = MenuManager->GetPC();
	if (PC)
	{
		PC->SetBasePaintColor(Color);
		PC->GetMenuPawn()->UpdateCustomization();
	}
}


void SFlareColorPanel::OnPaintColorPicked(FLinearColor Color)
{
	AFlarePlayerController* PC = MenuManager->GetPC();
	if (PC)
	{
		PC->SetPaintColor(Color);
		PC->GetMenuPawn()->UpdateCustomization();
	}
}

void SFlareColorPanel::OnOverlayColorPicked(FLinearColor Color)
{
	AFlarePlayerController* PC = MenuManager->GetPC();
	if (PC)
	{
		PC->SetOverlayColor(Color);
		PC->GetMenuPawn()->UpdateCustomization();
	}
}

void SFlareColorPanel::OnLightColorPicked(FLinearColor Color)
{
	AFlarePlayerController* PC = MenuManager->GetPC();
	if (PC)
	{
		PC->SetLightColor(Color);
		PC->GetMenuPawn()->UpdateCustomization();
	}
}

void SFlareColorPanel::OnPatternPicked(int32 Index)
{
	AFlarePlayerController* PC = MenuManager->GetPC();
	if (PC)
	{
		FLOGV("SFlareColorPanel::OnPatternPicked %d", Index);
		PC->SetPatternIndex(Index);
		PC->GetMenuPawn()->UpdateCustomization();
	}
}

#undef LOCTEXT_NAMESPACE
