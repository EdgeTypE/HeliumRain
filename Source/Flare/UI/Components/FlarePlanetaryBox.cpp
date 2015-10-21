
#include "../../Flare.h"
#include "FlarePlanetaryBox.h"


void SFlarePlanetaryBox::Construct(const SFlarePlanetaryBox::FArguments& InArgs)
{
	Radius = 200;
	PlanetImage = NULL;
	ClearChildren();

	// Add slots
	const int32 NumSlots = InArgs.Slots.Num();
	for (int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
	{
		Children.Add(InArgs.Slots[SlotIndex]);
	}
}

void SFlarePlanetaryBox::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	if (Children.Num() > 1)
	{
		FVector2D TypicalSize = Children[1].GetWidget()->GetDesiredSize();

		for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
		{
			// Get all required data
			const SFlarePlanetaryBox::FSlot& CurChild = Children[ChildIndex];
			const EVisibility ChildVisibility = CurChild.GetWidget()->GetVisibility();
			FVector2D Origin = (AllottedGeometry.GetLocalSize() - CurChild.GetWidget()->GetDesiredSize()) / 2;
			FVector2D Offset = FVector2D::ZeroVector;

			// Child with index 0 is the main body, index 1 is probably the name but we don't care, others are sectors
			if (ChildIndex > 0)
			{
				float X, Y;
				float Angle = (360 / (Children.Num() - 1)) * (ChildIndex - 1) - 90;
				FMath::PolarToCartesian(Radius, FMath::DegreesToRadians(Angle), X, Y);
				Offset =  FVector2D(X, Y);
			}
			
			// Arrange the child
			ArrangedChildren.AddWidget(ChildVisibility, AllottedGeometry.MakeChild(
				CurChild.GetWidget(),
				Origin + Offset,
				CurChild.GetWidget()->GetDesiredSize()
			));
		}
	}
}

FVector2D SFlarePlanetaryBox::ComputeDesiredSize(float) const
{
	return 2 * FVector2D(Radius, Radius);
}

FChildren* SFlarePlanetaryBox::GetChildren()
{
	return &Children;
}

int32 SFlarePlanetaryBox::RemoveSlot(const TSharedRef<SWidget>& SlotWidget)
{
	for (int32 SlotIdx = 0; SlotIdx < Children.Num(); ++SlotIdx)
	{
		if (SlotWidget == Children[SlotIdx].GetWidget())
		{
			Children.RemoveAt(SlotIdx);
			return SlotIdx;
		}
	}

	return -1;
}

void SFlarePlanetaryBox::ClearChildren()
{
	Children.Empty();

	if (PlanetImage)
	{
		AddSlot()
		[
			SNew(SImage).Image(PlanetImage)
		];
	}
}
