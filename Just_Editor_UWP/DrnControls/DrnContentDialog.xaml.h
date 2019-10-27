//
// DrnContentDialog.xaml.h
// Declaration of the DrnContentDialog class
//

#pragma once

#include "DrnControls/DrnContentDialog.g.h"

namespace Just_Editor_UWP
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnContentDialog sealed
	{
	public:
		DrnContentDialog(Platform::String^ content, Platform::String^ Fst, Platform::String^ Sec, Platform::String^ Cls);
	private:
	};
}
