//
// DrnMenuItem.xaml.h
// Declaration of the DrnMenuItem class
//

#pragma once

#include "DrnControls\DrnMenuItem.g.h"

namespace Just_Editor_UWP
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DrnMenuItem sealed
	{
	public:
		DrnMenuItem();
		
		property Platform::String^ IconSrc;
		property Platform::String^ Text;
		property Platform::String^ Tip;
	};
}
