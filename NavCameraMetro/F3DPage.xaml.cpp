//
// F3DPage.xaml.cpp
// Implementation of the F3DPage class
//

#include "pch.h"
#include "F3DPage.xaml.h"

#include <sstream>

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

// The Basic Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234237

F3DPage::F3DPage()
{
	InitializeComponent();
	SetValue(_defaultViewModelProperty, ref new Map<String^,Object^>(std::less<String^>()));
	auto navigationHelper = ref new Common::NavigationHelper(this);
	SetValue(_navigationHelperProperty, navigationHelper);
	navigationHelper->LoadState += ref new Common::LoadStateEventHandler(this, &F3DPage::LoadState);
	navigationHelper->SaveState += ref new Common::SaveStateEventHandler(this, &F3DPage::SaveState);
}

DependencyProperty^ F3DPage::_defaultViewModelProperty =
	DependencyProperty::Register("DefaultViewModel",
		TypeName(IObservableMap<String^,Object^>::typeid), TypeName(F3DPage::typeid), nullptr);

/// <summary>
/// used as a trivial view model.
/// </summary>
IObservableMap<String^, Object^>^ F3DPage::DefaultViewModel::get()
{
	return safe_cast<IObservableMap<String^, Object^>^>(GetValue(_defaultViewModelProperty));
}

DependencyProperty^ F3DPage::_navigationHelperProperty =
	DependencyProperty::Register("NavigationHelper",
		TypeName(Common::NavigationHelper::typeid), TypeName(F3DPage::typeid), nullptr);

/// <summary>
/// Gets an implementation of <see cref="NavigationHelper"/> designed to be
/// used as a trivial view model.
/// </summary>
Common::NavigationHelper^ F3DPage::NavigationHelper::get()
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

void F3DPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedTo(e);
}

void F3DPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedFrom(e);
}

#pragma endregion

/// <summary>
/// Populates the page with content passed during navigation. Any saved state is also
/// provided when recreating a page from a prior session.
/// </summary>
/// <param name="sender">
/// The source of the event; typically <see cref="NavigationHelper"/>
/// </param>
/// <param name="e">Event data that provides both the navigation parameter passed to
/// <see cref="Frame.Navigate(Type, Object)"/> when this page was initially requested and
/// a dictionary of state preserved by this page during an earlier
/// session. The state will be null the first time a page is visited.</param>
void F3DPage::LoadState(Object^ sender, Common::LoadStateEventArgs^ e)
{
	(void) sender;	// Unused parameter
	(void) e;	// Unused parameter
}

/// <summary>
/// Preserves state associated with this page in case the application is suspended or the
/// page is discarded from the navigation cache.  Values must conform to the serialization
/// requirements of <see cref="SuspensionManager::SessionState"/>.
/// </summary>
/// <param name="sender">The source of the event; typically <see cref="NavigationHelper"/></param>
/// <param name="e">Event data that provides an empty dictionary to be populated with
/// serializable state.</param>
void F3DPage::SaveState(Object^ sender, Common::SaveStateEventArgs^ e){
	(void) sender;	// Unused parameter
	(void) e; // Unused parameter
}


void NavCameraMetro::F3DPage::LeftButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_currentImageId--;
	if (_currentImageId <= 0){
		_currentImageId = _maxImageId;
	}

	std::wstringstream ios;
	ios << "ms-appx:///Assets/f3d/heritage-item-1/" << _currentImageId << ".jpg";
	std::wstring path;
	ios >> path;
	imageFrame->Source = ref new Imaging::BitmapImage(ref new Uri(ref new String(path.c_str())));
}

void NavCameraMetro::F3DPage::RightButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_currentImageId++;
	if (_currentImageId > _maxImageId)
	{
		_currentImageId = 1;
	}

	std::wstringstream ios;
	ios << "ms-appx:///Assets/f3d/heritage-item-1/" << _currentImageId << ".jpg";
	std::wstring path;
	ios >> path;
	imageFrame->Source = ref new Imaging::BitmapImage(ref new Uri(ref new String(path.c_str())));
}
