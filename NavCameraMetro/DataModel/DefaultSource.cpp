//
// DefaultDataSource.cpp
// Implementation of the DefaultDataSource, DefaultDataGroup, DefaultDataItem, and SampleDataCommon classes
//

#include "pch.h"

using namespace NavCameraMetro::Data;

using namespace Platform;
using namespace Platform::Collections;
using namespace concurrency;
using namespace Windows::ApplicationModel::Resources::Core;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage;
using namespace Windows::Data::Json;

//
// DefaultDataItem
//

DefaultDataHotspot::DefaultDataHotspot(String^ label, int width, int height, int top, int left) :
	_label(label),
	_width(width),
	_height(height),
	_top(top),
	_left(left)
{
}

String^ DefaultDataHotspot::Label::get()
{
	return _label;
}

int DefaultDataHotspot::Width::get()
{
	return _width;
}

int DefaultDataHotspot::Height::get()
{
	return _height;
}

int DefaultDataHotspot::Top::get()
{
	return _top;
}

int DefaultDataHotspot::Left::get()
{
	return _left;
}

Windows::UI::Xaml::Data::ICustomProperty^ DefaultDataHotspot::GetCustomProperty(Platform::String^ name)
{
	return nullptr;
}

Windows::UI::Xaml::Data::ICustomProperty^ DefaultDataHotspot::GetIndexedProperty(Platform::String^ name, Windows::UI::Xaml::Interop::TypeName type)
{
	return nullptr;
}

Platform::String^ DefaultDataHotspot::GetStringRepresentation()
{
	return _label;
}

Windows::UI::Xaml::Interop::TypeName DefaultDataHotspot::Type::get()
{
	return this->GetType();
}

//
// DefaultDataItem
//

DefaultDataItem::DefaultDataItem(String^ uniqueId, String^ title, String^ subtitle, String^ imagePath, String^ description,
	String^ content) :
	_uniqueId(uniqueId),
	_title(title),
	_subtitle(subtitle),
	_description(description),
	_imagePath(imagePath),
	_content(content),
	_hotspots(ref new Vector<DefaultDataHotspot^>())
{
}

String^ DefaultDataItem::UniqueId::get()
{
	return _uniqueId;
}

String^ DefaultDataItem::Title::get()
{
	return _title;
}

String^ DefaultDataItem::Subtitle::get()
{
	return _subtitle;
}

String^ DefaultDataItem::Description::get()
{
	return _description;
}

String^ DefaultDataItem::Content::get()
{
	return _content;
}

String^ DefaultDataItem::ImagePath::get()
{
	return _imagePath;
}

IObservableVector<DefaultDataHotspot^>^ DefaultDataItem::Hotspots::get()
{
	return _hotspots;
}

Windows::UI::Xaml::Data::ICustomProperty^ DefaultDataItem::GetCustomProperty(Platform::String^ name)
{
	return nullptr;
}

Windows::UI::Xaml::Data::ICustomProperty^ DefaultDataItem::GetIndexedProperty(Platform::String^ name, Windows::UI::Xaml::Interop::TypeName type)
{
	return nullptr;
}

Platform::String^ DefaultDataItem::GetStringRepresentation()
{
	return Title;
}

Windows::UI::Xaml::Interop::TypeName DefaultDataItem::Type::get()
{
	return this->GetType();
}

//
// DefaultDataGroup
//

DefaultDataGroup::DefaultDataGroup(String^ uniqueId, String^ title, String^ subtitle, String^ imagePath, String^ description) :
	_uniqueId(uniqueId),
	_title(title),
	_subtitle(subtitle),
	_description(description),
	_imagePath(imagePath),
	_items(ref new Vector<DefaultDataItem^>())
{
}

String^ DefaultDataGroup::UniqueId::get()
{
	return _uniqueId;
}

String^ DefaultDataGroup::Title::get()
{
	return _title;
}

String^ DefaultDataGroup::Subtitle::get()
{
	return _subtitle;
}

String^ DefaultDataGroup::Description::get()
{
	return _description;
}

IObservableVector<DefaultDataItem^>^ DefaultDataGroup::Items::get()
{
	return _items;
}

String^ DefaultDataGroup::ImagePath::get()
{
	return _imagePath;
}

Windows::UI::Xaml::Data::ICustomProperty^ DefaultDataGroup::GetCustomProperty(Platform::String^ name)
{
	return nullptr;
}

Windows::UI::Xaml::Data::ICustomProperty^ DefaultDataGroup::GetIndexedProperty(Platform::String^ name, Windows::UI::Xaml::Interop::TypeName type)
{
	return nullptr;
}

Platform::String^ DefaultDataGroup::GetStringRepresentation()
{
	return Title;
}

Windows::UI::Xaml::Interop::TypeName DefaultDataGroup::Type::get()
{
	return this->GetType();
}

//
// DefaultDataSource
//

DefaultDataSource::DefaultDataSource()
{
	_groups = ref new Vector<DefaultDataGroup^>();

	Uri^ uri = ref new Uri("ms-appx:///DataModel/DefaultData.json");
	create_task(StorageFile::GetFileFromApplicationUriAsync(uri))
		.then([](StorageFile^ storageFile)
	{
		return FileIO::ReadTextAsync(storageFile);
	})
		.then([this](String^ jsonText)
	{
		JsonObject^ jsonObject = JsonObject::Parse(jsonText);
		auto jsonVector = jsonObject->GetNamedArray("Groups")->GetView();

		for (const auto &jsonGroupValue : jsonVector)
		{
			JsonObject^ groupObject = jsonGroupValue->GetObject();
			DefaultDataGroup^ group = ref new DefaultDataGroup(groupObject->GetNamedString("UniqueId"),
				groupObject->GetNamedString("Title"),
				groupObject->GetNamedString("Subtitle"),
				groupObject->GetNamedString("ImagePath"),
				groupObject->GetNamedString("Description"));

			auto jsonItemVector = groupObject->GetNamedArray("Items")->GetView();
			for (const auto &jsonItemValue : jsonItemVector)
			{
				JsonObject^ itemObject = jsonItemValue->GetObject();

				DefaultDataItem^ item = ref new DefaultDataItem(itemObject->GetNamedString("UniqueId"),
					itemObject->GetNamedString("Title"),
					itemObject->GetNamedString("Subtitle"),
					itemObject->GetNamedString("ImagePath"),
					itemObject->GetNamedString("Description"),
					itemObject->GetNamedString("Content"));

				if (itemObject->HasKey("Hotspots"))
				{
					auto jsonHotspotVector = itemObject->GetNamedArray("Hotspots")->GetView();
					for (const auto &jsonHotspotValue : jsonHotspotVector)
					{
						JsonObject^ hotspotObject = jsonHotspotValue->GetObject();

						DefaultDataHotspot^ hotspot = ref new DefaultDataHotspot(hotspotObject->GetNamedString("Label"),
							(int)hotspotObject->GetNamedNumber("Width"),
							(int)hotspotObject->GetNamedNumber("Height"),
							(int)hotspotObject->GetNamedNumber("Top"),
							(int)hotspotObject->GetNamedNumber("Left"));

						item->Hotspots->Append(hotspot);
					};
				}

				group->Items->Append(item);
			};

			_groups->Append(group);
		};
	})
		.then([this](task<void> t)
	{
		try
		{
			t.get();
		}
		catch (Platform::COMException^ e)
		{
			OutputDebugString(e->Message->Data());
			// TODO: If App can recover from exception,
			// remove throw; below and add recovery code.
			throw;
		}
		// Signal load completion event
		_loadCompletionEvent.set();
	});
}

IObservableVector<DefaultDataGroup^>^ DefaultDataSource::Groups::get()
{
	return _groups;
}

DefaultDataSource^ DefaultDataSource::_sampleDataSource = nullptr;

task<void> DefaultDataSource::Init()
{
	if (_sampleDataSource == nullptr)
	{
		_sampleDataSource = ref new DefaultDataSource();
	}
	return create_task(_sampleDataSource->_loadCompletionEvent);
}

task<IIterable<DefaultDataGroup^>^> DefaultDataSource::GetGroups()
{
	return Init()
		.then([]() -> IIterable<DefaultDataGroup^> ^
	{
		return _sampleDataSource->Groups;
	});
}

task<DefaultDataGroup^> DefaultDataSource::GetGroup(String^ uniqueId)
{
	return Init()
		.then([uniqueId]() -> DefaultDataGroup ^
	{
		// Simple linear search is acceptable for small data sets
		for (const auto& group : _sampleDataSource->Groups)
		{
			if (group->UniqueId == uniqueId)
			{
				return group;
			}
		}
		return nullptr;
	});
}

task<DefaultDataItem^> DefaultDataSource::GetItem(String^ uniqueId)
{
	return Init()
		.then([uniqueId]() -> DefaultDataItem ^
	{
		// Simple linear search is acceptable for small data sets
		for (const auto& group : _sampleDataSource->Groups)
		{
			for (const auto& item : group->Items)
			{
				if (item->UniqueId == uniqueId)
				{
					return item;
				}
			}
		}
		return nullptr;
	});
}
