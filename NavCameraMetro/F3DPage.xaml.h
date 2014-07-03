//
// F3DPage.xaml.h
// Declaration of the F3DPage class
//

#pragma once

#include "F3DPage.g.h"
#include "Common\NavigationHelper.h"

namespace NavCameraMetro
{
	/// <summary>
	/// A basic page that provides characteristics common to most applications.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class F3DPage sealed
	{
	public:
		F3DPage();

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
		void SaveState(Platform::Object^ sender, Common::SaveStateEventArgs^ e);

		static Windows::UI::Xaml::DependencyProperty^ _defaultViewModelProperty;
		static Windows::UI::Xaml::DependencyProperty^ _navigationHelperProperty;
		
		void LeftButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void RightButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		int _currentImageId = 1;
		int _maxImageId = 70;
	};
}
