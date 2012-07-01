#include <iostream>
#include <time.h>
#include <thread>
#include <string>
#include <chrono>
#include <iomanip> 
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <gtkmm.h>


namespace timekeeper {
	
	class comboColumns : public Gtk::TreeModel::ColumnRecord {
		public:
			comboColumns (){
				add(id);
				add(datestring);
			}

			Gtk::TreeModelColumn<int> id;
			Gtk::TreeModelColumn<std::string> datestring;
	} comboColumns;
	
	class timeKeeper : public Gtk::Window {
		public:
			timeKeeper();
			~timeKeeper(){}


			void dateChanged();			
		private:
			//void buttonPressed();
			bool started;
			Gtk::Button startStop;
			Gtk::Label timeLabel;
			Gtk::VBox layoutBox;
			Gtk::HBox timerLayout;
			Gtk::ComboBox dateDropDown;
			Glib::RefPtr<Gtk::ListStore> comboModel;
			//timeView displayTimes;
	};

	

	/*class timeView : public Gtk::TreeView {
		public:
			timeView() { set }
			~timeView() { }
		private:
			Gtk::ListStore timeList;
			std::vector< std::vector<time_t> > times;
	};
	*/
}
