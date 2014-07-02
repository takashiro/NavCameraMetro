//
// ActivityPage.xaml.cpp
// Implementation of the ActivityPage class
//

#include "pch.h"
#include "ActivityPage.xaml.h"

using namespace NavCameraMetro;
using namespace NavCameraMetro::Common;

using namespace Platform;
using namespace Platform::Collections;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Split Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234234

ActivityPage::ActivityPage()
{
	InitializeComponent();
	SetValue(_defaultViewModelProperty, ref new Map<String^,Object^>(std::less<String^>()));
	auto navigationHelper = ref new Common::NavigationHelper(this,
		ref new Common::RelayCommand(
		[this](Object^) -> bool
	{
		return CanGoBack();
	},
		[this](Object^) -> void
	{
		GoBack();
	}
	)
		);
	SetValue(_navigationHelperProperty, navigationHelper);
	navigationHelper->LoadState += ref new Common::LoadStateEventHandler(this, &ActivityPage::LoadState);
	navigationHelper->SaveState += ref new Common::SaveStateEventHandler(this, &ActivityPage::SaveState);

	itemListView->SelectionChanged += ref new SelectionChangedEventHandler(this, &ActivityPage::ItemListView_SelectionChanged);
	Window::Current->SizeChanged += ref new WindowSizeChangedEventHandler (this, &ActivityPage::Window_SizeChanged);
	InvalidateVisualState();

}

DependencyProperty^ ActivityPage::_defaultViewModelProperty =
	DependencyProperty::Register("DefaultViewModel",
		TypeName(IObservableMap<String^,Object^>::typeid), TypeName(ActivityPage::typeid), nullptr);

/// <summary>
/// used as a trivial view model.
/// </summary>
IObservableMap<String^, Object^>^ ActivityPage::DefaultViewModel::get()
{
	return safe_cast<IObservableMap<String^, Object^>^>(GetValue(_defaultViewModelProperty));
}

DependencyProperty^ ActivityPage::_navigationHelperProperty =
	DependencyProperty::Register("NavigationHelper",
		TypeName(Common::NavigationHelper::typeid), TypeName(ActivityPage::typeid), nullptr);

/// <summary>
/// Gets an implementation of <see cref="NavigationHelper"/> designed to be
/// used as a trivial view model.
/// </summary>
Common::NavigationHelper^ ActivityPage::NavigationHelper::get()
{
	//	return _navigationHelper;
	return safe_cast<Common::NavigationHelper^>(GetValue(_navigationHelperProperty));
}

#pragma region Page state management

/// <summary>
/// Populates the page with content passed during navigation.  Any saved state is also
/// provided when recreating a page from a prior session.
/// </summary>
/// <param name="navigationParameter">The parameter value passed to
/// <see cref="Frame::Navigate(Type, Object)"/> when this page was initially requested.
/// </param>
/// <param name="pageState">A map of state preserved by this page during an earlier
/// session.  This will be null the first time a page is visited.</param>
void ActivityPage::LoadState(Platform::Object^ sender, Common::LoadStateEventArgs^ e)
{
	Data::SampleDataSource::GetGroup("Group-Activity")
		.then([this](Data::SampleDataGroup^ group)
	{
		DefaultViewModel->Insert("Items", group->Items);
	}, task_continuation_context::use_current());

	if (e->PageState == nullptr)
	{
		// When this is a new page, select the first item automatically unless logical page
		// navigation is being used (see the logical page navigation #region below.)
		if (!UsingLogicalPageNavigation() && itemsViewSource->View != nullptr)
		{
			itemsViewSource->View->MoveCurrentToFirst();
		}
	}
	else
	{
		// Restore the previously saved state associated with this page
		if (e->PageState->HasKey("SelectedItem") && itemsViewSource->View != nullptr)
		{
			// TODO: Invoke itemsViewSource->View->MoveCurrentTo() with the selected
			//       item as specified by the value of pageState->Lookup("SelectedItem")
		}
	}
}

/// <summary>
/// Preserves state associated with this page in case the application is suspended or the
/// page is discarded from the navigation cache.  Values must conform to the serialization
/// requirements of <see cref="SuspensionManager::SessionState"/>.
/// </summary>
/// <param name="sender">The source of the event; typically <see cref="NavigationHelper"/></param>
/// <param name="e">Event data that provides an empty dictionary to be populated with
/// serializable state.</param>
void ActivityPage::SaveState(Platform::Object^ sender, Common::SaveStateEventArgs^ e)
{
	if (itemsViewSource->View != nullptr)
	{
		auto selectedItem = itemsViewSource->View->CurrentItem;
		// TODO: Derive a serializable navigation parameter and pass it to
		//       pageState->Insert("SelectedItem", <value>)
	}
}

#pragma endregion

#pragma region Logical page navigation

// Visual state management typically reflects the four application view states directly (full
// screen landscape and portrait plus snapped and filled views.)  The split page is designed so
// that the snapped and portrait view states each have two distinct sub-states: either the item
// list or the details are displayed, but not both at the same time.
//
// This is all implemented with a single physical page that can represent two logical pages.
// The code below achieves this goal without making the user aware of the distinction.

/// <summary>
/// Invoked to determine whether the page should act as one logical page or two.
/// </summary>
/// <returns>True when the current view state is portrait or snapped, false
/// otherwise.</returns>
bool ActivityPage::CanGoBack()
{
	if (UsingLogicalPageNavigation() && itemListView->SelectedItem != nullptr)
	{
		return true;
	}
	else
	{
		return NavigationHelper->CanGoBack();
	}
}

void ActivityPage::GoBack()
{
	if (UsingLogicalPageNavigation() && itemListView->SelectedItem != nullptr)
	{
		// When logical page navigation is in effect and there's a selected item that
		// item's details are currently displayed.  Clearing the selection will return to
		// the item list.  From the user's point of view this is a logical backward
		// navigation.
		itemListView->SelectedItem = nullptr;
	}
	else
	{
		NavigationHelper->GoBack();
	}
}

/// <summary>
/// Invoked with the Window changes size
/// </summary>
/// <param name="sender">The current Window</param>
/// <param name="e">Event data that describes the new size of the Window</param>
void ActivityPage::Window_SizeChanged(Platform::Object^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ e)
{
	InvalidateVisualState();
}

/// <summary>
/// Invoked when an item within the list is selected.
/// </summary>
/// <param name="sender">The GridView displaying the selected item.</param>
/// <param name="e">Event data that describes how the selection was changed.</param>
void ActivityPage::ItemListView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	if (UsingLogicalPageNavigation())
	{
		InvalidateVisualState();
	}
}

/// <summary>
/// Invoked to determine whether the page should act as one logical page or two.
/// </summary>
/// <returns>True if the window should show act as one logical page, false
/// otherwise.</returns>
bool ActivityPage::UsingLogicalPageNavigation()
{
	return Windows::UI::Xaml::Window::Current->Bounds.Width < MinimumWidthForSupportingTwoPanes;
}

void ActivityPage::InvalidateVisualState()
{
	auto visualState = DetermineVisualState();
	Windows::UI::Xaml::VisualStateManager::GoToState(this, visualState, false);
	NavigationHelper->GoBackCommand->RaiseCanExecuteChanged();
}

/// <summary>
/// Invoked to determine the name of the visual state that corresponds to an application
/// view state.
/// </summary>
/// <returns>The name of the desired visual state.  This is the same as the name of the
/// view state except when there is a selected item in portrait and snapped views where
/// this additional logical page is represented by adding a suffix of _Detail.</returns>
Platform::String^ ActivityPage::DetermineVisualState()
{
	if (!UsingLogicalPageNavigation())
		return "PrimaryView";

	// Update the back button's enabled state when the view state changes
	auto logicalPageBack = UsingLogicalPageNavigation() && itemListView->SelectedItem != nullptr;

	return logicalPageBack ? "SinglePane_Detail" : "SinglePane";
}

#pragma endregion

#pragma region Navigation support

/// The methods provided in this section are simply used to allow
/// NavigationHelper to respond to the page's navigation methods.
/// 
/// Page specific logic should be placed in event handlers for the  
/// <see cref="NavigationHelper::LoadState"/>
/// and <see cref="NavigationHelper::SaveState"/>.
/// The navigation parameter is available in the LoadState method 
/// in addition to page state preserved during an earlier session.

void ActivityPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedTo(e);
}

void ActivityPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedFrom(e);

}
#pragma endregion