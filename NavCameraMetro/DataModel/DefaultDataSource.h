//
// DefaultDataSource.h
// Declaration of the DefaultDataSource, DefaultDataGroup, DefaultDataItem, and SampleDataCommon classes
//

#pragma once

// The data model defined by this file serves as a representative example of a strongly-typed
// model.  The property names chosen coincide with data bindings in the standard item templates.
//
// Applications may use this model as a starting point and build on it, or discard it entirely and
// replace it with something appropriate to their needs. If using this model, you might improve app
// responsiveness by initiating the data loading task in the code behind for App.xaml when the app
// is first launched.

namespace NavCameraMetro
{
	namespace Data
	{
		ref class DefaultDataGroup; // Resolve circular relationship between DefaultDataItem and DefaultDataGroup

		/// <summary>
		/// Generic hotspot data model.
		/// </summary>
		[Windows::UI::Xaml::Data::Bindable]
		public ref class DefaultDataHotspot sealed : public Windows::UI::Xaml::Data::ICustomPropertyProvider
		{
		public:
			property Platform::String^ Label { Platform::String^ get(); }
			property int Width { int get(); }
			property int Height { int get(); }
			property int Top { int get(); }
			property int Left { int get(); }

			// Implementation of ICustomPropertyProvider provides a string representation for the object to be used as automation name for accessibility
			virtual Windows::UI::Xaml::Data::ICustomProperty^ GetCustomProperty(Platform::String^ name);
			virtual Windows::UI::Xaml::Data::ICustomProperty^ GetIndexedProperty(Platform::String^ name, Windows::UI::Xaml::Interop::TypeName type);
			virtual Platform::String^ GetStringRepresentation();
			property Windows::UI::Xaml::Interop::TypeName Type { virtual Windows::UI::Xaml::Interop::TypeName get(); }

		internal:
			DefaultDataHotspot(Platform::String^ label, int width, int height, int top, int left);

		private:
			Platform::String^ _label;
			int _width;
			int _height;
			int _top;
			int _left;
		};

		/// <summary>
		/// Generic item data model.
		/// </summary>
		[Windows::UI::Xaml::Data::Bindable]
		public ref class DefaultDataItem sealed : public Windows::UI::Xaml::Data::ICustomPropertyProvider
		{
		public:
			property Platform::String^ UniqueId { Platform::String^ get(); }
			property Platform::String^ Title { Platform::String^ get(); }
			property Platform::String^ Subtitle { Platform::String^ get(); }
			property Platform::String^ Description { Platform::String^ get(); }
			property Platform::String^ Content { Platform::String^ get(); }
			property Platform::String^ ImagePath { Platform::String^ get(); }
			property Windows::Foundation::Collections::IObservableVector<DefaultDataHotspot^>^ Hotspots
			{
				Windows::Foundation::Collections::IObservableVector<DefaultDataHotspot^>^ get();
			}

			// Implementation of ICustomPropertyProvider provides a string representation for the object to be used as automation name for accessibility
			virtual Windows::UI::Xaml::Data::ICustomProperty^ GetCustomProperty(Platform::String^ name);
			virtual Windows::UI::Xaml::Data::ICustomProperty^ GetIndexedProperty(Platform::String^ name, Windows::UI::Xaml::Interop::TypeName type);
			virtual Platform::String^ GetStringRepresentation();
			property Windows::UI::Xaml::Interop::TypeName Type { virtual Windows::UI::Xaml::Interop::TypeName get(); }

		internal:
			DefaultDataItem(Platform::String^ uniqueId, Platform::String^ title, Platform::String^ subtitle, Platform::String^ imagePath,
				Platform::String^ description, Platform::String^ content);

		private:
			Platform::String^ _uniqueId;
			Platform::String^ _title;
			Platform::String^ _subtitle;
			Platform::String^ _imagePath;
			Platform::String^ _description;
			Platform::String^ _content;
			Platform::Collections::Vector<DefaultDataHotspot^>^ _hotspots;
		};

		/// <summary>
		/// Generic group data model.
		/// </summary>
		[Windows::UI::Xaml::Data::Bindable]
		public ref class DefaultDataGroup sealed : public Windows::UI::Xaml::Data::ICustomPropertyProvider
		{
		public:
			property Platform::String^ UniqueId { Platform::String^ get(); }
			property Platform::String^ Title { Platform::String^ get(); }
			property Platform::String^ Subtitle { Platform::String^ get(); }
			property Platform::String^ Description { Platform::String^ get(); }
			property Windows::Foundation::Collections::IObservableVector<DefaultDataItem^>^ Items
			{
				Windows::Foundation::Collections::IObservableVector<DefaultDataItem^>^ get();
			}
			property Platform::String^ ImagePath { Platform::String^ get(); }

			// Implementation of ICustomPropertyProvider provides a string representation for the object to be used as automation name for accessibility
			virtual Windows::UI::Xaml::Data::ICustomProperty^ GetCustomProperty(Platform::String^ name);
			virtual Windows::UI::Xaml::Data::ICustomProperty^ GetIndexedProperty(Platform::String^ name, Windows::UI::Xaml::Interop::TypeName type);
			virtual Platform::String^ GetStringRepresentation();
			property Windows::UI::Xaml::Interop::TypeName Type { virtual Windows::UI::Xaml::Interop::TypeName get(); }

		internal:
			DefaultDataGroup(Platform::String^ uniqueId, Platform::String^ title, Platform::String^ subtitle, Platform::String^ imagePath,
				Platform::String^ description);

		private:
			Platform::String^ _uniqueId;
			Platform::String^ _title;
			Platform::String^ _subtitle;
			Platform::String^ _imagePath;
			Platform::String^ _description;
			Platform::Collections::Vector<DefaultDataItem^>^ _items;
		};

		/// <summary>
		/// Creates a collection of groups and items with content read from a static json file.
		///
		/// DefaultDataSource initializes with data read from a static json file included in the
		/// project.  This provides sample data at both design-time and run-time.
		/// </summary>
		[Windows::UI::Xaml::Data::Bindable]
		public ref class DefaultDataSource sealed
		{
		public:
			property Windows::Foundation::Collections::IObservableVector<DefaultDataGroup^>^ Groups
			{
				Windows::Foundation::Collections::IObservableVector<DefaultDataGroup^>^ get();
			}

		internal:
			DefaultDataSource();
			static concurrency::task<Windows::Foundation::Collections::IIterable<DefaultDataGroup^>^> GetGroups();
			static concurrency::task<DefaultDataGroup^> GetGroup(Platform::String^ uniqueId);
			static concurrency::task<DefaultDataItem^> GetItem(Platform::String^ uniqueId);

		private:
			concurrency::task_completion_event<void> _loadCompletionEvent;
			Platform::Collections::Vector<DefaultDataGroup^>^ _groups;
			static concurrency::task<void> Init();

			static DefaultDataSource^ _sampleDataSource;
		};
	}
}
