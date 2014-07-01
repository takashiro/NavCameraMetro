//
// NavigatorPage.xaml.cpp
// Implementation of the NavigatorPage class
//

#include "pch.h"
#include "NavigatorPage.xaml.h"

using namespace NavCameraMetro;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Hub Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=321224

NavigatorPage::NavigatorPage()
{
	InitializeComponent();
	SetValue(_defaultViewModelProperty, ref new Map<String^,Object^>(std::less<String^>()));
	auto navigationHelper = ref new Common::NavigationHelper(this);
	SetValue(_navigationHelperProperty, navigationHelper);
	navigationHelper->LoadState += ref new Common::LoadStateEventHandler(this, &NavigatorPage::LoadState);

}

DependencyProperty^ NavigatorPage::_defaultViewModelProperty =
	DependencyProperty::Register("DefaultViewModel",
		TypeName(IObservableMap<String^,Object^>::typeid), TypeName(NavigatorPage::typeid), nullptr);

/// <summary>
/// used as a trivial view model.
/// </summary>
IObservableMap<String^, Object^>^ NavigatorPage::DefaultViewModel::get()
{
	return safe_cast<IObservableMap<String^, Object^>^>(GetValue(_defaultViewModelProperty));
}

DependencyProperty^ NavigatorPage::_navigationHelperProperty =
	DependencyProperty::Register("NavigationHelper",
	TypeName(Common::NavigationHelper::typeid), TypeName(NavigatorPage::typeid), nullptr);

/// <summary>
/// Gets an implementation of <see cref="NavigationHelper"/> designed to be
/// used as a trivial view model.
/// </summary>
Common::NavigationHelper^ NavigatorPage::NavigationHelper::get()
{
	return safe_cast<Common::NavigationHelper^>(GetValue(_navigationHelperProperty));
}

#pragma region Navigation support

/// The methods provided in this section are simply used to allow
/// NavigationHelper to respond to the page's navigation methods.
/// 
/// Page specific logic should be placed in event handlers for the  
/// <see cref="NavigationHelper::LoadState"/>
/// and <see cref="NavigationHelper::SaveState"/>.
/// The navigation parameter is available in the LoadState method 
/// in addition to page state preserved during an earlier session.

void NavigatorPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	this->NavigationHelper->OnNavigatedTo(e);
}

void NavigatorPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	this->NavigationHelper->OnNavigatedFrom(e);
}

#pragma endregion
/// <summary>
/// 
/// </summary>
void NavigatorPage::LoadState(Object^ sender, Common::LoadStateEventArgs^ e)
{
	(void) sender;	// Unused parameter
	(void) e;	// Unused parameter

	// TODO: Set a collection of bindable groups using DefaultViewModel->Insert("Groups", <value>)
}
