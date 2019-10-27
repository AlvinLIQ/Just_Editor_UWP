//
// DrnCoreEditorSelectionBlock.xaml.h
// Declaration of the DrnCoreEditorSelectionBlock class
//

#pragma once

#include "DrnControls/DrnCoreEditorSelectionBlock.g.h"

namespace Just_Editor_UWP
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnCoreEditorSelectionBlock sealed
	{
	public:
		DrnCoreEditorSelectionBlock();
		void SetSelection(double x, double y, double len)
		{
			rectTrans->X = x;
			rectTrans->Y = y;
			this->Width = len < 0 ? 0 : len;
		}
		double GetX()
		{
			return rectTrans->X;
		}
		double GetY()
		{
			return rectTrans->Y;
		}
	};
}
