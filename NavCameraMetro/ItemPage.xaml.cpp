//
// ItemPage.xaml.cpp
// Implementation of the ItemPage class
//

#include "pch.h"
#include "ItemPage.xaml.h"
#include "MediaPage.xaml.h"
#include "F3DPage.xaml.h"

using namespace NavCameraMetro;

using namespace Platform;
using namespace Platform::Collections;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Item Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234232

ItemPage::ItemPage()
{
	InitializeComponent();
	SetValue(_defaultViewModelProperty, ref new Map<String^,Object^>(std::less<String^>()));
	auto navigationHelper = ref new Common::NavigationHelper(this);
	SetValue(_navigationHelperProperty, navigationHelper);
	navigationHelper->LoadState += ref new Common::LoadStateEventHandler(this, &ItemPage::LoadState);
}

DependencyProperty^ ItemPage::_defaultViewModelProperty =
	DependencyProperty::Register("DefaultViewModel",
		TypeName(IObservableMap<String^,Object^>::typeid), TypeName(ItemPage::typeid), nullptr);

/// <summary>
/// used as a trivial view model.
/// </summary>
IObservableMap<String^, Object^>^ ItemPage::DefaultViewModel::get()
{
	return safe_cast<IObservableMap<String^, Object^>^>(GetValue(_defaultViewModelProperty));
}

DependencyProperty^ ItemPage::_navigationHelperProperty =
	DependencyProperty::Register("NavigationHelper",
		TypeName(Common::NavigationHelper::typeid), TypeName(ItemPage::typeid), nullptr);

/// <summary>
/// Gets an implementation of <see cref="NavigationHelper"/> designed to be
/// used as a trivial view model.
/// </summary>
Common::NavigationHelper^ ItemPage::NavigationHelper::get()
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

void ItemPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedTo(e);
}

void ItemPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedFrom(e);
}

#pragma endregion

/// <summary>
/// Populates the page with content passed during navigation.  Any saved state is also
/// provided when recreating a page from a prior session.
/// </summary>
/// <param name="sender">
/// The source of the event; typically <see cref="NavigationHelper"/>
/// </param>
/// <param name="e">Event data that provides both the navigation parameter passed to
/// <see cref="Frame::Navigate(Type, Object)"/> when this page was initially requested and
/// a dictionary of state preserved by this page during an earlier
/// session.  The state will be null the first time a page is visited.</param>
void ItemPage::LoadState(Object^ sender, Common::LoadStateEventArgs^ e)
{
	String^ navigationParameter = safe_cast<String^>(e->NavigationParameter);

	// Allow saved page state to override the initial item to display
	if (e->PageState != nullptr && e->PageState->HasKey("SelectedItem"))
	{
		navigationParameter = safe_cast<String^>(e->PageState->Lookup("SelectedItem"));
	}

	// TODO: Create an appropriate data model for your problem domain to replace the sample data
	Data::DefaultDataSource::GetItem(safe_cast<String^>(navigationParameter))
	.then([this](Data::DefaultDataItem^ item)
	{
		DefaultViewModel->Insert("Item", item);
	}, task_continuation_context::use_current());
}

void ItemPage::AppBarMediaButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs e)
{
	// TODO: transfer item id to the media page
	Frame->Navigate(TypeName(MediaPage::typeid));
}

void NavCameraMetro::ItemPage::AppBar3DModelButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Frame->Navigate(TypeName(F3DPage::typeid));
}

void NavCameraMetro::ItemPage::HotspotButton_PointerEntered(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	Button ^button = safe_cast<Button ^>(sender);
	button->Opacity = 100;
}


void NavCameraMetro::ItemPage::HotspotButton_PointerExited(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	Button ^button = safe_cast<Button ^>(sender);
	button->Opacity = 0;
}


void NavCameraMetro::ItemPage::HotspotButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Button ^button = safe_cast<Button ^>(sender);
	Frame->Navigate(TypeName(ItemPage::typeid), button->CommandParameter);
}
