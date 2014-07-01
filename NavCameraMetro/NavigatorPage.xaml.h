﻿//
// NavigatorPage.xaml.h
// Declaration of the NavigatorPage class
//

#pragma once

#include "NavigatorPage.g.h"
#include "Common\NavigationHelper.h"

namespace NavCameraMetro
{
	/// <summary>
	/// A page that displays a grouped collection of items.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class NavigatorPage sealed
	{
	public:
		NavigatorPage();

		/// <summary>
		/// This can be changed to a strongly typed view model.
		/// </summary>
		property Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^ DefaultViewModel
		{
			Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^  get();
		}

		/// <summary>
		/// NavigationHelper is used on each page to aid in navigation and 
		/// process lifetime management
		/// </summary>
		property Common::NavigationHelper^ NavigationHelper
		{
			Common::NavigationHelper^ get();
		}

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		void LoadState(Platform::Object^ sender, Common::LoadStateEventArgs^ e);
		
		static Windows::UI::Xaml::DependencyProperty^ _defaultViewModelProperty;
		static Windows::UI::Xaml::DependencyProperty^ _navigationHelperProperty;
	};
}
